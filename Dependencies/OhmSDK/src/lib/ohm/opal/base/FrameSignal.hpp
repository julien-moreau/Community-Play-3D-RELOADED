/*****************************************************************************

        FrameSignal.hpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 58011

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



#if defined (ohm_opal_FrameSignal_CURRENT_CODEHEADER)
   #error Recursive inclusion of FrameSignal code header.
#endif
#define  ohm_opal_FrameSignal_CURRENT_CODEHEADER

#if ! defined (ohm_opal_FrameSignal_CODEHEADER_INCLUDED)
#define  ohm_opal_FrameSignal_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : bind_rollover
==============================================================================
*/

template <class T, void (T::*rollover_begin) (FrameSignal::Event &), void (T::*rollover_end) (FrameSignal::Event &)>
void  FrameSignal::bind_rollover (T & receiver)
{
   _rollover_begin.param_ref_set <T, FrameSignal::Event, rollover_begin> (receiver);
   _rollover_end.param_ref_set <T, FrameSignal::Event, rollover_end> (receiver);
}



/*
==============================================================================
Name : bind_gesture
==============================================================================
*/

template <class T, void (T::*gesture_begin) (FrameSignal::Event &), void (T::*gesture_end) (FrameSignal::Event &)>
void  FrameSignal::bind_gesture (T & receiver)
{
   _gesture_begin.param_ref_set <T, FrameSignal::Event, gesture_begin> (receiver);
   _gesture_end.param_ref_set <T, FrameSignal::Event, gesture_end> (receiver);
}



/*
==============================================================================
Name : bind_gesture_trip
==============================================================================
*/

template <class T, void (T::*gesture_entered) (FrameSignal::Event &), void (T::*gesture_exited) (FrameSignal::Event &)>
void  FrameSignal::bind_gesture_trip (T & receiver)
{
   _gesture_entered.param_ref_set <T, FrameSignal::Event, gesture_entered> (receiver);
   _gesture_exited.param_ref_set <T, FrameSignal::Event, gesture_exited> (receiver);
}



/*
==============================================================================
Name : bind_gesture_signal
==============================================================================
*/

template <class T, void (T::*gesture_signal) (FrameSignal::Event &)>
void  FrameSignal::bind_gesture_signal (T & receiver)
{
   _gesture_signal.param_ref_set <T, FrameSignal::Event, gesture_signal> (receiver);
}



/*
==============================================================================
Name : bind_gesture_timer
==============================================================================
*/

template <class T, void (T::*gesture_timer) (FrameSignal::Event &)>
void  FrameSignal::bind_gesture_timer (T & receiver)
{
   install_regular_timer <FrameSignal, &FrameSignal::timer_fired> ();
   
   _gesture_timer.param_ref_set <T, FrameSignal::Event, gesture_timer> (receiver);
}



/*
==============================================================================
Name : bind_hit_test
==============================================================================
*/

template <class T, void (T::*hit_test) (FrameSignal::Event &)>
void  FrameSignal::bind_hit_test (T & receiver)
{
   _hit_test.param_ref_set <T, FrameSignal::Event, hit_test> (receiver);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opal
}  // namespace ohm



#endif   // ohm_opal_FrameSignal_CODEHEADER_INCLUDED

#undef ohm_opal_FrameSignal_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
