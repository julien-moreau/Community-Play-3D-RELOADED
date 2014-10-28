/*****************************************************************************

        DataMaster.cpp
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

#include "ohm/flip/DataMaster.h"

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

DataMaster::DataMaster ()
:  _data_sptr ()
,  _locked_flag (false)
,  _offset (0)
,  _size (0)
{
   assert (false);
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

DataMaster::DataMaster (const DataMaster & other)
:  _data_sptr ()
,  _locked_flag (false)
,  _offset (0)
,  _size (0)
{
   assert (false);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

DataMaster::~DataMaster ()
{
   // destruction (undefine) is handled in base class Object
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

DataMaster &   DataMaster::operator = (const DataMaster &other)
{
   assert (false);
   
   return *this;
}



/*
==============================================================================
Name : assign
==============================================================================
*/

void  DataMaster::assign (const task::IpcData & ipc_data)
{
   assert (false);
}



/*
==============================================================================
Name : assign
==============================================================================
*/

void  DataMaster::assign (const DataSPtr & data_sptr)
{
   assert (false);
}



/*
==============================================================================
Name : assign
==============================================================================
*/

void  DataMaster::assign (std::auto_ptr <Data> & data_aptr)
{
   assert (false);
}



/*
==============================================================================
Name : lock
Description :
   Lock the data contained in 'this', making it impossible to assign or
   modify it (or copies of 'this').
==============================================================================
*/

void  DataMaster::lock ()
{
   assert (false);
}



/*
==============================================================================
Name : unlock
==============================================================================
*/

void  DataMaster::unlock ()
{
   assert (false);
}



/*
==============================================================================
Name : is_locked
==============================================================================
*/

bool  DataMaster::is_locked () const
{
   assert (false);
   
   return false;
}



/*
==============================================================================
Name : use
Description :
   When not locked, allow to change the data.
   This is only possible when 'this' is not a portion of the data.
==============================================================================
*/

Data &   DataMaster::use ()
{
   assert (false);
   
   return *_data_sptr;
}



/*
==============================================================================
Name : subdata
==============================================================================
*/

DataMaster  DataMaster::subdata (size_t offset, size_t size) const
{
   assert (false);
   
   DataMaster ret_val;
   
   return ret_val;
}



/*
==============================================================================
Name : data
==============================================================================
*/

const archi::UByte * DataMaster::data () const
{
   assert (false);
   
   return 0;
}



/*
==============================================================================
Name : size
==============================================================================
*/

size_t   DataMaster::size () const
{
   assert (false);
   
   return 0;
}



/*
==============================================================================
Name : empty
==============================================================================
*/

bool  DataMaster::empty () const
{
   assert (false);
   
   return true;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
