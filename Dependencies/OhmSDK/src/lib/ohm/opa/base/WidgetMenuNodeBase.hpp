/*****************************************************************************

        WidgetMenuNodeBase.hpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 38424

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



#if defined (ohm_opa_WidgetMenuNodeBase_CURRENT_CODEHEADER)
   #error Recursive inclusion of WidgetMenuNodeBase code header.
#endif
#define  ohm_opa_WidgetMenuNodeBase_CURRENT_CODEHEADER

#if ! defined (ohm_opa_WidgetMenuNodeBase_CODEHEADER_INCLUDED)
#define  ohm_opa_WidgetMenuNodeBase_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : push
==============================================================================
*/

template <class T>
mem::SharedPtr <T>   WidgetMenuNodeBase::push ()
{
   mem::SharedPtr <T> widget_sptr = T::create ();
   
   push (widget_sptr);
   
   return widget_sptr;
}



/*
==============================================================================
Name : push
==============================================================================
*/

template <class T>
void  WidgetMenuNodeBase::push (mem::SharedPtr <T> widget_sptr)
{
   _widget_sptr_list.push_back (
      mem::shared_dynamic_cast <WidgetMenuBase, T> (widget_sptr)
   );
   
   embed (*widget_sptr);
   
   invalidate_layout ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opa
}  // namespace ohm



#endif   // ohm_opa_WidgetMenuNodeBase_CODEHEADER_INCLUDED

#undef ohm_opa_WidgetMenuNodeBase_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
