/*****************************************************************************

        IpcPortLocalImpl.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 68689

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



#if ! defined (ohm_task_IpcPortLocalImpl_HEADER_INCLUDED)
#define  ohm_task_IpcPortLocalImpl_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <string>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>



namespace ohm
{
namespace task
{


class IpcData;

class IpcPortLocalImpl
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
                  IpcPortLocalImpl (const char * name_0);
   virtual        ~IpcPortLocalImpl ();
   
   void           wait_connect (double timeout);
   bool           recv (IpcData & data, double timeout);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           connect_sync (double timeout);
   void           disconnect (bool log_flag = true);
   
   
   ::HANDLE       _h_pipe; // INVALID_HANDLE_VALUE = not open
   ::OVERLAPPED   _ol;
   std::string    _name;   // Mostly for debugging purpose
   
   bool           _connected_flag;
   bool           _read_pending;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  IpcPortLocalImpl ();
                  IpcPortLocalImpl (const IpcPortLocalImpl &other);
   IpcPortLocalImpl &   operator = (const IpcPortLocalImpl &other);
   bool           operator == (const IpcPortLocalImpl &other) const;
   bool           operator != (const IpcPortLocalImpl &other) const;

}; // class IpcPortLocalImpl



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

//#include "ohm/task/IpcPortLocalImpl.hpp"



#endif   // ohm_task_IpcPortLocalImpl_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
