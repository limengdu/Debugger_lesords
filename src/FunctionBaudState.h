#ifndef FUNCTIONBAUDSTATE_H
#define FUNCTIONBAUDSTATE_H

#include "MenuStates.h"
#include "Global.h"
#include "LvglStyle.h"
#include "StateManager.h"
#include "FunctionPowerState.h"

#define BAUD_4800 4800
#define BAUD_9600 9600
#define BAUD_19200 19200
#define BAUD_38400 38400
#define BAUD_57600 57600
#define BAUD_115200 115200
#define BAUD_230400 230400
#define BAUD_460800 460800
#define BAUD_921600 921600

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

    void increaceBaudRate();
    void decreaceBaudRate();

public:
    static uint m_baudRate;
private:
    BaudStateUI m_baudStateUI;
    uint m_baudRateList[9] = {BAUD_4800, BAUD_9600, BAUD_19200, BAUD_38400, BAUD_57600, BAUD_115200, BAUD_230400, BAUD_460800, BAUD_921600};
    uint8_t m_currentBaudIndex;
};

#endif //FUNCTIONBAUDSTATE_H
