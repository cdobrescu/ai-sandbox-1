# Copilot Instructions for Candlestick App

This project is a C++ desktop application for visualizing cryptocurrency candlestick data from Binance, with technical analysis overlays and an interactive GUI. Follow these guidelines to be productive as an AI coding agent in this codebase.

## Architecture Overview
- **Main Components:**
  - `src/binance_client.cpp` / `.h`: Implements the Binance API client and defines the `Candlestick` struct.
  - `src/main.cpp`: Contains the application entry point, GUI logic, and chart rendering using ImGui/ImPlot.
- **Data Flow:**
  - The GUI triggers data fetch via `BinanceClient`.
  - Fetched OHLCV data is processed into a DataFrame for analysis and plotting.
  - EMA9 and EMA21 are calculated and visualized alongside candlesticks.
- **External Dependencies:** (You should be familiar with these libraries)
  - [ImGui](https://github.com/ocornut/imgui) for GUI
  - [ImPlot](https://github.com/epezent/implot) for plotting
  - [imgui_bundle](https://github.com/pthom/imgui_bundle) for easy ImGui setup
  - [DataFrame](https://github.com/hosseinmoein/DataFrame) for data manipulation
  - [cpr](https://github.com/libcpr/cpr) for HTTP requests
  - [nlohmann/json](https://github.com/nlohmann/json) for JSON parsing
  
## Developer Workflows
- **Build:**
  - Use CMake from the project root or `build/` directory:
    ```bash
    mkdir build && cd build
    cmake ..
    cmake --build .
    ```
- **Run:**
  - Launch the app from `build/`:
    ```bash
    ./candlestick_app
    ```
- **Configuration:**
  - App settings in `Candlestick_App.ini` (in root and build/)
- **Debugging:**
  - Main logic is in `src/main.cpp`.
  - Binance API and data handling in `src/binance_client.cpp`.

## Project-Specific Patterns
- **GUI:**
  - ImGui/ImPlot used for all charting and UI; follow their idioms for widgets and plotting.
- **Data Handling:**
  - Use DataFrame for time series and technical analysis.
- **API Integration:**
  - Binance API requests are handled via custom client in `binance_client.cpp`.
- **Technical Analysis:**
  - EMA9 and EMA21 overlays are standard; see main.cpp for calculation pattern.

## Conventions
- **Source files:** All main logic in `src/`.
- **Build artifacts:** All output in `build/`.
- **Config files:** `.ini` files for app settings.
- **No tests or CI/CD scripts present.**

## Key References
- `README.md`: Build, run, and architecture summary
- `src/binance_client.cpp` / `.h`: API and data model
- `src/main.cpp`: GUI and chart logic
- `Candlestick_App.ini`: App configuration

---

If any section is unclear or missing important patterns, please provide feedback to improve these instructions.