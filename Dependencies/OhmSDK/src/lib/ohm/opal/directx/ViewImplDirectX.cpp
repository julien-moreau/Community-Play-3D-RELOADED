/*****************************************************************************

        ViewImplDirectX.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59978

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
#include "ohm/opal/directx/ViewImplDirectX.h"
#include "ohm/opal/directx/DirectXContext.h"
#include "ohm/opal/directx/DirectXEngine.h"
#include "ohm/opal/win32/ViewImpl.h"
#include "ohm/opal/View.h"
#include "ohm/opal/ImageManager.h"
#include "ohm/opal/FontManager.h"
#include "ohm/opak/base/CachePool.h"
#include "ohm/util/trace.h"
#include "ohm/sys/TimerReliable.h"

#include <cassert>

#undef OHM_OPAL_COARSE_INVALIDATE
#undef OHM_OPAL_NO_INVALIDATION



namespace ohm
{
namespace opal
{




/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


/*
==============================================================================
Name : ctor
Thread : Main
==============================================================================
*/

ViewImplDirectX::ViewImplDirectX (ViewImpl & view_impl)
:  _view_impl (view_impl)
,  _engine_ptr (0)
,  _context_ptr (0)
,  _view_size (opa::Size::_zero)
,  _invalid_rect_arr ()
,  _show_invalid_rect_flag (false)
{
}



/*
==============================================================================
Name : dtor
Thread : Main
==============================================================================
*/

ViewImplDirectX::~ViewImplDirectX ()
{
   try
   {
   }
   catch (...)
   {
      assert (false);
   }
}




/*
==============================================================================
Name : vt_init
Thread : Video
Description :
   Called from the VideoThread, Set up the directx context for this view.
==============================================================================
*/

void  ViewImplDirectX::vt_init ()
{
   assert (_engine_ptr == 0);
   assert (_context_ptr == 0);
   
   // no thread concurrency problem
   ::HWND hwnd = _view_impl.get_system_view ();
   
   _engine_ptr = new DirectXEngine (hwnd);
   _context_ptr = new DirectXContext (*_engine_ptr);
}



/*
==============================================================================
Name : vt_restore
Thread : Video
Description :
   Called from the VideoThread, Restore the directx context for this view.
==============================================================================
*/

void  ViewImplDirectX::vt_restore ()
{
   assert (_engine_ptr != 0);
   assert (_context_ptr != 0);
   
   if (_engine_ptr != 0)
   {
      // release all resources associated to this engine
      
      opak::CachePool::use_instance ().release (*_engine_ptr);
      FontManager::use_instance ().release (*_engine_ptr);
      ImageManager::use_instance ().release (*_engine_ptr);
   }
      
   delete _context_ptr;
   _context_ptr = 0;
   
   delete _engine_ptr;
   _engine_ptr = 0;
}



/*
==============================================================================
Name : vt_update
Thread : Video
Description :
   Update the directx view context in response to a change of origin or bounds
   of the view.
==============================================================================
*/

void  ViewImplDirectX::vt_update (const opa::Size & view_size)
{
   if (_view_size != view_size)
   {
      _view_size = view_size;
      
      assert (_engine_ptr != 0);
      
      _engine_ptr->hwnd_size_changed (
         int (_view_size._width), int (_view_size._height)
      );
      
      assert (_context_ptr != 0);
   
      _context_ptr->set_context_size (_view_size);
      
      _invalid_rect_arr.clear ();
      _invalid_rect_arr.push_back (opa::Rect (opa::Point::_zero, _view_size));
   }
}



/*
==============================================================================
Name : vt_draw
Thread : Video
Description :
   wait for vertical retrace iff 'wait_vbr_flag' is true.
==============================================================================
*/

void  ViewImplDirectX::vt_draw (bool wait_vbr_flag)
{
   if (_context_ptr == 0)
   {
      // abort
      
      return;
   }
   
   //
   
   assert (_engine_ptr != 0);
   
   if (!_invalid_rect_arr.empty ())
   {
      bool ok_flag = _engine_ptr->begin_scene ();
      
      if (ok_flag)
      {
         if (!_invalid_rect_arr.empty ())
         {
            size_t i = 0;
         
            if (_show_invalid_rect_flag)
            {
               for (i = 0 ; i < _invalid_rect_arr.size () ; ++i)
               {
                  const opa::Rect & invalid_rect = _invalid_rect_arr [i];
                  
                  _engine_ptr->debug_fill (invalid_rect);
               }

               _engine_ptr->debug_flash ();
            }
         
            View & view = _view_impl.use_view ();
         
            for (i = 0 ; i < _invalid_rect_arr.size () ; ++i)
            {
               const opa::Rect & invalid_rect = _invalid_rect_arr [i];
               
               _context_ptr->set_clip_rect (invalid_rect);
            
               view.draw (*_context_ptr);
            }
            
            _invalid_rect_arr.clear ();
         }
      }
      else
      {
         ::Sleep (100);
      }
      
      _engine_ptr->end_scene ();
      _engine_ptr->present ();
   }
   else
   {
      // There is nothing to draw. Since the video thread loop
      // is called as fast as possible, sleep a little time here
      
      // we may achieve a 1 ms resolution
      ::Sleep (1);
   }
}



/*
==============================================================================
Name : invalidate
Thread : Video
Description :
   Called from video thread, in response to the Clock tick.
==============================================================================
*/

void  ViewImplDirectX::invalidate (const opa::Rect & rect)
{
#if defined (OHM_OPAL_NO_INVALIDATION)
   return;
#endif

   if (!rect.is_empty ())
   {
      opa::Rect invalid_rect (opa::Point::_zero, _view_size);

#if !defined (OHM_OPAL_COARSE_INVALIDATE)
      invalid_rect &= rect;
#endif
      
      if (!invalid_rect.is_empty ())
      {
         size_t i = 0;
         
         for (; i < _invalid_rect_arr.size () ; ++i)
         {
            opa::Rect & rect = _invalid_rect_arr [i];
            
            if (rect.is_intersecting (invalid_rect))
            {
               rect |= invalid_rect;
               
               break;
            }
         }
         
         if (i == _invalid_rect_arr.size ())
         {
            _invalid_rect_arr.push_back (invalid_rect);
         }
      }
   }
}



/*
==============================================================================
Name : debug_toggle_show_invalid_rect
==============================================================================
*/

void  ViewImplDirectX::debug_toggle_show_invalid_rect ()
{
   _show_invalid_rect_flag = !_show_invalid_rect_flag;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
