#include "binance_client.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "DataFrame/DataFrame.h"

std::vector<Candlestick> BinanceClient::getCandlestickData(const std::string& symbol, const std::string& interval, int limit) {
    cpr::Response r = cpr::Get(cpr::Url{"https://api.binance.com/api/v3/klines"},
                               cpr::Parameters{{"symbol", symbol},
                                               {"interval", interval},
                                               {"limit", std::to_string(limit)}});

    if (r.status_code != 200) {
        // Handle error
        return {};
    }

    auto json_data = nlohmann::json::parse(r.text);
    std::vector<Candlestick> candlesticks;

    for (const auto& item : json_data) {
        Candlestick c;
        c.openTime = item[0].get<long long>();
        c.open = std::stod(item[1].get<std::string>());
        c.high = std::stod(item[2].get<std::string>());
        c.low = std::stod(item[3].get<std::string>());
        c.close = std::stod(item[4].get<std::string>());
        c.volume = std::stod(item[5].get<std::string>());
        c.closeTime = item[6].get<long long>();
        c.quoteAssetVolume = std::stod(item[7].get<std::string>());
        c.numberOfTrades = item[8].get<int>();
        c.takerBuyBaseAssetVolume = std::stod(item[9].get<std::string>());
        c.takerBuyQuoteAssetVolume = std::stod(item[10].get<std::string>());
        candlesticks.push_back(c);
    }

    return candlesticks;
}

hmdf::StdDataFrame<double> BinanceClient::createDataFrame(const std::vector<Candlestick>& candlesticks) {
    using namespace hmdf;
    StdDataFrame<double> df;
    std::vector<double> open_times, opens, highs, lows, closes, volumes, close_times, quote_asset_volumes, taker_buy_base_volumes, taker_buy_quote_volumes;
    std::vector<int> number_of_trades;
    for (const auto& c : candlesticks) {
        open_times.push_back(static_cast<double>(c.openTime));
        opens.push_back(c.open);
        highs.push_back(c.high);
        lows.push_back(c.low);
        closes.push_back(c.close);
        volumes.push_back(c.volume);
        close_times.push_back(static_cast<double>(c.closeTime));
        quote_asset_volumes.push_back(c.quoteAssetVolume);
        number_of_trades.push_back(c.numberOfTrades);
        taker_buy_base_volumes.push_back(c.takerBuyBaseAssetVolume);
        taker_buy_quote_volumes.push_back(c.takerBuyQuoteAssetVolume);
    }
    df.load_index(std::move(open_times));
    df.load_column("opens", std::move(opens));
    df.load_column("highs", std::move(highs));
    df.load_column("lows", std::move(lows));
    df.load_column("closes", std::move(closes));
    df.load_column("volumes", std::move(volumes));
    df.load_column("close_times", std::move(close_times));
    df.load_column("quote_asset_volumes", std::move(quote_asset_volumes));
    df.load_column("number_of_trades", std::move(number_of_trades));
    df.load_column("taker_buy_base_volumes", std::move(taker_buy_base_volumes));
    df.load_column("taker_buy_quote_volumes", std::move(taker_buy_quote_volumes));
    return df;
}

hmdf::StdDataFrame<double> BinanceClient::getCandlestickDataAsDataFrame(const std::string& symbol, const std::string& interval, int limit) {
    using namespace hmdf;
    cpr::Response r = cpr::Get(cpr::Url{"https://api.binance.com/api/v3/klines"},
                               cpr::Parameters{{"symbol", symbol},
                                               {"interval", interval},
                                               {"limit", std::to_string(limit)}});
    if (r.status_code != 200) {
        // Handle error
        return {};
    }
    auto json_data = nlohmann::json::parse(r.text);
    StdDataFrame<double> df;
    std::vector<double> open_times, opens, highs, lows, closes, volumes, close_times, quote_asset_volumes, taker_buy_base_volumes, taker_buy_quote_volumes;
    std::vector<int> number_of_trades;
    for (const auto& item : json_data) {
        // Use nlohmann::json helpers for direct conversion
        open_times.push_back(item.at(0).get<double>());
        opens.push_back(std::stod(item.at(1).get<std::string>()));
        highs.push_back(std::stod(item.at(2).get<std::string>()));
        lows.push_back(std::stod(item.at(3).get<std::string>()));
        closes.push_back(std::stod(item.at(4).get<std::string>()));
        volumes.push_back(std::stod(item.at(5).get<std::string>()));
        close_times.push_back(item.at(6).get<double>());
        quote_asset_volumes.push_back(std::stod(item.at(7).get<std::string>()));
        number_of_trades.push_back(item.at(8).get<int>());
        taker_buy_base_volumes.push_back(std::stod(item.at(9).get<std::string>()));
        taker_buy_quote_volumes.push_back(std::stod(item.at(10).get<std::string>()));
    }
    df.load_index(std::move(open_times));
    df.load_column("opens", std::move(opens));
    df.load_column("highs", std::move(highs));
    df.load_column("lows", std::move(lows));
    df.load_column("closes", std::move(closes));
    df.load_column("volumes", std::move(volumes));
    df.load_column("close_times", std::move(close_times));
    df.load_column("quote_asset_volumes", std::move(quote_asset_volumes));
    df.load_column("number_of_trades", std::move(number_of_trades));
    df.load_column("taker_buy_base_volumes", std::move(taker_buy_base_volumes));
    df.load_column("taker_buy_quote_volumes", std::move(taker_buy_quote_volumes));
    return df;
}
