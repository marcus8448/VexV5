#ifndef _SCREEN_MANAGEMENT_H_
#define _SCREEN_MANAGEMENT_H_

#include "definitions.hpp"
#include "display/lvgl.h"
#include <string>

static lv_obj_t* force_auto_button = nullptr;
static lv_obj_t* autoscroll_button = nullptr;
static lv_obj_t* static_information = nullptr;
static lv_obj_t* printed_information = nullptr;

static bool autoscroll = false;
static bool autoscroll_direction = true;

lv_res_t autoscroll_event(struct _lv_obj_t* obj) {
    if (!autoscroll) {
        autoscroll = true;
        autoscroll_direction = true;
    } else {
        if (autoscroll_direction) {
            autoscroll_direction = false;
        } else {
            autoscroll = false;
        }
    }
    return LV_RES_OK;
}

void init_screen(lv_action_t action) {
    force_auto_button = lv_btn_create(lv_scr_act(), nullptr);
    autoscroll_button = lv_btn_create(lv_scr_act(), nullptr);
    static_information = lv_list_create(lv_scr_act(), nullptr);
    printed_information = lv_list_create(lv_scr_act(), nullptr);

    lv_coord_t column_width = lv_obj_get_width(lv_scr_act()) / 2;
    lv_coord_t button_height = lv_obj_get_height(lv_scr_act()) - 40;

    lv_obj_set_size(force_auto_button, column_width, 40);
    lv_obj_set_size(autoscroll_button, column_width, 40);
    lv_obj_set_size(static_information, column_width, button_height);
    lv_obj_set_size(printed_information, column_width, button_height);

    lv_obj_set_pos(force_auto_button, 0, button_height);
    lv_obj_set_pos(autoscroll_button, column_width, button_height);
    lv_obj_set_pos(static_information, 0, 0);
    lv_obj_set_pos(printed_information, column_width, 0);

    lv_list_set_sb_mode(printed_information, LV_SB_MODE_AUTO);
    lv_list_set_sb_mode(static_information, LV_SB_MODE_AUTO);

    lv_btn_set_action(force_auto_button, LV_BTN_ACTION_CLICK, action);
    lv_btn_set_action(autoscroll_button, LV_BTN_ACTION_CLICK, autoscroll_event);

    lv_label_set_text(lv_label_create(force_auto_button, nullptr), "Force Autonomous");
    lv_label_set_text(lv_label_create(autoscroll_button, nullptr), "Autoscroll");
}

void update_screen() {
    if (autoscroll) {
        lv_page_scroll_ver(printed_information, autoscroll_direction ? -10 : 10);
    }
}

void init_btn(lv_obj_t* btn) {
    lv_obj_set_height(btn, lv_obj_get_height(btn) - lv_obj_get_height(btn) / 4);
    lv_obj_set_click(btn, false);
}

lv_obj_t* create_static_line(const char* line) {
    lv_obj_t* btn = lv_list_add(static_information, nullptr, line, always_ok_event);
    init_btn(btn);
    return btn;
}

lv_obj_t* create_static_line(const std::string& line) {
    return create_static_line(line.c_str());
}

void remove_static_line(uint32_t index) {
    lv_list_remove(static_information, index);
}

void remove_static_line(lv_obj_t* btn) {
    remove_static_line(lv_list_get_btn_index(static_information, btn));
}

void print_out(const char* line) {
    if (lv_list_get_size(printed_information) > 30) {
        lv_list_remove(printed_information, 0);
    }
    lv_obj_t* btn = lv_list_add(printed_information, nullptr, line, always_ok_event);
    init_btn(btn);
}

void print_out(const std::string& line) {
    print_out(line.c_str());
}

void print_out(int line) {
    print_out(std::to_string(line));
}

void print_out(double line) {
    print_out(std::to_string(line));
}

void print_out(long long line) {
    print_out(std::to_string(line));
}

void set_line(lv_obj_t* button, const char* line) {
    lv_label_set_text(lv_obj_get_child(button, nullptr), line);
}

void set_line(lv_obj_t* button, const std::string& line) {
    set_line(button, line.c_str());
}

int32_t print_errno(int32_t return_code) {
    if (return_code == PROS_ERR) {
        print_out(strerror(errno));
    }
    return return_code;
}

double print_errno(double return_code) {
    if (return_code == PROS_ERR_F) {
        print_out(strerror(errno));
    }
    return return_code;
}

#endif//_SCREEN_MANAGEMENT_H_
