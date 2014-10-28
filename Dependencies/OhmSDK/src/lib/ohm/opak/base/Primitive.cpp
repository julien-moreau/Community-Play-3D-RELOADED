/*****************************************************************************

        Primitive.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 67384

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

#include "ohm/archi/def.h"
#include "ohm/opak/base/Primitive.h"
#include "ohm/opak/base/PrimitiveContainer.h"
#include "ohm/lang/fnc.h"

#if defined (OHM_OPA_DEBUGGER)
   #include "ohm/opa/Debugger.h"
   #include "ohm/util/trace.h"
#endif

#include <cassert>



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Thread : Main
==============================================================================
*/

Primitive::Primitive ()
:  _visibility (*this, &Primitive::visibility_changed)
,  _origin (*this, &Primitive::origin_changed, &Primitive::origin_changed_prev)

,  _name_0 (0)
,  _bbox_valid_flag (true)
,  _bbox ()
,  _destroyable_flag (false)
,  _parent_ptr_arr ()
,  _keep_alive_flag (*this, &Primitive::nothing_changed)
,  _ref_cnt (0)
{
#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
   TransactionMaster::use ().debug_add (this);
#endif
   
   TransactionMaster::use ().push (
      do_init_display_proc, this, 0, 0
   );

   _visibility.init (true);
   _origin.init (opa::Point::_zero);
}



/*
==============================================================================
Name : set_name
Description :
   Set optionnal name to the 'this'. This is used for debugging purpose
   only. 'name_0' should be static.
==============================================================================
*/

void  Primitive::set_name (const char * name_0)
{
   assert (_name_0 == 0);
   assert (name_0 != 0);
   
   _name_0 = name_0;
}



/*
==============================================================================
Name: get_name
Description :
   See 'set_name'
==============================================================================
*/

const char * Primitive::get_name () const
{
   return _name_0;
}



/*
==============================================================================
Name : remove_from_parents
Thread : Main
Description :
   Remove 'this' primitive from *all* its parents. The removal itself
   will wait for any animation that 'this' or 'this' children may run.
==============================================================================
*/

void  Primitive::remove_from_parents ()
{
   archi::UByte val = 0;

   TransactionMaster::use ().push (
      do_remove_from_parents_proc, this, 1, &val
   );
}



/*
==============================================================================
Name : remove_from_parents_immediate
Thread : Main
Description :
   Remove 'this' primitive from *all* its parents. The removal itself
   will be done immediatly. It won't wait for animations that 'this' or 'this'
   children might run.
==============================================================================
*/

void  Primitive::remove_from_parents_immediate ()
{
   archi::UByte val = 1;

   TransactionMaster::use ().push (
      do_remove_from_parents_proc, this, 1, &val
   );
}



/*
==============================================================================
Name : set_order_nbr
Thread : Main
Description :
   Set the order number in parent primitive.
   This is generally followed by a call to 'sort' in the parent when all
   children order have been set.
==============================================================================
*/

void  Primitive::set_order_nbr (size_t order)
{
   TransactionMaster::use ().push (
      do_set_order_nbr_proc, this, sizeof (size_t), (archi::UByte *) &order
   );
}



/*
==============================================================================
Name : keep_alive
Description :
   Helper function that will fake an animation to delay removal of a primitive
   from its parents.
==============================================================================
*/

void  Primitive::keep_alive (double time)
{
   _keep_alive_flag.stop ().push (true, time);
}



/*
==============================================================================
Name : add_animatable
Thread : Main
Description :
   Internal. This is used to add the different 'Type*' to the primitive.
==============================================================================
*/

void  Primitive::add_animatable (Animatable & animatable)
{
   assert (&animatable != 0);
   
   _animatable_ptr_arr.push_back (&animatable);
}



/*
==============================================================================
Name : remove_animatable
Description :
   Internal. This is used to remove the different 'Type*' to the primitive.
==============================================================================
*/

void  Primitive::remove_animatable (Animatable & animatable)
{
   assert (&animatable != 0);
   
   size_t size = _animatable_ptr_arr.size ();
   
   for (size_t i = 0 ; i < size ; ++i)
   {
      Animatable * & animatable_ptr = _animatable_ptr_arr [i];
      // animatable_ptr may be null
      
      if (animatable_ptr == &animatable)
      {
         animatable_ptr = 0;
      }
   }
}



/*
==============================================================================
Name : add_type_debug
Description :
   Internal. Debug.
==============================================================================
*/

#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
void  Primitive::add_type_debug (TypeDebug & type_debug)
{
   assert (&type_debug != 0);
   
   _type_debug_ptr_arr.push_back (&type_debug);
}
#endif



/*
==============================================================================
Name : remove_type_debug
Description :
   Internal. Debug.
==============================================================================
*/

#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
void  Primitive::remove_type_debug (TypeDebug & type_debug)
{
   assert (&type_debug != 0);
   
   size_t size = _type_debug_ptr_arr.size ();
   
   for (size_t i = 0 ; i < size ; ++i)
   {
      TypeDebug * & type_debug_ptr = _type_debug_ptr_arr [i];
      // type_debug_ptr may be null
      
      if (type_debug_ptr == &type_debug)
      {
         type_debug_ptr = 0;
      }
   }
}
#endif



/*
==============================================================================
Name : stop
Thread : Main
Description :
   Stop every animations on 'this' primitive.
==============================================================================
*/

void  Primitive::stop ()
{
   size_t size = _animatable_ptr_arr.size ();
   
   for (size_t i = 0 ; i < size ; ++i)
   {
      Animatable * animatable_ptr = _animatable_ptr_arr [i];
      assert (animatable_ptr != 0);
      
      animatable_ptr->mt_stop ();
   }
}



/*
==============================================================================
Name : do_init_display_proc
Thread : Video
==============================================================================
*/

void  Primitive::do_init_display_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   assert (target_ptr != 0);
   assert (data_size == 0);
   assert (data_ptr == 0);
   
   Primitive * this_ptr = reinterpret_cast <Primitive *> (target_ptr);
   
   this_ptr->do_init_display ();
}



/*
==============================================================================
Name : do_set_destroyable_proc
Thread : Video
==============================================================================
*/

void  Primitive::do_set_destroyable_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   assert (target_ptr != 0);
   assert (data_size == 0);
   assert (data_ptr == 0);
   
   Primitive * this_ptr = reinterpret_cast <Primitive *> (target_ptr);
   
   this_ptr->do_set_destroyable ();
}



/*
==============================================================================
Name : do_remove_from_parents_proc
Thread : Video
==============================================================================
*/

void  Primitive::do_remove_from_parents_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   assert (target_ptr != 0);
   assert (data_size == sizeof (archi::UByte));
   assert (data_ptr != 0);
   
   Primitive * this_ptr = reinterpret_cast <Primitive *> (target_ptr);
   
   archi::UByte immediate_flag = *data_ptr;
   
   this_ptr->do_remove_from_parents (immediate_flag != 0);
}



/*
==============================================================================
Name : do_set_order_nbr_proc
Thread : Video
==============================================================================
*/

void  Primitive::do_set_order_nbr_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   assert (target_ptr != 0);
   assert (data_size == sizeof (size_t));
   assert (data_ptr != 0);
   
   Primitive * this_ptr = reinterpret_cast <Primitive *> (target_ptr);
   size_t value = *reinterpret_cast <const size_t *> (data_ptr);
   
   this_ptr->do_set_order_nbr (value);
}



/*
==============================================================================
Name : do_init_display
Thread : Video
==============================================================================
*/

void  Primitive::do_init_display ()
{
   init_display ();
}



/*
==============================================================================
Name : do_set_destroyable
Thread : Video
==============================================================================
*/

void  Primitive::do_set_destroyable ()
{
   _destroyable_flag = true;
   
   if (is_detachable ())
   {
      do_destroy ();
   }
}



/*
==============================================================================
Name : do_destroy
Thread : Video
==============================================================================
*/

void  Primitive::do_destroy ()
{
#if defined (OHM_OPA_DEBUGGER)
#if (ohm_archi_SYS != ohm_archi_SYS_WIN)
   if (opa::Debugger::use_instance ().has_break (*this, &Primitive::do_destroy))
   {
      ohm_util_TRACE_DEBUGGER ();
   }
#endif
#endif

   assert (is_detachable ());
   
   do_stop ();
   
   restore_display ();
         
   delete this;
}



/*
==============================================================================
Name : do_remove_from_parents
Thread : Video
==============================================================================
*/

void  Primitive::do_remove_from_parents (bool immediate_flag)
{
#if defined (OHM_OPA_DEBUGGER)
#if (ohm_archi_SYS != ohm_archi_SYS_WIN)
   if (opa::Debugger::use_instance ().has_break (*this, &Primitive::do_remove_from_parents))
   {
      ohm_util_TRACE_DEBUGGER ();
   }
#endif
#endif

   for (size_t i = 0 ; i < _parent_ptr_arr.size () ;)
   {
      Primitive * parent_ptr = _parent_ptr_arr [i];
      
      PrimitiveContainer * container_ptr = dynamic_cast <PrimitiveContainer *> (parent_ptr);
      
      size_t cur_size = _parent_ptr_arr.size ();
      
      if (container_ptr != 0)
      {
         container_ptr->do_remove (this, immediate_flag);
      }
      else
      {
         assert (false);
      }
      
      size_t new_size = _parent_ptr_arr.size ();
      
      if (cur_size == new_size)
      {
         ++i;
      }
      else
      {
         // stay at same position
      }
   }
}



/*
==============================================================================
Name : do_set_order_nbr
Thread : Video
==============================================================================
*/

void  Primitive::do_set_order_nbr (size_t order)
{
   ParentPtrArr::iterator it = _parent_ptr_arr.begin ();
   ParentPtrArr::iterator it_end = _parent_ptr_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      Primitive * parent_ptr = *it;
      
      PrimitiveContainer * container_ptr = dynamic_cast <PrimitiveContainer *> (parent_ptr);
      assert (container_ptr != 0);
      
      container_ptr->do_set_order_nbr (this, order);
   }
}



/*
==============================================================================
Name : do_attach
Thread : Video
==============================================================================
*/

void  Primitive::do_attach (Primitive & parent)
{
#if defined (OHM_OPA_DEBUGGER)
#if (ohm_archi_SYS != ohm_archi_SYS_WIN)
   if (opa::Debugger::use_instance ().has_break (*this, &Primitive::do_attach))
   {
      ohm_util_TRACE_DEBUGGER ();
   }
#endif
#endif

#if ! defined (NDEBUG)
   ParentPtrArr::iterator it = _parent_ptr_arr.begin ();
   const ParentPtrArr::iterator it_end = _parent_ptr_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      Primitive * parent_ptr = *it;
      
      assert (parent_ptr != &parent);
   }
#endif
   
   _parent_ptr_arr.push_back (&parent);
   
   //#warning for now
   invalidate_all ();
   
   do_notify_attached (parent);
}



/*
==============================================================================
Name : do_detach
Thread : Video
==============================================================================
*/

void  Primitive::do_detach (Primitive & parent)
{
#if defined (OHM_OPA_DEBUGGER)
#if (ohm_archi_SYS != ohm_archi_SYS_WIN)
   if (opa::Debugger::use_instance ().has_break (*this, &Primitive::do_detach))
   {
      ohm_util_TRACE_DEBUGGER ();
   }
#endif
#endif

   do_notify_detaching (parent);
   
   //#warning for now
   invalidate_all ();
   
   bool found_flag = false;
   
   ParentPtrArr::iterator it = _parent_ptr_arr.begin ();
   const ParentPtrArr::iterator it_end = _parent_ptr_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      Primitive * parent_ptr = *it;
      
      if (parent_ptr == &parent)
      {
         _parent_ptr_arr.erase (it);
         found_flag = true;
         break;
      }
   }
   
   assert (found_flag);
   
   //#warning this is not going to invalidate well
   // we need to invalidate at least in the detached parent,
   // as it is now not available anymore
   
   //#warning for now
   //invalidate_all ();
}



/*
==============================================================================
Name : is_detachable
Thread : Video
==============================================================================
*/

bool  Primitive::is_detachable () const
{
   return _parent_ptr_arr.empty () && _destroyable_flag;
}



/*
==============================================================================
Name : init_display
Thread : Video
Description :
   Very first function called from within video thread for 'this' primitive.
   ie. it will be called before any other Type* related notification
   (with our convention 'name_of_member_changed')
Note :
   This is generally the place to allocate ressources that will live accross
   the whole life time of the object.
==============================================================================
*/

void  Primitive::init_display ()
{
   // default implementation : nothing
}



/*
==============================================================================
Name : restore_display
Thread : Video
Description :
   Very last function called.
Note :
   This is the place to deallocate every resources.
==============================================================================
*/

void  Primitive::restore_display ()
{
   // default implementation : nothing
}



/*
==============================================================================
Name : display
Thread : Video
==============================================================================
*/

void  Primitive::display (GraphicContext & context)
{
   // default implementation : nothing
}



/*
==============================================================================
Name : invalidate
Thread : Video
Note :
   rect is relative to origin
==============================================================================
*/

void  Primitive::invalidate (const opa::Rect & rect, const opa::Margin & margin)
{
   bool is_visible_flag = is_visible ();
   bool was_visible_flag = was_visible ();
   
   if (is_visible_flag || was_visible_flag)
   {
      do_invalidate (rect, margin);
   }
}



/*
==============================================================================
Name : do_stop
Thread : Video
==============================================================================
*/

void  Primitive::do_stop ()
{
   size_t size = _animatable_ptr_arr.size ();
   
   for (size_t i = 0 ; i < size ; ++i)
   {
      Animatable * animatable_ptr = _animatable_ptr_arr [i];
      assert (animatable_ptr != 0);
      
      animatable_ptr->vt_stop ();
   }
}



/*
==============================================================================
Name : is_animating
Thread : Video
==============================================================================
*/

bool  Primitive::is_animating ()
{
   bool ret_val = false;
   
   size_t size = _animatable_ptr_arr.size ();
   
   for (size_t i = 0 ; i < size ; ++i)
   {
      Animatable * animatable_ptr = _animatable_ptr_arr [i];

      if (animatable_ptr != 0)
      {
         ret_val |= animatable_ptr->vt_is_animating ();
      }
      
      if (ret_val)
      {
         break;
      }
   }
   
   return ret_val;
}



/*
==============================================================================
Name : is_visible
Thread : Video
==============================================================================
*/

bool  Primitive::is_visible ()
{
   bool visible_flag = _visibility.get_cur_value ();
   
   return visible_flag;
}



/*
==============================================================================
Name : was_visible
Thread : Video
==============================================================================
*/

bool  Primitive::was_visible ()
{
   bool visible_flag = _visibility.get_prev_value ();
   
   return visible_flag;
}



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : set_destroyable
Thread : Main
==============================================================================
*/

void  Primitive::set_destroyable ()
{
   TransactionMaster::use ().push (
      do_set_destroyable_proc, this, 0, 0
   );

#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
   TransactionMaster::use ().debug_remove (this);
   
   size_t size = _type_debug_ptr_arr.size ();
   
   for (size_t i = 0 ; i < size ; ++i)
   {
      TypeDebug * type_debug_ptr = _type_debug_ptr_arr [i];
      assert (type_debug_ptr != 0);
      
      type_debug_ptr->notify_parent_destroyable ();
   }
#endif
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : get_parent_ptr_arr
==============================================================================
*/

const Primitive::ParentPtrArr &  Primitive::get_parent_ptr_arr () const
{
   return _parent_ptr_arr;
}



/*
==============================================================================
Name : get_parent
Note :
   Only usable when parent count is exactly 1
==============================================================================
*/

Primitive & Primitive::get_parent () const
{
   assert (_parent_ptr_arr.size () == 1);
   
   Primitive * prim_ptr = _parent_ptr_arr.front ();
   
   return *prim_ptr;
}



/*
==============================================================================
Name : set_no_bounding_box
Description :
   When called, the derived class tells the system it does not have a
   fast way to know its bounding box.
   Most likely, this occurs for primitives like Layer which do not track
   the bounding box union of their child.

   This will protect us to use _bbox which is invalid in this case.
==============================================================================
*/

void  Primitive::set_no_bounding_box ()
{
   _bbox_valid_flag = false;
}



/*
==============================================================================
Name : has_bounding_box
==============================================================================
*/

bool  Primitive::has_bounding_box () const
{
   return _bbox_valid_flag;
}



/*
==============================================================================
Name : dtor
Note :
   Do NEVER use 'delete' on a primitive. Always use the 'AutoPrimPtr'
   class.
==============================================================================
*/

Primitive::~Primitive ()
{
   assert (!is_animating ());
   assert (_parent_ptr_arr.empty ());
}



/*
==============================================================================
Name : set_bounding_box
Thread : Video
==============================================================================
*/

void  Primitive::set_bounding_box (const opa::Rect & bbox, const opa::Margin & margin)
{
   if ((_bbox != bbox) || (_margin != margin) || (!_bbox_valid_flag))
   {
      if (is_visible ())
      {
         // short cuts normal 'invalidate' call, as we don't want
         // the invalidate rectangle to be changed
         
         do_invalidate (_bbox, _margin);
         do_invalidate (bbox, margin);
      }
      
      _bbox = bbox;
      _margin = margin;
      _bbox_valid_flag = true;
   }
}



/*
==============================================================================
Name : get_bounding_box
Thread : Video
==============================================================================
*/

const opa::Rect & Primitive::get_bounding_box ()
{
   assert (_bbox_valid_flag);
   
   return _bbox;
}



/*
==============================================================================
Name : get_margin
Thread : Video
==============================================================================
*/

const opa::Margin &  Primitive::get_margin ()
{
   assert (_bbox_valid_flag);
   
   return _margin;
}



/*
==============================================================================
Name : do_invalidate
Thread : Video
Note :
   'rect' is relative to origin
   Primitive calling this function directly must take care of primitive
   visibility
==============================================================================
*/

void  Primitive::do_invalidate (const opa::Rect & rect, const opa::Margin & margin)
{
#if defined (OHM_OPAL_NO_INVALIDATION)
   return;
#endif
   
   ParentPtrArr::iterator it = _parent_ptr_arr.begin ();
   const ParentPtrArr::iterator it_end = _parent_ptr_arr.end ();
   
   opa::Rect parent_rel_rect = rect + _origin.get_cur_value ();
   
   for (; it != it_end ; ++it)
   {
      Primitive * parent_ptr = *it;
      
      parent_ptr->invalidate (parent_rel_rect, margin);
   }
}



/*
==============================================================================
Name : invalidate_all
Thread : Video
==============================================================================
*/

void  Primitive::invalidate_all ()
{
#if defined (OHM_OPAL_NO_INVALIDATION)
   return;
#endif

   assert (_bbox_valid_flag);
   
   bool is_visible_flag = is_visible ();
   bool was_visible_flag = was_visible ();
   
   if (is_visible_flag || was_visible_flag)
   {
      do_invalidate (_bbox, _margin);
   }
}



/*
==============================================================================
Name : do_notify_attached
Thread : Video
Description :
   Sent when the parent has been attached
==============================================================================
*/

void  Primitive::do_notify_attached (Primitive & parent)
{
   // default implementation : nothing
}



/*
==============================================================================
Name : do_notify_detaching
Thread : Video
Description :
   Sent when the parent is going to be detached
==============================================================================
*/

void  Primitive::do_notify_detaching (Primitive & parent)
{
   // default implementation : nothing
}



/*
==============================================================================
Name : Auto::ctor
Thread : Video
==============================================================================
*/

Primitive::Auto::Auto (Primitive & primitive, GraphicContext & context)
:  _primitive (primitive)
,  _context (context)
#if ! defined (NDEBUG)
,  _stack_size (_context.get_stack_size ())
#endif
{
   _primitive.Primitive::pre_display (_old_tx, _old_ty, _context);
}



/*
==============================================================================
Name : Auto::dtor
Thread : Video
==============================================================================
*/

Primitive::Auto::~Auto ()
{
   _primitive.Primitive::post_display (_old_tx, _old_ty, _context);

#if ! defined (NDEBUG)  
   size_t cur_stack_size = _context.get_stack_size ();
   
   assert (_stack_size == cur_stack_size);
#endif
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : pre_display
Thread : Video
==============================================================================
*/

void  Primitive::pre_display (float & old_tx, float & old_ty, GraphicContext & context)
{
   assert (!_parent_ptr_arr.empty ());
   
   opa::Point origin = _origin.get_cur_value ();
   
   context.translate (old_tx, old_ty, origin._x, origin._y);
}



/*
==============================================================================
Name : post_display
Thread : Video
==============================================================================
*/

void  Primitive::post_display (float old_tx, float old_ty, GraphicContext & context)
{
   assert (!_parent_ptr_arr.empty ());
   
   context.override_translate (old_tx, old_ty);
}



/*
==============================================================================
Name : visibility_changed
Thread : Video
==============================================================================
*/

void  Primitive::visibility_changed ()
{
   bool is_visible_flag = is_visible ();
   bool was_visible_flag = was_visible ();
   
   if (is_visible_flag != was_visible_flag)
   {
      invalidate_all ();
   }
}



/*
==============================================================================
Name : origin_changed_prev
Thread : Video
==============================================================================
*/

void  Primitive::origin_changed_prev ()
{
   invalidate_all ();
}



/*
==============================================================================
Name : origin_changed
Thread : Video
==============================================================================
*/

void  Primitive::origin_changed ()
{
   invalidate_all ();
}



/*
==============================================================================
Name : nothing_changed
Thread : Video
==============================================================================
*/

void  Primitive::nothing_changed ()
{
   // nothing
}



/*
==============================================================================
Name : inc_ref_cnt
Thread : Video
Description :
   Used by 'PrimitivePtrOrder' and indirectly by 'PrimitiveContainer' to
   allow fast push back when a primitive is not referenced.
==============================================================================
*/

void  Primitive::inc_ref_cnt ()
{
   ++_ref_cnt;
}



/*
==============================================================================
Name : dec_ref_cnt
Thread : Video
Description :
   see 'inc_ref_cnt'
==============================================================================
*/

void  Primitive::dec_ref_cnt ()
{
   assert (_ref_cnt > 0);
   --_ref_cnt;
}



/*
==============================================================================
Name : is_referenced
Thread : Video
Description :
   see 'inc_ref_cnt'
==============================================================================
*/

size_t   Primitive::is_referenced ()
{
   return _ref_cnt > 0;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
