/*****************************************************************************

        TypeKeyframable.hpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 58552

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



#if defined (ohm_opak_TypeKeyframable_CURRENT_CODEHEADER)
   #error Recursive inclusion of TypeKeyframable code header.
#endif
#define  ohm_opak_TypeKeyframable_CURRENT_CODEHEADER

#if ! defined (ohm_opak_TypeKeyframable_CODEHEADER_INCLUDED)
#define  ohm_opak_TypeKeyframable_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/private/Animator.h"
#include "ohm/opak/private/TransactionMaster.h"
#include "ohm/lang/fnc.h"

#include <cmath>



namespace ohm
{
namespace opak
{



/*
==============================================================================
                                 MODEL
   
   Internally, the proxy maintains a queue of animation with each element
   (target_val, transition_time, transition_function)
   
   Cumulative mode : a new element is pushed into the queue
   Immediate mode : the queue is flushed and the element is pushed into the
   queue.
   
   prev_proc : some primitive requires to know if the cur_val is going to
   change. Therefore if we are to change the value of cur_val, we need
   to call the callback before.
   
   stabilization : some primitive aspect depends on both cur_val & prev_val
   (that is not only cur_val). When stabilized, the callback will fire until
   prev_val == cur_val (that is we don't stop when cur_val reaches target_val)
   
   State properties : delay, function are state properties of this proxy.
   This means that they are implicitly accumulated in immediate mode, and
   changing them have no effect on the current animation.
   
   operator type () : the user except the following :
   a = b => a == b. So in command thread, the return value is the last one
   previously set. (the latest element pushed in cumulative mode)
   
   Loop : the animation can be looped. In that case, when a queue element
   has been fully executed, it is removed from front and pushed to back.
   Normally before starting a loop animation, the client would flush the
   current queue.
   
==============================================================================
*/



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::TypeKeyframable (Target & target, Proc proc, ConstAssignedTypeRef default_assigned_val, const StoredType & default_stored_val)
:  _target (target)
,  _proc (proc)
,  _prev_proc (0)
,  _stabilization_flag (false)

#if ! defined (ohm_opak_TypeKeyframable_RMU)
,  _transition_time_s (0.0)
,  _transition_function (step)
#endif
,  _main_thread_target_val (default_assigned_val)

,  _prev_stored_val (default_stored_val)
,  _cur_stored_val (default_stored_val)
,  _cur_assigned_val (default_assigned_val)
,  _state (State_STOPPED)
,  _loop_flag (false)
,  _key_frame_list ()
,  _key_frame_total_time (0.0)
{
   assert (&_target != 0);
   
#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
   TransactionMaster::use ().debug_add (this);
   _target.add_type_debug (*this);
#endif   
   
   _target.add_animatable (*this);
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::TypeKeyframable (Target & target, Proc proc, Proc prev_proc, ConstAssignedTypeRef default_assigned_val, const StoredType & default_stored_val)
:  _target (target)
,  _proc (proc)
,  _prev_proc (prev_proc)
,  _stabilization_flag (false)

#if ! defined (ohm_opak_TypeKeyframable_RMU)
,  _transition_time_s (0.0)
,  _transition_function (step)
#endif
,  _main_thread_target_val (default_assigned_val)

,  _prev_stored_val (default_stored_val)
,  _cur_stored_val (default_stored_val)
,  _cur_assigned_val (default_assigned_val)
,  _state (State_STOPPED)
,  _loop_flag (false)
,  _key_frame_list ()
,  _key_frame_total_time (0.0)
{
   assert (&_target != 0);

#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
   TransactionMaster::use ().debug_add (this);
   _target.add_type_debug (*this);
#endif   
   
   _target.add_animatable (*this);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::~TypeKeyframable ()
{
   assert (_state == State_STOPPED);
   
   _target.remove_animatable (*this);
}



/*
==============================================================================
Name : enable_stabilization
Description :
   Require stabilization for 'this'.
   Stabilization is conceptually immutable, so this must be called in the
   ctor that use this class.
   Therefore this call is not thread safe : you cannot change stabilization
   dynamically.
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::enable_stabilization ()
{
   // check somehow that we are still at ctor stage
   // while not being a complete check, this might help
   // to find programming errors
   
#if ! defined (ohm_opak_TypeKeyframable_RMU)
   assert (_transition_time_s == 0.0);
#endif

   assert (!_loop_flag);
   assert (_key_frame_list.empty ());
   
   _stabilization_flag = true;
}



/*
==============================================================================
Name : init
Description :
   Similar to constructor with default parameter, so we don't
   trigger an animation, but we issue a notification.
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::init (ConstAssignedTypeRef value)
{
   _main_thread_target_val = value;
   
   archi::UByte buf [sizeof (AssignedType)];
   
   do_convert_push (buf, value);
   
   TransactionMaster::use ().push (
      do_init_proc, this, sizeof (AssignedType), buf
   );
}



/*
==============================================================================
Name : set_transition_time
==============================================================================
*/

#if ! defined (ohm_opak_TypeKeyframable_RMU)
template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::set_transition_time (double delay_s)
{
   if (delay_s < 0.0)
   {
      delay_s = 0.0;
   }
   
   _transition_time_s = delay_s;
}
#endif



/*
==============================================================================
Name : set_transition_function
==============================================================================
*/

#if ! defined (ohm_opak_TypeKeyframable_RMU)
template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::set_transition_function (TransitionFunction function)
{
   _transition_function = function;
}
#endif



/*
==============================================================================
Name : do_init_proc
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::do_init_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   assert (target_ptr != 0);
   assert (data_size == sizeof (AssignedType));
   assert (data_ptr != 0);
   
   TypeKeyframable * this_ptr = reinterpret_cast <TypeKeyframable *> (target_ptr);

   AssignedType value;
   this_ptr->do_convert_pop (value, data_ptr);
   
   this_ptr->do_init (value);
}



/*
==============================================================================
Name : do_push_proc
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::do_push_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   assert (target_ptr != 0);
   assert (data_size == sizeof (AssignedType) + sizeof (double) + sizeof (TransitionFunction));
   assert (data_ptr != 0);
   
   TypeKeyframable * this_ptr = reinterpret_cast <TypeKeyframable *> (target_ptr);
   
   AssignedType value;
   this_ptr->do_convert_pop (value, (archi::UByte *) data_ptr);
   
   data_ptr += sizeof (AssignedType);
   
   double transition_time = *reinterpret_cast <const double *> (data_ptr);
   data_ptr += sizeof (double);
   
   //union Union {archi::UByte _data [sizeof (TransitionFunction)]; TransitionFunction _fnc;};
   
   const TransitionFunction & transition_function = *reinterpret_cast <const TransitionFunction *> (data_ptr);
   
   this_ptr->do_push (value, transition_time, transition_function);
}



/*
==============================================================================
Name : do_loop_proc
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::do_loop_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   assert (target_ptr != 0);
   assert (data_size == 0);
   assert (data_ptr == 0);
   
   TypeKeyframable * this_ptr = reinterpret_cast <TypeKeyframable *> (target_ptr);
   
   this_ptr->do_loop ();
}



/*
==============================================================================
Name : do_stop_proc
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::do_stop_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   assert (target_ptr != 0);
   assert (data_size == 0);
   assert (data_ptr == 0);
   
   TypeKeyframable * this_ptr = reinterpret_cast <TypeKeyframable *> (target_ptr);
   
   this_ptr->do_stop ();
}



/*
==============================================================================
Name : do_init
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::do_init (ConstAssignedTypeRef value)
{
   deanimate ();
   
   flush ();
   
   call_prev_proc ();
   
   do_convert (_prev_stored_val, value);
   _cur_assigned_val = value;
   do_convert (_cur_stored_val, value);
   
   call_proc ();
}



/*
==============================================================================
Name : do_push
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::do_push (ConstAssignedTypeRef value, double transition_time, TransitionFunction transition_function)
{
   KeyFrame key_frame;
   
   prepare (key_frame, value, transition_time, transition_function);
   
   _key_frame_list.push_back (key_frame);
   
   animate ();
}



/*
==============================================================================
Name : do_loop
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::do_loop ()
{
   _loop_flag = can_loop ();
   
   assert (_loop_flag);
   
   if (_loop_flag)
   {
      animate ();
   }
}



/*
==============================================================================
Name : stop
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::do_stop ()
{
   if (is_animating ())
   {
      double time = Animator::use_instance ().get_time ();
      
      tick (time);
   }  
   
   flush ();
      
   stabilize ();
}



/*
==============================================================================
Name : is_animating
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
bool  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::is_animating () const
{
   return _state != State_STOPPED;
}



/*
==============================================================================
Name : get_cur_value
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
const StoredType &   TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::get_cur_value () const
{
   return _cur_stored_val;
}



/*
==============================================================================
Name : get_prev_value
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
const StoredType &   TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::get_prev_value () const
{
   return _prev_stored_val;
}



/*
==============================================================================
Name : tick
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::tick (double time_s)
{
   if (_loop_flag)
   {
      assert (_key_frame_list.size () >= 2);
      assert (_key_frame_total_time > 0.0);
      
      double start_time = _key_frame_list.front ()._start_time;
      
      double diff_time = time_s - start_time;
      
      if (diff_time >= 0.0)
      {
         double advance_time
            = floor (diff_time / _key_frame_total_time)
            * _key_frame_total_time;
         
         typename KeyFrameList::iterator it = _key_frame_list.begin ();
         const typename KeyFrameList::iterator it_end = _key_frame_list.end ();
         typename KeyFrameList::iterator fit = _key_frame_list.end ();
         
         // search for the current keyframe & update times
         
         for (; it != it_end ; ++it)
         {
            KeyFrame & key_frame = *it;
            
            key_frame._start_time += advance_time;
            key_frame._target_time += advance_time;
            
            if (
               (fit == it_end)
               && (key_frame._start_time <= time_s)
               && (time_s <= key_frame._target_time)
            )
            {
               fit = it;
            }
         }
         
         if (fit != it_end)
         {
            KeyFrame & key_frame = *fit;
            
            AssignedType cur_val;
            interpolate (cur_val, key_frame, time_s);
            
            set_new_cur_val (cur_val);
         }
         else
         {
            // 'time_s' might slighty bigger than the last 'key_frame._target_time'
            // due to calculations imprecisions
            
            const KeyFrame & key_frame = _key_frame_list.back ();
            
            set_new_cur_val (key_frame._target_val);
         }
      }
      else
      {
         const KeyFrame & key_frame = _key_frame_list.front ();
         
         set_new_cur_val (key_frame._start_val);
      }
   }
   else
   {
      if (_state == State_STABILIZATING)
      {
         stabilize ();
      }
      else
      {
         typename KeyFrameList::iterator it = _key_frame_list.begin ();
         const typename KeyFrameList::iterator it_end = _key_frame_list.end ();
         
         // search for the current keyframe
         
         bool processed_flag = false;
         AssignedType cur_val;
         
         if (!_key_frame_list.empty ())
         {
            const KeyFrame & key_frame = _key_frame_list.back ();
            
            if (time_s >= key_frame._target_time)
            {
               // optimisation :
               // time equality is a very common case
               
               it = it_end;
               processed_flag = true;
            }
         }
         
         if (!processed_flag)
         {
            for (; it != it_end ; ++it)
            {
               KeyFrame & key_frame = *it;
               
               if ((key_frame._start_time <= time_s) && (time_s <= key_frame._target_time))
               {
                  break;
               }
            }
         }
         
         if (it == it_end)
         {
            // we apply the last value
            
            assert (!_key_frame_list.empty ());
            
            KeyFrame & key_frame = _key_frame_list.back ();
            
            cur_val = key_frame._target_val;
         }
         else
         {
            KeyFrame & key_frame = *it;
            
            interpolate (cur_val, key_frame, time_s);
         }
         
         _key_frame_list.erase (_key_frame_list.begin (), it);
         
         set_new_cur_val (cur_val);

         bool stop_flag
            = _key_frame_list.empty ()
            && ((!_stabilization_flag) || (_prev_stored_val == _cur_stored_val));

         if (stop_flag)
         {
            _prev_stored_val = _cur_stored_val;
            
            deanimate ();
         }
         else if (_stabilization_flag)
         {
            _state = State_STABILIZATING;
         }
      }
   }
}



/*
==============================================================================
Name : mt_stop
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::mt_stop ()
{
   sp_stop ();
}



/*
==============================================================================
Name : vt_stop
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::vt_stop ()
{
   do_stop ();
}



/*
==============================================================================
Name : vt_is_animating
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
bool  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::vt_is_animating ()
{
   return is_animating ();
}



/*
==============================================================================
Name : notify_parent_destroyable
==============================================================================
*/

#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::notify_parent_destroyable ()
{
   _target.remove_type_debug (*this);
   TransactionMaster::use ().debug_remove (this);
}
#endif



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : assign
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::assign (ConstAssignedTypeRef value)
{
   sp_stop ();

#if defined (ohm_opak_TypeKeyframable_RMU)
   sp_push (value, 0.0, step);
#else
   sp_push (value, _transition_time_s, _transition_function);
#endif
}



/*
==============================================================================
Name : get_mt_value
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
ConstAssignedTypeRef TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::get_mt_value () const
{
   return _main_thread_target_val;
}



/*
==============================================================================
Name : sp_push
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::sp_push (ConstAssignedTypeRef value, double transition_time, TransitionFunction transition_function)
{
   _main_thread_target_val = value;
   
   archi::UByte buf [sizeof (AssignedType) + sizeof (double) + sizeof (TransitionFunction)];
   
   do_convert_push (buf, value);
   reinterpret_cast <double &> (buf [sizeof (AssignedType)]) = transition_time;
   reinterpret_cast <TransitionFunction &> (buf [sizeof (AssignedType) + sizeof (double)])
      = transition_function;

   TransactionMaster::use ().push (
      do_push_proc, this,
      sizeof (AssignedType) + sizeof (double) + sizeof (TransitionFunction), buf
   );
}



/*
==============================================================================
Name : sp_loop
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::sp_loop ()
{
   TransactionMaster::use ().push (do_loop_proc, this, 0, 0);
}



/*
==============================================================================
Name : sp_stop
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::sp_stop ()
{
   TransactionMaster::use ().push (do_stop_proc, this, 0, 0);
}



/*
==============================================================================
Name : do_convert_push
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::do_convert_push (archi::UByte * dst_ptr, ConstAssignedTypeRef src)
{
   new (&dst_ptr [0]) AssignedType (src);
}



/*
==============================================================================
Name : do_convert_pop
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::do_convert_pop (AssignedType & dst, const archi::UByte * src_ptr)
{
   const AssignedType * val_ptr = reinterpret_cast <const AssignedType *> (&src_ptr [0]);
   
   dst = *val_ptr;
   
   val_ptr->~AssignedType ();
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : stabilize
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::stabilize ()
{
   if (_stabilization_flag && (_cur_stored_val != _prev_stored_val))
   {
      call_prev_proc ();
      
      _prev_stored_val = _cur_stored_val;
      
      call_proc ();
   }
   
   deanimate ();
}



/*
==============================================================================
Name : call_proc
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::call_proc ()
{
   assert (&_target != 0);
   (_target.*_proc) ();
}



/*
==============================================================================
Name : call_prev_proc
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::call_prev_proc ()
{
   if (_prev_proc != 0)
   {
      assert (&_target != 0);
      (_target.*_prev_proc) ();
   }
}



/*
==============================================================================
Name : animate
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::animate ()
{
   if (_state == State_STOPPED)
   {
      Animator::use_instance ().insert (*this);
      
      _state = State_ANIMATING;
   }
}



/*
==============================================================================
Name : deanimate
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::deanimate ()
{
   if (_state != State_STOPPED)
   {
      Animator::use_instance ().erase (*this);
      
      _state = State_STOPPED;
   }
}



/*
==============================================================================
Name : flush
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::flush ()
{
   _key_frame_list.clear ();
   _loop_flag = false;
   _key_frame_total_time = 0.0;
}



/*
==============================================================================
Name : prepare
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::prepare (KeyFrame & key_frame, ConstAssignedTypeRef value, double transition_time, TransitionFunction transition_function)
{
   if (_key_frame_list.empty ())
   {
      key_frame._start_val = _cur_assigned_val;
      key_frame._start_time = Animator::use_instance ().get_time ();
   }
   else
   {
      key_frame._start_val = _key_frame_list.back ()._target_val;
      key_frame._start_time = _key_frame_list.back ()._target_time;
   }
   
   key_frame._target_val = value;
   key_frame._target_time = key_frame._start_time + transition_time;
   key_frame._transition_time = transition_time;
   key_frame._function = transition_function;
   
   _key_frame_total_time += transition_time;
}



/*
==============================================================================
Name : interpolate
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::interpolate (AssignedType & value, const KeyFrame & key_frame, double time_s)
{
   double dt = key_frame._target_time - key_frame._start_time;
   assert (dt >= 0);
   
   double t = 1.f;
   
   if (dt > 0)
   {
      double rel_time = time_s - key_frame._start_time;
      t = rel_time / dt;
      
      assert (t >= 0.f);
      assert (t <= 1.f);
   }
   
   float interp_val = key_frame._function (float (t));
   
   do_interpolate (value, key_frame._start_val, key_frame._target_val, interp_val);
}



/*
==============================================================================
Name : set_new_cur_val
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
void  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::set_new_cur_val (ConstAssignedTypeRef value)
{
   bool notify_flag = _stabilization_flag || (value != _cur_assigned_val);
   
   if (notify_flag)
   {
      call_prev_proc ();
   }
      
   _prev_stored_val = _cur_stored_val;
   _cur_assigned_val =  value;
   do_convert (_cur_stored_val, value);
   
   if (notify_flag)
   {
      call_proc ();
   }
}



/*
==============================================================================
Name : can_loop
==============================================================================
*/

template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
bool  TypeKeyframable <Target, AssignedType, ConstAssignedTypeRef, StoredType>::can_loop () const
{
   bool can_loop_flag = false;
   
   if (_key_frame_list.size () >= 2)
   {
      // at least 2 keyframes with non zero transition time are needed
      
      typename KeyFrameList::const_iterator it = _key_frame_list.begin ();
      const typename KeyFrameList::const_iterator it_end = _key_frame_list.end ();
      
      size_t cnt = 0;
      
      for (; it != it_end ; ++it)
      {
         const KeyFrame & key_frame = *it;
         
         if (key_frame._target_time > key_frame._start_time)
         {
            ++cnt;
         }
      }
      
      can_loop_flag = cnt >= 2;
   }
   
   return can_loop_flag;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



#endif   // ohm_opak_TypeKeyframable_CODEHEADER_INCLUDED

#undef ohm_opak_TypeKeyframable_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
