/*****************************************************************************

        ConductStack.hpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 38817

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



#if defined (ohm_opal_ConductStack_CURRENT_CODEHEADER)
   #error Recursive inclusion of ConductStack code header.
#endif
#define  ohm_opal_ConductStack_CURRENT_CODEHEADER

#if ! defined (ohm_opal_ConductStack_CODEHEADER_INCLUDED)
#define  ohm_opal_ConductStack_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : set
Description :
==============================================================================
*/

template <class ConductType>
void  ConductStack::set (ConductType * conduct_ptr)
{
   /*
   ISO/IEC 14882:1998
   5.2.8 [expr.typeid]
   "The result of a typeid expression is an lvalue of static type [...]"
   "The lifetime of the object referred to by the lvalue extends to the
   end of the program."
   */
   
   const std::type_info * info_ptr = &typeid (ConductType);
   
   TypeInfoPtr type_info_ptr;
   type_info_ptr._info_ptr = info_ptr;
   type_info_ptr._conduct_ptr = conduct_ptr;
   
   _type_info_ptr_arr.push_back (type_info_ptr);
   
   if (conduct_ptr->is_spatial ())
   {
      assert (!_pos_flag_arr.empty ());
      _pos_flag_arr.back ()._spatial_flag = true;
      
      conduct_ptr->invalidate ();
   }
}



/*
==============================================================================
Name : get
Description :
==============================================================================
*/

template <class ConductType>
ConductType *  ConductStack::get ()
{
   /*
   ISO/IEC 14882:1998
   5.2.8 [expr.typeid]
   "The result of a typeid expression is an lvalue of static type [...]"
   "The lifetime of the object referred to by the lvalue extends to the
   end of the program."
   */
   
   const std::type_info * info_ptr = &typeid (ConductType);
   
   TypeInfoPtrArr::reverse_iterator it = _type_info_ptr_arr.rbegin ();
   const TypeInfoPtrArr::reverse_iterator it_end = _type_info_ptr_arr.rend ();
   
   ConductType * conduct_ptr = 0;
   
   for (; it != it_end ; ++it)
   {
      TypeInfoPtr & type_info_ptr = *it;
      
      if (type_info_ptr._info_ptr == info_ptr)
      {
         conduct_ptr = reinterpret_cast <ConductType *> (type_info_ptr._conduct_ptr);
         
         break;
      }
   }
   
   return conduct_ptr;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



#endif   // ohm_opal_ConductStack_CODEHEADER_INCLUDED

#undef ohm_opal_ConductStack_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
