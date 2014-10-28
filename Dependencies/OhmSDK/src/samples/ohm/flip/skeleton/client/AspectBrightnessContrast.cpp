/*****************************************************************************

        AspectBrightnessContrast.cpp
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



#if defined (_MSC_VER)
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in layo member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "client/AspectBrightnessContrast.h"

#include "ohm/opal/ColorEngine.h"
#include "ohm/opal/ImageLoader.h"
#include "ohm/opal/ImageManager.h"
#include "ohm/util/BinaryStreamOutput.h"

#include <cassert>



namespace client
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

AspectBrightnessContrast::AspectBrightnessContrast ()
:  _brightness (0.f)
,  _contrast (0.f)
,  _need_update_flag (true)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

AspectBrightnessContrast::~AspectBrightnessContrast ()
{
}



/*
==============================================================================
Name : make
==============================================================================
*/

void  AspectBrightnessContrast::make (ohm::opak::GraphicContext & context)
{
   bool update_flag = need_make (context);
   
   update_flag |= _need_update_flag;
   
   if (update_flag)
   {
      update ();
   }
}



/*
==============================================================================
Name : process_color
==============================================================================
*/

void  AspectBrightnessContrast::process_color (ohm::opal::Color & color) const
{
   color._r = process (color._r);
   color._g = process (color._g);
   color._b = process (color._b);
}



/*
==============================================================================
Name : process_color
==============================================================================
*/

void  AspectBrightnessContrast::process_color (ohm::opal::PixArgb * pix_ptr, size_t nbr) const
{
   using namespace ohm;
   
   if (_need_update_flag)
   {
      update ();
   }
   
   for (size_t i = 0 ; i < nbr ; ++i)
   {
      opal::PixArgb & pix = pix_ptr [i];
      
      pix._r = _val_arr [pix._r];
      pix._g = _val_arr [pix._g];
      pix._b = _val_arr [pix._b];
   }
}



/*
==============================================================================
Name : set_brightness
==============================================================================
*/

void  AspectBrightnessContrast::set_brightness (float brightness)
{
   _brightness = brightness;
   _need_update_flag = true;
   
   write_key ();
}



/*
==============================================================================
Name : set_contrast
==============================================================================
*/

void  AspectBrightnessContrast::set_contrast (float contrast)
{
   _contrast = contrast;
   _need_update_flag = true;
   
   write_key ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_init
==============================================================================
*/

void  AspectBrightnessContrast::do_init (ohm::opak::GraphicContext & context)
{
   // nothing
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : write_key
==============================================================================
*/

void  AspectBrightnessContrast::write_key ()
{
   using namespace ohm;
   
   util::BinaryStreamOutput bso (use_key ());
   
   bso.clear ();
   bso.write (_brightness);
   bso.write (_contrast);
}



/*
==============================================================================
Name : update
==============================================================================
*/

void  AspectBrightnessContrast::update () const
{
   using namespace ohm;
   
   for (int i = 0 ; i < 256 ; ++i)
   {
      float t = float (i) * (1.f / 255.f);
      
      float val = process (t) * 255.f;
      
      lang::limit_in_place (val, 0.f, 255.f);
      
      _val_arr [i] = archi::UByte (val);
   }
   
   _need_update_flag = false;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
