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
    uint m_baudRateList[9] = {4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600};
    uint8_t m_currentBaudIndex;
    lv_coord_t m_screenWidth;
    lv_coord_t m_screenHeight;
    bool m_posFlag;
};

#endif //FUNCTIONBAUDSTATE_H
