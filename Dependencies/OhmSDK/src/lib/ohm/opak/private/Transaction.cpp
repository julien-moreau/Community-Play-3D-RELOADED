/*****************************************************************************

        Transaction.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 53796

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

#include "ohm/opak/private/Transaction.h"

#include <cassert>



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

Transaction::Transaction ()
/*:   _immediate_execution_flag (false)*/
:  _data ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Transaction::~Transaction ()
{
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

Transaction &  Transaction::operator = (const Transaction & other)
{
   /*assert (!_immediate_execution_flag);*/
   
   _data = other._data;
   
   return *this;
}



/*
==============================================================================
Name : enable_immediate_execution
==============================================================================
*/

/*void   Transaction::enable_immediate_execution ()
{
   _immediate_execution_flag = true;
}*/



/*
==============================================================================
Name : clear
==============================================================================
*/

void  Transaction::clear ()
{
   /*assert (!_immediate_execution_flag);*/
   
   _data.clear ();
}



/*
==============================================================================
Name : empty
==============================================================================
*/

bool  Transaction::empty ()
{
   return _data.empty ();
}



/*
==============================================================================
Name : push
==============================================================================
*/

void  Transaction::push (Proc proc, void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   assert (target_ptr != 0);
#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
   assert (_target_ptr_set.find (target_ptr) != _target_ptr_set.end ());
#endif

   /*if (_immediate_execution_flag)
   {
      proc (target_ptr, data_size, data_ptr);
   }
   else*/
   {
      using namespace std;
      
      size_t nbr_bytes = sizeof (Proc) + sizeof (void *) + sizeof (size_t) + data_size;
      
      _data.resize (_data.size () + nbr_bytes);
      archi::UByte * pos_ptr = &_data [_data.size () - nbr_bytes];

      memcpy (pos_ptr, &proc, sizeof (Proc));
      pos_ptr += sizeof (Proc);

      memcpy (pos_ptr, &target_ptr, sizeof (void *));
      pos_ptr += sizeof (void *);
      
      memcpy (pos_ptr, &data_size, sizeof (size_t));
      pos_ptr += sizeof (size_t);
      
      if (data_size > 0)
      {
         memcpy (pos_ptr, data_ptr, data_size);
      }
   }
}



/*
==============================================================================
Name : run
==============================================================================
*/

void  Transaction::run () const
{
   /*assert (!_immediate_execution_flag);*/

   if (_data.size () > 0)
   {
      bool loop_flag = true;
      
      const archi::UByte * pos_ptr = &_data [0];

      while (loop_flag)
      {
         run_atom (pos_ptr);
         
         assert (pos_ptr <= &_data [0] + _data.size ());
         
         loop_flag = (pos_ptr < &_data [0] + _data.size ());
      }
   }
}



/*
==============================================================================
Name : debug_add
Description :
   Add a target (a primitive) to the target set.
==============================================================================
*/

#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
void  Transaction::debug_add (void * target_ptr)
{
   assert (_target_ptr_set.find (target_ptr) == _target_ptr_set.end ());
   
   _target_ptr_set.insert (target_ptr);
}
#endif



/*
==============================================================================
Name : debug_remove
Description :
   Remove a target (a primitive) from the target set.
==============================================================================
*/

#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
void  Transaction::debug_remove (void * target_ptr)
{
   assert (_target_ptr_set.find (target_ptr) != _target_ptr_set.end ());
   
   _target_ptr_set.erase (target_ptr);
}
#endif



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : run_atom
==============================================================================
*/

#define Transaction_run_atom_COPY

void  Transaction::run_atom (const archi::UByte * & pos_ptr) const
{
   assert (pos_ptr + sizeof (Proc) + sizeof (void *) + sizeof (size_t) <= &_data [0] + _data.size ());
   
   Proc proc = 0;
   void * target_ptr = 0;
   size_t data_size = 0;
   
#if defined Transaction_run_atom_COPY
   std::vector <archi::UByte> data;
#endif
   
   memcpy (&proc, pos_ptr, sizeof (Proc));
   pos_ptr += sizeof (Proc);

   memcpy (&target_ptr, pos_ptr, sizeof (void *));
   pos_ptr += sizeof (void *);

   memcpy (&data_size, pos_ptr, sizeof (size_t));
   pos_ptr += sizeof (size_t);
   
   const archi::UByte * data_end_ptr = &_data [0] + _data.size ();
   
   assert (pos_ptr + data_size <= data_end_ptr);
   
   const archi::UByte * data_ptr = 0;
   
   if (data_size > 0)
   {
#if defined Transaction_run_atom_COPY
      data.resize (data_size);
      data_ptr = &data [0];
      
      memcpy (&data [0], pos_ptr, data_size);
#else
      data_ptr = pos_ptr;
#endif
      pos_ptr += data_size;
   }
   
   assert (proc != 0);
   assert (target_ptr != 0);
   
   proc (target_ptr, data_size, data_ptr);
}
   


/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
