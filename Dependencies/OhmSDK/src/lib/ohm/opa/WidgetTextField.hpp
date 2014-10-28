/*****************************************************************************

        WidgetTextField.hpp
        Copyright (c) 2009 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 40388

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



#if defined (ohm_opa_WidgetTextField_CURRENT_CODEHEADER)
   #error Recursive inclusion of WidgetTextField code header.
#endif
#define  ohm_opa_WidgetTextField_CURRENT_CODEHEADER

#if ! defined (ohm_opa_WidgetTextField_CODEHEADER_INCLUDED)
#define  ohm_opa_WidgetTextField_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : bind
==============================================================================
*/

template <class T, void (T::*handle_event) (WidgetTextField::Event &)>
void WidgetTextField::bind (T & receiver)
{
   _delegate.param_ref_set <T, Event, handle_event> (receiver);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opa
}  // namespace ohm



#endif   // ohm_opa_WidgetTextField_CODEHEADER_INCLUDED

#undef ohm_opa_WidgetTextField_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

