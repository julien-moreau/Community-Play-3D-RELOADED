/*****************************************************************************

        WidgetDialog.hpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
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



#if defined (ohm_opa_WidgetDialog_CURRENT_CODEHEADER)
   #error Recursive inclusion of WidgetDialog code header.
#endif
#define  ohm_opa_WidgetDialog_CURRENT_CODEHEADER

#if ! defined (ohm_opa_WidgetDialog_CODEHEADER_INCLUDED)
#define  ohm_opa_WidgetDialog_CODEHEADER_INCLUDED



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

template <class T, void (T::*handle_event) (WidgetDialog::Event &)>
void  WidgetDialog::bind (T & receiver)
{
   _delegate.param_ref_set <T, Event, handle_event> (receiver);
}



/*
==============================================================================
Name : set_header
==============================================================================
*/

template <class T>
mem::SharedPtr <T>   WidgetDialog::set_header ()
{
   mem::SharedPtr <T> widget_sptr = T::create ();
   
   set_header (widget_sptr);
   
   return widget_sptr;
}



/*
==============================================================================
Name : set_header
==============================================================================
*/

template <class T>
void  WidgetDialog::set_header (mem::SharedPtr <T> widget_sptr)
{
   _header_widget_sptr.assign_dynamic (widget_sptr);
   
   embed (*widget_sptr, Slot_HEADER);
   
   invalidate_layout ();
}



/*
==============================================================================
Name : set_content
==============================================================================
*/

template <class T>
mem::SharedPtr <T>   WidgetDialog::set_content ()
{
   mem::SharedPtr <T> widget_sptr = T::create ();
   
   set_content (widget_sptr);
   
   return widget_sptr;
}



/*
==============================================================================
Name : set_content
==============================================================================
*/

template <class T>
void  WidgetDialog::set_content (mem::SharedPtr <T> widget_sptr)
{
   _content_widget_sptr.assign_dynamic (widget_sptr);
   
   embed (*widget_sptr, Slot_CONTENT);
   
   invalidate_layout ();
}



/*
==============================================================================
Name : set_footer
==============================================================================
*/

template <class T>
mem::SharedPtr <T>   WidgetDialog::set_footer ()
{
   mem::SharedPtr <T> widget_sptr = T::create ();
   
   set_footer (widget_sptr);
   
   return widget_sptr;
}



/*
==============================================================================
Name : set_footer
==============================================================================
*/

template <class T>
void  WidgetDialog::set_footer (mem::SharedPtr <T> widget_sptr)
{
   _footer_widget_sptr.assign_dynamic (widget_sptr);
   
   embed (*widget_sptr, Slot_FOOTER);
   
   invalidate_layout ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opa
}  // namespace ohm



#endif   // ohm_opa_WidgetDialog_CODEHEADER_INCLUDED

#undef ohm_opa_WidgetDialog_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
