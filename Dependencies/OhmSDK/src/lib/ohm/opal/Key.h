/*****************************************************************************

        Key.h
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



#if ! defined (ohm_opal_Key_HEADER_INCLUDED)
#define  ohm_opal_Key_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/opal/KeyChar.h"
#include "ohm/opal/KeyCode.h"

#if ohm_archi_SYS == ohm_archi_SYS_WIN
   #define  NOMINMAX
   #include <windows.h>
#endif

#include <string>



namespace ohm
{
namespace util
{
class BinaryStreamOutputBase;
class BinaryStreamInput;
}

namespace opal
{



class SystemEventMgr;
class ViewImpl;

class Key
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  Key ();
   explicit       Key (KeyChar key_char);
   explicit       Key (KeyCode key_code);

#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   explicit       Key (::EventRef e);
#endif
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
                  Key (::WPARAM wparam, ::LPARAM lparam);
#endif

   virtual        ~Key ();

   bool           operator == (const Key &other) const;
   bool           operator != (const Key &other) const;
   bool           operator < (const Key &other) const;
   
   void           write (util::BinaryStreamOutputBase & bso) const;
   int            read (util::BinaryStreamInput & bsi);
   
   void           trace () const;
   
   KeyCode        get_key_code () const;
   KeyChar        get_key_char () const;
   bool           has_shift_modifier () const;
   bool           has_command_modifier () const;
   bool           has_alt_modifier () const;

#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   bool           to_menu_item_command_key (::Boolean & virtual_key_flag, ::UInt16 & key) const;
   bool           to_menu_item_key_glyph (::SInt16 & glyph) const;
#elif (ohm_archi_SYS == ohm_archi_SYS_WIN)
   void           to_menu_modifier (::ACCEL & accel, std::string & shortcut) const;
   bool           to_menu_info (::ACCEL & accel, std::string & shortcut) const;
   bool           to_utf_8 (std::string & str) const;
#endif
   
   static Key     None;
   static Key     Shift;
   static Key     Alt;
   static Key     Command;
   
   

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   friend class SystemEventMgr;
   friend class ViewImpl;
   friend Key     operator + (const Key & l_op, const Key & r_op);

                  Key (bool shift_flag, bool alt_flag, bool command_flag);

// char           _mod_char;     // can be empty. 
   bool           _shift_flag;
   bool           _alt_flag;
   bool           _command_flag; // command on MacOS X, ctrl on PC
   KeyCode        _key_code;  
   KeyChar        _key_char;     
                                 

   
/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

}; // class Key



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

Key   operator + (const Key & l_op, const Key & r_op);


}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_Key_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
