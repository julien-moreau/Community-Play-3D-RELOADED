/*****************************************************************************

        Event.hpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 47676

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



#if defined (ohm_opal_Event_CURRENT_CODEHEADER)
   #error Recursive inclusion of Event code header.
#endif
#define  ohm_opal_Event_CURRENT_CODEHEADER

#if ! defined (ohm_opal_Event_CODEHEADER_INCLUDED)
#define  ohm_opal_Event_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <typeinfo>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : has_mark
==============================================================================
*/

template <class FrameType>
bool  Event::has_mark (archi::UInt32 mark_id) const
{
   return _mark_stack.has <FrameType> (mark_id);
}



/*
==============================================================================
Name : convert_to_mark
==============================================================================
*/

template <class FrameType>
void  Event::convert_to_mark (opa::Point & pt, archi::UInt32 mark_id) const
{
   assert (_state_arr.size () > 0);
   
   const State & state = _state_arr.back ();
   
   state._affine_transform.apply (pt);
   
   size_t pos = _mark_stack.find <FrameType> (mark_id);
   
   const State & mark_state = _state_arr [pos];
   mark_state._affine_transform.apply_inverse (pt);
}



/*
==============================================================================
Name : convert_to_mark
==============================================================================
*/

template <class FrameType>
void  Event::convert_to_mark (opa::Size & sz, archi::UInt32 mark_id) const
{
   assert (_state_arr.size () > 0);
   
   const State & state = _state_arr.back ();
   
   state._affine_transform.apply (sz);
   
   size_t pos = _mark_stack.find <FrameType> (mark_id);
   
   const State & mark_state = _state_arr [pos];
   mark_state._affine_transform.apply_inverse (sz);
}



/*
==============================================================================
Name : convert_from_mark
==============================================================================
*/

template <class FrameType>
void  Event::convert_from_mark (opa::Point & pt, archi::UInt32 mark_id) const
{
   assert (_state_arr.size () > 0);
   
   size_t pos = _mark_stack.find <FrameType> (mark_id);

   const State & mark_state = _state_arr [pos];
   mark_state._affine_transform.apply (pt);
   
   const State & state = _state_arr.back ();
   
   state._affine_transform.apply_inverse (pt);
}



/*
==============================================================================
Name : convert_from_mark
==============================================================================
*/

template <class FrameType>
void  Event::convert_from_mark (opa::Size & sz, archi::UInt32 mark_id) const
{
   assert (_state_arr.size () > 0);
   
   size_t pos = _mark_stack.find <FrameType> (mark_id);

   const State & mark_state = _state_arr [pos];
   mark_state._affine_transform.apply (sz);
   
   const State & state = _state_arr.back ();
   
   state._affine_transform.apply_inverse (sz);
}



/*
==============================================================================
Name : mark
==============================================================================
*/

template <class FrameType>
void  Event::mark (archi::UInt32 mark_id)
{
   _mark_stack.set <FrameType> (mark_id);
}



/*
==============================================================================
Name : set
==============================================================================
*/

template <class ConductType>
void  Event::set (ConductType * conduct_ptr)
{
   assert (conduct_ptr != 0);
   
   if (conduct_ptr->is_spatial ())
   {
      conduct_ptr->bind (*this);
   }

   _conduct_stack.set (conduct_ptr);
}



/*
==============================================================================
Name : get
==============================================================================
*/

template <class ConductType>
ConductType *  Event::get ()
{
   return _conduct_stack.get <ConductType> ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name :
==============================================================================
*/



}  // namespace opal
}  // namespace ohm



#endif   // ohm_opal_Event_CODEHEADER_INCLUDED

#undef ohm_opal_Event_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
