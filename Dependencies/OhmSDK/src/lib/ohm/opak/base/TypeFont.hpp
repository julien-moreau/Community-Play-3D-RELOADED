/*****************************************************************************

        TypeFont.hpp
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



#if defined (ohm_opak_TypeFont_CURRENT_CODEHEADER)
   #error Recursive inclusion of TypeFont code header.
#endif
#define  ohm_opak_TypeFont_CURRENT_CODEHEADER

#if ! defined (ohm_opak_TypeFont_CODEHEADER_INCLUDED)
#define  ohm_opak_TypeFont_CODEHEADER_INCLUDED



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
TypeFont <Target>::TypeFont (Target & target, Proc proc)
:  TypeKeyframable <Target, NameSizePair, const NameSizePair &, FontProxy> (target, proc, std::make_pair ("", 0.f), FontProxy ())
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class Target>
TypeFont <Target>::TypeFont (Target & target, Proc proc, Proc prev_proc)
:  TypeKeyframable <Target, NameSizePair, const NameSizePair &, FontProxy> (target, proc, prev_proc, std::make_pair ("", 0.f), FontProxy ())
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class Target>
TypeFont <Target>::~TypeFont ()
{
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target>
TypeFont <Target> &  TypeFont <Target>::operator = (const TypeFont & other)
{
   operator = (other.operator const NameSizePair & ());
   
   return *this;
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target>
TypeFont <Target> &  TypeFont <Target>::operator = (const NameSizePair & value)
{
   TypeKeyframable <Target, NameSizePair, const NameSizePair &, FontProxy>::assign (value);
   
   return *this;
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target>
TypeFont <Target> &  TypeFont <Target>::operator = (const std::pair <const char *, float> & pair)
{
   NameSizePair value;
   value = pair;
   
   TypeKeyframable <Target, NameSizePair, const NameSizePair &, FontProxy>::assign (value);
   
   return *this;
}



/*
==============================================================================
Name : operator Type
==============================================================================
*/

template <class Target>
TypeFont <Target>::operator const NameSizePair & () const
{
   return TypeKeyframable <Target, NameSizePair, const NameSizePair &, FontProxy>::get_mt_value ();
}



/*
==============================================================================
Name : push
==============================================================================
*/

template <class Target>
TypeFont <Target> &  TypeFont <Target>::push (const NameSizePair & value, double transition_time)
{
   TypeKeyframable <Target, NameSizePair, const NameSizePair &, FontProxy>
      ::sp_push (value, transition_time, step);
   
   return *this;
}



/*
==============================================================================
Name : push
==============================================================================
*/

template <class Target>
TypeFont <Target> &  TypeFont <Target>::push (const std::string & name, float size, double transition_time)
{
   NameSizePair value;
   value.first = name;
   value.second = size;

   TypeKeyframable <Target, NameSizePair, const NameSizePair &, FontProxy>
      ::sp_push (value, transition_time, step);
   
   return *this;
}



/*
==============================================================================
Name : loop
==============================================================================
*/

template <class Target>
TypeFont <Target> &  TypeFont <Target>::loop ()
{
   TypeKeyframable <Target, NameSizePair, const NameSizePair &, FontProxy>::sp_loop ();
   
   return *this;
}



/*
==============================================================================
Name : stop
==============================================================================
*/

template <class Target>
TypeFont <Target> &  TypeFont <Target>::stop ()
{
   TypeKeyframable <Target, NameSizePair, const NameSizePair &, FontProxy>::sp_stop ();
   
   return *this;
}




/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_convert
==============================================================================
*/

template <class Target>
void  TypeFont <Target>::do_convert (FontProxy & dst, const NameSizePair & src)
{
   if (src.first.empty ())
   {
      FontProxy empty_font_proxy;
      
      dst = empty_font_proxy;
   }
   else
   {
      NameSizePair font_proxy;
      font_proxy = src;
      
      dst = font_proxy;
   }
}



/*
==============================================================================
Name : do_interpolate
==============================================================================
*/

template <class Target>
void  TypeFont <Target>::do_interpolate (NameSizePair & dst, const NameSizePair & start_val, const NameSizePair & target_val, float interp_val)
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



#endif   // ohm_opak_TypeFont_CODEHEADER_INCLUDED

#undef ohm_opak_TypeFont_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
