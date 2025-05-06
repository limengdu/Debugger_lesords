//
// Created by Administrator on 25-4-29.
//

#ifndef FUNCTIONPOWERSTATE_H
#define FUNCTIONPOWERSTATE_H

#include "Global.h"
#include "LvglStyle.h"
#include "MenuStates.h"

struct PowerStateUI {
    lv_obj_t* Screen;

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

    void powerInterface_1();
    void powerInterface_2();
    void powerInterface_3();

    void clearScreen();


private:
    // lv_obj_t *m_titleLabel;
    PowerStateUI m_powerStateUI;
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
    InterfaceFunction interfaceFunctions[3];
    int currentInterfaceIndex;
};


#endif //FUNCTIONPOWERSTATE_H
