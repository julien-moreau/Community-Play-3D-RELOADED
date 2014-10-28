/*****************************************************************************

        WidgetScrollBar.h
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 57529

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



#if ! defined (ohm_opa_WidgetScrollBar_HEADER_INCLUDED)
#define  ohm_opa_WidgetScrollBar_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/mem/SharedPtr.h"
#include "ohm/opa/base/WidgetBase.h"
#include "ohm/opa/base/WidgetMetricAuto.h"
#include "ohm/opa/frame/FrameScrollBar.h"
#include "ohm/opa/prim/PrimScrollBar.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opak/Layer.h"
#include "ohm/util/Delegate.h"



namespace ohm
{
namespace opa
{



class WidgetScroll;
class WidgetScrollBar;
typedef mem::SharedPtr <WidgetScrollBar>  WidgetScrollBarSPtr;

class WidgetScrollBar
:  public WidgetBase
,  public WidgetCreationHelper <WidgetScrollBar>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum Type
   {
                  Type_VERTICAL = 0,
                  Type_HORIZONTAL
   };
   
   virtual        ~WidgetScrollBar ();

   void           bind (WidgetScroll & widget);
   
   void           set_type (Type type);
   bool           is_horizontal () const;

   void           set_length (float length);
   void           set_length_auto_parent (float weight = 1.f);
   float          get_length ();
   bool           is_length_auto () const;
   
   void           set_content_length (float length);
   void           set_visible_position (float position);
   void           set_visible_length (float length);

   // internal 
   void           req_set_position (float position);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

                  WidgetScrollBar ();

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

   ohm_opa_WidgetCreationHelper_INVITE (WidgetScrollBar);
   
   void           update_metrics ();

   
   opal::AutoFramePtr <FrameScrollBar>
                  _frame_aptr;
   opak::AutoPrimPtr <PrimScrollBar>
                  _prim_aptr;
   
   Type           _type;
   WidgetMetricAuto
                  _length;
   WidgetScroll * _listener_ptr;

   bool           _activated_flag;
   
   

/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetScrollBar (const WidgetScrollBar &other);
   WidgetScrollBar & operator = (const WidgetScrollBar &other);
   bool           operator == (const WidgetScrollBar &other) const;
   bool           operator != (const WidgetScrollBar &other) const;

}; // class WidgetScrollBar




}  // namespace opa
}  // namespace ohm



//#include  "ohm/opa/WidgetScrollBar.hpp"



#endif   // ohm_opa_WidgetScrollBar_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

