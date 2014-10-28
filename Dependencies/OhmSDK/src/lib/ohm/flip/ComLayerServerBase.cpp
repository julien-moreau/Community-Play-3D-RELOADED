/*****************************************************************************

        ComLayerServerBase.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 63111

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

#include "ohm/flip/ComLayerServerBase.h"
#include "ohm/util/BinaryStreamInput.h"

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
==============================================================================
*/

ComLayerServerBase::ComLayerServerBase ()
:  _listener_ptr (0)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ComLayerServerBase::~ComLayerServerBase ()
{
}



/*
==============================================================================
Name : bind
==============================================================================
*/

void  ComLayerServerBase::bind (ComLayerServerListener & listener)
{
   assert (&listener != 0);
   _listener_ptr = &listener;
}



/*
==============================================================================
Name : send
==============================================================================
*/

void  ComLayerServerBase::send (const std::string & to, const archi::UByte * data_ptr, size_t data_size)
{
   do_send (to, data_ptr, data_size);
}



/*
==============================================================================
Name : broadcast
==============================================================================
*/

void  ComLayerServerBase::broadcast (const archi::UByte * data_ptr, size_t data_size)
{
   do_broadcast (data_ptr, data_size);
}



/*
==============================================================================
Name : broadcast
==============================================================================
*/

void  ComLayerServerBase::broadcast (const std::string & except, const archi::UByte * data_ptr, size_t data_size)
{
   do_broadcast (except, data_ptr, data_size);
}



/*
==============================================================================
Name : receive
==============================================================================
*/

void  ComLayerServerBase::receive (const std::string & from, util::BinaryStreamInput & bsi)
{
   assert (_listener_ptr != 0);
   
   // is going to call either send or broadcast
   
   _listener_ptr->receive (from, bsi);
}



/*
==============================================================================
Name : notify_document_need_flush
==============================================================================
*/

void  ComLayerServerBase::notify_document_need_flush ()
{
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
