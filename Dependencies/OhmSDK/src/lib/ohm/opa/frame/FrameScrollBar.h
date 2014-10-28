/*****************************************************************************

        FrameScrollBar.h
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 69631

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



#if ! defined (ohm_opa_FrameScrollBar_HEADER_INCLUDED)
#define  ohm_opa_FrameScrollBar_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/Frame.h"
#include "ohm/opal/base/TrackerBase.h"
#include "ohm/opal/base/EventDispatcherStandard.h"



namespace ohm
{
namespace opa
{



class WidgetScrollBar;

class FrameScrollBar
:  public opal::Frame
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  FrameScrollBar (WidgetScrollBar & widget);
   virtual        ~FrameScrollBar ();
   
   virtual void   mouse_target_lost (const opal::Event & event);

   void           set_content_length (float length);
   void           set_visible_position (float position);
   void           set_visible_length (float length);
   void           frame_bounds_changed ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   class TrackerValue
   :  public opal::TrackerBase
   {
   public:
                  TrackerValue (FrameScrollBar & frame);
      virtual     ~TrackerValue () {}
      
   protected:
   
      virtual bool   
                  do_start (opal::Event & event);
      virtual void   
                  do_track (opal::Event & event);
      virtual void
                  do_commit (opal::Event & event) {}
      virtual void
                  do_revert () {}
   private:
   
      FrameScrollBar &
                  _frame;
                  
   private:
                  TrackerValue ();
   };
   
   // delegates
   void           mouse_button_pressed (opal::Event & event);
   void           mouse_button_released (opal::Event & event);
   void           mouse_dragged (opal::Event & event);

   void           user_input_lost (opal::Event & event);
   
   bool           start (opal::Event & event);
   void           track (opal::Event & event);
   void           update ();

   WidgetScrollBar &
                  _widget;
   opal::EventDispatcherStandard <FrameScrollBar>
                  _dispatcher;
   TrackerValue   _tracker;
   
   float          _content_length;
   float          _visible_position;
   float          _visible_length;
   
   float          _handle_position;
   float          _handle_length;
   
   float          _click_position;


   
/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  FrameScrollBar ();
                  FrameScrollBar (const FrameScrollBar &other);
   FrameScrollBar &  operator = (const FrameScrollBar &other);
   bool           operator == (const FrameScrollBar &other) const;
   bool           operator != (const FrameScrollBar &other) const;

}; // class FrameScrollBar



}  // namespace opa
}  // namespace ohm



//#include  "ohm/opa/FrameScrollBar.hpp"



#endif   // ohm_opa_FrameScrollBar_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

