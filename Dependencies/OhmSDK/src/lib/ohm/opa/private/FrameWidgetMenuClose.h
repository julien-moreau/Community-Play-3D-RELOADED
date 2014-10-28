/*****************************************************************************

        FrameWidgetMenuClose.h
        Copyright (c) 2010 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 49822

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



#if ! defined (ohm_opa_FrameWidgetMenuClose_HEADER_INCLUDED)
#define  ohm_opa_FrameWidgetMenuClose_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/Frame.h"
#include "ohm/opal/base/EventDispatcherStandard.h"
#include "ohm/opal/base/EventDispatcherBlock.h"



namespace ohm
{
namespace opa
{



class BundleView;
class WidgetMenuRoot;

class FrameWidgetMenuClose
:  public opal::Frame
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  FrameWidgetMenuClose (BundleView & bundle, WidgetMenuRoot & menu_root);
   virtual        ~FrameWidgetMenuClose ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   // delegates
   void           mouse_button_released (opal::Event & event);
   void           user_input_lost (opal::Event & event);

   BundleView &   _bundle;
   WidgetMenuRoot &
                  _menu_root;
   opal::EventDispatcherStandard <FrameWidgetMenuClose>
                  _dispatcher;
   
   opal::EventDispatcherBlock
                  _dispatcher_block;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  FrameWidgetMenuClose (const FrameWidgetMenuClose &other);
   FrameWidgetMenuClose &  operator = (const FrameWidgetMenuClose &other);
   bool           operator == (const FrameWidgetMenuClose &other) const;
   bool           operator != (const FrameWidgetMenuClose &other) const;

}; // class FrameWidgetMenuClose



}  // namespace opa
}  // namespace ohm



//#include  "ohm/opa/FrameWidgetMenuClose.hpp"



#endif   // ohm_opa_FrameWidgetMenuClose_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

