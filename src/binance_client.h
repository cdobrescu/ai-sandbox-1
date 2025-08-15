#pragma once

#include <string>
#include <vector>

struct Candlestick {
    long long openTime;
    double open;
    double high;
    double low;
    double close;
    double volume;
    long long closeTime;
    double quoteAssetVolume;
    int numberOfTrades;
    double takerBuyBaseAssetVolume;
    double takerBuyQuoteAssetVolume;
};

class BinanceClient {
public:
    std::vector<Candlestick> getCandlestickData(const std::string& symbol, const std::string& interval, int limit);
};
