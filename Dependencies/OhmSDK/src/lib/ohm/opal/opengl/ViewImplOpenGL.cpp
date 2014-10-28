/*****************************************************************************

        ViewImplOpenGL.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59382

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

#include "ohm/opal/opengl/ViewImplOpenGL.h"
#include "ohm/opal/opengl/OpenGLContext.h"
#include "ohm/opal/opengl/OpenGLEngine.h"
#include "ohm/opal/carbon/ViewImpl.h"
#include "ohm/opal/View.h"
#include "ohm/opal/FontManager.h"
#include "ohm/opal/ImageManager.h"
#include "ohm/opak/base/CachePool.h"
#include "ohm/util/trace.h"

#include <glut/glut.h>

#undef OHM_OPAL_COARSE_INVALIDATE
#undef OHM_OPAL_NO_INVALIDATION

#include <cassert>



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

ViewImplOpenGL::ViewImplOpenGL (ViewImpl & view_impl)
:  _view_impl (view_impl)
,  _agl_context (0)
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

ViewImplOpenGL::~ViewImplOpenGL ()
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
   Called from the VideoThread, Set up the agl context for this view.
==============================================================================
*/

void  ViewImplOpenGL::vt_init ()
{
   assert (_agl_context == 0);
   assert (_engine_ptr == 0);
   assert (_context_ptr == 0);
   
   // setup pixel format
   
   std::vector <GLint> attrib_arr;
   
   attrib_arr.push_back (AGL_NO_RECOVERY);
   attrib_arr.push_back (AGL_RGBA);
   attrib_arr.push_back (AGL_ACCELERATED);
   attrib_arr.push_back (AGL_RED_SIZE);
   attrib_arr.push_back (8);
   attrib_arr.push_back (AGL_GREEN_SIZE);
   attrib_arr.push_back (8);
   attrib_arr.push_back (AGL_BLUE_SIZE);
   attrib_arr.push_back (8);
   attrib_arr.push_back (AGL_ALPHA_SIZE);
   attrib_arr.push_back (8);
   attrib_arr.push_back (AGL_PIXEL_SIZE);
   attrib_arr.push_back (32);
   attrib_arr.push_back (AGL_DEPTH_SIZE);
   attrib_arr.push_back (0);
   
   bool use_stencil_invalidation_flag = false;
   
   if (use_stencil_invalidation_flag)
   {
      attrib_arr.push_back (AGL_STENCIL_SIZE);
      attrib_arr.push_back (1);
   }
   else
   {
      attrib_arr.push_back (AGL_STENCIL_SIZE);
      attrib_arr.push_back (0);
   }
   
   attrib_arr.push_back (AGL_MINIMUM_POLICY);
   attrib_arr.push_back (AGL_NONE);
   
   AGLPixelFormat agl_pixel_format = aglChoosePixelFormat (
      0, 0, // pixel format available for all devices on the system
      &attrib_arr [0]
   );
   
   check_agl_error ();
   assert (agl_pixel_format != 0);
   
   // setup agl context
   
   _agl_context = aglCreateContext (agl_pixel_format, 0);
   check_agl_error ();
   assert (_agl_context != 0);
   
   HIViewRef hi_view_ref = _view_impl.get_system_view ();
   assert (hi_view_ref != 0);

   bool  ok_flag = aglSetHIViewRef (_agl_context, hi_view_ref);
   check_agl_error ();
   assert (ok_flag);
   
   // set to 0 when developing, as the swap function
   // will block, sampling performance will be biased
   // set to 1 for deployment

   GLint swap = 1;
   aglSetInteger (_agl_context, AGL_SWAP_INTERVAL, &swap);
   
   aglDestroyPixelFormat (agl_pixel_format);
   
   _engine_ptr = new OpenGLEngine (_agl_context);
   _context_ptr = new OpenGLContext (*_engine_ptr);
   
   ok_flag = aglUpdateContext (_agl_context);
   check_agl_error ();
   assert (ok_flag);
}



/*
==============================================================================
Name : vt_restore
Thread : Video
Description :
   Called from the VideoThread, Restore the agl context for this view.
==============================================================================
*/

void  ViewImplOpenGL::vt_restore ()
{
   assert (_agl_context != 0);
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
   
   assert (_agl_context != 0);
   
   if (_agl_context != 0)
   {
      bool ok_flag = aglDestroyContext (_agl_context);
      check_agl_error ();
      assert (ok_flag);
      
      _agl_context = 0;
   }
}



/*
==============================================================================
Name : vt_update
Thread : Video
Description :
   Update the opengl view context in response to a change of origin or bounds
   of the view.
==============================================================================
*/

void  ViewImplOpenGL::vt_update (const opa::Size & view_size)
{
   bool ok_flag = aglUpdateContext (_agl_context);
   check_agl_error ();
   assert (ok_flag);
         
   if (_view_size != view_size)
   {
      _view_size = view_size;
      
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

void  ViewImplOpenGL::vt_draw (bool wait_vbr_flag)
{
   if (_context_ptr == 0)
   {
      // abort
      
      return;
   }
   
   //
   
   if (!_invalid_rect_arr.empty ())
   {
      assert (_engine_ptr != 0);
      
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

      _engine_ptr->present_context (wait_vbr_flag);
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

void  ViewImplOpenGL::invalidate (const opa::Rect & rect)
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

void  ViewImplOpenGL::debug_toggle_show_invalid_rect ()
{
   _show_invalid_rect_flag = !_show_invalid_rect_flag;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : check_agl_error
Description :
   trace agl errors
==============================================================================
*/

void  ViewImplOpenGL::check_agl_error ()
{
   GLenum err = aglGetError ();
   
   if (err != AGL_NO_ERROR)
   {
      const char * err_0 = (const char *) aglErrorString (err);
      
      ohm_util_TRACE_ERROR1 ("AGL : %1%", err_0);
      assert (false);
   }
}



/*
==============================================================================
Name : check_gl_error
Description :
   trace gl errors
==============================================================================
*/

void  ViewImplOpenGL::check_gl_error ()
{
   GLenum err = glGetError ();
   
   if (err != GL_NO_ERROR)
   {
      const char * err_0 = (const char *) gluErrorString (err);
      
      ohm_util_TRACE_ERROR1 ("GL : %1%", err_0);
      assert (false);
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
