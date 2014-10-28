/*****************************************************************************

        AutoPrimPtr.hpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 36758

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



#if defined (ohm_opak_AutoPrimPtr_CURRENT_CODEHEADER)
   #error Recursive inclusion of AutoPrimPtr code header.
#endif
#define  ohm_opak_AutoPrimPtr_CURRENT_CODEHEADER

#if ! defined (ohm_opak_AutoPrimPtr_CODEHEADER_INCLUDED)
#define  ohm_opak_AutoPrimPtr_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <cassert>



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

template <class T>
AutoPrimPtr <T>::AutoPrimPtr ()
:  _primitive_ptr (0)
{
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

template <class T>
AutoPrimPtr <T>::AutoPrimPtr (T * primitive_ptr)
:  _primitive_ptr (primitive_ptr)
{
   // primitive_ptr can be null
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class T>
AutoPrimPtr <T>::~AutoPrimPtr ()
{
   reset (0);
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T>
AutoPrimPtr <T> & AutoPrimPtr <T>::operator = (T * primitive_ptr)
{
   reset (primitive_ptr);
   
   return *this;
}



/*
==============================================================================
Name : operator ->
==============================================================================
*/

template <class T>
T *   AutoPrimPtr <T>::operator -> ()
{
   assert (_primitive_ptr != 0);
   
   return _primitive_ptr;
}



/*
==============================================================================
Name : operator ->
==============================================================================
*/

template <class T>
const T *   AutoPrimPtr <T>::operator -> () const
{
   assert (_primitive_ptr != 0);
   
   return _primitive_ptr;
}



/*
==============================================================================
Name : operator *
==============================================================================
*/

template <class T>
T &   AutoPrimPtr <T>::operator * ()
{
   assert (_primitive_ptr != 0);
   
   return *_primitive_ptr;
}



/*
==============================================================================
Name : operator *
==============================================================================
*/

template <class T>
const T &   AutoPrimPtr <T>::operator * () const
{
   assert (_primitive_ptr != 0);
   
   return *_primitive_ptr;
}



/*
==============================================================================
Name : get
==============================================================================
*/

template <class T>
T *   AutoPrimPtr <T>::get ()
{
   // _primitive_ptr can be null
   
   return _primitive_ptr;
}



/*
==============================================================================
Name : reset
==============================================================================
*/

template <class T>
void  AutoPrimPtr <T>::reset (T * primitive_ptr)
{
   if (_primitive_ptr != primitive_ptr)
   {
      if (_primitive_ptr != 0)
      {
         _primitive_ptr->remove_from_parents ();
         _primitive_ptr->set_destroyable ();
      }
      
      _primitive_ptr = primitive_ptr;
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



#endif   // ohm_opak_AutoPrimPtr_CODEHEADER_INCLUDED

#undef ohm_opak_AutoPrimPtr_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
