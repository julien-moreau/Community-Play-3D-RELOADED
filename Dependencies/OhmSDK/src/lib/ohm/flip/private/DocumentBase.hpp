/*****************************************************************************

        DocumentBase.hpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 36322

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



#if defined (ohm_flip_DocumentBase_CURRENT_CODEHEADER)
   #error Recursive inclusion of DocumentBase code header.
#endif
#define  ohm_flip_DocumentBase_CURRENT_CODEHEADER

#if ! defined (ohm_flip_DocumentBase_CODEHEADER_INCLUDED)
#define  ohm_flip_DocumentBase_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : get_root
==============================================================================
*/

template <class T>
T &   DocumentBase::get_root ()
{
   T * root_ptr = dynamic_cast <T*> (use_object_manager ().get_root ());
   assert (root_ptr != 0);
   
   return (*root_ptr);
}



/*
==============================================================================
Name : get_object
==============================================================================
*/

template <class T>
T *   DocumentBase::get_object (flip::Ref ref)
{
   T * ret_ptr = dynamic_cast <T*> (use_object_manager ().get_object (ref));
   
   return (ret_ptr);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace flip
}  // namespace ohm



#endif   // ohm_flip_DocumentBase_CODEHEADER_INCLUDED

#undef ohm_flip_DocumentBase_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

