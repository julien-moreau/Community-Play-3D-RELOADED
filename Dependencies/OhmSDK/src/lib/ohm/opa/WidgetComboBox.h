/*****************************************************************************

        WidgetComboBox.h
        Copyright (c) 2009 Ohm Force

        Author: Charles Francoise <charles.francoise@ohmforce.com>
        Revision: 61006

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



#if ! defined (ohm_opa_WidgetComboBox_HEADER_INCLUDED)
#define  ohm_opa_WidgetComboBox_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/base/WidgetBase.h"
#include "ohm/opa/base/WidgetMetricAuto.h"
#include "ohm/opa/WidgetMenuItem.h"
#include "ohm/opa/WidgetMenuRoot.h"
#include "ohm/opa/WidgetLabel.h"
#include "ohm/opa/frame/FrameComboBox.h"
#include "ohm/opa/prim/PrimComboBox.h"
#include "ohm/opa/prim/LayerWidget.h"
#include "ohm/opal/Scripture.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opal/base/FrameSignal.h"
#include "ohm/util/Delegate.h"

#include <memory>
#include <string>
#include <list>



namespace ohm
{
namespace opa
{




class FrameComboBox;
class WidgetMenuRoot;

class WidgetComboBox;
typedef mem::SharedPtr <WidgetComboBox>   WidgetComboBoxSPtr;

class WidgetComboBox
:  public WidgetBase
,  public WidgetCreationHelper <WidgetComboBox>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   class Event
   {
   public:

      enum Type
      {
                  Type_OPENING = 0,
                  Type_CLOSED,
      };
      
                  Event (WidgetComboBox & widget) : _widget (widget) {}
      
      WidgetComboBox &
                  _widget;
      Type        _type;
      WidgetMenuItem *
                  _menu_item_ptr;
      opa::Point  _position;
   };
   
   virtual        ~WidgetComboBox ();
   
   void           set_width (float width);
   void           set_width_auto_content ();
   void           set_width_auto_parent (float weight = 1.f);
   float          get_width ();

   void           set_label (const std::string & label);
   const std::string &
                  get_label () const;

   WidgetMenuRoot &
                  begin_edit ();
   void           end_edit ();
   
   
   template <class T, void (T::*handle_event) (Event &)>
   void           bind (T & receiver);

   void           shake ();
   
   // called from FrameComboBox
   void           req_open_menu (opal::Event & event);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  WidgetComboBox ();
   
   // WidgetBase
   virtual void   do_get_min_bbox (Rect & min_bbox);
   virtual void   do_get_margin (Margin & margin);
   
   virtual void   do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf);
   virtual void   do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf);
   
   virtual void   do_enable (bool enabled_flag);
   virtual void   do_show (bool show_flag);
   virtual void   do_activate (bool activate_flag);
   
   // opal::FrameSignal delegates

   // opa::WidgetMenuItem delegates
   void           req_item_selected (opa::WidgetMenuItem::Event & event);



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   ohm_opa_WidgetCreationHelper_INVITE (WidgetComboBox);
   
   opal::AutoFramePtr <FrameComboBox>
                  _frame_aptr;
   opak::AutoPrimPtr <LayerWidget>
                  _layer_aptr;
   opak::AutoPrimPtr <PrimComboBox>
                  _prim_aptr;
   WidgetLabelSPtr
                  _widget_label_sptr;
   
   bool           _editing_flag;
   
   WidgetMenuRoot *
                  _menu_root_ptr;
   WidgetMenuRoot::SafeLink
                  _menu_root_safe_link;
   
   util::Delegate _delegate;
   
   static float   _inner_hori_margin;
   
   
   
/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetComboBox (const WidgetComboBox &other);
   WidgetComboBox &  operator = (const WidgetComboBox &other);
   bool           operator == (const WidgetComboBox &other) const;
   bool           operator != (const WidgetComboBox &other) const;

}; // class WidgetComboBox



}  // namespace opa
}  // namespace ohm



#include "ohm/opa/WidgetComboBox.hpp"



#endif   // ohm_opa_WidgetComboBox_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

