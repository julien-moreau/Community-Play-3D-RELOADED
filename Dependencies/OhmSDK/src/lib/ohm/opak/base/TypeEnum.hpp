/*****************************************************************************

        TypeEnum.hpp
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



#if defined (ohm_opak_TypeEnum_CURRENT_CODEHEADER)
   #error Recursive inclusion of TypeEnum code header.
#endif
#define  ohm_opak_TypeEnum_CURRENT_CODEHEADER

#if ! defined (ohm_opak_TypeEnum_CODEHEADER_INCLUDED)
#define  ohm_opak_TypeEnum_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/private/Animator.h"
#include "ohm/opak/private/TransactionMaster.h"
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

template <class Target, typename Type, Type default_val>
TypeEnum <Target, Type, default_val>::TypeEnum (Target & target, Proc proc)
:  TypeKeyframable <Target, Type, Type, Type> (target, proc, default_val, default_val)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class Target, typename Type, Type default_val>
TypeEnum <Target, Type, default_val>::TypeEnum (Target & target, Proc proc, Proc prev_proc)
:  TypeKeyframable <Target, Type, Type, Type> (target, proc, prev_proc, default_val, default_val)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class Target, typename Type, Type default_val>
TypeEnum <Target, Type, default_val>::~TypeEnum ()
{
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target, typename Type, Type default_val>
TypeEnum <Target, Type, default_val> & TypeEnum <Target, Type, default_val>::operator = (const TypeEnum & other)
{
   operator = (other.operator Type ());
   
   return *this;
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target, typename Type, Type default_val>
TypeEnum <Target, Type, default_val> & TypeEnum <Target, Type, default_val>::operator = (Type value)
{
   TypeKeyframable <Target, Type, Type, Type>::assign (value);
   
   return *this;
}



/*
==============================================================================
Name : operator Type
==============================================================================
*/

template <class Target, typename Type, Type default_val>
TypeEnum <Target, Type, default_val>::operator Type () const
{
   return TypeKeyframable <Target, Type, Type, Type>::get_mt_value ();
}



/*
==============================================================================
Name : push
==============================================================================
*/

template <class Target, typename Type, Type default_val>
TypeEnum <Target, Type, default_val> & TypeEnum <Target, Type, default_val>::push (Type value, double transition_time)
{
   TypeKeyframable <Target, Type, Type, Type>
      ::sp_push (value, transition_time, step);
   
   return *this;
}



/*
==============================================================================
Name : loop
==============================================================================
*/

template <class Target, typename Type, Type default_val>
TypeEnum <Target, Type, default_val> & TypeEnum <Target, Type, default_val>::loop ()
{
   TypeKeyframable <Target, Type, Type, Type>::sp_loop ();
   
   return *this;
}



/*
==============================================================================
Name : stop
==============================================================================
*/

template <class Target, typename Type, Type default_val>
TypeEnum <Target, Type, default_val> & TypeEnum <Target, Type, default_val>::stop ()
{
   TypeKeyframable <Target, Type, Type, Type>::sp_stop ();
   
   return *this;
}




/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_convert
==============================================================================
*/

template <class Target, typename Type, Type default_val>
void  TypeEnum <Target, Type, default_val>::do_convert (Type & dst, Type src)
{
   dst = src;
}



/*
==============================================================================
Name : do_interpolate
==============================================================================
*/

template <class Target, typename Type, Type default_val>
void  TypeEnum <Target, Type, default_val>::do_interpolate (Type & dst, Type start_val, Type target_val, float interp_val)
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



#endif   // ohm_opak_TypeEnum_CODEHEADER_INCLUDED

#undef ohm_opak_TypeEnum_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
