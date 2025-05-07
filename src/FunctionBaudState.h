#ifndef FUNCTIONBAUDSTATE_H
#define FUNCTIONBAUDSTATE_H

#include "MenuStates.h"
#include "Global.h"
#include "LvglStyle.h"
#include "StateManager.h"
#include "FunctionPowerState.h"

struct BaudStateUI {
    lv_obj_t* Screen;
    lv_obj_t* Line;
    lv_obj_t* currentBaudLabel;
    lv_obj_t* previousBaudLabel;
    lv_obj_t* nextBaudLabel;
};

// 特定功能状态
class FunctionBaudState : public FunctionState {
public:
    enum { ID = 3 };

    FunctionBaudState();

    // 特定实现
    virtual void updateDisplay(DisplayContext* display);
    int getID() const override;
    const char* getName() const override;
    void onEnter() override;
    void onExit() override;
    bool handleEvent(StateMachine* machine, const Event* event) override;

    void increaceBaudIndex();
    void decreaceBaudIndex();
    void updateBaudRate();

public:
    static uint m_baudRate;
private:
    BaudStateUI m_baudStateUI;
    //设置10个元素，有一个是0
    //如果只有9个元素，当前 index为0，前一个index为8，那么久判断后就在同一侧了
    uint m_baudRateList[10] = {4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600,0};
    uint8_t m_currentBaudIndex;
    lv_coord_t m_screenWidth;
    lv_coord_t m_screenHeight;
};

#endif //FUNCTIONBAUDSTATE_H
