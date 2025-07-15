#include "Tool.h"
#include "Global.h"

#include <Arduino.h>
#include <Wire.h>

struct EEPROM_VALUE_T {
  double A[8]; // level 1
  double B[5]; // level 2
  double C[4]; // level 3
} g_value;

void initValueFromEEPROM() {
    Wire1.setPins(EEPROM_I2C_SDA, EEPROM_I2C_SCL);
    Wire1.setClock(EEPROM_I2C_FREQUENCY);
    Wire1.begin();

    // 读取 EEPROM
    byte *ptr = (byte *) &g_value;
    int structSize = sizeof(EEPROM_VALUE_T);

    for (int i = 0; i < structSize; i++) {
        Wire1.beginTransmission(EEPROM_I2C_ADDR);
        Wire1.write(i);
        Wire1.endTransmission();

        Wire1.requestFrom(EEPROM_I2C_ADDR, 1);
        if (Wire1.available()) {
            *(ptr + i) = Wire1.read();
        }
    }
}

double calFormula(double x, double a = 0, double b = 0, double c = 0, double d = 0) {
    return (a) + (x) * (b) + (x) * (x) * (c) + (x) * (x) * (x) * (d);
}

double getCompensationCurrentLevel1(double x) {
    if (x >= 0.0005) {
        return -0.00095 + g_value.A[0];         // 高电流区补偿
    } else if (x >= 0.0002) {
        return -0.00110 + g_value.A[1];         // 中高电流区补偿
    } else if (x >= 0.0001) {
        return -0.00120 + g_value.A[2];         // 中电流区补偿
    } else if (x >= 0.00007) {
        return -0.00070 + g_value.A[3];         // 中低电流区补偿
    } else if (x >= 0.00005) {
        return -0.00060 + g_value.A[4];         // 低电流区补偿
    } else if (x >= 0.00003) {
        return -0.00075 + g_value.A[5];         // 超低电流区补偿
    } else if (x >= 0.00002) {
        return 0.30 * x - 0.00012 + g_value.A[6];
    } else {
        return -0.00045 + g_value.A[7];         // 超低电流补偿
    }

    return 0;
}

double getCompensationCurrentLevel2(double x) {
    if (x >= 0.03) {
        return calFormula(x, 0.0007, 0.085, -550) + g_value.B[0];               // 高电流区
    } else if (0.02 <= x && x < 0.03) {
        return calFormula(x, -0.00055, 0.055) + g_value.B[1];                   // 中高电流区
    } else if (0.01 <= x && x < 0.02) {
        return calFormula(x, -0.00025, 0.035) + g_value.B[2];                   // 中电流区
    } else if (0.007 <= x && x < 0.01) {
        return calFormula(x, 0.00012, 0.025, -3000, 5000000) + g_value.B[3];    // 低电流区
    } else if (x < 0.007) {
        return calFormula(x, -0.00182, 0.142) + g_value.B[4];                   // 超低电流区
    }

    return 0;
}

double getCompensationCurrentLevel3(double x) {
    if (x < 0.025) {
        return calFormula(x, 0.65, -58, 1800, -18500) + g_value.C[0];      // 低电流区
    } else if (0.025 <= x && x < 0.043) {
        return calFormula(x, 1.85, -150, 3800, -32000) + g_value.C[1];     // 中低电流区
    } else if (0.043 <= x && x < 0.066) {
        return calFormula(x, -4.6, 350, -8500, 68000) + g_value.C[2];      // 中电流区
    } else if (x >= 0.066) {
        return calFormula(x, 10.2, -575, 13500, -105000) + g_value.C[3];   // 高电流区
    }

    return 0;
}

// x: shunt voltage (V)
double getCompensationCurrent(double x) {
    int mos1 = 0, mos2 = 0;

    mos1 = digitalRead(MOS1_PIN);
    mos2 = digitalRead(MOS2_PIN);
    if (mos1 == 0 && mos2 == 0) {
        return getCompensationCurrentLevel1(x);
    } else if(mos1 == 1 && mos2 == 0) {
        return getCompensationCurrentLevel2(x);
    } else if(mos1 == 1 && mos2 == 1) {
        return getCompensationCurrentLevel3(x);
    }
}

// x: current (mA)
// y: temperatue ℃ 
// return current (A)
double getCompensationOfTemp(double x, double y) {
    double result = (y - 25) * x;

    if (x < 50) {
        return result * 0.0005;
    } else if (50 <= x && x < 300) {
        return result * 0.0012;
    } else if (x >= 300) {
        return result * 0.0018;
    }

    return result;
}
