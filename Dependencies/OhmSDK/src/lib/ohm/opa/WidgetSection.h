/*****************************************************************************

        WidgetSection.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 60933

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



#if ! defined (ohm_opa_WidgetSection_HEADER_INCLUDED)
#define  ohm_opa_WidgetSection_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/base/WidgetBase.h"
#include "ohm/opa/base/WidgetMetricAuto.h"
#include "ohm/opa/prim/PrimSection.h"
#include "ohm/opa/prim/LayerWidget.h"
#include "ohm/opal/AutoFramePtr.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opak/Layer.h"



namespace ohm
{
namespace opa
{




class WidgetSection;
typedef mem::SharedPtr <WidgetSection> WidgetSectionSPtr;

class WidgetSection
:  public WidgetBase
,  public WidgetCreationHelper <WidgetSection>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~WidgetSection ();
   
   void           set_style_header ();
   void           set_style_content ();
   void           set_style_footer ();
   
   void           set_width (float width);
   void           set_width_auto_content ();
   void           set_width_auto_parent (float weight = 1.f);
   float          get_width ();

   void           set_height (float height);
   void           set_height_auto_content ();
   void           set_height_auto_parent (float weight = 1.f);
   float          get_height ();
   
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
   
   // WidgetBase
   virtual void   invalidate_layout ();
   virtual bool   layout (const Size & bounds);
   virtual void   spread_view (const Rect & rect);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  WidgetSection ();
   
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
   ohm_opa_WidgetCreationHelper_INVITE (WidgetSection);
   
   void           update_metrics ();

   
   opal::AutoFramePtr <opal::Frame>
                  _frame_aptr;
   opal::AutoFramePtr <opal::Frame>
                  _frame_content_aptr;

   opak::AutoPrimPtr <LayerWidget>
                  _layer_widget_aptr;
   opak::AutoPrimPtr <PrimSection>
                  _prim_back_aptr;
   opak::AutoPrimPtr <opak::Layer>
                  _layer_content_aptr;
   
   WidgetMetricAuto
                  _width;
   WidgetMetricAuto
                  _height;
   Margin         _margin;
   
   WidgetSPtr     _widget_sptr;
   
   static float   _padding_x;
   static float   _padding_y;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetSection (const WidgetSection &other);
   WidgetSection &   operator = (const WidgetSection &other);
   bool           operator == (const WidgetSection &other);
   bool           operator != (const WidgetSection &other);

}; // class WidgetSection



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/WidgetSection.hpp"



#endif   // ohm_opa_WidgetSection_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
