#include "Tool.h"
#include "Global.h"

#include <Arduino.h>

double calFormula(double x, double a = 0, double b = 0, double c = 0, double d = 0) {
    return (a) + (x) * (b) + (x) * (x) * (c) + (x) * (x) * (x) * (d);
}

double getCompensationCurrentLevel1(double x) {
    if (x >= 0.0005) {
        return -0.00118;            // 高电压稳定区
    } else if (x >= 0.0002) {
        return -0.00125;            // 高电压过渡区
    } else if (x >= 0.0001) {
        return -0.00132;            // 高电压增强区
    } else if (x >= 0.00007) {
        return -0.00085;            // 中电压高段
    } else if (x >= 0.00005) {
        return -0.00075;            // 中电压低段
    } else if (x >= 0.00003) {
        return -0.00090;            // 低电压高段
    } else if (x >= 0.00002) {
        return 0.25 * x - 0.00015;  // 低电压线性区
    } else {
        return -0.00060;            // 超低电压区
    }

    return 0;
}

double getCompensationCurrentLevel2(double x) {
    if (x >= 0.03) {
        return calFormula(x, -0.00182, 0.142);
    } else if (0.02 <= x && x < 0.03) {
        return calFormula(x, 0.0005, 0.08, -500);
    } else if (0.01 <= x && x < 0.02) {
        return calFormula(x, -0.00055, 0.055);
    } else if (0.007 <= x && x < 0.01) {
        return calFormula(x, -0.00025, 0.035);
    } else if (x < 0.007) {
        return calFormula(x, 0.00012, 0.025, -3000, 5000000);
    }

    return 0;
}

double getCompensationCurrentLevel3(double x) {
    if (x < 0.025) {
        return calFormula(x, 0.65, -58, 1800, -18500);
    } else if (0.025 <= x && x < 0.043) {
        return calFormula(x, 1.85, -150, 3800, -32000);
    } else if (0.043 <= x && x < 0.066) {
        return calFormula(x, -4.8, 350, -8500, 68000);
    } else if (x >= 0.066) {
        return calFormula(x, 9.8, -575, 13500, -105000);
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
