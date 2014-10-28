/*****************************************************************************

        TypeColor.hpp
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



#if defined (ohm_opak_TypeColor_CURRENT_CODEHEADER)
   #error Recursive inclusion of TypeColor code header.
#endif
#define  ohm_opak_TypeColor_CURRENT_CODEHEADER

#if ! defined (ohm_opak_TypeColor_CODEHEADER_INCLUDED)
#define  ohm_opak_TypeColor_CODEHEADER_INCLUDED



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
TypeColor <Target>::TypeColor (Target & target, Proc proc)
:  _a (target, proc)
,  _r (target, proc)
,  _g (target, proc)
,  _b (target, proc)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class Target>
TypeColor <Target>::~TypeColor ()
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
void  TypeColor <Target>::enable_stabilization ()
{
   _a.enable_stabilization ();
   _r.enable_stabilization ();
   _g.enable_stabilization ();
   _b.enable_stabilization ();
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target>
TypeColor <Target> & TypeColor <Target>::operator = (const TypeColor &other)
{
   _a = other._a;
   _r = other._r;
   _g = other._g;
   _b = other._b;
   
   return *this;
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Target>
TypeColor <Target> & TypeColor <Target>::operator = (const opal::Color & value)
{
   _a = value._a;
   _r = value._r;
   _g = value._g;
   _b = value._b;
   
   return *this;
}



/*
==============================================================================
Name : operator opal::Color
==============================================================================
*/

template <class Target>
TypeColor <Target>::operator opal::Color () const
{
   opal::Color ret_val;
   ret_val._a = _a;
   ret_val._r = _r;
   ret_val._g = _g;
   ret_val._b = _b;
   
   return ret_val;
}



/*
==============================================================================
Name : init
==============================================================================
*/

template <class Target>
void  TypeColor <Target>::init (const opal::Color & value)
{
   _a.init (value._a);
   _r.init (value._r);
   _g.init (value._g);
   _b.init (value._b);
}



/*
==============================================================================
Name : init
==============================================================================
*/

template <class Target>
void  TypeColor <Target>::init (float a, float r, float g, float b)
{
   _a.init (a);
   _r.init (r);
   _g.init (g);
   _b.init (b);
}



/*
==============================================================================
Name : push
==============================================================================
*/

template <class Target>
TypeColor <Target> & TypeColor <Target>::push (const opal::Color & value, double transition_time, TransitionFunction transition_function)
{
   _a.push (value._a, transition_time, transition_function);
   _r.push (value._r, transition_time, transition_function);
   _g.push (value._g, transition_time, transition_function);
   _b.push (value._b, transition_time, transition_function);
   
   return *this;
}



/*
==============================================================================
Name : loop
==============================================================================
*/

template <class Target>
TypeColor <Target> & TypeColor <Target>::loop ()
{
   _a.loop ();
   _r.loop ();
   _g.loop ();
   _b.loop ();
   
   return *this;
}



/*
==============================================================================
Name : stop
==============================================================================
*/

template <class Target>
TypeColor <Target> & TypeColor <Target>::stop ()
{
   _a.stop ();
   _r.stop ();
   _g.stop ();
   _b.stop ();
   
   return *this;
}



/*
==============================================================================
Name : set_transition_time
==============================================================================
*/

template <class Target>
void  TypeColor <Target>::set_transition_time (double delay_s)
{
   _a.set_transition_time (delay_s);
   _r.set_transition_time (delay_s);
   _g.set_transition_time (delay_s);
   _b.set_transition_time (delay_s);
}



/*
==============================================================================
Name : set_transition_function
==============================================================================
*/

template <class Target>
void  TypeColor <Target>::set_transition_function (TransitionFunction function)
{
   _a.set_transition_function (function);
   _r.set_transition_function (function);
   _g.set_transition_function (function);
   _b.set_transition_function (function);
}



/*
==============================================================================
Name : do_stop
==============================================================================
*/

template <class Target>
void  TypeColor <Target>::do_stop ()
{
   _a.do_stop ();
   _r.do_stop ();
   _g.do_stop ();
   _b.do_stop ();
}



/*
==============================================================================
Name : is_animating
==============================================================================
*/

template <class Target>
bool  TypeColor <Target>::is_animating () const
{
   return (
      _a.is_animating ()
      || _r.is_animating ()
      || _g.is_animating ()
      || _b.is_animating ()
   );
}



/*
==============================================================================
Name : get_cur_value
==============================================================================
*/

template <class Target>
opal::Color TypeColor <Target>::get_cur_value () const
{
   opal::Color ret_val;
   ret_val._a = _a.get_cur_value ();
   ret_val._r = _r.get_cur_value ();
   ret_val._g = _g.get_cur_value ();
   ret_val._b = _b.get_cur_value ();
   
   return ret_val;
}



/*
==============================================================================
Name : get_prev_value
==============================================================================
*/

template <class Target>
opal::Color TypeColor <Target>::get_prev_value () const
{
   opal::Color ret_val;
   ret_val._a = _a.get_prev_value ();
   ret_val._r = _r.get_prev_value ();
   ret_val._g = _g.get_prev_value ();
   ret_val._b = _b.get_prev_value ();
   
   return ret_val;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



#endif   // ohm_opak_TypeColor_CODEHEADER_INCLUDED

#undef ohm_opak_TypeColor_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
