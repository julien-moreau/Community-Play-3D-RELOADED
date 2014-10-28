/*****************************************************************************

        TimerManager.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 45047

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



#if ! defined (ohm_opal_TimerManager_HEADER_INCLUDED)
#define  ohm_opal_TimerManager_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/util/Delegate.h"

#include <set>
#include <map>
#include <list>



namespace ohm
{
namespace opal
{



class Event;

class TimerManager
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  TimerManager ();
   virtual        ~TimerManager ();

   void           install_regular_timer (util::Delegate & delegate, double time);
   void           install_one_shot_timer (util::Delegate & delegate, double delay, double time);
   void           remove_timer (util::Delegate & delegate);
   void           remove_timers (void * object_ptr);
   
   void           tick (Event & event);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   class Info
   {
   public:
      double      _start_time;
      double      _delay;
   };
   
   typedef std::set <util::Delegate> TimerSet;
   typedef std::map <util::Delegate, Info> TimerInfoSet;
   typedef std::list <std::pair <util::Delegate, Info> > TimerList;
   
   bool           _notifying_flag;
   
   TimerInfoSet   _timer_set;
   TimerList      _latent_insertion_timer_list;
   TimerSet       _latent_removal_timer_set;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  TimerManager (const TimerManager &other);
   TimerManager & operator = (const TimerManager &other);
   bool           operator == (const TimerManager &other);
   bool           operator != (const TimerManager &other);

}; // class TimerManager



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_TimerManager_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
