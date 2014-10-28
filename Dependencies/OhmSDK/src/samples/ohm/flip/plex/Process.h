/*****************************************************************************

        Process.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71330

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



#if ! defined (plex_Process_HEADER_INCLUDED)
#define  plex_Process_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"

#include <list>
#include <string>
#include <vector>



namespace plex
{



class Process
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  Process (const std::string bin_path, const std::list <std::string> & arg_list);
   virtual        ~Process ();
   
   pid_t          acquire ();

   ssize_t        write (const void * data_ptr, size_t data_size);
   int            get_read_fd ();
   int            get_write_fd ();
   
   void           kill ();
   //int          get_exit_status ();

   static const char *
                  _ns_log_0;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   
   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum {         READ = 0    };
   enum {         WRITE = 1   };
   
   void           init_pipe (int fds []);
   
   void           run_parent ();
   void           run_child (const std::string bin_path, const std::list <std::string> & arg_list);
   
   int            _to_child [2];
   int            _to_parent [2];
   
   pid_t          _child_pid;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Process ();
                  Process (const Process &other);
   Process &      operator = (const Process &other);
   bool           operator == (const Process &other);
   bool           operator != (const Process &other);

}; // class Process



}  // namespace plex



//#include  "Process.hpp"



#endif   // plex_Process_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

