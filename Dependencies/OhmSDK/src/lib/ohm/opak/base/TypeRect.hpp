/*****************************************************************************

        TypeRect.hpp
        Copyright (c) 2009 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
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



#if defined (ohm_opak_TypeRect_CURRENT_CODEHEADER)
   #error Recursive inclusion of TypeRect code header.
#endif
#define  ohm_opak_TypeRect_CURRENT_CODEHEADER

#if ! defined (ohm_opak_TypeRect_CODEHEADER_INCLUDED)
#define  ohm_opak_TypeRect_CODEHEADER_INCLUDED



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
TypeRect <Target>::TypeRect (Target & target, Proc proc)
:  _origin (target, proc)
,  _size (target, proc)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class Target>
TypeRect <Target>::TypeRect (Target & target, Proc proc, Proc prev_proc)
:  _origin (target, proc, prev_proc)
,  _size (target, proc, prev_proc)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class Target>
TypeRect <Target>::~TypeRect ()
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
void  TypeRect <Target>::enable_stabilization ()
{
   _origin.enable_stabilization ();
   _size.enable_stabilization ();
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target>
TypeRect <Target> &  TypeRect <Target>::operator = (const TypeRect &other)
{
   _origin = other._origin;
   _size = other._size;
   
   return *this;
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target>
TypeRect <Target> &  TypeRect <Target>::operator = (const opa::Rect & value)
{
   _origin = value._origin;
   _size = value._size;
   
   return *this;
}



/*
==============================================================================
Name : operator opa::Rect
==============================================================================
*/

template <class Target>
TypeRect <Target>::operator opa::Rect () const
{
   opa::Rect ret_val (_origin, _size);
   
   return ret_val;
}



/*
==============================================================================
Name : init
==============================================================================
*/

template <class Target>
void  TypeRect <Target>::init (const opa::Rect & value)
{
   _origin.init (value._origin);
   _size.init (value._size);
}



/*
==============================================================================
Name : init
==============================================================================
*/

template <class Target>
void  TypeRect <Target>::init (const opa::Point & origin, const opa::Size & size)
{
   _origin.init (_origin);
   _size.init (_size);
}



/*
==============================================================================
Name : init
==============================================================================
*/

template <class Target>
void  TypeRect <Target>::init (float x, float y, float w, float h)
{
   _origin.init (x, y);
   _size.init (w, h);
}



/*
==============================================================================
Name : push
==============================================================================
*/

template <class Target>
TypeRect <Target> &  TypeRect <Target>::push (const opa::Rect & value, double transition_time, TransitionFunction transition_function)
{
   _origin.push (value._origin, transition_time, transition_function);
   _size.push (value._size, transition_time, transition_function);
   
   return *this;
}



/*
==============================================================================
Name : push_speed
==============================================================================
*/

template <class Target>
TypeRect <Target> &  TypeRect <Target>::push_speed (const opa::Rect & value, const opa::Rect & abs_dv_per_sec, TransitionFunction transition_function)
{
   _origin.push_speed (value._origin, abs_dv_per_sec._origin, transition_function);
   _size.push_speed (value._size, abs_dv_per_sec._size, transition_function);
   
   return *this;
}



/*
==============================================================================
Name : loop
==============================================================================
*/

template <class Target>
TypeRect <Target> &  TypeRect <Target>::loop ()
{
   _origin.loop ();
   _size.loop ();
   
   return *this;
}



/*
==============================================================================
Name : stop
==============================================================================
*/

template <class Target>
TypeRect <Target> &  TypeRect <Target>::stop ()
{
   _origin.stop ();
   _size.stop ();
   
   return *this;
}



/*
==============================================================================
Name : set_transition_delay
==============================================================================
*/

template <class Target>
void  TypeRect <Target>::set_transition_time (double delay_s)
{
   _origin.set_transition_time (delay_s);
   _size.set_transition_time (delay_s);
}



/*
==============================================================================
Name : set_transition_function
==============================================================================
*/

template <class Target>
void  TypeRect <Target>::set_transition_function (TransitionFunction function)
{
   _origin.set_transition_function (function);
   _size.set_transition_function (function);
}



/*
==============================================================================
Name : do_stop_transition
==============================================================================
*/

template <class Target>
void  TypeRect <Target>::do_stop_transition ()
{
   _origin.do_stop ();
   _size.do_stop ();
}



/*
==============================================================================
Name : is_animating
==============================================================================
*/

template <class Target>
bool  TypeRect <Target>::is_animating () const
{
   return (_origin.is_animating () || _size.is_animating ());
}



/*
==============================================================================
Name : get_cur_value
==============================================================================
*/

template <class Target>
opa::Rect   TypeRect <Target>::get_cur_value () const
{
   opa::Rect ret_val (_origin.get_cur_value (), _size.get_cur_value ());
   
   return ret_val;
}



/*
==============================================================================
Name : get_prev_value
==============================================================================
*/

template <class Target>
opa::Rect   TypeRect <Target>::get_prev_value () const
{
   opa::Rect ret_val (_origin.get_prev_value (), _size.get_prev_value ());
   
   return ret_val;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



#endif   // ohm_opak_TypeRect_CODEHEADER_INCLUDED

#undef ohm_opak_TypeRect_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
