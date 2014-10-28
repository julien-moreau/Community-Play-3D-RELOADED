/*****************************************************************************

        WidgetDiscloser.hpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59547

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



#if defined (ohm_opa_WidgetDiscloser_CURRENT_CODEHEADER)
   #error Recursive inclusion of WidgetDiscloser code header.
#endif
#define  ohm_opa_WidgetDiscloser_CURRENT_CODEHEADER

#if ! defined (ohm_opa_WidgetDiscloser_CODEHEADER_INCLUDED)
#define  ohm_opa_WidgetDiscloser_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : set
==============================================================================
*/

template <class T>
mem::SharedPtr <T>   WidgetDiscloser::set ()
{
   mem::SharedPtr <T> widget_sptr = T::create ();
   
   set (widget_sptr);
   
   return widget_sptr;
}



/*
==============================================================================
Name : set
==============================================================================
*/

template <class T>
void  WidgetDiscloser::set (mem::SharedPtr <T> widget_sptr)
{
   WidgetSPtr widget_base_sptr;
   widget_base_sptr.assign_dynamic (widget_sptr);
   
   if (widget_base_sptr.get () != _widget_sptr.get ())
   {
      if (_widget_sptr.get () != 0)
      {
         unembed (*_widget_sptr);
      }
      
      _widget_sptr.assign_dynamic (widget_sptr);
      
      embed (*_widget_sptr);
      
      invalidate_layout ();
   }
}



/*
==============================================================================
Name : get
==============================================================================
*/

template <class T>
T &   WidgetDiscloser::get ()
{
   WidgetBase * widget_ptr = _widget_sptr.get ();
   assert (widget_ptr != 0);
   
   T * cast_widget_ptr = dynamic_cast <T *> (widget_ptr);
   assert (cast_widget_ptr != 0);
   
   return *cast_widget_ptr;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opa
}  // namespace ohm



#endif   // ohm_opa_WidgetDiscloser_CODEHEADER_INCLUDED

#undef ohm_opa_WidgetDiscloser_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
