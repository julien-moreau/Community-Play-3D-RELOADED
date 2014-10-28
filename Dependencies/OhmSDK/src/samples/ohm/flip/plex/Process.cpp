/*****************************************************************************

        Process.cpp
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



#if defined (_MSC_VER)
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "plex/Process.h"

#include "plex/fnc.h"
#include "plex/shared/LogMgr.h"

#include "ohm/util/trace.h"

#include <stdexcept>

#include <fcntl.h>



#undef SKELETON_SERVER_PROCESS_DEBUG_FLAG

#if defined (SKELETON_SERVER_PROCESS_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error SKELETON_SERVER_PROCESS_DEBUG_FLAG is defined in shipping level code !
   #endif

   #define proc_TRACE(msg_0)  ohm_util_TRACE_INFO(msg_0)
   #define proc_TRACE1(fmt_0, arg1) ohm_util_TRACE_INFO1(fmt_0, arg1)
   #define proc_TRACE2(fmt_0, arg1, arg2) ohm_util_TRACE_INFO2(fmt_0, arg1, arg2)
   #define proc_TRACE3(fmt_0, arg1, arg2, arg3) ohm_util_TRACE_INFO3(fmt_0, arg1, arg2, arg3)
   #define proc_TRACE4(fmt_0, arg1, arg2, arg3, arg4) ohm_util_TRACE_INFO4(fmt_0, arg1, arg2, arg3, arg4)
#else
   #define proc_TRACE(msg_0)  (void (0))
   #define proc_TRACE1(fmt_0, arg1) (void (0))
   #define proc_TRACE2(fmt_0, arg1, arg2) (void (0))
   #define proc_TRACE3(fmt_0, arg1, arg2, arg3) (void (0))
   #define proc_TRACE4(fmt_0, arg1, arg2, arg3, arg4) (void (0))
#endif



namespace plex
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

const char *   Process::_ns_log_0 = "Process";

/*
==============================================================================
Name : ctor
==============================================================================
*/

Process::Process (const std::string bin_path, const std::list <std::string> & arg_list)
:  _child_pid (-1)
{
#if defined (SKELETON_SERVER_PROCESS_DEBUG_FLAG)
   {
      std::string cmd = bin_path + " ";
      
      std::list <std::string>::const_iterator it = arg_list.begin ();
      const std::list <std::string>::const_iterator it_end = arg_list.end ();
      
      for (; it != it_end ; ++it)
      {
         const std::string & arg = *it;
         
         cmd += arg + " ";
      }
      
      TRACEo1 ("%1%", cmd.c_str ());
   }
#endif

   init_pipe (_to_child);
   init_pipe (_to_parent);
   
   _child_pid = fork ();
   
   if (_child_pid == 0)
   {
      run_child (bin_path, arg_list);
      assert (false);
   }
   else if (_child_pid > 0)
   {
      run_parent ();
   }
   else
   {
      assert (false);
      TRACEX1 ("'fork' failed with error `%1%'", strerror (errno));
      
      throw std::runtime_error ("Process::ctor fork failed");
   }
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Process::~Process ()
{
   try
   {
      if (_child_pid != 0)
      {
         ::close (_to_child [WRITE]);
         ::close (_to_parent [READ]);

         int child = 0;
         int status = 0;
         
         do
         {
            child = waitpid (_child_pid, &status, 0);
         }
         while ((child == -1) && (errno == EINTR));
         
         assert (child == _child_pid);
      }
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : acquire
==============================================================================
*/

pid_t Process::acquire ()
{
   ::close (_to_child [WRITE]);
   ::close (_to_parent [READ]);
   
   pid_t child_pid = _child_pid;
   _child_pid = 0;
   
   return child_pid;
}



/*
==============================================================================
Name : write
==============================================================================
*/

ssize_t  Process::write (const void * data_ptr, size_t data_size)
{
   int fd = (_child_pid == 0) ? _to_parent [WRITE] : _to_child [WRITE];
   
   ssize_t write_size = ::write (fd, data_ptr, data_size);
   
   return write_size;
}



/*
==============================================================================
Name : get_read_fd
==============================================================================
*/

int   Process::get_read_fd ()
{
   return (_child_pid == 0) ? _to_child [READ] : _to_parent [READ];
}



/*
==============================================================================
Name : get_write_fd
==============================================================================
*/

int   Process::get_write_fd ()
{
   return (_child_pid == 0) ? _to_parent [WRITE] : _to_child [WRITE];
}



/*
==============================================================================
Name : kill
==============================================================================
*/

void  Process::kill ()
{
   if (_child_pid != 0)
   {
      ::kill (_child_pid, SIGTERM);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : init_pipe
==============================================================================
*/

void  Process::init_pipe (int fds [])
{
   int err = ::pipe (fds);
   
   if (err != 0)
   {
      assert (false);
      TRACEX1 ("'pipe' failed with error `%1%'", strerror (errno));
      
      throw std::runtime_error ("Process::init_pipe failed");
   }
}



/*
==============================================================================
Name : run_parent
==============================================================================
*/

void  Process::run_parent ()
{
   ::close (_to_child [READ]);
   ::close (_to_parent [WRITE]);
   
   set_non_blocking (_to_parent [READ]);
   set_non_blocking (_to_child [WRITE]);
}



/*
==============================================================================
Name : run_child
==============================================================================
*/

void  Process::run_child (const std::string bin_path, const std::list <std::string> & arg_list)
{
   ::close (_to_child [WRITE]);
   ::close (_to_parent [READ]);
   
   if (dup2 (_to_child [READ], 0) < 0)
   {
      exit (-1);
   }

   if (dup2 (_to_parent [WRITE], 1) < 0)
   {
      exit (-1);
   }
   
   _to_child [READ] = 0;
   _to_parent [WRITE] = 1;
   
   std::vector <const char *> arg_0_arr;
   
   arg_0_arr.push_back (bin_path.c_str ());
   
   std::list <std::string>::const_iterator it = arg_list.begin ();
   const std::list <std::string>::const_iterator it_end = arg_list.end ();
   
   size_t i = 0;
   
   for (; it != it_end ; ++it, ++i)
   {
      const std::string & arg = *it;
      
      arg_0_arr.push_back (arg.c_str ());
   }
   
   arg_0_arr.push_back (0);
   
   int err = execv (bin_path.c_str (), (char * const *) &arg_0_arr [0]);
   err;
   assert (false);
}



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

