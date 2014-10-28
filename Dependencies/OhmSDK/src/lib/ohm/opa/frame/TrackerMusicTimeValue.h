/*****************************************************************************

        TrackerMusicTimeValue.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 66675

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



#if ! defined (ohm_opa_TrackerMusicTimeValue_HEADER_INCLUDED)
#define  ohm_opa_TrackerMusicTimeValue_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/base/TrackerBase.h"
#include "ohm/archi/types.h"



namespace ohm
{
namespace opa
{



class WidgetMusicTime;
class FrameMusicTime;

class TrackerMusicTimeValue
:  public opal::TrackerBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  TrackerMusicTimeValue (WidgetMusicTime & widget, FrameMusicTime & frame, int field);
   virtual        ~TrackerMusicTimeValue ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   virtual bool   do_start (opal::Event & event);
   virtual void   do_track (opal::Event & event);
   virtual void   do_commit (opal::Event & event);
   virtual void   do_revert ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   WidgetMusicTime &
                  _widget;
   FrameMusicTime &
                  _frame;
   const int      _field;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  TrackerMusicTimeValue ();
                  TrackerMusicTimeValue (const TrackerMusicTimeValue &other);
   TrackerMusicTimeValue &    operator = (const TrackerMusicTimeValue &other);
   bool           operator == (const TrackerMusicTimeValue &other);
   bool           operator != (const TrackerMusicTimeValue &other);

}; // class TrackerMusicTimeValue



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/opa/client/TrackerMusicTimeValue.hpp"



#endif   // ohm_opa_TrackerMusicTimeValue_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
