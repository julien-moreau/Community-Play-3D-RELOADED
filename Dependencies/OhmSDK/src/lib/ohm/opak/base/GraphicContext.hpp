/*****************************************************************************

        GraphicContext.hpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54317

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



#if defined (ohm_opak_GraphicContext_CURRENT_CODEHEADER)
   #error Recursive inclusion of GraphicContext code header.
#endif
#define  ohm_opak_GraphicContext_CURRENT_CODEHEADER

#if ! defined (ohm_opak_GraphicContext_CODEHEADER_INCLUDED)
#define  ohm_opak_GraphicContext_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : convert_to_mark
==============================================================================
*/

template <class PrimitiveType>
void  GraphicContext::convert_to_mark (opa::Point & pt, archi::UInt32 mark_id) const
{
   assert (_graphic_state_stack.size () > 0);
   
   const GraphicState & graphic_state = _graphic_state_stack.back ();
   
   graphic_state._affine_transform.apply (pt);
   
   size_t pos = _mark_stack.find <PrimitiveType> (mark_id);
   
   const GraphicState & mark_graphic_state = _graphic_state_stack [pos];
   mark_graphic_state._affine_transform.apply_inverse (pt);
}



/*
==============================================================================
Name : convert_to_mark
==============================================================================
*/

template <class PrimitiveType>
void  GraphicContext::convert_to_mark (opa::Size & sz, archi::UInt32 mark_id) const
{
   assert (_graphic_state_stack.size () > 0);
   
   const GraphicState & graphic_state = _graphic_state_stack.back ();
   
   graphic_state._affine_transform.apply (sz);
   
   size_t pos = _mark_stack.find <PrimitiveType> (mark_id);
   
   const GraphicState & mark_graphic_state = _graphic_state_stack [pos];
   mark_graphic_state._affine_transform.apply_inverse (sz);
}



/*
==============================================================================
Name : convert_from_mark
==============================================================================
*/

template <class PrimitiveType>
void  GraphicContext::convert_from_mark (opa::Point & pt, archi::UInt32 mark_id) const
{
   assert (_graphic_state_stack.size () > 0);
   
   size_t pos = _mark_stack.find <PrimitiveType> (mark_id);

   const GraphicState & mark_graphic_state = _graphic_state_stack [pos];
   mark_graphic_state._affine_transform.apply (pt);
   
   const GraphicState & graphic_state = _graphic_state_stack.back ();
   
   graphic_state._affine_transform.apply_inverse (pt);
}



/*
==============================================================================
Name : convert_from_mark
==============================================================================
*/

template <class PrimitiveType>
void  GraphicContext::convert_from_mark (opa::Size & sz, archi::UInt32 mark_id) const
{
   assert (_graphic_state_stack.size () > 0);
   
   size_t pos = _mark_stack.find <PrimitiveType> (mark_id);

   const GraphicState & mark_graphic_state = _graphic_state_stack [pos];
   mark_graphic_state._affine_transform.apply (sz);
   
   const GraphicState & graphic_state = _graphic_state_stack.back ();
   
   graphic_state._affine_transform.apply_inverse (sz);
}



/*
==============================================================================
Name : get
==============================================================================
*/

template <class AspectType>
const AspectType &   GraphicContext::get () const
{
   const AspectType * aspect_ptr = _aspect_stack.get <AspectType> ();
   assert (aspect_ptr != 0);
   
   return *aspect_ptr;
}



/*
==============================================================================
Name : get_ptr
==============================================================================
*/

template <class AspectType>
const AspectType *   GraphicContext::get_ptr () const
{
   const AspectType * aspect_ptr = _aspect_stack.get <AspectType> ();
   // aspect_ptr might be 0
   
   return aspect_ptr;
}



/*
==============================================================================
Name : get
==============================================================================
*/

template <class AspectType>
AspectType &   GraphicContext::use () const
{
   AspectType * aspect_ptr = _aspect_stack.use <AspectType> ();
   assert (aspect_ptr != 0);
   
   return *aspect_ptr;
}



/*
==============================================================================
Name : set
==============================================================================
*/

template <class AspectType>
void  GraphicContext::set (AspectType & aspect)
{
   _aspect_stack.set (&aspect);
}



/*
==============================================================================
Name : mark
==============================================================================
*/

template <class PrimitiveType>
void  GraphicContext::mark (archi::UInt32 mark_id)
{
   _mark_stack.set <PrimitiveType> (mark_id);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



#endif   // ohm_opak_GraphicContext_CODEHEADER_INCLUDED

#undef ohm_opak_GraphicContext_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
