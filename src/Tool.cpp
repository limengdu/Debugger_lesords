#include "Tool.h"
#include "Global.h"

#include <Arduino.h>
#include <Wire.h>

#ifdef EVT3_BOARD
struct EEPROM_VALUE_T {
    double A[8]; // level 1
    double B[6]; // level 2
    double C[4]; // level 3
} g_value;
#else
struct EEPROM_VALUE_T {
    double value[13];
} g_value;
#endif

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

#ifdef EVT3_BOARD
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
    } else if (x >= 0.000001) {
        return -0.00045 + g_value.A[7];         // 超低电流补偿
    } else {
        return 0;
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
    } else if (0.0085 <= x && x < 0.01) {
        return calFormula(x, 0.00012, 0.025, -3000, 5000000) + g_value.B[3];    // 低电流区
    } else if (0.007 <= x && x < 0.0085) {
        return calFormula(x, 0.00012, 0.025, -3000, 5000000) + g_value.B[4];    // 低电流区
    } else if (x < 0.007) {
        return calFormula(x, -0.00182, 0.142) + g_value.B[5];                   // 超低电流区
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
#endif

// U: V
// return: mA
double calCompensationByShuntVol(double U) {
    double I = 0.0;

    if      (U > 0           && U <= 9.86e-05)     I = -4.96717885515552e-05 + U * (-1.15024973780837    ) + U * U * ( 7.50380233713587e+03) + g_value.value[0];
    else if (U > 9.86e-05    && U <= 0.002075875)  I = -1.17572123826341e-04 + U * (-3.76425950965523e-01) + U * U * (-2.64636863429469e+01) + g_value.value[1];
    else if (U > 0.002075875 && U <= 0.00213061)   I =  4.00173135933271e-01 + U * (-1.93091389761448e+02) + U * U * (-8.12502147858271e-01) + g_value.value[2];
    else if (U > 0.00213061  && U <= 0.005489525)  I =  2.80464450771294e+01 + U * (-1.06774687954604e+04) + U * U * ( 1.01433693037010e+06) + g_value.value[3];
    else if (U > 0.005489525 && U <= 0.00795575)   I =  4.15192674006609e-01 + U * (-1.37494120471743e+02) + U * U * ( 1.11074715236611e+04) + g_value.value[4];
    else if (U > 0.00795575  && U <= 0.010019594)  I =  4.73398883967983e+01 + U * (-9.45635027923620e+03) + U * U * ( 4.71917518500573e+05) + g_value.value[5];
    else if (U > 0.010019594 && U <= 0.021018875)  I = -4.94687661493529e+00 + U * ( 7.42737702877842e+02) + U * U * (-2.48740191915014e+04) + g_value.value[6];
    else if (U > 0.021018875 && U <= 0.02638275)   I = -7.42724335142089e+02 + U * ( 6.33783048995403e+04) + U * U * (-1.33432949251484e+06) + g_value.value[7];
    else if (U > 0.02638275  && U <= 0.047608391)  I =  5.94080582648901e+01 + U * (-2.80934246583276e+03) + U * U * ( 3.34248352232072e+04) + g_value.value[8];
    else if (U > 0.047608391 && U <= 0.076459006)  I = -2.69169784448099e+01 + U * ( 8.67709858761791e+02) + U * U * (-6.70079569468977e+03) + g_value.value[9];
    else if (U > 0.076459006 && U <= 0.101737855)  I =  9.49616304079839e+01 + U * (-2.16873837281881e+03) + U * U * ( 1.22241630543939e+04) + g_value.value[10];
    else if (U > 0.101737855 && U <= 0.104738533)  I =  2.25813965130917e+03 + U * (-4.64186589084307e+04) + U * U * ( 2.37333030351984e+05) + g_value.value[11];
    else if (U > 0.104738533 && U <= 0.105482)     I =  2.25813965130917e+03 + U * (-4.64186589084307e+04) + U * U * ( 2.37333030351984e+05) + g_value.value[12];

    return I;
}
