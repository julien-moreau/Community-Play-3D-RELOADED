/*****************************************************************************

        TypeTicker.hpp
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



#if defined (ohm_opak_TypeTicker_CURRENT_CODEHEADER)
   #error Recursive inclusion of TypeTicker code header.
#endif
#define  ohm_opak_TypeTicker_CURRENT_CODEHEADER

#if ! defined (ohm_opak_TypeTicker_CODEHEADER_INCLUDED)
#define  ohm_opak_TypeTicker_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/private/Animator.h"
#include "ohm/opak/private/TransactionMaster.h"
#include "ohm/lang/fnc.h"



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class Target>
TypeTicker <Target>::TypeTicker (Target & target, Proc proc)
:  _target (target)
,  _proc (proc)

,  _main_thread_running_flag (false)

,  _animating_flag (false)
{
   assert (&_target != 0);
   
#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
   TransactionMaster::use ().debug_add (this);
#endif   
   
   _target.add_animatable (*this);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class Target>
TypeTicker <Target>::~TypeTicker ()
{
   assert (!_animating_flag);
   
   _target.remove_animatable (*this);
}



/*
==============================================================================
Name : start
==============================================================================
*/

template <class Target>
void  TypeTicker <Target>::start ()
{
   _main_thread_running_flag = true;
   
   TransactionMaster::use ().push (
      do_start_proc, this, 0, 0
   );
}



/*
==============================================================================
Name : stop
==============================================================================
*/

template <class Target>
void  TypeTicker <Target>::stop ()
{
   _main_thread_running_flag = false;
   
   TransactionMaster::use ().push (
      do_stop_proc, this, 0, 0
   );
}



/*
==============================================================================
Name : do_start_proc
==============================================================================
*/

template <class Target>
void  TypeTicker <Target>::do_start_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   assert (target_ptr != 0);
   assert (data_size == 0);
   assert (data_ptr == 0);
   
   TypeTicker * this_ptr = reinterpret_cast <TypeTicker *> (target_ptr);
   
   this_ptr->do_start ();
}



/*
==============================================================================
Name : do_stop_proc
==============================================================================
*/

template <class Target>
void  TypeTicker <Target>::do_stop_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   assert (target_ptr != 0);
   assert (data_size == 0);
   assert (data_ptr == 0);
   
   TypeTicker * this_ptr = reinterpret_cast <TypeTicker *> (target_ptr);
   
   this_ptr->do_stop ();
}



/*
==============================================================================
Name : do_start
==============================================================================
*/

template <class Target>
void  TypeTicker <Target>::do_start ()
{
   animate ();
}



/*
==============================================================================
Name : do_stop
==============================================================================
*/

template <class Target>
void  TypeTicker <Target>::do_stop ()
{
   deanimate ();
}



/*
==============================================================================
Name : is_animating
==============================================================================
*/

template <class Target>
bool  TypeTicker <Target>::is_animating () const
{
   return _animating_flag;
}



/*
==============================================================================
Name : tick
==============================================================================
*/

template <class Target>
void  TypeTicker <Target>::tick (double time_s)
{
   call_proc ();
}



/*
==============================================================================
Name : mt_stop
==============================================================================
*/

template <class Target>
void  TypeTicker <Target>::mt_stop ()
{
   stop ();
}



/*
==============================================================================
Name : vt_stop
==============================================================================
*/

template <class Target>
void  TypeTicker <Target>::vt_stop ()
{
   do_stop ();
}



/*
==============================================================================
Name : vt_is_animating
==============================================================================
*/

template <class Target>
bool  TypeTicker <Target>::vt_is_animating ()
{
   return is_animating ();
}



/*
==============================================================================
Name : notify_parent_destroyable
==============================================================================
*/

#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
template <class Target>
void  TypeTicker <Target>::notify_parent_destroyable ()
{
   _target.remove_type_debug (*this);
   TransactionMaster::use ().debug_remove (this);
}
#endif



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : call_proc
==============================================================================
*/

template <class Target>
void  TypeTicker <Target>::call_proc ()
{
   assert (&_target != 0);
   (_target.*_proc) ();
}



/*
==============================================================================
Name : animate
==============================================================================
*/

template <class Target>
void  TypeTicker <Target>::animate ()
{
   if (!_animating_flag)
   {
      Animator::use_instance ().insert (*this);
      
      _animating_flag = true;
   }
}



/*
==============================================================================
Name : deanimate
==============================================================================
*/

template <class Target>
void  TypeTicker <Target>::deanimate ()
{
   if (_animating_flag)
   {
      Animator::use_instance ().erase (*this);
      
      _animating_flag = false;
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



#endif   // ohm_opak_TypeTicker_CODEHEADER_INCLUDED

#undef ohm_opak_TypeTicker_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
