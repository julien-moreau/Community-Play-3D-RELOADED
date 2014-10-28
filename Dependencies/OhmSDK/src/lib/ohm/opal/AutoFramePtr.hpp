/*****************************************************************************

        AutoFramePtr.hpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 49464

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



#if defined (ohm_opal_AutoFramePtr_CURRENT_CODEHEADER)
   #error Recursive inclusion of AutoFramePtr code header.
#endif
#define  ohm_opal_AutoFramePtr_CURRENT_CODEHEADER

#if ! defined (ohm_opal_AutoFramePtr_CODEHEADER_INCLUDED)
#define  ohm_opal_AutoFramePtr_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

template <class T>
AutoFramePtr <T>::AutoFramePtr ()
:  _frame_ptr (0)
{
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

template <class T>
AutoFramePtr <T>::AutoFramePtr (T * frame_ptr)
:  _frame_ptr (frame_ptr)
{
   // frame_ptr can be null
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class T>
AutoFramePtr <T>::~AutoFramePtr ()
{
   reset (0);
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T>
AutoFramePtr <T> &   AutoFramePtr <T>::operator = (T * frame_ptr)
{
   reset (frame_ptr);
   
   return *this;
}



/*
==============================================================================
Name : operator ->
==============================================================================
*/

template <class T>
T *   AutoFramePtr <T>::operator -> ()
{
   assert (_frame_ptr != 0);
   
   return _frame_ptr;
}



/*
==============================================================================
Name : operator ->
==============================================================================
*/

template <class T>
const T *   AutoFramePtr <T>::operator -> () const
{
   assert (_frame_ptr != 0);
   
   return _frame_ptr;
}



/*
==============================================================================
Name : operator *
==============================================================================
*/

template <class T>
T &   AutoFramePtr <T>::operator * ()
{
   assert (_frame_ptr != 0);
   
   return *_frame_ptr;
}



/*
==============================================================================
Name : operator *
==============================================================================
*/

template <class T>
const T &   AutoFramePtr <T>::operator * () const
{
   assert (_frame_ptr != 0);
   
   return *_frame_ptr;
}



/*
==============================================================================
Name : get
==============================================================================
*/

template <class T>
T *   AutoFramePtr <T>::get ()
{
   // _frame_ptr can be null
   
   return _frame_ptr;
}



/*
==============================================================================
Name : get
==============================================================================
*/

template <class T>
const T *   AutoFramePtr <T>::get () const
{
   // _frame_ptr can be null
   
   return _frame_ptr;
}



/*
==============================================================================
Name : reset
==============================================================================
*/

template <class T>
void  AutoFramePtr <T>::reset (T * frame_ptr)
{
   if (_frame_ptr != frame_ptr)
   {
      if (_frame_ptr != 0)
      {
         _frame_ptr->remove_from_parents ();
         _frame_ptr->set_destroyable ();
      }
      
      _frame_ptr = frame_ptr;
   }
}



/*
==============================================================================
Name : take_out
==============================================================================
*/

template <class T>
void  AutoFramePtr <T>::take_out (T * & frame_ptr)
{
   if (frame_ptr != 0)
   {
      frame_ptr->remove_from_parents ();
      frame_ptr->set_destroyable ();
      
      frame_ptr = 0;
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



#endif   // ohm_opal_AutoFramePtr_CODEHEADER_INCLUDED

#undef ohm_opal_AutoFramePtr_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
