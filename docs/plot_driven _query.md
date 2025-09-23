# ImPLot Driven queries


## OHLCV State

The implot starts with an initial state:
- PAIR
- INTERVAL
- VisibleRange - How many candle sticks are visible
- Last Visible Bar - Defaulting to current unclosed candle

The visible range can change by zooming in the plot.
The last visible bar can be changed by horizontap panning the plot


## Pixel perfect rendering

We always want the bars aligned to the pixels. In order to keep this we want to have discreet zooming that result in increasing the width of a candle bar and the space between them


The maxium zoomout is 1 pixel candle and 0 pixels in between

The maximum zoom in is 11 pixels candle 3 pixels in between


Taking into account the implot viewpoert pixel width, ZOOM level we cand calculate the VIsible range

Note: We may add graphical annotations these should change proportional with zooming and stay aligned with the candlesticks


## Infinite panning

Even If we don't have data from tomorrow or next week or month, we should be able to pan to the right

When current cnadled is not visible anymore show a button with a left or right arrow that whill pann until the candle is in the viewport


## Implot style
Check tradingView

Current Axis are too big
Ticker prices show be dinamically andm inimally listed  the right price axis