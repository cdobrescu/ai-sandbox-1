#include "binance_client.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

std::vector<Candlestick> BinanceClient::getCandlestickData(const std::string& symbol, const std::string& interval, int limit) {
    cpr::Response r = cpr::Get(cpr::Url{"https://fapi.binance.com/fapi/v1/klines"},
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
