/*****************************************************************************

        TypeMargin.hpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54606

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



#if defined (ohm_opak_TypeMargin_CURRENT_CODEHEADER)
   #error Recursive inclusion of TypeMargin code header.
#endif
#define  ohm_opak_TypeMargin_CURRENT_CODEHEADER

#if ! defined (ohm_opak_TypeMargin_CODEHEADER_INCLUDED)
#define  ohm_opak_TypeMargin_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

template <class Target>
TypeMargin <Target>::TypeMargin (Target & target, Proc proc)
:  _left (target, proc)
,  _top (target, proc)
,  _right (target, proc)
,  _bottom (target, proc)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class Target>
TypeMargin <Target>::~TypeMargin ()
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
void  TypeMargin <Target>::enable_stabilization ()
{
   _left.enable_stabilization ();
   _top.enable_stabilization ();
   _right.enable_stabilization ();
   _bottom.enable_stabilization ();
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target>
TypeMargin <Target> &   TypeMargin <Target>::operator = (const TypeMargin &other)
{
   _left = other._left;
   _top = other._top;
   _right = other._right;
   _bottom = other._bottom;
   
   return *this;
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target>
TypeMargin <Target> &   TypeMargin <Target>::operator = (const opa::Margin & value)
{
   _left = value._left;
   _top = value._top;
   _right = value._right;
   _bottom = value._bottom;
   
   return *this;
}



/*
==============================================================================
Name : operator opa::Margin
==============================================================================
*/

template <class Target>
TypeMargin <Target>::operator opa::Margin () const
{
   opa::Margin ret_val;
   ret_val._left = _left;
   ret_val._top = _top;
   ret_val._right = _right;
   ret_val._bottom = _bottom;
   
   return ret_val;
}



/*
==============================================================================
Name : init
==============================================================================
*/

template <class Target>
void  TypeMargin <Target>::init (const opa::Margin & value)
{
   _left.init (value._left);
   _top.init (value._top);
   _right.init (value._right);
   _bottom.init (value._bottom);
}



/*
==============================================================================
Name : init
==============================================================================
*/

template <class Target>
void  TypeMargin <Target>::init (float a, float r, float g, float b)
{
   _left.init (a);
   _top.init (r);
   _right.init (g);
   _bottom.init (b);
}



/*
==============================================================================
Name : push
==============================================================================
*/

template <class Target>
TypeMargin <Target> &   TypeMargin <Target>::push (const opa::Margin & value, double transition_time, TransitionFunction transition_function)
{
   _left.push (value._left, transition_time, transition_function);
   _top.push (value._top, transition_time, transition_function);
   _right.push (value._right, transition_time, transition_function);
   _bottom.push (value._bottom, transition_time, transition_function);
   
   return *this;
}



/*
==============================================================================
Name : loop
==============================================================================
*/

template <class Target>
TypeMargin <Target> &   TypeMargin <Target>::loop ()
{
   _left.loop ();
   _top.loop ();
   _right.loop ();
   _bottom.loop ();
   
   return *this;
}



/*
==============================================================================
Name : stop
==============================================================================
*/

template <class Target>
TypeMargin <Target> &   TypeMargin <Target>::stop ()
{
   _left.stop ();
   _top.stop ();
   _right.stop ();
   _bottom.stop ();
   
   return *this;
}



/*
==============================================================================
Name : set_transition_time
==============================================================================
*/

template <class Target>
void  TypeMargin <Target>::set_transition_time (double delay_s)
{
   _left.set_transition_time (delay_s);
   _top.set_transition_time (delay_s);
   _right.set_transition_time (delay_s);
   _bottom.set_transition_time (delay_s);
}



/*
==============================================================================
Name : set_transition_function
==============================================================================
*/

template <class Target>
void  TypeMargin <Target>::set_transition_function (TransitionFunction function)
{
   _left.set_transition_function (function);
   _top.set_transition_function (function);
   _right.set_transition_function (function);
   _bottom.set_transition_function (function);
}



/*
==============================================================================
Name : do_stop
==============================================================================
*/

template <class Target>
void  TypeMargin <Target>::do_stop ()
{
   _left.do_stop ();
   _top.do_stop ();
   _right.do_stop ();
   _bottom.do_stop ();
}



/*
==============================================================================
Name : is_animating
==============================================================================
*/

template <class Target>
bool  TypeMargin <Target>::is_animating () const
{
   return (
      _left.is_animating ()
      || _top.is_animating ()
      || _right.is_animating ()
      || _bottom.is_animating ()
   );
}



/*
==============================================================================
Name : get_cur_value
==============================================================================
*/

template <class Target>
opa::Margin TypeMargin <Target>::get_cur_value () const
{
   opa::Margin ret_val;
   ret_val._left = _left.get_cur_value ();
   ret_val._top = _top.get_cur_value ();
   ret_val._right = _right.get_cur_value ();
   ret_val._bottom = _bottom.get_cur_value ();
   
   return ret_val;
}



/*
==============================================================================
Name : get_prev_value
==============================================================================
*/

template <class Target>
opa::Margin TypeMargin <Target>::get_prev_value () const
{
   opa::Margin ret_val;
   ret_val._left = _left.get_prev_value ();
   ret_val._top = _top.get_prev_value ();
   ret_val._right = _right.get_prev_value ();
   ret_val._bottom = _bottom.get_prev_value ();
   
   return ret_val;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



#endif   // ohm_opak_TypeMargin_CODEHEADER_INCLUDED

#undef ohm_opak_TypeMargin_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
