/*****************************************************************************

        TypeImage.hpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59396

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



#if defined (ohm_opak_TypeImage_CURRENT_CODEHEADER)
   #error Recursive inclusion of TypeImage code header.
#endif
#define  ohm_opak_TypeImage_CURRENT_CODEHEADER

#if ! defined (ohm_opak_TypeImage_CODEHEADER_INCLUDED)
#define  ohm_opak_TypeImage_CODEHEADER_INCLUDED



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
TypeImage <Target>::TypeImage (Target & target, Proc proc)
:  TypeKeyframable <Target, std::string, const std::string &, opal::Image> (target, proc, "", opal::Image ())
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class Target>
TypeImage <Target>::TypeImage (Target & target, Proc proc, Proc prev_proc)
:  TypeKeyframable <Target, std::string, const std::string &, opal::Image> (target, proc, prev_proc, "", opal::Image ())
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class Target>
TypeImage <Target>::~TypeImage ()
{
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target>
TypeImage <Target> & TypeImage <Target>::operator = (const TypeImage & other)
{
   operator = (other.operator const std::string & ());
   
   return *this;
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target>
TypeImage <Target> & TypeImage <Target>::operator = (const std::string & value)
{
   TypeKeyframable <Target, std::string, const std::string &, opal::Image>::assign (value);
   
   return *this;
}



/*
==============================================================================
Name : operator Type
==============================================================================
*/

template <class Target>
TypeImage <Target>::operator const std::string & () const
{
   return TypeKeyframable <Target, std::string, const std::string &, opal::Image>::get_mt_value ();
}



/*
==============================================================================
Name : push
==============================================================================
*/

template <class Target>
TypeImage <Target> & TypeImage <Target>::push (const std::string & value, double transition_time)
{
   TypeKeyframable <Target, std::string, const std::string &, opal::Image>
      ::sp_push (value, transition_time, step);
   
   return *this;
}



/*
==============================================================================
Name : loop
==============================================================================
*/

template <class Target>
TypeImage <Target> & TypeImage <Target>::loop ()
{
   TypeKeyframable <Target, std::string, const std::string &, opal::Image>::sp_loop ();
   
   return *this;
}



/*
==============================================================================
Name : stop
==============================================================================
*/

template <class Target>
TypeImage <Target> & TypeImage <Target>::stop ()
{
   TypeKeyframable <Target, std::string, const std::string &, opal::Image>::sp_stop ();
   
   return *this;
}




/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_convert
==============================================================================
*/

template <class Target>
void  TypeImage <Target>::do_convert (opal::Image & dst, const std::string & src)
{
   assert (false);
   
   if (src.empty ())
   {
      opal::Image empty_image;
      
      dst = empty_image;
   }
   else
   {
      /*dst = opal::ImageManager::use_instance ().get_image (src);*/
   }
}



/*
==============================================================================
Name : do_interpolate
==============================================================================
*/

template <class Target>
void  TypeImage <Target>::do_interpolate (std::string & dst, const std::string & start_val, const std::string & target_val, float interp_val)
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



#endif   // ohm_opak_TypeImage_CODEHEADER_INCLUDED

#undef ohm_opak_TypeImage_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
