/*****************************************************************************

        TypeKeyframable.h
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



#if ! defined (ohm_opak_TypeKeyframable_HEADER_INCLUDED)
#define  ohm_opak_TypeKeyframable_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/private/AnimatorListener.h"
#include "ohm/opak/base/Animatable.h"
#include "ohm/opak/base/TransitionFunction.h"
#include "ohm/opak/base/TransitionDelay.h"
#include "ohm/opak/private/CellList.h"
#include "ohm/opak/private/TransactionDebug.h"
#include "ohm/opak/private/TypeDebug.h"

#include <list>

// reduce memory usage
#undef ohm_opak_TypeKeyframable_RMU



namespace ohm
{
namespace opak
{



template <class Target, class AssignedType, class ConstAssignedTypeRef, class StoredType>
class TypeKeyframable
:  public AnimatorListener
,  public Animatable
#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
,  public TypeDebug
#endif
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef void (Target::*Proc)();
   
                  TypeKeyframable (Target & target, Proc proc, ConstAssignedTypeRef default_assigned_val, const StoredType & default_stored_val);
                  TypeKeyframable (Target & target, Proc proc, Proc prev_proc, ConstAssignedTypeRef default_val, const StoredType & default_stored_val);
   virtual        ~TypeKeyframable ();
   void           enable_stabilization ();
   
   void           init (ConstAssignedTypeRef value);
   
#if ! defined (ohm_opak_TypeKeyframable_RMU)
   void           set_transition_time (double delay_s);
   void           set_transition_function (TransitionFunction function);
#endif
   
   // Video Thread
   static void    do_init_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr);
   static void    do_push_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr);
   static void    do_loop_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr);
   static void    do_stop_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr);

   void           do_init (ConstAssignedTypeRef value);
   void           do_push (ConstAssignedTypeRef value, double transition_time, TransitionFunction transition_function);
   void           do_loop ();
   void           do_stop ();

   bool           is_animating () const;
   const StoredType &
                  get_cur_value () const;
   const StoredType &
                  get_prev_value () const;
   
   // AnimatorListener (in Video Thread)
   virtual void   tick (double time_s);
   
   // Animatable
   virtual void   mt_stop ();
   virtual void   vt_stop ();
   virtual bool   vt_is_animating ();
   
   // TypeDebug
#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
   virtual void   notify_parent_destroyable ();
#endif
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   void           assign (ConstAssignedTypeRef value);
   ConstAssignedTypeRef
                  get_mt_value () const;
   void           sp_push (ConstAssignedTypeRef value, double transition_time, TransitionFunction transition_function);
   void           sp_loop ();
   void           sp_stop ();
   
   virtual void   do_convert_push (archi::UByte * dst_ptr, ConstAssignedTypeRef src);
   virtual void   do_convert_pop (AssignedType & dst, const archi::UByte * src_ptr);
   virtual void   do_convert (StoredType & dst, ConstAssignedTypeRef src) = 0;
   virtual void   do_interpolate (AssignedType & dst, ConstAssignedTypeRef start_val, ConstAssignedTypeRef target_val, float interp_val) = 0;
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum State
   {
                  State_STOPPED = 0,
                  State_ANIMATING,
                  State_STABILIZATING,
   };
   
   class KeyFrame
   {
   public:
      AssignedType
                  _start_val;
      double      _start_time;
      AssignedType
                  _target_val;
      double      _target_time;
      double      _transition_time;
      TransitionFunction
                  _function;
   };
   
   //typedef std::list <KeyFrame> KeyFrameList;
   typedef CellList <KeyFrame>   KeyFrameList;
   
   void           stabilize ();
   void           call_proc ();
   void           call_prev_proc ();
   
   void           animate ();
   void           deanimate ();
   
   void           flush ();
   void           prepare (KeyFrame & key_frame, ConstAssignedTypeRef value, double transition_time, TransitionFunction transition_function);

   void           interpolate (AssignedType & value, const KeyFrame & key_frame, double time_s);
   
   void           set_new_cur_val (ConstAssignedTypeRef value);
   
   bool           can_loop () const;
   
   // Immutable data
   
   Target &       _target;
   Proc           _proc;
   Proc           _prev_proc;

   // Main thread

#if ! defined (ohm_opak_TypeKeyframable_RMU)
   double         _transition_time_s;
   TransitionFunction
                  _transition_function;
#endif
   AssignedType   _main_thread_target_val;
   
   // Video thread
   
   StoredType     _prev_stored_val;
   StoredType     _cur_stored_val;
   AssignedType   _cur_assigned_val;
   State          _state;
   
   KeyFrameList   _key_frame_list;
   double         _key_frame_total_time;
   
   // Immutable data

   bool           _stabilization_flag;
   
   // Video thread
   
   bool           _loop_flag;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  TypeKeyframable ();
                  TypeKeyframable (const TypeKeyframable &other);
   bool           operator == (const TypeKeyframable &other);
   bool           operator != (const TypeKeyframable &other);

}; // class TypeKeyframable



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/opak/base/TypeKeyframable.hpp"



#endif   // ohm_opak_TypeKeyframable_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
