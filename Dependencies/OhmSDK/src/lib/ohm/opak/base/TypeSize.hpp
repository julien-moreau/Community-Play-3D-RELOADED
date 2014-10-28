/*****************************************************************************

        TypeSize.hpp
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



#if defined (ohm_opak_TypeSize_CURRENT_CODEHEADER)
   #error Recursive inclusion of TypeSize code header.
#endif
#define  ohm_opak_TypeSize_CURRENT_CODEHEADER

#if ! defined (ohm_opak_TypeSize_CODEHEADER_INCLUDED)
#define  ohm_opak_TypeSize_CODEHEADER_INCLUDED



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
TypeSize <Target>::TypeSize (Target & target, Proc proc)
:  _width (target, proc)
,  _height (target, proc)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class Target>
TypeSize <Target>::~TypeSize ()
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
void  TypeSize <Target>::enable_stabilization ()
{
   _width.enable_stabilization ();
   _height.enable_stabilization ();
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target>
TypeSize <Target> &  TypeSize <Target>::operator = (const TypeSize &other)
{
   _width = other._width;
   _height = other._height;
   
   return *this;
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target>
TypeSize <Target> &  TypeSize <Target>::operator = (const opa::Size & value)
{
   _width = value._width;
   _height = value._height;
   
   return *this;
}



/*
==============================================================================
Name : operator opa::Size
==============================================================================
*/

template <class Target>
TypeSize <Target>::operator opa::Size () const
{
   opa::Size ret_val (_width, _height);
   
   return ret_val;
}



/*
==============================================================================
Name : init
==============================================================================
*/

template <class Target>
void  TypeSize <Target>::init (const opa::Size & value)
{
   _width.init (value._width);
   _height.init (value._height);
}



/*
==============================================================================
Name : init
==============================================================================
*/

template <class Target>
void  TypeSize <Target>::init (float width, float height)
{
   _width.init (width);
   _height.init (height);
}



/*
==============================================================================
Name : push
==============================================================================
*/

template <class Target>
TypeSize <Target> &  TypeSize <Target>::push (const opa::Size & value, double transition_time, TransitionFunction transition_function)
{
   _width.push (value._width, transition_time, transition_function);
   _height.push (value._height, transition_time, transition_function);
   
   return *this;
}



/*
==============================================================================
Name : push_speed
==============================================================================
*/

template <class Target>
TypeSize <Target> &  TypeSize <Target>::push_speed (const opa::Size & value, const opa::Size & abs_dv_per_sec, TransitionFunction transition_function)
{
   _width.push_speed (value._width, abs_dv_per_sec._width, transition_function);
   _height.push_speed (value._height, abs_dv_per_sec._height, transition_function);
   
   return *this;
}



/*
==============================================================================
Name : loop
==============================================================================
*/

template <class Target>
TypeSize <Target> &  TypeSize <Target>::loop ()
{
   _width.loop ();
   _height.loop ();
   
   return *this;
}



/*
==============================================================================
Name : stop
==============================================================================
*/

template <class Target>
TypeSize <Target> &  TypeSize <Target>::stop ()
{
   _width.stop ();
   _height.stop ();
   
   return *this;
}



/*
==============================================================================
Name : set_transition_time
==============================================================================
*/

template <class Target>
void  TypeSize <Target>::set_transition_time (double delay_s)
{
   _width.set_transition_time (delay_s);
   _height.set_transition_time (delay_s);
}



/*
==============================================================================
Name : set_transition_function
==============================================================================
*/

template <class Target>
void  TypeSize <Target>::set_transition_function (TransitionFunction function)
{
   _width.set_transition_function (function);
   _height.set_transition_function (function);
}



/*
==============================================================================
Name : do_stop
==============================================================================
*/

template <class Target>
void  TypeSize <Target>::do_stop ()
{
   _width.do_stop ();
   _height.do_stop ();
}



/*
==============================================================================
Name : is_animating
==============================================================================
*/

template <class Target>
bool  TypeSize <Target>::is_animating () const
{
   return (_width.is_animating () || _height.is_animating ());
}



/*
==============================================================================
Name : get_cur_value
==============================================================================
*/

template <class Target>
opa::Size   TypeSize <Target>::get_cur_value () const
{
   opa::Size ret_val (_width.get_cur_value (), _height.get_cur_value ());
   
   return ret_val;
}



/*
==============================================================================
Name : get_prev_value
==============================================================================
*/

template <class Target>
opa::Size   TypeSize <Target>::get_prev_value () const
{
   opa::Size ret_val (_width.get_prev_value (), _height.get_prev_value ());
   
   return ret_val;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



#endif   // ohm_opak_TypeSize_CODEHEADER_INCLUDED

#undef ohm_opak_TypeSize_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
