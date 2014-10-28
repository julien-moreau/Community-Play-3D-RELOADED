/*****************************************************************************

        LayerAspectBrightnessContrast.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70462

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



#if ! defined (client_LayerAspectBrightnessContrast_HEADER_INCLUDED)
#define  client_LayerAspectBrightnessContrast_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "client/AspectBrightnessContrast.h"

#include "ohm/opak/Layer.h"
#include "ohm/opak/base/TypeFloat.h"



namespace client
{



class LayerAspectBrightnessContrast
:  public ohm::opak::Layer
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
public:
   
                  LayerAspectBrightnessContrast ();
   
   // Video Thread
   virtual void   display (ohm::opak::GraphicContext & context);
   
   ohm::opak::TypeFloat <LayerAspectBrightnessContrast>
                  _brightness;
   ohm::opak::TypeFloat <LayerAspectBrightnessContrast>
                  _contrast;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   virtual        ~LayerAspectBrightnessContrast ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           aspect_changed ();
   
   AspectBrightnessContrast
                  _aspect;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  LayerAspectBrightnessContrast (const LayerAspectBrightnessContrast &other);
   LayerAspectBrightnessContrast &        operator = (const LayerAspectBrightnessContrast &other);
   bool           operator == (const LayerAspectBrightnessContrast &other);
   bool           operator != (const LayerAspectBrightnessContrast &other);

}; // class LayerAspectBrightnessContrast



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "client/LayerAspectBrightnessContrast.hpp"



#endif   // client_LayerAspectBrightnessContrast_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
