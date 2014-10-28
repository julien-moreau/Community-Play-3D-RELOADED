/*****************************************************************************

        Clock.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 31600

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



#if ! defined (ohm_opal_Clock_HEADER_INCLUDED)
#define  ohm_opal_Clock_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/util/SingletonBase.h"

#include <set>
#include <map>
#include <list>



namespace ohm
{
namespace opal
{



class ClockListener;

class Clock
:  public util::SingletonBase <Clock>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   virtual        ~Clock ();
   
   void           add (ClockListener & listener, int stage);
   void           remove (ClockListener & listener);
   
   void           init (double timestamp);
   void           tick (double timestamp);
   
   double         get_time ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

                  Clock ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   ohm_util_SingletonBase_INVITE_SINGLETON (Clock)
   
   typedef std::set <ClockListener *> ClockListenerPtrSet;
   typedef std::map <int, ClockListenerPtrSet> StageClockListenerPtrSetMap;
   typedef std::pair <int, ClockListener *> StageClockListenerPtrPair;
   typedef std::list <ClockListener *> ClockListenerPtrList;
   typedef std::list <StageClockListenerPtrPair> StageClockListenerPtrPairList;
   
   void           init_listeners ();
   void           init_listeners_stage (ClockListenerPtrSet & clock_listener_ptr_set);

   void           notify_listeners ();
   void           notify_listeners_stage (ClockListenerPtrSet & clock_listener_ptr_set);
   void           process_actions ();
   
   bool           is_in_latent_removal_state (ClockListener * listener_ptr);
   
   StageClockListenerPtrSetMap
                  _stage_clock_listener_ptr_set_map;
   
   bool           _inited_flag;
   double         _timestamp;

   bool           _notifying_flag;
   StageClockListenerPtrPairList
                  _latent_insertion_clock_listener_ptr_list;
   ClockListenerPtrList
                  _latent_removal_clock_listener_ptr_list;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Clock (const Clock &other);
   Clock &     operator = (const Clock &other);
   bool           operator == (const Clock &other);
   bool           operator != (const Clock &other);

}; // class Clock



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_Clock_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
