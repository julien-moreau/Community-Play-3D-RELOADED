/*****************************************************************************

        Primitive.hpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 34733

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



#if defined (ohm_opak_Primitive_CURRENT_CODEHEADER)
   #error Recursive inclusion of Primitive code header.
#endif
#define  ohm_opak_Primitive_CURRENT_CODEHEADER

#if ! defined (ohm_opak_Primitive_CODEHEADER_INCLUDED)
#define  ohm_opak_Primitive_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : get_ancestor
==============================================================================
*/

template <class T>
T *   Primitive::get_ancestor ()
{
   Primitive * primitive_ptr = this;
   
   T * cast_ptr = 0;
   
   while (cast_ptr == 0)
   {
      cast_ptr = dynamic_cast <T *> (primitive_ptr);
      
      if (cast_ptr == 0)
      {
         if (!primitive_ptr->_parent_ptr_arr.empty ())
         {
            primitive_ptr = &primitive_ptr->get_parent ();
         }
         else
         {
            break;
         }
      }
   }
   
   return cast_ptr;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



#endif   // ohm_opak_Primitive_CODEHEADER_INCLUDED

#undef ohm_opak_Primitive_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
