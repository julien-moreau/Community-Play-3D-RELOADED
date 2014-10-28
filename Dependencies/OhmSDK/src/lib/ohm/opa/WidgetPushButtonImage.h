/*****************************************************************************

        WidgetPushButtonImage.h
        Copyright (c) 2011 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 59681

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



#if ! defined (ohm_opa_WidgetPushButtonImage_HEADER_INCLUDED)
#define  ohm_opa_WidgetPushButtonImage_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/base/WidgetBase.h"
#include "ohm/opa/base/WidgetMetricAuto.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opak/Image.h"
#include "ohm/opal/base/FrameSignal.h"



namespace ohm
{
namespace opal
{
class Typesetter;
}



namespace opa
{



class WidgetPushButtonImage;
typedef mem::SharedPtr <WidgetPushButtonImage>  WidgetPushButtonImageSPtr;


class WidgetPushButtonImage
:  public WidgetBase
,  public WidgetCreationHelper <WidgetPushButtonImage>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   class Event
   {
   public:
                  Event (WidgetPushButtonImage & widget, opal::Event & event) : _widget (widget), _event (event) {}
                  
      WidgetPushButtonImage &
                  _widget;
      opal::Event &
                  _event;
   };
   
   virtual        ~WidgetPushButtonImage ();
   
   template <class T, void (T::*handle_event) (Event &)>
   void           bind (T & receiver);

   void           set_width (float width);
   void           set_width_auto_content ();
   void           set_width_auto_parent (float weight = 1.f);
   float          get_width ();
   bool           is_width_auto () const;
   
   void           set_height (float height);
   void           set_height_auto_content ();
   void           set_height_auto_parent (float weight = 1.f);
   float          get_height ();
   bool           is_height_auto () const;
   
   void           set_path (const std::string & path);
   const std::string &
                  get_path () const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

                  WidgetPushButtonImage ();

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

   ohm_opa_WidgetCreationHelper_INVITE (WidgetPushButtonImage);
   
   // delegates
   void           handle_gesture_signal (opal::FrameSignal::Event & event);
   
   void           update_metrics ();
   
   opak::AutoPrimPtr <opak::Image>
                  _prim_aptr;
   opal::AutoFramePtr <opal::FrameSignal>
                  _frame_aptr;

   WidgetMetricAuto
                  _width;
   WidgetMetricAuto
                  _height;
   std::string    _path;

   util::Delegate _delegate;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  WidgetPushButtonImage (const WidgetPushButtonImage &other);
   WidgetPushButtonImage & operator = (const WidgetPushButtonImage &other);
   bool           operator == (const WidgetPushButtonImage &other) const;
   bool           operator != (const WidgetPushButtonImage &other) const;

}; // class WidgetPushButtonImage



}  // namespace opa
}  // namespace ohm



#include "ohm/opa/WidgetPushButtonImage.hpp"



#endif   // ohm_opa_WidgetPushButtonImage_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

