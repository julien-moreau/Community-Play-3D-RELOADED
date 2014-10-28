/*****************************************************************************

        ViewImplOpenGL.h
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59363

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



#if ! defined (ohm_opal_ViewImplOpenGL_HEADER_INCLUDED)
#define  ohm_opal_ViewImplOpenGL_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/def.h"
#include "ohm/opa/Rect.h"

#include <vector>

#include <AGL/agl.h>



namespace ohm
{
namespace opal
{



class Window;
class ViewImpl;
class OpenGLContext;
class OpenGLEngine;

class ViewImplOpenGL
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  ViewImplOpenGL (ViewImpl & view_impl);
   virtual        ~ViewImplOpenGL ();
   
   void           vt_init ();
   void           vt_restore ();
   void           vt_update (const opa::Size & view_size);
   void           vt_draw (bool wait_vbr_flag);

   void           invalidate (const opa::Rect & rect);
   
   void           debug_toggle_show_invalid_rect ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum EventType
   {
                  EventType_UPDATE_VIEW_SIZE = 0,
                  // opa::Size : view size
   };
   
   
   void           check_agl_error ();
   void           check_gl_error ();

   ViewImpl &     _view_impl;
   
   // Video Thread
   AGLContext     _agl_context;
   OpenGLEngine * _engine_ptr;
   OpenGLContext *_context_ptr;
   opa::Size      _view_size;

   std::vector <opa::Rect>
                  _invalid_rect_arr;

   bool           _show_invalid_rect_flag;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ViewImplOpenGL (const ViewImplOpenGL &other);
   ViewImplOpenGL &     operator = (const ViewImplOpenGL &other);
   bool           operator == (const ViewImplOpenGL &other);
   bool           operator != (const ViewImplOpenGL &other);

}; // class ViewImplOpenGL



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_ViewImplOpenGL_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
