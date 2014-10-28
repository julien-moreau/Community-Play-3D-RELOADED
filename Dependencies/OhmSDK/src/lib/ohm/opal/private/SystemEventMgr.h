/*****************************************************************************

        SystemEventMgr.h
        Copyright (c) 2011 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 67167

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



#if ! defined (ohm_opal_SystemEventMgr_HEADER_INCLUDED)
#define  ohm_opal_SystemEventMgr_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/private/SystemEvent.h"



namespace ohm
{
namespace opal
{


class View;

class SystemEventMgr
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  SystemEventMgr (View & view);
   virtual        ~SystemEventMgr ();
   
   
   bool           process (SystemEvent & event);
   
   

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           handle_button_event (SystemEvent::MouseButton mbut, const SystemEvent & event);
   bool           handle_key_change (const SystemEvent & event);
   bool           handle_key (const SystemEvent & event);
   
   View &         _view;
   SystemEvent    _event;


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  SystemEventMgr (const SystemEventMgr &other);
   SystemEventMgr &  operator = (const SystemEventMgr &other);
   bool           operator == (const SystemEventMgr &other) const;
   bool           operator != (const SystemEventMgr &other) const;

}; // class SystemEventMgr



}  // namespace opal
}  // namespace ohm



//#include "ohm/opal/SystemEventMgr.hpp"



#endif   // ohm_opal_SystemEventMgr_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

