/*****************************************************************************

        PrimitiveContainer.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 67386

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
#include "ohm/opak/base/PrimitiveContainer.h"
#include "ohm/opak/private/TransactionMaster.h"
#include "ohm/odbg/LogMgr.h"
#include "ohm/util/trace.h"
#include "ohm/math/def.h"

#if defined (OHM_OPA_DEBUGGER)
   #include "ohm/opa/Debugger.h"
#endif

#include <algorithm>

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

PrimitiveContainer::PrimitiveContainer ()
:  _marked_flag (false)
,  _mark_id (0)
,  _primitive_ptr_list ()
,  _animating_flag (false)
{
#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
   // base class Primitive address will also be added
   // because pointers are different

   TransactionMaster::use ().debug_add (this);
#endif
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

PrimitiveContainer::~PrimitiveContainer ()
{
   try
   {
      bool immediate_flag = true;
      
      do_clear (immediate_flag);
   }
   catch (...)
   {
      assert (false);
   }
}




/*
==============================================================================
Name : mark
Warning :
   NOT THREAD SAFE - to use before pushing the primitive in a container.
==============================================================================
*/

void  PrimitiveContainer::mark (archi::UInt32 mark_id)
{
   assert (!_marked_flag);
   
   _marked_flag = true;
   _mark_id = mark_id;
}



/*
==============================================================================
Name : push_back
==============================================================================
*/

void  PrimitiveContainer::push_back (Primitive & primitive)
{
   Primitive * primitive_ptr = &primitive;
   
   TransactionMaster::use ().push (
      do_push_back_proc, this, sizeof (Primitive *), (archi::UByte *) &primitive_ptr
   );
}



/*
==============================================================================
Name : remove
==============================================================================
*/

void  PrimitiveContainer::remove (Primitive & primitive)
{
   archi::UByte buf [sizeof (&primitive) + sizeof (archi::UByte)];
   reinterpret_cast <Primitive * &> (buf [0]) = &primitive;
   buf [sizeof (&primitive)] = 0;
   
   TransactionMaster::use ().push (
      do_remove_proc, this,
      sizeof (&primitive) + sizeof (archi::UByte), buf
   );
}



/*
==============================================================================
Name : remove_immediate
==============================================================================
*/

void  PrimitiveContainer::remove_immediate (Primitive & primitive)
{
   archi::UByte buf [sizeof (&primitive) + sizeof (archi::UByte)];
   reinterpret_cast <Primitive * &> (buf [0]) = &primitive;
   buf [sizeof (&primitive)] = 1;

   TransactionMaster::use ().push (
      do_remove_proc, this,
      sizeof (&primitive) + sizeof (archi::UByte), buf
   );
}



/*
==============================================================================
Name : clear
==============================================================================
*/

void  PrimitiveContainer::clear ()
{
   archi::UByte val = 0;

   TransactionMaster::use ().push (
      do_clear_proc, this, 1, &val
   );
}



/*
==============================================================================
Name : clear_immediate
==============================================================================
*/

void  PrimitiveContainer::clear_immediate ()
{
   archi::UByte val = 1;

   TransactionMaster::use ().push (
      do_clear_proc, this, 1, &val
   );
}



/*
==============================================================================
Name : intercalate
==============================================================================
*/

void  PrimitiveContainer::intercalate (PrimitiveContainer & primitive)
{
   PrimitiveContainer * primitive_ptr = &primitive;
   
   TransactionMaster::use ().push (
      do_intercalate_proc, this,
      sizeof (PrimitiveContainer *), (archi::UByte *) &primitive_ptr
   );
}



/*
==============================================================================
Name : deintercalate
==============================================================================
*/

void  PrimitiveContainer::deintercalate (PrimitiveContainer & primitive)
{
   PrimitiveContainer * primitive_ptr = &primitive;
   
   TransactionMaster::use ().push (
      do_deintercalate_proc, this,
      sizeof (PrimitiveContainer *), (archi::UByte *) &primitive_ptr
   );
}



/*
==============================================================================
Name : set_order_nbr
==============================================================================
*/

void  PrimitiveContainer::set_order_nbr (Primitive & primitive, size_t order)
{
   archi::UByte buf [sizeof (&primitive) + sizeof (size_t)];
   reinterpret_cast <Primitive * &> (buf [0]) = &primitive;
   reinterpret_cast <size_t &> (buf [sizeof (&primitive)]) = order;

   TransactionMaster::use ().push (
      do_sort_proc, this, sizeof (&primitive) + sizeof (size_t), buf
   );
}



/*
==============================================================================
Name : sort
==============================================================================
*/

void  PrimitiveContainer::sort ()
{
   TransactionMaster::use ().push (
      do_sort_proc, this, 0, 0
   );
}



/*
==============================================================================
Name : stop
==============================================================================
*/

void  PrimitiveContainer::stop ()
{
   TransactionMaster::use ().push (
      do_stop_proc, this, 0, 0
   );
}



/*
==============================================================================
Name : do_push_back_proc
==============================================================================
*/

void  PrimitiveContainer::do_push_back_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   assert (target_ptr != 0);
   assert (data_size == sizeof (Primitive *));
   assert (data_ptr != 0);
   
   PrimitiveContainer * this_ptr = reinterpret_cast <PrimitiveContainer *> (target_ptr);
   Primitive * value = *reinterpret_cast <Primitive * const *> (data_ptr);
   
   this_ptr->do_push_back (value);
}



/*
==============================================================================
Name : do_remove_proc
==============================================================================
*/

void  PrimitiveContainer::do_remove_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   assert (target_ptr != 0);
   assert (data_size == sizeof (Primitive *) + sizeof (archi::UByte));
   assert (data_ptr != 0);
   
   PrimitiveContainer * this_ptr = reinterpret_cast <PrimitiveContainer *> (target_ptr);

   Primitive * primitive_ptr = reinterpret_cast <Primitive * const &> (data_ptr [0]);
   archi::UByte immediate_flag = data_ptr [sizeof (Primitive *)];
   
   this_ptr->do_remove ((Primitive *) primitive_ptr, immediate_flag != 0);
}



/*
==============================================================================
Name : do_clear_proc
==============================================================================
*/

void  PrimitiveContainer::do_clear_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   assert (target_ptr != 0);
   assert (data_size == sizeof (archi::UByte));
   assert (data_ptr != 0);
   
   PrimitiveContainer * this_ptr = reinterpret_cast <PrimitiveContainer *> (target_ptr);

   archi::UByte immediate_flag = data_ptr [0];
   
   this_ptr->do_clear (immediate_flag != 0);
}



/*
==============================================================================
Name : do_intercalate_proc
==============================================================================
*/

void  PrimitiveContainer::do_intercalate_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   assert (target_ptr != 0);
   assert (data_size == sizeof (PrimitiveContainer *));
   assert (data_ptr != 0);
   
   PrimitiveContainer * this_ptr = reinterpret_cast <PrimitiveContainer *> (target_ptr);
   PrimitiveContainer * value = *reinterpret_cast <PrimitiveContainer * const *> (data_ptr);
   
   this_ptr->do_intercalate (value);
}



/*
==============================================================================
Name : do_deintercalate_proc
==============================================================================
*/

void  PrimitiveContainer::do_deintercalate_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   assert (target_ptr != 0);
   assert (data_size == sizeof (PrimitiveContainer *));
   assert (data_ptr != 0);
   
   PrimitiveContainer * this_ptr = reinterpret_cast <PrimitiveContainer *> (target_ptr);
   PrimitiveContainer * value = *reinterpret_cast <PrimitiveContainer * const *> (data_ptr);
   
   this_ptr->do_deintercalate (value);
}



/*
==============================================================================
Name : do_set_order_nbr_proc
==============================================================================
*/

void  PrimitiveContainer::do_set_order_nbr_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   struct Bundle {Primitive * _primitive_ptr; size_t _order;};
   
   assert (target_ptr != 0);
   assert (data_size == sizeof (Bundle));
   assert (data_ptr != 0);
   
   PrimitiveContainer * this_ptr = reinterpret_cast <PrimitiveContainer *> (target_ptr);
   Bundle bundle = *reinterpret_cast <const Bundle *> (data_ptr);
   
   this_ptr->do_set_order_nbr (bundle._primitive_ptr, bundle._order);
}



/*
==============================================================================
Name : do_sort_proc
==============================================================================
*/

void  PrimitiveContainer::do_sort_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   assert (target_ptr != 0);
   assert (data_size == 0);
   assert (data_ptr == 0);
   
   PrimitiveContainer * this_ptr = reinterpret_cast <PrimitiveContainer *> (target_ptr);
   
   this_ptr->do_sort ();
}



/*
==============================================================================
Name : do_stop_proc
==============================================================================
*/

void  PrimitiveContainer::do_stop_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   assert (target_ptr != 0);
   assert (data_size == 0);
   assert (data_ptr == 0);
   
   PrimitiveContainer * this_ptr = reinterpret_cast <PrimitiveContainer *> (target_ptr);
   
   this_ptr->do_stop ();
}



/*
==============================================================================
Name : do_push_back
==============================================================================
*/

void  PrimitiveContainer::do_push_back (Primitive * primitive_ptr)
{
#if defined (OHM_OPA_DEBUGGER)
#if (ohm_archi_SYS != ohm_archi_SYS_WIN)
   if (opa::Debugger::use_instance ().has_break (*primitive_ptr, &PrimitiveContainer::do_push_back))
   {
      ohm_util_TRACE_DEBUGGER ();
   }
#endif
#endif

   PrimitivePtrOrderList::iterator it_end = _primitive_ptr_list.end ();
   PrimitivePtrOrderList::iterator it = it_end;
   
   if (primitive_ptr->is_referenced ())
   {
      // only in that case, do a full search
      // in the other case, this permits us to skip this long
      // linear search
      
      it = find_child (primitive_ptr);
   }
   
   if (it != it_end)
   {
      PrimitivePtrOrder & primitive_ptr_order = *it;
      
      primitive_ptr_order.resurrect ();
      
      PrimitivePtrOrderList::iterator next_it = it;
      ++next_it;
      
      if (next_it != it_end)
      {
         _primitive_ptr_list.splice (
            it_end,
            _primitive_ptr_list,
            it, next_it
         );
      }
   }
   else
   {
      _primitive_ptr_list.push_back (PrimitivePtrOrder (*primitive_ptr));
   
      primitive_ptr->do_attach (*this);
   }
}



/*
==============================================================================
Name : do_remove
==============================================================================
*/

void  PrimitiveContainer::do_remove (Primitive * primitive_ptr, bool immediate_flag)
{
#if defined (OHM_OPA_DEBUGGER)
#if (ohm_archi_SYS != ohm_archi_SYS_WIN)
   if (opa::Debugger::use_instance ().has_break (*primitive_ptr, &PrimitiveContainer::do_remove))
   {
      ohm_util_TRACE_DEBUGGER ();
   }
#endif
#endif

   PrimitivePtrOrderList::iterator it = find_child (primitive_ptr);

   if (it != _primitive_ptr_list.end ())
   {
      PrimitivePtrOrder & primitive_ptr_order = *it;
      
      if (primitive_ptr->is_animating () && !immediate_flag)
      {
         primitive_ptr_order.kill ();
         
         animate ();
      }
      else
      {
         primitive_ptr->do_detach (*this);
         
         _primitive_ptr_list.erase (it);
      }
   }
}



/*
==============================================================================
Name : do_clear
==============================================================================
*/

void  PrimitiveContainer::do_clear (bool immediate_flag)
{
   PrimitivePtrOrderList::iterator it = _primitive_ptr_list.begin ();
   const PrimitivePtrOrderList::iterator it_end = _primitive_ptr_list.end ();
   
   for (; it != it_end ;)
   {
      PrimitivePtrOrderList::iterator next_it = it;
      ++next_it;
      
      Primitive * primitive_ptr = it->get_primitive_ptr ();
      
      do_remove (primitive_ptr, immediate_flag);
      
      it = next_it;
   }
}



/*
==============================================================================
Name : do_intercalate
==============================================================================
*/

void  PrimitiveContainer::do_intercalate (PrimitiveContainer * primitive_ptr)
{
   PrimitivePtrOrderList::iterator fit = find_child (primitive_ptr);
   
   if (fit == _primitive_ptr_list.end ())
   {
      PrimitivePtrOrderList::iterator it = _primitive_ptr_list.begin ();
      const PrimitivePtrOrderList::iterator it_end = _primitive_ptr_list.end ();
      
      for (; it != it_end ;)
      {
         PrimitivePtrOrderList::iterator next_it = it;
         ++next_it;
         
         Primitive * sub_primitive_ptr = it->get_primitive_ptr ();
         
         primitive_ptr->_primitive_ptr_list.push_back (*it);
         
         sub_primitive_ptr->do_detach (*this);
         sub_primitive_ptr->do_attach (*primitive_ptr);
         
         _primitive_ptr_list.erase (it);
         
         it = next_it;
      }
      
      assert (_primitive_ptr_list.empty ());
      
      _primitive_ptr_list.push_back (PrimitivePtrOrder (*primitive_ptr));
      
      primitive_ptr->do_attach (*this);
   }
}



/*
==============================================================================
Name : do_deintercalate
==============================================================================
*/

void  PrimitiveContainer::do_deintercalate (PrimitiveContainer * primitive_ptr)
{
   PrimitivePtrOrderList::iterator fit = find_child (primitive_ptr);
   
   if (fit != _primitive_ptr_list.end ())
   {
      PrimitivePtrOrderList::iterator it = primitive_ptr->_primitive_ptr_list.begin ();
      const PrimitivePtrOrderList::iterator it_end = primitive_ptr->_primitive_ptr_list.end ();
      
      for (; it != it_end ;)
      {
         PrimitivePtrOrderList::iterator next_it = it;
         ++next_it;
         
         Primitive * sub_primitive_ptr = it->get_primitive_ptr ();
         
         _primitive_ptr_list.push_back (*it);
         
         sub_primitive_ptr->do_detach (*primitive_ptr);
         sub_primitive_ptr->do_attach (*this);
         
         primitive_ptr->_primitive_ptr_list.erase (it);
         
         it = next_it;
      }
      
      assert (primitive_ptr->_primitive_ptr_list.empty ());
      
      do_remove (primitive_ptr, true);
   }
}



/*
==============================================================================
Name : do_set_order_nbr
==============================================================================
*/

void  PrimitiveContainer::do_set_order_nbr (Primitive * primitive_ptr, size_t order)
{
   PrimitivePtrOrderList::iterator it = find_child (primitive_ptr);
   assert (it != _primitive_ptr_list.end ());
   
   PrimitivePtrOrder & primitive_ptr_order = *it;
   
   primitive_ptr_order.set_order (order);
}



/*
==============================================================================
Name : do_sort
==============================================================================
*/

void  PrimitiveContainer::do_sort ()
{
   _primitive_ptr_list.sort ();
   
   invalidate_all ();
}



/*
==============================================================================
Name : do_stop
==============================================================================
*/

void  PrimitiveContainer::do_stop ()
{
   Primitive::do_stop ();
   
   {
      PrimitivePtrOrderList::iterator it = _primitive_ptr_list.begin ();
      const PrimitivePtrOrderList::iterator it_end = _primitive_ptr_list.end ();
      
      for (; it != it_end ;)
      {
         PrimitivePtrOrderList::iterator next_it = it;
         ++next_it;
         
         PrimitivePtrOrder & primitive_ptr_order = *it;
         
         Primitive * primitive_ptr = primitive_ptr_order.get_primitive_ptr ();
         
         primitive_ptr->do_stop ();
         
         if (primitive_ptr_order.is_dying ())
         {
            assert (!primitive_ptr->is_animating ());
            
            bool immediate_flag = true;
            do_remove (primitive_ptr, immediate_flag);
         }
         
         it = next_it;
      }
   }
   
   deanimate ();
}



/*
==============================================================================
Name : is_animating
==============================================================================
*/

bool  PrimitiveContainer::is_animating ()
{
   bool animating_flag = _animating_flag;
   
   if (!animating_flag)
   {
      animating_flag = Primitive::is_animating ();
   }
   
   if (!animating_flag)
   {
      PrimitivePtrOrderList::iterator it = _primitive_ptr_list.begin ();
      const PrimitivePtrOrderList::iterator it_end = _primitive_ptr_list.end ();
      
      for (; it != it_end ; ++it)
      {
         PrimitivePtrOrder & primitive_ptr_order = *it;
         
         Primitive * primitive_ptr = primitive_ptr_order.get_primitive_ptr ();
         
         if (primitive_ptr->is_animating ())
         {
            animating_flag = true;
            
            break;
         }
      }
   }
   
   return animating_flag;
}



/*
==============================================================================
Name : tick
==============================================================================
*/

void  PrimitiveContainer::tick (double time_s)
{
   bool animating_flag = false;
   
   PrimitivePtrOrderList::iterator it = _primitive_ptr_list.begin ();
   const PrimitivePtrOrderList::iterator it_end = _primitive_ptr_list.end ();
   
   for (; it != it_end ;)
   {
      PrimitivePtrOrderList::iterator next_it = it;
      ++next_it;
      
      PrimitivePtrOrder & primitive_ptr_order = *it;
      
      if (primitive_ptr_order.is_dying ())
      {
         Primitive * primitive_ptr = primitive_ptr_order.get_primitive_ptr ();
         
         if (primitive_ptr->is_animating ())
         {
            animating_flag = true;
         }
         else
         {
            bool immediate_flag = true;
            do_remove (primitive_ptr, immediate_flag);
         }
      }
      
      it = next_it;
   }
   
   if (!animating_flag)
   {
      deanimate ();
   }
}



/*
==============================================================================
Name : set_destroyable
Note :
   We have this function virtual since we need to remove the pointer of
   'this' which is different from our base class.
==============================================================================
*/

void  PrimitiveContainer::set_destroyable ()
{
#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
   TransactionMaster::use ().debug_remove (this);
#endif   
   
   Primitive::set_destroyable ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : display_children
==============================================================================
*/

#if 0
void  PrimitiveContainer::display_children (GraphicContext & context)
{
#if defined (OHM_OPA_DEBUGGER)
   if (opa::Debugger::use_instance ().is_tracing_prim_display ())
   {
      ohm_util_TRACE_INFO1 ("%1%", this->get_name ());
      ohm_util_TRACE_INDENT (2);
   }
#endif

   PrimitivePtrOrderList::iterator it = _primitive_ptr_list.begin ();
   const PrimitivePtrOrderList::iterator it_end = _primitive_ptr_list.end ();
   
   const opa::Rect clip_rect = context.get ().get_clip_bounding_box ();
   
   const opa::Point inv_px = context.get_inv_px ();
            
   for (; it != it_end ; ++it)
   {
      Primitive * primitive_ptr = it->get_primitive_ptr ();
      
#if defined (OHM_OPA_DEBUGGER)
#if (ohm_archi_SYS != ohm_archi_SYS_WIN)
      if (opa::Debugger::use_instance ().has_break (
         *primitive_ptr, &PrimitiveContainer::display_children)
      )
      {
         ohm_util_TRACE_DEBUGGER ();
      }
#endif
#endif
      
      if (primitive_ptr->is_visible ())
      {
         bool draw_flag = true;
         
         if (primitive_ptr->has_bounding_box ())
         {
            opa::Point origin = primitive_ptr->_origin.get_cur_value ();
            
            float old_tx, old_ty;
            context.translate (old_tx, old_ty, origin._x, origin._y);
            
            // untransformed bbox
            opa::Rect bbox = primitive_ptr->get_bounding_box ();
            
            opa::Margin margin = primitive_ptr->get_margin ();
            margin._left *= inv_px._x;
            margin._top *= inv_px._y;
            margin._right *= inv_px._x;
            margin._bottom *= inv_px._y;
            
            bbox += margin;

            opa::Point pt_0 = bbox._origin;
            context.apply (pt_0);
            
            opa::Size sz_0 (bbox._size._width, 0.f);
            context.apply (sz_0);

            opa::Size sz_1 (0.f, bbox._size._height);
            context.apply (sz_1);
            
            float left = lang::min (pt_0._x, pt_0._x + sz_0._width);
            lang::min_in_place (left, left + sz_1._width);

            float right = lang::max (pt_0._x, pt_0._x + sz_0._width);
            lang::max_in_place (right, right + sz_1._width);

            float top = lang::min (pt_0._y, pt_0._y + sz_0._height);
            lang::min_in_place (top, top + sz_1._height);

            float bottom = lang::max (pt_0._y, pt_0._y + sz_0._height);
            lang::max_in_place (bottom, bottom + sz_1._height);
            
            lang::max_in_place (left, clip_rect._origin._x);
            lang::min_in_place (right, clip_rect._origin._x + clip_rect._size._width);
            lang::max_in_place (top, clip_rect._origin._y);
            lang::min_in_place (bottom, clip_rect._origin._y + clip_rect._size._height);
            
            draw_flag = (left < right) && (top < bottom);
            
            context.override_translate (old_tx, old_ty);
         }
         
         if (draw_flag)
         {
            primitive_ptr->display (context);
         }
      }
   }
   
#if defined (OHM_OPA_DEBUGGER)
   if (opa::Debugger::use_instance ().is_tracing_prim_display ())
   {
      ohm_util_TRACE_INDENT (-2);
   }
#endif
}
#endif



/*
==============================================================================
Name : display_children
==============================================================================
*/

#if 1
void  PrimitiveContainer::display_children (GraphicContext & context)
{
   PrimitivePtrOrderList::iterator it = _primitive_ptr_list.begin ();
   const PrimitivePtrOrderList::iterator it_end = _primitive_ptr_list.end ();
   
   opa::Rect clip_rect = context.get ().get_clip_bounding_box ();
   context.apply_inverse (clip_rect._origin);
   context.apply_inverse (clip_rect._size);
   clip_rect = clip_rect.standardize ();
   
   const opa::Point inv_px = context.get_inv_px ();
            
   for (; it != it_end ; ++it)
   {
      Primitive * primitive_ptr = it->get_primitive_ptr ();
      
      if (primitive_ptr->is_visible ())
      {
         bool draw_flag = true;
         
         if (primitive_ptr->has_bounding_box ())
         {
            opa::Point origin = primitive_ptr->_origin.get_cur_value ();
            
            // untransformed bbox
            opa::Rect bbox = primitive_ptr->get_bounding_box ();
            bbox += origin;
            
            opa::Margin margin = primitive_ptr->get_margin ();
            margin._left *= inv_px._x;
            margin._top *= inv_px._y;
            margin._right *= inv_px._x;
            margin._bottom *= inv_px._y;
            
            bbox += margin;
            
            if (bbox.is_empty ())
            {
               // skip
               continue;
            }

            float left = lang::max (bbox._origin._x, clip_rect._origin._x);
            float right = lang::min (
               bbox._origin._x + bbox._size._width,
               clip_rect._origin._x + clip_rect._size._width
            );
            
            draw_flag = left < right;
            
            if (draw_flag)
            {
               float top = lang::max (bbox._origin._y, clip_rect._origin._y);
               float bottom = lang::min (
                  bbox._origin._y + bbox._size._height,
                  clip_rect._origin._y + clip_rect._size._height
               );
            
               draw_flag = top < bottom;
            }
         }
         
         if (draw_flag)
         {
            primitive_ptr->display (context);
         }
      }
   }
}
#endif



/*
==============================================================================
Name : invalidate_children
==============================================================================
*/

void  PrimitiveContainer::invalidate_children ()
{
#if defined (OHM_OPAL_NO_INVALIDATION)
   return;
#endif

   PrimitivePtrOrderList::iterator it = _primitive_ptr_list.begin ();
   const PrimitivePtrOrderList::iterator it_end = _primitive_ptr_list.end ();
   
   for (; it != it_end ; ++it)
   {
      Primitive * primitive_ptr = it->get_primitive_ptr ();
      
      if (primitive_ptr->is_visible ())
      {
         primitive_ptr->invalidate_all ();
      }
   }
}



/*
==============================================================================
Name : add_mark
==============================================================================
*/

void  PrimitiveContainer::add_mark (GraphicContext & context)
{
   if (_marked_flag)
   {
      context.mark (&typeid (*this), _mark_id);
   }
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : animate
==============================================================================
*/

void  PrimitiveContainer::animate ()
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

void  PrimitiveContainer::deanimate ()
{
   if (_animating_flag)
   {
      Animator::use_instance ().erase (*this);
      
      _animating_flag = false;
   }
}



/*
==============================================================================
Name : find_child
==============================================================================
*/

PrimitivePtrOrderList::iterator  PrimitiveContainer::find_child (Primitive * primitive_ptr)
{
   PrimitivePtrOrderList::iterator it = _primitive_ptr_list.begin ();
   const PrimitivePtrOrderList::iterator it_end = _primitive_ptr_list.end ();
   
   for (; it != it_end ; ++it)
   {
      if (it->get_primitive_ptr () == primitive_ptr)
      {
         break;
      }
   }
   
   return it;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
