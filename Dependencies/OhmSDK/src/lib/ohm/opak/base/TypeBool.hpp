/*****************************************************************************

        TypeBool.hpp
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



#if defined (ohm_opak_TypeBool_CURRENT_CODEHEADER)
   #error Recursive inclusion of TypeBool code header.
#endif
#define  ohm_opak_TypeBool_CURRENT_CODEHEADER

#if ! defined (ohm_opak_TypeBool_CODEHEADER_INCLUDED)
#define  ohm_opak_TypeBool_CODEHEADER_INCLUDED



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
TypeBool <Target>::TypeBool (Target & target, Proc proc)
:  TypeKeyframable <Target, bool, bool, bool> (target, proc, false, false)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class Target>
TypeBool <Target>::TypeBool (Target & target, Proc proc, Proc prev_proc)
:  TypeKeyframable <Target, bool, bool, bool> (target, proc, prev_proc, false, false)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class Target>
TypeBool <Target>::~TypeBool ()
{
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target>
TypeBool <Target> &  TypeBool <Target>::operator = (const TypeBool & other)
{
   operator = (other.operator bool ());
   
   return *this;
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target>
TypeBool <Target> &  TypeBool <Target>::operator = (bool value)
{
   TypeKeyframable <Target, bool, bool, bool>::assign (value);
   
   return *this;
}



/*
==============================================================================
Name : operator Type
==============================================================================
*/

template <class Target>
TypeBool <Target>::operator bool () const
{
   return TypeKeyframable <Target, bool, bool, bool>::get_mt_value ();
}



/*
==============================================================================
Name : push
==============================================================================
*/

template <class Target>
TypeBool <Target> &  TypeBool <Target>::push (bool value, double transition_time)
{
   TypeKeyframable <Target, bool, bool, bool>
      ::sp_push (value, transition_time, step);
   
   return *this;
}



/*
==============================================================================
Name : loop
==============================================================================
*/

template <class Target>
TypeBool <Target> &  TypeBool <Target>::loop ()
{
   TypeKeyframable <Target, bool, bool, bool>::sp_loop ();
   
   return *this;
}



/*
==============================================================================
Name : stop
==============================================================================
*/

template <class Target>
TypeBool <Target> &  TypeBool <Target>::stop ()
{
   TypeKeyframable <Target, bool, bool, bool>::sp_stop ();
   
   return *this;
}




/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_convert
==============================================================================
*/

template <class Target>
void  TypeBool <Target>::do_convert (bool & dst, bool src)
{
   dst = src;
}



/*
==============================================================================
Name : do_interpolate
==============================================================================
*/

template <class Target>
void  TypeBool <Target>::do_interpolate (bool & dst, bool start_val, bool target_val, float interp_val)
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



#endif   // ohm_opak_TypeBool_CODEHEADER_INCLUDED

#undef ohm_opak_TypeBool_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
