/*****************************************************************************

        TypeSprungImage.hpp
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



#if defined (ohm_opak_TypeSprungImage_CURRENT_CODEHEADER)
   #error Recursive inclusion of TypeSprungImage code header.
#endif
#define  ohm_opak_TypeSprungImage_CURRENT_CODEHEADER

#if ! defined (ohm_opak_TypeSprungImage_CODEHEADER_INCLUDED)
#define  ohm_opak_TypeSprungImage_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/private/Animator.h"
#include "ohm/opak/private/TransactionMaster.h"
#include "ohm/opal/ImageManager.h"
#include "ohm/lang/fnc.h"



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class Target>
TypeSprungImage <Target>::TypeSprungImage (Target & target, Proc proc)
:  TypeKeyframable <Target, ImageRectPair, const ImageRectPair &, opal::SprungImage> (target, proc, std::make_pair ("", opa::Rect::_zero), opal::SprungImage ())
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class Target>
TypeSprungImage <Target>::TypeSprungImage (Target & target, Proc proc, Proc prev_proc)
:  TypeKeyframable <Target, ImageRectPair, const ImageRectPair &, opal::SprungImage> (target, proc, prev_proc, std::make_pair ("", opa::Rect::_zero), opal::SprungImage ())
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class Target>
TypeSprungImage <Target>::~TypeSprungImage ()
{
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target>
TypeSprungImage <Target> & TypeSprungImage <Target>::operator = (const TypeSprungImage & other)
{
   operator = (other.operator const ImageRectPair & ());
   
   return *this;
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target>
TypeSprungImage <Target> & TypeSprungImage <Target>::operator = (const ImageRectPair & value)
{
   TypeKeyframable <Target, ImageRectPair, const ImageRectPair &, opal::SprungImage>::assign (value);
   
   return *this;
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target>
TypeSprungImage <Target> & TypeSprungImage <Target>::operator = (const std::pair <const char *, opa::Rect> & pair)
{
   ImageRectPair value;
   value.first = pair.first;
   value.second = pair.second;
   
   TypeKeyframable <Target, ImageRectPair, const ImageRectPair &, opal::SprungImage>::assign (value);
   
   return *this;
}



/*
==============================================================================
Name : operator Type
==============================================================================
*/

template <class Target>
TypeSprungImage <Target>::operator const ImageRectPair & () const
{
   return TypeKeyframable <Target, ImageRectPair, const ImageRectPair &, opal::SprungImage>::get_mt_value ();
}



/*
==============================================================================
Name : push
==============================================================================
*/

template <class Target>
TypeSprungImage <Target> & TypeSprungImage <Target>::push (const ImageRectPair & value, double transition_time)
{
   TypeKeyframable <Target, ImageRectPair, const ImageRectPair &, opal::SprungImage>
      ::sp_push (value, transition_time, step);
   
   return *this;
}



/*
==============================================================================
Name : push
==============================================================================
*/

template <class Target>
TypeSprungImage <Target> & TypeSprungImage <Target>::push (const std::string & path, const opa::Rect & rect, double transition_time)
{
   ImageRectPair value;
   value.first = path;
   value.second = rect;

   TypeKeyframable <Target, ImageRectPair, const ImageRectPair &, opal::SprungImage>
      ::sp_push (value, transition_time, step);
   
   return *this;
}



/*
==============================================================================
Name : loop
==============================================================================
*/

template <class Target>
TypeSprungImage <Target> & TypeSprungImage <Target>::loop ()
{
   TypeKeyframable <Target, ImageRectPair, const ImageRectPair &, opal::SprungImage>::sp_loop ();
   
   return *this;
}



/*
==============================================================================
Name : stop
==============================================================================
*/

template <class Target>
TypeSprungImage <Target> & TypeSprungImage <Target>::stop ()
{
   TypeKeyframable <Target, ImageRectPair, const ImageRectPair &, opal::SprungImage>::sp_stop ();
   
   return *this;
}




/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_convert
==============================================================================
*/

template <class Target>
void  TypeSprungImage <Target>::do_convert (opal::SprungImage & dst, const ImageRectPair & src)
{
   if (src.first.empty ())
   {
      opal::SprungImage empty_image;
      
      dst = empty_image;
   }
   else
   {
      dst = opal::ImageManager::use_instance ()
         .get_sprung_image (src.first, src.second);
   }
}



/*
==============================================================================
Name : do_interpolate
==============================================================================
*/

template <class Target>
void  TypeSprungImage <Target>::do_interpolate (ImageRectPair & dst, const ImageRectPair & start_val, const ImageRectPair & target_val, float interp_val)
{
   if (interp_val >= 1.f)
   {
      dst = target_val;
   }
   else
   {
      dst = start_val;
   }
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



#endif   // ohm_opak_TypeSprungImage_CODEHEADER_INCLUDED

#undef ohm_opak_TypeSprungImage_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
