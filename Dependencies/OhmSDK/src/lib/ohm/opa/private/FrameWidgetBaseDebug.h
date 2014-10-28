/*****************************************************************************

        FrameWidgetBaseDebug.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54597

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



#if ! defined (ohm_opa_FrameWidgetBaseDebug_HEADER_INCLUDED)
#define  ohm_opa_FrameWidgetBaseDebug_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/Frame.h"
#include "ohm/opal/base/EventDispatcherStandard.h"



namespace ohm
{
namespace opa
{



class WidgetBase;

class FrameWidgetBaseDebug
:  public opal::Frame
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  FrameWidgetBaseDebug (WidgetBase & widget);
   virtual        ~FrameWidgetBaseDebug ();
   
   // opal::Frame
   virtual void   mouse_entered (opal::Event & event);
   virtual void   mouse_exited (opal::Event & event);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   // delegates
   void           mouse_moved (opal::Event & event);
   
   WidgetBase &   _widget;
   
   opal::EventDispatcherStandard <FrameWidgetBaseDebug>
                  _dispatcher;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  FrameWidgetBaseDebug ();
                  FrameWidgetBaseDebug (const FrameWidgetBaseDebug &other);
   FrameWidgetBaseDebug &  operator = (const FrameWidgetBaseDebug &other);
   bool           operator == (const FrameWidgetBaseDebug &other);
   bool           operator != (const FrameWidgetBaseDebug &other);

}; // class FrameWidgetBaseDebug



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/opa/private/FrameWidgetBaseDebug.hpp"



#endif   // ohm_opa_FrameWidgetBaseDebug_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
