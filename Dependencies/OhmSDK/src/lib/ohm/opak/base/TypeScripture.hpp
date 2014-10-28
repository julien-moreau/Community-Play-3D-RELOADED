/*****************************************************************************

        TypeScripture.hpp
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



#if defined (ohm_opak_TypeScripture_CURRENT_CODEHEADER)
   #error Recursive inclusion of TypeScripture code header.
#endif
#define  ohm_opak_TypeScripture_CURRENT_CODEHEADER

#if ! defined (ohm_opak_TypeScripture_CODEHEADER_INCLUDED)
#define  ohm_opak_TypeScripture_CODEHEADER_INCLUDED



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
TypeScripture <Target>::TypeScripture (Target & target, Proc proc)
:  TypeKeyframable <Target, opal::Scripture, const opal::Scripture &, opal::Scripture> (target, proc, opal::Scripture (), opal::Scripture ())
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class Target>
TypeScripture <Target>::TypeScripture (Target & target, Proc proc, Proc prev_proc)
:  TypeKeyframable <Target, opal::Scripture, const opal::Scripture &, opal::Scripture> (target, proc, prev_proc, opal::Scripture (), opal::Scripture ())
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class Target>
TypeScripture <Target>::~TypeScripture ()
{
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target>
TypeScripture <Target> &   TypeScripture <Target>::operator = (const TypeScripture & other)
{
   operator = (other.operator const opal::Scripture & ());
   
   return *this;
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target>
TypeScripture <Target> &   TypeScripture <Target>::operator = (const opal::Scripture & value)
{
   TypeKeyframable <Target, opal::Scripture, const opal::Scripture &, opal::Scripture>::assign (value);
   
   return *this;
}



/*
==============================================================================
Name : operator Type
==============================================================================
*/

template <class Target>
TypeScripture <Target>::operator const opal::Scripture & () const
{
   return TypeKeyframable <Target, opal::Scripture, const opal::Scripture &, opal::Scripture>::get_mt_value ();
}



/*
==============================================================================
Name : push
==============================================================================
*/

template <class Target>
TypeScripture <Target> &   TypeScripture <Target>::push (const opal::Scripture & value, double transition_time)
{
   TypeKeyframable <Target, opal::Scripture, const opal::Scripture &, opal::Scripture>
      ::sp_push (value, transition_time, step);
   
   return *this;
}



/*
==============================================================================
Name : loop
==============================================================================
*/

template <class Target>
TypeScripture <Target> &   TypeScripture <Target>::loop ()
{
   TypeKeyframable <Target, opal::Scripture, const opal::Scripture &, opal::Scripture>::sp_loop ();
   
   return *this;
}



/*
==============================================================================
Name : stop
==============================================================================
*/

template <class Target>
TypeScripture <Target> &   TypeScripture <Target>::stop ()
{
   TypeKeyframable <Target, opal::Scripture, const opal::Scripture &, opal::Scripture>::sp_stop ();
   
   return *this;
}




/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_convert
==============================================================================
*/

template <class Target>
void  TypeScripture <Target>::do_convert (opal::Scripture & dst, const opal::Scripture & src)
{
   dst = src;
}



/*
==============================================================================
Name : do_interpolate
==============================================================================
*/

template <class Target>
void  TypeScripture <Target>::do_interpolate (opal::Scripture & dst, const opal::Scripture & start_val, const opal::Scripture & target_val, float interp_val)
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



#endif   // ohm_opak_TypeScripture_CODEHEADER_INCLUDED

#undef ohm_opak_TypeScripture_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
