/*****************************************************************************

        MouseFrameRoundTripManager.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 53341

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



#if ! defined (ohm_opal_MouseFrameRoundTripManager_HEADER_INCLUDED)
#define  ohm_opal_MouseFrameRoundTripManager_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/FramePath.h"

#include <vector>
#include <set>



namespace ohm
{
namespace opal
{



class Frame;
class Event;

class MouseFrameRoundTripManager
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  MouseFrameRoundTripManager (Frame & frame);
   virtual        ~MouseFrameRoundTripManager ();
   
   bool           notify_mouse_in (const Event & event);
   bool           notify_mouse_out (const Event & event);
   void           unbind (Frame & parent);
   void           unbind (Frame & parent, Frame & frame);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   class ReverseLess
   {
   public:
      bool        operator () (const FramePath & l_op, const FramePath & r_op) const;
   };

   typedef std::set <FramePath, ReverseLess> FramePathSet;
   
   void           increment (const FramePath & frame_path);
   void           decrement (const FramePath & frame_path);
   
   bool           has (const FramePath & frame_path, Frame & parent, Frame & frame);
   
   Frame &        _frame;
   
   FramePathSet   _frame_path_set;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  MouseFrameRoundTripManager ();
                  MouseFrameRoundTripManager (const MouseFrameRoundTripManager &other);
   MouseFrameRoundTripManager &
                  operator = (const MouseFrameRoundTripManager &other);
   bool           operator == (const MouseFrameRoundTripManager &other);
   bool           operator != (const MouseFrameRoundTripManager &other);

}; // class MouseFrameRoundTripManager



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_MouseFrameRoundTripManager_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
