/*****************************************************************************

        ConductMarquee.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70477

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

#include "client/ConductMarquee.h"

#include "ohm/opal/Event.h"

#include <cassert>



namespace client
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

ConductMarquee::ConductMarquee ()
:  _event_ptr (0)
,  _collecting_flag (false)
,  _view_relative_marquee_rect_arr ()
,  _marquee_rect_need_update_flag (true)
,  _marquee_rect ()
,  _object_ptr_set ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ConductMarquee::~ConductMarquee ()
{
}



/*
==============================================================================
Name : begin_collecting
==============================================================================
*/

void  ConductMarquee::begin_collecting (const ohm::opa::Rect & marquee_rect)
{
   using namespace ohm;
   
   assert (_event_ptr != 0);
   
   assert (!_collecting_flag);
   
   _collecting_flag = true;
   
   // put back in view relative coords
   
   _view_relative_marquee_rect_arr.resize (1);
   
   opa::Rect & view_relative_marquee_rect = _view_relative_marquee_rect_arr [0];
   
   view_relative_marquee_rect = marquee_rect;
   
   _event_ptr->apply_inverse_transform (view_relative_marquee_rect._origin);
   _event_ptr->apply_inverse_transform (view_relative_marquee_rect._size);
   
   _marquee_rect = view_relative_marquee_rect;
   
   _object_ptr_set.clear ();
}



/*
==============================================================================
Name : is_collecting
==============================================================================
*/

bool  ConductMarquee::is_collecting () const
{
   return _collecting_flag;
}



/*
==============================================================================
Name : end_collecting
==============================================================================
*/

void  ConductMarquee::end_collecting ()
{
   assert (_collecting_flag);
   
   _collecting_flag = false;
}



/*
==============================================================================
Name : is_intersecting
==============================================================================
*/

bool  ConductMarquee::is_intersecting (const ohm::opa::Rect & rect)
{
   using namespace ohm;
   
   update ();
   
   opa::Rect inter_rect = _marquee_rect & rect;
   
   if (inter_rect._size._width < 1e-4f)
   {
      return false;
   }
   else if (inter_rect._size._height < 1e-4f)
   {
      return false;
   }
   else
   {
      return !inter_rect.is_empty ();
   }
}



/*
==============================================================================
Name : push_intersection
==============================================================================
*/

void  ConductMarquee::push_intersection (const ohm::opa::Rect & rect)
{
   using namespace ohm;
   
   assert (!_view_relative_marquee_rect_arr.empty ());
   
   opa::Rect view_relative_rect = rect;
   
   _event_ptr->apply_inverse_transform (view_relative_rect._origin);
   _event_ptr->apply_inverse_transform (view_relative_rect._size);
   
   opa::Rect view_relative_marquee_rect = _view_relative_marquee_rect_arr.back ();
   
   view_relative_rect &= view_relative_marquee_rect;
   
   _view_relative_marquee_rect_arr.push_back (view_relative_rect);
   
   _marquee_rect_need_update_flag = true;
}



/*
==============================================================================
Name : pop_intersection
==============================================================================
*/

void  ConductMarquee::pop_intersection ()
{
   assert (_view_relative_marquee_rect_arr.size () > 1);
   
   _view_relative_marquee_rect_arr.pop_back ();
   
   _marquee_rect_need_update_flag = true;
}



/*
==============================================================================
Name : add
==============================================================================
*/

void  ConductMarquee::add (model::SpacialObject & object)
{
   assert (&object != 0);
   
   _object_ptr_set.insert (&object);
}



/*
==============================================================================
Name : use_object_ptr_set
==============================================================================
*/

const std::set <model::SpacialObject *> & ConductMarquee::use_object_ptr_set () const
{
   return _object_ptr_set;
}



/*
==============================================================================
Name : is_spatial
==============================================================================
*/

bool  ConductMarquee::is_spatial () const
{
   return true;
}



/*
==============================================================================
Name : bind
==============================================================================
*/

void  ConductMarquee::bind (ohm::opal::Event & event)
{
   assert (&event != 0);
   
   _event_ptr = &event;
}



/*
==============================================================================
Name : invalidate
==============================================================================
*/

void  ConductMarquee::invalidate ()
{
   _marquee_rect_need_update_flag = true;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : update
==============================================================================
*/

void  ConductMarquee::update ()
{
   assert (_event_ptr != 0);
   
   if (_marquee_rect_need_update_flag)
   {
      assert (!_view_relative_marquee_rect_arr.empty ());
      
      _marquee_rect = _view_relative_marquee_rect_arr.back ();
      
      _event_ptr->apply_transform (_marquee_rect._origin);
      _event_ptr->apply_transform (_marquee_rect._size);
      
      _marquee_rect_need_update_flag = false;
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
