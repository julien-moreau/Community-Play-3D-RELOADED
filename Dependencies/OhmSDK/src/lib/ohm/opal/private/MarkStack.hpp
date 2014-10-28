/*****************************************************************************

        MarkStack.hpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 52274

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



#if defined (ohm_opal_MarkStack_CURRENT_CODEHEADER)
   #error Recursive inclusion of MarkStack code header.
#endif
#define  ohm_opal_MarkStack_CURRENT_CODEHEADER

#if ! defined (ohm_opal_MarkStack_CODEHEADER_INCLUDED)
#define  ohm_opal_MarkStack_CODEHEADER_INCLUDED



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

template <class FrameType>
void  MarkStack::set (archi::UInt32 mark_id)
{
   /*
   ISO/IEC 14882:1998
   5.2.8 [expr.typeid]
   "The result of a typeid expression is an lvalue of static type [...]"
   "The lifetime of the object referred to by the lvalue extends to the
   end of the program."
   */
   
   const std::type_info * info_ptr = &typeid (FrameType);
   
   set (info_ptr, mark_id);
}



/*
==============================================================================
Name : has
Description :
==============================================================================
*/

template <class FrameType>
bool  MarkStack::has (archi::UInt32 mark_id) const
{
   const std::type_info * info_ptr = &typeid (FrameType);
   
   TypeInfoIdArr::const_reverse_iterator it = _type_info_id_arr.rbegin ();
   const TypeInfoIdArr::const_reverse_iterator it_end = _type_info_id_arr.rend ();
   
   for (; it != it_end ; ++it)
   {
      const TypeInfoId & type_info_id = *it;
      
      if ((type_info_id._info_ptr == info_ptr) && (type_info_id._id == mark_id))
      {
         return true;
      }
   }
   
   return false;
}



/*
==============================================================================
Name : find
Description :
==============================================================================
*/

template <class FrameType>
size_t   MarkStack::find (archi::UInt32 mark_id) const
{
   /*
   ISO/IEC 14882:1998
   5.2.8 [expr.typeid]
   "The result of a typeid expression is an lvalue of static type [...]"
   "The lifetime of the object referred to by the lvalue extends to the
   end of the program."
   */
   
   const std::type_info * info_ptr = &typeid (FrameType);
   
   TypeInfoIdArr::const_reverse_iterator it = _type_info_id_arr.rbegin ();
   const TypeInfoIdArr::const_reverse_iterator it_end = _type_info_id_arr.rend ();
   
   for (; it != it_end ; ++it)
   {
      const TypeInfoId & type_info_id = *it;
      
      if ((type_info_id._info_ptr == info_ptr) && (type_info_id._id == mark_id))
      {
         return type_info_id._stack_size;
      }
   }
   
   assert (false);
   return 0;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



#endif   // ohm_opal_MarkStack_CODEHEADER_INCLUDED

#undef ohm_opal_MarkStack_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
