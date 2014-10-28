/*****************************************************************************

        TypeSprungImage.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 45161

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



#if ! defined (ohm_opak_TypeSprungImage_HEADER_INCLUDED)
#define  ohm_opak_TypeSprungImage_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/base/TypeKeyframable.h"
#include "ohm/opal/SprungImage.h"

#include <list>



namespace ohm
{
namespace opak
{



typedef std::pair <std::string, opa::Rect> ImageRectPair;



template <class Target>
class TypeSprungImage
:  public TypeKeyframable <Target, ImageRectPair, const ImageRectPair &, opal::SprungImage>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef void (Target::*Proc)();
   
                  TypeSprungImage (Target & target, Proc proc);
                  TypeSprungImage (Target & target, Proc proc, Proc prev_proc);
   virtual        ~TypeSprungImage ();
   
   TypeSprungImage &
                  operator = (const TypeSprungImage &other);
   TypeSprungImage &
                  operator = (const ImageRectPair & value);
   TypeSprungImage &
                  operator = (const std::pair <const char *, opa::Rect> & pair);
                  operator const ImageRectPair & () const;

   TypeSprungImage &
                  push (const ImageRectPair & value, double transition_time);
   TypeSprungImage &
                  push (const std::string & path, const opa::Rect & rect, double transition_time);
   TypeSprungImage &
                  loop ();
   TypeSprungImage &
                  stop ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   // TypeKeyframable
   virtual void   do_convert (opal::SprungImage & dst, const ImageRectPair & src);
   virtual void   do_interpolate (ImageRectPair & dst, const ImageRectPair & start_val, const ImageRectPair & target_val, float interp_val);
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  TypeSprungImage ();
                  TypeSprungImage (const TypeSprungImage &other);
   bool           operator == (const TypeSprungImage &other);
   bool           operator != (const TypeSprungImage &other);

}; // class TypeSprungImage



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/opak/base/TypeSprungImage.hpp"



#endif   // ohm_opak_TypeSprungImage_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
