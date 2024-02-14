#pragma once

#include <stdint.h>
#include <iostream>

namespace zrn {

// from SDL_keycode.h
enum class ScanCode : uint32_t {

    UNKNOWN            = 0,

    A                  = 4,
    B                  = 5,
    C                  = 6,
    D                  = 7,
    E                  = 8,
    F                  = 9,
    G                  = 10,
    H                  = 11,
    I                  = 12,
    J                  = 13,
    K                  = 14,
    L                  = 15,
    M                  = 16,
    N                  = 17,
    O                  = 18,
    P                  = 19,
    Q                  = 20,
    R                  = 21,
    S                  = 22,
    T                  = 23,
    U                  = 24,
    V                  = 25,
    W                  = 26,
    X                  = 27,
    Y                  = 28,
    Z                  = 29,

    D1                 = 30,
    D2                 = 31,
    D3                 = 32,
    D4                 = 33,
    D5                 = 34,
    D6                 = 35,
    D7                 = 36,
    D8                 = 37,
    D9                 = 38,
    D0                 = 39,

    RETURN             = 40,
    ESCAPE             = 41,
    BACKSPACE          = 42,
    TAB                = 43,
    SPACE              = 44,

    MINUS              = 45,
    EQUALS             = 46,
    LEFTBRACKET        = 47,
    RIGHTBRACKET       = 48,
    BACKSLASH          = 49, /**< Located at the lower left of the return
                              *   key on ISO keyboards and at the right end
                              *   of the QWERTY row on ANSI keyboards.
                              *   Produces REVERSE SOLIDUS (backslash) and
                              *   VERTICAL LINE in a US layout, REVERSE
                              *   SOLIDUS and VERTICAL LINE in a UK Mac
                              *   layout, NUMBER SIGN and TILDE in a UK
                              *   Windows layout, DOLLAR SIGN and POUND SIGN
                              *   in a Swiss German layout, NUMBER SIGN and
                              *   APOSTROPHE in a German layout, GRAVE
                              *   ACCENT and POUND SIGN in a French Mac
                              *   layout, and ASTERISK and MICRO SIGN in a
                              *   French Windows layout.
                              */
    NONUSHASH          = 50, /**< ISO USB keyboards actually use this code
                              *   instead of 49 for the same key, but all
                              *   OSes I've seen treat the two codes
                              *   identically. So, as an implementor, unless
                              *   your keyboard generates both of those
                              *   codes and your OS treats them differently,
                              *   you should generate BACKSLASH
                              *   instead of this code. As a user, you
                              *   should not rely on this code because ZRN
                              *   will never generate it with most (all?)
                              *   keyboards.
                              */
    SEMICOLON          = 51,
    APOSTROPHE         = 52,
    GRAVE              = 53, /**< Located in the top left corner (on both ANSI
                              *   and ISO keyboards). Produces GRAVE ACCENT and
                              *   TILDE in a US Windows layout and in US and UK
                              *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                              *   and NOT SIGN in a UK Windows layout, SECTION
                              *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                              *   layouts on ISO keyboards, SECTION SIGN and
                              *   DEGREE SIGN in a Swiss German layout (Mac:
                              *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                              *   DEGREE SIGN in a German layout (Mac: only on
                              *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                              *   French Windows layout, COMMERCIAL AT and
                              *   NUMBER SIGN in a French Mac layout on ISO
                              *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                              *   SIGN in a Swiss German, German, or French Mac
                              *   layout on ANSI keyboards.
                              */
    COMMA              = 54,
    PERIOD             = 55,
    SLASH              = 56,

    CAPSLOCK           = 57,

    F1                 = 58,
    F2                 = 59,
    F3                 = 60,
    F4                 = 61,
    F5                 = 62,
    F6                 = 63,
    F7                 = 64,
    F8                 = 65,
    F9                 = 66,
    F10                = 67,
    F11                = 68,
    F12                = 69,

    PRINTSCREEN        = 70,
    SCROLLLOCK         = 71,
    PAUSE              = 72,
    INSERT             = 73, /**< insert on PC, help on some Mac keyboards (but
                                  does send code 73, not 117) */
    HOME               = 74,
    PAGEUP             = 75,
    DELETE             = 76,
    END                = 77,
    PAGEDOWN           = 78,
    RIGHT              = 79,
    LEFT               = 80,
    DOWN               = 81,
    UP                 = 82,

    NUMLOCKCLEAR       = 83, /**< num lock on PC, clear on Mac keyboards*/
    KP_DIVIDE          = 84,
    KP_MULTIPLY        = 85,
    KP_MINUS           = 86,
    KP_PLUS            = 87,
    KP_ENTER           = 88,
    KP_1               = 89,
    KP_2               = 90,
    KP_3               = 91,
    KP_4               = 92,
    KP_5               = 93,
    KP_6               = 94,
    KP_7               = 95,
    KP_8               = 96,
    KP_9               = 97,
    KP_0               = 98,
    KP_PERIOD          = 99,

    NONUSBACKSLASH     = 100, /**< This is the additional key that ISO
                               *   keyboards have over ANSI ones,
                               *   located between left shift and Y.
                               *   Produces GRAVE ACCENT and TILDE in a
                               *   US or UK Mac layout, REVERSE SOLIDUS
                               *   (backslash) and VERTICAL LINE in a
                               *   US or UK Windows layout, and
                               *   LESS-THAN SIGN and GREATER-THAN SIGN
                               *   in a Swiss German, German, or French
                               *   layout. */
    APPLICATION        = 101, /**< windows contextual menu, compose */
    POWER              = 102, /**< The USB document says this is a status flag,
                               *   not a physical key - but some Mac keyboards
                               *   do have a power key. */
    KP_EQUALS          = 103,
    F13                = 104,
    F14                = 105,
    F15                = 106,
    F16                = 107,
    F17                = 108,
    F18                = 109,
    F19                = 110,
    F20                = 111,
    F21                = 112,
    F22                = 113,
    F23                = 114,
    F24                = 115,
    EXECUTE            = 116,
    HELP               = 117, /**< AL Integrated Help Center */
    MENU               = 118, /**< Menu (show menu) */
    SELECT             = 119,
    STOP               = 120, /**< AC Stop */
    AGAIN              = 121, /**< AC Redo/Repeat */
    UNDO               = 122, /**< AC Undo */
    CUT                = 123, /**< AC Cut */
    COPY               = 124, /**< AC Copy */
    PASTE              = 125, /**< AC Paste */
    FIND               = 126, /**< AC Find */
    MUTE               = 127,
    VOLUMEUP           = 128,
    VOLUMEDOWN         = 129,
    
    KP_COMMA           = 133,
    KP_EQUALSAS400     = 134,

    INTERNATIONAL1     = 135, /**< used on Asian keyboards, see
                                            footnotes in USB doc */
    INTERNATIONAL2     = 136,
    INTERNATIONAL3     = 137, /**< Yen */
    INTERNATIONAL4     = 138,
    INTERNATIONAL5     = 139,
    INTERNATIONAL6     = 140,
    INTERNATIONAL7     = 141,
    INTERNATIONAL8     = 142,
    INTERNATIONAL9     = 143,
    LANG1              = 144, /**< Hangul/English toggle */
    LANG2              = 145, /**< Hanja conversion */
    LANG3              = 146, /**< Katakana */
    LANG4              = 147, /**< Hiragana */
    LANG5              = 148, /**< Zenkaku/Hankaku */
    LANG6              = 149, /**< reserved */
    LANG7              = 150, /**< reserved */
    LANG8              = 151, /**< reserved */
    LANG9              = 152, /**< reserved */

    ALTERASE           = 153, /**< Erase-Eaze */
    SYSREQ             = 154,
    CANCEL             = 155, /**< AC Cancel */
    CLEAR              = 156,
    PRIOR              = 157,
    RETURN2            = 158,
    SEPARATOR          = 159,
    OUT                = 160,
    OPER               = 161,
    CLEARAGAIN         = 162,
    CRSEL              = 163,
    EXSEL              = 164,

    KP_00              = 176,
    KP_000             = 177,
    THOUSANDSSEPARATOR = 178,
    DECIMALSEPARATOR   = 179,
    CURRENCYUNIT       = 180,
    CURRENCYSUBUNIT    = 181,
    KP_LEFTPAREN       = 182,
    KP_RIGHTPAREN      = 183,
    KP_LEFTBRACE       = 184,
    KP_RIGHTBRACE      = 185,
    KP_TAB             = 186,
    KP_BACKSPACE       = 187,
    KP_A               = 188,
    KP_B               = 189,
    KP_C               = 190,
    KP_D               = 191,
    KP_E               = 192,
    KP_F               = 193,
    KP_XOR             = 194,
    KP_POWER           = 195,
    KP_PERCENT         = 196,
    KP_LESS            = 197,
    KP_GREATER         = 198,
    KP_AMPERSAND       = 199,
    KP_DBLAMPERSAND    = 200,
    KP_VERTICALBAR     = 201,
    KP_DBLVERTICALBAR  = 202,
    KP_COLON           = 203,
    KP_HASH            = 204,
    KP_SPACE           = 205,
    KP_AT              = 206,
    KP_EXCLAM          = 207,
    KP_MEMSTORE        = 208,
    KP_MEMRECALL       = 209,
    KP_MEMCLEAR        = 210,
    KP_MEMADD          = 211,
    KP_MEMSUBTRACT     = 212,
    KP_MEMMULTIPLY     = 213,
    KP_MEMDIVIDE       = 214,
    KP_PLUSMINUS       = 215,
    KP_CLEAR           = 216,
    KP_CLEARENTRY      = 217,
    KP_BINARY          = 218,
    KP_OCTAL           = 219,
    KP_DECIMAL         = 220,
    KP_HEXADECIMAL     = 221,

    LCTRL              = 224,
    LSHIFT             = 225,
    LALT               = 226, /**< alt, option */
    LGUI               = 227, /**< windows, command (apple), meta */
    RCTRL              = 228,
    RSHIFT             = 229,
    RALT               = 230, /**< alt gr, option */
    RGUI               = 231, /**< windows, command (apple), meta */
};

#define ZRN_SCANCODE_MASK (1<<30)
#define ZRN_SCANCODE_TO_KEYCODE(x)  (static_cast<uint32_t>(ScanCode::x) | ZRN_SCANCODE_MASK)

typedef enum class KeyCode : uint32_t {
    Unknown         = 0,

    Return          = '\r',
    Escape          = '\x1B',
    Backspace       = '\b',
    Tab             = '\t',
    Space           = ' ',
    Exclaim         = '!',
    Quotedbl        = '"',
    Hash            = '#',
    Percent         = '%',
    Dollar          = '$',
    Ampersand       = '&',
    Quote           = '\'',
    LeftParen       = '(',
    RightParen      = ')',
    Asterisk        = '*',
    Plus            = '+',
    Comma           = ',',
    Minus           = '-',
    Period          = '.',
    Slash           = '/',
    D_0             = '0',
    D_1             = '1',
    D_2             = '2',
    D_3             = '3',
    D_4             = '4',
    D_5             = '5',
    D_6             = '6',
    D_7             = '7',
    D_8             = '8',
    D_9             = '9',
    Colon           = ':',
    Semicolon       = ';',
    Less            = '<',
    Equals          = '=',
    Greater         = '>',
    Question        = '?',
    At              = '@',
    
    /* Skip uppercase letters */

    LeftBracket     = '[',
    Backslash       = '\\',
    RightBracket    = ']',
    Caret           = '^',
    Underscore      = '_',
    Backquote       = '`',

    a               = 'a',
    b               = 'b',
    c               = 'c',
    d               = 'd',
    e               = 'e',
    f               = 'f',
    g               = 'g',
    h               = 'h',
    i               = 'i',
    j               = 'j',
    k               = 'k',
    l               = 'l',
    m               = 'm',
    n               = 'n',
    o               = 'o',
    p               = 'p',
    q               = 'q',
    r               = 'r',
    s               = 's',
    t               = 't',
    u               = 'u',
    v               = 'v',
    w               = 'w',
    x               = 'x',
    y               = 'y',
    z               = 'z',

    Capslock        = ZRN_SCANCODE_TO_KEYCODE(CAPSLOCK),

    F1              = ZRN_SCANCODE_TO_KEYCODE(F1),
    F2              = ZRN_SCANCODE_TO_KEYCODE(F2),
    F3              = ZRN_SCANCODE_TO_KEYCODE(F3),
    F4              = ZRN_SCANCODE_TO_KEYCODE(F4),
    F5              = ZRN_SCANCODE_TO_KEYCODE(F5),
    F6              = ZRN_SCANCODE_TO_KEYCODE(F6),
    F7              = ZRN_SCANCODE_TO_KEYCODE(F7),
    F8              = ZRN_SCANCODE_TO_KEYCODE(F8),
    F9              = ZRN_SCANCODE_TO_KEYCODE(F9),
    F10             = ZRN_SCANCODE_TO_KEYCODE(F10),
    F11             = ZRN_SCANCODE_TO_KEYCODE(F11),
    F12             = ZRN_SCANCODE_TO_KEYCODE(F12),

    Printscreen     = ZRN_SCANCODE_TO_KEYCODE(PRINTSCREEN),
    Scrolllock      = ZRN_SCANCODE_TO_KEYCODE(SCROLLLOCK),
    Pause           = ZRN_SCANCODE_TO_KEYCODE(PAUSE),
    Insert          = ZRN_SCANCODE_TO_KEYCODE(INSERT),
    Home            = ZRN_SCANCODE_TO_KEYCODE(HOME),
    Pageup          = ZRN_SCANCODE_TO_KEYCODE(PAGEUP),
    Delete          = '\x7F',
    End             = ZRN_SCANCODE_TO_KEYCODE(END),
    Pagedown        = ZRN_SCANCODE_TO_KEYCODE(PAGEDOWN),
    Right           = ZRN_SCANCODE_TO_KEYCODE(RIGHT),
    Left            = ZRN_SCANCODE_TO_KEYCODE(LEFT),
    Down            = ZRN_SCANCODE_TO_KEYCODE(DOWN),
    Up              = ZRN_SCANCODE_TO_KEYCODE(UP),

    NumlockClear    = ZRN_SCANCODE_TO_KEYCODE(NUMLOCKCLEAR),
    KP_Divide       = ZRN_SCANCODE_TO_KEYCODE(KP_DIVIDE),
    KP_Multiply     = ZRN_SCANCODE_TO_KEYCODE(KP_MULTIPLY),
    KP_Minus        = ZRN_SCANCODE_TO_KEYCODE(KP_MINUS),
    KP_Plus         = ZRN_SCANCODE_TO_KEYCODE(KP_PLUS),
    KP_Enter        = ZRN_SCANCODE_TO_KEYCODE(KP_ENTER),
    KP_1            = ZRN_SCANCODE_TO_KEYCODE(KP_1),
    KP_2            = ZRN_SCANCODE_TO_KEYCODE(KP_2),
    KP_3            = ZRN_SCANCODE_TO_KEYCODE(KP_3),
    KP_4            = ZRN_SCANCODE_TO_KEYCODE(KP_4),
    KP_5            = ZRN_SCANCODE_TO_KEYCODE(KP_5),
    KP_6            = ZRN_SCANCODE_TO_KEYCODE(KP_6),
    KP_7            = ZRN_SCANCODE_TO_KEYCODE(KP_7),
    KP_8            = ZRN_SCANCODE_TO_KEYCODE(KP_8),
    KP_9            = ZRN_SCANCODE_TO_KEYCODE(KP_9),
    KP_0            = ZRN_SCANCODE_TO_KEYCODE(KP_0),
    KP_Period       = ZRN_SCANCODE_TO_KEYCODE(KP_PERIOD),

    LeftControl     = ZRN_SCANCODE_TO_KEYCODE(LCTRL),
    LeftShift       = ZRN_SCANCODE_TO_KEYCODE(LSHIFT),
    LeftAlt         = ZRN_SCANCODE_TO_KEYCODE(LALT),
    LeftSuper       = ZRN_SCANCODE_TO_KEYCODE(LGUI),
    RightControl    = ZRN_SCANCODE_TO_KEYCODE(RCTRL),
    RightShift      = ZRN_SCANCODE_TO_KEYCODE(RSHIFT),
    RightAlt        = ZRN_SCANCODE_TO_KEYCODE(RALT),
    RightSuper      = ZRN_SCANCODE_TO_KEYCODE(RGUI),
} Key;

enum class KeyState {
    None = -1,
    Pressed,
    Held,
    Released
};

typedef enum KeyMod {
    LShift  = 0x0001,
    RShift  = 0x0002,
    LCtrl   = 0x0040,
    RCtrl   = 0x0080,
    LAlt    = 0x0100,
    RAlt    = 0x0200,
    LSuper  = 0x0400,
    RSuper  = 0x0800,
    Num     = 0x1000,
    Caps    = 0x2000,
    Mode    = 0x4000,
    Scroll  = 0x8000,

    Ctrl    = LCtrl  | RCtrl,
    Shift   = LShift | RShift,
    Alt     = LAlt   | RAlt,
    Super   = LSuper | RSuper,
} Mod;

inline std::ostream& operator<<(std::ostream& os, KeyCode keycode) {
    os << static_cast<int32_t>(keycode);
    return os;
}

} // namespace zrn