/*****************************************************************************

        SprungImage.h
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59357

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



#if ! defined (ohm_opak_SprungImage_HEADER_INCLUDED)
#define  ohm_opak_SprungImage_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/base/Primitive.h"
#include "ohm/opak/base/Opacifiable.h"
#include "ohm/opak/base/Colorizable.h"
#include "ohm/opak/base/BlendModable.h"
#include "ohm/opak/base/Sizable.h"
#include "ohm/opak/base/BorderScalable.h"
#include "ohm/opak/base/Hintable.h"
#include "ohm/opak/base/TypeRect.h"
#include "ohm/opak/base/TypeString.h"
#include "ohm/opal/SprungImage.h"



namespace ohm
{
namespace opak
{



class SprungImage
:  public virtual Primitive
,  public Opacifiable
,  public Colorizable
,  public BlendModable
,  public Sizable
,  public BorderScalable
,  public Hintable
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  SprungImage ();
   
   // Video Thread
   virtual void   display (GraphicContext & context);

   TypeString <SprungImage>
                  _image;
   TypeRect <SprungImage>
                  _rect;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   virtual        ~SprungImage ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           image_changed ();

   opal::SprungImage
                  _sprung_image_tex;
   bool           _image_changed_flag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  SprungImage (const SprungImage &other);
   SprungImage &  operator = (const SprungImage &other);
   bool           operator == (const SprungImage &other);
   bool           operator != (const SprungImage &other);

}; // class SprungImage



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/opak/SprungImage.hpp"



#endif   // ohm_opak_SprungImage_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
