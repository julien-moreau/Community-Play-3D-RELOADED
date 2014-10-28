/*****************************************************************************

        EventTypeCounter.hpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 48853

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



#if defined (ohm_opal_EventTypeCounter_CURRENT_CODEHEADER)
   #error Recursive inclusion of EventTypeCounter code header.
#endif
#define  ohm_opal_EventTypeCounter_CURRENT_CODEHEADER

#if ! defined (ohm_opal_EventTypeCounter_CODEHEADER_INCLUDED)
#define  ohm_opal_EventTypeCounter_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/Event.h"



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : get_pass_plane
==============================================================================
*/

int   EventTypeCounter::get_pass_plane (Event::Pass pass) const
{
   int pass_plane
      = (pass == Event::Pass_CRUMB)
      ? 1 : 0;
   
   return pass_plane;
}



/*
==============================================================================
Name : get_button_nbr_plane
==============================================================================
*/

int   EventTypeCounter::get_button_nbr_plane (const Event & event) const
{
   int button_nbr_plane;
   
   if (event._type == Event::Type_MOUSE_MOVED)
   {
      button_nbr_plane = event._mouse._pressed_flag ? 1 : 0;
   }
   else
   {
      button_nbr_plane
         = (event._mouse._button_nbr <= 1)
         ? 0 : 1;
   }
   
   return button_nbr_plane;
}



/*
==============================================================================
Name : get_button_nbr_plane
==============================================================================
*/

int   EventTypeCounter::get_button_nbr_plane (int button_nbr) const
{
   int button_nbr_plane
      = (button_nbr <= 1)
      ? 0 : 1;
   
   return button_nbr_plane;
}



/*
==============================================================================
Name : get_click_count_plane
==============================================================================
*/

int   EventTypeCounter::get_click_count_plane (int click_count) const
{
   int click_count_plane
      = (click_count <= 1)
      ? 0 : 1;
   
   return click_count_plane;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opal
}  // namespace ohm



#endif   // ohm_opal_EventTypeCounter_CODEHEADER_INCLUDED

#undef ohm_opal_EventTypeCounter_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
