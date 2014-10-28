/*****************************************************************************

        WidgetDialog.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 69122

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



#if ! defined (ohm_opa_WidgetDialog_HEADER_INCLUDED)
#define  ohm_opa_WidgetDialog_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/WidgetDialog.h"
#include "ohm/opa/WidgetPushButton.h"
#include "ohm/opa/base/WidgetBase.h"
#include "ohm/opa/base/WidgetEmbedderBase.h"
#include "ohm/opa/prim/PrimDialog.h"
#include "ohm/opa/prim/LayerWidget.h"
#include "ohm/opa/private/FrameDialogBack.h"
#include "ohm/opa/private/FrameDialogBlock.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opak/Layer.h"
#include "ohm/opak/LayerClip.h"
#include "ohm/opak/RectFilled.h"
#include "ohm/opal/base/FrameKey.h"
#include "ohm/util/Delegate.h"

#include <list>



namespace ohm
{
namespace opa
{



class EventBundleView;
class BundleView;
typedef mem::SharedPtr <WidgetDialog>  WidgetDialogSPtr;

class WidgetDialog
:  public virtual WidgetBase
,  public WidgetEmbedderBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class Event
   {
   public:
      
      enum Type
      {
                  Type_CANCEL = 0,
                  Type_OK,
      };

                  Event (WidgetDialog & widget) : _widget (widget) {}
      
      WidgetDialog &
                  _widget;
      Type        _type;
   };
   
   virtual        ~WidgetDialog ();
   
   static WidgetDialogSPtr
                  create (opa::BundleView & view);

   template <class T, void (T::*handle_event) (Event &)>
   void           bind (T & receiver);

   void           set_title (const std::string & title);
   void           add_cancel_ok_buttons (const std::string & cancel_label = "Cancel", const std::string & ok_label = "OK", bool cancel_active_flag = true, bool ok_active_flag = true);
   void           add_ok_button (const std::string & ok_label = "OK", bool ok_active_flag = true);
   
   template <class T>
   mem::SharedPtr <T>
                  set_header ();
   template <class T>
   void           set_header (mem::SharedPtr <T> widget_sptr);
   template <class T>
   mem::SharedPtr <T>
                  set_content ();
   template <class T>
   void           set_content (mem::SharedPtr <T> widget_sptr);
   template <class T>
   mem::SharedPtr <T>
                  set_footer ();
   template <class T>
   void           set_footer (mem::SharedPtr <T> widget_sptr);
   
   void           shake ();
   void           close ();
   
   // WidgetEmbedderBase
   virtual void   update_layout ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  WidgetDialog (opa::BundleView & view);

   // opa::WidgetBase
   virtual void   do_get_min_bbox (Rect & min_bbox);
   virtual void   do_get_margin (Margin & margin);
   
   virtual void   do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf);
   virtual void   do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf);
   
   virtual void   do_enable (bool enabled_flag);
   virtual void   do_show (bool show_flag);
   virtual void   do_activate (bool activate_flag);
   
   // delegates
   void           handle_event_default (Event & event);
   void           handle_key_event (opal::FrameKey::Event & event);
   void           handle_push_button_event (WidgetPushButton::Event & event);
   void           handle_bundle_view_event (opa::EventBundleView & event);



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   enum Slot
   {
                  Slot_HEADER = 0,
                  Slot_CONTENT,
                  Slot_FOOTER,
                  
                  NBR_Slot_TOTAL
   };

   opal::AutoFramePtr <opal::Frame>
                  _frame_aptr;
   opal::AutoFramePtr <FrameDialogBack>
                  _frame_back_aptr;
   opal::AutoFramePtr <FrameDialogBlock>
                  _frame_block_aptr;
   opal::AutoFramePtr <opal::FrameKey>
                  _frame_key_aptr;
   opal::AutoFramePtr <opal::Frame>
                  _frame_dialog_aptr;
   opal::AutoFramePtr <opal::Frame>
                  _frame_header_aptr;
   opal::AutoFramePtr <opal::Frame>
                  _frame_content_aptr;
   opal::AutoFramePtr <opal::Frame>
                  _frame_footer_aptr;

   opak::AutoPrimPtr <opak::Layer>
                  _layer_aptr;
   opak::AutoPrimPtr <opa::PrimDialog>
                  _prim_dialog_aptr;
   opak::AutoPrimPtr <LayerWidget>
                  _layer_header_aptr;
   opak::AutoPrimPtr <opak::Layer>
                  _layer_content_aptr;
   opak::AutoPrimPtr <opak::Layer>
                  _layer_footer_aptr;

   opak::AutoPrimPtr <opak::RectFilled>
                  _test_aptr;
   
   WidgetSPtr     _header_widget_sptr;
   WidgetSPtr     _content_widget_sptr;
   WidgetSPtr     _footer_widget_sptr;

   util::Delegate _delegate;
   
   static const float
                  _header_height;
   static const float
                  _header_padding_x;

   static const float
                  _content_padding_x;
   static const float
                  _content_padding_y;

   static const float
                  _footer_height;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetDialog ();
                  WidgetDialog (const WidgetDialog &other);
   WidgetDialog & operator = (const WidgetDialog &other);
   bool           operator == (const WidgetDialog &other);
   bool           operator != (const WidgetDialog &other);

}; // class WidgetDialog



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/WidgetDialog.hpp"



#endif   // ohm_opa_WidgetDialog_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
