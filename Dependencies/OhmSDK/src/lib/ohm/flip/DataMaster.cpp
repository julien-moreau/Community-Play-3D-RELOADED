/*****************************************************************************

        DataMaster.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 45458

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
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

DataMaster::DataMaster (const DataMaster & other)
:  _data_sptr (other._data_sptr)
,  _ipc_data (other._ipc_data)
,  _locked_flag (other._locked_flag)
,  _offset (other._offset)
,  _size (other._size)
{
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
   _data_sptr = other._data_sptr;
   _ipc_data = other._ipc_data;
   _locked_flag = other._locked_flag;
   _offset = other._offset;
   _size = other._size;
   
   return *this;
}



/*
==============================================================================
Name : assign
==============================================================================
*/

void  DataMaster::assign (const task::IpcData & ipc_data)
{
   assert (!_locked_flag);
   
   _data_sptr.destroy ();
   _ipc_data = ipc_data;
   _offset = 0;
   _size = _ipc_data.size ();
}



/*
==============================================================================
Name : assign
==============================================================================
*/

void  DataMaster::assign (const DataSPtr & data_sptr)
{
   assert (!_locked_flag);
   
   _data_sptr = data_sptr;
   _ipc_data = task::IpcData ();
   _offset = 0;
   _size = data_sptr->size ();
}



/*
==============================================================================
Name : assign
==============================================================================
*/

void  DataMaster::assign (std::auto_ptr <Data> & data_aptr)
{
   assert (!_locked_flag);
   
   assign (DataSPtr (data_aptr.release ()));
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
   assert (!_locked_flag);
   
   _locked_flag = true;
}



/*
==============================================================================
Name : unlock
==============================================================================
*/

void  DataMaster::unlock ()
{
   assert (_locked_flag);
   
   _locked_flag = false;
}



/*
==============================================================================
Name : is_locked
==============================================================================
*/

bool  DataMaster::is_locked () const
{
   return _locked_flag;
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
   assert (!_locked_flag);
   assert (_data_sptr.get () != 0);
   assert (_offset == 0);
   assert (_size == _data_sptr->size ());
   
   return *_data_sptr;
}



/*
==============================================================================
Name : subdata
==============================================================================
*/

DataMaster  DataMaster::subdata (size_t offset, size_t size) const
{
   assert ((_data_sptr.get () != 0) ^ (_ipc_data.data () != 0));
   assert (offset + size <= _size);
   
   DataMaster ret_val (*this);
   ret_val._offset = _offset + offset;
   ret_val._size = size;
   
   return ret_val;
}



/*
==============================================================================
Name : data
==============================================================================
*/

const archi::UByte * DataMaster::data () const
{
   if (size () == 0)
   {
      return 0;
   }
   else
   {
      if (_data_sptr.get () != 0)
      {
         assert (_offset < _data_sptr->size ());
      
         return &(*_data_sptr) [_offset];
      }
      else if (_ipc_data.data () != 0)
      {
         assert (_offset < _ipc_data.size ());
         
         return &(_ipc_data.data ()) [_offset];
      }
      else
      {
         assert (false);
         
         return 0;
      }
   }
}



/*
==============================================================================
Name : size
==============================================================================
*/

size_t   DataMaster::size () const
{
#if ! defined (NDEBUG)
   if (_data_sptr.get () != 0)
   {
      assert (_offset + _size <= _data_sptr->size ());
   }
   else if (_ipc_data.data () != 0)
   {
      assert (_offset + _size <= _ipc_data.size ());
   }
   else
   {
      assert (_size == 0);
   }
#endif
   
   // might be 0
   return _size;
}



/*
==============================================================================
Name : empty
==============================================================================
*/

bool  DataMaster::empty () const
{
   return (size () == 0);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
