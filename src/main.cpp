#include "immapp/immapp.h"
#include "implot/implot.h"
#include "implot/implot_internal.h"
#include "binance_client.h"
#include <iostream>
#include <vector>
#include "DataFrame/DataFrame.h"
#include "DataFrame/DataFrameStatsVisitors.h"

namespace MyImPlot {
   int BinarySearch(const double* arr, int l, int r, double x) {
       if (r >= l) {
           int mid = l + (r - l) / 2;
           if (arr[mid] == x)
               return mid;
           if (arr[mid] > x)
               return BinarySearch(arr, l, mid - 1, x);
           return BinarySearch(arr, mid + 1, r, x);
       }
       return -1;
   }

   void PlotCandlestick(const char* label_id, const double* xs, const double* opens, const double* closes, const double* lows, const double* highs, int count, bool tooltip = true, float width_percent = 0.25f, ImVec4 bullCol = ImVec4(0,1,0,1), ImVec4 bearCol = ImVec4(1,0,0,1)) {
       // begin plot item
       if (ImPlot::BeginItem(label_id)) {
           // override legend icon color
           ImPlot::GetCurrentItem()->Color = ImGui::ColorConvertFloat4ToU32(ImVec4(1,1,1,1));
           // fit data if requested
           if (ImPlot::FitThisFrame()) {
               for (int i = 0; i < count; ++i) {
                   ImPlot::FitPoint(ImPlotPoint(xs[i], lows[i]));
                   ImPlot::FitPoint(ImPlotPoint(xs[i], highs[i]));
               }
           }
           // get ImGui window DrawList
           ImDrawList* draw_list = ImPlot::GetPlotDrawList();
           // calc real value width
           double half_width = count > 1 ? (xs[1] - xs[0]) * width_percent : width_percent;
           // render data
           for (int i = 0; i < count; ++i) {
               ImVec2 open_pos = ImPlot::PlotToPixels(xs[i] - half_width, opens[i]);
               ImVec2 close_pos = ImPlot::PlotToPixels(xs[i] + half_width, closes[i]);
               ImVec2 low_pos = ImPlot::PlotToPixels(xs[i], lows[i]);
               ImVec2 high_pos = ImPlot::PlotToPixels(xs[i], highs[i]);
               ImU32 color = ImGui::GetColorU32(opens[i] > closes[i] ? bearCol : bullCol);
               draw_list->AddLine(low_pos, high_pos, color);
               draw_list->AddRectFilled(open_pos, close_pos, color);
           }
           // custom tool
           if (ImPlot::IsPlotHovered() && tooltip) {
               ImPlotPoint mouse = ImPlot::GetPlotMousePos();
               mouse.x = round(mouse.x);
               float tool_l = ImPlot::PlotToPixels(mouse.x - half_width * 1.5, mouse.y).x;
               float tool_r = ImPlot::PlotToPixels(mouse.x + half_width * 1.5, mouse.y).x;
               float tool_t = ImPlot::GetPlotPos().y;
               float tool_b = tool_t + ImPlot::GetPlotSize().y;
               draw_list->AddRectFilled(ImVec2(tool_l, tool_t), ImVec2(tool_r, tool_b), IM_COL32(0,255,255,64));
               // find mouse location index
               int idx = BinarySearch(xs, 0, count - 1, mouse.x);
               // render tool tip (won't be affected by plot clip rect)
               if (idx != -1) {
                   ImGui::BeginTooltip();
                   ImGui::Text("Time: %.0f", xs[idx]);
                   ImGui::Text("Open: $%.2f", opens[idx]);
                   ImGui::Text("Close: $%.2f", closes[idx]);
                   ImGui::Text("Low: $%.2f", lows[idx]);
                   ImGui::Text("High: $%.2f", highs[idx]);
                   ImGui::EndTooltip();
               }
           }
           // end plot item
           ImPlot::EndItem();
       }
   }
} // namespace MyImPlot


int main(int, char **)
{
    BinanceClient client;
    std::vector<Candlestick> candlesticks;

    HelloImGui::SimpleRunnerParams runnerParams;
    runnerParams.guiFunction = [&]() {
        if (ImGui::Button("Fetch BTC/USDT")) {
            candlesticks = client.getCandlestickData("BTCUSDT", "1m", 100); // Fetch more data for a better plot
        }

        if (!candlesticks.empty()) {
            std::vector<double> open_times, opens, highs, lows, closes;
            for (const auto& c : candlesticks) {
                open_times.push_back(static_cast<double>(c.openTime / 1000)); // ImPlot expects seconds
                opens.push_back(c.open);
                highs.push_back(c.high);
                lows.push_back(c.low);
                closes.push_back(c.close);
            }

            using namespace hmdf;
            using DoubleVec = std::vector<double>;
            StdDataFrame<double> df;
            df.load_index(std::move(open_times));
            df.load_column("opens", std::move(opens));
            df.load_column("highs", std::move(highs));
            df.load_column("lows", std::move(lows));
            df.load_column("closes", std::move(closes));

            ewm_v<double, unsigned long> ema9_visitor(exponential_decay_spec::span, 9);
            const auto      &ema9_values = df.single_act_visit<double>("closes", ema9_visitor).get_result();

            ewm_v<double, unsigned long> ema21_visitor(exponential_decay_spec::span, 21);
            const auto      &ema21_values = df.single_act_visit<double>("closes", ema21_visitor).get_result();

            if (ImPlot::BeginPlot("Candlestick Chart", "Time", "Price")) {
                const auto& df_opens = df.get_column<double>("opens");
                const auto& df_closes = df.get_column<double>("closes");
                const auto& df_lows = df.get_column<double>("lows");
                const auto& df_highs = df.get_column<double>("highs");

                MyImPlot::PlotCandlestick("BTC/USDT", df.get_index().data(), df_opens.data(), df_closes.data(), df_lows.data(), df_highs.data(), candlesticks.size());
                ImPlot::PlotLine("EMA9", df.get_index().data(), ema9_values.data(), candlesticks.size());
                ImPlot::PlotLine("EMA21", df.get_index().data(), ema21_values.data(), candlesticks.size());
                ImPlot::EndPlot();
            }
        } else {
            ImGui::Text("No data yet. Click 'Fetch BTC/USDT' to load data.");
        }
    };
    runnerParams.windowSize = {1200, 800};
    runnerParams.windowTitle = "Candlestick App";
    ImmApp::AddOnsParams addOnsParams;
    addOnsParams.withImplot = true;
    ImmApp::Run(runnerParams, addOnsParams);
    return 0;
}
