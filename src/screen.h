#ifndef _LVGL_SCREEN_H_
#define _LVGL_SCREEN_H_

#include <string>
#include "display/lvgl.h"

static lv_obj_t* force_auto_button = NULL;
static lv_obj_t* autoscroll_button = NULL;
static lv_obj_t* static_information = NULL;
static lv_obj_t* printed_information = NULL;

static bool autoscroll = false;
static bool autoscroll_direction = true;

lv_res_t always_ok_event(struct _lv_obj_t * obj) {
    return LV_RES_OK;
}

lv_res_t autoscroll_event(struct _lv_obj_t * obj) {
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
    force_auto_button = lv_btn_create(lv_scr_act(), NULL);
    autoscroll_button = lv_btn_create(lv_scr_act(), NULL);
    static_information = lv_list_create(lv_scr_act(), NULL);
    printed_information = lv_list_create(lv_scr_act(), NULL);

    lv_obj_set_width(force_auto_button, lv_obj_get_width(lv_scr_act()) / 2);
    lv_obj_set_width(autoscroll_button, lv_obj_get_width(lv_scr_act()) / 2);
    lv_obj_set_width(static_information, lv_obj_get_width(lv_scr_act()) / 2);
    lv_obj_set_width(printed_information, lv_obj_get_width(lv_scr_act()) / 2);

    lv_obj_set_height(force_auto_button, 40);
    lv_obj_set_height(autoscroll_button, 40);
    lv_obj_set_height(static_information, lv_obj_get_height(lv_scr_act()) - 40);
    lv_obj_set_height(printed_information, lv_obj_get_height(lv_scr_act()) - 40);

    lv_obj_set_x(force_auto_button, 0);
    lv_obj_set_x(autoscroll_button, lv_obj_get_width(lv_scr_act()) / 2);
    lv_obj_set_x(static_information, 0);
    lv_obj_set_x(printed_information, lv_obj_get_width(lv_scr_act()) / 2);

    lv_obj_set_y(force_auto_button, lv_obj_get_height(lv_scr_act()) - 40);
    lv_obj_set_y(autoscroll_button, lv_obj_get_height(lv_scr_act()) - 40);
    lv_obj_set_y(static_information, 0);
    lv_obj_set_y(printed_information, 0);
    
    lv_list_set_sb_mode(printed_information, LV_SB_MODE_AUTO);
    lv_list_set_sb_mode(static_information, LV_SB_MODE_AUTO);

    lv_btn_set_action(force_auto_button, LV_BTN_ACTION_CLICK, action);
    lv_btn_set_action(autoscroll_button, LV_BTN_ACTION_CLICK, autoscroll_event);

    lv_label_set_text(lv_label_create(force_auto_button, NULL), "Force Autonomous");
    lv_label_set_text(lv_label_create(autoscroll_button, NULL), "Autoscroll");
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

lv_obj_t* create_static_line(const char * line) {
    lv_obj_t* btn = lv_list_add(static_information, NULL, line, always_ok_event);
    init_btn(btn);
    return btn;
}

lv_obj_t* create_static_line(std::string line) {
    return create_static_line(line.c_str());
}

void remove_static_line(uint32_t index) {
    lv_list_remove(static_information, index);
}

void remove_static_line(lv_obj_t* btn) {
    remove_static_line(lv_list_get_btn_index(static_information, btn));
}

void print_out(const char * line) {
    if (lv_list_get_size(printed_information) > 30) {
        lv_list_remove(printed_information, 0);
    }
	lv_obj_t* btn = lv_list_add(printed_information, NULL, line, always_ok_event);
    init_btn(btn);
}

void print_out(std::string line) {
	print_out(line.c_str());
}

void set_line(lv_obj_t* button, const char * line) {
	lv_label_set_text(lv_obj_get_child(button, NULL), line);
}

void set_line(lv_obj_t* button, std::string line) {
	set_line(button, line.c_str());
}

template <typename T> T print_error(T value) {
	if (value == PROS_ERR || value == PROS_ERR_F) {
        print_out(std::string("Operation Failed: ") + std::string(strerror(errno)));
	}
	return value;
}

#endif //_LVGL_SCREEN_H_
