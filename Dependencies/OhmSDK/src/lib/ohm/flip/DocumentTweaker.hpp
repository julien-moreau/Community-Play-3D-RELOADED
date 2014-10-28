/*****************************************************************************

        DocumentTweaker.hpp
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 63856

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



#if defined (ohm_flip_DocumentTweaker_CURRENT_CODEHEADER)
   #error Recursive inclusion of DocumentTweaker code header.
#endif
#define  ohm_flip_DocumentTweaker_CURRENT_CODEHEADER

#if ! defined (ohm_flip_DocumentTweaker_CODEHEADER_INCLUDED)
#define  ohm_flip_DocumentTweaker_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <algorithm>
#include <cassert>


namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

template <class T>
DocumentTweaker <T>::DocumentTweaker ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class T>
DocumentTweaker <T>::~DocumentTweaker ()
{
}



/*
==============================================================================
Name : request_process
==============================================================================
*/

template <class T>
void  DocumentTweaker <T>::request_process (Object & obj)
{
   Object * obj_ptr = &obj;
   
   T * root_ptr = dynamic_cast <T *> (obj_ptr);
   assert (root_ptr != 0);
   
   process (*root_ptr);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



#endif   // ohm_flip_DocumentTweaker_CODEHEADER_INCLUDED

#undef ohm_flip_DocumentTweaker_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
