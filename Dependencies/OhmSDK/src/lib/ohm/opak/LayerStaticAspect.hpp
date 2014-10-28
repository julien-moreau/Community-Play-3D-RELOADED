/*****************************************************************************

        LayerStaticAspect.hpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 41076

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



#if defined (ohm_opak_LayerStaticAspect_CURRENT_CODEHEADER)
   #error Recursive inclusion of LayerStaticAspect code header.
#endif
#define  ohm_opak_LayerStaticAspect_CURRENT_CODEHEADER

#if ! defined (ohm_opak_LayerStaticAspect_CODEHEADER_INCLUDED)
#define  ohm_opak_LayerStaticAspect_CODEHEADER_INCLUDED



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

template <class AspectType>
LayerStaticAspect <AspectType>::LayerStaticAspect ()
:  _static_aspect ()
{
}



/*
==============================================================================
Name : display
==============================================================================
*/

template <class AspectType>
void  LayerStaticAspect <AspectType>::display (GraphicContext & context)
{
   context.save ();
   
   opa::Point origin = _origin.get_cur_value ();
   
   context.translate (origin._x, origin._y);

   context.set (&_static_aspect);
   
   display_children (context);
   
   context.restore ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class AspectType>
LayerStaticAspect <AspectType>::~LayerStaticAspect ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



#endif   // ohm_opak_LayerStaticAspect_CODEHEADER_INCLUDED

#undef ohm_opak_LayerStaticAspect_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
