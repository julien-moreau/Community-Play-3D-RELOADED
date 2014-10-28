/*****************************************************************************

        KeyCode.h
        Copyright (c) 2010 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 40489

        This file is part of Ohm Force Libraries.

        This program is free software; you can redistribute it and/or
        modify it under the terms of the GNU General Public License
        version 2 as published by the Free Software Foundation;

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program;
        if not, write to the Free Software Foundation, Inc.,
        59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*Tab=3***********************************************************************/



#if ! defined (ohm_opal_KeyCode_HEADER_INCLUDED)
#define  ohm_opal_KeyCode_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace opal
{



// physical keys layout
// name corresponds to the ANSI US layout
// only keys common to mac and pc are described
enum KeyCode
{
                  KeyCode_UNKNOWN = -1,
               
                  KeyCode_RETURN,
                  KeyCode_ESCAPE,
   
                  KeyCode_TAB,
                  KeyCode_SPACE,
                  KeyCode_BACKSPACE,
                  KeyCode_DEL,
                  
                  KeyCode_NUMERIC_KEYPAD_PLUS_SIGN,
                  KeyCode_NUMERIC_KEYPAD_MINUS_SIGN,
                  KeyCode_NUMERIC_KEYPAD_MULTIPLY_SIGN,
                  KeyCode_NUMERIC_KEYPAD_DIVIDE_SIGN,
//                KeyCode_NUMERIC_KEYPAD_EQUAL_SIGN,  // non existent on windows

//                KeyCode_NUMERIC_KEYPAD_ENTER, // not distinguishable on windows
                  KeyCode_NUMERIC_KEYPAD_PERIOD,
                  KeyCode_NUMERIC_KEYPAD_0,
                  KeyCode_NUMERIC_KEYPAD_1,
                  KeyCode_NUMERIC_KEYPAD_2,
                  KeyCode_NUMERIC_KEYPAD_3,
                  KeyCode_NUMERIC_KEYPAD_4,
                  KeyCode_NUMERIC_KEYPAD_5,
                  KeyCode_NUMERIC_KEYPAD_6,
                  KeyCode_NUMERIC_KEYPAD_7,
                  KeyCode_NUMERIC_KEYPAD_8,
                  KeyCode_NUMERIC_KEYPAD_9,

                  KeyCode_PAGE_UP,
                  KeyCode_PAGE_DOWN,
                  KeyCode_HOME,
                  KeyCode_END,
                  
                  KeyCode_LEFT_ARROW,
                  KeyCode_RIGHT_ARROW,
                  KeyCode_DOWN_ARROW,
                  KeyCode_UP_ARROW,

                  KeyCode_F1,
                  KeyCode_F2,
                  KeyCode_F3,
                  KeyCode_F4,
                  KeyCode_F5,
                  KeyCode_F6,
                  KeyCode_F7,
                  KeyCode_F8,
                  KeyCode_F9,
                  KeyCode_F10,
                  KeyCode_F11,
                  KeyCode_F12,
                  KeyCode_F13,
                  KeyCode_F14,
                  KeyCode_F15,
                  KeyCode_F16,
                  KeyCode_F17,
                  KeyCode_F18,
                  KeyCode_F19,
                  KeyCode_F20,

                  KeyCode_Q,
                  KeyCode_W,
                  KeyCode_E,
                  KeyCode_R,
                  KeyCode_T,
                  KeyCode_Y,
                  KeyCode_U,
                  KeyCode_I,
                  KeyCode_O,
                  KeyCode_P,
                  KeyCode_BRACKET_LEFT,
                  KeyCode_BRACKET_RIGHT,

                  KeyCode_A,
                  KeyCode_S,
                  KeyCode_D,
                  KeyCode_F,
                  KeyCode_G,
                  KeyCode_H,
                  KeyCode_J,
                  KeyCode_K,
                  KeyCode_L,
                  KeyCode_SEMICOLON,
                  KeyCode_QUOTE,
                  KeyCode_BACKSLASH,

//                KeyCode_GRAVE, // non existent on windows
                  KeyCode_Z,
                  KeyCode_X,
                  KeyCode_C,
                  KeyCode_V,
                  KeyCode_B,
                  KeyCode_N,
                  KeyCode_M,
                  KeyCode_COMMA,
                  KeyCode_PERIOD,
                  KeyCode_SLASH,

                  KeyCode_SECTION,  // ¤/@(french)/grave/square(french) (on pc)
                  KeyCode_1,
                  KeyCode_2,
                  KeyCode_3,
                  KeyCode_4,
                  KeyCode_5,
                  KeyCode_6,
                  KeyCode_7,
                  KeyCode_8,
                  KeyCode_9,
                  KeyCode_0,
                  KeyCode_MINUS,
                  KeyCode_EQUAL,
                  
                  KeyCode_NBR_ELT
};



}  // namespace opal
}  // namespace ohm



//#include  "ohm/opal/KeyCode.hpp"



#endif   // ohm_opal_KeyCode_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

