/*****************************************************************************

        IpcData.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 47146

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

#include "ohm/task/IpcData.h"

#include <cassert>



namespace ohm
{
namespace task
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

IpcData::IpcData ()
:  _data_ptr (0)
,  _data_size (0)
,  _count_ptr (0)
{
   // nothing
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

IpcData::IpcData (const archi::UByte * data_ptr, size_t data_size)
:  _data_ptr (0)
,  _data_size (0)
,  _count_ptr (0)
{
   // nothing
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

IpcData::IpcData (const IpcData & other)
:  _data_ptr (0)
,  _data_size (0)
,  _count_ptr (0)
{
   add_ref ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

IpcData::~IpcData ()
{
   try
   {
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

IpcData &   IpcData::operator = (const IpcData &other)
{
   assert (false);
   
   return *this;
}



/*
==============================================================================
Name : data
==============================================================================
*/

const archi::UByte * IpcData::data () const
{
   assert (false);
   
   return 0;
}


/*
==============================================================================
Name : size
==============================================================================
*/

size_t   IpcData::size () const
{
   assert (false);
   
   return 0;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : add_ref
==============================================================================
*/

void  IpcData::add_ref ()
{
   assert (false);
}



/*
==============================================================================
Name : remove_ref
==============================================================================
*/

void  IpcData::remove_ref ()
{
   assert (false);
}



/*
==============================================================================
Name : release
==============================================================================
*/

void  IpcData::release ()
{
   assert (false);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
