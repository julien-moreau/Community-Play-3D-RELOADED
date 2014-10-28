/*****************************************************************************

        SprungBitmap.h
        Copyright (c) 2012 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 68147

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



#if ! defined (ohm_opak_SprungBitmap_HEADER_INCLUDED)
#define  ohm_opak_SprungBitmap_HEADER_INCLUDED

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
#include "ohm/opak/base/TypeBitmap.h"
#include "ohm/opak/base/TypeRect.h"
#include "ohm/opal/Bitmap.h"



namespace ohm
{
namespace opak
{



class SprungBitmap
:  public virtual Primitive
,  public Opacifiable
,  public BlendModable
,  public Sizable
,  public BorderScalable
,  public Hintable
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  SprungBitmap ();
   
   // Video Thread
   virtual void   display (GraphicContext & context);

   TypeBitmap <SprungBitmap>
                  _bitmap;
   TypeRect <SprungBitmap>
                  _rect;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   virtual        ~SprungBitmap ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           something_changed ();



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  SprungBitmap (const SprungBitmap &other);
   SprungBitmap & operator = (const SprungBitmap &other);
   bool           operator == (const SprungBitmap &other);
   bool           operator != (const SprungBitmap &other);

}; // class SprungBitmap



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/opak/SprungBitmap.hpp"



#endif   // ohm_opak_SprungBitmap_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
