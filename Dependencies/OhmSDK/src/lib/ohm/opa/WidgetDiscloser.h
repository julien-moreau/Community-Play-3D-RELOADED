/*****************************************************************************

        WidgetDiscloser.h
        Copyright (c) 2009 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 38611

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



#if ! defined (ohm_opa_WidgetDiscloser_HEADER_INCLUDED)
#define  ohm_opa_WidgetDiscloser_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/base/WidgetBase.h"
#include "ohm/opal/base/FrameSignal.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opak/Layer.h"
#include "ohm/opak/Image.h"
#include "ohm/opak/RectFilled.h"
#include "ohm/mem/SharedPtr.h"


namespace ohm
{
namespace opa
{



class WidgetLabel;
class WidgetDiscloser;
typedef mem::SharedPtr <WidgetDiscloser>  WidgetDiscloserSPtr;

class WidgetDiscloser
:  public WidgetBase
,  public WidgetCreationHelper <WidgetDiscloser>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   virtual        ~WidgetDiscloser ();
   
   void           set_label (const std::string & label);
   
   template <class T>
   mem::SharedPtr <T>
                  set ();
   template <class T>
   void           set (mem::SharedPtr <T> widget_sptr);
   void           unset ();
   template <class T>
   T &            get ();

   // opa::WidgetBase
   virtual bool   layout (const opa::Size & bounds);
   virtual void   get_min_bbox (Rect & min_bbox);
   virtual void   get_margin (Margin & margin);
   virtual void   get_weight (float & weight);
   
   

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  WidgetDiscloser ();

   // opa::WidgetBase
   virtual void   do_set_bounds (const Size & bounds);
   virtual void   do_enable (bool enabled_flag);



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   ohm_opa_WidgetCreationHelper_INVITE (WidgetDiscloser);
   
   enum Slot
   {
      Slot_ZONE = 0,
      Slot_DECORATION,
      
      Slot_NBR_ELT
   };
   
   // opal::FrameSignal delegates
   void           req_gesture_signal (opal::FrameSignal::Event & event);

   void           compute_zone_rect ();
   void           update_size ();

   opal::AutoFramePtr <opal::Frame>
                  _frame_aptr;
   opal::AutoFramePtr <opal::FrameSignal>
                  _frame_toggle_aptr;
   opak::AutoPrimPtr <opak::Layer>
                  _layer_aptr;
   opal::AutoFramePtr <opal::Frame>
                  _zone_frame_aptr;
   opak::AutoPrimPtr <opak::Layer>
                  _zone_layer_aptr;
   opak::AutoPrimPtr <opak::Image>
                  _image_aptr;
   opak::AutoPrimPtr <opak::RectFilled>
                  _bkg_aptr;
   mem::SharedPtr <WidgetLabel>
                  _label_sptr;

   bool           _flag_zone_displayed;
   Rect           _zone_rect;

   WidgetSPtr     _widget_sptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetDiscloser (const WidgetDiscloser &other);
   WidgetDiscloser & operator = (const WidgetDiscloser &other);
   bool           operator == (const WidgetDiscloser &other) const;
   bool           operator != (const WidgetDiscloser &other) const;

}; // class WidgetDiscloser



}  // namespace opa
}  // namespace ohm



#include "ohm/opa/WidgetDiscloser.hpp"



#endif   // ohm_opa_WidgetDiscloser_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

