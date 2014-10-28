/*****************************************************************************

        IpcPortBiDir.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 67796

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



#if ! defined (ohm_task_IpcPortBiDir_HEADER_INCLUDED)
#define  ohm_task_IpcPortBiDir_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/task/IpcPortLocal.h"
#include "ohm/task/IpcPortRemote.h"

#include <string>



namespace ohm
{
namespace task
{


class IpcData;

class IpcPortBiDir
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   enum Reply
   {
                  Reply_DATA_AVAILABLE = 0,
                  Reply_TIME_OUT,
                  Reply_CONNECTION_CLOSED,
   };
   
                  IpcPortBiDir (const char * name_0, bool host_flag);
   virtual        ~IpcPortBiDir ();
   
   void           wait_hand_shake ();
   
   bool           send (const archi::UByte * data_ptr, size_t data_size);
   Reply          recv (IpcData & data, double timeout = -1.0);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   const bool     _host_flag;
   const std::string
                  _name;
   bool           _log_flag;
                                 
   std::auto_ptr <IpcPortLocal>
                  _port_local_aptr;
   std::auto_ptr <IpcPortRemote>
                  _port_remote_aptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  IpcPortBiDir ();
                  IpcPortBiDir (const IpcPortBiDir &other);
   IpcPortBiDir & operator = (const IpcPortBiDir &other);
   bool           operator == (const IpcPortBiDir &other) const;
   bool           operator != (const IpcPortBiDir &other) const;

}; // class IpcPortBiDir



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

//#include "ohm/task/IpcPortBiDir.hpp"



#endif   // ohm_task_IpcPortBiDir_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
