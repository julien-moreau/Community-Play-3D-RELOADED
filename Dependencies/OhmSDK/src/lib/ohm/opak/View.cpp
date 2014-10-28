/*****************************************************************************

        View.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59590

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

#include "ohm/opal/def.h"
#include "ohm/opak/View.h"
#include "ohm/opak/base/CachePool.h"
#include "ohm/opak/private/TransactionMaster.h"
#include "ohm/opal/View.h"
#include "ohm/opal/Frame.h"
#include "ohm/task/RunLoopGui.h"

#include <cassert>



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : release
==============================================================================
*/

View &   View::create (opal::View & view)
{
   View * view_ptr = new View (view);
   
   return *view_ptr;
}



/*
==============================================================================
Name : release
==============================================================================
*/

void  View::release (View & view)
{
   assert (&view != 0);
   
   View * view_ptr = &view;
   
   delete view_ptr;
}



/*
==============================================================================
Name : run
Description :
   Attach the layer.
==============================================================================
*/

void  View::run ()
{
   TransactionMaster::use ().push (
      do_create_proc, this, 0, 0
   );
}



/*
==============================================================================
Name : clear
Description :
   Clean the layer. This is to be called before the view is released, and
   the caller must wait for the layer to be cleaned before it can release
   the view.
==============================================================================
*/

void  View::clear ()
{
   TransactionMaster::use ().push (
      do_clear_proc, this, 0, 0
   );
   
   TransactionMaster::use_instance ().send ();
   
   _clear_sync.wait ();
}



/*
==============================================================================
Name : stop
==============================================================================
*/

void  View::stop ()
{
   TransactionMaster::use ().push (
      do_stop_proc, this, 0, 0
   );
}



/*
==============================================================================
Name : is_animating
==============================================================================
*/

bool  View::is_animating ()
{
   return _layer_aptr->is_animating ();
}



/*
==============================================================================
Name : display
==============================================================================
*/

void  View::display (GraphicContext & context)
{
   context.get ().save_graphic_state ();
   
   if (_layer_aptr.get () != 0)
   {
      opa::Point origin = _layer_aptr->_origin.get_cur_value ();
      context.get ().concat_translate (origin._x, origin._y);
      
      _layer_aptr->display (context);
   }
   
   context.get ().restore_graphic_state ();
}



/*
==============================================================================
Name : draw
==============================================================================
*/

void  View::draw (opal::GraphicContextInterface & context)
{
   opak::Primitive * prim_ptr = this;
   
   GraphicContext graphic_context (context);
   
   prim_ptr->display (graphic_context);
}



/*
==============================================================================
Name : request_invalidate_all
==============================================================================
*/

void  View::request_invalidate_all ()
{
#if defined (OHM_OPAL_NO_INVALIDATION)
   return;
#endif
   
   opa::Rect view_rect (opa::Point::_zero, _view.get_bounds ());
   
   invalidate (view_rect, opa::Rect::_zero);
}



/*
==============================================================================
Name : invalidate
==============================================================================
*/

void  View::invalidate (const opa::Rect & rect, const opa::Margin & margin)
{
   opa::Rect total_rect = rect;
   total_rect += margin;
   
   _view.invalidate (total_rect);
}



/*
==============================================================================
Name : do_create_proc
==============================================================================
*/

void  View::do_create_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   assert (target_ptr != 0);
   assert (data_size == 0);
   assert (data_ptr == 0);
   
   View * this_ptr = reinterpret_cast <View *> (target_ptr);
   
   this_ptr->do_create ();
}



/*
==============================================================================
Name : do_clear_proc
==============================================================================
*/

void  View::do_clear_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   assert (target_ptr != 0);
   assert (data_size == 0);
   assert (data_ptr == 0);
   
   View * this_ptr = reinterpret_cast <View *> (target_ptr);
   
   this_ptr->do_clear ();
}



/*
==============================================================================
Name : do_stop_proc
==============================================================================
*/

void  View::do_stop_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   assert (target_ptr != 0);
   assert (data_size == 0);
   assert (data_ptr == 0);
   
   View * this_ptr = reinterpret_cast <View *> (target_ptr);
   
   this_ptr->do_stop ();
}



/*
==============================================================================
Name : do_create
==============================================================================
*/

void  View::do_create ()
{
   _layer_aptr->do_attach (*this);
}



/*
==============================================================================
Name : do_clear
==============================================================================
*/

void  View::do_clear ()
{
   _layer_aptr->do_stop ();

   bool immediate_flag = true;
   _layer_aptr->do_clear (immediate_flag);

   _layer_aptr->do_detach (*this);
   
   _clear_sync.signal ();
}



/*
==============================================================================
Name : do_stop
==============================================================================
*/

void  View::do_stop ()
{
   _layer_aptr->do_stop ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

View::View (opal::View & view)
:  _view (view)
,  _observer ("ohm.opak.View")
{
   set_name ("View");
   
   _layer_aptr = new Layer;
   
   _view.bind (*this);
   
   _observer.bind_post_process <
      View,
      &View::post_process
   > (*this);
   
   _observer.set_order (1000);
   
   task::RunLoopGui::use_instance ().add (_observer);
   
   //request_invalidate_all ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

View::~View ()
{
   try
   {
      task::RunLoopGui::use_instance ().remove (_observer);
      
      _view.unbind (*this);
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : invalidate
==============================================================================
*/

void  View::invalidate ()
{
   request_invalidate_all ();
}



/*
==============================================================================
Name : bounding_box_changed
==============================================================================
*/

void  View::bounding_box_changed ()
{
   // nothing
}



/*
==============================================================================
Name : do_propagate_child_bounding_box_changed
==============================================================================
*/

void  View::do_propagate_child_bounding_box_changed ()
{
   // nothing
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : post_process
==============================================================================
*/

void  View::post_process ()
{
   opak::TransactionMaster::use_instance ().send ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
