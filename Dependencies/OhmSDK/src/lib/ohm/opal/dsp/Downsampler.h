/*****************************************************************************

        Downsampler.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 31871

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



#if ! defined (ohm_opal_Downsampler_HEADER_INCLUDED)
#define  ohm_opal_Downsampler_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"



namespace ohm
{
namespace opa
{
class Size;
}



namespace opal
{



class Bitmap;

class Downsampler
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum Method
   {
                  Method_NEAREST = 0,
                  Method_BILINEAR,
                  Method_LANCZOS2,
                  Method_TURKOWSKI,
   };
   
                  Downsampler ();
   virtual        ~Downsampler ();
   
   void           set (Method method);
   void           set (archi::UInt32 width, archi::UInt32 height);

   void           process (Bitmap & dst, const Bitmap & src);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           process_bilinear (Bitmap & dst, const Bitmap & src);
   void           process_lanczos2_x (Bitmap & dst, const Bitmap & src);
   void           process_lanczos2_y (Bitmap & dst, const Bitmap & src);
   void           process_turkowski_x (Bitmap & dst, const Bitmap & src);
   void           process_turkowski_y (Bitmap & dst, const Bitmap & src);

   float          box (float x);
   float          tent (float x);
   float          lanczos2 (float x);
   float          sinc (float x);
   
   Method         _method;
   
   archi::UInt32  _width;
   archi::UInt32  _height;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Downsampler (const Downsampler &other);
   Downsampler &        operator = (const Downsampler &other);
   bool           operator == (const Downsampler &other);
   bool           operator != (const Downsampler &other);

}; // class Downsampler



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_Downsampler_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
