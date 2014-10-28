/*****************************************************************************

        TxDesc.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 48991

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
#include "ohm/flip/private/TxDesc.h"
#include "ohm/util/BinaryStreamOutput.h"
#include "ohm/util/BinaryStreamInput.h"
#include "ohm/flip/private/SeedGenerator.h"

#include "ohm/lang/fnc.h"

#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

TxDesc::TxDesc ()
:  _session_id (0)
,  _sub_id (0)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

TxDesc::TxDesc (const TxDesc &other)
:  _session_id (other._session_id)
,  _sub_id (other._sub_id)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

TxDesc::~TxDesc ()
{
   // don't destroy sptr explicitely
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

TxDesc & TxDesc::operator = (const TxDesc &other)
{
   _session_id = other._session_id;
   _sub_id = other._sub_id;
   
   return *this;
}



/*
==============================================================================
Name : promote_public
==============================================================================
*/

void  TxDesc::promote_public ()
{
   assert (_sub_id != SESSION_SUB_ID);
   
   _sub_id = SESSION_SUB_ID;
}



/*
==============================================================================
Name : is_public
==============================================================================
*/

bool  TxDesc::is_public () const
{
   return (_sub_id == SESSION_SUB_ID);
}



/*
==============================================================================
Name : begin_public
==============================================================================
*/

void  TxDesc::begin_public (archi::Int64 tx_id)
{
   assert (_session_id == 0);
   assert (tx_id > 0);
   
   _session_id = tx_id;
   _sub_id = 0;
}



/*
==============================================================================
Name : end_public
==============================================================================
*/

void  TxDesc::end_public ()
{
   assert (_session_id != 0);
   
   _session_id = 0;
}



/*
==============================================================================
Name : increment_local
==============================================================================
*/

void  TxDesc::increment_local ()
{
   assert (_session_id != 0);
   assert (_sub_id != SESSION_SUB_ID);
   
   ++_sub_id;
}



/*
==============================================================================
Name : inverse_direction
==============================================================================
*/

void  TxDesc::inverse_direction ()
{
   assert (_session_id != 0);
   assert (_sub_id == 0);
   
   _session_id = - _session_id;
}



/*
==============================================================================
Name : get_tx_id
==============================================================================
*/

archi::Int64   TxDesc::get_tx_id () const
{
   assert (_session_id != 0);
   
   return _session_id;
}



/*
==============================================================================
Name : get_sub_id
==============================================================================
*/

archi::UInt32  TxDesc::get_sub_id () const
{
   return _sub_id;
}



/*
==============================================================================
Name : is_same_user
==============================================================================
*/

bool  TxDesc::is_same_user (archi::Int32 user_id) const
{
   return (SeedGenerator::to_user_id (_session_id) == user_id);
}



/*
==============================================================================
Name : is_same_thread
==============================================================================
*/

bool  TxDesc::is_same_thread (archi::Int32 thread_id) const
{
   return (SeedGenerator::to_thread_id (_session_id) == thread_id);
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  TxDesc::write (util::BinaryStreamOutputBase & bso) const
{
   bso.write (_session_id);
   bso.write (_sub_id);
}



/*
==============================================================================
Name : is_same_thread
==============================================================================
*/

int   TxDesc::read (util::BinaryStreamInput & bsi)
{
   int err = bsi.read (_session_id);
   assert (err == Err_OK);

   err = bsi.read (_sub_id);
   assert (err == Err_OK);
   
   return 0;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
