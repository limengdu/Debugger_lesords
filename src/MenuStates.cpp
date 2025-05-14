#include "Global.h"
#include "MenuStates.h"
#include "StateManager.h"
#include "LvglStyle.h"
#include "FunctionBaudState.h"

// MainMenuState实现
MainMenuState::MainMenuState() 
    : m_itemCount(0), 
      m_currentSelection(0) 
{
    // 初始化菜单项数组
    for (int i = 0; i < MAX_ITEMS; ++i) {
        m_items[i].label = nullptr;
        m_items[i].stateId = 0;
    }

    m_mainMenu = {
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
    };
}

bool MainMenuState::addMenuItem(const char* label, int stateId) {
    if (m_itemCount >= MAX_ITEMS) {
        return false;
    }
    
    m_items[m_itemCount].label = label;
    m_items[m_itemCount].stateId = stateId;
    m_itemCount++;
    
    return true;
}

void MainMenuState::onEnter() {
    // 重置选择位置
    m_currentSelection = -1;

    if (m_mainMenu.screen != nullptr) {
        if ( lv_scr_act() != m_mainMenu.screen) {
            lv_scr_load(m_mainMenu.screen);
        }
        return;
    }

    m_mainMenu.screen = lv_obj_create(NULL);

    lv_obj_add_style(m_mainMenu.screen, &style_screen, 0);

    // Title
    lv_obj_t* label = lv_label_create(m_mainMenu.screen);
    lv_label_set_text( label, "SWD" );
    lv_obj_set_pos( label, 55 + 12, 23 ); // 62 + *
    lv_obj_set_style_text_color(label, lv_color_hex(0xA6ACAF), LV_PART_MAIN);
    lv_obj_add_style(label, &style_font_18, 0);

    label = lv_label_create(m_mainMenu.screen);
    lv_label_set_text( label, "DEBUG" );
    lv_obj_set_pos( label, 55 + 12 + 50, 18 );
    lv_obj_set_style_text_color(label, lv_color_hex(0xACE62F), LV_PART_MAIN);
    lv_obj_add_style(label, &style_font_28, 0);

    lv_obj_t* led_title = lv_led_create(m_mainMenu.screen);
    lv_obj_set_pos(led_title, 55 + 12 + 50 + 110 + 3, 28);
    lv_led_on(led_title);
    lv_led_set_color(led_title, lv_color_hex(0xACE62F));
    lv_obj_add_style(led_title, &style_led, 0);

    // Left Block
    m_mainMenu.uart_bg = lv_obj_create(m_mainMenu.screen);
    lv_obj_set_size(m_mainMenu.uart_bg, 108, 154);
    lv_obj_set_pos(m_mainMenu.uart_bg, 12 + 12, 62);
    lv_obj_add_style(m_mainMenu.uart_bg, &style_nofocus_bg, 0);

    // Baud
    label = lv_label_create(m_mainMenu.uart_bg);
    lv_label_set_text(label, "Baud");
    lv_obj_set_pos(label, 12, 13);
    lv_obj_set_style_text_color(label, lv_color_hex(0xA6ACAF), LV_PART_MAIN);
    lv_obj_add_style(label, &style_font_12, 0);

    // Baud Value
    m_mainMenu.baud_value = lv_label_create(m_mainMenu.uart_bg);
    lv_label_set_text(m_mainMenu.baud_value, "9600");
    lv_obj_set_pos(m_mainMenu.baud_value, 13, 33);
    lv_obj_set_style_text_color(m_mainMenu.baud_value, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_style(m_mainMenu.baud_value, &style_font_20, 0);

    // RX
    label = lv_label_create(m_mainMenu.uart_bg);
    lv_label_set_text(label, "RX");
    lv_obj_set_pos(label, 13, 70);
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_style(label, &style_font_18, 0);

    lv_obj_t* led_RX = lv_led_create(m_mainMenu.uart_bg);
    lv_obj_set_pos(led_RX, 45, 77);
    lv_led_on(led_RX);
    lv_led_set_color(led_RX, lv_color_hex(0xDDE62F));
    lv_obj_add_style(led_RX, &style_led, 0);

    // TX
    label = lv_label_create(m_mainMenu.uart_bg);
    lv_label_set_text(label, "TX");
    lv_obj_set_pos(label, 13, 109);
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_style(label, &style_font_18, 0);

    lv_obj_t* led_TX = lv_led_create(m_mainMenu.uart_bg);
    lv_obj_set_pos(led_TX, 45, 115);
    lv_led_on(led_TX);
    lv_led_set_color(led_TX, lv_color_hex(0x2FE6AC));
    lv_obj_add_style(led_TX, &style_led, 0);

    // Right Block
    m_mainMenu.power_bg = lv_obj_create(m_mainMenu.screen);
    lv_obj_set_size(m_mainMenu.power_bg, 152, 154);
    lv_obj_set_pos(m_mainMenu.power_bg, 12 + 132, 62);
    lv_obj_add_style(m_mainMenu.power_bg, &style_nofocus_bg, 0);

    // 电压
    label = lv_label_create(m_mainMenu.power_bg);
    lv_label_set_text(label, "U");
    lv_obj_set_pos(label, 12, 20);
    lv_obj_set_style_text_color(label, lv_color_hex(0xDDE62F), LV_PART_MAIN);
    lv_obj_add_style(label, &style_font_22, 0);

    label = lv_label_create(m_mainMenu.power_bg);
    lv_label_set_text(label, "V");
    lv_obj_set_pos(label, 123, 28);
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_style(label, &style_font_14, 0);

    // 电流
    label = lv_label_create(m_mainMenu.power_bg);
    lv_label_set_text(label, "I");
    lv_obj_set_pos(label, 12, 64);
    lv_obj_set_style_text_color(label, lv_color_hex(0x2FE6AC), LV_PART_MAIN);
    lv_obj_add_style(label, &style_font_22, 0);

    label = lv_label_create(m_mainMenu.power_bg);
    lv_label_set_text(label, "A");
    lv_obj_set_pos(label, 123, 72);
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_style(label, &style_font_14, 0);

    // 功耗
    label = lv_label_create(m_mainMenu.power_bg);
    lv_label_set_text(label, "P");
    lv_obj_set_pos(label, 12, 107);
    lv_obj_set_style_text_color(label, lv_color_hex(0x2F8EE6), LV_PART_MAIN);
    lv_obj_add_style(label, &style_font_22, 0);

    label = lv_label_create(m_mainMenu.power_bg);
    lv_label_set_text(label, "W");
    lv_obj_set_pos(label, 123, 115);
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_style(label, &style_font_14, 0);

    // value
    m_mainMenu.vol = lv_label_create(m_mainMenu.power_bg);
    lv_label_set_text(m_mainMenu.vol, "5.0345");
    lv_obj_set_pos(m_mainMenu.vol, 41, 20);
    lv_obj_set_style_text_color(m_mainMenu.vol, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_style(m_mainMenu.vol, &style_font_22, 0);

    m_mainMenu.cur = lv_label_create(m_mainMenu.power_bg);
    lv_label_set_text(m_mainMenu.cur, "0.0182");
    lv_obj_set_pos(m_mainMenu.cur, 41, 64);
    lv_obj_set_style_text_color(m_mainMenu.cur, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_style(m_mainMenu.cur, &style_font_22, 0);

    m_mainMenu.power = lv_label_create(m_mainMenu.power_bg);
    lv_label_set_text(m_mainMenu.power, "0.0934");
    lv_obj_set_pos(m_mainMenu.power, 41, 64);
    lv_obj_set_pos(m_mainMenu.power, 41, 107);
    lv_obj_set_style_text_color(m_mainMenu.power, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_add_style(m_mainMenu.power, &style_font_22, 0);

    lv_scr_load(m_mainMenu.screen);
}

void MainMenuState::onExit() {
    // 清理工作
}

bool MainMenuState::handleEvent(StateMachine* machine, const Event* event) {
    if (!machine || !event) {
        return false;
    }
    
    switch (event->getType()) {
        case EVENT_WHEEL_CLOCKWISE:
        case EVENT_WHEEL_COUNTERCLOCKWISE: {
            if (m_currentSelection < 0) {
                m_currentSelection = 0;
            } else {
                m_currentSelection = !m_currentSelection;
            }

            return true;
        }
        
        case EVENT_BUTTON_PRESS: {
            if (m_currentSelection < 0) {
                m_currentSelection = 0;
                break;
            }

            const ButtonEvent* buttonEvent = static_cast<const ButtonEvent*>(event);
            if (buttonEvent->getButtonId() == BOOT_BTN) {
                if (m_currentSelection != -1) {
                    int stateId = m_items[m_currentSelection].stateId;
                    State* nextState = StateManager::getInstance()->getState(stateId);
                    if (nextState) {
                        machine->changeState(nextState);
                        return true;
                    }
                }
            }
            return false;
        }

        default:
            return false;
    }

    return true;
}

void MainMenuState::updateDisplay(DisplayContext* display) {
    if (!display) {
        return;
    }

    Adafruit_INA228* ina228 = nullptr;
    char value[10];
    float vol = 0, cur = 0, power = 0;

    if (m_currentSelection == -1) {
        lv_obj_add_style(m_mainMenu.uart_bg, &style_nofocus_bg, 0);
        lv_obj_add_style(m_mainMenu.power_bg, &style_nofocus_bg, 0);
    } else if (m_currentSelection == 0) {
        lv_obj_add_style(m_mainMenu.uart_bg, &style_focus_bg, 0);
        lv_obj_add_style(m_mainMenu.power_bg, &style_nofocus_bg, 0);
    } else if (m_currentSelection == 1) {
        lv_obj_add_style(m_mainMenu.uart_bg, &style_nofocus_bg, 0);
        lv_obj_add_style(m_mainMenu.power_bg, &style_focus_bg, 0);
    }

    ina228 = display->getINA228();
    display->updateShuntOfINA();
    vol = (ina228->readBusVoltage() / 1000 - ina228->readShuntVoltage()) / 1000;
    cur = _max(0.0, ina228->readCurrent() / 1000);
    power = vol * cur;

    lv_label_set_text_fmt(m_mainMenu.baud_value, "%d", FunctionBaudState::m_baudRate);

    snprintf(value, sizeof(value), "%.4f", vol);
    lv_label_set_text(m_mainMenu.vol, value);

    snprintf(value, sizeof(value), "%.4f", cur);
    lv_label_set_text(m_mainMenu.cur, value);

    snprintf(value, sizeof(value), "%.4f", power);
    lv_label_set_text(m_mainMenu.power, value);
}
