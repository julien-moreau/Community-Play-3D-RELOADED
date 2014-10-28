/*****************************************************************************

        IpcPortRemote.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 66420

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

#include "ohm/task/IpcPortRemote.h"

#include "ohm/archi/def.h"

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #include "ohm/task/win32/IpcPortRemoteImpl.h"

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   #include "ohm/task/mach/IpcPortRemoteImpl.h"

#else
   #error Operation System not supported

#endif

#include <cassert>



namespace ohm
{
namespace task
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

IpcPortRemote::IpcPortRemote (const char * name_0)
:  _impl_aptr ()
{
   _impl_aptr = std::auto_ptr <IpcPortRemoteImpl> (
      new IpcPortRemoteImpl (name_0)
   );
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

IpcPortRemote::~IpcPortRemote ()
{
}



/*
==============================================================================
Name : hand_shake
==============================================================================
*/

void  IpcPortRemote::hand_shake ()
{
   return _impl_aptr->hand_shake ();
}



/*
==============================================================================
Name : send
Description :
   Send data to the local port.
   The function returns immediatly, that is, it must not block until
   the local port received the data.
   
   Returns true if the msg could be sent, false otherwise.
   Returns also false if the local port does not exists.
==============================================================================
*/

bool  IpcPortRemote::send (const archi::UByte * data_ptr, size_t data_size)
{
   return _impl_aptr->send (data_ptr, data_size);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
