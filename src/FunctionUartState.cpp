#include "FunctionUartState.h"

FunctionUartState::FunctionUartState()
    : FunctionState("FunctionUartState"),
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
}

void FunctionUartState::uartTaskFunc(void* params) {

    while (true) {
        if (COMSerial.available()) {
            char c = COMSerial.read();
            sprintf(FunctionUartState::rxBuff, "%c", c);
            ShowSerial.write(c);
        }

        if (ShowSerial.available()) {
            char c = ShowSerial.read();
            COMSerial.write(c);
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void FunctionUartState::onEnter()
{
    if (m_uartStateUI.Screen != nullptr) {
        if ( lv_scr_act() != m_uartStateUI.Screen) {
            lv_scr_load(m_uartStateUI.Screen);
        }
        return;
    }
    m_uartStateUI.Screen = lv_obj_create(NULL);
    lv_obj_add_style(m_uartStateUI.Screen, &style_screen, 0);

    // 创建UI标签
    m_uartStateUI.UartTypeBg = lv_obj_create(m_uartStateUI.Screen);
    m_uartStateUI.UartBaudBg = lv_obj_create(m_uartStateUI.Screen);
    m_uartStateUI.UartRxBg= lv_obj_create(m_uartStateUI.Screen);
    m_uartStateUI.UartTxBg = lv_obj_create(m_uartStateUI.Screen);
    m_uartStateUI.UartTypeLabel = lv_label_create(m_uartStateUI.Screen);
    m_uartStateUI.UartBaudLabel = lv_label_create(m_uartStateUI.Screen);
    // Create a textarea for rx and tx display
    m_uartStateUI.UartRxTextArea = lv_textarea_create(m_uartStateUI.UartRxBg);
    m_uartStateUI.UartTxTextArea = lv_textarea_create(m_uartStateUI.UartTxBg);

    // 画背景块
    lv_obj_add_style(m_uartStateUI.Screen, &style_screen, 0);

    lv_obj_set_size(m_uartStateUI.UartTypeBg, 100, 40);
    lv_obj_set_pos(m_uartStateUI.UartTypeBg, 20, 20);
    lv_obj_add_style(m_uartStateUI.UartTypeBg, &style_focus_bg, 0);

    lv_obj_set_size(m_uartStateUI.UartBaudBg, 100, 40);
    lv_obj_set_pos(m_uartStateUI.UartBaudBg, 20+160, 20);
    lv_obj_add_style(m_uartStateUI.UartBaudBg, &style_nofocus_bg, 0);

    lv_obj_set_size(m_uartStateUI.UartRxBg, 230, 70);
    lv_obj_set_pos(m_uartStateUI.UartRxBg, 70, 70);
    lv_obj_add_style(m_uartStateUI.UartRxBg, &style_rxtx_bg, 0);

    lv_obj_set_size(m_uartStateUI.UartTxBg, 230, 70);
    lv_obj_set_pos(m_uartStateUI.UartTxBg, 70, 70+80);
    lv_obj_add_style(m_uartStateUI.UartTxBg, &style_rxtx_bg, 0);
    // 画元素
    // UART
    lv_obj_t* label = lv_label_create(m_uartStateUI.Screen);
    lv_obj_set_pos(label, 25, 30);
    lv_label_set_text(label, "UART:");
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_style(label, &style_font_12, 0);
    lv_label_set_text_fmt(m_uartStateUI.UartTypeLabel, "%s", (m_uartType == UartType::UART_TYPE_XIAO)? "XIAO" : "Grove");
    lv_obj_set_pos(m_uartStateUI.UartTypeLabel,65,30);
    lv_obj_set_style_text_color(m_uartStateUI.UartTypeLabel, lv_color_hex(0x2FE6AC), LV_PART_MAIN);
    lv_obj_add_style(m_uartStateUI.UartTypeLabel, &style_font_18, 0);

    // 波特率
    lv_label_set_text_fmt(m_uartStateUI.UartBaudLabel, "%d", FunctionBaudState::m_baudRate);
    lv_obj_set_pos(m_uartStateUI.UartBaudLabel, 20+160+20, 30);
    lv_obj_set_style_text_color(m_uartStateUI.UartBaudLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_style(m_uartStateUI.UartBaudLabel, &style_font_22, 0);

    // RX
    label = lv_label_create(m_uartStateUI.Screen);
    lv_label_set_text(label, "RX");
    lv_obj_set_pos(label, 20, 75);
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_style(label, &style_font_18, 0);
    // RX LED
    lv_obj_t* led_RX = lv_led_create(m_uartStateUI.Screen);
    lv_obj_set_pos(led_RX, 20+30, 75+5);
    lv_led_on(led_RX);
    lv_led_set_color(led_RX, lv_color_hex(0xDDE62F));
    lv_obj_add_style(led_RX, &style_led, 0);

    // TX
    label = lv_label_create(m_uartStateUI.Screen);
    lv_label_set_text(label, "TX");
    lv_obj_set_pos(label, 20, 75+80);
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_style(label, &style_font_18, 0);
    // TX LED
    lv_obj_t* led_TX = lv_led_create(m_uartStateUI.Screen);
    lv_obj_set_pos(led_TX, 20+30, 75+80+5);
    lv_led_on(led_TX);
    lv_led_set_color(led_TX, lv_color_hex(0x2FE6AC));
    lv_obj_add_style(led_TX, &style_led, 0);
    // 滚屏框
    lv_obj_set_size(m_uartStateUI.UartRxTextArea  , 200, 60);
    lv_obj_set_size(m_uartStateUI.UartTxTextArea  , 200, 60);
    lv_obj_set_pos(m_uartStateUI.UartRxTextArea, 5, 0);
    lv_obj_set_pos(m_uartStateUI.UartTxTextArea, 5, 0);
    lv_textarea_set_one_line(m_uartStateUI.UartRxTextArea, false);
    lv_textarea_set_one_line(m_uartStateUI.UartTxTextArea, false);
    lv_obj_set_style_text_color(m_uartStateUI.UartRxTextArea, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_color(m_uartStateUI.UartTxTextArea, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_style(m_uartStateUI.UartRxTextArea, &style_font_14, 0);
    lv_obj_add_style(m_uartStateUI.UartTxTextArea, &style_font_14, 0);
    lv_obj_add_style(m_uartStateUI.UartRxTextArea, &style_nofocus_bg, 0);
    lv_obj_add_style(m_uartStateUI.UartTxTextArea, &style_nofocus_bg, 0);
    // 去掉 UartRxTextArea 的边框
    lv_obj_set_style_border_width(m_uartStateUI.UartRxTextArea, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(m_uartStateUI.UartTxTextArea, 0, LV_PART_MAIN);
    lv_scr_load(m_uartStateUI.Screen);
}

void FunctionUartState::onExit()
{

}

bool FunctionUartState::handleEvent(StateMachine* machine, const Event* event)
{
    if (!machine || !event) {
        return false;
    }

    switch (event->getType()) {
        case EVENT_WHEEL_CLOCKWISE: {
            // 滚轮顺时针，选择下一项
            return false;
        }

        case EVENT_WHEEL_COUNTERCLOCKWISE: {
            //滚轮逆时针，选择上一项
            return false;
        }

        case EVENT_BUTTON_PRESS: {
            // 按钮按下，进入选中的功能
            const ButtonEvent* buttonEvent = static_cast<const ButtonEvent*>(event);
            if (buttonEvent->getButtonId() == 0) {
                int stateId = FunctionBaudState::ID;
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

void FunctionUartState::updateDisplay(DisplayContext* display) {
    if (!display) {
        return;
    }

    // 获取当前时间戳（毫秒）
    unsigned long timestamp = millis();
    // 将时间戳格式化为字符串并更新 rxBuff 和 txBuff
    char rxText[128];
    snprintf(rxText, sizeof(rxText), "RX: %lu\n", timestamp);  // 模拟接收数据，添加换行符

    // 更新UI显示
    lv_label_set_text_fmt(m_uartStateUI.UartTypeLabel, "%s", (m_uartType == UartType::UART_TYPE_XIAO) ? "XIAO" : "Grove");
    lv_label_set_text_fmt(m_uartStateUI.UartBaudLabel, "%d", FunctionBaudState::m_baudRate);

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
    // 实现具体串口切换，现在只是界面有变化
    //...
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
