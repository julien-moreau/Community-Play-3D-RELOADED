/*****************************************************************************

        Primitive.h
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



#if ! defined (ohm_opak_Primitive_HEADER_INCLUDED)
#define  ohm_opak_Primitive_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Constants.h"
#include "ohm/opa/Rect.h"
#include "ohm/opa/Margin.h"
#include "ohm/opak/base/TypePoint.h"
#include "ohm/opak/base/TypeEnum.h"
#include "ohm/opak/base/TypeBool.h"
#include "ohm/opak/base/BlendMode.h"
#include "ohm/opak/base/GraphicContext.h"
#include "ohm/opak/base/HelperRect.h"
#include "ohm/opak/base/HelperImage.h"
#include "ohm/opak/base/HelperSprungImage.h"
#include "ohm/opak/base/Animatable.h"
#include "ohm/opak/private/TransactionDebug.h"
#include "ohm/opak/private/TypeDebug.h"

#include <list>
#include <vector>



namespace ohm
{
namespace opa
{
class Rect;
}


namespace opal
{
class GraphicContextInterface;
}



namespace opak
{



class PrimitiveContainer;

class Primitive
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  Primitive ();
   
   void           set_name (const char * name_0);
   const char *   get_name () const;
   
   void           remove_from_parents ();
   void           remove_from_parents_immediate ();
   void           set_order_nbr (size_t order);
   void           keep_alive (double time);
   
   void           add_animatable (Animatable & animatable);
   void           remove_animatable (Animatable & animatable);

#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
   void           add_type_debug (TypeDebug & type_debug);
   void           remove_type_debug (TypeDebug & type_debug);
#endif   

   virtual void   stop ();

   // Video Thread
   static void    do_init_display_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr);
   static void    do_set_destroyable_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr);
   static void    do_remove_from_parents_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr);
   static void    do_set_order_nbr_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr);

   void           do_init_display ();
   void           do_set_destroyable ();
   void           do_destroy ();
   void           do_remove_from_parents (bool immediate_flag);
   void           do_set_order_nbr (size_t order);
   void           do_attach (Primitive & parent);
   void           do_detach (Primitive & parent);
   bool           is_detachable () const;
   
   virtual void   init_display ();
   virtual void   restore_display ();
   virtual void   display (GraphicContext & context);
   virtual void   invalidate (const opa::Rect & rect, const opa::Margin & margin);

   virtual void   do_stop ();
   virtual bool   is_animating ();
   virtual bool   is_visible ();
   virtual bool   was_visible ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   typedef std::vector <Animatable *>  AnimatablePtrArr;
   AnimatablePtrArr
                  _animatable_ptr_arr;

#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
   typedef std::vector <TypeDebug *>   TypeDebugPtrArr;
   TypeDebugPtrArr
                  _type_debug_ptr_arr;
#endif


   
/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   TypeBool <Primitive>
                  _visibility;
   TypePoint <Primitive>
                  _origin;
   


/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   // called by AutoPrimPtr
   virtual void   set_destroyable ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   class Auto
   {
   public:
                  Auto (Primitive & primitive, GraphicContext & context);
                  ~Auto ();
   private:
      Primitive & _primitive;
      GraphicContext &
                  _context;
      size_t      _stack_size;
      float       _old_tx;
      float       _old_ty;
   };
   
   typedef std::vector <Primitive *> ParentPtrArr;
   
   const ParentPtrArr &
                  get_parent_ptr_arr () const;
   Primitive &    get_parent () const;
   template <class T>
   T *            get_ancestor ();
   
   virtual        ~Primitive ();

   void           set_no_bounding_box ();
   bool           has_bounding_box () const;

   void           set_bounding_box (const opa::Rect & bbox, const opa::Margin & margin = opa::Margin::_zero);
   const opa::Rect &
                  get_bounding_box ();
   const opa::Margin &
                  get_margin ();
   
   void           do_invalidate (const opa::Rect & rect, const opa::Margin & margin);

   virtual void   invalidate_all ();
   virtual void   do_notify_attached (Primitive & parent);
   virtual void   do_notify_detaching (Primitive & parent);


   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   friend class PrimitiveContainer;
   
   void           pre_display (float & old_tx, float & old_ty, GraphicContext & context);
   void           post_display (float old_tx, float old_ty, GraphicContext & context);

   void           visibility_changed ();
   void           origin_changed_prev ();
   void           origin_changed ();
   void           nothing_changed ();
   
   friend class PrimitivePtrOrder;
   void           inc_ref_cnt ();
   void           dec_ref_cnt ();
   size_t         is_referenced ();
   
   const char *   _name_0;
   
   bool           _bbox_valid_flag;
   opa::Rect      _bbox;   // relative to this origin
   opa::Margin    _margin;
   
   bool           _destroyable_flag;
   ParentPtrArr   _parent_ptr_arr;

   TypeBool <Primitive>
                  _keep_alive_flag;
   
   size_t         _ref_cnt;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
               Primitive (const Primitive &other);
   Primitive & operator = (const Primitive &other);
   bool        operator == (const Primitive &other);
   bool        operator != (const Primitive &other);

}; // class Primitive



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/opak/base/Primitive.hpp"



#endif   // ohm_opak_Primitive_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
