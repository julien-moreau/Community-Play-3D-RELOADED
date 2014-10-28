/*****************************************************************************

        Frame.hpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 65062

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



#if defined (ohm_opal_Frame_CURRENT_CODEHEADER)
   #error Recursive inclusion of Frame code header.
#endif
#define  ohm_opal_Frame_CURRENT_CODEHEADER

#if ! defined (ohm_opal_Frame_CODEHEADER_INCLUDED)
#define  ohm_opal_Frame_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/View.h"

#include <typeinfo>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : install_regular_timer
Description :
   Install a regular timer with id. "Regular" is about 1/60 of a second.
   The timer will fire indefinitely until it is removed or the frame is
   removed from the tree.
   This type of timer is repushed automatically when the frame is put back
   in the tree.
==============================================================================
*/

template <class DerivedType, void (DerivedType::*Method)(Event &)>
void  Frame::install_regular_timer ()
{
   DerivedType * this_ptr = static_cast <DerivedType *> (this);
   
   util::Delegate delegate;
   delegate.param_ref_set <DerivedType, Event, Method> (*this_ptr);

   if (_view_ptr != 0)
   {
      _view_ptr->install_regular_timer (delegate);
   }
   else
   {
      TimerCmd timer_cmd;
      timer_cmd._delegate = delegate;
      timer_cmd._action = TimerCmd::Action_INSTALL_REGULAR_TIMER;
      timer_cmd._delay = -1.0;
      
      _latent_timer_cmd_arr.push_back (timer_cmd);
   }
}



/*
==============================================================================
Name : install_one_shot_timer
Description :
   Install a one shot timer with id that will file in approximatively 'delay'
   seconds. The timer is automatically removed when it fires or the frame is
   removed from the tree (that is there is no path to link root to this frame)
==============================================================================
*/

template <class DerivedType, void (DerivedType::*Method)(Event &)>
void  Frame::install_one_shot_timer (double delay)
{
   DerivedType * this_ptr = static_cast <DerivedType *> (this);
   
   util::Delegate delegate;
   delegate.param_ref_set <DerivedType, Event, Method> (*this_ptr);

   if (_view_ptr != 0)
   {
      _view_ptr->install_one_shot_timer (delegate, delay);
   }
   else
   {
      TimerCmd timer_cmd;
      timer_cmd._delegate = delegate;
      timer_cmd._action = TimerCmd::Action_INSTALL_ONE_SHOT_TIMER;
      timer_cmd._delay = delay;
      
      _latent_timer_cmd_arr.push_back (timer_cmd);
   }
}



/*
==============================================================================
Name : remove_timer
Description :
   Remove timer with associated id. This will stop the regular timer to fire,
   and can be used to prevent a one shot timer to fire.
==============================================================================
*/

template <class DerivedType, void (DerivedType::*Method)(Event &)>
void  Frame::remove_timer ()
{
   DerivedType * this_ptr = static_cast <DerivedType *> (this);
   
   util::Delegate delegate;
   delegate.param_ref_set <DerivedType, Event, Method> (*this_ptr);

   if (_view_ptr != 0)
   {
      _view_ptr->remove_timer (delegate);
   }
   else
   {
      TimerCmd timer_cmd;
      timer_cmd._delegate = delegate;
      timer_cmd._action = TimerCmd::Action_REMOVE_TIMER;
      timer_cmd._delay = -1.0;
      
      _latent_timer_cmd_arr.push_back (timer_cmd);
   }
}



/*
==============================================================================
Name : post_conduct_changed_event
Description :
   Post a conduct changed event
==============================================================================
*/

template <class T>
void  Frame::post_conduct_changed_event (T & conduct, Event::Direction direction)
{
   if (_view_ptr != 0)
   {
      ConductChange conduct_change;
      conduct_change.set <T> ();
      
      _view_ptr->post_conduct_changed_event (conduct_change, direction);
   }
}



/*
==============================================================================
Name : post
Description :
   Post a custom event
==============================================================================
*/

template <class T>
void  Frame::post (const T & event, Event::Direction direction)
{
   Custom custom;
   custom.set (event);
   
   if (_view_ptr != 0)
   {
      _view_ptr->post (custom, direction);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name :
==============================================================================
*/



}  // namespace opal
}  // namespace ohm



#endif   // ohm_opal_Frame_CODEHEADER_INCLUDED

#undef ohm_opal_Frame_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
