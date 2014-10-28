/*****************************************************************************

        Bundle.hpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 40396

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



#if defined (ohm_opa_Bundle_CURRENT_CODEHEADER)
   #error Recursive inclusion of Bundle code header.
#endif
#define  ohm_opa_Bundle_CURRENT_CODEHEADER

#if ! defined (ohm_opa_Bundle_CODEHEADER_INCLUDED)
#define  ohm_opa_Bundle_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/




/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : get_ancestor
==============================================================================
*/

template <class T>
T &   Bundle::get_ancestor ()
{
   Bundle * bnd_ptr = this->_parent_ptr;
   
   T * ptr = 0;
   
   while (bnd_ptr != 0)
   {
      // don't forget to #include the ancestor class you're trying to fetch
      ptr = dynamic_cast <T *> (bnd_ptr);
      
      if (ptr != 0)
      {
         break;
      }
      
      bnd_ptr = bnd_ptr->_parent_ptr;
   }
   
   // no 'T' in ancestor chain
   assert (ptr != 0);
   
   return *ptr;
}



/*
==============================================================================
Name : get_ancestor_ptr
==============================================================================
*/

template <class T>
T *   Bundle::get_ancestor_ptr ()
{
   Bundle * bnd_ptr = this->_parent_ptr;
   
   T * ptr = 0;
   
   while (bnd_ptr != 0)
   {
      ptr = dynamic_cast <T *> (bnd_ptr);
      
      if (ptr != 0)
      {
         break;
      }
      
      bnd_ptr = bnd_ptr->_parent_ptr;
   }
   
   // 'ptr' can be null
   
   return ptr;
}



/*
==============================================================================
Name : bind
==============================================================================
*/

template <class T, class EventType, void (T::*handle_event) (EventType &)>
void  Bundle::bind ()
{
   util::Delegate delegate;
   
   T * target_ptr = dynamic_cast <T *> (this);
   assert (target_ptr != 0);
   
   delegate.param_ref_set <T, EventType, handle_event> (*target_ptr);
   
   _delegate_set.insert (delegate);
}



}  // namespace opa
}  // namespace ohm



#endif   // ohm_opa_Bundle_CODEHEADER_INCLUDED

#undef ohm_opa_Bundle_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
