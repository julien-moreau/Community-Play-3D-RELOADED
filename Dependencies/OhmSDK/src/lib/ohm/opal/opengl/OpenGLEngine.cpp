/*****************************************************************************

        OpenGLEngine.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 66813

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

#include "ohm/opal/opengl/OpenGLEngine.h"
#include "ohm/opal/private/FragmentProgramManager.h"
#include "ohm/opal/Cache.h"
#include "ohm/opal/ImageManager.h"
#include "ohm/opal/Image.h"
#include "ohm/opa/Path.h"
#include "ohm/opa/Quad.h"
#include "ohm/math/def.h"
#include "ohm/opal/def.h"
#include "ohm/lang/def.h"
#include "ohm/util/trace.h"

#include <AGL/aglMacro.h>

#undef AGL_MACRO_CONTEXT
#define AGL_MACRO_CONTEXT _agl_context

#include <glut/glut.h>

#include <set>
#include <cassert>
#include <cmath>

#undef OHM_OPAL_ENGINE_DEBUG_FLAG

#if defined (OHM_OPAL_ENGINE_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_OPAL_ENGINE_DEBUG_FLAG is defined in shipping level code !
   #endif

   #define eng_TRACE(msg_0)   ohm_util_TRACE_DEBUG(msg_0)
   #define eng_TRACE1(fmt_0, arg1)  ohm_util_TRACE_DEBUG1(fmt_0, arg1)
   #define eng_TRACE2(fmt_0, arg1, arg2)  ohm_util_TRACE_DEBUG2(fmt_0, arg1, arg2)
   #define eng_TRACE3(fmt_0, arg1, arg2, arg3)  ohm_util_TRACE_DEBUG3(fmt_0, arg1, arg2, arg3)
   #define eng_TRACE4(fmt_0, arg1, arg2, arg3, arg4)  ohm_util_TRACE_DEBUG4(fmt_0, arg1, arg2, arg3, arg4)
#else
   #define eng_TRACE(msg_0)
   #define eng_TRACE1(fmt_0, arg1)
   #define eng_TRACE2(fmt_0, arg1, arg2)
   #define eng_TRACE3(fmt_0, arg1, arg2, arg3)
   #define eng_TRACE4(fmt_0, arg1, arg2, arg3, arg4)

#endif



namespace ohm
{
namespace opal
{



// http://developer.apple.com/graphicsimaging/opengl/optimizingdata.html
// VERTEX_ARRAY_BUFFER_BINDING_ARB

#if (ohm_opal_API == ohm_opal_API_OPENGL)

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

OpenGLEngine::OpenGLEngine (AGLContext agl_context)
:  _thread_id (sys::ThreadMgr::get_current_thread_id ())
,  _agl_context (agl_context)
,  _video_memory_size (0)
{
   _cur_view_size = opa::Size::_zero;
   _context_id = 0;
   _cur_translation = opa::Point::_zero;
   _cur_scissor_rect = opa::Rect::_zero;
   _cur_fragment_program_enabled_flag = false;
   
   glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
   
   // for color masks
   glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   
   glDisable (GL_DITHER);
   glDisable (GL_ALPHA_TEST);
   glDisable (GL_STENCIL_TEST);
   glDisable (GL_FOG);
   glDisable (GL_TEXTURE_2D);
   glDisable (GL_TEXTURE_RECTANGLE_ARB);
   glDisable (GL_DEPTH_TEST);
   glPixelZoom (1.0,1.0);

   glEnable (GL_BLEND);
   
   glEnable (GL_SCISSOR_TEST);

   glLineWidth (1.f);

   glDisable (GL_MULTISAMPLE_ARB);
   
   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity ();

   glMatrixMode (GL_PROJECTION);
   
   glBlendEquation (GL_FUNC_ADD);
   glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   
   _cur_blend_equation = BlendEquation_ADD;
   _cur_blend_func = BlendFunc_ONE_ONE_MINUS_SRC_ALPHA;
   
   glClearColor (1.0f, 1.0f, 1.0f, 1.0f);
   glClear (GL_COLOR_BUFFER_BIT);
   
   glClearStencil (1);
   glClear (GL_STENCIL_BUFFER_BIT);
   
   build_strategy ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

OpenGLEngine::~OpenGLEngine ()
{
}



/*
==============================================================================
Name : check_thread_id
==============================================================================
*/

void  OpenGLEngine::check_thread_id () const
{
   sys::ThreadMgr::IdType cur_thread_id
      = sys::ThreadMgr::get_current_thread_id ();
   
   assert (cur_thread_id == _thread_id);
}



/*
==============================================================================
Name : get_video_memory_size
==============================================================================
*/

size_t   OpenGLEngine::get_video_memory_size () const
{
   return _video_memory_size;
}



/*
==============================================================================
Name : init
==============================================================================
*/

/*void   OpenGLEngine::init (bool double_buffer_flag)
{
   _strategy._use_double_buffer_flag = double_buffer_flag;
}*/



/*
==============================================================================
Name : update_context
==============================================================================
*/

void  OpenGLEngine::update_context (GLuint context_id, const opa::Size & size, const GraphicState & graphic_state)
{
   const bool frame_buffer_flag = context_id != 0;
   
   bool frame_buffer_changed_flag = false;
   
   if (context_id != _context_id)
   {
      eng_TRACE1 ("glBindFramebufferEXT %1%", context_id);
      glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, context_id);
      
      _context_id = context_id;
      
      frame_buffer_changed_flag = true;
   }
   
   bool view_port_changed_flag = false;
   
   bool force_viewport_call_flag
      = frame_buffer_changed_flag
   ;
   
   #warning
   force_viewport_call_flag = true;
   
   if ((_cur_view_size != size) || force_viewport_call_flag)
   {
      eng_TRACE2 ("glViewport %1% %2%", size._width, size._height);
      glViewport (
         0, 0,
         size._width, size._height
      );
      
      view_port_changed_flag = true;
   }
   
   const opa::Point & translation = graphic_state._total_translation;
   
   bool projection_matrix_changed_flag = false;
   
   if (view_port_changed_flag || (translation != _cur_translation))
   {
      float bottom = size._height;
      float top = 0;
      
      if (frame_buffer_flag)
      {
         std::swap (bottom, top);
      }
      
      // GL_PROJECTION
      float invx = 1.f / size._width;
      float invy = 1.f / (top - bottom);
      float dx = 2.f * invx;
      float dy = 2.f * invy;
      float tx = -1.f;
      tx += translation._x * dx;
      float ty = (bottom > top) ? 1 : -1;
      ty += translation._y * dy;
      
      float mat [16] = {
         dx,  0,  0, 0,
          0, dy,  0, 0,
          0,  0,  0, 0,
         tx, ty,  0, 1
      };
      
      glLoadMatrixf (mat);
      
      _cur_translation = translation;
      _cur_view_size = size;
      
      projection_matrix_changed_flag = true;
   }
   
   switch (graphic_state._blend_mode)
   {
   case BlendMode_NORMAL:
      set_blend_mode_normal ();
      break;

   case BlendMode_MULTIPLY:
      set_blend_mode_multiply ();
      break;

   case BlendMode_SCREEN:
      set_blend_mode_screen ();
      break;

   case BlendMode_REPLACE:
      set_blend_mode_replace ();
      break;

   case BlendMode_ADD:
      set_blend_mode_add ();
      break;

   case BlendMode_MAX:
      set_blend_mode_max ();
      break;

   case BlendMode_MIN:
      set_blend_mode_min ();
      break;
   
   default:
      assert (false);
      break;
   }
   
   opa::Rect clip_rect = graphic_state._global_clip_rect;
   assert (!clip_rect.is_empty ());
   
   // compensate small calculation imprecisions
   clip_rect.round ();
   
   opa::Rect view_rect (opa::Point::_zero, _cur_view_size);
   
   // clip rect for cache might be slightly bigger than the view
   //assert ((clip_rect | view_rect) == view_rect);
   
   if (!frame_buffer_flag)
   {
      clip_rect._origin._y
         = _cur_view_size._height
         - (clip_rect._origin._y + clip_rect._size._height);
   }
   
   bool force_scissor_call_flag
      = projection_matrix_changed_flag
      || frame_buffer_changed_flag
      || view_port_changed_flag
   ;
   
   if ((_cur_scissor_rect != clip_rect) || force_scissor_call_flag)
   {
      // fix for ATI Radeon HD 4870
      // it seems that it just need to be changed, or probably the driver
      // thinks it is the same and does not apply it
      // this work actually with any rect different from the clip_rect
      glScissor (0, 0, 0, 0);
      
      eng_TRACE4 ("glScissor %1% %2% %3% %4%", clip_rect._origin._x, clip_rect._origin._y, clip_rect._size._width, clip_rect._size._height);
      glScissor (
         clip_rect._origin._x,
         clip_rect._origin._y,
         clip_rect._size._width,
         clip_rect._size._height
      );
      
      _cur_scissor_rect = clip_rect;
   }
   
   bool fragment_program_enabled_flag = (graphic_state._fragment_program_proc != 0);
   
   if (_cur_fragment_program_enabled_flag != fragment_program_enabled_flag)
   {
      if (fragment_program_enabled_flag)
      {
         glEnable (GL_FRAGMENT_PROGRAM_ARB);
      }
      else
      {
         glDisable (GL_FRAGMENT_PROGRAM_ARB);
      }
      
      _cur_fragment_program_enabled_flag = fragment_program_enabled_flag;
   }
   
   if (fragment_program_enabled_flag)
   {
      GLuint program_id = FragmentProgramManager::use_instance ()
         .get (graphic_state._fragment_program_proc);
      
      assert (program_id != 0);
   
      glBindProgramARB (
         GL_FRAGMENT_PROGRAM_ARB,
         program_id
      );
      
      glProgramLocalParameter4fARB (
         GL_FRAGMENT_PROGRAM_ARB,
         0, // index
         graphic_state._fragment_var_arr [0],
         graphic_state._fragment_var_arr [1],
         graphic_state._fragment_var_arr [2],
         graphic_state._fragment_var_arr [3]
      );
   }
}



/*
==============================================================================
Name : present_context
==============================================================================
*/

void  OpenGLEngine::present_context (bool wait_vbr_flag)
{
   if (_context_id != 0)
   {
      eng_TRACE1 ("glBindFramebufferEXT %1%", 0);
      glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, 0);
      
      _context_id = 0;
   }
   
   if (_strategy._use_double_buffer_flag)
   {
      assert (_agl_context != 0);
      
      aglSwapBuffers (_agl_context);
   }
   else
   {
      if (wait_vbr_flag)
      {
         glFinish ();
      }
      else
      {
         glFlush ();
      }
   }
}



/*
==============================================================================
Name : fill
==============================================================================
*/

void  OpenGLEngine::fill (const opa::Path & path, const Color & color)
{
   const float r = color._r * color._a;
   const float g = color._g * color._a;
   const float b = color._b * color._a;
   const float a = color._a;
   
   eng_TRACE4 ("glColor4f %1% %2% %3% %4%", r, g, b, a);
   glColor4f (r, g, b, a);
   
   const std::vector <opa::Point> & pt_arr = path.get_polygon ();
   
   eng_TRACE ("glBegin GL_POLYGON");
   glBegin (GL_POLYGON);
   
   for (size_t i = 0 ; i < pt_arr.size () ; ++i)
   {
      const opa::Point & pt = pt_arr [i];
      
      eng_TRACE2 ("  glVertex2f %1% %2%", pt._x, pt._y);
      glVertex2f (pt._x, pt._y);
   }
   
   eng_TRACE ("glEnd");
   glEnd ();
}



/*
==============================================================================
Name : fill
==============================================================================
*/

void  OpenGLEngine::fill (const opa::Rect & rect, const Color & color)
{
   const float r = color._r * color._a;
   const float g = color._g * color._a;
   const float b = color._b * color._a;
   const float a = color._a;
   
   eng_TRACE4 ("glColor4f %1% %2% %3% %4%", r, g, b, a);
   glColor4f (r, g, b, a);
   
   eng_TRACE4 (
      "glBegin GL_QUADS %1% %2% %3% %4%",
      rect._origin._x, rect._origin._y,
      rect._size._width, rect._size._height
   );
   glBegin (GL_QUADS);
   
   glVertex2f (rect._origin._x, rect._origin._y);
   glVertex2f (rect._origin._x + rect._size._width, rect._origin._y);
   glVertex2f (rect._origin._x + rect._size._width, rect._origin._y + rect._size._height);
   glVertex2f (rect._origin._x, rect._origin._y + rect._size._height);
   
   eng_TRACE ("glEnd");
   glEnd ();
}



/*
==============================================================================
Name : fill
==============================================================================
*/

void  OpenGLEngine::fill (const opa::Quad & vertices, const Color & color)
{
   const float r = color._r * color._a;
   const float g = color._g * color._a;
   const float b = color._b * color._a;
   const float a = color._a;
   
   eng_TRACE4 ("glColor4f %1% %2% %3% %4%", r, g, b, a);
   glColor4f (r, g, b, a);
   
   eng_TRACE ("glBegin GL_QUADS");
   glBegin (GL_QUADS);
   
   {
      const opa::Point & pt = vertices [0];

      eng_TRACE2 ("  pt %1% %2%", pt._x, pt._y);
      glVertex2f (pt._x, pt._y);
   }
   
   {
      const opa::Point & pt = vertices [1];

      eng_TRACE2 ("  pt %1% %2%", pt._x, pt._y);
      glVertex2f (pt._x, pt._y);
   }

   {
      const opa::Point & pt = vertices [2];

      eng_TRACE2 ("  pt %1% %2%", pt._x, pt._y);
      glVertex2f (pt._x, pt._y);
   }

   {
      const opa::Point & pt = vertices [3];

      eng_TRACE2 ("  pt %1% %2%", pt._x, pt._y);
      glVertex2f (pt._x, pt._y);
   }
   
   eng_TRACE ("glEnd");
   glEnd ();
}



/*
==============================================================================
Name : stroke
==============================================================================
*/

void  OpenGLEngine::stroke (const opa::Path & path, const Color & color)
{
   const float r = color._r * color._a;
   const float g = color._g * color._a;
   const float b = color._b * color._a;
   const float a = color._a;
   
   eng_TRACE4 ("glColor4f %1% %2% %3% %4%", r, g, b, a);
   glColor4f (r, g, b, a);
   
   const std::vector <opa::Point> & pt_arr = path.get_polygon ();
   
   eng_TRACE ("glBegin GL_LINE_STRIP");
   glBegin (GL_LINE_STRIP);
   
   for (size_t i = 0 ; i < pt_arr.size () ; ++i)
   {
      const opa::Point & pt = pt_arr [i];
      
      eng_TRACE2 ("  glVertex2f %1% %2%", pt._x, pt._y);
      glVertex2f (pt._x, pt._y);
   }
   
   eng_TRACE ("glEnd");
   glEnd ();
}



/*
==============================================================================
Name : draw
==============================================================================
*/

void  OpenGLEngine::draw (const Image & image, const opa::Quad & vertices, const opa::Quad & tex_coords, float alpha)
{
   if (image._data_ref != 0)
   {
      GLenum cur_tex_target = image._data_ref->_tex_target;
      
      eng_TRACE1 ("glEnable %1%", cur_tex_target);
      glEnable (cur_tex_target);
      
      GLuint texture_id = image._data_ref->_texture_id;
      
      eng_TRACE1 ("glBindTexture %1%", texture_id);
      glBindTexture (cur_tex_target, texture_id);
      
      eng_TRACE1 ("glColor4f %1%", alpha);
      glColor4f (alpha, alpha, alpha, alpha);
      
      eng_TRACE ("glBegin GL_QUADS");
      glBegin (GL_QUADS);
      
      {
         const opa::Point & tx = tex_coords [0];
         const opa::Point & pt = vertices [0];

         eng_TRACE4 ("  tx %1% %2% pt %3% %4%", tx._x, tx._y, pt._x, pt._y);
         glTexCoord2f (tx._x, tx._y);
         glVertex2f (pt._x, pt._y);
      }
      
      {
         const opa::Point & tx = tex_coords [1];
         const opa::Point & pt = vertices [1];

         eng_TRACE4 ("  tx %1% %2% pt %3% %4%", tx._x, tx._y, pt._x, pt._y);
         glTexCoord2f (tx._x, tx._y);
         glVertex2f (pt._x, pt._y);
      }
      
      {
         const opa::Point & tx = tex_coords [2];
         const opa::Point & pt = vertices [2];

         eng_TRACE4 ("  tx %1% %2% pt %3% %4%", tx._x, tx._y, pt._x, pt._y);
         glTexCoord2f (tx._x, tx._y);
         glVertex2f (pt._x, pt._y);
      }
      
      {
         const opa::Point & tx = tex_coords [3];
         const opa::Point & pt = vertices [3];

         eng_TRACE4 ("  tx %1% %2% pt %3% %4%", tx._x, tx._y, pt._x, pt._y);
         glTexCoord2f (tx._x, tx._y);
         glVertex2f (pt._x, pt._y);
      }
      
      eng_TRACE ("glEnd");
      glEnd ();
      
      glDisable (cur_tex_target);
   }
}



/*
==============================================================================
Name : draw
==============================================================================
*/

void  OpenGLEngine::draw (const Image & mask, const opa::Quad & vertices, const opa::Quad & tex_coords, const Color & color)
{
   if (mask._data_ref != 0)
   {
      GLenum cur_tex_target = mask._data_ref->_tex_target;
      
      eng_TRACE1 ("glEnable %1%", cur_tex_target);
      glEnable (cur_tex_target);
      
      GLuint texture_id = mask._data_ref->_texture_id;
      
      eng_TRACE1 ("glBindTexture %1%", texture_id);
      glBindTexture (cur_tex_target, texture_id);
      
      float r = color._r * color._a;
      float g = color._g * color._a;
      float b = color._b * color._a;
      float a = color._a;
      
      eng_TRACE4 ("glColor4f %1% %2% %3% %4%", r, g, b, a);
      glColor4f (r, g, b, a);
      
      eng_TRACE ("glBegin GL_QUADS");
      glBegin (GL_QUADS);
      
      {
         const opa::Point & tx = tex_coords [0];
         const opa::Point & pt = vertices [0];

         eng_TRACE4 ("  tx %1% %2% pt %3% %4%", tx._x, tx._y, pt._x, pt._y);
         glTexCoord2f (tx._x, tx._y);
         glVertex2f (pt._x, pt._y);
      }
      
      {
         const opa::Point & tx = tex_coords [1];
         const opa::Point & pt = vertices [1];

         eng_TRACE4 ("  tx %1% %2% pt %3% %4%", tx._x, tx._y, pt._x, pt._y);
         glTexCoord2f (tx._x, tx._y);
         glVertex2f (pt._x, pt._y);
      }
      
      {
         const opa::Point & tx = tex_coords [2];
         const opa::Point & pt = vertices [2];

         eng_TRACE4 ("  tx %1% %2% pt %3% %4%", tx._x, tx._y, pt._x, pt._y);
         glTexCoord2f (tx._x, tx._y);
         glVertex2f (pt._x, pt._y);
      }
      
      {
         const opa::Point & tx = tex_coords [3];
         const opa::Point & pt = vertices [3];

         eng_TRACE4 ("  tx %1% %2% pt %3% %4%", tx._x, tx._y, pt._x, pt._y);
         glTexCoord2f (tx._x, tx._y);
         glVertex2f (pt._x, pt._y);
      }
      
      eng_TRACE ("glEnd");
      glEnd ();
      
      glDisable (cur_tex_target);
   }
}



/*
==============================================================================
Name : prepare
Description :
   Prepare the sizes of the texture with respect of current strategy and
   hardware limitations.
==============================================================================
*/

void  OpenGLEngine::prepare (Image & image, size_t width, size_t height)
{
   image._width = width;
   image._height = height;
   
   if (_strategy._support_npot_flag || _strategy._support_rect_flag)
   {
      image._tex_width = width;
      image._tex_height = height;
   }
   else
   {
      image._tex_width = get_next_pot (width);
      image._tex_height = get_next_pot (height);
   }
   
   image._scanline_width = ((image._tex_width + 31) / 32) * 32;
}



/*
==============================================================================
Name : create
==============================================================================
*/

void  OpenGLEngine::create (Image & image, const PixArgb * buf_ptr, const Image::Conf & conf, bool buf_owner_flag)
{
   assert (image._data_ref == 0);
   
   image._eng_ptr = this;
   
   GLenum tex_filter = GL_NEAREST;
   GLenum tex_wrap_s = GL_CLAMP_TO_EDGE;
   GLenum tex_wrap_t = GL_CLAMP_TO_EDGE;
   
   image._data_ref = new Image::Data;
   
   Image::Data * data_ref = image._data_ref;
   
   data_ref->_retain_cnt = 1;
   data_ref->_buf_ptr = buf_ptr;
   data_ref->_buf_owner_flag = buf_owner_flag;

   make_tex_props (image, tex_filter, tex_wrap_s, tex_wrap_t, conf);
   
   GLenum cur_tex_target = data_ref->_tex_target;
   
   eng_TRACE1 ("glEnable %1%", cur_tex_target);
   glEnable (cur_tex_target);
   eng_TRACE1 ("glBindTexture %1%", data_ref->_texture_id);
   glBindTexture (cur_tex_target, data_ref->_texture_id);
   glTexParameteri (cur_tex_target, GL_TEXTURE_STORAGE_HINT_APPLE, GL_STORAGE_CACHED_APPLE);
   glPixelStorei (GL_UNPACK_CLIENT_STORAGE_APPLE, GL_TRUE);
   glPixelStorei (GL_UNPACK_ROW_LENGTH, image._scanline_width);
   glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
   
   // we cannot mipmap with GL_TEXTURE_RECTANGLE_ARB
   
   glTexParameteri (cur_tex_target, GL_TEXTURE_MIN_FILTER, tex_filter);
   glTexParameteri (cur_tex_target, GL_TEXTURE_MAG_FILTER, tex_filter);
   glTexParameteri (cur_tex_target, GL_TEXTURE_WRAP_S, tex_wrap_s);
   glTexParameteri (cur_tex_target, GL_TEXTURE_WRAP_T, tex_wrap_t);
   
   // to check ?
   // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, pTexture->GetEnvMode());
   
   glTexImage2D (
      cur_tex_target, 0, GL_RGBA8,
      image._tex_width, image._tex_height, 0,
      GL_BGRA_EXT, GL_UNSIGNED_INT_8_8_8_8_REV, buf_ptr
   );
   
   GLenum err = glGetError ();
   
   if (err != GL_NO_ERROR)
   {
      const char * err_0 = (const char *) gluErrorString (err);
      
      ohm_util_TRACE_ERROR1 ("GL : %1%", err_0);
      assert (false);
   }
   
   glDisable (cur_tex_target);
}



/*
==============================================================================
Name : release
==============================================================================
*/

void  OpenGLEngine::release (Image & image)
{
   glDeleteTextures (1, &image._data_ref->_texture_id);
   image._data_ref->_texture_id = 0;
}



/*
==============================================================================
Name : create
==============================================================================
*/

void  OpenGLEngine::create (GLuint & frame_buffer_id, Image & image, const Image::Conf & conf)
{
   assert (image._data_ref == 0);
   
   image._eng_ptr = this;
   
   GLenum tex_filter = GL_NEAREST;
   GLenum tex_wrap_s = GL_CLAMP_TO_EDGE;
   GLenum tex_wrap_t = GL_CLAMP_TO_EDGE;

   image._data_ref = new Image::Data;
   
   Image::Data * data_ref = image._data_ref;
   
   data_ref->_retain_cnt = 1;
   data_ref->_buf_ptr = 0;
   data_ref->_buf_owner_flag = false;

   make_tex_props (image, tex_filter, tex_wrap_s, tex_wrap_t, conf);
   
   GLenum cur_tex_target = data_ref->_tex_target;
   
   eng_TRACE1 ("glEnable %1%", cur_tex_target);
   glEnable (cur_tex_target);
   eng_TRACE1 ("glBindTexture %1%", data_ref->_texture_id);
   glBindTexture (cur_tex_target, data_ref->_texture_id);
   glTexParameteri (cur_tex_target, GL_TEXTURE_STORAGE_HINT_APPLE, GL_STORAGE_CACHED_APPLE);
   glPixelStorei (GL_UNPACK_CLIENT_STORAGE_APPLE, GL_FALSE);
   glPixelStorei (GL_UNPACK_ROW_LENGTH, image._scanline_width);
   glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

   glTexParameteri (cur_tex_target, GL_TEXTURE_MIN_FILTER, tex_filter);
   glTexParameteri (cur_tex_target, GL_TEXTURE_MAG_FILTER, tex_filter);
   glTexParameteri (cur_tex_target, GL_TEXTURE_WRAP_S, tex_wrap_s);
   glTexParameteri (cur_tex_target, GL_TEXTURE_WRAP_T, tex_wrap_t);

   glTexImage2D (
      cur_tex_target, 0, GL_RGBA8,
      image._tex_width, image._tex_height, 0,
      GL_BGRA_EXT, GL_UNSIGNED_INT_8_8_8_8_REV, 0
   );
   
   glGenFramebuffersEXT (1, &frame_buffer_id);
   eng_TRACE1 ("glBindFramebufferEXT %1%", frame_buffer_id);
   glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, frame_buffer_id);
   
   glFramebufferTexture2DEXT (
      GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
      cur_tex_target, data_ref->_texture_id, 0
   );
   
   GLenum status = glCheckFramebufferStatusEXT (GL_FRAMEBUFFER_EXT);
   assert (status == GL_FRAMEBUFFER_COMPLETE_EXT);
   
   eng_TRACE1 ("glBindFramebufferEXT %1%", 0);
   glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, 0);
   
   _context_id = 0;
   
   glDisable (cur_tex_target);
}



/*
==============================================================================
Name : release
==============================================================================
*/

void  OpenGLEngine::release (GLuint & frame_buffer_id)
{
   glDeleteFramebuffersEXT (1, &frame_buffer_id);
   frame_buffer_id = 0;
}



/*
==============================================================================
Name : create
==============================================================================
*/

void  OpenGLEngine::create (GLuint & program_id, FragmentProgramProc fragment_program_proc)
{
   glGenProgramsARB (1, &program_id);
   
   const std::string & program_text
      = FragmentProgramManager::use_instance ().compile (fragment_program_proc);
   
#if 1
   eng_TRACE1 ("%1%", program_text.c_str ());
#endif
   
   glBindProgramARB (GL_FRAGMENT_PROGRAM_ARB, program_id);
   
   set_program (program_text);
}



/*
==============================================================================
Name : debug_fill
==============================================================================
*/

void  OpenGLEngine::debug_fill (const opa::Rect & rect)
{
   if (_context_id == 0)
   {
      opa::Rect trect = rect;
      trect -= _cur_translation;
      
      trect.round ();
      
      glDisable (GL_SCISSOR_TEST);
      
      fill (trect, Color (1.f, 1.f, 0.f, 1.f));
      
      glEnable (GL_SCISSOR_TEST);
   }
}



/*
==============================================================================
Name : debug_flash
==============================================================================
*/

void  OpenGLEngine::debug_flash ()
{
   if (_context_id == 0)
   {
      if (_strategy._use_double_buffer_flag)
      {
         assert (_agl_context != 0);
         
         aglSwapBuffers (_agl_context);
         aglSwapBuffers (_agl_context);
      }
      else
      {
         glFinish ();
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : make_tex_props
==============================================================================
*/

void  OpenGLEngine::make_tex_props (Image & image, GLenum & tex_filter, GLenum & tex_wrap_s, GLenum & tex_wrap_t, const Image::Conf & conf)
{
   GLuint texture_id = 0;
   glGenTextures (1, &texture_id);
   
   Image::Data * data_ref = image._data_ref;
   assert (data_ref != 0);
   
   data_ref->_texture_id = texture_id;
   
   tex_filter = GL_NEAREST;

   if (conf._filter_mode == FilterMode_NEAREST)
   {
      tex_filter = GL_NEAREST;
   }
   else if (conf._filter_mode == FilterMode_LINEAR)
   {
      tex_filter = GL_LINEAR;
   }
   
   tex_wrap_s = GL_CLAMP_TO_EDGE;
   tex_wrap_t = GL_CLAMP_TO_EDGE;
   
   image._sprung_mode_x = conf._sprung_mode_x;
   image._sprung_mode_y = conf._sprung_mode_y;
   
   if (
      (conf._sprung_mode_x == SprungMode_REPEAT)
      || (conf._sprung_mode_y == SprungMode_REPEAT)
      )
   {
      // some warnings for the graphist
      
      if ((conf._sprung_mode_x == SprungMode_REPEAT) && (image._width != image._tex_width))
      {
         if (image._name_0 != 0)
         {
            eng_TRACE1 (
               "Repeat X requested for '%1%', but width is not a power of two",
               image._name_0
            );
         }
         else
         {
            eng_TRACE (
               "Repeat X requested for an unnamed image, but width is not a power of two"
            );
         }
      }
      
      if ((conf._sprung_mode_y == SprungMode_REPEAT) && (image._height != image._tex_height))
      {
         if (image._name_0 != 0)
         {
            eng_TRACE1 (
               "Repeat Y requested for '%1%', but height is not a power of two",
               image._name_0
            );
         }
         else
         {
            eng_TRACE (
               "Repeat Y requested for an unnamed image, but height is not a power of two"
            );
         }
      }
      
      // configuration
      
      if (_strategy._support_npot_repeat_flag)
      {
         data_ref->_tex_target = GL_TEXTURE_2D;
         image._tex_s = 1.f;
         image._tex_t = 1.f;
         image._software_wrap_repeat_x_flag = false;
         image._software_wrap_repeat_y_flag = false;
         tex_wrap_s = (conf._sprung_mode_x == SprungMode_REPEAT) ? GL_REPEAT : GL_CLAMP_TO_EDGE;
         tex_wrap_t = (conf._sprung_mode_y == SprungMode_REPEAT) ? GL_REPEAT : GL_CLAMP_TO_EDGE;
      }
      else if (_strategy._support_rect_flag)
      {
         data_ref->_tex_target = GL_TEXTURE_RECTANGLE_ARB;
         image._tex_s = image._width;
         image._tex_t = image._height;
         image._software_wrap_repeat_x_flag = true;
         image._software_wrap_repeat_y_flag = true;
         tex_wrap_s = GL_CLAMP_TO_EDGE;
         tex_wrap_t = GL_CLAMP_TO_EDGE;
      }
      else  // pot / npot with no repeat
      {
         data_ref->_tex_target = GL_TEXTURE_2D;
         
         if (
            (conf._sprung_mode_x == SprungMode_REPEAT)
            && (image._width == image._tex_width)
            )
         {
            image._tex_s = 1.f;
            tex_wrap_s = GL_REPEAT;
            image._software_wrap_repeat_x_flag = false;
         }
         else
         {
            image._tex_s = float (image._width) / float (image._tex_width);
            tex_wrap_s = GL_CLAMP_TO_EDGE;
            image._software_wrap_repeat_x_flag = true;
         }
         
         if (
            (conf._sprung_mode_y == SprungMode_REPEAT)
            && (image._height == image._tex_height)
            )
         {  
            image._tex_t = 1.f;
            tex_wrap_t = GL_REPEAT;
            image._software_wrap_repeat_y_flag = false;
         }
         else
         {
            image._tex_t = float (image._height) / float (image._tex_height);
            tex_wrap_t = GL_CLAMP_TO_EDGE;
            image._software_wrap_repeat_y_flag = true;
         }
      }
   }
   else
   {
      tex_wrap_s = GL_CLAMP_TO_EDGE;
      tex_wrap_t = GL_CLAMP_TO_EDGE;
      image._software_wrap_repeat_x_flag = false;
      image._software_wrap_repeat_y_flag = false;
      
      // prefer NPOT over RECT
      
      if (_strategy._support_npot_flag)
      {
         data_ref->_tex_target = GL_TEXTURE_2D;
         image._tex_s = 1.f;
         image._tex_t = 1.f;
      }
      else if (_strategy._support_rect_flag)
      {
         data_ref->_tex_target = GL_TEXTURE_RECTANGLE_ARB;
         image._tex_s = image._width;
         image._tex_t = image._height;
      }
      else
      {
         // POT case
         data_ref->_tex_target = GL_TEXTURE_2D;
         image._tex_s = float (image._width) / float (image._tex_width);
         image._tex_t = float (image._height) / float (image._tex_height);
      }
   }
}



/*
==============================================================================
Name : get_next_pot
==============================================================================
*/

size_t   OpenGLEngine::get_next_pot (size_t x)
{
   ohm_lang_CHECK_CST (size_t_is_32_bits, sizeof (size_t) == 4);
   
   --x;
   x = (x >> 1) | x;
   x = (x >> 2) | x;
   x = (x >> 4) | x;
   x = (x >> 8) | x;
   x = (x >> 16) | x;
   ++x;
   
   return x;
}



/*
==============================================================================
Name : build_strategy
Description :
   Check the capacilities of the OpenGL Driver to issue different strategies
   dependant on the hardware we are running to.
==============================================================================
*/

void  OpenGLEngine::build_strategy ()
{
   const char * version_0 = (const char *) glGetString (GL_VERSION);
   assert (version_0 != 0);
      
   const char * extensions_0 = (const char *) glGetString (GL_EXTENSIONS);
   assert (extensions_0 != 0);

   std::string extension (extensions_0);
   
   const char * renderer_0 = (const char *) glGetString (GL_RENDERER);
   assert (renderer_0 != 0);
   
   std::string renderer (renderer_0);
   
   const char * vendor_0 = (const char *) glGetString (GL_VENDOR);
   assert (vendor_0 != 0);
   
   // texture strategy : based on advertised extensions & GL version
   {
      // fall back case : always support POT
      
      _strategy._support_npot_flag = false;
      _strategy._support_rect_flag = false;
      _strategy._support_npot_repeat_flag = false;
      
      bool gl_2_flag = (version_0 [0] == '2');
      // for now we don't test GL3 but we probably should ?
      
      if (gl_2_flag)
      {
         // GL_ARB_texture_non_power_of_two is in GL2.0 Core
         
         _strategy._support_npot_flag = true;
         _strategy._support_npot_repeat_flag = true;
      }
      else
      {
         if (extension.find ("GL_ARB_texture_non_power_of_two") != std::string::npos)
         {
            _strategy._support_npot_flag = true;
            _strategy._support_npot_repeat_flag = true;
         }
         
         if (extension.find ("GL_ARB_texture_rectangle") != std::string::npos)
         {
            _strategy._support_rect_flag = true;
         }
      }
   }
   
   // texture strategy : specific ATI problems
   if (renderer.find ("ATI") == 0)
   {
      size_t num_start = renderer.find_first_of ("01234566789");
      size_t num_stop = renderer.find_first_not_of ("01234566789", num_start);
      
      std::string numbers;
         
      if (num_start != std::string::npos)
      {
         if (num_stop != std::string::npos)
         {
            numbers = renderer.substr (num_start, num_stop - num_start);
         }
         else
         {
            numbers = renderer.substr (num_start);
         }
      }
      else
      {
         // huh. ignore.
         ohm_util_TRACE_WARNING1 ("'%1%' : problem detecting card serie", renderer.c_str ());
      }
      
      std::set <std::string> npot_repeat_pb_set;
      npot_repeat_pb_set.insert ("1800");
      npot_repeat_pb_set.insert ("1900");
      npot_repeat_pb_set.insert ("1600");
      npot_repeat_pb_set.insert ("1300");
      npot_repeat_pb_set.insert ("1000");
      npot_repeat_pb_set.insert ("9600");
      npot_repeat_pb_set.insert ("9550");
      npot_repeat_pb_set.insert ("9700");
      npot_repeat_pb_set.insert ("9800");
      npot_repeat_pb_set.insert ("600");
      npot_repeat_pb_set.insert ("800");
      
      if (npot_repeat_pb_set.find (numbers) != npot_repeat_pb_set.end ())
      {
         // driver advertised that NPOT mode is supported, but actually
         // GL_REPEAT is not supported with NPOT textures. This even
         // make the FBO completeness test to fail.
         
         _strategy._support_npot_repeat_flag = false;
      }
   }
   
   // texture strategy : test fallback
   {
#if 0
      _strategy._support_npot_flag = false;
      _strategy._support_rect_flag = false;
      _strategy._support_npot_repeat_flag = false;
#endif
   }
   
   // sanity checks
   if (_strategy._support_npot_repeat_flag)
   {
      assert (_strategy._support_npot_flag);
   }

   _strategy._use_double_buffer_flag = false;
   
   ohm_util_TRACE_INFO ("OpenGL strategy :");
   
   if (_strategy._support_npot_flag)
      ohm_util_TRACE_INFO ("  Support NPOT");

   if (_strategy._support_rect_flag)
      ohm_util_TRACE_INFO ("  Support RECT");

   if (_strategy._support_npot_repeat_flag)
      ohm_util_TRACE_INFO ("  Support NPOT for REPEAT");
   
   
   // memory
   
   {
      AGLRendererInfo info_ptr = aglQueryRendererInfoForCGDirectDisplayIDs (0, 0);
      
      GLint video_memory_size = 0;
      bool ok_flag = aglDescribeRenderer (info_ptr, AGL_VIDEO_MEMORY, &video_memory_size);
      if (ok_flag)
      {
         _video_memory_size = size_t (video_memory_size);
      }
      
      aglDestroyRendererInfo (info_ptr);
      info_ptr = 0;
   }
}



/*
==============================================================================
Name : set_blend_mode_normal
==============================================================================
*/

void  OpenGLEngine::set_blend_mode_normal ()
{
   if (_cur_blend_equation != BlendEquation_ADD)
   {
      eng_TRACE ("glBlendEquation GL_FUNC_ADD");
      glBlendEquation (GL_FUNC_ADD);

      _cur_blend_equation = BlendEquation_ADD;
   }
   
   if (_cur_blend_func != BlendFunc_ONE_ONE_MINUS_SRC_ALPHA)
   {
      eng_TRACE ("glBlendFunc GL_ONE GL_ONE_MINUS_SRC_ALPHA");
      glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

      _cur_blend_func = BlendFunc_ONE_ONE_MINUS_SRC_ALPHA;
   }
}



/*
==============================================================================
Name : set_blend_mode_multiply
==============================================================================
*/

void  OpenGLEngine::set_blend_mode_multiply ()
{
   if (_cur_blend_equation != BlendEquation_ADD)
   {
      eng_TRACE ("glBlendEquation GL_FUNC_ADD");
      glBlendEquation (GL_FUNC_ADD);

      _cur_blend_equation = BlendEquation_ADD;
   }
   
   if (_cur_blend_func != BlendFunc_DST_COLOR_ONE_MINUS_SRC_ALPHA)
   {
      eng_TRACE ("glBlendFunc GL_DST_COLOR GL_ONE_MINUS_SRC_ALPHA");
      glBlendFunc (GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);

      _cur_blend_func = BlendFunc_DST_COLOR_ONE_MINUS_SRC_ALPHA;
   }
}



/*
==============================================================================
Name : set_blend_mode_screen
==============================================================================
*/

void  OpenGLEngine::set_blend_mode_screen ()
{
   if (_cur_blend_equation != BlendEquation_ADD)
   {
      eng_TRACE ("glBlendEquation GL_FUNC_ADD");
      glBlendEquation (GL_FUNC_ADD);

      _cur_blend_equation = BlendEquation_ADD;
   }
   
   if (_cur_blend_func != BlendFunc_ONE_MINUS_DST_COLOR_ONE)
   {
      eng_TRACE ("glBlendFunc GL_ONE_MINUS_DST_COLOR GL_ONE");
      glBlendFunc (GL_ONE_MINUS_DST_COLOR, GL_ONE);

      _cur_blend_func = BlendFunc_ONE_MINUS_DST_COLOR_ONE;
   }
}



/*
==============================================================================
Name : set_blend_mode_replace
==============================================================================
*/

void  OpenGLEngine::set_blend_mode_replace ()
{
   if (_cur_blend_equation != BlendEquation_ADD)
   {
      eng_TRACE ("glBlendEquation GL_FUNC_ADD");
      glBlendEquation (GL_FUNC_ADD);

      _cur_blend_equation = BlendEquation_ADD;
   }
   
   if (_cur_blend_func != BlendFunc_ONE_ZERO)
   {
      eng_TRACE ("glBlendFunc GL_ONE GL_ZERO");
      glBlendFunc (GL_ONE, GL_ZERO);

      _cur_blend_func = BlendFunc_ONE_ZERO;
   }
}



/*
==============================================================================
Name : set_blend_mode_add
==============================================================================
*/

void  OpenGLEngine::set_blend_mode_add ()
{
   if (_cur_blend_equation != BlendEquation_ADD)
   {
      eng_TRACE ("glBlendEquation GL_FUNC_ADD");
      glBlendEquation (GL_FUNC_ADD);

      _cur_blend_equation = BlendEquation_ADD;
   }
   
   if (_cur_blend_func != BlendFunc_ONE_ONE)
   {
      eng_TRACE ("glBlendFunc GL_ONE GL_ONE");
      glBlendFunc (GL_ONE, GL_ONE);

      _cur_blend_func = BlendFunc_ONE_ONE;
   }
}



/*
==============================================================================
Name : set_blend_mode_max
==============================================================================
*/

void  OpenGLEngine::set_blend_mode_max ()
{
   if (_cur_blend_equation != BlendEquation_MAX)
   {
      eng_TRACE ("glBlendEquation GL_MAX");
      glBlendEquation (GL_MAX);

      _cur_blend_equation = BlendEquation_MAX;
   }
   
   // glBlendFunc is ignored
}



/*
==============================================================================
Name : set_blend_mode_min
==============================================================================
*/

void  OpenGLEngine::set_blend_mode_min ()
{
   if (_cur_blend_equation != BlendEquation_MIN)
   {
      eng_TRACE ("glBlendEquation GL_MIN");
      glBlendEquation (GL_MIN);

      _cur_blend_equation = BlendEquation_MIN;
   }
   
   // glBlendFunc is ignored
}



/*
==============================================================================
Name : set_program
==============================================================================
*/

void  OpenGLEngine::set_program (const std::string & program_text)
{
   glProgramStringARB (
      GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB,
      program_text.size (), program_text.c_str ()
   );
   
   GLint ok_flag = 0;
   glGetProgramivARB (
      GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_UNDER_NATIVE_LIMITS_ARB,
      &ok_flag
   );
   
   if (ok_flag != 1)
   {
      GLint cur, max;
      glGetProgramivARB (
         GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_ALU_INSTRUCTIONS_ARB, &cur
      );
      
      glGetProgramivARB (
         GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_ALU_INSTRUCTIONS_ARB, &max
      );

      if (cur > max)
      {
         ohm_util_TRACE_ERROR2 ("Too many ALU instructions %1% max %2%", cur, max);
      }
   }

   if (ok_flag != 1)
   {
      GLint cur, max;
      glGetProgramivARB (
         GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_TEX_INSTRUCTIONS_ARB, &cur
      );
      
      glGetProgramivARB (
         GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_TEX_INSTRUCTIONS_ARB, &max
      );

      if (cur > max)
      {
         ohm_util_TRACE_ERROR2 ("Too many texture instructions %1% max %2%", cur, max);
      }
   }

   if (ok_flag != 1)
   {
      GLint cur, max;
      glGetProgramivARB (
         GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_TEX_INDIRECTIONS_ARB, &cur
      );
      
      glGetProgramivARB (
         GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_TEX_INDIRECTIONS_ARB, &max
      );

      if (cur > max)
      {
         ohm_util_TRACE_ERROR2 ("Too many texture indirections %1% max %2%", cur, max);
      }
   }

   if (ok_flag != 1)
   {
      GLint cur, max;
      glGetProgramivARB (
         GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB, &cur
      );
      
      glGetProgramivARB (
         GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB, &max
      );

      if (cur > max)
      {
         ohm_util_TRACE_ERROR2 ("Too many native texture indirections %1% max %2%", cur, max);
      }
   }

   if (ok_flag != 1)
   {
      GLint cur, max;
      glGetProgramivARB (
         GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB, &cur
      );
      
      glGetProgramivARB (
         GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB, &max
      );

      if (cur > max)
      {
         ohm_util_TRACE_ERROR2 ("Too many native ALU instructions %1% max %2%", cur, max);
      }
   }
   
   assert (ok_flag == 1);
   
   GLint error_line_nbr = 0;
   
   glGetIntegerv (GL_PROGRAM_ERROR_POSITION_ARB, &error_line_nbr);
   
   if (error_line_nbr != -1)
   {
      ohm_util_TRACE_ERROR1 ("%1%", program_text.c_str ());
      
      if (error_line_nbr != 0)
      {
         ohm_util_TRACE_ERROR1 ("at line %1%:", error_line_nbr);
      }
      
      const GLubyte * err_0 = glGetString (GL_PROGRAM_ERROR_STRING_ARB);
      
      ohm_util_TRACE_ERROR1 ("%1%", (char *) err_0);
   }
}



#endif   // (ohm_opal_API == ohm_opal_API_OPENGL)


/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
