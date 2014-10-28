/*****************************************************************************

        FrameComboBox.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 49157

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



#if ! defined (ohm_opa_FrameComboBox_HEADER_INCLUDED)
#define  ohm_opa_FrameComboBox_HEADER_INCLUDED

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



class WidgetComboBox;

class FrameComboBox
:  public opal::Frame
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  FrameComboBox (WidgetComboBox & widget);
   virtual        ~FrameComboBox ();
   
   // delegates
   void           mouse_button_pressed (opal::Event & event);

   

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   WidgetComboBox &
                  _widget;

   opal::EventDispatcherStandard <FrameComboBox>
                  _dispatcher;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  FrameComboBox ();
                  FrameComboBox (const FrameComboBox &other);
   FrameComboBox &operator = (const FrameComboBox &other);
   bool           operator == (const FrameComboBox &other) const;
   bool           operator != (const FrameComboBox &other) const;

}; // class FrameComboBox



}  // namespace opa
}  // namespace ohm



//#include  "ohm/opa/FrameComboBox.hpp"



#endif   // ohm_opa_FrameComboBox_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

