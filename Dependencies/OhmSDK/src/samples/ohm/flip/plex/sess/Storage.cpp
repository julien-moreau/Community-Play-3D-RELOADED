/*****************************************************************************

        Storage.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71439

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

#include "plex/sess/Storage.h"

#include "plex/sess/SessCentral.h"
#include "plex/shared/Configuration.h"

// warning : the following #include depends on inclusion rules
#include "model/Root.h"

#include "ohm/flip/DocumentServer.h"
#include "ohm/sys/TimerReliable.h"
#include "ohm/txt/out/Print.h"
#include "ohm/util/trace.h"

#include <stdexcept>

#include <openssl/rand.h>

#include <cassert>



namespace plex
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
   Create the flip document server and read the document if any.
   Throws if the session could not be created, for example if a non
   recoverable while reading on S3 occurs.
==============================================================================
*/

Storage::Storage (SessCentral & central, ohm::flip::DocumentServer & document, const std::string & session_id)
:  _central (central)
,  _document (document)
,  _session_id (session_id)
{
   using namespace ohm;
   
   sys::TimerReliable & tr = sys::TimerReliable::use_instance ();
   
   double t0 = tr.get ();
   
   bool exist_flag = load (); // throws
   
   if (!exist_flag)
   {
      ohm_util_TRACE_INFO1 ("s%1%: creating empty document", _session_id.c_str ());
      
      flip::ObjectManager & object_manager = document.use_object_manager ();
      flip::Object * obj_ptr = object_manager.get_root ();
      assert (obj_ptr != 0);
      
      model::Root * root_ptr = dynamic_cast <model::Root *> (obj_ptr);
      assert (root_ptr != 0);
      
      document.begin_transaction ();
      
      root_ptr->ext_init ();
      
      document.end_transaction ();
   }
   
   double t1 = tr.get ();
   
   ohm_util_TRACE_INFO2 ("s%1%:   in %2% ms", _session_id.c_str (), (t1 - t0) * 1000.0);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Storage::~Storage ()
{
   try
   {
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : flush
==============================================================================
*/

void  Storage::flush ()
{
   using namespace ohm;
   
   std::vector <archi::UByte> data;
   write (data);

   ohm_util_TRACE_INFO1 ("s%1%: flush", _session_id.c_str ());

   flush (data, _session_id);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : load
Description :
   Load the document with '_session_id'.
   Returns true iff the document exists.
   This is typically here that you implement your storage solution.
   Here we are using the local filesystem, loading from the current directory.
==============================================================================
*/

bool  Storage::load ()
{
   using namespace ohm;
   
   FILE * file_ptr = fopen (_session_id.c_str (), "r");
   
   if (file_ptr == 0)
   {
      // no such file exists : assume a new session must be created.
      
      return false;
      
      // in the case where an error occurs, for example the session exists but cannot
      // accessed, then one would 'throw' to stop the session (and to prevent overwriting
      // the session)
   }
   
   // read the document and report loading progress
   
   ::fseek (file_ptr, 0, SEEK_END);
   size_t file_len = ::ftell (file_ptr);
   
   if (file_len == 0)
   {
      ohm_util_TRACE_ERROR1 (
         "s%1%: loading session %1% failed (file empty)",
         _session_id.c_str ()
      );

      fclose (file_ptr);
      file_ptr = 0;
      
      throw std::runtime_error ("loading session failed (file empty)");
   }
   
   ::fseek (file_ptr, 0, SEEK_SET);
   
   _central.notify_storage_reading (0);
   
   std::vector <archi::UByte> data;
   data.resize (file_len);
   
   const size_t block_size = 256 * 1024;
   
   for (size_t i = 0 ; i < file_len ; i += block_size)
   {
      _central.notify_storage_reading (i);
      
      size_t block_len = lang::min (file_len - i, block_size);
      
      size_t nmemb = ::fread (&data [i], block_len, 1, file_ptr);
      
      if (nmemb != 1)
      {
         ohm_util_TRACE_ERROR1 (
            "s%1%: loading session %1% failed (fread failed)",
            _session_id.c_str ()
         );

         fclose (file_ptr);
         file_ptr = 0;
         
         throw std::runtime_error ("loading session failed (fread failed)");
      }
   }
   
   int err = load (&data [0], data.size ());
      
   if (err != 0)
   {
      ohm_util_TRACE_ERROR2 (
         "s%1%: loading session %1% failed with flip code %2%",
         _session_id.c_str (),
         err
      );
      
      fclose (file_ptr);
      file_ptr = 0;
         
      throw std::runtime_error ("loading session failed (flip error)");
   }
   
   _central.notify_storage_idle ();
   
   return true;
}



/*
==============================================================================
Name : load
==============================================================================
*/

int   Storage::load (const ohm::archi::UByte * data_ptr, size_t data_size)
{
   using namespace ohm;
   
   int err = 0;
   
   const Configuration & conf = Configuration::get ();
   
   sys::TimerReliable & tr = sys::TimerReliable::use_instance ();
   
   double t0 = tr.get ();
   
   if (err == 0)
   {
      ohm_util_TRACE_INFO1 ("s%1%:   loading", _session_id.c_str ());
      
      err = _document.read_mem (data_ptr, data_size);
   }
   
   if (err == 0)
   {
      double t1 = tr.get ();
      
      if (conf._timings_flag)
      {
         ohm_util_TRACE_INFO2 ("s%1%:   OK (%2% ms)", _session_id.c_str (), (t1 - t0) * 1000.0);
      }
      else
      {
         ohm_util_TRACE_INFO1 ("s%1%:   OK", _session_id.c_str ());
      }
   }
   else
   {
      ohm_util_TRACE_ERROR2 ("s%1%:  load failed with error %2%", _session_id.c_str (), err);
   }

   return err;
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  Storage::write (std::vector <ohm::archi::UByte> & data)
{
   using namespace ohm;
   
   const Configuration & conf = Configuration::get ();
   
   sys::TimerReliable & tr = sys::TimerReliable::use_instance ();
   double t0 = tr.get ();
   
   if (conf._comp_flag)
   {
      _document.write_mem (data);
   }
   else
   {
      _document.write_mem_no_comp (data);
   }
   
   double t1 = tr.get ();
   
   if (conf._timings_flag)
   {
      ohm_util_TRACE_INFO2 ("s%1%:   wr %2% ms", _session_id.c_str (), (t1 - t0) * 1000.0);
   }
}



/*
==============================================================================
Name : flush
Description :
   This is typically here that you implement your storage solution.
   Here we are using the local filesystem, writing in the current directory.
==============================================================================
*/

void  Storage::flush (const std::vector <ohm::archi::UByte> & data, const std::string & resource)
{
   using namespace ohm;
   
   FILE * file_ptr = fopen (_session_id.c_str (), "w");
   
   if (file_ptr == 0)
   {
      ohm_util_TRACE_ERROR1 (
         "s%1%: writing session %1% failed (fopen failed)",
         _session_id.c_str ()
      );
      
      return;
   }

   _central.notify_storage_writing (0, data.size ());
   
   const size_t file_len = data.size ();
   const size_t block_size = 256 * 1024;
   
   for (size_t i = 0 ; i < file_len ; i += block_size)
   {
      _central.notify_storage_writing (i, file_len);
      
      size_t block_len = lang::min (file_len - i, block_size);
      
      size_t nmemb = ::fwrite (&data [i], block_len, 1, file_ptr);
      
      if (nmemb != 1)
      {
         ohm_util_TRACE_ERROR1 (
            "s%1%: loading session %1% failed (fwrite failed)",
            _session_id.c_str ()
         );

         fclose (file_ptr);
         file_ptr = 0;
         
         _central.notify_storage_idle ();
         
         return;
      }
   }
   
   _central.notify_storage_idle ();
}



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

