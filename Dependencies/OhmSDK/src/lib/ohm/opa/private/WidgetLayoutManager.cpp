/*****************************************************************************

        WidgetLayoutManager.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 58088

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

#include "ohm/opa/private/WidgetLayoutManager.h"
#include "ohm/opa/base/WidgetEmbedderBase.h"
#include "ohm/task/RunLoopGui.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

WidgetLayoutManager::~WidgetLayoutManager ()
{
}



/*
==============================================================================
Name : init
==============================================================================
*/

void  WidgetLayoutManager::init ()
{
   task::RunLoopGui::use_instance ().add (_observer);
}



/*
==============================================================================
Name : restore
==============================================================================
*/

void  WidgetLayoutManager::restore ()
{
   task::RunLoopGui::use_instance ().remove (_observer);
}



/*
==============================================================================
Name : invalidate
==============================================================================
*/

void  WidgetLayoutManager::invalidate (WidgetEmbedderBase & widget)
{
   assert (&widget != 0);
   
   if (_active_flag)
   {
      _invalid_widget_ptr_set.insert (&widget);
   }
}



/*
==============================================================================
Name : remove
==============================================================================
*/

void  WidgetLayoutManager::remove (WidgetEmbedderBase & widget)
{
   assert (&widget != 0);
   
   _invalid_widget_ptr_set.erase (&widget);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

WidgetLayoutManager::WidgetLayoutManager ()
:  _observer ("ohm.opa.WidgetLayoutManager")
,  _active_flag (true)
,  _invalid_widget_ptr_set ()
{
   task::RunLoopGui::use_instance ();
   
   _observer.bind_post_process <
      WidgetLayoutManager,
      &WidgetLayoutManager::post_process
   > (*this);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : post_process
==============================================================================
*/

void  WidgetLayoutManager::post_process ()
{
   if (_active_flag)
   {
      WidgetPtrSet::iterator it = _invalid_widget_ptr_set.begin ();
      const WidgetPtrSet::iterator it_end = _invalid_widget_ptr_set.end ();
      
      for (; it != it_end ;)
      {
         WidgetPtrSet::iterator it_next = it;
         ++it_next;
         
         WidgetEmbedderBase * widget_ptr = *it;
         
         // erase before because...
         _invalid_widget_ptr_set.erase (it);
         
         // ... it might trigger an invalidation for the same widget
         widget_ptr->update_layout ();
         
         it = it_next;
      }
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
