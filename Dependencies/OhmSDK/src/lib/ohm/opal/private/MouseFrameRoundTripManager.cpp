/*****************************************************************************

        MouseFrameRoundTripManager.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54468

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

#include "ohm/opal/private/MouseFrameRoundTripManager.h"
#include "ohm/opal/Frame.h"
#include "ohm/opal/Event.h"

#include <algorithm>

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

MouseFrameRoundTripManager::MouseFrameRoundTripManager (Frame & frame)
:  _frame (frame)
,  _frame_path_set ()
{
   
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

MouseFrameRoundTripManager::~MouseFrameRoundTripManager ()
{
}



/*
==============================================================================
Name : notify_mouse_in
Description :
   Returns true if the mouse just entered, false otherwise.
==============================================================================
*/

bool  MouseFrameRoundTripManager::notify_mouse_in (const Event & event)
{
   FramePath frame_path = event.get_frame_path ();
   assert (frame_path.back ().first == &_frame);
   
   bool old_empty_flag = _frame_path_set.empty ();
   
   std::pair <FramePathSet::iterator, bool> pair
      = _frame_path_set.insert (frame_path);
      
   if (pair.second)
   {
      increment (frame_path);
   }
   
   bool new_empty_flag = _frame_path_set.empty ();
   
   return (old_empty_flag && !new_empty_flag);
}



/*
==============================================================================
Name : notify_mouse_out
Description :
   Returns true if the mouse just exited, false otherwise.
==============================================================================
*/

bool  MouseFrameRoundTripManager::notify_mouse_out (const Event & event)
{
   FramePath frame_path = event.get_frame_path ();
   assert (frame_path.back ().first == &_frame);

   bool old_empty_flag = _frame_path_set.empty ();
   
   size_t nbr_erased = _frame_path_set.erase (frame_path);
   assert (nbr_erased >= 0);
   assert (nbr_erased <= 1);
   
   if (nbr_erased == 1)
   {
      decrement (frame_path);
   }
   
   bool new_empty_flag = _frame_path_set.empty ();
   
   return (new_empty_flag && !old_empty_flag);
}



/*
==============================================================================
Name : unbind
Description :
   Remove all frame paths associated to '&parent' whatever the loop nbr
   (that is all frame paths which end by '&parent' then 'this')
==============================================================================
*/

void  MouseFrameRoundTripManager::unbind (Frame & parent)
{
   FramePathSet::iterator it = _frame_path_set.begin ();
   const FramePathSet::iterator it_end = _frame_path_set.end ();

   for (; it != it_end ;)
   {
      FramePathSet::iterator it_next = it;
      ++it_next;
      
      const FramePath & frame_path = *it;
      
      bool remove_flag = false;
      
      // the iterator must be dtored before its container
      
      {
         assert (frame_path.size () > 1);
      
         FramePath::const_iterator fit = frame_path.end ();
         fit -= 2;
         
         remove_flag = (fit->first == &parent);
      }
      
      if (remove_flag)
      {
         decrement (frame_path);

         _frame_path_set.erase (it);
      }

      it = it_next;
   }
}



/*
==============================================================================
Name : unbind
Description :
   Remove all frame paths which contains '&parent' followed by '&frame'
Note :
   This is used when 'frame' is going to be deleted. However '_frame'
   might be a sub child of 'frame' and 'frame' might not exist in the
   future, leaving invalid frame paths in 'this'.
==============================================================================
*/

void  MouseFrameRoundTripManager::unbind (Frame & parent, Frame & frame)
{
   FramePathSet::iterator it = _frame_path_set.begin ();
   const FramePathSet::iterator it_end = _frame_path_set.end ();

   for (; it != it_end ;)
   {
      FramePathSet::iterator it_next = it;
      ++it_next;
      
      const FramePath & frame_path = *it;
      
      assert (frame_path.size () > 1);
      
      if (has (frame_path, parent, frame))
      {
         decrement (frame_path);

         _frame_path_set.erase (it);
      }

      it = it_next;
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : increment
==============================================================================
*/

void  MouseFrameRoundTripManager::increment (const FramePath & frame_path)
{
   FramePath::const_reverse_iterator it = frame_path.rbegin ();
   const FramePath::const_reverse_iterator it_end = frame_path.rend ();
   
   for (; it != it_end ; ++it)
   {
      Frame * frame_ptr = it->first;
      assert (frame_ptr != 0);
      
      frame_ptr->increment_mouse_in ();
   }
}



/*
==============================================================================
Name : decrement
==============================================================================
*/

void  MouseFrameRoundTripManager::decrement (const FramePath & frame_path)
{
   FramePath::const_reverse_iterator it = frame_path.rbegin ();
   const FramePath::const_reverse_iterator it_end = frame_path.rend ();
   
   for (; it != it_end ; ++it)
   {
      Frame * frame_ptr = it->first;
      assert (frame_ptr != 0);
      
      frame_ptr->decrement_mouse_in ();
   }
}



/*
==============================================================================
Name : has
Description :
   Returns true iff '&parent' followed by '&frame' is present in the
   'frame_path', irrespective of loop nbr
==============================================================================
*/

bool  MouseFrameRoundTripManager::has (const FramePath & frame_path, Frame & parent, Frame & frame)
{
   bool ret_val = false;
   
   FramePath::const_reverse_iterator it = frame_path.rbegin ();
   const FramePath::const_reverse_iterator it_end = frame_path.rend ();
   
   for (; it != it_end ; ++it)
   {
      Frame * frame_ptr = it->first;
      assert (frame_ptr != 0);
      
      if (frame_ptr == &frame)
      {
         break;
      }
   }
   
   if (it != it_end)
   {
      ++it;
   }
   
   if (it != it_end)
   {
      Frame * frame_ptr = it->first;
      assert (frame_ptr != 0);
      
      ret_val = frame_ptr == &parent;
   }
   
   return ret_val;
}



/*
==============================================================================
Name : operator ()
Description :
   Perform the comparison in reverse order for speed.
==============================================================================
*/

bool  MouseFrameRoundTripManager::ReverseLess::operator () (const FramePath & l_op, const FramePath & r_op) const
{
   return std::lexicographical_compare (
      l_op.rbegin (), l_op.rend (),
      r_op.rbegin (), r_op.rend ()
   );
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
