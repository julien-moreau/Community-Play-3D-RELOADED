/*****************************************************************************

        LogMgr.cpp
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 68168

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

#include "ohm/sys/CritSec.h"
#include "ohm/sys/ThreadMgr.h"
#include "ohm/sys/TimerReliable.h"
#include "ohm/util/LogMgr.h"

#include <cassert>



namespace ohm
{
namespace util
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*
==============================================================================
Name: dtor
Description:
   Destroy the log object, and flush and close the file before, if openend.
   It is the client reponsibility to ensure there are no concurrent access
   to the object.
Throws: Nothing
==============================================================================
*/

LogMgr::~LogMgr ()
{
   try
   {
      close_file_no_guard ();
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name: open_file
Description:
   Open a file to dump the log. If a file was previously open, log is flushed
   and file is closed. Log counter is not reset.
Input parameters:
   - filename_0: File to open.
Returns:
   0  : OK,                    
   -1 : file couldn't be open.
Throws: Nothing
==============================================================================
*/

int   LogMgr::open_file (const char *filename_0)
{
   assert (filename_0 != 0);

   _mutex.lock ();

   int            ret_val = 0;

   try
   {
      ret_val = close_file_no_lock ();

      if (ret_val == 0)
      {
         using namespace std;

         _file_ptr = fopen (filename_0, "w");
         if (_file_ptr == 0)
         {
            ret_val = -1;
         }
         else
         {
            _filename = filename_0;
            write_no_lock ("Log started");
         }
      }
   }

   catch (...)
   {
      ret_val = -1;
      assert (false);
   }

   _mutex.unlock ();

   return (ret_val);
}



/*
==============================================================================
Name: close_file
Description:
   Flush the log and close the current open file. Does nothing if there isn't
   any open file.
Returns:
   0: OK
Throws: Nothing
==============================================================================
*/

int   LogMgr::close_file ()
{
   _mutex.lock ();

   int   ret_val = close_file_no_lock ();
   
   _mutex.unlock ();

   return (ret_val);
}



/*
==============================================================================
Name: get_filename
Description:
   Return the filename of the open file. If there isn't any open file, return
   an empty string.
Returns: The filename. Lifetime : until open_file/close_file/dtor.
Throws: Nothing
==============================================================================
*/

const char *   LogMgr::get_filename ()
{
   return (_filename.c_str ());
}



/*
==============================================================================
Name: use_aux_stream
Description:
   Use a second stream to print the log. ex : std::clog.
Input/output parameters:
   - aux_stream_ptr: stream to use.
Throws: Nothing
==============================================================================
*/

void  LogMgr::use_aux_stream (FILE *aux_stream_ptr)
{
   assert (aux_stream_ptr != 0);

   _mutex.lock ();

   flush_no_lock ();
   _aux_stream_ptr = aux_stream_ptr;

   _mutex.unlock ();
}



/*
==============================================================================
Name: stop_using_aux_stream
Description:
   Cancel the use of the second stream, if used.
Throws: Nothing
==============================================================================
*/

void  LogMgr::stop_using_aux_stream ()
{
   _mutex.lock ();

   flush_no_lock ();
   _aux_stream_ptr = 0;

   _mutex.unlock ();
}


/*
==============================================================================
Name: flush
Description:
   Write log lines to the file. If there isn't any open file, lines are lost.
Throws: Nothing
==============================================================================
*/

void  LogMgr::flush ()
{
   _mutex.lock ();

   flush_no_lock ();
   
   _mutex.unlock ();
}



/*
==============================================================================
Name: write
Description:
   Write a message into the log. It is flushed to file immediately (if log
   file is open).
Input parameters:
   - msg_0: Main message body. Extra-characters are truncated (see maximum
      length in LogLine).
   - scope_0: Scope/file information describing where the log line is
      generated. Extra-char are truncated.
   - line_nbr: Line number in the source file if available, <= 0 if unknown.
Throws: Nothing
==============================================================================
*/

void  LogMgr::write (const char *msg_0, const char *scope_0, long line_nbr)
{
   _mutex.lock ();

   write_no_lock (msg_0, scope_0, line_nbr);

   _mutex.unlock ();
}



/*
==============================================================================
Name: write_cache
Description:
   Store a message in memory. If memory is full, flush the stored lines into
   file before (if file is open). Lightweight and doesn't perform memory
   allocation if no flush is done.
Input parameters:
   - msg_0: Main message body. Extra-characters are truncated (see maximum
      length in LogLine).
   - scope_0: Scope/file information describing where the log line is
      generated. Extra-char are truncated.
   - line_nbr: Line number in the source file if available, <= 0 if unknown.
Throws: Nothing
==============================================================================
*/

void  LogMgr::write_cache (const char *msg_0, const char *scope_0, long line_nbr)
{
   _mutex.lock ();

   write_cache_no_lock (msg_0, scope_0, line_nbr);

   _mutex.unlock ();
}



/*
==============================================================================
Name: needs_flush
Description:
   Indicates if flush will occur at the next write_cache () call.
Returns: true if there is no more room in memory (flush neede).
Throws: Nothing
==============================================================================
*/

bool  LogMgr::needs_flush () const
{
   return (_line_buf.is_full ());
}



/*
==============================================================================
Name: use_instance
Description:
   Access to log singleton object. First access create the object.
Returns: Reference on the object.
Throws: Nothing
==============================================================================
*/

LogMgr & LogMgr::use_instance ()
{
   sys::CritSec   crit_sec (_instance_mutex);

   if (_instance_ptr.get () == 0)
   {
      try
      {
         std::auto_ptr <LogMgr> log_mgr_auto_ptr (new LogMgr (_instance_mutex));
         _instance_ptr = log_mgr_auto_ptr;
      }
      catch (...)
      {
         assert (false);
      }
   }

   return (*_instance_ptr);
}



void  LogMgr::force_destroy ()
{
   _instance_ptr.reset (0);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*
==============================================================================
Name: ctor
Input/output parameters:
   - mutex: Critical section associated to object.
Throws: Nothing
==============================================================================
*/

LogMgr::LogMgr (sys::Mutex &mutex)
:  _mutex (mutex)
,  _time_ref (sys::TimerReliable::use_instance ().get ())
,  _max_nbr_lines (1000L*1000L)
,  _ord (0)
,  _line_buf (NBR_BUF_LINES)
,  _file_ptr (0)
,  _filename ()
,  _aux_stream_ptr (0)
{
   // Nothing
}



int   LogMgr::close_file_no_lock ()
{
   int            ret_val = 0;

   try
   {
      ret_val = close_file_no_guard_no_lock ();
      _filename.erase ();
   }

   catch (...)
   {
      assert (false);
   }
   
   return (ret_val);
}



void  LogMgr::write_no_lock (const char *msg_0, const char *scope_0, long line_nbr)
{
   write_cache_no_lock (msg_0, scope_0, line_nbr);
   flush_no_lock ();
}



void  LogMgr::write_cache_no_lock (const char *msg_0, const char *scope_0, long line_nbr)
{
   if (needs_flush ())
   {
      flush_no_lock ();
   }

   if (_ord < _max_nbr_lines)
   {
      long           nbr_elt;
      LogLine &      line = *_line_buf.use_data_block_write (nbr_elt);
      if (nbr_elt > 0)
      {
         ohm::sys::TimerReliable &  tf = ohm::sys::TimerReliable::use_instance ();
         double         cur_time = tf.get () - _time_ref;

         line.set_log (_ord, cur_time, msg_0, scope_0, line_nbr);
         _line_buf.validate (1);
         ++ _ord;
      }
   }
}



void  LogMgr::flush_no_lock ()
{
   try
   {
      bool           something_written_flag = false;

      while (! _line_buf.is_empty ())
      {
         long           nbr_elt;
         const LogLine& line = *_line_buf.use_data_block_read (nbr_elt);
         assert (nbr_elt >= 1);

         if (_file_ptr != 0)
         {
            flush_line (_file_ptr, line, true);
            something_written_flag = true;
         }
         if (_aux_stream_ptr != 0)
         {
            flush_line (_aux_stream_ptr, line, false);
            something_written_flag = true;
         }
         _line_buf.discard (1);
      }

      if (something_written_flag)
      {
         if (_file_ptr != 0)
         {
            fflush (_file_ptr);
         }
         if (_aux_stream_ptr != 0)
         {
            fflush (_aux_stream_ptr);
         }
      }
   }

   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name: close_file_no_guard
Description:
   Flush log to file and close the file. Nothing happens if file isn't open.
Returns:
   0: OK
Throws: Nothing
==============================================================================
*/

int   LogMgr::close_file_no_guard ()
{
   if (_file_ptr != 0)
   {
// The write() uses some objects of uncontrolled lifetime like the
// sys::TimerReliable singleton, which may not exist anymore at call time.
// Therefore we don't print this end message.
//    write ("Log ended");
      flush ();
      fclose (_file_ptr);
      _file_ptr = 0;
   }

   return (0);
}



int   LogMgr::close_file_no_guard_no_lock ()
{
   if (_file_ptr != 0)
   {
//    write_no_lock ("Log ended");
      flush_no_lock ();
      fclose (_file_ptr);
      _file_ptr = 0;
   }

   return (0);
}



/*
==============================================================================
Name: flush_line
Description:
   Write a line to the file or auxiliary output stream.
Input parameters:
   - line: Line to write.
   - verbose_flag: If true, displays the line number and time.
Input/output parameters:
   - file_ptr: Stream where to write the line.
Throws: Nothing.
==============================================================================
*/

void  LogMgr::flush_line (FILE *file_ptr, const LogLine &line, bool verbose_flag)
{
   assert (file_ptr != 0);
   assert (&line != 0);

   if (verbose_flag)
   {
      fprintf (
         file_ptr,
         "%ld\t%ld\t%.3f\t",
         line.get_ord (),
         (long) (sys::ThreadMgr::get_current_thread_id ()),
         line.get_date ()
      );
   }
   fprintf (file_ptr, "%s\t", line.get_msg ());
   if (verbose_flag)
   {
      fprintf (file_ptr, "%s", line.get_scope ());
      if (line.get_line_nbr () > 0)
      {
         fprintf (file_ptr, ", %ld", line.get_line_nbr ());
      }
   }
   fprintf (file_ptr, "\n");
}


//#*jno: order is important here
// _instance_mutex needs to be destroyed after _instance_ptr
// however this is not enough. Good behaviour depends on 
// ohm::sys::TimerReliable::use_instance () getting called before
// before LogMgr::use_instance ()

sys::Mutex LogMgr::_instance_mutex; 
std::auto_ptr <LogMgr>  LogMgr::_instance_ptr (0);



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace util
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
