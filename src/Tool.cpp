#include "Tool.h"
#include "Global.h"

#include <Arduino.h>

double A = 0;
double B = 0;
double C = 0;
double D = 0;
double E = 0;
double F = 0;

double calFormula(double x, double a = 0, double b = 0, double c = 0, double d = 0) {
    return (a) + (x) * (b) + (x) * (x) * (c) + (x) * (x) * (x) * (d);
}

double getCompensationCurrentLevel1(double x) {
    if (x >= 0.0005) {
        return -0.00095 + A;        // A 影响高电流区
    } else if (x >= 0.0002) {
        return -0.00110 + 0.5 * A;  // A 部分影响
    } else if (x >= 0.0001) {
        return -0.00120 + B;        // B 主要影响区
    } else if (x >= 0.00007) {
        return -0.00070 + 0.5 * B;  // B 部分影响
    } else if (x >= 0.00005) {
        return -0.00060;
    } else if (x >= 0.00003) {
        return -0.00075 + A;        // A 影响低电流区
    } else if (x >= 0.00002) {
        return 0.30 * x - 0.00012;
    } else {
        return -0.00045 + A;        // A 影响超低电流区
    }

    return 0;
}

double getCompensationCurrentLevel2(double x) {
    if (x >= 0.03) {
        return calFormula(x, 0.0007, 0.085, -550) + D;                  // D 影响高电流区
    } else if (0.02 <= x && x < 0.03) {
        return calFormula(x, -0.00055, 0.055) + 0.5 * D;                // D 部分影响
    } else if (0.01 <= x && x < 0.02) {
        return calFormula(x, -0.00025, 0.035) + C;                      // C 主要影响区
    } else if (0.007 <= x && x < 0.01) {
        return calFormula(x, 0.00012, 0.025, -3000, 5000000) + 0.5 * C; // C 部分影响
    } else if (x < 0.007) {
        return calFormula(x, -0.00182, 0.142) + C;                      // C 影响低电流区
    }

    return 0;
}

double getCompensationCurrentLevel3(double x) {
    if (x < 0.025) {
        return calFormula(x, 0.65, -58, 1800, -18500);      // E 影响低电流区
    } else if (0.025 <= x && x < 0.043) {
        return calFormula(x, 1.85, -150, 3800, -32000);     // E 主要影响区
    } else if (0.043 <= x && x < 0.066) {
        return calFormula(x, -4.6, 350, -8500, 68000);      // F 影响中电流区
    } else if (x >= 0.066) {
        return calFormula(x, 10.2, -575, 13500, -105000);   // F影响高电流区
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
