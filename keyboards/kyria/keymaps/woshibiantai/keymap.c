#include QMK_KEYBOARD_H

enum layers {
    _QWERTY = 0,
    _LOWER,
    _RAISE,
    _ADJUST,
    _WIN
};

// Tap Dance keycodes
enum td_keycodes {
    TAB_LBRC,  // `[` when held, `TAB` when tapped
    QUOT_RBRC, // `]` when held, `'` when tapped
    ESC_LCBR,  // `{` when held, `ESC` when tapped
    BSLS_RCBR, // `}` when held, `\` when tapped
};

// Define a type containing as many tapdance states as you need
typedef enum {
    SINGLE_TAP,
    SINGLE_HOLD,
    DOUBLE_TAP,
    DOUBLE_HOLD
} td_state_t;

// Create a global instance of the tapdance state type
static td_state_t td_state;

// Declare your tapdance functions:

// Function to determine the current tapdance state
uint8_t cur_dance(qk_tap_dance_state_t *state);

// `finished` and `reset` functions for each tapdance keycode
void tablbrc_finished(qk_tap_dance_state_t *state, void *user_data);
void tablbrc_reset(qk_tap_dance_state_t *state, void *user_data);
void quotrbrc_finished(qk_tap_dance_state_t *state, void *user_data);
void quotrbrc_reset(qk_tap_dance_state_t *state, void *user_data);
void esclcbr_finished(qk_tap_dance_state_t *state, void *user_data);
void esclcbr_reset(qk_tap_dance_state_t *state, void *user_data);
void bslsrcbr_finished(qk_tap_dance_state_t *state, void *user_data);
void bslsrcbr_reset(qk_tap_dance_state_t *state, void *user_data);

bool is_app_switching = false;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: QWERTY
 *
 * ,-------------------------------------------.                                   ,-------------------------------------------.
 * |  ESC { |   Q  |   W  |   E  |   R  |   T  |                                   |   Y  |   U  |   I  |   O  |   P  |  \ | } |
 * |--------+------+------+------+------+------|                                   |------+------+------+------+------+--------|
 * |  TAB [ |   A  |   S  |   D  |   F  |   G  |                                   |   H  |   J  |   K  |   L  |  ; : |  ' " ] |
 * |--------+------+------+------+------+------+-----------------.  ,--------------+------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  |Window|          |  |  Tab |       |   N  |   M  |  , < |  . > |  / ? |  Bksp  |
 * `----------------------+------+------+------+------+----------|  |------+-------+------+------+------+----------------------'
 *                        | Mute | Lower|  Alt | GUI  |Ctrl Enter|  | Enter| Space | Raise| Lower| Play |
 *                        |      |      |      |      |          |  |      |       |      |      |      |
 *                        `--------------------------------------'  `-----------------------------------'
 */
    [_QWERTY] = LAYOUT(
      TD(ESC_LCBR),  KC_Q, KC_W,    KC_E,   KC_R,        KC_T,                                   KC_Y,    KC_U,    KC_I,      KC_O,     KC_P,    TD(BSLS_RCBR),
      TD(TAB_LBRC),  KC_A, KC_S,    KC_D,   KC_F,        KC_G,                                   KC_H,    KC_J,    KC_K,      KC_L,     KC_SCLN, TD(QUOT_RBRC),
      KC_LSFT, KC_Z, KC_X, KC_C,    KC_V,   KC_B,        MO(_WIN), _______,     KC_TAB, _______, KC_N,    KC_M,    KC_COMM,   KC_DOT,   KC_SLSH, KC_BSPC,
                        KC_MUTE, MO(_LOWER), KC_LALT, KC_LGUI, MT(MOD_LCTL, KC_ENT),  MT(MOD_LCTL, KC_ENT), KC_SPC, MO(_RAISE), MO(_LOWER), KC_MPLY
    ),

/*
 * Lower Layer: Numbers/Symbols
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |    ~   |   1  |   2  |   3  |   4  |   5  |                              |   6  |   7  |   8  |   9  |   0  |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |    `   |   !  |   @  |   #  |   $  |   %  |                              |   ^  |   &  |   *  |   (  |   )  |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |   -  |   _  |   =  |   +  |      |      |      |  |      |      |  {/[ |  }/] |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_LOWER] = LAYOUT(
      KC_TILDE,   KC_1, KC_2,    KC_3,    KC_4,    KC_5,                                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0, _______,
      KC_GRV,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR, KC_PERC,                                      KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______,
      _______, KC_MINS, KC_UNDS, KC_EQL,  KC_PLUS, _______, _______, _______, _______, _______, KC_LBRC, KC_RBRC, _______, _______, _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

/*
 * Raise Layer: Function keys
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  F1  |  F2  |  F3  |  F4  |  F5  |                              |  F6  |  F7  |  F8  |  F9  |  F10 |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      | Prev | Play | Next |  F11 |                              |  F12 | Left |  Up  | Right|      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |  CAPS  |      |      |      |      |      |      |      |  |      |      | PSCR |      | Down |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_RAISE] = LAYOUT(
      _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                            KC_F6,   KC_F7,   KC_F8,    KC_F9,  KC_F10, _______,
      _______, _______, KC_MPRV, KC_MPLY, KC_MNXT, KC_F11,                                          KC_F12, KC_LEFT,   KC_UP, KC_RIGHT, _______, _______,
      KC_CAPS, _______, _______, _______, _______, _______, _______, _______, _______, _______, SGUI(KC_4), _______, KC_DOWN,  _______, _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

/*
 * Adjust Layer: RGB
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        | TOG  | SAI  | HUI  | VAI  | MOD  |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      | SAD  | HUD  | VAD  | RMOD |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_ADJUST] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
      _______, RGB_TOG, RGB_SAI, RGB_HUI, RGB_VAI, RGB_MOD,                                     _______, _______, _______, _______, _______, _______,
      _______, _______, RGB_SAD, RGB_HUD, RGB_VAD, RGB_RMOD,_______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

/*
 * Window Layer: Jump between tabs and desktops
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  T1  |  T2  |  T3  |  T4  |  T5  |                              |  T6  |  T7  |  T8  |  T9  |  T10 |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  D1  |  D2  |  D3  |  D4  |  D5  |                              |  D6  |  D7  |  D8  |  D9  |  D10 |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_WIN] = LAYOUT(
      LGUI(KC_GRV), LGUI(KC_1), LGUI(KC_2), LGUI(KC_3), LGUI(KC_4), LGUI(KC_5),                LGUI(KC_6), LGUI(KC_7), LGUI(KC_8), LGUI(KC_9), LGUI(KC_0), _______,
      LCTL(KC_GRV), LCTL(KC_1), LCTL(KC_2), LCTL(KC_3), LCTL(KC_4), LCTL(KC_5),                LCTL(KC_6), LCTL(KC_7), LCTL(KC_8), LCTL(KC_9), LCTL(KC_0), _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
};

void keyboard_post_init_user(void) {
  rgblight_sethsv(183, 155, 196);
}

layer_state_t layer_state_set_user(layer_state_t state) { return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST); }

// Determine the tapdance state to return
uint8_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return SINGLE_TAP;
        else return SINGLE_HOLD;
    }

    if (state->count == 2) {
        if (state->interrupted || !state->pressed) return DOUBLE_TAP;
        else return DOUBLE_HOLD;
    }
    else return 3; // Any number higher than the maximum state value you return above
}

// Handle the possible states for each tapdance keycode you define:

void tablbrc_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case SINGLE_HOLD:
            register_code16(KC_LBRC);
            break;
        case DOUBLE_TAP:
            tap_code16(KC_TAB);
            register_code16(KC_TAB);
            break;
        case DOUBLE_HOLD: // Allow nesting of 2 `[[` within tapping term
            tap_code16(KC_LBRC);
            register_code16(KC_LBRC);
            break;
        default:
            register_code16(KC_TAB);
            break;
    }
}

void tablbrc_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case SINGLE_HOLD:
            unregister_code16(KC_LBRC);
            break;
        case DOUBLE_TAP:
            unregister_code16(KC_TAB);
            break;
        case DOUBLE_HOLD:
            unregister_code16(KC_LBRC);
            break;
        default:
            unregister_code16(KC_TAB);
            break;
    }
}

void quotrbrc_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case SINGLE_HOLD:
            register_code16(KC_RBRC);
            break;
        case DOUBLE_TAP:
            tap_code16(KC_QUOT);
            register_code16(KC_QUOT);
            break;
        case DOUBLE_HOLD: // Allow nesting of 2 `]]` within tapping term
            tap_code16(KC_RBRC);
            register_code16(KC_RBRC);
            break;
        default:
            register_code16(KC_QUOT);
            break;
    }
}

void quotrbrc_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case SINGLE_HOLD:
            unregister_code16(KC_RBRC);
            break;
        case DOUBLE_TAP:
            unregister_code16(KC_QUOT);
            break;
        case DOUBLE_HOLD:
            unregister_code16(KC_RBRC);
            break;
        default:
            unregister_code16(KC_QUOT);
            break;
    }
}

void esclcbr_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case SINGLE_HOLD:
            register_code16(KC_LCBR);
            break;
        case DOUBLE_TAP:
            tap_code16(KC_ESC);
            register_code16(KC_ESC);
            break;
        case DOUBLE_HOLD: // Allow nesting of 2 `{{` within tapping term
            tap_code16(KC_LCBR);
            register_code16(KC_LCBR);
            break;
        default:
            register_code16(KC_ESC);
            break;
    }
}

void esclcbr_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case SINGLE_HOLD:
            unregister_code16(KC_LCBR);
            break;
        case DOUBLE_TAP:
            unregister_code16(KC_ESC);
            break;
        case DOUBLE_HOLD:
            unregister_code16(KC_LCBR);
            break;
        default:
            unregister_code16(KC_ESC);
            break;
    }
}

void bslsrcbr_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case SINGLE_HOLD:
            register_code16(KC_RCBR);
            break;
        case DOUBLE_TAP:
            tap_code16(KC_BSLS);
            register_code16(KC_BSLS);
            break;
        case DOUBLE_HOLD: // Allow nesting of 2 `}}` within tapping term
            tap_code16(KC_RCBR);
            register_code16(KC_RCBR);
            break;
        default:
            register_code16(KC_BSLS);
            break;
    }
}

void bslsrcbr_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case SINGLE_HOLD:
            unregister_code16(KC_RCBR);
            break;
        case DOUBLE_TAP:
            unregister_code16(KC_BSLS);
            break;
        case DOUBLE_HOLD:
            unregister_code16(KC_RCBR);
            break;
        default:
            unregister_code16(KC_BSLS);
            break;
    }
}

// Define `ACTION_TAP_DANCE_FN_ADVANCED()` for each tapdance keycode, passing in `finished` and `reset` functions
qk_tap_dance_action_t tap_dance_actions[] = {
    [TAB_LBRC] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, tablbrc_finished, tablbrc_reset),
    [QUOT_RBRC] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, quotrbrc_finished, quotrbrc_reset),
    [ESC_LCBR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, esclcbr_finished, esclcbr_reset),
    [BSLS_RCBR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, bslsrcbr_finished, bslsrcbr_reset),
};

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_180; }

static void render_status(void) {
    oled_write_P(PSTR("ruth.w\n"), false);

    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
            oled_write_P(PSTR("qwerty\n"), false);
            break;
        case _LOWER:
            oled_write_P(PSTR("num/symbols\n"), false);
            break;
        case _RAISE:
            oled_write_P(PSTR("function\n"), false);
            break;
        case _ADJUST:
            oled_write_P(PSTR("adjust\n"), false);
            break;
        case _WIN:
            oled_write_P(PSTR("workspaces\n"), false);
            break;
        default:
            oled_write_P(PSTR("undefined\n"), false);
    }

    // Host Keyboard LED Status
    uint8_t led_usb_state = host_keyboard_leds();
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_NUM_LOCK) ? PSTR("NUMLCK ") : PSTR("       "), false);
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_CAPS_LOCK) ? PSTR("CAPLCK ") : PSTR("       "), false);
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_SCROLL_LOCK) ? PSTR("SCRLCK ") : PSTR("       "), false);
}

void oled_task_user(void) {
    if (is_keyboard_master()) {
        render_status();  // Renders the current keyboard state (layer, lock, caps, scroll, etc)
    }
}
#endif

#ifdef ENCODER_ENABLE
void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        // Volume control
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }
    else if (index == 1) {
        if (is_app_switching) {
            // Application switching
            if (clockwise) {
                tap_code16(KC_TAB);
            } else {
                tap_code16(S(KC_TAB));
            }
        } else {
            // Page up/Page down
            if (clockwise) {
                tap_code(KC_PGDN);
            } else {
                tap_code(KC_PGUP);
            }
        }
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_LGUI:
            if (record->event.pressed) {
                is_app_switching = true;
            } else {
                is_app_switching = false;
            }
            break;
    }
    return true;
}
#endif
