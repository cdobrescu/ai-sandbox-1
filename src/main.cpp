#include "immapp/immapp.h"
#include "binance_client.h"
#include <iostream>
#include <vector>

int main(int, char **)
{
    BinanceClient client;
    std::vector<Candlestick> candlesticks;

    HelloImGui::SimpleRunnerParams runnerParams;
    runnerParams.guiFunction = [&]() {
        if (ImGui::Button("Fetch BTC/USDT")) {
            candlesticks = client.getCandlestickData("BTCUSDT", "1m", 10);
            for(const auto& c : candlesticks) {
                std::cout << "Open time: " << c.openTime << ", Close price: " << c.close << std::endl;
            }
        }

        if (!candlesticks.empty()) {
            ImGui::Text("Last close price: %f", candlesticks.back().close);
        } else {
            ImGui::Text("No data yet.");
        }
    };
    runnerParams.windowSize = {300, 100};
    runnerParams.windowTitle = "Candlestick App";
    ImmApp::Run(runnerParams);
    return 0;
}
