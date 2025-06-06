#include "Global.h"
#include "MenuStates.h"
#include "FunctionUartState.h"

FunctionUartState::FunctionUartState()
    : FunctionState("FunctionUartState"),
      m_isUartInfoDisplay(false),
      m_currentSelection(0),
      m_uartType(UartType::UART_TYPE_XIAO),
      m_uartStateUI(),
      m_uartTask(nullptr)
{
    xTaskCreate(
        uartTaskFunc,
        "UartTask",
        20480,
        this,
        2,
        &m_uartTask
    );

    vTaskSuspend(m_uartTask);
}

void FunctionUartState::uartTaskFunc(void* params) {

    while (true) {
        // RX, From XIAO to Debugger
        if (COMSerial.available()) {
            char c = COMSerial.read();
            sprintf(FunctionUartState::rxBuff, "%c", c);
            ShowSerial.write(c);
        }

        // TX, From PC to XIAO
        if (ShowSerial.available()) {
            char c = ShowSerial.read();
            COMSerial.write(c);
        }

        vTaskDelay(pdMS_TO_TICKS(30));
    }
}

void FunctionUartState::createOptionBarUI() {
    // UART Type
    m_uartStateUI.UartTypeBg = lv_obj_create(m_uartStateUI.Screen);
    lv_obj_set_size(m_uartStateUI.UartTypeBg, 130, 42);
    lv_obj_set_pos(m_uartStateUI.UartTypeBg, 12 + 12, 15);
    lv_obj_add_style(m_uartStateUI.UartTypeBg, &style_nofocus_uart_bg, 0);

    lv_obj_t* label = lv_label_create(m_uartStateUI.UartTypeBg);
    lv_obj_set_pos(label, 22, 10);
    lv_label_set_text(label, "UART:");
    lv_obj_set_style_text_color(label, lv_color_hex(0xA6ACAF), LV_PART_MAIN);
    lv_obj_add_style(label, &style_font_14, 0);

    m_uartStateUI.UartTypeLabel = lv_label_create(m_uartStateUI.UartTypeBg);
    lv_label_set_text_fmt(m_uartStateUI.UartTypeLabel, "%s", (m_uartType == UartType::UART_TYPE_XIAO)? "XIAO" : "Grove");
    lv_obj_set_pos(m_uartStateUI.UartTypeLabel, 12 + 12 + 50, 10);
    lv_obj_set_style_text_color(m_uartStateUI.UartTypeLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_style(m_uartStateUI.UartTypeLabel, &style_font_14, 0);

    // BaudRate
    m_uartStateUI.UartBaudBg = lv_obj_create(m_uartStateUI.Screen);
    lv_obj_set_size(m_uartStateUI.UartBaudBg, 130, 42);
    lv_obj_set_pos(m_uartStateUI.UartBaudBg, 154 + 12, 15);
    lv_obj_add_style(m_uartStateUI.UartBaudBg, &style_nofocus_uart_bg, 0);

    m_uartStateUI.UartBaudLabel = lv_label_create(m_uartStateUI.UartBaudBg);
    lv_label_set_text_fmt(m_uartStateUI.UartBaudLabel, "%d", FunctionBaudState::m_baudRate);
    lv_obj_align(m_uartStateUI.UartBaudLabel, LV_ALIGN_TOP_MID, 0, 5);
    lv_obj_set_style_text_color(m_uartStateUI.UartBaudLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_style(m_uartStateUI.UartBaudLabel, &style_font_14, 0);

    static lv_point_precise_t p[] = {{0, 5}, {45, 5}, {45, 0}, {45, 5}, {90, 5}};
    m_uartStateUI.UartBaudLine = lv_line_create(m_uartStateUI.UartBaudBg);
    lv_obj_add_style(m_uartStateUI.UartBaudLine, &style_uart_line, 0);
    lv_line_set_points(m_uartStateUI.UartBaudLine, p, 5);
    lv_obj_align(m_uartStateUI.UartBaudLine, LV_ALIGN_TOP_MID, 0, 22);
}

void FunctionUartState::createLedUI() {
    // RX LED Group
    m_uartStateUI.UartRxLEDGroup = lv_obj_create(m_uartStateUI.Screen);
    lv_obj_set_size(m_uartStateUI.UartRxLEDGroup, 194, 37);
    lv_obj_set_pos(m_uartStateUI.UartRxLEDGroup, 12 + 51, 88);
    lv_obj_add_style(m_uartStateUI.UartRxLEDGroup, &style_screen, 0);

    lv_obj_t* label = lv_label_create(m_uartStateUI.UartRxLEDGroup);
    lv_label_set_text(label, "RX");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xDDE62), LV_PART_MAIN);
    lv_obj_add_style(label, &style_font_28, 0);

    // TX LED Group
    m_uartStateUI.UartTxLEDGroup = lv_obj_create(m_uartStateUI.Screen);
    lv_obj_set_size(m_uartStateUI.UartTxLEDGroup, 194, 37);
    lv_obj_set_pos(m_uartStateUI.UartTxLEDGroup, 12 + 51, 155);
    lv_obj_add_style(m_uartStateUI.UartTxLEDGroup, &style_screen, 0);

    label = lv_label_create(m_uartStateUI.UartTxLEDGroup);
    lv_label_set_text(label, "TX");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x2FE6AC), LV_PART_MAIN);
    lv_obj_add_style(label, &style_font_28, 0);

    // LED List
    // Left Part, start from the middle to the left
    int xPos = 60;
    for (int i = 0; i < 5; i++) {
        // TX
        m_uartStateUI.UartRxLedLeftList[i] = lv_led_create(m_uartStateUI.UartRxLEDGroup);
        lv_obj_set_pos(m_uartStateUI.UartRxLedLeftList[i], xPos, 14 + i);
        lv_led_off(m_uartStateUI.UartRxLedLeftList[i]);
        lv_led_set_color(m_uartStateUI.UartRxLedLeftList[i], lv_color_hex(0xDDE62F));
        lv_obj_set_size(m_uartStateUI.UartRxLedLeftList[i], 10 - (i * 2), 10 - (i * 2));
        lv_obj_add_style(m_uartStateUI.UartRxLedLeftList[i], &style_led, 0);

        // RX
        m_uartStateUI.UartTxLedLeftList[i] = lv_led_create(m_uartStateUI.UartTxLEDGroup);
        lv_obj_set_pos(m_uartStateUI.UartTxLedLeftList[i], xPos, 14 + i);
        lv_led_off(m_uartStateUI.UartTxLedLeftList[i]);
        lv_led_set_color(m_uartStateUI.UartTxLedLeftList[i], lv_color_hex(0x2FE6AC));
        lv_obj_set_size(m_uartStateUI.UartTxLedLeftList[i], 10 - (i * 2), 10 - (i * 2));
        lv_obj_add_style(m_uartStateUI.UartTxLedLeftList[i], &style_led, 0);

        xPos -= 18 - (i * 2);
    }

    // Right Part, start from the middle to the right
    xPos = 124;
    for (int i = 0; i < 5; i++) {
        // TX
        m_uartStateUI.UartRxLedRightList[i] = lv_led_create(m_uartStateUI.UartRxLEDGroup);
        lv_obj_set_pos(m_uartStateUI.UartRxLedRightList[i], xPos, 14 + i);
        lv_led_off(m_uartStateUI.UartRxLedRightList[i]);
        lv_led_set_color(m_uartStateUI.UartRxLedRightList[i], lv_color_hex(0xDDE62F));
        lv_obj_set_size(m_uartStateUI.UartRxLedRightList[i], 10 - (i * 2), 10 - (i * 2));
        lv_obj_add_style(m_uartStateUI.UartRxLedRightList[i], &style_led, 0);

        // RX
        m_uartStateUI.UartTxLedRightList[i] = lv_led_create(m_uartStateUI.UartTxLEDGroup);
        lv_obj_set_pos(m_uartStateUI.UartTxLedRightList[i], xPos, 14 + i);
        lv_led_off(m_uartStateUI.UartTxLedRightList[i]);
        lv_led_set_color(m_uartStateUI.UartTxLedRightList[i], lv_color_hex(0x2FE6AC));
        lv_obj_set_size(m_uartStateUI.UartTxLedRightList[i], 10 - (i * 2), 10 - (i * 2));
        lv_obj_add_style(m_uartStateUI.UartTxLedRightList[i], &style_led, 0);

        xPos += 18 - (i * 2);
    }
}

void FunctionUartState::createMessageUI() {
    // RX Group
    m_uartStateUI.UartRxGroup = lv_obj_create(m_uartStateUI.Screen);
    lv_obj_set_size(m_uartStateUI.UartRxGroup, 272, 77);
    lv_obj_set_pos(m_uartStateUI.UartRxGroup, 12 + 12, 64);
    lv_obj_add_style(m_uartStateUI.UartRxGroup, &style_screen, 0);

    // RX
    lv_obj_t* label = lv_label_create(m_uartStateUI.UartRxGroup);
    lv_label_set_text(label, "RX");
    lv_obj_set_pos(label, 0, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_style(label, &style_font_18, 0);

    // RX LED
    m_uartStateUI.UartRxLed = lv_led_create(m_uartStateUI.UartRxGroup);
    lv_obj_set_pos(m_uartStateUI.UartRxLed, 8, 28);
    lv_led_on(m_uartStateUI.UartRxLed);
    lv_led_set_color(m_uartStateUI.UartRxLed, lv_color_hex(0xDDE62F));
    lv_obj_add_style(m_uartStateUI.UartRxLed, &style_led, 0);

    // RX Area
    m_uartStateUI.UartRxBg= lv_obj_create(m_uartStateUI.UartRxGroup);
    lv_obj_set_size(m_uartStateUI.UartRxBg, 245, 77);
    lv_obj_set_pos(m_uartStateUI.UartRxBg, 27, 0);
    lv_obj_add_style(m_uartStateUI.UartRxBg, &style_nofocus_bg, 0);

    m_uartStateUI.UartRxTextArea = lv_textarea_create(m_uartStateUI.UartRxGroup);
    lv_obj_set_size(m_uartStateUI.UartRxTextArea, 215, 59);
    lv_obj_set_pos(m_uartStateUI.UartRxTextArea, 42, 9);
    lv_textarea_set_one_line(m_uartStateUI.UartRxTextArea, false);
    lv_obj_set_style_text_color(m_uartStateUI.UartRxTextArea, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_border_width(m_uartStateUI.UartRxTextArea, 0, LV_PART_MAIN);
    lv_obj_add_style(m_uartStateUI.UartRxTextArea, &style_font_14, 0);
    lv_obj_add_style(m_uartStateUI.UartRxTextArea, &style_nofocus_bg, 0);

    // TX Group
    m_uartStateUI.UartTxGroup = lv_obj_create(m_uartStateUI.Screen);
    lv_obj_set_size(m_uartStateUI.UartTxGroup, 272, 77);
    lv_obj_set_pos(m_uartStateUI.UartTxGroup, 12 + 12, 151);
    lv_obj_add_style(m_uartStateUI.UartTxGroup, &style_screen, 0);

    // TX
    label = lv_label_create(m_uartStateUI.UartTxGroup);
    lv_label_set_text(label, "TX");
    lv_obj_set_pos(label, 0, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_style(label, &style_font_18, 0);

    // TX LED
    m_uartStateUI.UartTxLed = lv_led_create(m_uartStateUI.UartTxGroup);
    lv_obj_set_pos(m_uartStateUI.UartTxLed, 8, 28);
    lv_led_on(m_uartStateUI.UartTxLed);
    lv_led_set_color(m_uartStateUI.UartTxLed, lv_color_hex(0x2FE6AC));
    lv_obj_add_style(m_uartStateUI.UartTxLed, &style_led, 0);

    // TX Area
    m_uartStateUI.UartTxBg = lv_obj_create(m_uartStateUI.UartTxGroup);
    lv_obj_set_size(m_uartStateUI.UartTxBg, 245, 77);
    lv_obj_set_pos(m_uartStateUI.UartTxBg, 27, 0);
    lv_obj_add_style(m_uartStateUI.UartTxBg, &style_nofocus_bg, 0);

    m_uartStateUI.UartTxTextArea = lv_textarea_create(m_uartStateUI.UartTxGroup);
    lv_obj_set_size(m_uartStateUI.UartTxTextArea, 215, 59);
    lv_obj_set_pos(m_uartStateUI.UartTxTextArea, 42, 9);
    lv_textarea_set_one_line(m_uartStateUI.UartTxTextArea, false);
    lv_obj_set_style_text_color(m_uartStateUI.UartTxTextArea, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_border_width(m_uartStateUI.UartTxTextArea, 0, LV_PART_MAIN);
    lv_obj_add_style(m_uartStateUI.UartTxTextArea, &style_font_14, 0);
    lv_obj_add_style(m_uartStateUI.UartTxTextArea, &style_nofocus_bg, 0);
}

void FunctionUartState::onEnter()
{
    m_currentSelection = -1;

    if (m_uartStateUI.Screen != nullptr) {
        if (lv_scr_act() != m_uartStateUI.Screen) {
            lv_scr_load(m_uartStateUI.Screen);
            vTaskResume(m_uartTask);
        }
        return;
    }

    m_uartStateUI.Screen = lv_obj_create(NULL);
    lv_obj_add_style(m_uartStateUI.Screen, &style_screen, 0);

    createOptionBarUI();
    createLedUI();
    createMessageUI();

    lv_obj_add_flag(m_uartStateUI.UartRxGroup, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(m_uartStateUI.UartTxGroup, LV_OBJ_FLAG_HIDDEN);

    lv_scr_load(m_uartStateUI.Screen);

    vTaskResume(m_uartTask);
}

void FunctionUartState::onExit()
{
    vTaskSuspend(m_uartTask);
}

bool FunctionUartState::handleEvent(StateMachine* machine, const Event* event)
{
    if (!machine || !event) {
        return false;
    }

    switch (event->getType()) {
        case EVENT_WHEEL_CLOCKWISE:
        case EVENT_WHEEL_COUNTERCLOCKWISE: {
            if (m_currentSelection < 0) {
                m_isUartInfoDisplay = !m_isUartInfoDisplay;
            } else {
                m_currentSelection = !m_currentSelection;
            }
            break;
        }

        case EVENT_BUTTON_PRESS: {
            if (m_currentSelection < 0) {
                m_currentSelection = 0;
                break;
            }

            const ButtonEvent* buttonEvent = static_cast<const ButtonEvent*>(event);
            if (buttonEvent->getButtonId() == BOOT_BTN) {
                if (m_currentSelection == -1) {
                    m_currentSelection = 0;
                } else if (m_currentSelection == 0) {
                    changeUartType();
                    break;
                } else if (m_currentSelection == 1) {
                    int stateId = FunctionBaudState::ID;
                    State* nextState = StateManager::getInstance()->getState(stateId);
                    if (nextState) {
                        machine->changeState(nextState);
                        break;
                    }
                }
            }
            return false;
        }

        case EVENT_BUTTON_LONGPRESS: {
            if (m_currentSelection >= 0) {
                m_currentSelection = -1;
                break;
            }

            const ButtonEvent* buttonEvent = static_cast<const ButtonEvent*>(event);
            if (buttonEvent->getButtonId() == BOOT_BTN) {
                int stateId = MainMenuState::ID;
                State* nextState = StateManager::getInstance()->getState(stateId);
                if (nextState) {
                    machine->changeState(nextState);
                    break;
                }
            }
            return false;
        }

        default:
            return false;
    }

    return true;
}

void FunctionUartState::updateLedEffect() {
    static int rxPos = 4, txPos = 0;
    static int time = millis();
    int lastPos = 0;

    // RX, 4 -> 0, Light up from both sides to the middle
    if (COMSerial.available()) {
        if (millis() - time >= 1000) {
            lv_led_on(m_uartStateUI.UartRxLedLeftList[rxPos]);
            lv_led_on(m_uartStateUI.UartRxLedRightList[rxPos]);

            lastPos = (rxPos <= 3) ? (rxPos + 1) : 0;
            lv_led_off(m_uartStateUI.UartRxLedLeftList[lastPos]);
            lv_led_off(m_uartStateUI.UartRxLedRightList[lastPos]);

            if (--rxPos < 0) rxPos = 4;
        }
    } else {
        lastPos = (rxPos <= 3) ? (rxPos + 1) : 0;
        lv_led_off(m_uartStateUI.UartRxLedLeftList[lastPos]);
        lv_led_off(m_uartStateUI.UartRxLedRightList[lastPos]);
        rxPos = 0;
    }

    // TX, 0 -> 4, Light up from the middle to both sides
    if (ShowSerial.available()) {
        if (millis() - time >= 1000) {
            lv_led_on(m_uartStateUI.UartTxLedLeftList[txPos]);
            lv_led_on(m_uartStateUI.UartTxLedRightList[txPos]);

            lastPos = (txPos >= 1) ? (txPos - 1) : 4;
            lv_led_off(m_uartStateUI.UartTxLedLeftList[lastPos]);
            lv_led_off(m_uartStateUI.UartTxLedRightList[lastPos]);

            if (++txPos > 4) txPos = 0;
        }
    } else {
        lastPos = (txPos >= 1) ? (txPos - 1) : 4;
        lv_led_off(m_uartStateUI.UartTxLedLeftList[lastPos]);
        lv_led_off(m_uartStateUI.UartTxLedRightList[lastPos]);
    }
}

void FunctionUartState::updateDisplay(DisplayContext* display) {
    if (!display) {
        return;
    }

    lv_label_set_text_fmt(m_uartStateUI.UartTypeLabel, "%s", (m_uartType == UartType::UART_TYPE_XIAO) ? "XIAO" : "Grove");
    lv_label_set_text_fmt(m_uartStateUI.UartBaudLabel, "%d", FunctionBaudState::m_baudRate);

    if (m_isUartInfoDisplay) {
        lv_obj_add_flag(m_uartStateUI.UartRxLEDGroup, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(m_uartStateUI.UartTxLEDGroup, LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_flag(m_uartStateUI.UartRxGroup, LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_flag(m_uartStateUI.UartTxGroup, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(m_uartStateUI.UartRxGroup, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(m_uartStateUI.UartTxGroup, LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_flag(m_uartStateUI.UartRxLEDGroup, LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_flag(m_uartStateUI.UartTxLEDGroup, LV_OBJ_FLAG_HIDDEN);
    }

    if (m_currentSelection == -1) {
        lv_obj_add_style(m_uartStateUI.UartTypeBg, &style_nofocus_uart_bg, 0);
        lv_obj_set_style_text_color(m_uartStateUI.UartTypeLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

        lv_obj_add_style(m_uartStateUI.UartBaudBg, &style_nofocus_uart_bg, 0);
        lv_obj_add_style(m_uartStateUI.UartBaudLine, &style_uart_line, 0);
        lv_obj_set_style_text_color(m_uartStateUI.UartBaudLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    } else if (m_currentSelection == 0) {
        lv_obj_add_style(m_uartStateUI.UartTypeBg, &style_focus_uart_bg, 0);
        lv_obj_set_style_text_color(m_uartStateUI.UartTypeLabel, lv_color_hex(0xACE62F), LV_PART_MAIN);

        lv_obj_add_style(m_uartStateUI.UartBaudBg, &style_nofocus_uart_bg, 0);
        lv_obj_add_style(m_uartStateUI.UartBaudLine, &style_uart_line, 0);
        lv_obj_set_style_text_color(m_uartStateUI.UartBaudLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    } else if (m_currentSelection == 1) {
        lv_obj_add_style(m_uartStateUI.UartTypeBg, &style_nofocus_uart_bg, 0);
    lv_obj_set_style_text_color(m_uartStateUI.UartTypeLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

        lv_obj_add_style(m_uartStateUI.UartBaudBg, &style_focus_uart_bg, 0);
        lv_obj_add_style(m_uartStateUI.UartBaudLine, &style_focus_uart_line, 0);
        lv_obj_set_style_text_color(m_uartStateUI.UartBaudLabel, lv_color_hex(0xACE62F), LV_PART_MAIN);
    }

    if (!m_isUartInfoDisplay) {
        updateLedEffect();
        return;
    }

    // 获取当前时间戳（毫秒）
    unsigned long timestamp = millis();
    // 将时间戳格式化为字符串并更新 rxBuff 和 txBuff
    char rxText[128];
    snprintf(rxText, sizeof(rxText), "RX: %lu\n", timestamp);  // 模拟接收数据，添加换行符

    // 获取当前 UartRxTextArea 的文本内容
    const char* currentRxText = lv_textarea_get_text(m_uartStateUI.UartRxTextArea);
    // 定义一个足够大的缓冲区来拼接新数据
    char combinedRxText[1024];
    snprintf(combinedRxText, sizeof(combinedRxText), "%s%s", currentRxText, rxText);
    // 定义最大显示长度，超过该长度则截断前面的内容
    const int MAX_DISPLAY_LENGTH = 90;
    if (strlen(combinedRxText) > MAX_DISPLAY_LENGTH) {
        memmove(combinedRxText, combinedRxText + strlen(combinedRxText) - MAX_DISPLAY_LENGTH, MAX_DISPLAY_LENGTH + 1);
    }
    // 更新 UartRxTextArea 的文本内容
    lv_textarea_set_text(m_uartStateUI.UartRxTextArea, combinedRxText);

    // 将 UartRxTextArea 滚动到末尾，确保显示最新数据
    lv_obj_scroll_to_y(m_uartStateUI.UartRxTextArea, LV_COORD_MAX, LV_ANIM_ON);

    lv_textarea_set_text(m_uartStateUI.UartTxTextArea, txBuff);
}

int FunctionUartState::getID() const
{
    return ID;
}

const char* FunctionUartState::getName() const
{
    return "FunctionUartState";
}

void FunctionUartState::changeUartType()
{
    if(m_uartType == UartType::UART_TYPE_XIAO){
        m_uartType = UartType::UART_TYPE_Grove;
        digitalWrite(UART_SWITCH, HIGH);
    }else{
        m_uartType = UartType::UART_TYPE_XIAO;
        digitalWrite(UART_SWITCH, LOW);
    }
}

char FunctionUartState::rxBuff[UART_DATA_SIZE]="";
char FunctionUartState::txBuff[UART_DATA_SIZE]="";
