#ifndef GLOBAL_H
#define GLOBAL_H

#define BOOT_BTN 0

#define INA_I2C_SDA 4
#define INA_I2C_SCL 3
#define INA_I2C_ADDR 0x43
#define INA_I2C_FREQUENCY 100000

/*Set to your screen resolution and rotation*/
#define TFT_HOR_RES   240
#define TFT_VER_RES   320
#define TFT_ROTATION  LV_DISPLAY_ROTATION_270

#define DEBOUNCE_DELAT 50       // Debounce delay, in milliseconds
#define LONG_PRESS_DELAY 1000   // Long press delay, in milliseconds

#define DELETE_LABEL(label)   \
if (label != nullptr) {   \
lv_obj_del(label);     \
label = nullptr;       \
}

extern unsigned long startTime;

#endif
