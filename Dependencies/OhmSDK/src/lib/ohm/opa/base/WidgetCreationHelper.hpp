/*****************************************************************************

        WidgetCreationHelper.hpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 38302

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



#if defined (ohm_opa_WidgetCreationHelper_CURRENT_CODEHEADER)
   #error Recursive inclusion of WidgetCreationHelper code header.
#endif
#define  ohm_opa_WidgetCreationHelper_CURRENT_CODEHEADER

#if ! defined (ohm_opa_WidgetCreationHelper_CODEHEADER_INCLUDED)
#define  ohm_opa_WidgetCreationHelper_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
WidgetCreationHelper <T>::WidgetCreationHelper ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class T>
WidgetCreationHelper <T>::~WidgetCreationHelper ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class T>
mem::SharedPtr <T>   WidgetCreationHelper <T>::create ()
{
   mem::SharedPtr <T> ret_val_sptr
      = mem::SharedPtr <T> (new T);
   
   return ret_val_sptr;
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

/*template <class T>
mem::SharedPtr <T>   WidgetCreationHelper <T>::create (opa::Bundle & parent_bundle, size_t slot_nbr)
{
   assert (&parent_bundle != 0);
   
   mem::SharedPtr <T> ret_val_sptr
      = mem::SharedPtr <T> (new T);
   
   parent_bundle.embed (*ret_val_sptr, slot_nbr);
   
   return ret_val_sptr;
}*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opa
}  // namespace ohm



#endif   // ohm_opa_WidgetCreationHelper_CODEHEADER_INCLUDED

#undef ohm_opa_WidgetCreationHelper_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
