cdobrescu: I want to  cache the data retrieved from the api  on disk in such a way that is easy to load into a dataframe and also it is easy to fill in missing gaps. Design this change keeping in mind that I want to support multiple pairs, multiple intervals 
I want to keep the cache files to a reasonable size thus, files for the same pair and interval will be split like this basedon interval size:
- for intervals lower than 1h (e.g. 10m, 5m, 1m), split the file by days (include date in file name)
- for intervals lower than 1 day but higher or equal to 1h, split the file by months
- for intervals higher or equal to 1d, split the files by year



GitHub Copilot: To cache candlestick data efficiently for multiple pairs and intervals, and make it easy to load into a DataFrame and fill gaps, here’s a recommended design:

---

## **Cache File Structure**
We may support multiple exchanges in the future so exchange name must be part of the cache path.

- **Directory Layout:**  
  ```
  cache/
    └── binance/
        └── <pair>/
            └── <interval>/
                └── <start_timestamp>.csv
        └── meta.csv        
  ```
  Example:  
  ```
  cache/binance/BTCUSDT/1m/20250901.csv
  cache/binance/BTCUSDT/1m/20250902.csv
  cache/binance/ETHUSDT/5m/20250901.csv
  cache/binance/ETHUSDT/5m/20250902.csv
  cache/binance/ETHUSDT/1h/20240901.csv
  cache/binance/ETHUSDT/1h/20241001.csv
  cache/binance/ETHUSDT/1d/20230101.csv
  cache/binance/ETHUSDT/1d/20240101.csv
  ```

- **File Format:**  
  Use **CSV** for easy DataFrame loading and manual inspection.  
  Each row:  
  ```
  openTime,open,high,low,close,volume
  1630000000000,43000.0,43100.0,42900.0,43050.0,12.34
  ```
- **meta.csv**
  This file keeps track of the cache status. It will contain record only for the cach files that are complete.
  A completed file is a file wich contains all the required records.
  E.g. for `cache/ETHUSDT/5m/20250902.csv` which correspond to a 5 minute interval:
    The file should have the 5 minute candlesticks for the entire day of 2025-09-02 (YYYY-MM-DD) which is be a total of 24 (hours)  times 12 (5  minute interval in an hour) candlesticks.

  Each row will contain the filename and md5sum:
  ```cache/binance/meta.csv
  filename,md5sum
  BTCUSDT/1m/20250901.csv,51cb61b0eec0f99398e29a8d16f7558f
  ```
---

## **Cache Logic**



- **On API Fetch:**
  1. **Load existing cache** for the pair/interval.
  2. **Merge new data**:  
     - Overwrite rows with matching `openTime`.
     - Append new rows.
     - Sort by `openTime`.
  3. **Fill gaps**:  
     - Detect missing intervals (based on expected time step).
     - Optionally fill with placeholder rows (e.g., NaN or zero volume).
  4. **Limit file size**:  
     - Keep only the last N rows (e.g., 10,000).
     - Truncate older data if necessary.

- **On Data Load:**
  - Read CSV into DataFrame.
  - Handle missing data as needed (interpolate, drop, etc.).

---

## **API Changes**

- **BinanceClient**:  
  - Add methods for cache read/write.
  - On `getCandlestickData`, check cache before fetching from API.

---

---

## **Benefits**

- **Easy DataFrame Loading:** CSV is natively supported.
- **Gap Filling:** Detect and fill missing intervals during merge.
- **Multi-pair/interval:** Directory structure keeps files organized.
- **Size Control:** Truncate old data to keep cache manageable.
- **Reduce API calls** No API calls for cached data

---

