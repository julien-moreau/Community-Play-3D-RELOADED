/*****************************************************************************

        WidgetStackBase.h
        Copyright (c) 2011 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 59722

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



#if ! defined (ohm_opa_WidgetStackBase_HEADER_INCLUDED)
#define  ohm_opa_WidgetStackBase_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/base/WidgetListBase.h"
#include "ohm/opa/base/WidgetMetricAuto.h"
#include "ohm/opa/base/WidgetOrientableBase.h"
#include "ohm/opa/prim/LayerWidget.h"
#include "ohm/opal/AutoFramePtr.h"
#include "ohm/opal/Frame.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opak/Layer.h"



namespace ohm
{
namespace opa
{



class WidgetStackBase
:  public WidgetListBase
,  public WidgetOrientableBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  WidgetStackBase ();
   virtual        ~WidgetStackBase ();
   
   void           set_width (float width);
   void           set_width_auto_content ();
   void           set_width_auto_parent (float weight = 1.f);
   float          get_width ();

   void           set_height (float height);
   void           set_height_auto_content ();
   void           set_height_auto_parent (float weight = 1.f);
   float          get_height ();
   
   // WidgetBase
   virtual void   invalidate_layout ();
   virtual bool   layout (const Size & bounds);
   virtual void   spread_view (const Rect & rect);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

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

   void           update_metrics ();
   
   opal::AutoFramePtr <opal::Frame>
                  _frame_aptr;

   opak::AutoPrimPtr <LayerWidget>
                  _layer_aptr;
   
   WidgetMetricAuto
                  _width;
   WidgetMetricAuto
                  _height;
   
   Point          _min_bbox_origin;
   Margin         _margin;
   float          _total_sub_weight;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetStackBase (const WidgetStackBase &other);
   WidgetStackBase & operator = (const WidgetStackBase &other);
   bool           operator == (const WidgetStackBase &other);
   bool           operator != (const WidgetStackBase &other);

}; // class WidgetStackBase



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

//#include "ohm/opa/WidgetStackBase.hpp"



#endif   // ohm_opa_WidgetStackBase_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
