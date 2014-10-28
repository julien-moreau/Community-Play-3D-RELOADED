/*****************************************************************************

        ObjectRef.hpp
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



#if defined (model_ObjectRef_CURRENT_CODEHEADER)
   #error Recursive inclusion of ObjectRef code header.
#endif
#define  model_ObjectRef_CURRENT_CODEHEADER

#if ! defined (model_ObjectRef_CODEHEADER_INCLUDED)
#define  model_ObjectRef_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace model
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: get_object_ptr
==============================================================================
*/

template <class T>
T * ObjectRef::get_object_ptr () const
{
   ohm::flip::Ref ref = 0;
   
   if (was_restored ())
   {
      ref = _object_ref.get_old_ref ();
   }
   else
   {
      ref = _object_ref.get ();
   }

   T * obj_ptr = use_document ().template get_object <T> (ref);
   
   return obj_ptr;   
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace model



#endif   // model_ObjectRef_CODEHEADER_INCLUDED

#undef model_ObjectRef_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

