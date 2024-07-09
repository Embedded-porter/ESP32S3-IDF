/*
* Copyright 2024 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



void setup_scr_screen(lv_ui *ui)
{
	//Write codes screen
	ui->screen = lv_obj_create(NULL);
	lv_obj_set_size(ui->screen, 240, 320);
	lv_obj_set_scrollbar_mode(ui->screen, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_1
	ui->screen_img_1 = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_img_1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_1, &_2_alpha_240x320);
	lv_img_set_pivot(ui->screen_img_1, 50,50);
	lv_img_set_angle(ui->screen_img_1, 0);
	lv_obj_set_pos(ui->screen_img_1, 0, 0);
	lv_obj_set_size(ui->screen_img_1, 240, 320);

	//Write style for screen_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_chart_1
	ui->screen_chart_1 = lv_chart_create(ui->screen);
	lv_chart_set_type(ui->screen_chart_1, LV_CHART_TYPE_LINE);
	lv_chart_set_div_line_count(ui->screen_chart_1, 3, 5);
	lv_chart_set_point_count(ui->screen_chart_1, 5);
	lv_chart_set_range(ui->screen_chart_1, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
	lv_chart_set_range(ui->screen_chart_1, LV_CHART_AXIS_SECONDARY_Y, 0, 100);
	lv_chart_set_zoom_x(ui->screen_chart_1, 256);
	lv_chart_set_zoom_y(ui->screen_chart_1, 256);
	ui->screen_chart_1_0 = lv_chart_add_series(ui->screen_chart_1, lv_color_hex(0x000000), LV_CHART_AXIS_PRIMARY_Y);
#if LV_USE_FREEMASTER == 0
	lv_chart_set_next_value(ui->screen_chart_1, ui->screen_chart_1_0, 1);
	lv_chart_set_next_value(ui->screen_chart_1, ui->screen_chart_1_0, 20);
	lv_chart_set_next_value(ui->screen_chart_1, ui->screen_chart_1_0, 30);
	lv_chart_set_next_value(ui->screen_chart_1, ui->screen_chart_1_0, 40);
	lv_chart_set_next_value(ui->screen_chart_1, ui->screen_chart_1_0, 5);
#endif
	lv_obj_set_pos(ui->screen_chart_1, 18, 87);
	lv_obj_set_size(ui->screen_chart_1, 205, 155);
	lv_obj_set_scrollbar_mode(ui->screen_chart_1, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_chart_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_chart_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_chart_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_chart_1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_chart_1, lv_color_hex(0xe8e8e8), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_chart_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_width(ui->screen_chart_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->screen_chart_1, lv_color_hex(0xe8e8e8), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->screen_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_chart_1, Part: LV_PART_TICKS, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_chart_1, lv_color_hex(0x151212), LV_PART_TICKS|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_chart_1, &lv_font_montserratMedium_12, LV_PART_TICKS|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_chart_1, 255, LV_PART_TICKS|LV_STATE_DEFAULT);
	lv_obj_set_style_line_width(ui->screen_chart_1, 2, LV_PART_TICKS|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->screen_chart_1, lv_color_hex(0xe8e8e8), LV_PART_TICKS|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->screen_chart_1, 255, LV_PART_TICKS|LV_STATE_DEFAULT);

	//Write codes screen_bar_1
	ui->screen_bar_1 = lv_bar_create(ui->screen);
	lv_obj_set_style_anim_time(ui->screen_bar_1, 1000, 0);
	lv_bar_set_mode(ui->screen_bar_1, LV_BAR_MODE_NORMAL);
	lv_bar_set_range(ui->screen_bar_1, 0, 100);
	lv_bar_set_value(ui->screen_bar_1, 50, LV_ANIM_OFF);
	lv_obj_set_pos(ui->screen_bar_1, 54, 263);
	lv_obj_set_size(ui->screen_bar_1, 121, 18);

	//Write style for screen_bar_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_bar_1, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_bar_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_bar_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_bar_1, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_bar_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_bar_1, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_bar_1, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_bar_1, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_bar_1, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_bar_1, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);

	//The custom code of screen.
	

	//Update current screen layout.
	lv_obj_update_layout(ui->screen);

}
