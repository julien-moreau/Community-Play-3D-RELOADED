/*****************************************************************************

        WidgetListItem.h
        Copyright (c) 2010 Ohm Force

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



#if ! defined (ohm_opa_WidgetListItem_HEADER_INCLUDED)
#define  ohm_opa_WidgetListItem_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/base/WidgetBase.h"
#include "ohm/opa/prim/PrimListItem.h"
#include "ohm/opal/base/FrameSignal.h"
#include "ohm/opal/Scripture.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opak/Layer.h"
#include "ohm/util/Delegate.h"

#include <list>



namespace ohm
{
namespace opa
{



class WidgetListItem;
typedef mem::SharedPtr <WidgetListItem>   WidgetListItemSPtr;

typedef std::list <WidgetListItemSPtr> WidgetListItemSPtrList;

class WidgetListItem
:  public WidgetBase
,  public WidgetCreationHelper <WidgetListItem>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class Event
   {
   public:
                  Event (WidgetListItem & widget) : _widget (widget) {}
                  
      WidgetListItem &
                  _widget;
      bool        _toggle_flag;
   };
   
   virtual        ~WidgetListItem ();

   void           set_label (const std::string & label);
   const std::string &
                  get_label () const;
      
   void           select (bool select_flag = true);
   bool           is_selected () const;
   
   template <class T, void (T::*handle_event) (Event &)>
   void           bind (T & receiver);
   
   void           handle_event_default (Event & event);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  WidgetListItem ();
   
   // delegates
   void           req_gesture_signal (opal::FrameSignal::Event & event);
   
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
   ohm_opa_WidgetCreationHelper_INVITE (WidgetListItem);
   
   opal::AutoFramePtr <opal::FrameSignal>
                  _frame_aptr;
   opak::AutoPrimPtr <PrimListItem>
                  _prim_aptr;

   std::string    _label;
   opal::Scripture
                  _scripture;
   float          _width;

   util::Delegate _delegate;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetListItem (const WidgetListItem &other);
   WidgetListItem &  operator = (const WidgetListItem &other);
   bool           operator == (const WidgetListItem &other);
   bool           operator != (const WidgetListItem &other);

}; // class WidgetListItem



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/base/WidgetListItem.hpp"



#endif   // ohm_opa_WidgetListItem_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
