/*****************************************************************************

        WidgetProxy.h
        Copyright (c) 2011 Ohm Force

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



#if ! defined (ohm_opa_WidgetProxy_HEADER_INCLUDED)
#define  ohm_opa_WidgetProxy_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/base/WidgetBase.h"
#include "ohm/opa/base/WidgetMetricAuto.h"
#include "ohm/opal/AutoFramePtr.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opak/Layer.h"



namespace ohm
{
namespace opa
{



class WidgetProxy;
typedef mem::SharedPtr <WidgetProxy>   WidgetProxySPtr;

class WidgetProxy
:  public WidgetBase
,  public WidgetCreationHelper <WidgetProxy>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class Event
   {
   public:

      enum Type
      {
                  Type_CREATE = 0,
                  Type_RELEASE,
      };
      
                  Event (WidgetProxy & widget, WidgetSPtr & ret_widget_sptr) : _widget (widget), _ret_widget_sptr (ret_widget_sptr) {}
      
      WidgetProxy &
                  _widget;
      Type        _type;
      WidgetSPtr &_ret_widget_sptr;
   };
   
   virtual        ~WidgetProxy ();
   
   void           set_width (float width);
   void           set_width_auto_parent (float weight = 1.f);
   float          get_width ();

   void           set_height (float height);
   void           set_height_auto_parent (float weight = 1.f);
   float          get_height ();
   
   template <class T, void (T::*handle_event) (Event &)>
   void           bind (T & receiver);

   // WidgetBase
   virtual void   invalidate_layout ();
   virtual bool   layout (const Size & bounds);
   virtual void   spread_view (const Rect & rect);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  WidgetProxy ();
   
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
   ohm_opa_WidgetCreationHelper_INVITE (WidgetProxy);
   
   void           update_metrics ();
   void           show_actual (bool show_flag);
   
   
   WidgetMetricAuto
                  _width;
   WidgetMetricAuto
                  _height;

   WidgetSPtr     _widget_sptr;
   util::Delegate _delegate;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetProxy (const WidgetProxy &other);
   WidgetProxy &  operator = (const WidgetProxy &other);
   bool           operator == (const WidgetProxy &other);
   bool           operator != (const WidgetProxy &other);

}; // class WidgetProxy



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/WidgetProxy.hpp"



#endif   // ohm_opa_WidgetProxy_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
