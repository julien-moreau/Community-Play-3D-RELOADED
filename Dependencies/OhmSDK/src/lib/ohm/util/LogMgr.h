/*****************************************************************************

        LogMgr.h
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 67722

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



#if defined (ohm_util_LogMgr_CURRENT_HEADER)
   #error Recursive inclusion of ohm/util/LogMgr header.
#endif
#define  ohm_util_LogMgr_CURRENT_HEADER

#if ! defined (ohm_util_LogMgr_HEADER_INCLUDED)
#define  ohm_util_LogMgr_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/mem/LockFreeRingFifo.h"
#include "ohm/util/LogLine.h"
#include "ohm/sys/Mutex.h"

#include <memory>
#include <string>

#include <cstdio>



namespace ohm
{

namespace sys
{
   class Mutex;
}

namespace util
{



class LogMgr
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum {         NBR_BUF_LINES  = 64  };

                  ~LogMgr ();

   int            open_file ();
   int            open_file (const char *filename_0);
   int            close_file ();
   void           use_aux_stream (FILE *aux_stream_ptr);
   void           stop_using_aux_stream ();
   const char *   get_filename ();
   void           flush ();

   void           write (const char *msg_0, const char *scope_0 = "", long line_nbr = -1);
   void           write_cache (const char *msg_0, const char *scope_0 = "", long line_nbr = -1);
   bool           needs_flush () const;

   static LogMgr& use_instance ();
   static void    force_destroy ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  LogMgr (sys::Mutex &mutex);
   
   int            close_file_no_lock ();
   void           write_no_lock (const char *msg_0, const char *scope_0 = "", long line_nbr = -1);
   void           write_cache_no_lock (const char *msg_0, const char *scope_0 = "", long line_nbr = -1);
   void           flush_no_lock ();
   
   int            close_file_no_guard ();
   int            close_file_no_guard_no_lock ();
   void           flush_line (FILE *file_ptr, const LogLine &line, bool verbose_flag);

   sys::Mutex &   _mutex;

   double         _time_ref;
   long           _max_nbr_lines;
   volatile long  _ord;
   mem::LockFreeRingFifo <LogLine>
                  _line_buf;
   FILE *         _file_ptr;        // 0 = file closed
   std::string    _filename;
   FILE *         _aux_stream_ptr;  // 0 = no auxiliary stream selected

   static std::auto_ptr <LogMgr>
                  _instance_ptr;
   static sys::Mutex 
                  _instance_mutex;




/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  LogMgr ();
                  LogMgr (const LogMgr &other);
   LogMgr &       operator = (const LogMgr &other);
   bool           operator == (const LogMgr &other);
   bool           operator != (const LogMgr &other);

}; // class LogMgr



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace util
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_util_LogMgr_HEADER_INCLUDED

#undef ohm_util_LogMgr_CURRENT_HEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
