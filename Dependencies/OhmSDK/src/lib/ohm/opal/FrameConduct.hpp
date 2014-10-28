/*****************************************************************************

        FrameConduct.hpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 41077

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



#if defined (ohm_opal_FrameConduct_CURRENT_CODEHEADER)
   #error Recursive inclusion of FrameConduct code header.
#endif
#define  ohm_opal_FrameConduct_CURRENT_CODEHEADER

#if ! defined (ohm_opal_FrameConduct_CODEHEADER_INCLUDED)
#define  ohm_opal_FrameConduct_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/FrameConduct.h"
#include "ohm/util/trace.h"

#include <cassert>



namespace ohm
{
namespace opal
{


/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

template <class T>
FrameConduct <T>::FrameConduct (const char * name_0)
:  Frame (name_0)
,  _conduct ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class T>
FrameConduct <T>::~FrameConduct ()
{
   try
   {
   }
   catch (...)
   {
      ohm_util_TRACE_ERROR ("Uncatched Exception");
      assert (false);
   }
}



/*
==============================================================================
Name : use
==============================================================================
*/

template <class T>
T &   FrameConduct <T>::use ()
{
   return _conduct;
}



/*
==============================================================================
Name : push_child_conduct
Description :
   Change event to child conduct.
   WARNING : must be balanced with a 'pop_child_conduct' call
==============================================================================
*/

template <class T>
void  FrameConduct <T>::push_child_conduct (Event & event)
{
   event.save ();
   event.set (&_conduct);
}



/*
==============================================================================
Name : pop_child_conduct
==============================================================================
*/

template <class T>
void  FrameConduct <T>::pop_child_conduct (Event & event)
{
   event.restore ();
}



/*
==============================================================================
Name : propagate_children
==============================================================================
*/

template <class T>
void  FrameConduct <T>::propagate_children (Event & event)
{
   event.save ();
   event.set (&_conduct);
   
   Frame::propagate_children (event);

   event.restore ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



#endif   // ohm_opal_FrameConduct_CODEHEADER_INCLUDED

#undef ohm_opal_FrameConduct_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
