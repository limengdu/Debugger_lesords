# Debugger

Here is the Debugger source code repository

## Dependencies

Libraries that must be downloaded

- INA: https://github.com/adafruit/Adafruit_INA228

- DAPLink: https://github.com/Lesords/Seeed_Arduino_DAPLink/tree/feat/ESP32S3

- TFT: https://github.com/Lesords/TFT_eSPI/tree/feat/ST7789-EVT

- LVGL: https://github.com/lvgl/lvgl

- TinyUSB: https://github.com/adafruit/Adafruit_TinyUSB_Arduino

## Configuration items

The following configuration needs to be modified to the corresponding options

- `Partition Scheme` : "Maximum APP (7.9MB APP No OTA/No FS)"

- `PSRAM` : "OPI PSRAM"

- `USB Mode` : "USB-OTG (TinyUSB)"
