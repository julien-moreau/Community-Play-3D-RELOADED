/*****************************************************************************

        WidgetBase.hpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 47985

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



#if defined (ohm_opa_WidgetBase_CURRENT_CODEHEADER)
   #error Recursive inclusion of WidgetBase code header.
#endif
#define  ohm_opa_WidgetBase_CURRENT_CODEHEADER

#if ! defined (ohm_opa_WidgetBase_CODEHEADER_INCLUDED)
#define  ohm_opa_WidgetBase_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <typeinfo>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : set_data
==============================================================================
*/

template <class T>
void  WidgetBase::set_data (const T & data)
{
   release_data ();
   
   _data_type_info_ptr = &typeid (T);
   
   _data.resize (sizeof (T));
   
   new (&_data [0]) T (data);
   
   _dtor_fnc = &method_dtor <T>;
}



/*
==============================================================================
Name : pop_data
==============================================================================
*/

template <class T>
const T &   WidgetBase::get_data ()
{
   assert (_data_type_info_ptr == &typeid (T));
   assert (_data.size () == sizeof (T));
   
   const T * ptr = reinterpret_cast <const T *> (&_data [0]);
   
   return *ptr;
}



/*
==============================================================================
Name : get_data
==============================================================================
*/

template <class T>
bool  WidgetBase::is_data_of_type () const
{
   return (_data_type_info_ptr == &typeid (T));
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : method_dtor
==============================================================================
*/

template <class T>
void  WidgetBase::method_dtor (void * obj_ptr)
{
   T * cast_ptr = reinterpret_cast <T *> (obj_ptr);
   
   cast_ptr->~T ();
}



}  // namespace opa
}  // namespace ohm



#endif   // ohm_opa_WidgetBase_CODEHEADER_INCLUDED

#undef ohm_opa_WidgetBase_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
