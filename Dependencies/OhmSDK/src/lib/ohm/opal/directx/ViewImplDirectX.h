/*****************************************************************************

        ViewImplDirectX.h
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59387

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



#if ! defined (ohm_opal_ViewImplDirectX_HEADER_INCLUDED)
#define  ohm_opal_ViewImplDirectX_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/def.h"
#include "ohm/opa/Rect.h"

#include <vector>



namespace ohm
{
namespace opal
{



class Window;
class ViewImpl;
class DirectXContext;
class DirectXEngine;

class ViewImplDirectX
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  ViewImplDirectX (ViewImpl & view_impl);
   virtual        ~ViewImplDirectX ();
   
   void           vt_init ();
   void           vt_restore ();
   void           vt_update (const opa::Size & view_size);
   void           vt_draw (bool wait_vbr_flag);
   
   void           invalidate (const opa::Rect & rect);
   
   void           debug_toggle_show_invalid_rect ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ VIDEO THREAD \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
      
   enum EventType
   {
                  EventType_UPDATE_VIEW_SIZE = 0,
                  // opa::Size : view size
   };
   
   ViewImpl &     _view_impl;
   
   // Video Thread
   DirectXEngine *
                  _engine_ptr;
   DirectXContext *
                  _context_ptr;
   opa::Size      _view_size;

   std::vector <opa::Rect>
                  _invalid_rect_arr;

   bool           _show_invalid_rect_flag;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ViewImplDirectX (const ViewImplDirectX &other);
   ViewImplDirectX &    operator = (const ViewImplDirectX &other);
   bool           operator == (const ViewImplDirectX &other);
   bool           operator != (const ViewImplDirectX &other);

}; // class ViewImplDirectX



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_ViewImplDirectX_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
