/*****************************************************************************

        Cue.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 61605

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

#include "ohm/archi/types.h"
#include "ohm/flip/Cue.h"
#include "ohm/flip/ClassDescription.h"
#include "ohm/flip/ClassDescManager.h"
#include "ohm/flip/private/BasicTypesName.h"
#include "ohm/flip/private/MachineCode.h"
#include "ohm/flip/private/DocumentBase.h"

#include "ohm/util/trace.h"

#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : declare
==============================================================================
*/

void  Cue::declare ()
{
   ClassDescription <Cue>::use ().set_name (BasicTypesName::_cue_0);
   
   ClassDescManager::declare (ClassDescription <Cue>::use ());
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

Cue::Cue (DocumentBase & document)
:  BasicType (document)
,  _data ()
,  _pos_arr ()
,  _cnt_arr ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Cue::~Cue ()
{
   // destruction (undefine) is handled in base class Object
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

void  Cue::ctor (ohm::flip::Args & args)
{
   if (!args.empty ())
   {
      args.pop (*this);
   }
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

void  Cue::ctor ()
{
   fire ();
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

void  Cue::ctor (const archi::UByte * data_ptr, size_t data_size)
{
   fire (data_ptr, data_size);
}



/*
==============================================================================
Name : fire
==============================================================================
*/

void  Cue::fire ()
{
   make_transaction_and_set (0, 0);
}



/*
==============================================================================
Name : fire
==============================================================================
*/

void  Cue::fire (const archi::UByte * data_ptr, size_t data_size)
{
   make_transaction_and_set (data_ptr, data_size);
}



/*
==============================================================================
Name : was_fired
==============================================================================
*/

bool  Cue::was_fired () const
{
   return !_pos_arr.empty ();
}



/*
==============================================================================
Name : get_elem_cnt
==============================================================================
*/

size_t   Cue::get_elem_cnt () const
{
   assert (_pos_arr.size () == _cnt_arr.size ());
   
   return _pos_arr.size ();
}



/*
==============================================================================
Name : get_elem
==============================================================================
*/

void  Cue::get_elem (const archi::UByte * & data_ptr, size_t & data_size, int & direction, size_t elem_nbr) const
{
   assert (elem_nbr < get_elem_cnt ());
   
   size_t first_pos = _pos_arr [elem_nbr];
   direction = _cnt_arr [elem_nbr];
   assert (direction != 0);

   size_t second_pos;
   
   if (elem_nbr < get_elem_cnt () - 1)
   {
      second_pos = _pos_arr [elem_nbr + 1];
   }
   else
   {
      second_pos = _data.size ();
   }

   assert (second_pos >= first_pos);
   
   data_size = second_pos - first_pos;
   
   if (data_size != 0)
   {
      data_ptr = &_data [first_pos];
   }
   else
   {
      data_ptr = 0;
   }
}



/*
==============================================================================
Name : backup
==============================================================================
*/

void  Cue::backup ()
{
   assert (_pos_arr.empty ());
   assert (_cnt_arr.empty ());
}



/*
==============================================================================
Name : synchronize
==============================================================================
*/

void  Cue::synchronize ()
{
   _data.clear ();
   _pos_arr.clear ();
   _cnt_arr.clear ();
   
   Object::synchronize ();
}



/*
==============================================================================
Name : check_synchronized
==============================================================================
*/

void  Cue::check_synchronized () const
{
   assert (_data.empty ());
   assert (_pos_arr.empty ());
   assert (_cnt_arr.empty ());
   
   Object::check_synchronized ();
}



/*
==============================================================================
Name : check_inner_invalidation_cnt
==============================================================================
*/

void  Cue::check_inner_invalidation_cnt (size_t invalid_cnt) const
{
   if (!_pos_arr.empty ())
   {
      ohm_util_TRACE_DEBUG ("  CUE");
      assert (invalid_cnt >= _pos_arr.size ());
      invalid_cnt -= _pos_arr.size ();
   }
   
   Object::check_inner_invalidation_cnt (invalid_cnt);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : set_cue
Note :
   A 'data_size' of 0 is legal
==============================================================================
*/

void  Cue::set_cue (const archi::UByte * data_ptr, archi::UInt32 data_size, bool direction_normal_flag)
{
   const size_t elem_cnt = get_elem_cnt ();
   
   size_t i = 0;
   
   for (; i < elem_cnt ; ++i)
   {
      const archi::UByte * sub_data_ptr = 0;
      size_t sub_data_size = 0;
      int direction = 0;
      
      get_elem (sub_data_ptr, sub_data_size, direction, i);
      
      if (sub_data_size == data_size)
      {
         if (std::equal (data_ptr, data_ptr + data_size, sub_data_ptr))
         {
            _cnt_arr [i] += direction_normal_flag ? 1 : -1;
            
            if (_cnt_arr [i] == 0)
            {
               // remove
               
               _data.erase (
                  _data.begin () + _pos_arr [i],
                  _data.begin () + _pos_arr [i] + sub_data_size
               );
               
               _pos_arr.erase (_pos_arr.begin () + i);
               _cnt_arr.erase (_cnt_arr.begin () + i);
               
               size_t j = i;
               
               for (; j < elem_cnt - 1 ; ++j)
               {
                  assert (_pos_arr [i] >= sub_data_size);

                  _pos_arr [i] -= sub_data_size;
               }
            }
            
            break;
         }
      }
   }
   
   if (i == elem_cnt)
   {
      _pos_arr.push_back (_data.size ());
      _cnt_arr.push_back (direction_normal_flag ? 1 : -1);
      _data.insert (_data.end (), data_ptr, data_ptr + data_size);

      invalidate ();
   }
}



/*
==============================================================================
Name : make_transaction_and_set
==============================================================================
*/

void  Cue::make_transaction_and_set (const archi::UByte * data_ptr, size_t data_size)
{
   Data & transaction = use_document ().use_transaction ();

   Ref this_ref = get_ref ();
   assert (this_ref != 0);
   
   if (data_size > 0)
   {
      MachineCode::build_cue (
         transaction, this_ref,
         data_ptr, data_size
      );
   }
   else
   {
      MachineCode::build_cue (
         transaction, this_ref,
         0, 0
      );
   }
   
   bool direction_normal_flag = true;
   
   set_cue (data_ptr, data_size, direction_normal_flag);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
