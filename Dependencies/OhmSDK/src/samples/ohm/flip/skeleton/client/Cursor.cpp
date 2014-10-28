/*****************************************************************************

        Cursor.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71268

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

#include "client/Cursor.h"

#include "ohm/opal/Application.h"
#include "ohm/opal/CursorManager.h"
#include "ohm/util/trace.h"

#include <cassert>



namespace client
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : use
==============================================================================
*/

Cursor & Cursor::use ()
{
   Cursor & instance = use_instance ();
   
   assert (instance._base == Base_NONE);
   assert (instance._options.none ());
   
   return instance;
}



/*
==============================================================================
Name : arrow
==============================================================================
*/

Cursor & Cursor::arrow ()
{
   assert (_base == Base_NONE);
   
   _base = Base_ARROW;
   
   return *this;
}



/*
==============================================================================
Name : resize
==============================================================================
*/

Cursor & Cursor::resize ()
{
   assert (_base == Base_NONE);
   
   _base = Base_RESIZE;
   
   return *this;
}



/*
==============================================================================
Name : hand
==============================================================================
*/

Cursor & Cursor::hand ()
{
   assert (_base == Base_NONE);
   
   _base = Base_HAND;
   
   return *this;
}



/*
==============================================================================
Name : pen
==============================================================================
*/

Cursor & Cursor::pen ()
{
   assert (_base == Base_NONE);
   
   _base = Base_PEN;
   
   return *this;
}



/*
==============================================================================
Name : eraser
==============================================================================
*/

Cursor & Cursor::eraser ()
{
   assert (_base == Base_NONE);
   
   _base = Base_ERASER;
   
   return *this;
}



/*
==============================================================================
Name : move
==============================================================================
*/

Cursor & Cursor::move ()
{
   push (Opt_MOVE);
   
   return *this;
}



/*
==============================================================================
Name : copy
==============================================================================
*/

Cursor & Cursor::copy (bool copy_flag)
{
   if (copy_flag)
   {
      push (Opt_COPY);
   }
   
   return *this;
}



/*
==============================================================================
Name : right
==============================================================================
*/

Cursor & Cursor::right ()
{
   push (Opt_RIGHT);
   
   return *this;
}



/*
==============================================================================
Name : open
==============================================================================
*/

Cursor & Cursor::open ()
{
   push (Opt_OPEN);
   
   return *this;
}



/*
==============================================================================
Name : close
==============================================================================
*/

Cursor & Cursor::close (bool close_flag)
{
   if (close_flag)
   {
      push (Opt_CLOSE);
   }
   else
   {
      push (Opt_OPEN);
   }
   
   return *this;
}



/*
==============================================================================
Name : set
==============================================================================
*/

void  Cursor::set ()
{
   using namespace ohm;
   
   std::string path;
   opa::Point hot_point;
   
   switch (_base)
   {
   case Base_ARROW:
      get_arrow (path, hot_point);
      break;

   case Base_RESIZE:
      get_resize (path, hot_point);
      break;

   case Base_HAND:
      get_hand (path, hot_point);
      break;

   case Base_PEN:
      get_pen (path, hot_point);
      break;

   case Base_ERASER:
      get_eraser (path, hot_point);
      break;

   default:
      assert (false);
      break;
   }
   
   assert (!path.empty ());
   
   if (path.empty ())
   {
      ohm_util_TRACE_ERROR ("missing cursor");
      
      // put default cursor
      get (path, hot_point, "arrow", opa::Point (3, 4), true);
   }
   
   reset ();
   
   opal::Cursor & cursor = opal::CursorManager::use_instance ().get (path, hot_point);
   
   opal::Application::use_instance ().set (cursor);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

Cursor::Cursor ()
{
   reset ();
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : reset
==============================================================================
*/

void  Cursor::reset ()
{
   _base = Base_NONE;
   _options.reset ();
}



/*
==============================================================================
Name : option
==============================================================================
*/

void  Cursor::push (Opt opt)
{
   assert (_base != Base_NONE);
   
   _options.set (opt);
}



/*
==============================================================================
Name : test
==============================================================================
*/

bool  Cursor::test () const
{
   return _options.none ();
}



/*
==============================================================================
Name : test
==============================================================================
*/

bool  Cursor::test (Opt opt1) const
{
   return _options == Options ().set (opt1);
}



/*
==============================================================================
Name : test
==============================================================================
*/

bool  Cursor::test (Opt opt1, Opt opt2) const
{
   return _options == Options ().set (opt1).set (opt2);
}



/*
==============================================================================
Name : get_arrow
==============================================================================
*/

void  Cursor::get_arrow (std::string & path, ohm::opa::Point & hot_point)
{
   using namespace ohm;
   
   if (test ())
   {
      get (path, hot_point, "arrow", opa::Point (3, 4), true);
   }
   
   // move
   
   else if (test (Opt_MOVE))
   {
      get (path, hot_point, "arrow.move", opa::Point (3, 4), true);
   }
   else if (test (Opt_MOVE, Opt_COPY))
   {
      get (path, hot_point, "arrow.move.copy", opa::Point (3, 4), true);
   }
}



/*
==============================================================================
Name : get_resize
==============================================================================
*/

void  Cursor::get_resize (std::string & path, ohm::opa::Point & hot_point)
{
   using namespace ohm;
   
   if (test (Opt_RIGHT))
   {
      get (path, hot_point, "resize.right", opa::Point (9, 8));
   }
}



/*
==============================================================================
Name : get_hand
==============================================================================
*/

void  Cursor::get_hand (std::string & path, ohm::opa::Point & hot_point)
{
   using namespace ohm;
   
   if (test (Opt_OPEN))
   {
      get (path, hot_point, "hand.opened", opa::Point (10, 10));
   }
   else if (test (Opt_CLOSE))
   {
      get (path, hot_point, "hand.closed", opa::Point (9, 10));
   }
}



/*
==============================================================================
Name : get_pen
==============================================================================
*/

void  Cursor::get_pen (std::string & path, ohm::opa::Point & hot_point)
{
   using namespace ohm;
   
   if (test ())
   {
      get (path, hot_point, "pen", opa::Point (3, 3), true);
   }
}



/*
==============================================================================
Name : get
==============================================================================
*/

void  Cursor::get_eraser (std::string & path, ohm::opa::Point & hot_point)
{
   using namespace ohm;
   
   if (test ())
   {
      get (path, hot_point, "eraser", opa::Point (4, 6));
   }
}



/*
==============================================================================
Name : get
==============================================================================
*/

void  Cursor::get (std::string & path, ohm::opa::Point & hot_point, const std::string & type, const ohm::opa::Point & point, bool offset_flag)
{
   hot_point = point;
   
   path = "skeleton.cursor." + type + ".png";
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
