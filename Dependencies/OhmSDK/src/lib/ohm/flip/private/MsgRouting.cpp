/*****************************************************************************

        MsgRouting.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 45364

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

#include "ohm/flip/Err.h"
#include "ohm/flip/private/MsgRouting.h"
#include "ohm/util/BinaryStreamOutput.h"
#include "ohm/util/BinaryStreamInput.h"

#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : write
==============================================================================
*/

void  MsgRouting::write (util::BinaryStreamOutputBase & bso) const
{
   bso.write (archi::UByte (_self_slot_flag ? 1 : 0));
   bso.write (archi::UByte (_other_slots_flag ? 1 : 0));
   bso.write (archi::UByte (_network_flag ? 1 : 0));
}



/*
==============================================================================
Name : is_same_thread
==============================================================================
*/

int   MsgRouting::read (util::BinaryStreamInput & bsi)
{
   {
      archi::UByte flag = 0;
      int err = bsi.read (flag);
      assert (err == Err_OK);
      
      _self_slot_flag = flag != 0;
   }

   {
      archi::UByte flag = 0;
      int err = bsi.read (flag);
      assert (err == Err_OK);
      
      _other_slots_flag = flag != 0;
   }

   {
      archi::UByte flag = 0;
      int err = bsi.read (flag);
      assert (err == Err_OK);
      
      _network_flag = flag != 0;
   }
   
   return 0;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
