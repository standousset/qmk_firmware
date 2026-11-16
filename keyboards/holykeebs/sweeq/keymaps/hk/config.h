// Copyright 2023 Idan Kamara (@idank)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// Timing for the ERGOL_ARSENIK mode, same as the Kanata Arsenik settings. Per key values are in keymap.c.
#define TAPPING_TERM 250
#define TAPPING_TERM_PER_KEY
#define QUICK_TAP_TERM 200
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY

// Both inner thumbs together open the config layer.
#define COMBO_TERM 35

// One shot dead key, same timeout as Kanata.
#define ONESHOT_TIMEOUT 1000

// Alt codes need a short delay between taps.
#define TAP_CODE_DELAY 10

// The keymap has 8 layers. Via keeps the keymap in EEPROM, so clear the EEPROM after flashing a new layer count.
#define DYNAMIC_KEYMAP_LAYER_COUNT 8
