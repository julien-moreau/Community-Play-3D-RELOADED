/*****************************************************************************

        WidgetList.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 53326

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



#if ! defined (ohm_opa_WidgetList_HEADER_INCLUDED)
#define  ohm_opa_WidgetList_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/base/WidgetListBase.h"
#include "ohm/opa/base/WidgetListItem.h"
#include "ohm/opa/prim/LayerWidget.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opak/RectFilled.h"
#include "ohm/opak/Layer.h"

#include <list>



namespace ohm
{
namespace opa
{



class WidgetList;
typedef mem::SharedPtr <WidgetList> WidgetListSPtr;

class WidgetList
:  public WidgetListBase
,  public WidgetCreationHelper <WidgetList>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~WidgetList ();
   
   WidgetListItemSPtr
                  push_item ();

   // WidgetBase
   virtual void   invalidate_layout ();
   virtual bool   layout (const Size & bounds);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  WidgetList ();

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
   ohm_opa_WidgetCreationHelper_INVITE (WidgetList);
   
   void           update_metrics ();
   
   opal::AutoFramePtr <opal::Frame>
                  _frame_aptr;
   opak::AutoPrimPtr <LayerWidget>
                  _layer_aptr;
   opak::AutoPrimPtr <opak::RectFilled>
                  _prim_aptr;

   bool           _metrics_need_update_flag;
   Rect           _min_bbox;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetList (const WidgetList &other);
   WidgetList &   operator = (const WidgetList &other);
   bool           operator == (const WidgetList &other);
   bool           operator != (const WidgetList &other);

}; // class WidgetList



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

//#include "ohm/opa/WidgetList.hpp"



#endif   // ohm_opa_WidgetList_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
