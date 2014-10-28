/*****************************************************************************

        FrameTracker.h
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 58326

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



#if ! defined (ohm_opal_FrameTracker_HEADER_INCLUDED)
#define  ohm_opal_FrameTracker_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/Frame.h"
#include "ohm/opal/base/EventDispatcherStandard.h"
#include "ohm/opal/base/TrackerBase.h"
#include "ohm/util/Delegate.h"



namespace ohm
{
namespace opal
{



class FrameTracker
:  public opal::Frame
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class Event
   {
   public:
      enum Type
      {
                  Type_NONE = -1,
                  
                  Type_START = 0,
                  Type_TRACK,
                  Type_COMMIT,
                  Type_REVERT,
      };
                  Event (FrameTracker & frame);
      
      FrameTracker &
                  _frame;
      opal::Event *
                  _event_ptr;
      Type        _type;
      bool        _ok_flag;   // Type_START
      opa::Point  _start_location;
      opa::Point  _previous_location;
      opa::Point  _location;
      
   private:
                  Event ();
   };
   
                  FrameTracker ();
   virtual        ~FrameTracker ();
   
   template <class T, void (T::*process_event) (Event &)>
   void           bind (T & receiver);

   // delegates
   void           mouse_button_pressed (opal::Event & event);
   void           mouse_button_released (opal::Event & event);
   void           mouse_dragged (opal::Event & event);
   void           key_pressed (opal::Event & event);
   void           key_released (opal::Event & event);

   void           user_input_lost (opal::Event & event);
   
   // opal::Frame
   virtual void   mouse_target_lost (const opal::Event & event);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   class Tracker
   :  public opal::TrackerBase
   {
   public:
                  Tracker (FrameTracker & parent) : opal::TrackerBase (parent), _parent (parent) {}
      virtual     ~Tracker () {}
   
   protected:
      // opal::TrackerBase
      virtual bool
                  do_start (opal::Event & event);
      virtual void
                  do_track (opal::Event & event);
      virtual void
                  do_commit (opal::Event & event);
      virtual void
                  do_revert ();

   private:
      FrameTracker &
                  _parent;
   };
   
   friend class Tracker;
   
   opal::EventDispatcherStandard <FrameTracker>
                  _dispatcher;
   
   Event          _event;
   
   Tracker        _tracker;

   util::Delegate _delegate;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  FrameTracker (const FrameTracker &other);
   FrameTracker & operator = (const FrameTracker &other);
   bool           operator == (const FrameTracker &other);
   bool           operator != (const FrameTracker &other);

}; // class FrameTracker



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/opal/base/FrameTracker.hpp"



#endif   // ohm_opal_FrameTracker_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
