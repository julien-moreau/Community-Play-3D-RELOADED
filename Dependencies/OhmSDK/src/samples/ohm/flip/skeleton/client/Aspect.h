/*****************************************************************************

        Aspect.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70478

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



#if ! defined (client_Aspect_HEADER_INCLUDED)
#define  client_Aspect_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "client/AspectMain.h"

#include "model/Taxi.h"

#include "ohm/opal/Bitmap.h"
#include "ohm/opal/Color.h"
#include "ohm/opal/Image.h"
#include "ohm/opal/SprungImage.h"
#include "ohm/opak/base/AspectBase.h"



namespace client
{



class Aspect
:  public ohm::opak::AspectBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  Aspect ();
   virtual        ~Aspect ();
   
   // opak::AspectBase
   virtual void   make (ohm::opak::GraphicContext & context);
   
   // resources
   
   ohm::opal::Image
                  _back;

   ohm::opal::Image
                  _taxi_shadow;
   ohm::opal::Image
                  _taxi_selection_arr [AspectMain::ColorScheme_NBR_ELT];
   ohm::opal::Image
                  _taxi_back;
   ohm::opal::Image
                  _taxi_main;
   ohm::opal::Color
                  _taxi_color_arr [model::Taxi::Color_NBR_ELT];
   ohm::opal::Color
                  _taxi_color_h_arr [model::Taxi::Color_NBR_ELT];
   ohm::opal::Image
                  _taxi_front;
   ohm::opal::Image
                  _taxi_reflect;

   ohm::opal::Image
                  _pedestrian_selection_arr [AspectMain::ColorScheme_NBR_ELT];
   ohm::opal::Image
                  _pedestrian_main;
   ohm::opal::Image
                  _pedestrian_main_h;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   // opak::AspectBase
   virtual void   do_init (ohm::opak::GraphicContext & context);
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   class BaseElements
   {
   public:
      ohm::opal::Bitmap
                  _back;

      ohm::opal::Bitmap
                  _taxi_selection;
      ohm::opal::Bitmap
                  _taxi_back;
      ohm::opal::Bitmap
                  _taxi_main;
      ohm::opal::Bitmap
                  _taxi_front;

      ohm::opal::Bitmap
                  _pedestrian_selection;
      ohm::opal::Bitmap
                  _pedestrian_main;
   };
   
   static void    load_base_elements ();
   
   void           make (ohm::opak::GraphicContext & context, const AspectMain & aspect);
   
   ohm::opak::AspectKeyedBase::Key
                  _aspect_key;
   
   static BaseElements
                  _be;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Aspect (const Aspect &other);
   Aspect &       operator = (const Aspect &other);
   bool           operator == (const Aspect &other);
   bool           operator != (const Aspect &other);

}; // class Aspect



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "client/Aspect.hpp"



#endif   // client_Aspect_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
