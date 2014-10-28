/*****************************************************************************

        IdPtrMapSmall.hpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 45093

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



#if defined (ohm_flip_IdPtrMapSmall_CURRENT_CODEHEADER)
   #error Recursive inclusion of IdPtrMapSmall code header.
#endif
#define  ohm_flip_IdPtrMapSmall_CURRENT_CODEHEADER

#if ! defined (ohm_flip_IdPtrMapSmall_CODEHEADER_INCLUDED)
#define  ohm_flip_IdPtrMapSmall_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <typeinfo>

#include <cstring>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : set
Note :
   Set a pointer 'ptr' with 'typeid(T)' key
   If the key exists, its value is replaced with the new 'ptr'
   'ptr' may be null
==============================================================================
*/

template <class T>
void  IdPtrMapSmall::set (T * ptr)
{
   /*
   ISO/IEC 14882:1998
   5.2.8 [expr.typeid]
   "The result of a typeid expression is an lvalue of static type [...]"
   "The lifetime of the object referred to by the lvalue extends to the
   end of the program."
   */
   
   const std::type_info * type_info_ptr = &typeid (T);
   
   if (_size > 0)
   {
      for (size_t i = 0 ; i < _size ; ++i)
      {
         IdPtr & id_ptr = _data_ptr [i];
         
         if (id_ptr._type_info_ptr == type_info_ptr)
         {
            id_ptr._ptr = ptr;
            
            return;
         }
      }

      using namespace std;
      
      IdPtr * new_data_ptr = new IdPtr [_size + 1];
      
      memcpy (new_data_ptr, _data_ptr, _size * sizeof (IdPtr));
      
      delete [] _data_ptr;

      _data_ptr = new_data_ptr;
      ++_size;
   }
   else
   {
      _data_ptr = new IdPtr [1];
      _size = 1;
   }
   
   IdPtr & id_ptr = _data_ptr [_size - 1];

   id_ptr._type_info_ptr = type_info_ptr;
   id_ptr._ptr = ptr;
}



/*
==============================================================================
Name : get
Note :
   Complexity is linear time
   Returns value may be null if the key does not exists, or if it was
   resetted explicitely to '0'
==============================================================================
*/

template <class T>
T *   IdPtrMapSmall::get () const
{
   const std::type_info * type_info_ptr = &typeid (T);
   
   if (_size > 0)
   {
      for (size_t i = 0 ; i < _size ; ++i)
      {
         const IdPtr & id_ptr = _data_ptr [i];
         
         if (id_ptr._type_info_ptr == type_info_ptr)
         {
            // maybe null
            return reinterpret_cast <T *> (id_ptr._ptr);
         }
      }
      
      return 0;
   }
   else
   {
      return 0;
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



#endif   // ohm_flip_IdPtrMapSmall_CODEHEADER_INCLUDED

#undef ohm_flip_IdPtrMapSmall_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
