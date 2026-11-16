// Copyright 2023 Idan Kamara (@idank)
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Two modes, switched on layer CONFIG and remembered across power cycles (PDF):
//
//  PLAIN_KEYS     Own computers (Ergo-L layout installed in the OS, Arsenik running as Kanata/Karabiner).
//                 The keyboard acts like a plain 33 key board: QWERTY key positions, no tap-hold.
//                 Thumbs: Left Cmd/Meta, Space, Space, Right Cmd/Meta. Kanata/Karabiner turn them into
//                 Shift/Backspace, Nav and AltGr/Enter.
//
//  ERGOL_ARSENIK  Foreign Windows computers set to French AZERTY. The firmware does everything:
//                 it sends the AZERTY keycodes that print the Ergo-L letters, and implements
//                 Arsenik (home row mods, Nav, AltGr, Numbers and dead key layers).
//                 Characters AZERTY cannot type are sent as Windows Alt codes (needs NumLock, Windows only).
//
// Hold both inner thumbs together to reach layer CONFIG (mode switch, bootloader, trackpad buttons).

#include QMK_KEYBOARD_H
#include "users/holykeebs/holykeebs.h"
#include "keymap_french.h"

#define QK_C_EEPROM QK_CLEAR_EEPROM

enum layers {
    L_PLAIN_KEYS,
    L_ERGOL_ARSENIK,
    L_EA_NAV,
    L_CONFIG,
    L_EA_ALTGR,
    L_EA_NUM,
    L_EA_DEAD,
    L_EA_NAV_R, // Copy of L_EA_NAV. Gives the right inner thumb its own keycode, which the config combo needs.
};

enum custom_keycodes {
    SPC_R = QK_USER_0, // Space, but with its own keycode (PLAIN_KEYS right inner thumb)
    EL_MINS,           // Ergo-L - and ?
    EL_DOT,            // Ergo-L . and :
    EL_COMM,           // Ergo-L , and ;
    EL_DEAD,           // Ergo-L one dead key (and ! with Shift)
    EL_APOS,           // typographic apostrophe, after the dead key
    EL_NBS,            // no-break space
    LK_0,              // Keys of the AltGr, Numbers and dead key layers. Behavior depends on the active layer.
    LK_29 = LK_0 + 29,
};

// AZERTY has ^ on AltGr+9.
#define FR_CIRC_AG ALGR(FR_CCED)

// Home row mods. Ring finger is Alt, AltGr comes from the right thumb layer.
#define HM_A MT(MOD_LCTL, FR_A)
#define HM_S MT(MOD_LALT, FR_S)
#define HM_E MT(MOD_LGUI, FR_E)
#define HM_N MT(MOD_LSFT, FR_N)
#define HM_R MT(MOD_LSFT, FR_R)
#define HM_T MT(MOD_LGUI, FR_T)
#define HM_I MT(MOD_LALT, FR_I)
#define HM_U MT(MOD_LCTL, FR_U)

// Thumbs.
#define TH_SFT  MT(MOD_LSFT, KC_BSPC)       // hold Shift, tap Backspace
#define TH_SYM  LT(L_EA_ALTGR, KC_ENT)      // hold AltGr layer, tap Enter
#define TH_NAVL LT(L_EA_NAV, KC_SPC)        // hold Nav, tap Space
#define TH_NAVR LT(L_EA_NAV_R, KC_SPC)      // same, own keycode
#define TH_NUM  LT(L_EA_NUM, KC_ESC)        // in the AltGr layer: hold Numbers, tap Escape

// Navigation layer shortcuts (Windows, AZERTY positions).
#define NV_ALL   LCTL(FR_A)
#define NV_UNDO  LCTL(FR_Z)
#define NV_CUT   LCTL(FR_X)
#define NV_COPY  LCTL(FR_C)
#define NV_PASTE LCTL(FR_V)
#define NV_REDO  LCTL(FR_Y)

#define LAYOUT_wrapper(...) LAYOUT(__VA_ARGS__)

#define NAV_LAYER                                                                         \
    KC_TAB,  KC_HOME, KC_UP,   KC_END,  KC_PGUP,    S(KC_TAB), FR_7,  FR_8,    FR_9,    FR_SLSH, \
    NV_ALL,  KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN,    FR_MINS,   FR_4,  FR_5,    FR_6,    FR_0,    \
    NV_UNDO, NV_CUT,  NV_COPY, NV_PASTE, NV_REDO,   FR_COMM,   FR_1,  FR_2,    FR_3,    FR_DOT,  \
                          KC_DEL, KC_TRNS, KC_TRNS, KC_ESC

// Thirty layer-dependent keys plus the four thumbs t0..t3.
#define LK_LAYER(t0, t1, t2, t3)                                                                                     \
    LK_0,      LK_0 + 1,  LK_0 + 2,  LK_0 + 3,  LK_0 + 4,  LK_0 + 5,  LK_0 + 6,  LK_0 + 7,  LK_0 + 8,  LK_0 + 9,  \
    LK_0 + 10, LK_0 + 11, LK_0 + 12, LK_0 + 13, LK_0 + 14, LK_0 + 15, LK_0 + 16, LK_0 + 17, LK_0 + 18, LK_0 + 19, \
    LK_0 + 20, LK_0 + 21, LK_0 + 22, LK_0 + 23, LK_0 + 24, LK_0 + 25, LK_0 + 26, LK_0 + 27, LK_0 + 28, LK_0 + 29, \
    t0, t1, t2, t3

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // Own computers: Ergo-L and Arsenik come from the OS and from Kanata/Karabiner.
    [L_PLAIN_KEYS] = LAYOUT(
        KC_Q, KC_W, KC_E, KC_R, KC_T,    KC_Y, KC_U, KC_I,    KC_O,   KC_P,
        KC_A, KC_S, KC_D, KC_F, KC_G,    KC_H, KC_J, KC_K,    KC_L,   KC_SCLN,
        KC_Z, KC_X, KC_C, KC_V, KC_B,    KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH,
                       KC_LGUI, KC_SPC, SPC_R, KC_RGUI
    ),

    // Foreign Windows computers on AZERTY: Ergo-L letters and Arsenik in the firmware.
    [L_ERGOL_ARSENIK] = LAYOUT(
        FR_Q, FR_C, FR_O,    FR_P, FR_W,     FR_J, FR_M, FR_D, EL_DEAD, FR_Y,
        HM_A, HM_S, HM_E,    HM_N, FR_F,     FR_L, HM_R, HM_T, HM_I,    HM_U,
        FR_Z, FR_X, EL_MINS, FR_V, FR_B,     EL_DOT, FR_H, FR_G, EL_COMM, FR_K,
                              TH_SFT, TH_NAVL, TH_NAVR, TH_SYM
    ),

    [L_EA_NAV] = LAYOUT_wrapper(NAV_LAYER),

    [L_CONFIG] = LAYOUT(
        QK_BOOT,     HK_DUMP,     HK_SAVE,     HK_RESET,     HK_C_SCROLL,   PDF(L_PLAIN_KEYS), PDF(L_ERGOL_ARSENIK), KC_NO,   KC_NO,   QK_BOOT,
        QK_C_EEPROM, HK_P_SET_D,  HK_P_SET_S,  HK_P_SET_THR, HK_S_MODE_T,   MS_BTN1,           MS_BTN3,              MS_BTN2, MS_BTN4, QK_C_EEPROM,
        KC_NO,       KC_NO,       KC_NO,       KC_NO,        HK_D_MODE_T,   HK_D_MODE,         HK_S_MODE,            MS_BTN5, KC_NO,   KC_NO,
                                                    KC_NO, KC_NO, KC_NO, KC_NO
    ),

    [L_EA_ALTGR] = LAYOUT_wrapper(LK_LAYER(TH_NUM, KC_SPC, SPC_R, KC_TRNS)),
    [L_EA_NUM]   = LAYOUT_wrapper(LK_LAYER(KC_TRNS, EL_NBS, EL_NBS, KC_TRNS)),
    [L_EA_DEAD]  = LAYOUT_wrapper(LK_LAYER(KC_TRNS, EL_APOS, EL_APOS, KC_TRNS)),

    [L_EA_NAV_R] = LAYOUT_wrapper(NAV_LAYER),
};

// ---------------------------------------------------------------------------------------------
// Outputs for the AltGr, Numbers and dead key layers. Each key has a plain and a Shift output.
// ---------------------------------------------------------------------------------------------

typedef enum { O_NONE, O_KC, O_ALT, O_CIRC, O_DIAE, O_DEADSP } out_kind_t;

typedef struct {
    uint8_t  kind;
    uint16_t v;
} out_t;

typedef struct {
    out_t plain;
    out_t shifted;
} ent_t;

#define oNONE   {O_NONE, 0}
#define oKC(x)  {O_KC, (x)}     // tap a keycode on the AZERTY host
#define oALT(n) {O_ALT, (n)}    // Windows Alt code, decimal, Windows-1252
#define oCIR(x) {O_CIRC, (x)}   // circumflex dead key, then the letter x
#define oDIA(x) {O_DIAE, (x)}   // diaeresis dead key, then the letter x
#define oDSP(x) {O_DEADSP, (x)} // dead key x, then Space (gives the bare character)
#define ENT(p, s) {p, s}
#define SAME(p)   {p, p}

// AltGr layer. Ergo-L AltGr and AltGr+Shift levels. Characters outside Windows-1252 (<= >= != and dead
// keys) have no AZERTY output and send nothing.
static const ent_t ag_table[30] = {
    ENT(oKC(FR_CIRC_AG), oNONE), ENT(oKC(FR_LABK), oNONE),  ENT(oKC(FR_RABK), oNONE),  ENT(oKC(FR_DLR), oALT(164)), ENT(oKC(FR_PERC), oALT(137)),
    ENT(oKC(FR_AT), oNONE),      ENT(oKC(FR_AMPR), oNONE),  ENT(oKC(FR_ASTR), oALT(215)), ENT(oKC(FR_QUOT), oNONE), ENT(oDSP(FR_GRV), oNONE),

    ENT(oKC(FR_LCBR), oNONE),    ENT(oKC(FR_LPRN), oNONE),  ENT(oKC(FR_RPRN), oNONE),  ENT(oKC(FR_RCBR), oNONE),  ENT(oKC(FR_EQL), oNONE),
    ENT(oKC(FR_BSLS), oNONE),    ENT(oKC(FR_PLUS), oALT(177)), ENT(oKC(FR_MINS), oNONE), ENT(oKC(FR_SLSH), oALT(247)), ENT(oKC(FR_DQUO), oNONE),

    ENT(oDSP(FR_TILD), oNONE),   ENT(oKC(FR_LBRC), oNONE),  ENT(oKC(FR_RBRC), oNONE),  ENT(oKC(FR_UNDS), oALT(150)), ENT(oKC(FR_HASH), oNONE),
    ENT(oKC(FR_PIPE), oALT(166)), ENT(oKC(FR_EXLM), oALT(172)), ENT(oKC(FR_SCLN), oNONE), ENT(oKC(FR_COLN), oNONE), ENT(oKC(FR_QUES), oNONE),
};

// Numbers layer. Shift does not change anything here.
static const ent_t nm_table[30] = {
    SAME(oKC(FR_EURO)), SAME(oALT(171)), SAME(oALT(187)), SAME(oKC(FR_DLR)), SAME(oKC(FR_PERC)),
    SAME(oKC(FR_CIRC_AG)), SAME(oKC(FR_AMPR)), SAME(oKC(FR_ASTR)), SAME(oKC(FR_HASH)), SAME(oKC(FR_AT)),

    SAME(oKC(FR_1)), SAME(oKC(FR_2)), SAME(oKC(FR_3)), SAME(oKC(FR_4)), SAME(oKC(FR_5)),
    SAME(oKC(FR_6)), SAME(oKC(FR_7)), SAME(oKC(FR_8)), SAME(oKC(FR_9)), SAME(oKC(FR_0)),

    SAME(oALT(132)), SAME(oALT(147)), SAME(oALT(148)), SAME(oALT(162)), SAME(oALT(137)),
    SAME(oKC(FR_MINS)), SAME(oKC(FR_COMM)), SAME(oKC(FR_DOT)), SAME(oKC(FR_COLN)), SAME(oKC(FR_SLSH)),
};

// Dead key layer. One shot, reached with the Ergo-L dead key. Capitals use Alt codes.
static const ent_t dk_table[30] = {
    SAME(oCIR(FR_A)), ENT(oKC(FR_CCED), oALT(199)), ENT(oALT(156), oALT(140)), SAME(oCIR(FR_O)), ENT(oNONE, oNONE),
    ENT(oNONE, oNONE), ENT(oALT(181), oNONE), SAME(oKC(FR_UNDS)), ENT(oNONE, oALT(161)), SAME(oCIR(FR_U)),

    ENT(oKC(FR_AGRV), oALT(192)), ENT(oKC(FR_EACU), oALT(201)), ENT(oKC(FR_EGRV), oALT(200)), SAME(oCIR(FR_E)), ENT(oALT(241), oALT(209)),
    ENT(oKC(FR_LPRN), oNONE), ENT(oKC(FR_RPRN), oNONE), SAME(oCIR(FR_I)), SAME(oDIA(FR_I)), ENT(oKC(FR_UGRV), oALT(217)),

    ENT(oALT(230), oALT(198)), SAME(oALT(223)), ENT(oKC(FR_MINS), oALT(191)), ENT(oALT(150), oNONE), ENT(oALT(151), oNONE),
    ENT(oALT(133), oNONE), ENT(oNONE, oNONE), ENT(oNONE, oNONE), ENT(oALT(183), oALT(149)), ENT(oNONE, oNONE),
};

// Types a Windows Alt code: Alt held, 0 then three digits on the numpad. Turns NumLock on for it if needed.
static void alt_code(uint16_t n) {
    static const uint8_t pad[10] = {KC_KP_0, KC_KP_1, KC_KP_2, KC_KP_3, KC_KP_4, KC_KP_5, KC_KP_6, KC_KP_7, KC_KP_8, KC_KP_9};
    const bool numlock = host_keyboard_led_state().num_lock;
    if (!numlock) {
        tap_code(KC_NUM_LOCK);
    }
    register_code(KC_LALT);
    tap_code(KC_KP_0);
    tap_code(pad[(n / 100) % 10]);
    tap_code(pad[(n / 10) % 10]);
    tap_code(pad[n % 10]);
    unregister_code(KC_LALT);
    if (!numlock) {
        tap_code(KC_NUM_LOCK);
    }
}

static void send_out(out_t o, bool shifted) {
    switch (o.kind) {
        case O_KC:
            tap_code16(o.v);
            break;
        case O_ALT:
            alt_code(o.v);
            break;
        case O_CIRC:
            tap_code16(FR_CIRC);
            tap_code16(shifted ? S(o.v) : o.v);
            break;
        case O_DIAE:
            tap_code16(FR_DIAE);
            tap_code16(shifted ? S(o.v) : o.v);
            break;
        case O_DEADSP:
            tap_code16(o.v);
            tap_code(KC_SPC);
            break;
        default:
            break;
    }
}

// Sends the plain or the Shift output. The physical Shift is released while sending, so that the
// AZERTY keycodes are not shifted twice.
static void send_ent(ent_t e) {
    const uint8_t mods    = get_mods();
    const bool    shifted = (mods & MOD_MASK_SHIFT) != 0;
    del_mods(MOD_MASK_SHIFT);
    send_out(shifted ? e.shifted : e.plain, shifted);
    set_mods(mods);
    send_keyboard_report();
}

static bool dead_started = false;

bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
    if (keycode == SPC_R) {
        if (record->event.pressed) {
            register_code(KC_SPC);
        } else {
            unregister_code(KC_SPC);
        }
        return false;
    }

    if (keycode == EL_DEAD) {
        if (record->event.pressed) {
            if (get_mods() & MOD_MASK_SHIFT) {
                send_ent((ent_t)SAME(oKC(FR_EXLM)));
            } else {
                set_oneshot_layer(L_EA_DEAD, ONESHOT_START);
                dead_started = true;
            }
        } else if (dead_started) {
            // QMK clears ONESHOT_OTHER_KEY_PRESSED on this press (we return false below), so the layer
            // would turn off on release. Re-arm it so that it lasts until the next key or the timeout.
            // If another key was pressed while the dead key was held, the layer is already off.
            if (is_oneshot_layer_active()) {
                set_oneshot_layer(L_EA_DEAD, ONESHOT_OTHER_KEY_PRESSED);
            }
            dead_started = false;
        }
        return false;
    }

    if (keycode >= LK_0 && keycode <= LK_29) {
        if (record->event.pressed) {
            const ent_t *table = NULL;
            switch (get_highest_layer(layer_state)) {
                case L_EA_ALTGR: table = ag_table; break;
                case L_EA_NUM:   table = nm_table; break;
                case L_EA_DEAD:  table = dk_table; break;
                default: break;
            }
            if (table != NULL) {
                send_ent(table[keycode - LK_0]);
            }
        }
        return false;
    }

    if (record->event.pressed) {
        switch (keycode) {
            case EL_MINS: send_ent((ent_t)ENT(oKC(FR_MINS), oKC(FR_QUES))); return false;
            case EL_DOT:  send_ent((ent_t)ENT(oKC(FR_DOT), oKC(FR_COLN))); return false;
            case EL_COMM: send_ent((ent_t)ENT(oKC(FR_COMM), oKC(FR_SCLN))); return false;
            case EL_APOS: alt_code(146); return false;
            case EL_NBS:  alt_code(160); return false;
        }
    } else {
        switch (keycode) {
            case EL_MINS:
            case EL_DOT:
            case EL_COMM:
            case EL_APOS:
            case EL_NBS:
                return false;
        }
    }
    return true;
}

// ---------------------------------------------------------------------------------------------
// Config layer chord: both inner thumbs together. Each mode needs its own pair of keycodes,
// because a combo cannot use the same keycode twice.
// ---------------------------------------------------------------------------------------------
const uint16_t PROGMEM config_plain[]  = {KC_SPC, SPC_R, COMBO_END};
const uint16_t PROGMEM config_ergol[]  = {TH_NAVL, TH_NAVR, COMBO_END};
// Each half also opens CONFIG on its own (T1 + T2 on the left, T3 + T4 on the right), so that a single half
// plugged in alone can reach its Boot key.
const uint16_t PROGMEM config_plain_l[] = {KC_LGUI, KC_SPC, COMBO_END};
const uint16_t PROGMEM config_plain_r[] = {SPC_R, KC_RGUI, COMBO_END};
const uint16_t PROGMEM config_ergol_l[] = {TH_SFT, TH_NAVL, COMBO_END};
const uint16_t PROGMEM config_ergol_r[] = {TH_NAVR, TH_SYM, COMBO_END};

combo_t key_combos[] = {
    COMBO(config_plain, MO(L_CONFIG)),
    COMBO(config_ergol, MO(L_CONFIG)),
    COMBO(config_plain_l, MO(L_CONFIG)),
    COMBO(config_plain_r, MO(L_CONFIG)),
    COMBO(config_ergol_l, MO(L_CONFIG)),
    COMBO(config_ergol_r, MO(L_CONFIG)),
};

// ---------------------------------------------------------------------------------------------
// Timing, copied from the Kanata Arsenik settings: 250 ms, 170 ms for ring and index finger, 150 ms for thumbs.
// ---------------------------------------------------------------------------------------------
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HM_S:
        case HM_I:
        case HM_N:
        case HM_R:
            return 170;
        case TH_SFT:
        case TH_SYM:
        case TH_NUM:
            return 150;
        default:
            return TAPPING_TERM;
    }
}

// Thumbs act as modifiers or layers as soon as another key is pressed, like Kanata's tap-hold-press.
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TH_SFT:
        case TH_SYM:
        case TH_NUM:
            return true;
        default:
            return false;
    }
}
