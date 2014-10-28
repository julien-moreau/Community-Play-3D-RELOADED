/*****************************************************************************

        SessCentral.cpp
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

#include "plex/sess/SessCentral.h"

#include "plex/shared/Configuration.h"
#include "plex/sess/SessSession.h"
#include "plex/sess/Storage.h"
#include "plex/shared/StatsData.h"
#include "plex/shared/ProtocolOsSess.h"

// warning : the following #include depends on inclusion rules
#include "model/ModelValidator.h"
#include "model/ModelTweaker.h"
//#include "model/conv/VersionTree.h"

#include "ohm/flip/DocumentServer.h"

#include "ohm/sys/TimerReliable.h"
#include "ohm/util/BinaryStreamInput.h"
#include "ohm/util/BinaryStreamOutput.h"
#include "ohm/util/BinaryStreamOutputNa.h"
#include "ohm/util/trace.h"

#include <stdexcept>

#if defined (__linux__)
   #include <sys/sysinfo.h>

#elif defined (__APPLE__)
   #include <mach/task.h>
   #include <mach/mach_init.h>
#endif

#include <sys/wait.h>

#include <cassert>



namespace plex
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

SessCentral::SessCentral (const std::string & session_id)
:  _session_id (session_id)
,  _flush_flag (false)
,  _flush_ts (ohm::sys::TimerReliable::use_instance ().get ())
,  _last_sample_time_s (ohm::sys::TimerReliable::use_instance ().get ())
,  _cpu (0)
{
   using namespace ohm;
   
   try
   {
      const Configuration & conf = Configuration::get ();
      
      _session_aptr = std::auto_ptr <SessSession> (
         new SessSession (*this)
      );

      _document_server_aptr = std::auto_ptr <flip::DocumentServer> (
         new flip::DocumentServer (*_session_aptr)
      );
      
      _model_validator_aptr = std::auto_ptr <model::ModelValidator> (
         new model::ModelValidator
      );
      
      _model_tweaker_aptr = std::auto_ptr <model::ModelTweaker> (
         new model::ModelTweaker
      );
      
      _document_server_aptr->bind (*_model_validator_aptr);
      //_document_server_aptr->bind (&model::VersionTree::convert);
      _document_server_aptr->bind (*_model_tweaker_aptr);
      
      // will load the document or throw
      
      _storage_aptr = std::auto_ptr <Storage> (
         new Storage (*this, *_document_server_aptr, session_id)
      );
      
      ohm_util_TRACE_INFO1 ("s%1%: succesfully created", _session_id.c_str ());
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

SessCentral::~SessCentral ()
{
   try
   {
      _storage_aptr.reset ();
      _model_tweaker_aptr.reset ();
      _model_validator_aptr.reset ();
      _document_server_aptr.reset ();
      _session_aptr.reset ();
      
      ohm_util_TRACE_INFO1 ("s%1%: succesfully released", _session_id.c_str ());
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : run
Description :
   Start listening socket for connections. Each time a connection is made,
   the connection is checked and a new 'Client' is builded.
==============================================================================
*/

void  SessCentral::run ()
{
   // ignore SIGPIPE : write will return EPIPE
   ::signal (SIGPIPE, SIG_IGN);
   
   // run loop
   
   _run_flag = true;
   
   while (_run_flag)
   {
      process ();
   }
}



/*
==============================================================================
Name : status
==============================================================================
*/

void  SessCentral::status ()
{
   using namespace ohm;
   
   size_t size = StatsData::get_proc_mem_used ();
   
   double now = sys::TimerReliable::use_instance ().get ();
   
   if (now > _last_sample_time_s + 0.25)
   {
      _last_sample_time_s = now;
      
      _cpu = StatsData::get_proc_cpu_used ();
   }

   std::vector <archi::UByte> data;
   ProtocolOsSess::write_proc_stat (data, size, _cpu);
   
   send_msg (data);
}



/*
==============================================================================
Name : send
==============================================================================
*/

void  SessCentral::send (const std::string & to, const ohm::archi::UByte * data_ptr, size_t data_size)
{
   using namespace ohm;
   
   std::vector <archi::UByte> data;
   ProtocolOsSess::write_send (data, to, data_ptr, data_size);
   
   send_msg (data);
   
   status ();
}



/*
==============================================================================
Name : broadcast
==============================================================================
*/

void  SessCentral::broadcast (const ohm::archi::UByte * data_ptr, size_t data_size)
{
   using namespace ohm;
   
   std::vector <archi::UByte> data;
   ProtocolOsSess::write_broadcast (data, data_ptr, data_size);
   
   send_msg (data);
   
   status ();
}



/*
==============================================================================
Name : broadcast
==============================================================================
*/

void  SessCentral::broadcast (const std::string & except, const ohm::archi::UByte * data_ptr, size_t data_size)
{
   using namespace ohm;
   
   std::vector <archi::UByte> data;
   ProtocolOsSess::write_broadcast_except (data, except, data_ptr, data_size);
   
   send_msg (data);
   
   status ();
}



/*
==============================================================================
Name : document_need_flush
==============================================================================
*/

void  SessCentral::document_need_flush ()
{
   _flush_flag = true;
}



/*
==============================================================================
Name : notify_storage_idle
==============================================================================
*/

void  SessCentral::notify_storage_idle ()
{
   using namespace ohm;
   
   std::vector <archi::UByte> data;
   ProtocolOsSess::write_storage_idle (data);
   
   send_msg (data);
}



/*
==============================================================================
Name : notify_storage_reading
==============================================================================
*/

void  SessCentral::notify_storage_reading (size_t cur)
{
   using namespace ohm;
   
   std::vector <archi::UByte> data;
   ProtocolOsSess::write_storage_reading (data, cur);
   
   send_msg (data);
}



/*
==============================================================================
Name : notify_storage_writing
==============================================================================
*/

void  SessCentral::notify_storage_writing (size_t cur, size_t total)
{
   using namespace ohm;
   
   std::vector <archi::UByte> data;
   ProtocolOsSess::write_storage_writing (data, cur, total);
   
   send_msg (data);
}



/*
==============================================================================
Name : notify_storage_copy
==============================================================================
*/

void  SessCentral::notify_storage_copying ()
{
   using namespace ohm;
   
   std::vector <archi::UByte> data;
   ProtocolOsSess::write_storage_copying (data);
   
   send_msg (data);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : send_msg
==============================================================================
*/

void  SessCentral::send_msg (const std::vector <ohm::archi::UByte> & data)
{
   bool ok_flag = write (&data [0], data.size ());
   
   if (!ok_flag)
   {
      _run_flag = false;
   }
}



/*
==============================================================================
Name : process
==============================================================================
*/

void  SessCentral::process ()
{
   using namespace ohm;
   
   bool ok_flag = process_command ();
   
   if (!ok_flag)
   {
      _run_flag = false;
   }
   
   // post
   
   if (_run_flag)
   {
      double now = sys::TimerReliable::use_instance ().get ();
         
      const Configuration & conf = Configuration::get ();
         
      if (_flush_flag)
      {
         if (_flush_ts + conf._snapshot_flush_period < now)
         {
            _storage_aptr->flush ();
            
            _flush_flag = false;
            _flush_ts = sys::TimerReliable::use_instance ().get ();
         }
      }
   }
}



/*
==============================================================================
Name : process_command
==============================================================================
*/

bool  SessCentral::process_command ()
{
   using namespace ohm;
   
   archi::UByte buf [4];
   
   bool ok_flag = true;
   
   if (ok_flag)
   {
      ok_flag = read (buf, 4);
   }
   
   archi::UInt32 packet_size = 0;
   
   if (ok_flag)
   {
      util::BinaryStreamInput bsi (buf, 4);
      
      bsi.read (packet_size);
      
      ok_flag &= packet_size >= 4;
   }

   std::vector <archi::UByte> data;

   if (ok_flag)
   {
      size_t size = packet_size - 4;
      
      data.resize (size);
         
      ok_flag = read (&data [0], size);
   }
   
   if (ok_flag && !data.empty ())
   {
      util::BinaryStreamInput bsi (&data [0], data.size ());
      
      ProtocolOsSess::Sess type = ProtocolOsSess::Sess_INVALID;
      ProtocolOsSess::read_type (type, bsi);
      
      switch (type)
      {
      case ProtocolOsSess::Sess_TX:
         process_tx (bsi);
         break;

      case ProtocolOsSess::Sess_DISCONNECTED:
         process_disconnected (bsi);
         break;

      case ProtocolOsSess::Sess_FLUSH:
         process_flush (bsi);
         break;

      case ProtocolOsSess::Sess_STOP:
         process_stop (bsi);
         break;
      
      default:
         assert (false);
         break;
      }
   }
   
   return ok_flag;
}



/*
==============================================================================
Name : process_tx
==============================================================================
*/

void  SessCentral::process_tx (ohm::util::BinaryStreamInput & bsi)
{
   using namespace ohm;
   
   std::string username;
   size_t data_size = 0;
   const archi::UByte * data_ptr = 0;
   
   ProtocolOsSess::read_tx (username, data_ptr, data_size, bsi);
   
   _session_aptr->receive (username, data_ptr, data_size);
}



/*
==============================================================================
Name : process_disconnected
==============================================================================
*/

void  SessCentral::process_disconnected (ohm::util::BinaryStreamInput & bsi)
{
   using namespace ohm;
   
   std::string username;
   
   ProtocolOsSess::read_disconnected (username, bsi);
   
   _document_server_aptr->notify_disconnected (username);
   
   flip::DocumentServer & document = *_document_server_aptr;
   
   flip::ObjectManager & object_manager = document.use_object_manager ();
   flip::Object * obj_ptr = object_manager.get_root ();
   assert (obj_ptr != 0);
   
   model::Root * root_ptr = dynamic_cast <model::Root *> (obj_ptr);
   assert (root_ptr != 0);
   
   document.begin_transaction ();
   
   model::ClientDataColl::iterator it = root_ptr->find (username);
   const model::ClientDataColl::iterator it_end = root_ptr->use_client_data_coll ().end ();
   
   if (it != it_end)
   {
      model::ClientData & client_data = *it;
      model::Selection & selection = client_data.use_selection ();
      selection.ext_clear ();
   }
   
   document.commit_transaction ();
}



/*
==============================================================================
Name : process_flush
==============================================================================
*/

void  SessCentral::process_flush (ohm::util::BinaryStreamInput & bsi)
{
   _storage_aptr->flush ();
}



/*
==============================================================================
Name : process_stop
==============================================================================
*/

void  SessCentral::process_stop (ohm::util::BinaryStreamInput & bsi)
{
   using namespace ohm;
   
   _run_flag = false;

   _storage_aptr->flush ();
   
   std::vector <archi::UByte> data;
   ProtocolOsSess::write_stopping (data);
   
   send_msg (data);
}



/*
==============================================================================
Name : read
==============================================================================
*/

bool  SessCentral::read (ohm::archi::UByte * data_ptr, size_t data_size)
{
   size_t pos = 0;
   
   do 
   {
      int nbr_bytes = ::read (STDIN_FILENO, &data_ptr [pos], data_size - pos);
      
      if (nbr_bytes < 0)
      {
         if (errno == EINTR)
         {
            nbr_bytes == 0;
         }
         else
         {
            return false;
         }
      }
      else if (nbr_bytes == 0)
      {
         return false;
      }
      
      pos += nbr_bytes;
   } 
   while (pos < data_size);
   
   return true;
}



/*
==============================================================================
Name : write
==============================================================================
*/

bool  SessCentral::write (const std::vector <ohm::archi::UByte> & data)
{
   if (data.empty ())
   {
      return true;
   }
   
   return write (&data [0], data.size ());
}



/*
==============================================================================
Name : write
==============================================================================
*/

bool  SessCentral::write (const ohm::archi::UByte * data_ptr, size_t data_size)
{
   size_t pos = 0;
   
   do 
   {
      int nbr_bytes = ::write (STDOUT_FILENO, &data_ptr [pos], data_size - pos);
      
      if (nbr_bytes < 0)
      {
         if (errno == EINTR)
         {
            nbr_bytes == 0;
         }
         else
         {
            return false;
         }
      }
      else if (nbr_bytes == 0)
      {
         return false;
      }
      
      pos += nbr_bytes;
   } 
   while (pos < data_size);
   
   return true;
}



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

