/*****************************************************************************

        BinaryStreamOutputNa.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 61015

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

#include "ohm/util/BinaryStreamOutputNa.h"
#include "ohm/util/trace.h"
#include "ohm/lang/fnc.h"

#include <algorithm>
#include <stdexcept>

#include <cassert>



namespace ohm
{
namespace util
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

BinaryStreamOutputNa::BinaryStreamOutputNa (archi::UByte * buf_ptr, size_t buf_size)
:  _buf_ptr (buf_ptr)
,  _buf_size (buf_size)
,  _cur_size (0)
{
   assert (_buf_ptr != 0);
   assert (_buf_size > 0);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

BinaryStreamOutputNa::~BinaryStreamOutputNa ()
{
   // nothing
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*
==============================================================================
Name : do_data
==============================================================================
*/

const archi::UByte * BinaryStreamOutputNa::do_data () const
{
   if (size () > 0)
   {
      return _buf_ptr;
   }
   else
   {
      return 0;
   }
}



/*
==============================================================================
Name : do_size
==============================================================================
*/

size_t   BinaryStreamOutputNa::do_size () const
{
   return _cur_size;
}



/*
==============================================================================
Name : do_empty
==============================================================================
*/

bool  BinaryStreamOutputNa::do_empty () const
{
   return _cur_size == 0;
}



/*
==============================================================================
Name : do_clear
==============================================================================
*/

void  BinaryStreamOutputNa::do_clear ()
{
   _cur_size = 0;
}



/*
==============================================================================
Name : do_resize_of
==============================================================================
*/

archi::UByte * BinaryStreamOutputNa::do_resize_of (size_t diff_size)
{
   size_t pos = _cur_size;
   _cur_size += diff_size;
   assert (_cur_size <= _buf_size);
   if (_cur_size > _buf_size)
   {
      ohm_util_TRACE_PANIC ("do_resize_of(): buffer overflow.");
      throw std::runtime_error (
         "ohm::util::BinaryStreamOutputNa::do_resize_of(): "
         "buffer overflow."
      );
   }
   
   return &_buf_ptr [pos];
}



/*
==============================================================================
Name : do_resize_max_of
==============================================================================
*/

archi::UByte * BinaryStreamOutputNa::do_resize_max_of (size_t pos, size_t diff_size)
{
   lang::max_in_place (_cur_size, pos + diff_size);
   
   assert (_cur_size <= _buf_size);
   if (_cur_size > _buf_size)
   {
      ohm_util_TRACE_PANIC ("do_resize_max_of(): buffer overflow.");
      throw std::runtime_error (
         "ohm::util::BinaryStreamOutputNa::do_resize_max_of(): "
         "buffer overflow."
      );
   }

   return &_buf_ptr [pos];
}



/*
==============================================================================
Name : do_resize_write_bytes
==============================================================================
*/

void  BinaryStreamOutputNa::do_resize_write_bytes (const archi::UByte * data_ptr, size_t data_size)
{
   using namespace std;
   
   size_t pos = _cur_size;
   _cur_size += data_size;
   assert (_cur_size <= _buf_size);
   if (_cur_size > _buf_size)
   {
      ohm_util_TRACE_PANIC ("do_resize_write_bytes(): buffer overflow.");
      throw std::runtime_error (
         "ohm::util::BinaryStreamOutputNa::do_resize_write_bytes(): "
         "buffer overflow."
      );
   }
   
   memcpy (&_buf_ptr [pos], data_ptr, data_size);
}



/*
==============================================================================
Name : do_write_front_bytes
==============================================================================
*/

void  BinaryStreamOutputNa::do_write_front_bytes (const archi::UByte * data_ptr, size_t data_size)
{
   using namespace std;
   
   size_t old_size = _cur_size;
   _cur_size += data_size;
   assert (_cur_size <= _buf_size);
   if (_cur_size > _buf_size)
   {
      ohm_util_TRACE_PANIC ("do_write_front_bytes(): buffer overflow.");
      throw std::runtime_error (
         "ohm::util::BinaryStreamOutputNa::do_write_front_bytes(): "
         "buffer overflow."
      );
   }

   std::copy_backward (&_buf_ptr [0], &_buf_ptr [old_size], &_buf_ptr [_cur_size]);
   
   memcpy (&_buf_ptr [0], data_ptr, data_size);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace util
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
