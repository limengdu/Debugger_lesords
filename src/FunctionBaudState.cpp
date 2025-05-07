#include "FunctionBaudState.h"

FunctionBaudState::FunctionBaudState()
: FunctionState("FunctionBaudState")
, m_baudStateUI()
, m_currentBaudIndex(1)
{

}
void FunctionBaudState::onEnter()
{
    if (m_baudStateUI.Screen != nullptr) {
        if ( lv_scr_act() != m_baudStateUI.Screen) {
            lv_scr_load(m_baudStateUI.Screen);
        }
        return;
    }
    m_baudStateUI.Screen = lv_obj_create(NULL);
    lv_obj_add_style(m_baudStateUI.Screen, &style_screen, 0);

    m_baudStateUI.Line = lv_line_create(m_baudStateUI.Screen);
    // 获取屏幕的高度和宽度
    lv_coord_t screen_width = lv_disp_get_hor_res(NULL);
    lv_coord_t screen_height = lv_disp_get_ver_res(NULL);
    // 计算水平居中时线的 y 坐标
    lv_coord_t center_y = screen_height / 2;
    // 定义线的两个端点
    static lv_point_precise_t line_points[] = {
        {0, center_y},
        {screen_width, center_y}
    };
    // 设置线的端点
    lv_line_set_points(m_baudStateUI.Line, line_points, 2);
    // 设置线的样式
    lv_obj_set_style_line_color(m_baudStateUI.Line, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_line_width(m_baudStateUI.Line, 2, 0);

    m_baudStateUI.currentBaudLabel = lv_label_create(m_baudStateUI.Screen);
    m_baudStateUI.nextBaudLabel = lv_label_create(m_baudStateUI.Screen);
    m_baudStateUI.previousBaudLabel = lv_label_create(m_baudStateUI.Screen);

    char buf[16];
    sprintf(buf, "%d", m_baudRateList[m_currentBaudIndex]);
    lv_label_set_text(m_baudStateUI.currentBaudLabel, buf);
    lv_obj_set_pos(m_baudStateUI.currentBaudLabel, (screen_width/2 - 20), (screen_height/2-30));
    lv_obj_set_style_text_color(m_baudStateUI.currentBaudLabel, lv_color_hex(0xDDE62F), LV_PART_MAIN);
    lv_obj_add_style(m_baudStateUI.currentBaudLabel, &style_font_22, 0);

    uint8_t prevBaudIndex = m_currentBaudIndex - 1;
    if(prevBaudIndex < 0){
        m_currentBaudIndex = sizeof(m_baudRateList) / sizeof(m_baudRateList[0]) ;
    }
    sprintf(buf, "%d", m_baudRateList[prevBaudIndex]);
    lv_label_set_text(m_baudStateUI.previousBaudLabel, buf);
    lv_obj_set_pos(m_baudStateUI.previousBaudLabel, 20, (screen_height/2+10));
    lv_obj_set_style_text_color(m_baudStateUI.previousBaudLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_style(m_baudStateUI.previousBaudLabel, &style_font_22, 0);

    uint8_t nextBaudIndex = m_currentBaudIndex + 1;
    if(nextBaudIndex >= sizeof(m_baudRateList) / sizeof(m_baudRateList[0])){
        nextBaudIndex = 0;
    }
    sprintf(buf, "%d", m_baudRateList[nextBaudIndex]);
    lv_label_set_text(m_baudStateUI.nextBaudLabel, buf);
    lv_obj_set_pos(m_baudStateUI.nextBaudLabel, (screen_width - 100), (screen_height/2+10));
    lv_obj_set_style_text_color(m_baudStateUI.nextBaudLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_style(m_baudStateUI.nextBaudLabel, &style_font_22, 0);

    lv_scr_load(m_baudStateUI.Screen);
}

void FunctionBaudState::onExit()
{

}

bool FunctionBaudState::handleEvent(StateMachine* machine, const Event* event)
{
    if (!machine || !event) {
        return false;
    }

    switch (event->getType()) {
        case EVENT_WHEEL_CLOCKWISE: {
            return false;
        }

        case EVENT_WHEEL_COUNTERCLOCKWISE: {
            return false;
        }

        case EVENT_BUTTON_PRESS: {
            // 按钮按下，进入选中的功能
            const ButtonEvent* buttonEvent = static_cast<const ButtonEvent*>(event);
            if (buttonEvent->getButtonId() == 0) {
                increaceBaudRate();// 测试波特率增加没有
                int stateId = FunctionPowerState::ID;
                State* nextState = StateManager::getInstance()->getState(stateId);
                if (nextState) {
                    machine->changeState(nextState);
                    return true;
                }
            }
        }

        default:
            return false;
    }
}

void FunctionBaudState::updateDisplay(DisplayContext* display)
{
    if (!display) {
        return;
    }

    char buf[16];
    sprintf(buf, "%d", m_baudRateList[m_currentBaudIndex]);
    lv_label_set_text(m_baudStateUI.currentBaudLabel, buf);

    // Correctly calculate the previous baud index
    uint8_t prevBaudIndex = (m_currentBaudIndex == 0) ? (sizeof(m_baudRateList) / sizeof(m_baudRateList[0]) - 1) : m_currentBaudIndex - 1;
    sprintf(buf, "%d", m_baudRateList[prevBaudIndex]);
    lv_label_set_text(m_baudStateUI.previousBaudLabel, buf);

    // Correctly calculate the next baud index
    uint8_t nextBaudIndex = (m_currentBaudIndex == (sizeof(m_baudRateList) / sizeof(m_baudRateList[0]) - 1)) ? 0 : m_currentBaudIndex + 1;
    sprintf(buf, "%d", m_baudRateList[nextBaudIndex]);
    lv_label_set_text(m_baudStateUI.nextBaudLabel, buf);
}

void FunctionBaudState::increaceBaudRate()
{
    uint8_t nextBaudIndex = (m_currentBaudIndex == (sizeof(m_baudRateList) / sizeof(m_baudRateList[0]) - 1)) ? 0 : m_currentBaudIndex + 1;
    m_currentBaudIndex = nextBaudIndex;
    m_baudRate = m_baudRateList[m_currentBaudIndex];

    //具体修改串口波特率，这里只是修改界面
}

void FunctionBaudState::decreaceBaudRate()
{
    uint8_t prevBaudIndex = (m_currentBaudIndex == 0) ? (sizeof(m_baudRateList) / sizeof(m_baudRateList[0]) - 1) : m_currentBaudIndex - 1;
    m_currentBaudIndex = prevBaudIndex;
    m_baudRate = m_baudRateList[m_currentBaudIndex];

    //具体修改串口波特率，这里只是修改界面
}

int FunctionBaudState::getID() const
{
    return ID;
}

const char* FunctionBaudState::getName() const
{
    return "FunctionBaudState";
}

uint FunctionBaudState::m_baudRate = BAUD_9600;
