cmake_minimum_required(VERSION 3.16)
project(CandlestickApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

Set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(
  imgui_bundle
  GIT_REPOSITORY https://github.com/pthom/imgui_bundle.git
  GIT_TAG v1.92.0
)

FetchContent_MakeAvailable(imgui_bundle)

FetchContent_Declare(
  cpr
  GIT_REPOSITORY https://github.com/libcpr/cpr.git
  GIT_TAG 1.10.0
)
FetchContent_MakeAvailable(cpr)


# add_executable(candlestick_app src/main.cpp)

# target_link_libraries(candlestick_app PRIVATE imgui_bundle::imgui_bundle)
imgui_bundle_add_app(candlestick_app src/main.cpp src/binance_client.cpp)
target_link_libraries(candlestick_app PRIVATE cpr::cpr)
