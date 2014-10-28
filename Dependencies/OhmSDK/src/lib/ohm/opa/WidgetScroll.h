/*****************************************************************************

        WidgetScroll.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 58576

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



#if ! defined (ohm_opa_WidgetScroll_HEADER_INCLUDED)
#define  ohm_opa_WidgetScroll_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/base/WidgetBase.h"
#include "ohm/opa/base/WidgetMetricAuto.h"
#include "ohm/opa/private/FrameScroll.h"
#include "ohm/opa/prim/LayerWidget.h"
#include "ohm/opal/AutoFramePtr.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opak/Layer.h"
#include "ohm/opak/LayerClip.h"
#include "ohm/opak/SprungImage.h"

#include <list>



namespace ohm
{
namespace opa
{



class WidgetScrollBar;
class WidgetScroll;
typedef mem::SharedPtr <WidgetScroll>  WidgetScrollSPtr;

class WidgetScroll
:  public WidgetBase
,  public WidgetCreationHelper <WidgetScroll>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum Border
   {
                  Border_LEFT = 0,
                  Border_TOP,
                  Border_RIGHT,
                  Border_BOTTOM,
                  
                  Border_NBR_ELT
   };
   
   virtual        ~WidgetScroll ();
   
   void           bind (WidgetScrollBar & widget);
   
   void           set_width (float width);
   void           set_width_auto_content ();
   void           set_width_auto_parent (float weight = 1.f);
   float          get_width ();

   void           set_height (float height);
   void           set_height_auto_content ();
   void           set_height_auto_parent (float weight = 1.f);
   float          get_height ();
   
   void           set_flush (Border border);
   
   void           enable_clip (Border border, bool enable_flag);
   
   template <class T>
   mem::SharedPtr <T>
                  set ();
   template <class T>
   void           set (mem::SharedPtr <T> widget_sptr);
   void           unset ();
   template <class T>
   T &            use ();
   template <class T>
   T *            use_ptr ();
   bool           empty () const;
   
   void           req_move_content (const opa::Point & delta);
   void           req_set_content (const opa::Point & point);
   opa::Point     get_content_position ();

   // WidgetBase
   virtual void   invalidate_layout ();
   virtual bool   layout (const Size & bounds);
   virtual void   spread_view (const Rect & rect);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  WidgetScroll ();
   
   // WidgetBase
   virtual void   do_get_min_bbox (Rect & min_bbox);
   virtual void   do_get_margin (Margin & margin);
   
   virtual void   do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf);
   virtual void   do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf);
   
   virtual void   do_enable (bool enabled_flag);
   virtual void   do_show (bool show_flag);
   virtual void   do_activate (bool activate_flag);
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   ohm_opa_WidgetCreationHelper_INVITE (WidgetScroll);
   
   void           update_metrics ();
   void           update_content ();
   
   void           update_clip (double tt, opak::TransitionFunction tf);
   void           notify_listener ();
   
   WidgetScrollBar *
                  _listener_ptr;
   
   opal::AutoFramePtr <FrameScroll>
                  _frame_container_aptr;
   opak::AutoPrimPtr <LayerWidget>
                  _layer_aptr;
   opak::AutoPrimPtr <opak::LayerClip>
                  _layer_container_aptr;
   opak::AutoPrimPtr <opak::SprungImage>
                  _prim_shadow_aptr_arr [Border_NBR_ELT];

   opal::AutoFramePtr <opal::Frame>
                  _frame_content_aptr;
   opak::AutoPrimPtr <opak::Layer>
                  _layer_content_aptr;
   
   WidgetMetricAuto
                  _width;
   WidgetMetricAuto
                  _height;

   bool           _flush_right_flag;
   bool           _flush_bottom_flag;
   bool           _enable_clip_flag_arr [Border_NBR_ELT];

   WidgetSPtr     _widget_sptr;
   Size           _content_bounds;
   Rect           _view_rect;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetScroll (const WidgetScroll &other);
   WidgetScroll & operator = (const WidgetScroll &other);
   bool           operator == (const WidgetScroll &other);
   bool           operator != (const WidgetScroll &other);

}; // class WidgetScroll



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/WidgetScroll.hpp"



#endif   // ohm_opa_WidgetScroll_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
