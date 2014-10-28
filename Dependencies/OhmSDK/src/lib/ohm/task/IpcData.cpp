/*****************************************************************************

        IpcData.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 62624

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

#include "ohm/conc/AioAdd.h"
#include "ohm/conc/AtomicIntOp.h"

#include "ohm/task/IpcData.h"

#include "ohm/archi/def.h"

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   #include <mach/mach.h>

#elif (ohm_archi_SYS == ohm_archi_SYS_LINUX)

#else
   #error Operation System not supported

#endif


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
:  _data_ptr (data_size > 0 ? data_ptr : 0)
,  _data_size ((_data_ptr != 0) ? data_size : 0)
,  _count_ptr ((_data_ptr != 0) ? new conc::AtomicInt <long> (1) : 0)
{
   // nothing
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

IpcData::IpcData (const IpcData & other)
:  _data_ptr (other._data_ptr)
,  _data_size (other._data_size)
,  _count_ptr (other._count_ptr)
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
      remove_ref ();
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
   if (_data_ptr != other._data_ptr)
   {
      remove_ref ();
      
      _data_ptr = other._data_ptr;
      _data_size = other._data_size;
      _count_ptr = other._count_ptr;

      add_ref ();
   }
   
   return *this;
}



/*
==============================================================================
Name : data
==============================================================================
*/

const archi::UByte * IpcData::data () const
{
   // might be null
   
   return _data_ptr;
}


/*
==============================================================================
Name : size
==============================================================================
*/

size_t   IpcData::size () const
{
   // might be 0
   
   return _data_size;
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
   if (_count_ptr != 0)
   {
      const conc::AioAdd <long>  ftor_inc (1);
      conc::AtomicIntOp_exec (*_count_ptr, ftor_inc);
   }
}



/*
==============================================================================
Name : remove_ref
==============================================================================
*/

void  IpcData::remove_ref ()
{
   if (_data_ptr != 0)
   {
      const conc::AioAdd <long>  ftor_inc (-1);
      long count = conc::AtomicIntOp_exec_new (*_count_ptr, ftor_inc);
      
      if (count == 0)
      {
         release ();

         delete _count_ptr;
         _count_ptr = 0;
      }
   }
}



/*
==============================================================================
Name : release
==============================================================================
*/

void  IpcData::release ()
{
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   delete [] _data_ptr;

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   kern_return_t err = vm_deallocate (
      mach_task_self (),
      (vm_address_t) _data_ptr,
      _data_size
   );
   
   assert (err == KERN_SUCCESS);

#elif (ohm_archi_SYS == ohm_archi_SYS_LINUX)
   assert (false);

#else
   #error Operation System not supported

#endif

   _data_ptr = 0;
   _data_size = 0;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
