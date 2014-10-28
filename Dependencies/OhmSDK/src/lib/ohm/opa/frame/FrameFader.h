/*****************************************************************************

        FrameFader.h
        Copyright (c) 2010 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 57306

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



#if ! defined (ohm_opa_FrameFader_HEADER_INCLUDED)
#define  ohm_opa_FrameFader_HEADER_INCLUDED

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



class WidgetFader;

class FrameFader
:  public opal::Frame
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  FrameFader (WidgetFader & widget);
   virtual        ~FrameFader ();
   
   void           set_horizontal (bool flag);
   
   virtual void   mouse_target_lost (const opal::Event & event);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   class TrackerValue : public opal::TrackerBase
   {
   public:
                  TrackerValue (FrameFader & frame, WidgetFader & bundle);
      virtual     ~TrackerValue () {}
      
      bool        _horizontal_flag;
      float       _len_inv;      // inverse of the fader course in pixels  
      
   protected:
   
      virtual bool   
                  do_start (opal::Event & event);
      virtual void   
                  do_track (opal::Event & event);
      virtual void
                  do_commit (opal::Event & event);
      virtual void
                  do_revert ();
   private:
   
      float       norm_value_for_event (opal::Event & event) const;
      
      WidgetFader &
                  _bundle;
      FrameFader &
                  _frame;
                  
   private:
                  TrackerValue ();
   };
   
   // delegates
   void           mouse_button_pressed (opal::Event & event);
   void           mouse_button_released (opal::Event & event);
   void           mouse_dragged (opal::Event & event);

   void           mouse_button_pressed_twice (opal::Event & event);
   void           mouse_button_released_twice (opal::Event & event);
   
   bool           is_in (opal::Event & event);
   
   

   WidgetFader &  _widget;
   opal::EventDispatcherStandard <FrameFader>
                  _dispatcher;
   TrackerValue   _tracker;


   
/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  FrameFader ();
                  FrameFader (const FrameFader &other);
   FrameFader &   operator = (const FrameFader &other);
   bool           operator == (const FrameFader &other) const;
   bool           operator != (const FrameFader &other) const;

}; // class FrameFader



}  // namespace opa
}  // namespace ohm



//#include  "ohm/opa/FrameFader.hpp"



#endif   // ohm_opa_FrameFader_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

