/*****************************************************************************

        SelectionChangeInfo.hpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70455

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



#if defined (model_SelectionChangeInfo_CURRENT_CODEHEADER)
   #error Recursive inclusion of SelectionChangeInfo code header.
#endif
#define  model_SelectionChangeInfo_CURRENT_CODEHEADER

#if ! defined (model_SelectionChangeInfo_CODEHEADER_INCLUDED)
#define  model_SelectionChangeInfo_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace model
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: set
==============================================================================
*/

template <class T> 
void SelectionChangeInfo::set (int type, const T & data)
{
   assert (type >= 0);
   
   destroy_data ();
   
   assert (_type == -1);
   
   _type = type;
   
   _data_type_info_ptr = &typeid (T);
   
   _data.resize (sizeof (T));
   
   new (&_data [0]) T (data);
   
   _dtor_fnc = &data_destructor <T>;
}



/*
==============================================================================
Name: use_data
==============================================================================
*/

template <class T> 
const T & SelectionChangeInfo::use_data () const
{
   assert (_data_type_info_ptr == &typeid (T));
   assert (_data.size () == sizeof (T));
   
   const T * ptr = reinterpret_cast <const T *> (&_data [0]);
   
   return *ptr;
}



/*
==============================================================================
Name: is_data_of_type
==============================================================================
*/

template <class T> 
bool SelectionChangeInfo::is_data_of_type () const
{
   return (_data_type_info_ptr == &typeid (T));
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: data_destructor
==============================================================================
*/

template <class T> 
void SelectionChangeInfo::data_destructor (SelectionChangeInfo & obj)
{
   assert (obj._data_type_info_ptr == &typeid (T));
   assert (obj._data.size () == sizeof (T));

   const T * ptr = reinterpret_cast <const T *> (&obj._data [0]);
   ptr->~T ();

   obj._data.clear ();
   obj._data_type_info_ptr = 0;
   obj._type = -1;
   obj._dtor_fnc = 0;
}




}  // namespace model



#endif   // model_SelectionChangeInfo_CODEHEADER_INCLUDED

#undef model_SelectionChangeInfo_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

