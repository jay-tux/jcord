#ifndef JCORD_KEYBINDS_H
#define JCORD_KEYBINDS_H

#include <ncurses.h>

#define BIND_NO_KEY (-1)

//FOR COMMON KEY VALUES, SEE BELOW
//bindings can be either integers (see the list)
//or ncurses macros

//direct focus
#define BIND_FOCUS_SERVER   'z'
#define BIND_FOCUS_CHANNEL  'x'
#define BIND_FOCUS_TYPE     'c'
#define BIND_FOCUS_MEMBERS  'f'
#define BIND_FOCUS_MESSAGES 'e'

//direct focus, alts
#define BIND_FOCUS_SERVER_ALT   BIND_NO_KEY
#define BIND_FOCUS_CHANNEL_ALT  BIND_NO_KEY
#define BIND_FOCUS_TYPE_ALT     BIND_NO_KEY
#define BIND_FOCUS_MEMBERS_ALT  BIND_NO_KEY
#define BIND_FOCUS_MESSAGES_ALT BIND_NO_KEY

//indirect focus
#define BIND_FOCUS_UP    KEY_UP
#define BIND_FOCUS_DOWN  KEY_DOWN
#define BIND_FOCUS_LEFT  KEY_LEFT
#define BIND_FOCUS_RIGHT KEY_RIGHT

//indirect focus, alts
#define BIND_FOCUS_UP_ALT     'w'
#define BIND_FOCUS_DOWN_ALT   's'
#define BIND_FOCUS_LEFT_ALT   'a'
#define BIND_FOCUS_RIGHT_ALT  'd'

//actions
#define BIND_ACTION_QUIT         4   //CTRL-D
#define BIND_ACTION_ACT        '\n'  //RETURN
#define BIND_ACTION_EXIT_POPUP  'q'  //q key
#define BIND_ACTION_INPUT_MODE  'i'  //i key (like vim)
#define BIND_ACTION_EXIT_INPUT   9   //CTRL-I

//actions, alts
#define BIND_ACTION_QUIT_ALT        BIND_NO_KEY
#define BIND_ACTION_ACT_ALT         ' '
#define BIND_ACTION_EXIT_POPUP_ALT  BIND_NO_KEY
#define BIND_ACTION_INPUT_MODE_ALT  'r'
#define BIND_ACTION_EXIT_INPUT_ALT  18 //CTRL-R

//input mode movement
#define BIND_INPUT_FORWARD  KEY_RIGHT
#define BIND_INPUT_BACKWARD KEY_LEFT
#define BIND_INPUT_BEGIN    KEY_UP
#define BIND_INPUT_END      KEY_DOWN
#define BIND_INPUT_SEND     KEY_ENTER

//input mode movement, alts
#define BIND_INPUT_FORWARD_ALT  BIND_NO_KEY
#define BIND_INPUT_BACKWARD_ALT BIND_NO_KEY
#define BIND_INPUT_BEGIN_ALT    BIND_NO_KEY
#define BIND_INPUT_END_ALT      BIND_NO_KEY
#define BIND_INPUT_SEND_ALT     '\n'        //key_enter not always caught?

#endif

//for full key bindings, see the ncurses_keys file

/*
        +===========================+
        |   COMMON KEY BINDINGS     |
        +===========================+
      A   -       Z ->  97 - 122 (character values for 'a' - 'z')
SHIFT A   - SHIFT Z ->  65 -  90 (character values for 'A' - 'Z')
CTRL  A   - CTRL  Z ->   1 -  26
WIN   any           ->  24
ALT   any           ->  27
      0   -       9 ->  48 -  57 (character values for '0' - '9')
      F1  -     F12 -> 264 - 274 (or KEY_F(<number>))
     DOWN -   RIGHT -> 258 - 261 (DN - UP - LT - RT)

ENTER: 343

For other bindings, please refer to the ncurses_keys file
*/
