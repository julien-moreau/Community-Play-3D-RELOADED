/*****************************************************************************

        MenuRoot.hpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 49504

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



#if defined (ohm_opal_MenuRoot_CURRENT_CODEHEADER)
   #error Recursive inclusion of MenuRoot code header.
#endif
#define  ohm_opal_MenuRoot_CURRENT_CODEHEADER

#if ! defined (ohm_opal_MenuRoot_CODEHEADER_INCLUDED)
#define  ohm_opal_MenuRoot_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : bind_pre
==============================================================================
*/

template <class T, void (T::*pre_proc) ()>
void  MenuRoot::bind_pre (T & receiver)
{
   _pre_delegate.set <T, pre_proc> (receiver);
}



/*
==============================================================================
Name : bind_post
==============================================================================
*/

template <class T, void (T::*post_proc) ()>
void  MenuRoot::bind_post (T & receiver)
{
   _post_delegate.set <T, post_proc> (receiver);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opal
}  // namespace ohm



#endif   // ohm_opal_MenuRoot_CODEHEADER_INCLUDED

#undef ohm_opal_MenuRoot_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
