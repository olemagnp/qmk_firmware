// Copyright 2022 beekeeb
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "keymap_norwegian.h"

#define MO_NAV MO(_NAV)
#define MO_FUN MO(_FUN)
#define SPC_NSYM LT(_NUM_SYM, KC_SPC)
#define ENT_NSYM LT(_NUM_SYM, KC_ENT)
#define BSP_MODS LT(_MODS, KC_BSPC)

#define OSM_GUI OSM(MOD_LGUI)
#define OSM_SFT OSM(MOD_LSFT)
#define OSM_CTL OSM(MOD_LCTL)
#define OSM_ALT OSM(MOD_LALT)

#define KC_COPY LCTL(KC_C)
#define KC_PSTE LCTL(KC_V)
#define KC_CUT LCTL(KC_X)
#define KC_UNDO LCTL(KC_Z)

enum layers {
    _COLEMAK,
    _NUM_SYM,
    _NAV,
    _FUN,
    _MODS,
};

const key_override_t slash_override = ko_make_basic(MOD_MASK_SHIFT, NO_SLSH, NO_BSLS);
const key_override_t quot_override = ko_make_basic(MOD_MASK_SHIFT, NO_DQUO, NO_QUOT);

const key_override_t **key_overrides = (const key_override_t *[]) {
    &slash_override,
    &quot_override,
    NULL
};


bool is_alt_tab_active = false;
enum custom_keycodes {
    ALT_TAB = SAFE_RANGE,
};

layer_state_t layer_state_set_user(layer_state_t state) {
    if (is_alt_tab_active) {
        unregister_code(KC_LALT);
        is_alt_tab_active = false;
    }
    return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case ALT_TAB:
            if (record->event.pressed) {
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(KC_LALT);
                }
                register_code(KC_TAB);
            } else {
                unregister_code(KC_TAB);
            }
            break;
        return false;
    }
    return true;
}

// Using CAGS-home-row-mods
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
     /*
      * ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
      * │Tab│ Q │ W │ F │ P │ B │       │ J │ L │ U │ Y │ ' │ Æ │
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │Ctl│ A │ R │ S │ T │ G │       │ M │ N │ E │ I │ O │ Ø │
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │Sft│ Z │ X │ C │ D │ V │       │ K │ H │ , │ . │ / │ Å │
      * └───┴───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┴───┘
      *               ┌───┐                   ┌───┐
      *               │GUI├───┐           ┌───┤Alt│
      *               └───┤   ├───┐   ┌───┤   ├───┘
      *                   └───┤BSP│   │Ent├───┘
      *                       └───┘   └───┘
      */
    [_COLEMAK] = LAYOUT_split_3x6_3(
         KC_ESC,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                          KC_J,    KC_L,    KC_U,    KC_Y,  NO_DQUO, NO_AE,
         KC_TAB,  KC_A,    KC_R,    KC_S,    KC_T,    KC_G,                          KC_M,    KC_N,    KC_E,    KC_I,     KC_O, NO_OSTR,
        KC_LSFT,  KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,                          KC_K,    KC_H, NO_COMM,  NO_DOT,  NO_SLSH, NO_ARNG,
                                            MO_NAV, SPC_NSYM, OSM_SFT,       OSM_SFT, ENT_NSYM, MO_FUN
    ),

    [_NUM_SYM] = LAYOUT_split_3x6_3(
        NO_CIRC, NO_EXLM, NO_QUES, NO_LBRC, NO_RBRC,   NO_AT,                        NO_ASTR, KC_7, KC_8, KC_9, NO_SLSH,  NO_DLR,
        NO_AMPR, NO_LABK, NO_RABK, NO_LPRN, NO_RPRN, NO_PIPE,                        NO_EQL,  KC_4, KC_5, KC_6, NO_MINS, NO_PERC,
        _______, NO_GRV,  NO_TILD, NO_LCBR, NO_RCBR, NO_HASH,                        KC_0,    KC_1, KC_2, KC_3, NO_PLUS, _______,
                                            _______, _______, _______,      _______, _______, _______
    ),

    [_NAV] = LAYOUT_split_3x6_3(
        _______,  KC_ESC, _______, _______, _______, _______,                        _______, _______, _______, _______, _______, _______,
        _______, KC_HOME,  KC_END, KC_PGUP, KC_PGDN, _______,                        _______, KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, _______,
        _______, KC_WBAK, KC_WFWD,  KC_INS,  KC_DEL, _______,                        _______, OSM_SFT, OSM_GUI, OSM_ALT, OSM_CTL, _______,
                                            _______, _______, _______,      _______, _______, _______
    ),

    [_FUN] = LAYOUT_split_3x6_3(
        ALT_TAB, _______, _______, _______, _______, _______,                        _______, KC_F7, KC_F8, KC_F9, KC_F12, _______,
        _______, KC_UNDO,  KC_CUT, KC_COPY, KC_PSTE, _______,                        _______, KC_F4, KC_F5, KC_F6, KC_F11, _______,
        _______, _______, _______, KC_FIND,  QK_REP, _______,                        _______, KC_F1, KC_F2, KC_F3, KC_F10, _______,
                                            _______, _______, _______,      _______, _______, _______
    ),

    [_MODS] = LAYOUT_split_3x6(
        _______, _______, _______, _______, _______, _______,                        _______, _______, _______, _______, _______, _______,
        _______, OSM_CTL, OSM_ALT, OSM_GUI, OSM_SFT, _______,                        _______, OSM_SFT, OSM_GUI, OSM_ALT, OSM_CTL, _______,
        _______, _______, _______, _______, _______, _______,                        _______, _______, _______, _______, _______, _______,
                                            _______, _______, _______,      _______, _______, _______
    )
};
