/*****************************************************************************

        TypeInt.hpp
        Copyright (c) 2009 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
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



#if defined (ohm_opak_TypeInt_CURRENT_CODEHEADER)
   #error Recursive inclusion of TypeInt code header.
#endif
#define  ohm_opak_TypeInt_CURRENT_CODEHEADER

#if ! defined (ohm_opak_TypeInt_CODEHEADER_INCLUDED)
#define  ohm_opak_TypeInt_CODEHEADER_INCLUDED



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

template <class Target>
TypeInt <Target>::TypeInt (Target & target, Proc proc)
:  TypeKeyframable <Target, int, int, int> (target, proc, 0, 0)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class Target>
TypeInt <Target>::TypeInt (Target & target, Proc proc, Proc prev_proc)
:  TypeKeyframable <Target, int, int, int> (target, proc, prev_proc, 0, 0)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class Target>
TypeInt <Target>::~TypeInt ()
{
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target>
TypeInt <Target> &   TypeInt <Target>::operator = (const TypeInt & other)
{
   operator = (other.operator int ());
   
   return *this;
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target>
TypeInt <Target> &   TypeInt <Target>::operator = (int value)
{
   TypeKeyframable <Target, int, int, int>::assign (value);
   
   return *this;
}



/*
==============================================================================
Name : operator Type
==============================================================================
*/

template <class Target>
TypeInt <Target>::operator int () const
{
   return TypeKeyframable <Target, int, int, int>::get_mt_value ();
}



/*
==============================================================================
Name : push
==============================================================================
*/

template <class Target>
TypeInt <Target> &   TypeInt <Target>::push (int value, double transition_time)
{
   TypeKeyframable <Target, int, int, int>
      ::sp_push (value, transition_time, step);
   
   return *this;
}



/*
==============================================================================
Name : loop
==============================================================================
*/

template <class Target>
TypeInt <Target> &   TypeInt <Target>::loop ()
{
   TypeKeyframable <Target, int, int, int>::sp_loop ();
   
   return *this;
}



/*
==============================================================================
Name : stop
==============================================================================
*/

template <class Target>
TypeInt <Target> &   TypeInt <Target>::stop ()
{
   TypeKeyframable <Target, int, int, int>::sp_stop ();
   
   return *this;
}




/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_convert
==============================================================================
*/

template <class Target>
void  TypeInt <Target>::do_convert (int & dst, int src)
{
   dst = src;
}



/*
==============================================================================
Name : do_interpolate
==============================================================================
*/

template <class Target>
void  TypeInt <Target>::do_interpolate (int & dst, int start_val, int target_val, float interp_val)
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



#endif   // ohm_opak_TypeInt_CODEHEADER_INCLUDED

#undef ohm_opak_TypeInt_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
