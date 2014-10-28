/*****************************************************************************

        AspectBrightnessContrast.h
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



#if ! defined (client_AspectBrightnessContrast_HEADER_INCLUDED)
#define  client_AspectBrightnessContrast_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/base/AspectColorProcessorBase.h"
#include "ohm/lang/fnc.h"



namespace client
{



class AspectBrightnessContrast
:  public ohm::opak::AspectColorProcessorBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  AspectBrightnessContrast ();
   virtual        ~AspectBrightnessContrast ();
   
   // opak::AspectBase
   virtual void   make (ohm::opak::GraphicContext & context);
   
   // opak::AspectColorProcessorBase
   virtual void   process_color (ohm::opal::Color & color) const;
   virtual void   process_color (ohm::opal::PixArgb * pix_ptr, size_t nbr) const;
   
   void           set_brightness (float brightness);
   void           set_contrast (float contrast);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   // opak::AspectBase
   virtual void   do_init (ohm::opak::GraphicContext & context);
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           write_key ();
   void           update () const;
   
   inline float   process (float x) const
   {
      using namespace ohm;
   
      // contrast
      
      float cx;
      
      if (x <= .5f)
      {
         cx = x * (x - .5f) * 2.5f * _contrast;
      }
      else
      {
         cx = - (x - .5f) * (x - 1.f) * 2.5f * _contrast;
      }
      
      // brightness
      
      float x2m1 = 2.f * x - 1.f;
      
      float bx = _brightness * .25f * (1.f - x2m1 * x2m1);
      
      float ret_val = x + cx + bx;
      
      lang::limit_in_place (ret_val, 0.f, 1.f);
      
      return ret_val;
   }
   
   mutable bool   _need_update_flag;
   float          _brightness;
   float          _contrast;
   
   mutable ohm::archi::UByte
                  _val_arr [256];



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  AspectBrightnessContrast (const AspectBrightnessContrast &other);
   AspectBrightnessContrast & operator = (const AspectBrightnessContrast &other);
   bool           operator == (const AspectBrightnessContrast &other);
   bool           operator != (const AspectBrightnessContrast &other);

}; // class AspectBrightnessContrast



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "client/AspectBrightnessContrast.hpp"



#endif   // client_AspectBrightnessContrast_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
