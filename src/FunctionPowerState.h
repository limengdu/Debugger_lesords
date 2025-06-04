#ifndef FUNCTIONPOWERSTATE_H
#define FUNCTIONPOWERSTATE_H

#include "Global.h"
#include "LvglStyle.h"
#include "MenuStates.h"
#include "FunctionUartState.h"

#define POWER_INTERFACE_1  0
#define POWER_INTERFACE_2  1
#define POWER_INTERFACE_3  2

struct POWER_STATE_SIMPLE_T {
    lv_obj_t* voltage;
    lv_obj_t* current;
    lv_obj_t* power;
};

struct POWER_STATE_MEDIUM_T {
    lv_obj_t* voltage;
    lv_obj_t* current_A;
    lv_obj_t* current_mA;
    lv_obj_t* current_uA;
    lv_obj_t* power_W;
    lv_obj_t* power_mW;
};

struct POWER_STATE_COMPLEX_T {
    lv_obj_t* voltage;
    lv_obj_t* current;
    lv_obj_t* power;

    lv_obj_t* viewGroup;
    lv_obj_t* minCurrent_A;
    lv_obj_t* minPower_Wh;
    lv_obj_t* maxCurrent_A;
    lv_obj_t* maxPower_Wh;
    lv_obj_t* totalCurrent_Ah;
    lv_obj_t* totalPower_Wh;

    lv_obj_t* time;
};

struct POWER_STATE_UI_T {
    lv_obj_t* Screen;
    lv_obj_t* powerGroup[3];

    lv_obj_t *voltageLabel_V;
    lv_obj_t *currentLabel_A;
    lv_obj_t *currentLabel_mA;
    lv_obj_t *currentLabel_uA;
    lv_obj_t *powerLabel_W;
    lv_obj_t *powerLabel_mW;
    lv_obj_t *minLabel;
    lv_obj_t *minLabel_A;
    lv_obj_t *minLabel_wh;
    lv_obj_t *maxLabel;
    lv_obj_t *maxLabel_A;
    lv_obj_t *maxLabel_wh;
    lv_obj_t *totalLabel;
    lv_obj_t *totalLabel_wh;
    lv_obj_t *totalLabel_Ah;
    lv_obj_t *timeLabel;

    POWER_STATE_SIMPLE_T powerSimple;
    POWER_STATE_MEDIUM_T powerMedium;
    POWER_STATE_COMPLEX_T powerComplex;
};

//! todo 具体检测电流电压这些值
// 特定功能状态
class FunctionPowerState : public FunctionState {
public:
    enum { ID = 4 };
    // 定义函数指针类型
    typedef void (FunctionPowerState::*InterfaceFunction)();

    FunctionPowerState();
    void onEnter() override;
    void onExit() override;

    // 特定实现
    virtual void updateDisplay(DisplayContext* display);
    int getID() const override;
    const char* getName() const override;
    bool handleEvent(StateMachine* machine, const Event* event) override;

    void createPowerSimpleUI();
    void createPowerMediumUI();
    void createPowerComplexUI();

    void compute(float vol,float cur,float pow);

private:
    // lv_obj_t *m_titleLabel;
    POWER_STATE_UI_T m_powerStateUI;
    bool m_isFirstFlag;
    // 100ms 转换为小时，因为 updateDisplay() 每隔 100ms 刷新一次
    static constexpr float m_timeInterval = 1.0f / 36000.0f;
    //电压
    float m_voltage;
    //电流
    float m_current;
    float m_minCurrent;
    float m_maxCurrent;
    float m_totalCurrent;
    //功率
    float m_power;
    float m_minPower;
    float m_maxPower;
    float m_totalPower;

    // 界面数组
    int m_currentIndex;
    int m_update;
    unsigned long startTime;
};

#endif //FUNCTIONPOWERSTATE_H
