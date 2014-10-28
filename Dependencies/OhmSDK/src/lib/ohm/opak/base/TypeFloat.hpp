/*****************************************************************************

        TypeFloat.hpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 47250

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



#if defined (ohm_opak_TypeFloat_CURRENT_CODEHEADER)
   #error Recursive inclusion of TypeFloat code header.
#endif
#define  ohm_opak_TypeFloat_CURRENT_CODEHEADER

#if ! defined (ohm_opak_TypeFloat_CODEHEADER_INCLUDED)
#define  ohm_opak_TypeFloat_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/private/Animator.h"
#include "ohm/opak/private/TransactionMaster.h"
#include "ohm/lang/fnc.h"
#include "ohm/math/fnc.h"



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
TypeFloat <Target>::TypeFloat (Target & target, Proc proc)
:  TypeKeyframable <Target, float, float, float> (target, proc, 0.f, 0.f)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class Target>
TypeFloat <Target>::TypeFloat (Target & target, Proc proc, Proc prev_proc)
:  TypeKeyframable <Target, float, float, float> (target, proc, prev_proc, 0.f, 0.f)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class Target>
TypeFloat <Target>::~TypeFloat ()
{
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target>
TypeFloat <Target> & TypeFloat <Target>::operator = (const TypeFloat & other)
{
   operator = (other.operator float ());
   
   return *this;
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target>
TypeFloat <Target> & TypeFloat <Target>::operator = (float value)
{
   assert (math::is_finite (value));
   
   TypeKeyframable <Target, float, float, float>::assign (value);
   
   return *this;
}



/*
==============================================================================
Name : operator Type
==============================================================================
*/

template <class Target>
TypeFloat <Target>::operator float () const
{
   return TypeKeyframable <Target, float, float, float>::get_mt_value ();
}



/*
==============================================================================
Name : push
==============================================================================
*/

template <class Target>
TypeFloat <Target> & TypeFloat <Target>::push (float value, double transition_time, TransitionFunction transition_function)
{
   assert (math::is_finite (value));
   
   TypeKeyframable <Target, float, float, float>
      ::sp_push (value, transition_time, transition_function);
   
   return *this;
}



/*
==============================================================================
Name : push_speed
==============================================================================
*/

template <class Target>
TypeFloat <Target> & TypeFloat <Target>::push_speed (float value, float abs_dv_per_sec, TransitionFunction transition_function)
{
   assert (math::is_finite (value));
   
   abs_dv_per_sec = lang::abs (abs_dv_per_sec);
   
   assert (abs_dv_per_sec > 0.0);
   
   double transition_time = (value - float (*this)) / abs_dv_per_sec;
   
   TypeKeyframable <Target, float, float, float>
      ::sp_push (value, transition_time, transition_function);
   
   return *this;
}



/*
==============================================================================
Name : loop
==============================================================================
*/

template <class Target>
TypeFloat <Target> & TypeFloat <Target>::loop ()
{
   TypeKeyframable <Target, float, float, float>::sp_loop ();
   
   return *this;
}



/*
==============================================================================
Name : stop
==============================================================================
*/

template <class Target>
TypeFloat <Target> & TypeFloat <Target>::stop ()
{
   TypeKeyframable <Target, float, float, float>::sp_stop ();
   
   return *this;
}




/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_convert
==============================================================================
*/

template <class Target>
void  TypeFloat <Target>::do_convert (float & dst, float src)
{
   dst = src;
}



/*
==============================================================================
Name : do_interpolate
==============================================================================
*/

template <class Target>
void  TypeFloat <Target>::do_interpolate (float & dst, float start_val, float target_val, float interp_val)
{
   dst = start_val + interp_val * (target_val - start_val);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



#endif   // ohm_opak_TypeFloat_CODEHEADER_INCLUDED

#undef ohm_opak_TypeFloat_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
