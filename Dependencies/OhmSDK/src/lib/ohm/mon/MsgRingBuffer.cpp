/*****************************************************************************

        MsgRingBuffer.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 67511

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
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/mon/MsgRingBuffer.h"

#include "ohm/conc/AioAddMod.h"
#include "ohm/conc/AtomicIntOp.h"

#include <cassert>



namespace ohm
{
namespace mon
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

MsgRingBuffer::MsgRingBuffer ()
:  _pos (0)
,  _msg_arr ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

MsgRingBuffer::~MsgRingBuffer ()
{
}



/*
==============================================================================
Name : resize
==============================================================================
*/

void  MsgRingBuffer::resize (size_t size)
{
   _msg_arr.resize (size);
}



/*
==============================================================================
Name : size
==============================================================================
*/

size_t   MsgRingBuffer::size () const
{
   return _msg_arr.size ();
}



/*
==============================================================================
Name : push
==============================================================================
*/

Msg & MsgRingBuffer::push ()
{
   const conc::AioAddMod <size_t> ftor_inc (1, _msg_arr.size ());
   size_t new_pos = conc::AtomicIntOp_exec_new (_pos, ftor_inc);
   
   return _msg_arr [new_pos];
}



/*
==============================================================================
Name : operator []
==============================================================================
*/

const Msg & MsgRingBuffer::operator [] (size_t idx) const
{
   idx = (_pos + 1 + idx) % _msg_arr.size ();
   
   return _msg_arr [idx];
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace mon
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
