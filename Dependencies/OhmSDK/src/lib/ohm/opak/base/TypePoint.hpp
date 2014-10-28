/*****************************************************************************

        TypePoint.hpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 34885

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



#if defined (ohm_opak_TypePoint_CURRENT_CODEHEADER)
   #error Recursive inclusion of TypePoint code header.
#endif
#define  ohm_opak_TypePoint_CURRENT_CODEHEADER

#if ! defined (ohm_opak_TypePoint_CODEHEADER_INCLUDED)
#define  ohm_opak_TypePoint_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



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
TypePoint <Target>::TypePoint (Target & target, Proc proc)
:  _x (target, proc)
,  _y (target, proc)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class Target>
TypePoint <Target>::TypePoint (Target & target, Proc proc, Proc prev_proc)
:  _x (target, proc, prev_proc)
,  _y (target, proc, prev_proc)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class Target>
TypePoint <Target>::~TypePoint ()
{
}



/*
==============================================================================
Name : enable_stabilization
Description :
   Require stabilization for 'this'.
   Stabilization is conceptually immutable, so this must be called in the
   ctor that use this class.
   Therefore this call is not thread safe : you cannot change stabilization
   dynamically.
==============================================================================
*/

template <class Target>
void  TypePoint <Target>::enable_stabilization ()
{
   _x.enable_stabilization ();
   _y.enable_stabilization ();
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target>
TypePoint <Target> & TypePoint <Target>::operator = (const TypePoint &other)
{
   _x = other._x;
   _y = other._y;
   
   return *this;
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target>
TypePoint <Target> & TypePoint <Target>::operator = (const opa::Point & value)
{
   _x = value._x;
   _y = value._y;
   
   return *this;
}



/*
==============================================================================
Name : operator opa::Point
==============================================================================
*/

template <class Target>
TypePoint <Target>::operator opa::Point () const
{
   opa::Point ret_val (_x, _y);
   
   return ret_val;
}



/*
==============================================================================
Name : init
==============================================================================
*/

template <class Target>
void  TypePoint <Target>::init (const opa::Point & value)
{
   _x.init (value._x);
   _y.init (value._y);
}



/*
==============================================================================
Name : init
==============================================================================
*/

template <class Target>
void  TypePoint <Target>::init (float x, float y)
{
   _x.init (x);
   _y.init (y);
}



/*
==============================================================================
Name : push
==============================================================================
*/

template <class Target>
TypePoint <Target> & TypePoint <Target>::push (const opa::Point & value, double transition_time, TransitionFunction transition_function)
{
   _x.push (value._x, transition_time, transition_function);
   _y.push (value._y, transition_time, transition_function);
   
   return *this;
}



/*
==============================================================================
Name : push_speed
==============================================================================
*/

template <class Target>
TypePoint <Target> & TypePoint <Target>::push_speed (const opa::Point & value, const opa::Point & abs_dv_per_sec, TransitionFunction transition_function)
{
   _x.push_speed (value._x, abs_dv_per_sec._x, transition_function);
   _y.push_speed (value._y, abs_dv_per_sec._y, transition_function);
   
   return *this;
}



/*
==============================================================================
Name : loop
==============================================================================
*/

template <class Target>
TypePoint <Target> & TypePoint <Target>::loop ()
{
   _x.loop ();
   _y.loop ();
   
   return *this;
}



/*
==============================================================================
Name : stop
==============================================================================
*/

template <class Target>
TypePoint <Target> & TypePoint <Target>::stop ()
{
   _x.stop ();
   _y.stop ();
   
   return *this;
}



/*
==============================================================================
Name : set_transition_time
==============================================================================
*/

#if ! defined (ohm_opak_TypeKeyframable_RMU)
template <class Target>
void  TypePoint <Target>::set_transition_time (double delay_s)
{
   _x.set_transition_time (delay_s);
   _y.set_transition_time (delay_s);
}
#endif



/*
==============================================================================
Name : set_transition_function
==============================================================================
*/

#if ! defined (ohm_opak_TypeKeyframable_RMU)
template <class Target>
void  TypePoint <Target>::set_transition_function (TransitionFunction function)
{
   _x.set_transition_function (function);
   _y.set_transition_function (function);
}
#endif



/*
==============================================================================
Name : do_stop
==============================================================================
*/

template <class Target>
void  TypePoint <Target>::do_stop ()
{
   _x.do_stop ();
   _y.do_stop ();
}



/*
==============================================================================
Name : is_animating
==============================================================================
*/

template <class Target>
bool  TypePoint <Target>::is_animating () const
{
   return (_x.is_animating () || _y.is_animating ());
}



/*
==============================================================================
Name : get_cur_value
==============================================================================
*/

template <class Target>
opa::Point  TypePoint <Target>::get_cur_value () const
{
   opa::Point ret_val (_x.get_cur_value (), _y.get_cur_value ());
   
   return ret_val;
}



/*
==============================================================================
Name : get_prev_value
==============================================================================
*/

template <class Target>
opa::Point  TypePoint <Target>::get_prev_value () const
{
   opa::Point ret_val (_x.get_prev_value (), _y.get_prev_value ());
   
   return ret_val;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



#endif   // ohm_opak_TypePoint_CODEHEADER_INCLUDED

#undef ohm_opak_TypePoint_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
