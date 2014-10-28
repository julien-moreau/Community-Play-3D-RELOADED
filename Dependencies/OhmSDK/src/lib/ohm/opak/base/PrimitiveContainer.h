/*****************************************************************************

        PrimitiveContainer.h
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



#if ! defined (ohm_opak_PrimitiveContainer_HEADER_INCLUDED)
#define  ohm_opak_PrimitiveContainer_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/base/Primitive.h"
#include "ohm/opak/private/AnimatorListener.h"
#include "ohm/opak/private/PrimitivePtrOrder.h"
#include "ohm/opak/private/TransactionDebug.h"
#include "ohm/opak/private/TypeDebug.h"

#include <functional>
#include <set>
#include <list>



namespace ohm
{
namespace opak
{



class PrimitiveContainer
:  public virtual Primitive
,  public AnimatorListener
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
public:
   
                  PrimitiveContainer ();
   virtual        ~PrimitiveContainer ();
   
   void           mark (archi::UInt32 mark_id = 0);
   
   void           push_back (Primitive & primitive);
   void           remove (Primitive & primitive);
   void           remove_immediate (Primitive & primitive);
   void           clear ();
   void           clear_immediate ();
   void           intercalate (PrimitiveContainer & primitive);
   void           deintercalate (PrimitiveContainer & primitive);
   
   void           set_order_nbr (Primitive & primitive, size_t order);
   void           sort ();
   
   virtual void   stop ();
   
   // Video Thread
   static void    do_push_back_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr);
   static void    do_remove_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr);
   static void    do_clear_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr);
   static void    do_intercalate_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr);
   static void    do_deintercalate_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr);
   static void    do_set_order_nbr_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr);
   static void    do_sort_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr);
   static void    do_stop_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr);
   
   void           do_push_back (Primitive * primitive_ptr);
   void           do_remove (Primitive * primitive_ptr, bool immediate_flag);
   void           do_clear (bool immediate_flag);
   void           do_intercalate (PrimitiveContainer * primitive_ptr);
   void           do_deintercalate (PrimitiveContainer * primitive_ptr);
   void           do_set_order_nbr (Primitive * primitive_ptr, size_t order);
   void           do_sort ();

   virtual void   do_stop ();
   virtual bool   is_animating ();
   
   // AnimatorListener
   virtual void   tick (double time_s);
   


/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
   
   // Primitive
   // called by AutoPrimPtr
   virtual void   set_destroyable ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   void           display_children (GraphicContext & context);
   template <class PrimitiveType>
   void           notify_children (void (PrimitiveType::*fnc) ());
   
   void           invalidate_children ();
   
   void           add_mark (GraphicContext & context);
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   PrimitivePtrOrderList::iterator
                  find_child (Primitive * primitive_ptr);
   
   void           animate ();
   void           deanimate ();
   
   bool           _marked_flag;
   archi::UInt32  _mark_id;

   PrimitivePtrOrderList
                  _primitive_ptr_list;

   bool           _animating_flag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PrimitiveContainer (const PrimitiveContainer &other);
   PrimitiveContainer &    operator = (const PrimitiveContainer &other);
   bool           operator == (const PrimitiveContainer &other);
   bool           operator != (const PrimitiveContainer &other);

}; // class PrimitiveContainer



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/opak/base/PrimitiveContainer.hpp"



#endif   // ohm_opak_PrimitiveContainer_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
