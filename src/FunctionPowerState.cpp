//
// Created by Administrator on 25-4-29.
//

#include "FunctionPowerState.h"

#include "FunctionUartState.h"

FunctionPowerState::FunctionPowerState()
: FunctionState("FunctionPowerState")
, m_powerStateUI()
, m_voltage(0.0)
, m_current(0.0)
, m_power(0.0)
, m_minCurrent(0.0)
, m_maxCurrent(0.0)
, m_totalCurrent(0.0)
, m_minPower(0.0)
, m_maxPower(0.0)
, m_totalPower(0.0)
, currentInterfaceIndex(0)
{
    // 初始化界面函数数组
    interfaceFunctions[0] = &FunctionPowerState::powerInterface_1;
    interfaceFunctions[1] = &FunctionPowerState::powerInterface_2;
    interfaceFunctions[2] = &FunctionPowerState::powerInterface_3;
}

void FunctionPowerState::onEnter()
{
    if (m_powerStateUI.Screen != nullptr) {
        if ( lv_scr_act() != m_powerStateUI.Screen) {
            lv_scr_load(m_powerStateUI.Screen);
        }
        return;
    }
    m_powerStateUI.Screen = lv_obj_create(NULL);
    lv_obj_add_style(m_powerStateUI.Screen, &style_screen, 0);

    m_powerStateUI.voltageLabel_V = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.currentLabel_A = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.currentLabel_mA = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.currentLabel_uA = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.powerLabel_W = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.powerLabel_mW = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.minLabel = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.maxLabel = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.totalLabel = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.timeLabel = lv_label_create(m_powerStateUI.Screen);

    m_powerStateUI.minLabel_A = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.minLabel_wh = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.maxLabel_A = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.maxLabel_wh = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.totalLabel_wh = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.totalLabel_Ah = lv_label_create(m_powerStateUI.Screen);
    (this->*interfaceFunctions[currentInterfaceIndex])();
    lv_scr_load(m_powerStateUI.Screen);
}

void FunctionPowerState::onExit()
{

}

bool FunctionPowerState::handleEvent(StateMachine* machine, const Event* event)
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
            return false;
        }

        default:
            return false;
    }
}

void FunctionPowerState::updateDisplay(DisplayContext* display)
{
    if (!display) {
        return;
    }

    char buff[16];
    sprintf(buff, "U %.4f v", m_voltage);
    lv_label_set_text(m_powerStateUI.voltageLabel_V, buff);
    sprintf(buff, "I  %.4f A", m_current);
    lv_label_set_text(m_powerStateUI.currentLabel_A, buff);
    sprintf(buff, "P %.4f w", m_power);
    lv_label_set_text(m_powerStateUI.powerLabel_W, buff);
    sprintf(buff, " %.4f mA", m_current*1000);
    lv_label_set_text(m_powerStateUI.currentLabel_mA, buff);
    sprintf(buff, " %.1f uA", m_current*1000000);
    lv_label_set_text(m_powerStateUI.currentLabel_uA, buff);
    sprintf(buff, " %.4f mW", m_power*1000);
    lv_label_set_text(m_powerStateUI.powerLabel_mW, buff);
    sprintf(buff, " %.4f Ah", m_totalCurrent);
    lv_label_set_text(m_powerStateUI.totalLabel_Ah, buff);
    sprintf(buff, " %.4f wh", m_totalPower);
    lv_label_set_text(m_powerStateUI.totalLabel_wh, buff);
    sprintf(buff, " %.4f A", m_minCurrent);
    lv_label_set_text(m_powerStateUI.minLabel_A, buff);
    sprintf(buff, " %.4f w", m_minPower);
    lv_label_set_text(m_powerStateUI.minLabel_wh, buff);
    sprintf(buff, " %.4f A", m_maxCurrent);
    lv_label_set_text(m_powerStateUI.maxLabel_A, buff);
    sprintf(buff, " %.4f w", m_maxPower);
    lv_label_set_text(m_powerStateUI.maxLabel_wh, buff);

    unsigned long currentTime = millis() - startTime;
    unsigned long seconds = currentTime / 1000;
    unsigned long minutes = seconds / 60;
    seconds = seconds % 60;
    unsigned long hours = minutes / 60;
    minutes = minutes % 60;
    // 格式化时间为 00:00:00 格式，并使用换行符实现上下展示
    sprintf(buff, "Time\n%02lu:%02lu:%02lu", hours, minutes, seconds);
    lv_label_set_text(m_powerStateUI.timeLabel, buff);
}


void FunctionPowerState::powerInterface_1()
{
    // 电压
    char buf[16];
    sprintf(buf, "U %.4f v", m_voltage);
    lv_label_set_text(m_powerStateUI.voltageLabel_V, buf);
    lv_obj_set_pos(m_powerStateUI.voltageLabel_V, 42, 30);
    lv_obj_set_style_text_color(m_powerStateUI.voltageLabel_V, lv_color_hex(0xDDE62F), LV_PART_MAIN);
    lv_obj_add_style(m_powerStateUI.voltageLabel_V, &style_font_36, 0);

    // 电流
    sprintf(buf, "I %.4f A", m_current);
    lv_label_set_text(m_powerStateUI.currentLabel_A, buf);
    lv_obj_set_pos(m_powerStateUI.currentLabel_A, 42+7, 94);
    lv_obj_set_style_text_color(m_powerStateUI.currentLabel_A, lv_color_hex(0x2FE6AC), LV_PART_MAIN);
    lv_obj_add_style(m_powerStateUI.currentLabel_A, &style_font_36, 0);

    // 功耗
    sprintf(buf, "P %.4f w", m_power);
    lv_label_set_text(m_powerStateUI.powerLabel_W, buf);
    lv_obj_set_pos(m_powerStateUI.powerLabel_W, 42, 157);
    lv_obj_set_style_text_color(m_powerStateUI.powerLabel_W, lv_color_hex(0x2F8EE6), LV_PART_MAIN);
    lv_obj_add_style(m_powerStateUI.powerLabel_W, &style_font_36, 0);
}

void FunctionPowerState::powerInterface_2()
{

    char buf[16];
    // 电压
    sprintf(buf, "U %.4f v", m_voltage);
    lv_label_set_text(m_powerStateUI.voltageLabel_V, buf);
    lv_obj_set_pos(m_powerStateUI.voltageLabel_V, 42, 40);
    lv_obj_set_style_text_color(m_powerStateUI.voltageLabel_V, lv_color_hex(0xDDE62F), LV_PART_MAIN);
    lv_obj_add_style(m_powerStateUI.voltageLabel_V, &style_font_22, 0);

    // 电流
    sprintf(buf, "I %.4f A", m_current);
    lv_label_set_text(m_powerStateUI.currentLabel_A, buf);
    lv_obj_set_pos(m_powerStateUI.currentLabel_A, 48, 70);
    lv_obj_set_style_text_color(m_powerStateUI.currentLabel_A, lv_color_hex(0x2FE6AC), LV_PART_MAIN);
    lv_obj_add_style(m_powerStateUI.currentLabel_A, &style_font_22, 0);

    sprintf(buf, " %.4f mA", m_current*1000);
    lv_label_set_text(m_powerStateUI.currentLabel_mA, buf);
    lv_obj_set_pos(m_powerStateUI.currentLabel_mA, 58, 100);
    lv_obj_set_style_text_color(m_powerStateUI.currentLabel_mA, lv_color_hex(0x2FE6AC), LV_PART_MAIN);
    lv_obj_add_style(m_powerStateUI.currentLabel_mA, &style_font_22, 0);

    sprintf(buf, " %.1f uA", m_current*1000000);
    lv_label_set_text(m_powerStateUI.currentLabel_uA, buf);
    lv_obj_set_pos(m_powerStateUI.currentLabel_uA, 58, 130);
    lv_obj_set_style_text_color(m_powerStateUI.currentLabel_uA, lv_color_hex(0x2FE6AC), LV_PART_MAIN);
    lv_obj_add_style(m_powerStateUI.currentLabel_uA, &style_font_22, 0);

    // 功耗
    sprintf(buf, "P %.4f w", m_power);
    lv_label_set_text(m_powerStateUI.powerLabel_W, buf);
    lv_obj_set_pos(m_powerStateUI.powerLabel_W, 42, 160);
    lv_obj_set_style_text_color(m_powerStateUI.powerLabel_W, lv_color_hex(0x2F8EE6), LV_PART_MAIN);
    lv_obj_add_style(m_powerStateUI.powerLabel_W, &style_font_22, 0);

    sprintf(buf, " %.4f mw", m_power*1000);
    lv_label_set_text(m_powerStateUI.powerLabel_mW, buf);
    lv_obj_set_pos(m_powerStateUI.powerLabel_mW, 42+15, 190);
    lv_obj_set_style_text_color(m_powerStateUI.powerLabel_mW, lv_color_hex(0x2F8EE6), LV_PART_MAIN);
    lv_obj_add_style(m_powerStateUI.powerLabel_mW, &style_font_22, 0);
}

void FunctionPowerState::powerInterface_3()
{
    char buf[16];

    // 最小
    lv_label_set_text(m_powerStateUI.minLabel, "Min");
    lv_obj_set_pos(m_powerStateUI.minLabel, 30, 30);
    lv_obj_set_style_text_color(m_powerStateUI.minLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_style(m_powerStateUI.minLabel, &style_font_18, 0);
    // 最小电流
    sprintf(buf, "%.4f A", m_minCurrent);
    lv_label_set_text(m_powerStateUI.minLabel_A, buf);
    lv_obj_set_pos(m_powerStateUI.minLabel_A, 30, 30+20);
    lv_obj_set_style_text_color(m_powerStateUI.minLabel_A, lv_color_hex(0x2FE6AC), LV_PART_MAIN);
    lv_obj_add_style(m_powerStateUI.minLabel_A, &style_font_18, 0);
    // 最小功耗
    sprintf(buf, "%.4f w", m_minPower);
    lv_label_set_text(m_powerStateUI.minLabel_wh, buf);
    lv_obj_set_pos(m_powerStateUI.minLabel_wh, 30, 30+20+20);
    lv_obj_set_style_text_color(m_powerStateUI.minLabel_wh, lv_color_hex(0x2FE6AC), LV_PART_MAIN);
    lv_obj_add_style(m_powerStateUI.minLabel_wh, &style_font_18, 0);

    // 最大
    lv_label_set_text(m_powerStateUI.maxLabel, "Max");
    lv_obj_set_pos(m_powerStateUI.maxLabel, 180, 30);
    lv_obj_set_style_text_color(m_powerStateUI.maxLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_style(m_powerStateUI.maxLabel, &style_font_18, 0);
    // 最大电流
    sprintf(buf, "%.4f A", m_maxCurrent);
    lv_label_set_text(m_powerStateUI.maxLabel_A, buf);
    lv_obj_set_pos(m_powerStateUI.maxLabel_A, 180, 30+20);
    lv_obj_set_style_text_color(m_powerStateUI.maxLabel_A, lv_color_hex(0x2FE6AC), LV_PART_MAIN);
    lv_obj_add_style(m_powerStateUI.maxLabel_A, &style_font_18, 0);
    // 最大功耗
    sprintf(buf, "%.4f w", m_maxPower);
    lv_label_set_text(m_powerStateUI.maxLabel_wh, buf);
    lv_obj_set_pos(m_powerStateUI.maxLabel_wh, 180, 30+20+20);
    lv_obj_set_style_text_color(m_powerStateUI.maxLabel_wh, lv_color_hex(0x2FE6AC), LV_PART_MAIN);
    lv_obj_add_style(m_powerStateUI.maxLabel_wh, &style_font_18, 0);

    // 总计
    lv_label_set_text(m_powerStateUI.totalLabel, "Total");
    lv_obj_set_pos(m_powerStateUI.totalLabel, 30, 130);
    lv_obj_set_style_text_color(m_powerStateUI.totalLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_style(m_powerStateUI.totalLabel, &style_font_18, 0);
    // 总电流
    sprintf(buf, "%.4f Ah", m_totalPower);
    lv_label_set_text(m_powerStateUI.totalLabel_Ah, buf);
    lv_obj_set_pos(m_powerStateUI.totalLabel_Ah, 30, 130+20);
    lv_obj_set_style_text_color(m_powerStateUI.totalLabel_Ah, lv_color_hex(0x2FE6AC), LV_PART_MAIN);
    lv_obj_add_style(m_powerStateUI.totalLabel_Ah, &style_font_18, 0);
    // 总功耗
    sprintf(buf, "%.4f wh", m_totalPower);
    lv_label_set_text(m_powerStateUI.totalLabel_wh, buf);
    lv_obj_set_pos(m_powerStateUI.totalLabel_wh, 30, 130+20+20);
    lv_obj_set_style_text_color(m_powerStateUI.totalLabel_wh, lv_color_hex(0x2FE6AC), LV_PART_MAIN);
    lv_obj_add_style(m_powerStateUI.totalLabel_wh, &style_font_18, 0);



    unsigned long currentTime = millis() - startTime;
    unsigned long seconds = currentTime / 1000;
    unsigned long minutes = seconds / 60;
    seconds = seconds % 60;
    unsigned long hours = minutes / 60;
    minutes = minutes % 60;

    // 格式化时间为 00:00:00 格式，并使用换行符实现上下展示
    sprintf(buf, "Time\n%02lu:%02lu:%02lu", hours, minutes, seconds);
    lv_label_set_text(m_powerStateUI.timeLabel, buf);
    lv_obj_set_pos(m_powerStateUI.timeLabel, 180, 130);
    lv_obj_set_style_text_color(m_powerStateUI.timeLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_style(m_powerStateUI.timeLabel, &style_font_18, 0);
}

void FunctionPowerState::clearScreen()
{
    if (m_powerStateUI.Screen != nullptr) {
        // 删除所有标签对象
        DELETE_LABEL(m_powerStateUI.voltageLabel_V);
        DELETE_LABEL(m_powerStateUI.currentLabel_A);
        DELETE_LABEL(m_powerStateUI.currentLabel_mA);
        DELETE_LABEL(m_powerStateUI.currentLabel_uA);
        DELETE_LABEL(m_powerStateUI.powerLabel_W);
        DELETE_LABEL(m_powerStateUI.powerLabel_mW);
        DELETE_LABEL(m_powerStateUI.minLabel);
        DELETE_LABEL(m_powerStateUI.maxLabel);
        DELETE_LABEL(m_powerStateUI.totalLabel);
        DELETE_LABEL(m_powerStateUI.timeLabel);
        DELETE_LABEL(m_powerStateUI.minLabel_A);
        DELETE_LABEL(m_powerStateUI.minLabel_wh);
        DELETE_LABEL(m_powerStateUI.maxLabel_A);
        DELETE_LABEL(m_powerStateUI.maxLabel_wh);
        DELETE_LABEL(m_powerStateUI.totalLabel_wh);
        DELETE_LABEL(m_powerStateUI.totalLabel_Ah);
    }

    // 删了重建，实现清屏效果
    m_powerStateUI.voltageLabel_V = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.currentLabel_A = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.currentLabel_mA = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.currentLabel_uA = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.powerLabel_W = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.powerLabel_mW = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.minLabel = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.maxLabel = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.totalLabel = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.timeLabel = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.minLabel_A = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.minLabel_wh = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.maxLabel_A = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.maxLabel_wh = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.totalLabel_wh = lv_label_create(m_powerStateUI.Screen);
    m_powerStateUI.totalLabel_Ah = lv_label_create(m_powerStateUI.Screen);

}


int FunctionPowerState::getID() const
{
    return ID;
}

const char* FunctionPowerState::getName() const
{
    return "FunctionPowerState";
}
