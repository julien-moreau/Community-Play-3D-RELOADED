/*****************************************************************************

        ObjectMold.hpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70989

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



#if defined (ohm_flip_ObjectMold_CURRENT_CODEHEADER)
   #error Recursive inclusion of ObjectMold code header.
#endif
#define  ohm_flip_ObjectMold_CURRENT_CODEHEADER

#if ! defined (ohm_flip_ObjectMold_CODEHEADER_INCLUDED)
#define  ohm_flip_ObjectMold_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : is_of_type
==============================================================================
*/

template <class Type>
bool  ObjectMold::is_of_type () const
{
   assert (!_data_ptr_list.empty ());
   
   ObjectMoldData & data = *_data_ptr_list.front ();
   
   return data.is_of_type <Type> ();
}



/*
==============================================================================
Name : is_of_type
==============================================================================
*/

template <class Type>
ObjectMoldData &  ObjectMold::get () const
{
   ObjectMoldDataPtrList::const_iterator it = _data_ptr_list.begin ();
   const ObjectMoldDataPtrList::const_iterator it_end = _data_ptr_list.end ();
   
   
   for (; it != it_end ; ++it)
   {
      ObjectMoldData & data = **it;
      
      if (data.is_of_type <Type> ())
      {
         return data;
      }
   }
   
   // not found
   assert (false);
#if ! defined (ohm_flip_SERVER)
   ohm_lang_CRASH ();
#endif

   return (*(ObjectMoldData *)(0));
}



/*
==============================================================================
Name : reset_data
Note :
   ptr might be null
==============================================================================
*/

template <class T>
void  ObjectMold::reset_data (T * ptr)
{
   return _id_ptr_map.set <T> (ptr);
}



/*
==============================================================================
Name : get_data
==============================================================================
*/

template <class T>
T *   ObjectMold::get_data () const
{
   return _id_ptr_map.get <T> ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



#endif   // ohm_flip_ObjectMold_CODEHEADER_INCLUDED

#undef ohm_flip_ObjectMold_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
