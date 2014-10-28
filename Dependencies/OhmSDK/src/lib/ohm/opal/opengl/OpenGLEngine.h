/*****************************************************************************

        OpenGLEngine.h
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



#if ! defined (ohm_opal_OpenGLEngine_HEADER_INCLUDED)
#define  ohm_opal_OpenGLEngine_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/private/GraphicContextBase.h"
#include "ohm/opal/private/FragmentProgramProc.h"
#include "ohm/sys/ThreadMgr.h"

#include <AGL/agl.h>


#include <stack>

namespace ohm
{
namespace opal
{



class Bitmap;

class OpenGLEngine
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  OpenGLEngine (AGLContext agl_context);
   virtual        ~OpenGLEngine ();
   
   void           check_thread_id () const;
   size_t         get_video_memory_size () const;
   
   //void            init (bool double_buffer_flag);
   
   void           update_context (GLuint context_id, const opa::Size & size, const GraphicState & graphic_state);
   void           present_context (bool wait_vbr_flag);
   
   void           fill (const opa::Path & path, const Color & color);
   void           fill (const opa::Rect & rect, const Color & color);
   void           fill (const opa::Quad & vertices, const Color & color);

   void           stroke (const opa::Path & path, const Color & color);
   
   void           draw (const Image & image, const opa::Quad & vertices, const opa::Quad & tex_coords, float alpha);
   void           draw (const Image & mask, const opa::Quad & vertices, const opa::Quad & tex_coords, const Color & color);
   
   void           prepare (Image & image, size_t width, size_t height);
   void           create (Image & image, const PixArgb * buf_ptr, const Image::Conf & conf, bool buf_owner_flag);
   void           release (Image & image);
   
   void           create (GLuint & frame_buffer_id, Image & image, const Image::Conf & conf);
   void           release (GLuint & frame_buffer_id);
   
   void           create (GLuint & program_id, FragmentProgramProc fragment_program_proc);
   
   // debug
   void           debug_fill (const opa::Rect & rect);
   void           debug_flash ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum BlendEquation
   {
                  BlendEquation_ADD = 0,
                  BlendEquation_MAX,
                  BlendEquation_MIN,
   };
   
   enum BlendFunc
   {
                  BlendFunc_ONE_ONE_MINUS_SRC_ALPHA = 0,
                  BlendFunc_DST_COLOR_ONE_MINUS_SRC_ALPHA,
                  BlendFunc_ONE_MINUS_DST_COLOR_ONE,
                  BlendFunc_ONE_ZERO,
                  BlendFunc_ONE_ONE,
   };
   
   class Strategy
   {
   public:
      bool        _support_npot_flag;
      bool        _support_rect_flag;
      bool        _support_npot_repeat_flag;
      bool        _use_double_buffer_flag;
   };
   
   void           make_tex_props (Image & image, GLenum & tex_filter, GLenum & tex_wrap_s, GLenum & tex_wrap_t, const Image::Conf & conf);

   static size_t  get_next_pot (size_t x);
   
   void           build_strategy ();
   
   void           set_blend_mode_normal ();
   void           set_blend_mode_multiply ();
   void           set_blend_mode_screen ();
   void           set_blend_mode_replace ();
   void           set_blend_mode_add ();
   void           set_blend_mode_max ();
   void           set_blend_mode_min ();

   void           set_program (const std::string & program_text);

   const sys::ThreadMgr::IdType
                  _thread_id;
   const AGLContext
                  _agl_context;
   
   opa::Size      _cur_view_size;
   GLuint         _context_id;
   
   BlendEquation  _cur_blend_equation;
   BlendFunc      _cur_blend_func;
   
   opa::Point     _cur_translation;
   opa::Rect      _cur_scissor_rect;
   
   Strategy       _strategy;
   
   size_t         _video_memory_size;
   
   bool           _cur_fragment_program_enabled_flag;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  OpenGLEngine ();
                  OpenGLEngine (const OpenGLEngine &other);
   OpenGLEngine &operator = (const OpenGLEngine &other);
   bool           operator == (const OpenGLEngine &other);
   bool           operator != (const OpenGLEngine &other);

}; // class OpenGLEngine



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_OpenGLEngine_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
