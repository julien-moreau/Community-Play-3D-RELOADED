/*****************************************************************************

        Key.cpp
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70945

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



#if defined (_MSC_VER)
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/Key.h"
#include "ohm/lang/fnc.h"
#include "ohm/util/BinaryStreamInput.h"
#include "ohm/util/BinaryStreamOutputBase.h"
#include "ohm/util/SingletonBase.h"
#include "ohm/util/trace.h"

#include <map>
#include <cassert>

#if  (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   #include "ohm/opal/carbon/EventFnc.h"
   #include <AppKit/NSEvent.h>
#endif

namespace ohm
{
namespace opal
{



// private singleton

class Key_KeyMap
:  public util::SingletonBase <Key_KeyMap>
{
public:

   virtual     ~Key_KeyMap ();

   typedef std::map <archi::UInt16, KeyCode> ScanCodeMap;

   static archi::UInt16
               _scan_code_arr [KeyCode_NBR_ELT];

   ScanCodeMap _scan_code_map;



private:

   ohm_util_SingletonBase_INVITE_SINGLETON (Key_KeyMap);

               Key_KeyMap ();
};



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

Key::Key (bool shift_flag, bool alt_flag, bool command_flag)
:  _shift_flag (shift_flag)
,  _alt_flag (alt_flag)
,  _command_flag (command_flag)
,  _key_code (KeyCode_UNKNOWN)
,  _key_char (KeyChar_NONE)
{
}


/*
==============================================================================
Name : ctor
Note :
   should be created on the fly (not be stored)
   '_key_code' will depend on the language
==============================================================================
*/

Key::Key (KeyChar key_char)
:  _shift_flag (false)
,  _alt_flag (false)
,  _command_flag (false)
,  _key_code (KeyCode_UNKNOWN)
,  _key_char (KeyChar_NONE)
{
   assert (key_char != KeyChar_NONE);

#if 1 // (ohm_archi_SYS == ohm_archi_SYS_MACOS)

   _key_char = key_char;

#elif (ohm_archi_SYS == ohm_archi_SYS_WIN)

   static ::HKL h_kl = ::LoadKeyboardLayout ("00000409", 0);
   assert (h_kl != 0);

   // note : constants are not defined
   // #define MAPVK_VK_TO_VSC 0
   // #define MAPVK_VSC_TO_VK 1
   // #define MAPVK_VK_TO_CHAR 2
   // #define MAPVK_VSC_TO_VK_EX 3

   ::UINT scan_code = ::MapVirtualKey ('A' + UINT (key_char), 0/*MAPVK_VK_TO_VSC*/);
   assert (scan_code != 0);
   ::UINT us_vk = ::MapVirtualKeyEx (scan_code, 1/*MAPVK_VSC_TO_VK*/, h_kl);
   assert (us_vk != 0);

   Key_KeyMap::ScanCodeMap & scan_code_map = Key_KeyMap::use_instance ()._scan_code_map;
   Key_KeyMap::ScanCodeMap::iterator it = scan_code_map.find (us_vk);

   if (it != scan_code_map.end ())
   {
      _key_code = it->second;
      _key_char = key_char;
   }
   else
   {
      // Key will be equal to Key::None
      ohm_util_TRACE_ERROR1 ("Could not build opal::Key for char %1%", 'A' + char (key_char));
   }

#else

   #error architecture not supported

#endif // ohm_archi_SYS
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Key::Key (KeyCode key_code)
:  _shift_flag (false)
,  _alt_flag (false)
,  _command_flag (false)
,  _key_code (key_code)
,  _key_char (KeyChar_NONE)
{
}



#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)

/*
==============================================================================
Name : ctor
==============================================================================
*/

Key::Key (::EventRef event_ref)
:  _shift_flag (false)
,  _alt_flag (false)
,  _command_flag (false)
,  _key_code (KeyCode_UNKNOWN)
,  _key_char (KeyChar_NONE)
{
// EventFnc::use_instance ().print_key (event_ref);
   OSStatus err;
   
   // fill the modifiers
   
   {
      ::UInt32 modifiers;
      err = GetEventParameter (
         event_ref,
         kEventParamKeyModifiers,
         typeUInt32,
         0,
         sizeof (::UInt32),
         0,
         &modifiers
      );
      
      if (err != 0)
      {
         modifiers = GetCurrentKeyModifiers ();
      }
      
      _shift_flag = modifiers & shiftKey;
      _alt_flag = modifiers & optionKey;
      _command_flag = modifiers & cmdKey;
   }
   
   if (GetEventKind (event_ref) != kEventRawKeyModifiersChanged)
   {
      // Set the KeyCode value

      ::UInt32 scan_code;
      err = GetEventParameter (
         event_ref,
         kEventParamKeyCode,
         typeUInt32,
         0,
         sizeof (::UInt32),
         0,
         &scan_code
      );

      if (err == 0)
      {
         Key_KeyMap::ScanCodeMap & scan_code_map = Key_KeyMap::use_instance ()._scan_code_map;
         Key_KeyMap::ScanCodeMap::iterator it = scan_code_map.find (scan_code);
         
         if (it != scan_code_map.end ())
         {
            _key_code = it->second;
         }
         
         // If it is an alpha char ([a-zA-Z]) set the KeyChar value
         
         char ascii_char = EventFnc::use_instance ().get_ascii_for_scan_code (scan_code);
         
         if (ascii_char >= 'a' && ascii_char <= 'z')
         {
            _key_char = KeyChar (ascii_char - 'a');
         }
         else if (ascii_char >= 'A' && ascii_char <= 'Z')
         {
            _key_char = KeyChar (ascii_char - 'A');
         }
      }
   }
}

#endif // (ohm_archi_SYS == ohm_archi_SYS_MACOS)


#if (ohm_archi_SYS == ohm_archi_SYS_WIN)

/*
==============================================================================
Name : ctor
==============================================================================
*/

Key::Key (::WPARAM wparam, ::LPARAM lparam)
:  _shift_flag (false)
,  _alt_flag (false)
,  _command_flag (false)
,  _key_code (KeyCode_UNKNOWN)
,  _key_char (KeyChar_NONE)
{
   static ::HKL h_kl = ::LoadKeyboardLayout ("00000409", 0);
   assert (h_kl != 0);

   // fill the modifiers
   
   {
      _shift_flag   = ((::GetKeyState (VK_SHIFT  ) & 0x8000) != 0);
      _command_flag = ((::GetKeyState (VK_CONTROL) & 0x8000) != 0);
      _alt_flag     = ((::GetKeyState (VK_MENU   ) & 0x8000) != 0);
   }

   // set the KeyCode
   
   const int scan_code = (lparam >> 16) & 0xFF;

   // note : constants are not defined in some SDKs
   // #define MAPVK_VK_TO_VSC 0
   // #define MAPVK_VSC_TO_VK 1
   // #define MAPVK_VK_TO_CHAR 2
   // #define MAPVK_VSC_TO_VK_EX 3

   {
      //::UINT us_vk = ::MapVirtualKeyEx (scan_code, 1/*MAPVK_VSC_TO_VK*/, h_kl); 
      
      Key_KeyMap::ScanCodeMap & scan_code_map = Key_KeyMap::use_instance ()._scan_code_map;
      //Key_KeyMap::ScanCodeMap::iterator it = scan_code_map.find (us_vk);
      Key_KeyMap::ScanCodeMap::iterator it = scan_code_map.find (wparam);
      
      if (it != scan_code_map.end ())
      {
         _key_code = it->second;
      }
   }
   
   // If it is an alpha char ([a-zA-Z]) set the KeyChar value
   
   {
      //::UINT char_value = ::MapVirtualKey (scan_code, 1/*MAPVK_VSC_TO_VK*/);
      ::UINT char_value = ::MapVirtualKey (wparam, 2/*MAPVK_VK_TO_CHAR*/);
      if (char_value >= 'a' && char_value <= 'z')
      {
         _key_char = KeyChar (char_value - 'a');
      }
      else if (char_value >= 'A' && char_value <= 'Z')
      {
         _key_char = KeyChar (char_value - 'A');
      }
   }
   
}

#endif   // (ohm_archi_SYS == ohm_archi_SYS_WIN)




/*
==============================================================================
Name : dtor
==============================================================================
*/

Key::~Key ()
{
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

bool  Key::operator == (const Key &other) const
{
   // other must be a non system key (ie during a keyboard event we will test (event.key == Key (whatever)) and not the other way around
// assert ((other._key_char == KeyChar_NONE) || (other._key_code == KeyCode_UNKNOWN));

   if (other._key_char != KeyChar_NONE)
   {
      return   (
            (_shift_flag == other._shift_flag)
         && (_alt_flag == other._alt_flag)
         && (_command_flag == other._command_flag)
         && (_key_char == other._key_char)
      );
   }
   else if (other._key_code != KeyCode_UNKNOWN)
   {
      return   (
            (_shift_flag == other._shift_flag)
         && (_alt_flag == other._alt_flag)
         && (_command_flag == other._command_flag)
         && (_key_code == other._key_code)
      );
   }
   else 
   {
      return   (
            (_shift_flag == other._shift_flag)
         && (_alt_flag == other._alt_flag)
         && (_command_flag == other._command_flag)
         && (_key_char == other._key_char)
         && (_key_code == other._key_code)
      );
   }

}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

bool  Key::operator != (const Key &other) const
{
   return ! operator == (other);
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

bool  Key::operator < (const Key &other) const
{
   using namespace std;
   
   int c = 0;
   
   if (other._key_char != KeyChar_NONE)
   {
      c = lang::cmp (_key_char, other._key_char);
   }
   
   if (c == 0)
   {
      c = lang::cmp (_key_code, other._key_code);
   }
   
   if (c != 0) 
   {
      return c < 0;
   }
   else
   {
      c = lang::cmp (_shift_flag, other._shift_flag);
      if (c != 0) return c < 0;

      c = lang::cmp (_alt_flag, other._alt_flag);
      if (c != 0) return c < 0;

      c = lang::cmp (_command_flag, other._command_flag);
      return c < 0;
   }
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  Key::write (util::BinaryStreamOutputBase & bso) const
{
   bso.write (_shift_flag);
   bso.write (_alt_flag);
   bso.write (_command_flag);
   bso.write (archi::Int32 (_key_code));
   bso.write (archi::Int32 (_key_char));
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   Key::read (util::BinaryStreamInput & bsi)
{
   int err = 0;
   
   err = bsi.read (_shift_flag);
   assert (err == 0);

   err = bsi.read (_alt_flag);
   assert (err == 0);

   err = bsi.read (_command_flag);
   assert (err == 0);

   err = bsi.read_int32 (_key_code);
   assert (err == 0);

   err = bsi.read_int32 (_key_char);
   assert (err == 0);
   
   return err;
}



/*
==============================================================================
Name : trace
==============================================================================
*/

void  Key::trace () const
{
   std::string key_code;
   std::string key_char = "*";
   
   if (_key_char != KeyChar_NONE)
   {
      key_char [0] = 'A' + _key_char;
   }
   
   ohm_util_TRACE_INFO5 (
      "%1% %2% %3% %4% %5%",
      _shift_flag ? "Shift" : "",
      _alt_flag ? "Alt" : "",
      _command_flag ? "Command" : "",
      key_code.c_str (),
      key_char.c_str ()
   );
}



/*
==============================================================================
Name : get_key_code
==============================================================================
*/

KeyCode  Key::get_key_code () const
{
   return _key_code;
}



/*
==============================================================================
Name : get_key_char
==============================================================================
*/

KeyChar  Key::get_key_char () const
{
   return _key_char;
}



/*
==============================================================================
Name : has_shift_modifier
==============================================================================
*/

bool  Key::has_shift_modifier () const
{
   return _shift_flag;
}



/*
==============================================================================
Name : has_command_modifier
==============================================================================
*/

bool  Key::has_command_modifier () const
{
   return _command_flag;
}



/*
==============================================================================
Name : has_alt_modifier
==============================================================================
*/

bool  Key::has_alt_modifier () const
{
   return _alt_flag;
}



/*
==============================================================================
Name : to_menu_item_command_key
==============================================================================
*/

#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
bool  Key::to_menu_item_command_key (::Boolean & virtual_key_flag, ::UInt16 & key) const
{
   bool ok_flag = false;
   
   if (_key_char != KeyChar_NONE)
   {
      virtual_key_flag = false;
      key = ::UInt16 ('A' + _key_char);

      ok_flag = true;
   }
   
   if (!ok_flag)
   {
      // search for the scan code
      
      if (_key_code != KeyCode_UNKNOWN)
      {
         virtual_key_flag = true;
         key = Key_KeyMap::_scan_code_arr [_key_code];

         ok_flag = true;
      }
   }
   
   return ok_flag;
}
#endif   // (ohm_archi_SYS == ohm_archi_SYS_MACOS)



/*
==============================================================================
Name : to_menu_item_key_glyph
==============================================================================
*/

#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
bool  Key::to_menu_item_key_glyph (::SInt16 & glyph) const
{
   bool ok_flag = false;
   
   // special case, no automatic glyph for space bar
   if (_key_code == KeyCode_SPACE)
   {
      glyph = kMenuSpaceGlyph;
      ok_flag = true;
   }

   return ok_flag;
}
#endif   // (ohm_archi_SYS == ohm_archi_SYS_MACOS)



#if (ohm_archi_SYS == ohm_archi_SYS_WIN)

/*
==============================================================================
Name : to_menu_modifier
Note :
   will not change the ACCEL cmd value
   will empty the string before filling it
==============================================================================
*/

void  Key::to_menu_modifier (::ACCEL & accel, std::string & shortcut) const
{
#if defined UNICODE
#error we should not be using the UNICODE win32 api
#endif
   
   accel.fVirt = FVIRTKEY;
   shortcut = "";

   if (_command_flag)
   {
      accel.fVirt |= FCONTROL;
      shortcut += "Ctrl+";
   }
   
   if (_alt_flag)
   {
      accel.fVirt |= FALT;
      shortcut += "Alt+";
   }
   
   if (_shift_flag)
   {
      accel.fVirt |= FSHIFT;
      shortcut += "Shift+";
   }
}



/*
==============================================================================
Name : to_menu_info
Note :
   will not change the ACCEL cmd value
   will empty the string before filling it
==============================================================================
*/

bool  Key::to_menu_info (::ACCEL & accel, std::string & shortcut) const
{
#if defined UNICODE
#error we should not be using the UNICODE win32 api
#endif

   static ::HKL h_kl = ::LoadKeyboardLayout ("00000409", 0);
   assert (h_kl != 0);

   bool ret_val = false;

   shortcut = "";

   if (_key_char != KeyChar_NONE)
   {
      ret_val = true;

      to_menu_modifier (accel, shortcut);
      
      accel.key = 'A' + _key_char;
      shortcut += 'A' + char (_key_char);
   }
   else if (_key_code != KeyCode_UNKNOWN)
   {
      // get the scan code
      const ::UINT vk_us = Key_KeyMap::_scan_code_arr [_key_code];
      const ::UINT scan_code = ::MapVirtualKeyEx (vk_us, 0/*MAPVK_VK_TO_VSC*/, h_kl);
      assert (scan_code != 0);

      // get the raw key name
      // eventually, it may not existe on the physical keyboard. Then the name is empty

      char raw_key_name_0 [32];
      const ::LPARAM lparam = (scan_code & 0xFF) << 16;
      const int name_len = ::GetKeyNameText (lparam, raw_key_name_0, sizeof (raw_key_name_0));

      if (name_len > 0)
      {
         ret_val = true;
      }

      if (ret_val)
      {
         // get the local virtual key code
         
         const ::UINT local_vk = ::MapVirtualKey (scan_code, 1 /*MAPVK_VSC_TO_VK*/);

         // fill the output parameters
         shortcut ="";

         accel.fVirt = FVIRTKEY;
         accel.key = local_vk;

         if (_command_flag)
         {
            accel.fVirt |= FCONTROL;
            shortcut += "Ctrl+";
         }
         if (_alt_flag)
         {
            accel.fVirt |= FALT;
            shortcut += "Alt+";
         }
         if (_shift_flag)
         {
            accel.fVirt |= FSHIFT;
            shortcut += "Shift+";
         }

         shortcut += raw_key_name_0;

         if (raw_key_name_0 [0] == '&')
         {
            // special case, a single & would mean a shortcut. 
            assert (raw_key_name_0 [1] == '\0');
            shortcut += '&';
         }
      }
   }

   return ret_val;
}



/*
==============================================================================
Name : to_utf_8
==============================================================================
*/

bool Key::to_utf_8 (std::string & str) const
{
#if defined UNICODE
#error we should not be using the UNICODE win32 api
#endif

   static ::HKL h_kl = ::LoadKeyboardLayout ("00000409", 0);
   assert (h_kl != 0);

   bool ret_val = false;
   
   str.clear ();

   if (_key_char != KeyChar_NONE)
   {
      if (!_command_flag && !_alt_flag)
      {
         ret_val = true;

         if (_shift_flag)
         {
            str = char ('A' + _key_char);
         }
         else
         {
            str = char ('a' + _key_char);
         }
      }
   }
   else if (_key_code != KeyCode_UNKNOWN)
   {
      // get the character
      const ::UINT vk_us = Key_KeyMap::_scan_code_arr [_key_code];
      const ::UINT char_code = ::MapVirtualKeyEx (vk_us, 2/*MAPVK_VK_TO_CHAR*/, h_kl);

      if (char_code != 0)
      {
         ret_val = true;
         str = char (char_code);
      }
   }

   return ret_val;
}



#endif   // (ohm_archi_SYS == ohm_archi_SYS_WIN)



Key      Key::None = Key ();
Key      Key::Shift = Key (true, false, false);
Key      Key::Alt = Key (false, true, false);
Key      Key::Command = Key (false, false, true);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

Key::Key ()
:  _shift_flag (false)
,  _alt_flag (false)
,  _command_flag (false)
,  _key_code (KeyCode_UNKNOWN)
,  _key_char (KeyChar_NONE)
{
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : operator +
==============================================================================
*/

Key   operator + (const Key & l_op, const Key & r_op)
{
   Key ret_val;
   
#if 1 // (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   if (l_op._key_code != KeyCode_UNKNOWN)
   {
      assert (l_op._key_char == KeyChar_NONE);
      assert (r_op._key_char == KeyChar_NONE);
      assert (r_op._key_code == KeyCode_UNKNOWN);
      ret_val._key_code = l_op._key_code;
      ret_val._key_char = l_op._key_char;
   }
   else if (l_op._key_char != KeyChar_NONE)
   {
      assert (l_op._key_code == KeyCode_UNKNOWN);
      assert (r_op._key_code == KeyCode_UNKNOWN);
      assert (r_op._key_char == KeyChar_NONE);
      ret_val._key_code = l_op._key_code;
      ret_val._key_char = l_op._key_char;
   }
   else
   {
      ret_val._key_code = r_op._key_code;
      ret_val._key_char = r_op._key_char;
   }

#elif 0 // (ohm_archi_SYS == ohm_archi_SYS_WIN)
   if (l_op._key_code != KeyCode_UNKNOWN)
   {
      assert (r_op._key_code == KeyCode_UNKNOWN);
      ret_val._key_code = l_op._key_code;
      ret_val._key_char = l_op._key_char;
   }
   else
   {
      ret_val._key_code = r_op._key_code;
      ret_val._key_char = r_op._key_char;
   }
#endif   

   ret_val._shift_flag = l_op._shift_flag | r_op._shift_flag;
   ret_val._alt_flag = l_op._alt_flag | r_op._alt_flag;
   ret_val._command_flag = l_op._command_flag | r_op._command_flag;
   
   return ret_val;
}



/*
==============================================================================
Name : Key_KeyMap::dtor
==============================================================================
*/

Key_KeyMap::~Key_KeyMap ()
{
}



/*
==============================================================================
Name : Key_KeyMap::ctor
==============================================================================
*/

Key_KeyMap::Key_KeyMap ()
{
   for (int i = 0; i < KeyCode_NBR_ELT; ++i)
   {
      archi::UInt16 scan_code = _scan_code_arr [i];
      _scan_code_map [scan_code] = KeyCode (i);
   }

#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   _scan_code_map [kVK_ANSI_KeypadEnter] = KeyCode_RETURN;

#endif
}


archi::UInt16 Key_KeyMap::_scan_code_arr [] = 
{
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
      // the VK code are from the US layout keyboard (see ::LoadKeyboardLayout ("00000409", 0))
      VK_RETURN,                 // KeyCode_RETURN
      VK_ESCAPE,                 // KeyCode_ESCAPE

      VK_TAB,                    // KeyCode_TAB
      VK_SPACE,                  // KeyCode_SPACE
      VK_BACK,                   // KeyCode_BACKSPACE
      VK_DELETE,                 // KeyCode_DEL
      
      VK_ADD,                    // KeyCode_NUMERIC_KEYPAD_PLUS_SIGN
      VK_SUBTRACT,               // KeyCode_NUMERIC_KEYPAD_MINUS_SIGN
      VK_MULTIPLY,               // KeyCode_NUMERIC_KEYPAD_MULTIPLY_SIGN
      VK_DIVIDE,                 // KeyCode_NUMERIC_KEYPAD_DIVIDE_SIGN
//    Non existent,              // KeyCode_NUMERIC_KEYPAD_EQUAL_SIGN

//    Non existent,              // KeyCode_NUMERIC_KEYPAD_ENTER
      VK_DECIMAL,                // KeyCode_NUMERIC_KEYPAD_PERIOD
      VK_NUMPAD0,                // KeyCode_NUMERIC_KEYPAD_0
      VK_NUMPAD1,                // KeyCode_NUMERIC_KEYPAD_1
      VK_NUMPAD2,                // KeyCode_NUMERIC_KEYPAD_2
      VK_NUMPAD3,                // KeyCode_NUMERIC_KEYPAD_3
      VK_NUMPAD4,                // KeyCode_NUMERIC_KEYPAD_4
      VK_NUMPAD5,                // KeyCode_NUMERIC_KEYPAD_5
      VK_NUMPAD6,                // KeyCode_NUMERIC_KEYPAD_6
      VK_NUMPAD7,                // KeyCode_NUMERIC_KEYPAD_7
      VK_NUMPAD8,                // KeyCode_NUMERIC_KEYPAD_8
      VK_NUMPAD9,                // KeyCode_NUMERIC_KEYPAD_9

//    None existent,             // KeyCode_HELP
      VK_PRIOR,                  // KeyCode_PAGE_UP
      VK_NEXT,                   // KeyCode_PAGE_DOWN
      VK_HOME,                   // KeyCode_HOME
      VK_END,                    // KeyCode_END
      
      VK_LEFT,                   // KeyCode_LEFT_ARROW
      VK_RIGHT,                  // KeyCode_RIGHT_ARROW
      VK_DOWN,                   // KeyCode_DOWN_ARROW
      VK_UP,                     // KeyCode_UP_ARROW

      VK_F1,                     // KeyCode_F1
      VK_F2,                     // KeyCode_F2
      VK_F3,                     // KeyCode_F3
      VK_F4,                     // KeyCode_F4
      VK_F5,                     // KeyCode_F5
      VK_F6,                     // KeyCode_F6
      VK_F7,                     // KeyCode_F7
      VK_F8,                     // KeyCode_F8
      VK_F9,                     // KeyCode_F9
      VK_F10,                    // KeyCode_F10
      VK_F11,                    // KeyCode_F11
      VK_F12,                    // KeyCode_F12
      VK_F13,                    // KeyCode_F13
      VK_F14,                    // KeyCode_F14
      VK_F15,                    // KeyCode_F15
      VK_F16,                    // KeyCode_F16
      VK_F17,                    // KeyCode_F17
      VK_F18,                    // KeyCode_F18
      VK_F19,                    // KeyCode_F19
      VK_F20,                    // KeyCode_F20

      'Q',                       // KeyCode_Q
      'W',                       // KeyCode_W
      'E',                       // KeyCode_E
      'R',                       // KeyCode_R
      'T',                       // KeyCode_T
      'Y',                       // KeyCode_Y
      'U',                       // KeyCode_U
      'I',                       // KeyCode_I
      'O',                       // KeyCode_O
      'P',                       // KeyCode_P
      VK_OEM_4,                  // KeyCode_BRACKET_RIGHT
      VK_OEM_6,                  // KeyCode_BRACKET_LEFT

      'A',                       // KeyCode_A
      'S',                       // KeyCode_S
      'D',                       // KeyCode_D
      'F',                       // KeyCode_F
      'G',                       // KeyCode_G
      'H',                       // KeyCode_H
      'J',                       // KeyCode_J
      'K',                       // KeyCode_K
      'L',                       // KeyCode_L
      VK_OEM_1,                  // KeyCode_SEMICOLON
      VK_OEM_7,                  // KeyCode_QUOTE
      VK_OEM_5,                  // KeyCode_BACKSLASH

//    Non existent               // KeyCode_GRAVE
      'Z',                       // KeyCode_Z
      'X',                       // KeyCode_X
      'C',                       // KeyCode_C
      'V',                       // KeyCode_V
      'B',                       // KeyCode_B
      'N',                       // KeyCode_N
      'M',                       // KeyCode_M
      VK_OEM_COMMA,              // KeyCode_COMMA
      VK_OEM_PERIOD,             // KeyCode_PERIOD
      VK_OEM_2,                  // KeyCode_SLASH

      VK_OEM_3,                  // KeyCode_SECTION   
      '1',                       // KeyCode_1
      '2',                       // KeyCode_2
      '3',                       // KeyCode_3
      '4',                       // KeyCode_4
      '5',                       // KeyCode_5
      '6',                       // KeyCode_6
      '7',                       // KeyCode_7
      '8',                       // KeyCode_8
      '9',                       // KeyCode_9
      '0',                       // KeyCode_0
      VK_OEM_MINUS,              // KeyCode_MINUS
      VK_OEM_PLUS,               // KeyCode_EQUAL
      
#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)

      // scan code ignored on mac :
      // kVK_ANSI_KeypadClear (numlock on pc)
      // kVK_Command             
      // kVK_Shift               
      // kVK_CapsLock            
      // kVK_Option              
      // kVK_Control             
      // kVK_RightShift          
      // kVK_RightOption         
      // kVK_RightControl        
      // kVK_Function            
      // kVK_VolumeUp            
      // kVK_VolumeDown          
      // kVK_Mute                
      // kVK_Help 

      kVK_Return,                // KeyCode_RETURN
      kVK_Escape,                // KeyCode_ESCAPE

      kVK_Tab,                   // KeyCode_TAB
      kVK_Space,                 // KeyCode_SPACE
      kVK_Delete,                // KeyCode_BACKSPACE
      kVK_ForwardDelete,         // KeyCode_DEL
      
      kVK_ANSI_KeypadPlus,       // KeyCode_NUMERIC_KEYPAD_PLUS_SIGN
      kVK_ANSI_KeypadMinus,      // KeyCode_NUMERIC_KEYPAD_MINUS_SIGN
      kVK_ANSI_KeypadMultiply,   // KeyCode_NUMERIC_KEYPAD_MULTIPLY_SIGN
      kVK_ANSI_KeypadDivide,     // KeyCode_NUMERIC_KEYPAD_DIVIDE_SIGN
//    kVK_ANSI_KeypadEquals,     // KeyCode_NUMERIC_KEYPAD_EQUAL_SIGN

//    kVK_ANSI_KeypadEnter,      // KeyCode_NUMERIC_KEYPAD_ENTER
      kVK_ANSI_KeypadDecimal,    // KeyCode_NUMERIC_KEYPAD_PERIOD
      kVK_ANSI_Keypad0,          // KeyCode_NUMERIC_KEYPAD_0
      kVK_ANSI_Keypad1,          // KeyCode_NUMERIC_KEYPAD_1
      kVK_ANSI_Keypad2,          // KeyCode_NUMERIC_KEYPAD_2
      kVK_ANSI_Keypad3,          // KeyCode_NUMERIC_KEYPAD_3
      kVK_ANSI_Keypad4,          // KeyCode_NUMERIC_KEYPAD_4
      kVK_ANSI_Keypad5,          // KeyCode_NUMERIC_KEYPAD_5
      kVK_ANSI_Keypad6,          // KeyCode_NUMERIC_KEYPAD_6
      kVK_ANSI_Keypad7,          // KeyCode_NUMERIC_KEYPAD_7
      kVK_ANSI_Keypad8,          // KeyCode_NUMERIC_KEYPAD_8
      kVK_ANSI_Keypad9,          // KeyCode_NUMERIC_KEYPAD_9

      kVK_PageUp,                // KeyCode_PAGE_UP
      kVK_PageDown,              // KeyCode_PAGE_DOWN
      kVK_Home,                  // KeyCode_HOME
      kVK_End,                   // KeyCode_END
      
      kVK_LeftArrow,             // KeyCode_LEFT_ARROW
      kVK_RightArrow,            // KeyCode_RIGHT_ARROW
      kVK_DownArrow,             // KeyCode_DOWN_ARROW
      kVK_UpArrow,               // KeyCode_UP_ARROW

      kVK_F1,                    // KeyCode_F1
      kVK_F2,                    // KeyCode_F2
      kVK_F3,                    // KeyCode_F3
      kVK_F4,                    // KeyCode_F4
      kVK_F5,                    // KeyCode_F5
      kVK_F6,                    // KeyCode_F6
      kVK_F7,                    // KeyCode_F7
      kVK_F8,                    // KeyCode_F8
      kVK_F9,                    // KeyCode_F9
      kVK_F10,                   // KeyCode_F10
      kVK_F11,                   // KeyCode_F11
      kVK_F12,                   // KeyCode_F12
      kVK_F13,                   // KeyCode_F13
      kVK_F14,                   // KeyCode_F14
      kVK_F15,                   // KeyCode_F15
      kVK_F16,                   // KeyCode_F16
      kVK_F17,                   // KeyCode_F17
      kVK_F18,                   // KeyCode_F18
      kVK_F19,                   // KeyCode_F19
      kVK_F20,                   // KeyCode_F20
   
      kVK_ANSI_Q,                // KeyCode_Q
      kVK_ANSI_W,                // KeyCode_W
      kVK_ANSI_E,                // KeyCode_E
      kVK_ANSI_R,                // KeyCode_R
      kVK_ANSI_T,                // KeyCode_T
      kVK_ANSI_Y,                // KeyCode_Y
      kVK_ANSI_U,                // KeyCode_U
      kVK_ANSI_I,                // KeyCode_I
      kVK_ANSI_O,                // KeyCode_O
      kVK_ANSI_P,                // KeyCode_P
      kVK_ANSI_LeftBracket,      // KeyCode_BRACKET_LEFT
      kVK_ANSI_RightBracket,     // KeyCode_BRACKET_RIGHT
         
      kVK_ANSI_A,                // KeyCode_A
      kVK_ANSI_S,                // KeyCode_S
      kVK_ANSI_D,                // KeyCode_D
      kVK_ANSI_F,                // KeyCode_F
      kVK_ANSI_G,                // KeyCode_G
      kVK_ANSI_H,                // KeyCode_H
      kVK_ANSI_J,                // KeyCode_J
      kVK_ANSI_K,                // KeyCode_K
      kVK_ANSI_L,                // KeyCode_L
      kVK_ANSI_Semicolon,        // KeyCode_SEMICOLON
      kVK_ANSI_Quote,            // KeyCode_QUOTE
      kVK_ANSI_Backslash,        // KeyCode_BACKSLASH
      
//    kVK_ANSI_Grave,            // KeyCode_GRAVE
      kVK_ANSI_Z,                // KeyCode_Z
      kVK_ANSI_X,                // KeyCode_X
      kVK_ANSI_C,                // KeyCode_C
      kVK_ANSI_V,                // KeyCode_V
      kVK_ANSI_B,                // KeyCode_B
      kVK_ANSI_N,                // KeyCode_N
      kVK_ANSI_M,                // KeyCode_M
      kVK_ANSI_Comma,            // KeyCode_COMMA
      kVK_ANSI_Period,           // KeyCode_PERIOD
      kVK_ANSI_Slash,            // KeyCode_SLASH
               
      kVK_ISO_Section,           // KeyCode_SECTION   
      kVK_ANSI_1,                // KeyCode_1
      kVK_ANSI_2,                // KeyCode_2
      kVK_ANSI_3,                // KeyCode_3
      kVK_ANSI_4,                // KeyCode_4
      kVK_ANSI_5,                // KeyCode_5
      kVK_ANSI_6,                // KeyCode_6
      kVK_ANSI_7,                // KeyCode_7
      kVK_ANSI_8,                // KeyCode_8
      kVK_ANSI_9,                // KeyCode_9
      kVK_ANSI_0,                // KeyCode_0
      kVK_ANSI_Minus,            // KeyCode_MINUS
      kVK_ANSI_Equal,            // KeyCode_EQUAL
#endif
};



}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
