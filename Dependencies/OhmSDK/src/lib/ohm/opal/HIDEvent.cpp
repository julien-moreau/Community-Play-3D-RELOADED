/*****************************************************************************

        HIDEvent.cpp
        Copyright (c) 2012 Ohm Force

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

#include "ohm/opal/HIDEvent.h"

#include "ohm/util/BinaryStreamInput.h"
#include "ohm/util/BinaryStreamOutputBase.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

HIDEvent::HIDEvent ()
:  _type (Type_NONE)
,  _button_nbr (0)
,  _click_count (0)
,  _location ()
,  _delta ()
,  _key ()
,  _str ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

HIDEvent::~HIDEvent ()
{
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  HIDEvent::write (util::BinaryStreamOutputBase & bso) const
{
   bso.write (archi::UByte (_type));
   bso.write (archi::UByte (_button_nbr));
   bso.write (archi::UByte (_click_count));
   bso.write (archi::Float32 (_location._x));
   bso.write (archi::Float32 (_location._y));
   bso.write (archi::Float32 (_delta._x));
   bso.write (archi::Float32 (_delta._y));
   bso.write (_key);
   bso.write (_str);
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   HIDEvent::read (util::BinaryStreamInput & bsi)
{
   int err = 0;
   
   err = bsi.read_ubyte (_type);
   assert (err == 0);

   err = bsi.read_ubyte (_button_nbr);
   assert (err == 0);

   err = bsi.read_ubyte (_click_count);
   assert (err == 0);

   err = bsi.read_float32 (_location._x);
   assert (err == 0);

   err = bsi.read_float32 (_location._y);
   assert (err == 0);
   
   err = bsi.read_float32 (_delta._x);
   assert (err == 0);

   err = bsi.read_float32 (_delta._y);
   assert (err == 0);
   
   err = bsi.read (_key);
   assert (err == 0);
   
   err = bsi.read (_str);
   assert (err == 0);
   
   return err;
}



/*
==============================================================================
Name : trace
==============================================================================
*/

void  HIDEvent::trace () const
{
   ohm_util_TRACE_INFO ("HIDEvent:");
   
   odbg::LogMgr::use_instance ().indent (2);
   
   switch (_type)
   {
   case Type_MOUSE_BUTTON_PRESSED:
      ohm_util_TRACE_INFO ("MOUSE_BUTTON_PRESSED");
      break;

   case Type_MOUSE_BUTTON_RELEASED:
      ohm_util_TRACE_INFO ("MOUSE_BUTTON_RELEASED");
      break;

   case Type_MOUSE_MOVED:
      ohm_util_TRACE_INFO ("MOUSE_MOVED");
      break;

   case Type_MOUSE_WHEEL_MOVED:
      ohm_util_TRACE_INFO ("MOUSE_WHEEL_MOVED");
      break;

   case Type_KEY_PRESSED:
      ohm_util_TRACE_INFO ("KEY_PRESSED");
      _key.trace ();
      break;

   case Type_KEY_REPEATED:
      ohm_util_TRACE_INFO ("KEY_REPEATED");
      _key.trace ();
      break;

   case Type_KEY_RELEASED:
      ohm_util_TRACE_INFO ("KEY_RELEASED");
      _key.trace ();
      break;

   case Type_CHARACTERS_TYPED:
      ohm_util_TRACE_INFO ("CHARACTERS_TYPED");
      ohm_util_TRACE_INFO1 ("%1%", _str.c_str ());
      break;
   }
   
   odbg::LogMgr::use_instance ().indent (-2);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
