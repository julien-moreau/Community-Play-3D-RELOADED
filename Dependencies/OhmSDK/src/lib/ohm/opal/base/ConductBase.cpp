/*****************************************************************************

        ConductBase.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 58500

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



#if defined (_MSC_VER)
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/base/ConductBase.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

ConductBase::ConductBase ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ConductBase::~ConductBase ()
{
}



/*
==============================================================================
Name : is_spatial
Description :
   If the subclasses returns true, then the conduct is marked as having data
   which require transformation through the affine transformation system
   handled by events.
   When the subclasses returns true, it will immediatly get called through
   its 'bind' function, which passes the event that the conduct caches to
   do subsequent transformation operations.
   When 'invalidate' is called, this means that the affine transform in the
   event changed, and should mark all data internal to the conduct as invalid.
   Normally it is a good thing to set an invalid flag on the call of
   'invalidate', and then update the internal data only when it gets fetched :
   in practive 'invalidate' might get called very often, as any frames
   which has a non-zero origin triggers an invalidation.
   
   One should note that returning true this does not mean that one event
   which would contain this conduct would become automatically spatial.
   The conduct might be send through an event which will go through different
   passes, in particular SPATIAL and CRUMB.
==============================================================================
*/

bool  ConductBase::is_spatial () const
{
   return false;
}



/*
==============================================================================
Name : bind
Description :
   See description of 'is_spatial'
Note :
   This is just defined to know that ConductBase must have a bind function
   to match prototype in template call.
   This will never get called.
==============================================================================
*/

void  ConductBase::bind (Event & event)
{
   assert (false);
}



/*
==============================================================================
Name : invalidate
Description :
   See description of 'is_spatial'
Note :
   Only valid when 'is_spatial' returns true.
==============================================================================
*/

void  ConductBase::invalidate ()
{
   // because the default conduct base is not spatial, we should
   // not arrive here.
   
   assert (false);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
