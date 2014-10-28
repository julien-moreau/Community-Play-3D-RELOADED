/*****************************************************************************

        BinaryStreamOutput.cpp
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

#include "ohm/util/BinaryStreamOutput.h"
#include "ohm/lang/fnc.h"

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

BinaryStreamOutput::BinaryStreamOutput (std::vector <archi::UByte> & dst)
:  _data (dst)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

BinaryStreamOutput::~BinaryStreamOutput ()
{
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*
==============================================================================
Name : do_data
==============================================================================
*/

const archi::UByte * BinaryStreamOutput::do_data () const
{
   if (size () > 0)
   {
      return &_data [0];
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

size_t   BinaryStreamOutput::do_size () const
{
   return _data.size ();
}



/*
==============================================================================
Name : do_empty
==============================================================================
*/

bool  BinaryStreamOutput::do_empty () const
{
   return _data.empty ();
}



/*
==============================================================================
Name : do_clear
==============================================================================
*/

void  BinaryStreamOutput::do_clear ()
{
   _data.clear ();
}



/*
==============================================================================
Name : do_resize_of
==============================================================================
*/

archi::UByte * BinaryStreamOutput::do_resize_of (size_t diff_size)
{
   size_t pos = _data.size ();
   
   _data.resize (pos + diff_size);
   
   return &_data [pos];
}



/*
==============================================================================
Name : do_resize_max_of
==============================================================================
*/

archi::UByte * BinaryStreamOutput::do_resize_max_of (size_t pos, size_t diff_size)
{
   _data.resize (lang::max (pos + diff_size, _data.size ()));
   
   return &_data [pos];
}



/*
==============================================================================
Name : do_resize_write_bytes
==============================================================================
*/

void  BinaryStreamOutput::do_resize_write_bytes (const archi::UByte * data_ptr, size_t data_size)
{
   _data.insert (_data.end (), data_ptr, data_ptr + data_size);
}



/*
==============================================================================
Name : do_write_front_bytes
==============================================================================
*/

void  BinaryStreamOutput::do_write_front_bytes (const archi::UByte * data_ptr, size_t data_size)
{
   _data.insert (_data.begin (), data_ptr, data_ptr + data_size);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace util
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
