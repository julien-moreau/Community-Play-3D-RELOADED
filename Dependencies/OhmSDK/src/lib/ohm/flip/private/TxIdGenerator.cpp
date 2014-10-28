/*****************************************************************************

        TxIdGenerator.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 45327

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

#include "ohm/flip/private/TxIdGenerator.h"
#include "ohm/flip/private/SeedGenerator.h"

#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
   Generator of unique transaction ids accross all the network.
   This object is directly attached to a document. Since there is one
   document per thread when needed, we need also the 'thread_id' to ensure
   uniqueness of the generated id.
==============================================================================
*/

TxIdGenerator::TxIdGenerator (archi::Int32 user_id, archi::Int32 thread_id)
{
   _cur_generator_tx_id = SeedGenerator::get (user_id, thread_id);

   assert (_cur_generator_tx_id > 0);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

TxIdGenerator::~TxIdGenerator ()
{
}



/*
==============================================================================
Name : get
Description :
   Returns a new unique transaction id
==============================================================================
*/

ohm::archi::Int64 TxIdGenerator::get ()
{
   archi::Int64 tx_id = _cur_generator_tx_id;
   ++_cur_generator_tx_id;
   
   assert (tx_id > 0);
   
   return tx_id;
}



/*
==============================================================================
Name : give_back
==============================================================================
*/

void  TxIdGenerator::give_back ()
{
   --_cur_generator_tx_id;
   assert (_cur_generator_tx_id > 0);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
