/*
 *
 * Copyright (c) 2023 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 *
 */

#include <zephyr/kernel.h>
#include "util.h"

LV_IMAGE_DECLARE(bolt);

/* Draws a filled rectangle on the layer, given its top-left corner and size. */
static void draw_rect_on_layer(lv_layer_t *layer,
                               int32_t x,
                               int32_t y,
                               int32_t w,
                               int32_t h,
                               const lv_draw_rect_dsc_t *dsc) {
    if (w <= 0 || h <= 0) {
        return;
    }

    lv_area_t area = {x, y, x + w - 1, y + h - 1};
    lv_draw_rect(layer, dsc, &area);
                               }

void rotate_canvas(lv_obj_t *canvas, lv_color_t cbuf[]) {
    static lv_color_t cbuf_tmp[CANVAS_SIZE * CANVAS_SIZE];
    memcpy(cbuf_tmp, cbuf, sizeof(cbuf_tmp));

    lv_image_dsc_t img;
    img.header.magic = LV_IMAGE_HEADER_MAGIC;
    img.header.cf = LV_COLOR_FORMAT_NATIVE;
    img.header.w = CANVAS_SIZE;
    img.header.h = CANVAS_SIZE;
    img.header.stride = CANVAS_SIZE * sizeof(lv_color_t);
    img.data = (const uint8_t *)cbuf_tmp;
    img.data_size = sizeof(cbuf_tmp);

    lv_canvas_fill_bg(canvas, LVGL_BACKGROUND, LV_OPA_COVER);

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);

    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    img_dsc.src = &img;
    img_dsc.rotation = 900; /* 0.1 degree units, so 90 degrees */
    img_dsc.pivot.x = CANVAS_SIZE / 2;
    img_dsc.pivot.y = CANVAS_SIZE / 2;

    lv_area_t area = {0, 0, CANVAS_SIZE - 1, CANVAS_SIZE - 1};
    lv_draw_image(&layer, &img_dsc, &area);

    lv_canvas_finish_layer(canvas, &layer);
}

void draw_battery(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_rect_dsc_t rect_black_dsc;
    init_rect_dsc(&rect_black_dsc, LVGL_BACKGROUND);
    lv_draw_rect_dsc_t rect_white_dsc;
    init_rect_dsc(&rect_white_dsc, LVGL_FOREGROUND);

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);

    draw_rect_on_layer(&layer, 0, 2, 29, 12, &rect_white_dsc);
    draw_rect_on_layer(&layer, 1, 3, 27, 10, &rect_black_dsc);
    draw_rect_on_layer(&layer, 2, 4, (state->battery + 2) / 4, 8, &rect_white_dsc);
    draw_rect_on_layer(&layer, 30, 5, 3, 6, &rect_white_dsc);
    draw_rect_on_layer(&layer, 31, 6, 1, 4, &rect_black_dsc);

    if (state->charging) {
        lv_draw_image_dsc_t img_dsc;
        lv_draw_image_dsc_init(&img_dsc);
        img_dsc.src = &bolt;

        lv_area_t bolt_area = {9, -1, 9 + bolt.header.w - 1, -1 + bolt.header.h - 1};
        lv_draw_image(&layer, &img_dsc, &bolt_area);
    }

    lv_canvas_finish_layer(canvas, &layer);
}

void init_label_dsc(lv_draw_label_dsc_t *label_dsc,
                    lv_color_t color,
                    const lv_font_t *font,
                    lv_text_align_t align) {
    lv_draw_label_dsc_init(label_dsc);
    label_dsc->color = color;
    label_dsc->font = font;
    label_dsc->align = align;
                    }

void init_rect_dsc(lv_draw_rect_dsc_t *rect_dsc, lv_color_t bg_color) {
    lv_draw_rect_dsc_init(rect_dsc);
    rect_dsc->bg_color = bg_color;
}

void init_line_dsc(lv_draw_line_dsc_t *line_dsc, lv_color_t color, uint8_t width) {
    lv_draw_line_dsc_init(line_dsc);
    line_dsc->color = color;
    line_dsc->width = width;
}

void init_arc_dsc(lv_draw_arc_dsc_t *arc_dsc, lv_color_t color, uint8_t width) {
    lv_draw_arc_dsc_init(arc_dsc);
    arc_dsc->color = color;
    arc_dsc->width = width;
}
