// Copyright 2022 beekeeb
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "keymap_norwegian.h"

#define MO_FUN MO(_FUN)
#define ESC_NAV LT(_NAV, KC_ESC)
#define SPC_NSYM LT(_NUM_SYM, KC_SPC)
#define SPC_NS_M LT(_NM_SM_M, KC_SPC)
#define ENT_NSYM LT(_NUM_SYM, KC_ENT)
#define ENT_NS_M LT(_NM_SM_M, KC_ENT)
#define BSP_MODS LT(_MODS, KC_BSPC)

#define OSM_SFT OSM(MOD_LSFT)

// Mac symbols
#define MC_LCBR LOPT(LSFT(KC_8))
#define MC_RCBR LOPT(LSFT(KC_9))
#define MC_PIPE LOPT(KC_7)
#define MC_QUOT NO_PIPE
#define MC_AT KC_BSLS
#define MC_DLR LSFT(KC_4)
#define MC_BSLS LOPT(LSFT(KC_7))

// Alpha mods
#define SFT_A LSFT_T(KC_A)
#define ALT_R LALT_T(KC_R)
#define GUI_S LGUI_T(KC_S)
#define CTRL_T LCTL_T(KC_T)
#define HYP_G HYPR_T(KC_G)

#define MEH_M MEH_T(KC_M)
#define CTL_N RCTL_T(KC_N)
#define GUI_E RGUI_T(KC_E)
#define ALT_I LALT_T(KC_I)
#define SFT_O RSFT_T(KC_O)

// Symbol mods

#define MD_LABK LSFT_T(NO_LABK)
#define MD_RABK LALT_T(NO_RABK)
#define MD_LPRN LGUI_T(NO_LPRN)
#define MD_RPRN LCTL_T(NO_RPRN)
#define MD_PIPE HYPR_T(NO_PIPE)
#define MDC_PIP HYPR_T(NO_PIPE)

#define MD_EQL MEH_T(NO_EQL)
#define MD_4 RCTL_T(KC_4)
#define MD_5 RGUI_T(KC_5)
#define MD_6 LALT_T(KC_6)
#define MD_MINS RSFT_T(NO_MINS)

// Nav mods
#define MD_HOME LSFT_T(KC_HOME)
#define MD_END LALT_T(KC_END)
#define MD_PGUP LGUI_T(KC_PGUP)
#define MD_PGDN LCTL_T(KC_PGDN)
#define MD_LEFT LCTL_T(KC_LEFT)
#define MD_DOWN LGUI_T(KC_DOWN)
#define MD_UP LALT_T(KC_UP)
#define MD_RGHT LSFT_T(KC_RGHT)

// TMUX mods
#define TMUX_D LT(0, KC_D)
#define TMUX_H LT(0, KC_H)
#define TMX_DEL LT(0, KC_DEL)
#define TMX_RCB LT(0, KC_RCBR)
#define MTMX_RC LT(0, MC_RCBR)
#define TMUX_1  LT(0, KC_1)

#define ko_make_basic_mac(mod_mask, trigger_, replacement_) \
    ((const key_override_t){                            \
        .trigger_mods       = (mod_mask),                 \
        .layers             = ~0,                       \
        .suppressed_mods    = (mod_mask),                 \
        .options            = ko_options_default,          \
        .negative_mod_mask  = 0,                        \
        .custom_action      = NULL,                       \
        .context            = NULL,                        \
        .trigger            = (trigger_),                  \
        .replacement        = (replacement_),              \
        .enabled            = (&user_config.mac_mode)     \
    })

#define ko_make_basic_win(mod_mask, trigger_, replacement_) \
    ((const key_override_t){                            \
        .trigger_mods       = (mod_mask),                 \
        .layers             = ~0,                       \
        .suppressed_mods    = (mod_mask),                 \
        .options            = ko_options_default,          \
        .negative_mod_mask  = 0,                        \
        .custom_action      = NULL,                       \
        .context            = NULL,                        \
        .trigger            = (trigger_),                  \
        .replacement        = (replacement_),              \
        .enabled            = (&user_config.win_mode)     \
    })

#define mac_win(trigger, mac, win, record)   \
    case trigger:                           \
        if (record->event.pressed) {               \
            send_string(user_config.mac_mode ? mac : win);    \
        }                                   \
        return false;

enum layers {
    _COLEMAK,
    _MAC_MOD,
    _NUM_SYM,
    _NM_SM_M, // Mac mode num/sym layer
    _NAV,
    _FUN,
    _MODS,
};

typedef union {
    uint32_t raw;
    struct {
        bool mac_mode :1;
    };
} user_config_t;

typedef struct user_config_mem_t {
    bool mac_mode;
    bool win_mode;
} user_config_mem_t;

user_config_t eep_config;
user_config_mem_t user_config;

void map_config_to_store(void) {
    eep_config.mac_mode = user_config.mac_mode;
}

void map_config_from_store(void) {
    user_config.mac_mode = eep_config.mac_mode;
    user_config.win_mode = !user_config.mac_mode;
}

void update_mac_mode(void) {
    if (user_config.mac_mode) {
        layer_on(_MAC_MOD);
    } else {
        layer_off(_MAC_MOD);
    }
}

void keyboard_post_init_user(void) {
    // Read user config from EEPROM
    eep_config.raw = eeconfig_read_user();
    map_config_from_store();

    update_mac_mode();
}


const key_override_t slash_mac = ko_make_basic_mac(MOD_MASK_SHIFT, NO_SLSH, MC_BSLS);
const key_override_t slash_win = ko_make_basic_win(MOD_MASK_SHIFT, NO_SLSH, NO_BSLS);
const key_override_t quot_win = ko_make_basic_win(MOD_MASK_SHIFT, NO_DQUO, NO_QUOT);
const key_override_t quot_mac = ko_make_basic_mac(MOD_MASK_SHIFT, NO_DQUO, NO_PIPE);

const key_override_t **key_overrides = (const key_override_t *[]) {
    &slash_mac,
    &slash_win,
    &quot_mac,
    &quot_win,
    NULL
};

bool is_alt_tab_active = false;
enum custom_keycodes {
    ALT_TAB = SAFE_RANGE,
    MAC_MODE,
    WIN_MODE,
};

const uint16_t PROGMEM mac_combo[] = {KC_ESC, KC_TAB, KC_LSFT, KC_M, COMBO_END};
const uint16_t PROGMEM win_combo[] = {KC_ESC, KC_TAB, KC_LSFT, KC_W, COMBO_END};
const uint16_t PROGMEM boot_combo[] = {KC_ESC, KC_TAB, KC_LSFT, KC_B, COMBO_END};
combo_t key_combos[] = {
    COMBO(mac_combo, MAC_MODE),
    COMBO(win_combo, WIN_MODE),
    COMBO(boot_combo, QK_BOOT),
};

layer_state_t layer_state_set_user(layer_state_t state) {
    if (is_alt_tab_active) {
        unregister_code(user_config.mac_mode ? KC_LGUI : KC_LALT);
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
                    register_code(user_config.mac_mode ? KC_LGUI : KC_LALT);
                }
                register_code(KC_TAB);
            } else {
                unregister_code(KC_TAB);
            }
            break;
        case MAC_MODE:
            if (record->event.pressed) {
                user_config.win_mode = false;
                user_config.mac_mode = true;
                map_config_to_store();
                eeconfig_update_user(eep_config.raw);
                update_mac_mode();
            }
        return false;
        case WIN_MODE:
            if (record->event.pressed) {
                user_config.win_mode = true;
                user_config.mac_mode = false;
                map_config_to_store();
                eeconfig_update_user(eep_config.raw);
                update_mac_mode();
            }
            return false;
        case MD_RABK:
            if (record->tap.count && record->event.pressed) {
                tap_code16(NO_RABK);
                return false;
            }
            break;
        case MD_LPRN:
            if (record->tap.count && record->event.pressed) {
                tap_code16(NO_LPRN);
                return false;
            }
            break;
        case MD_RPRN:
            if (record->tap.count && record->event.pressed) {
                tap_code16(NO_RPRN);
                return false;
            }
            break;
        case MD_EQL:
            if (record->tap.count && record->event.pressed) {
                tap_code16(NO_EQL);
                return false;
            }
            break;
        // TMUX
        // Note missing break in first case
        case TMX_RCB:
            if (record->tap.count && record->event.pressed) {
                tap_code16(NO_RCBR);
                return false;
            }
        case TMUX_D:
        case TMUX_H:
        case TMX_DEL:
            if (!record->tap.count && record->event.pressed) {
                tap_code16(C(KC_S));
                return false;
            }
            return true;
    }
    return true;
}

// Using CAGS-home-row-mods
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_COLEMAK] = LAYOUT_split_3x6_3(
         KC_ESC,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                          KC_J,    KC_L,    KC_U,    KC_Y,  NO_DQUO, NO_AE,
         KC_TAB, SFT_A,   ALT_R,   GUI_S,  CTRL_T,   HYP_G,                         MEH_M,   CTL_N,   GUI_E,   ALT_I,    SFT_O, NO_OSTR,
        KC_LSFT,  KC_Z,    KC_X,    KC_C,  TMUX_D,    KC_V,                          KC_K,  TMUX_H, NO_COMM,  NO_DOT,  NO_SLSH, NO_ARNG,
                                            ESC_NAV, SPC_NSYM, KC_BSPC,       OSM_SFT, ENT_NSYM, MO_FUN
    ),

    [_MAC_MOD] = LAYOUT_split_3x6_3(
        _______, _______, _______, _______, _______, _______,                        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                        _______, _______, _______, _______, _______, _______,
                                            _______, SPC_NS_M, _______,      _______, ENT_NS_M, _______
    ),

    [_NUM_SYM] = LAYOUT_split_3x6_3(
        NO_CIRC, NO_EXLM, NO_QUES, NO_LBRC, NO_RBRC,   NO_AT,                        NO_ASTR, KC_7, KC_8, KC_9, NO_SLSH,  NO_DLR,
        NO_AMPR, MD_LABK, MD_RABK, MD_LPRN, MD_RPRN, MD_PIPE,                        MD_EQL,  MD_4, MD_5, MD_6, MD_MINS, NO_PERC,
        _______, NO_GRV,  NO_TILD, NO_LCBR, TMX_RCB, NO_HASH,                        KC_0,    TMUX_1, KC_2, KC_3, NO_PLUS, _______,
                                            _______, _______, _______,      _______, _______, _______
    ),

    [_NM_SM_M] = LAYOUT_split_3x6_3(
        NO_CIRC, NO_EXLM, NO_QUES, NO_LBRC, NO_RBRC,   MC_AT,                        NO_ASTR, KC_7, KC_8, KC_9, NO_SLSH,  MC_DLR,
        NO_AMPR, MD_LABK, MD_RABK, MD_LPRN, MD_RPRN, MDC_PIP,                        MD_EQL,  MD_4, MD_5, MD_6, MD_MINS, NO_PERC,
        _______, NO_GRV,  NO_TILD, MC_LCBR, MTMX_RC, NO_HASH,                        KC_0,    TMUX_1, KC_2, KC_3, NO_PLUS, _______,
                                            _______, _______, _______,      _______, _______, _______
    ),

    [_NAV] = LAYOUT_split_3x6_3(
        _______, _______, _______, _______, _______, _______,                        _______,  _______, _______, _______, _______, _______,
        ALT_TAB, MD_HOME,  MD_END, MD_PGUP, MD_DOWN, _______,                        _______,  MD_LEFT, MD_DOWN,   MD_UP, MD_RGHT, _______,
        _______, KC_WBAK, KC_WFWD,  KC_INS, TMX_DEL, _______,                        _______,  _______, _______, _______, _______, _______,
                                            _______, _______, _______,      _______, _______, _______
    ),

    [_FUN] = LAYOUT_split_3x6_3(
        _______, KC_BSPC, _______, _______, KC_VOLD, KC_VOLU,                        _______, KC_F7, KC_F8, KC_F9, KC_F12, DT_UP,
        _______, _______, _______, _______, _______, _______,                        _______, KC_F4, KC_F5, KC_F6, KC_F11, DT_PRNT,
        _______, _______, _______, _______,  QK_REP, _______,                        _______, KC_F1, KC_F2, KC_F3, KC_F10, DT_DOWN,
                                            _______, _______, _______,      _______, _______, _______
    ),
};
