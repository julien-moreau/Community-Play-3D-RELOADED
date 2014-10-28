/*****************************************************************************

        DocumentServer.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71438

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

#include "ohm/flip/Constants.h"
#include "ohm/flip/ComLayerMsgType.h"
#include "ohm/flip/DocumentServer.h"
#include "ohm/flip/DocumentSignal.h"
#include "ohm/flip/DocumentTweaker.h"
#include "ohm/flip/DocumentValidator.h"
#include "ohm/flip/ValidationReportWriter.h"
#include "ohm/flip/private/DataStreamOutput.h"
#include "ohm/flip/private/DocumentReader.h"
#include "ohm/flip/private/DocumentWriter.h"
#include "ohm/flip/private/MachineCode.h"
#include "ohm/flip/private/SeedGenerator.h"
#include "ohm/flip/private/StdCtlMsg.h"
#include "ohm/util/BinaryStreamInput.h"
#include "ohm/util/BinaryStreamOutput.h"
#include "ohm/util/BinaryStreamOutputNa.h"

#include "ohm/util/trace.h"

#include "zlib.h"

#include <cassert>

#undef OHM_FLIP_DOCUMENT_DEBUG_FLAG
#define OHM_FLIP_DOCUMENT_SERVER_INTERNAL_BROADCAST

#if defined (OHM_FLIP_DOCUMENT_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_FLIP_DOCUMENT_DEBUG_FLAG is defined in shipping level code !
   #endif

   #define docs_TRACE(msg_0)  ohm_util_TRACE_INFO(msg_0)
   #define docs_TRACE1(fmt_0, arg1) ohm_util_TRACE_INFO1(fmt_0, arg1)
   #define docs_TRACE2(fmt_0, arg1, arg2) ohm_util_TRACE_INFO2(fmt_0, arg1, arg2)
   #define docs_TRACE3(fmt_0, arg1, arg2, arg3) ohm_util_TRACE_INFO3(fmt_0, arg1, arg2, arg3)
   #define docs_TRACE4(fmt_0, arg1, arg2, arg3, arg4) ohm_util_TRACE_INFO4(fmt_0, arg1, arg2, arg3, arg4)
#else
   #define docs_TRACE(msg_0)  (void (0))
   #define docs_TRACE1(fmt_0, arg1) (void (0))
   #define docs_TRACE2(fmt_0, arg1, arg2) (void (0))
   #define docs_TRACE3(fmt_0, arg1, arg2, arg3) (void (0))
   #define docs_TRACE4(fmt_0, arg1, arg2, arg3, arg4) (void (0))
#endif



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
   New session version (creating root file skeleton)
==============================================================================
*/

DocumentServer::DocumentServer (ComLayerServer & com_layer)
:  DocumentBase ()
,  _com_layer (com_layer)
,  _rev_id (0)
,  _tx_id_generator (SeedGenerator::SERVER_USER_ID, 0)
,  _converter_proc (0)
,  _document_validator_ptr (0)
,  _document_signal_ptr (0)
,  _document_tweaker_ptr (0)
,  _int_ext_map ()
,  _cached_doc_data ()
{
   com_layer.bind (*this);
   
   set_controller (Controller_SERVER);
      
   use_object_manager ().create_root ();
   
   post_document_change_task ();
   
   set_controller (Controller_NONE);
   
   _rev_id = 1;
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

DocumentServer::~DocumentServer ()
{
   try
   {
      set_controller (Controller_DOC_RELEASE);
      
      use_object_manager ().release_root ();
      
      post_document_change_task ();
   
      set_controller (Controller_NONE);
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : bind
==============================================================================
*/

void  DocumentServer::bind (DocumentValidatorMaster & validator)
{
   assert (&validator != 0);
   assert (_document_validator_ptr == 0);
   
   _document_validator_ptr = &validator;
}



/*
==============================================================================
Name : bind
==============================================================================
*/

void  DocumentServer::bind (DocumentSignal & signal)
{
   assert (&signal != 0);
   assert (_document_signal_ptr == 0);
   
   _document_signal_ptr = &signal;
}



/*
==============================================================================
Name : bind
Description :
   Bind the static document converter.
==============================================================================
*/

void  DocumentServer::bind (ConverterProc converter_proc)
{
   assert (converter_proc != 0);
   
   _converter_proc = converter_proc;
}



/*
==============================================================================
Name : bind
==============================================================================
*/

void  DocumentServer::bind (DocumentTweakerMaster & tweaker)
{
   assert (&tweaker != 0);
   assert (_document_tweaker_ptr == 0);
   
   _document_tweaker_ptr = &tweaker;
}



/*
==============================================================================
Name : read_mem
Description :
   Read a document and perform any decompaction/convertion if needed.
Note :
   This will maintain a cached version of the original (unconverted)
   document. This allow the server to serve the document quicker when the
   first client connect to a session.
==============================================================================
*/

int   DocumentServer::read_mem (const archi::UByte * data_ptr, size_t data_size)
{
   int err = 0;
   
   if (err == 0)
   {
      // very rough approximation of document minimum size
      // (actually minimum needed for 'uncompress')

      bool ok_flag = data_size > 8;
      
      if (!ok_flag)
      {
         err = Err_DOC_BOOTLEG;
      }
   }
   
   if (err == 0)
   {
      // cache the original document. if the revision match,
      // send this one instead
      
      _cached_doc_data.assign (data_ptr, data_ptr + data_size);
   }
   
   // uncompress if needed
   
   Data resv_data;
   
   if (err == 0)
   {
      err = uncompress (resv_data, data_ptr, data_size);
   }
   
   // convert if needed
   
   Data resv_data2;

   if ((err == 0) && (_converter_proc != 0))
   {
      // first check if a conversion is needed
   
      DataStreamInput dsi;
      dsi.assign_no_copy (data_ptr, data_size);
      
      DocumentParser dp (dsi);
      
      err = dp.init ();
      
      bool convert_flag = true;
      
      if (err == 0)
      {
         const std::string & format_version = dp.get_format_version ();
         
         // latest revision

         convert_flag &=
            format_version != flip::ClassDescManager::use_instance ().get_format_version ();
         
         // "HEAD" is a development version : don't convert
         convert_flag &= format_version != "HEAD";
      }
      
      if ((err == 0) && convert_flag)
      {
         DataStreamOutput dso;
         dso.set_mem_output (resv_data2);
         
         try
         {
            // the converter should avoid to take cpu resources
            // if no convertion is necessary
            
            dsi.seek_beg (0);
            
            bool force_flag = false;
            err = (*_converter_proc) (dso, dsi, force_flag);
            
            data_ptr = &resv_data2 [0];
            data_size = resv_data2.size ();
            
            // conversion occured : reset '_cached_doc_data'
            // to force explicit write

            Data ().swap (_cached_doc_data);
         }
         catch (...)
         {
            ohm_util_TRACE_ERROR ("convertion exception");
            
            err = Err_EXCEPTION;
         }
         
         dso.close ();
      }
   }
   
   // read
   
   if (err == 0)
   {
      err = read (data_ptr, data_size);
   }
   
   // tweak
   
   if ((err == 0) && (_document_tweaker_ptr != 0))
   {
      Object * root_ptr = use_object_manager ().get_root ();
      assert (root_ptr != 0);
      
      begin_transaction ();
      
      _document_tweaker_ptr->request_process (*root_ptr);
      
      end_transaction ();
      
      post_document_change_task ();
   }
   
   return err;
}



/*
==============================================================================
Name : read_disk
Note :
   Used only in debug mode (when server and client operates in the same
   process)
==============================================================================
*/

int   DocumentServer::read_disk (const char * path_0)
{
   using namespace std;
   
   FILE * file_ptr = fopen (path_0, "rb");
   assert (file_ptr != 0);
   
   fseek (file_ptr, 0, SEEK_END);
   
   long file_size = ftell (file_ptr);
   
   fseek (file_ptr, 0, SEEK_SET);
   
   Data data;
   data.resize (file_size);
   
   int nmemb = fread (&data [0], file_size, 1, file_ptr);
   assert (nmemb == 1);
   
   fclose (file_ptr);
   file_ptr = 0;
   
   const archi::UByte * data_ptr = &data [0];
   size_t data_size = data.size ();
   
   return read_mem (data_ptr, data_size);
}



/*
==============================================================================
Name : write_mem
==============================================================================
*/

void  DocumentServer::write_mem (std::vector <archi::UByte> & data)
{
   DataStreamOutput dso;
   dso.set_mem_output (data);
   dso.set_filter_zlib ();
   
   DocumentWriter writer (*this);
   
   writer.write (dso);
}



/*
==============================================================================
Name : write_mem_no_comp
==============================================================================
*/

void  DocumentServer::write_mem_no_comp (std::vector <archi::UByte> & data)
{
   DataStreamOutput dso;
   dso.set_mem_output (data);
   
   DocumentWriter writer (*this);
   
   writer.write (dso);
}



/*
==============================================================================
Name : write_mem
==============================================================================
*/

void  DocumentServer::write_mem (std::string & document)
{
   DataStreamOutput dso;
   dso.set_mem_output (document);
   dso.set_filter_zlib ();
   
   DocumentWriter writer (*this);
   
   writer.write (dso);
}



/*
==============================================================================
Name : write_disk
==============================================================================
*/

void  DocumentServer::write_disk (const char * path_0)
{
   DataStreamOutput dso;
   dso.set_file_output (path_0);
   //dso.set_filter_zlib ();
   
   DocumentWriter writer (*this);
   
   writer.write (dso);
}



/*
==============================================================================
Name : get_rev_id
==============================================================================
*/

archi::Int64   DocumentServer::get_rev_id ()
{
   return _rev_id;
}



/*
==============================================================================
Name : receive
Description :
   Returns -1 in the case of suspicious activity, 0 otherwise.
==============================================================================
*/

int   DocumentServer::receive (const std::string & from, util::BinaryStreamInput & bsi)
{
   check_caller_thread ();
   
   int err = Err_OK;
   
   archi::UByte type = 0;
   err = bsi.read (type);  if (err != Err_OK) return err;
   
   switch (type)
   {
   case ComLayerMsgType_CS_TX:
      err = incoming_transaction (from, bsi);
      break;
   
   case ComLayerMsgType_CS_CTL:
      err = incoming_ctl_msg (from, bsi);
      break;
   
   default:
      err = -1;
      break;
   }
   
   return err;
}



/*
==============================================================================
Name : notify_disconnected
==============================================================================
*/

void  DocumentServer::notify_disconnected (const std::string & from)
{
   docs_TRACE1 ("notify_disconnected: %1%", from.c_str ());
   
   {
      NameMap::iterator it = _int_ext_map.begin ();
      const NameMap::iterator it_end = _int_ext_map.end ();
      
      for (; it != it_end ; ++it)
      {
         // take from
         const std::string & sub_from = it->second;
         
         if (sub_from == from)
         {
            _int_ext_map.erase (it);
            
            break;
         }
      }
   }
}



/*
==============================================================================
Name : send_signal_client
==============================================================================
*/

void  DocumentServer::send_signal_client (const std::string & to, archi::Int64 ref, archi::UInt32 type, const archi::UByte * data_ptr, archi::UInt32 data_size)
{
   assert (data_ptr != 0);
   
   std::vector <archi::UByte> data;
   
   util::BinaryStreamOutput bso (data);
   bso.write (archi::UByte (ComLayerMsgType_SC_CTL));
   
   bso.write (archi::UInt32 (StdCtlMsg_SIGNAL));
   
   archi::UInt32 ctl_size = sizeof (ref) + sizeof (type) + sizeof (data_size) + data_size;
   
   bso.write (ctl_size);
   
   bso.write (ref);
   bso.write (type);
   bso.write (data_size);

   if (data_size > 0)
   {
      bso.write (data_ptr, data_size);
   }
   
   _com_layer.send (to, &data [0], data.size ());
}



/*
==============================================================================
Name : begin_transaction
==============================================================================
*/

void  DocumentServer::begin_transaction ()
{
   assert (get_controller () == Controller_NONE);
}



/*
==============================================================================
Name : end_transaction
==============================================================================
*/

void  DocumentServer::end_transaction ()
{
   if (get_controller () == Controller_SELF)
   {
      set_controller (Controller_NONE);
      
      std::auto_ptr <Data> tx_data_aptr = get_tx_data_aptr ();
      
      Data * tx_data_ptr = tx_data_aptr.release ();
      
      if (tx_data_ptr != 0)
      {
         assert (!tx_data_ptr->empty ());

         archi::Int64 tx_id = _tx_id_generator.get ();
         
         broadcast_tx (&(*tx_data_ptr) [0], tx_data_ptr->size (), tx_id, _rev_id);
         increment_rev_id ();
      }     
   }
}



/*
==============================================================================
Name : commit_transaction
==============================================================================
*/

void  DocumentServer::commit_transaction ()
{
   if (get_controller () == Controller_SELF)
   {
      set_controller (Controller_NONE);
      
      std::auto_ptr <Data> tx_data_aptr = get_tx_data_aptr ();
      
      Data * tx_data_ptr = tx_data_aptr.release ();
      
      if (tx_data_ptr != 0)
      {
         assert (!tx_data_ptr->empty ());

         archi::Int64 tx_id = _tx_id_generator.get ();
         
         increment_rev_id ();
         broadcast_tx (&(*tx_data_ptr) [0], tx_data_ptr->size (), tx_id, _rev_id);
         
         post_document_change_task ();
      }     
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : uncompress
==============================================================================
*/

int   DocumentServer::uncompress (Data & alt_data, const archi::UByte * & data_ptr, size_t & data_size)
{
   int err = 0;
   
   bool osf_flag
      = (data_ptr [0] == '%')
      && (data_ptr [1] == 'O')
      && (data_ptr [2] == 'S')
      && (data_ptr [3] == 'F')
   ;
   
   bool zlib_flag
      = (data_ptr [0] == 'z')
      && (data_ptr [1] == 'l')
      && (data_ptr [2] == 'i')
      && (data_ptr [3] == 'b')
   ;
   
   assert (osf_flag || zlib_flag);
   
   if (zlib_flag)
   {
      archi::UInt32 alt_data_size = 0;
      alt_data_size += (data_ptr [4] << 24);
      alt_data_size += (data_ptr [5] << 16);
      alt_data_size += (data_ptr [6] << 8);
      alt_data_size += (data_ptr [7]);
      
      alt_data.resize (alt_data_size);
      
      uLongf dest_len = alt_data_size;
   
      err = ::uncompress (
         &alt_data [0], &dest_len,
         &data_ptr [8], data_size - 8
      );
      assert (err == Z_OK);
      assert (dest_len == alt_data_size);
      
      if (err != Z_OK)
      {
         err = Err_CODEC_DEFLATE;
      }
      
      if (err == 0)
      {
         if (dest_len != alt_data_size)
         {
            err = Err_DOC_BOOTLEG;
         }
      }
      
      if (err == 0)
      {
         bool ok_flag = alt_data_size > 4;
         
         if (!ok_flag)
         {
            err = Err_DOC_BOOTLEG;
         }
      }
      
      if (err == 0)
      {
         data_ptr = &alt_data [0];
         data_size = alt_data_size;
      }
   }
   
   if (err == 0)
   {
      bool ok_flag = true;
      ok_flag &= data_ptr [0] == '%';
      ok_flag &= data_ptr [1] == 'O';
      ok_flag &= data_ptr [2] == 'S';
      ok_flag &= data_ptr [3] == 'F';
      
      if (!ok_flag)
      {
         err = Err_DOC_BAD_HEADER;
      }
   }

   return err;
}



/*
==============================================================================
Name : read
Note :
   Expect the data to be :
   - uncompressed
   - to the current format revision
==============================================================================
*/

int   DocumentServer::read (const archi::UByte * data_ptr, size_t data_size)
{
   int err = 0;
   
   DocumentReader reader;
   reader.load_no_copy (data_ptr, data_size);
   
   if (err == 0)
   {
      err = reader.read ();
      assert (err == Err_OK);
   }
   
   if (err == 0)
   {
      _rev_id = reader.get_rev_id ();
      
      reader.get_users (use_user_manager ());
      set_controller (Controller_DOC_READ);
      
      err = reader.exec (*this);
      assert (err == Err_OK);
      
      post_document_change_task ();
      
      set_controller (Controller_NONE);
   }
   
   return err;
}



/*
==============================================================================
Name : incoming_transaction
Description :
   A client sent a transaction.
   Execute on the master side and reply approval/refusal and broadcast.
   Returns -1 in the case of suspicious activity, 0 otherwise.
Note :
   tx_id    8
   tx_size  4
   tx_data  var
==============================================================================
*/

int   DocumentServer::incoming_transaction (const std::string & from, util::BinaryStreamInput & bsi)
{
   docs_TRACE1 ("incoming_transaction: %1%", from.c_str ());
   
   archi::Int64 tx_id = 0;
   int err = bsi.read (tx_id);   if (err != Err_OK) return -1;

   archi::UInt32 tx_size = 0;
   err = bsi.read (tx_size);  if (err != Err_OK) return -1;
   
   if (tx_size != bsi.size ()) return -1;

   assert (get_controller () == Controller_NONE);
   set_controller (Controller_NETWORK_EXT);
   
   assert (use_object_manager ().is_recycle_bin_empty ());
   
   docs_TRACE ("CTL NETWORK");
   
   std::vector <archi::UByte> report_data;
   ValidationReportWriter report (report_data);
   
   VirtualMachine & virtual_machine = use_virtual_machine ();
   
   const archi::UByte * tx_data_ptr = bsi.data ();
   
   MachineCode machine_code (tx_data_ptr, tx_size);
   machine_code.set_mode_safe ();
   machine_code.bind (report);
   
   err = machine_code.compile ();
   
   if (err == Err_OK)
   {
      virtual_machine.set_mode_safe ();
      virtual_machine.set_mode_direction (tx_id);
      
      virtual_machine.run ();
      
      err = virtual_machine.execute (machine_code);
      
      virtual_machine.stop ();
   }

   Object * root_ptr = use_object_manager ().get_root ();
   assert (root_ptr != 0);
   
   if (err == Err_OK)
   {
      archi::Int32 user_id = SeedGenerator::to_user_id (tx_id);
      
      enable_post (true);
      
      if (_document_validator_ptr != 0)
      {
         _document_validator_ptr->request_validate (report, user_id, *root_ptr);
      }
      
      enable_post (false);
      
      bool ok_flag = report.has_no_error ();
      
      if (!ok_flag)
      {
         err = Err_VM_VALIDATION_FAILED;
         
         // rollback
         
         virtual_machine.set_mode_no_fault ();
         virtual_machine.set_mode_direction (- tx_id);
         
         virtual_machine.run ();
      
         int sub_err = virtual_machine.execute (machine_code);
         
         if (sub_err != Err_OK)
         {
            // rollback failed : panic
            err = Err_VM_INTERNAL_SYSTEM_ERROR;
         }
         
         virtual_machine.stop ();
      }
   }
   
   report.flush ();

   if (err == Err_OK)
   {
      //assert (report_data.empty ());
      
      increment_rev_id ();
      
      send_tx_approval (from, tx_id, _rev_id);
      
      archi::Int32 user_id = SeedGenerator::to_user_id (tx_id);
      
      if (user_id != SeedGenerator::SPECTATOR_USER_ID)
      {
         broadcast_tx (from, tx_data_ptr, tx_size, tx_id, _rev_id);
      }
      
      _com_layer.notify_document_need_flush ();
   }
   else
   {
      if (!report_data.empty ())
      {
         send_tx_refusal (from, tx_id, &report_data [0], report_data.size ());
      }
      else
      {
         send_tx_refusal (from, tx_id, 0, 0);
      }
   }
   
   post_document_change_task ();
   
   set_controller (Controller_NONE);
   
   docs_TRACE ("CTL NONE");
   
   int ret_val = 0;
   
   if (err == Err_VM_INTERNAL_SYSTEM_ERROR)
   {
      ret_val = -2;
   }
   else if (report.is_suspicious ())
   {
      ret_val = -1;
   }
   
   return ret_val;
}



/*
==============================================================================
Name : incoming_ctl_msg
Description :
   A client sent a control message
Note :
   ctl_type    4
   ctl_size    4
   ctl_data    var
==============================================================================
*/

int   DocumentServer::incoming_ctl_msg (const std::string & from, util::BinaryStreamInput & bsi)
{
   docs_TRACE1 ("incoming_ctl_msg: %1%", from.c_str ());
   
   archi::UInt32 ctl_type = 0;
   int err = bsi.read (ctl_type);   if (err != Err_OK) return -1;
   
   switch (ctl_type)
   {
   case StdCtlMsg_HELLO:
      err = incoming_ctl_msg_hello (from, bsi);
      break;

   case StdCtlMsg_SIGNAL:
      err = incoming_ctl_msg_signal (from, bsi);
      break;
   
   default:
      assert (false);
      err = -1;
      break;
   }
   
   return err;
}



/*
==============================================================================
Name : incoming_ctl_msg_hello
==============================================================================
*/

int   DocumentServer::incoming_ctl_msg_hello (const std::string & from, util::BinaryStreamInput & bsi)
{
   int err = 0;
   
   archi::UInt32 ctl_size = 0;
   err = bsi.read (ctl_size); if (err != Err_OK) return -1;
   
   if (ctl_size != bsi.size ()) return -1;

   std::string user_name;
   err = bsi.read (user_name); if (err != Err_OK) return -1;
   
   archi::Int64 rev_id = 0;
   err = bsi.read (rev_id); if (err != Err_OK) return -1;
   
   // quick fix for CM
   // SECURITY PROBLEM
   _int_ext_map [user_name] = from;
   
   bool send_document_flag = false;
   bool req_send_remaining_flag = false;
   
   if (rev_id == 0)
   {
      send_document_flag = true;
   }
   else
   {
      if (_rev_id == rev_id)
      {
         // the user already have the document and the right revision,
         // do nothing
      }
      else
      {
         // spurious deconnection case
         // we are going to send back the document
         // the idea is that the server may have accepted
         // or denied some transactions that the client
         // is not aware of, but all transactions are
         // going to be refused
         
         // worked offline case
         // this is the same, as the client will have
         // store a latent transaction that express the
         // difference from the last known revision to
         // its offline work
         
         send_document_flag = true;
         req_send_remaining_flag = true;
      }
      
//       #warning we need to send the user id, we must know it
   }
   
   // send user id
   {
      UserManager & user_manager = use_user_manager ();
      
      bool user_exist_flag = user_manager.has_user (user_name);
      
      archi::Int32 user_id = 0;
      
      if (user_exist_flag)
      {
         user_id = user_manager.get_user_id (user_name);
      }
      else
      {
         // this is a new client with no user nbr
         
         user_id = user_manager.create_user_id (user_name);
      }
      
      assert (user_id >= SeedGenerator::EDITOR_USER_ID_START_NBR);

      docs_TRACE2 ("broadcast_user_id: %1% %2%", user_name.c_str (), user_id);
      broadcast_user_id (user_name, user_id);
   }
   
   if (send_document_flag)
   {
      const std::string & to = from;
         
      if (_cached_doc_data.empty ())
      {
         std::string document;
         write_mem (document);
         
         docs_TRACE ("StdCtlMsg_SET_DOC");
         send_ctl_msg (
            to, StdCtlMsg_SET_DOC,
            (const archi::UByte *) document.c_str (), document.size ()
         );
      }
      else
      {
         docs_TRACE ("StdCtlMsg_SET_DOC (cached)");
         send_ctl_msg (
            to, StdCtlMsg_SET_DOC,
            &_cached_doc_data [0], _cached_doc_data.size ()
         );
      }
   }
   else
   {
      const std::string & to = from;
      
      docs_TRACE ("StdCtlMsg_DOC_OK");
      send_ctl_msg (
         to, StdCtlMsg_DOC_OK,
         0, 0
      );
   }

   /*if (req_send_remaining_flag)
   {
      const std::string & to = from;
      
      _post_man.send_ctl_msg (
         to, StdCtlMsg_REQ_REM_LAT,
         0, 0
      );
   }*/
   
   return Err_OK;
}



/*
==============================================================================
Name : incoming_ctl_msg_signal
==============================================================================
*/

int   DocumentServer::incoming_ctl_msg_signal (const std::string & from, util::BinaryStreamInput & bsi)
{
   int err = 0;
   
   archi::UInt32 ctl_size = 0;
   err = bsi.read (ctl_size); if (err != Err_OK) return -1;
   
   archi::Int64 ref = 0;
   err = bsi.read (ref);   if (err != Err_OK) return -1;
   
   archi::UInt32 type = 0;
   err = bsi.read (type);  if (err != Err_OK) return -1;
   
   archi::UInt32 data_size = 0;
   err = bsi.read (data_size);   if (err != Err_OK) return -1;
   
   const archi::UByte * data_ptr = 0;
   err = bsi.read (data_ptr, data_size);  if (err != Err_OK) return -1;
   
   // might be 0
   Object * obj_ptr = get_object (ref);
   
   if (_document_signal_ptr != 0)
   {
      _document_signal_ptr->signal (from, obj_ptr, type, data_ptr, data_size);
   }
   
   return err;
}



/*
==============================================================================
Name : broadcast_user_id
Description :
   The client except to receive his 'user_id' first.
   We need to send the user ids of the other already resident clients.
   All clients must receive the new user id.
   
   !!! However we cannot use 'broadcast_ctl_msg' because of concurrency
   problem in the server upper, which handle it synchronously !!!
   So that we handle manually broadcasting, from what we have.
   
   The 'user_manager' is already filled with the new user_name/user_id
   
==============================================================================
*/

void  DocumentServer::broadcast_user_id (const std::string & user_name, archi::Int32 user_id)
{
   assert (user_name.size () <= Constants::MAX_USER_NAME_SIZE);
         
   static const size_t buf_size = Constants::MAX_USER_NAME_SIZE + sizeof (user_id);
   
   archi::UByte buf [buf_size];
   
   util::BinaryStreamOutputNa bso (buf, buf_size);
   bso.write (user_name);
   bso.write (archi::UInt32 (user_id));
   
   UserManager & user_manager = use_user_manager ();
   
   const UserManager::UserNameIdMap & user_name_id_map
      = user_manager.get_user_name_id_map ();
   
   const std::string & user_name_to = _int_ext_map [user_name];
   assert (user_name_to != "");
            
   // first to requester
   {
      // quick fix for CM
      
      const std::string & to = user_name_to;
      
      // first user name
      send_ctl_msg (to, StdCtlMsg_SET_USER_ID, bso.data (), bso.size ());
      
      // then others user id
      
      UserManager::UserNameIdMap::const_iterator it = user_name_id_map.begin ();
      const UserManager::UserNameIdMap::const_iterator it_end = user_name_id_map.end ();
      
      for (; it != it_end ; ++it)
      {
         const std::string & sub_user_name = it->first;
         archi::Int32 sub_user_id = it->second;
         
         if (sub_user_name != user_name)
         {
            archi::UByte sub_buf [buf_size];
   
            util::BinaryStreamOutputNa sub_bso (sub_buf, buf_size);
            sub_bso.write (sub_user_name);
            sub_bso.write (archi::UInt32 (sub_user_id));
            
            send_ctl_msg (to, StdCtlMsg_SET_USER_ID, sub_bso.data (), sub_bso.size ());
         }
      }
   }
   
   // then to all other except requester
   {
      NameMap::iterator it = _int_ext_map.begin ();
      const NameMap::iterator it_end = _int_ext_map.end ();
      
      for (; it != it_end ; ++it)
      {
         const std::string & to = it->second;
         
         if (to != user_name_to)
         {
            send_ctl_msg (to, StdCtlMsg_SET_USER_ID, bso.data (), bso.size ());
         }
      }
   }
}



/*
==============================================================================
Name : broadcast_tx
==============================================================================
*/

void  DocumentServer::broadcast_tx (const std::string & except, const archi::UByte * tx_data_ptr, archi::UInt32 tx_size, ohm::archi::Int64 tx_id, ohm::archi::Int64 rev_id)
{
   docs_TRACE1 ("broadcast_tx except: %1%", except.c_str ());
   
   assert (tx_data_ptr != 0);
   assert (tx_size > 0);
   
#if defined (OHM_FLIP_DOCUMENT_SERVER_INTERNAL_BROADCAST)
   NameMap::iterator it = _int_ext_map.begin ();
   const NameMap::iterator it_end = _int_ext_map.end ();
   
   for (; it != it_end ; ++it)
   {
      const std::string & to = it->second;
      
      if (to != except)
      {
         send_tx (to, tx_data_ptr, tx_size, tx_id, rev_id);
      }
   }
   
#else 
   std::vector <archi::UByte> data;
   
   util::BinaryStreamOutput bso (data);
   bso.write (archi::UByte (ComLayerMsgType_SC_TX_EXT));
   
   bso.write (tx_id);
   bso.write (rev_id);
   bso.write (archi::UInt32 (tx_size));
   bso.write (tx_data_ptr, tx_size);

   _com_layer.broadcast (except, &data [0], data.size ());

#endif
}



/*
==============================================================================
Name : broadcast_tx
==============================================================================
*/

void  DocumentServer::broadcast_tx (const archi::UByte * tx_data_ptr, archi::UInt32 tx_size, ohm::archi::Int64 tx_id, ohm::archi::Int64 rev_id)
{
   docs_TRACE ("broadcast_tx");
   
   assert (tx_data_ptr != 0);
   assert (tx_size > 0);
   
#if defined (OHM_FLIP_DOCUMENT_SERVER_INTERNAL_BROADCAST)
   NameMap::iterator it = _int_ext_map.begin ();
   const NameMap::iterator it_end = _int_ext_map.end ();
   
   for (; it != it_end ; ++it)
   {
      const std::string & to = it->second;
      
      send_tx (to, tx_data_ptr, tx_size, tx_id, rev_id);
   }
   
#else 
   std::vector <archi::UByte> data;
   
   util::BinaryStreamOutput bso (data);
   bso.write (archi::UByte (ComLayerMsgType_SC_TX_EXT));
   
   bso.write (tx_id);
   bso.write (rev_id);
   bso.write (archi::UInt32 (tx_size));
   bso.write (tx_data_ptr, tx_size);
   
   _com_layer.broadcast (&data [0], data.size ());

#endif
}



/*
==============================================================================
Name : send_tx_approval
==============================================================================
*/

void  DocumentServer::send_tx_approval (const std::string & to, ohm::archi::Int64 tx_id, ohm::archi::Int64 rev_id)
{
   docs_TRACE1 ("send_tx_approval: %1%", to.c_str ());
   
   static const size_t buf_size
      = sizeof (archi::UByte) + sizeof (tx_id) + sizeof (rev_id);
   
   archi::UByte buf [buf_size];
   
   util::BinaryStreamOutputNa bso (buf, buf_size);
   bso.write (archi::UByte (ComLayerMsgType_SC_TX_ACK));
   
   bso.write (tx_id);
   bso.write (rev_id);
   
   _com_layer.send (to, buf, buf_size);
}



/*
==============================================================================
Name : send_tx_refusal
==============================================================================
*/

void  DocumentServer::send_tx_refusal (const std::string & to, ohm::archi::Int64 tx_id, const archi::UByte * report_data_ptr, archi::UInt32 report_size)
{
   docs_TRACE1 ("send_tx_refusal: %1%", to.c_str ());
   
   if (report_size == 0)
   {
      assert (report_data_ptr == 0);
      
      static const size_t buf_size
         = sizeof (archi::UByte) + sizeof (tx_id);
      
      archi::UByte buf [buf_size];
      
      util::BinaryStreamOutputNa bso (buf, buf_size);
      
      bso.write (archi::UByte (ComLayerMsgType_SC_TX_DENY));
      
      bso.write (tx_id);
      bso.write (report_size);
      
      _com_layer.send (to, buf, buf_size);
   }
   else
   {
      assert (report_data_ptr != 0);

      std::vector <archi::UByte> data;
      
      util::BinaryStreamOutput bso (data);
      bso.write (archi::UByte (ComLayerMsgType_SC_TX_DENY));
      
      bso.write (tx_id);
      bso.write (report_size);
      bso.write (report_data_ptr, report_size);
      
      _com_layer.send (to, &data [0], data.size ());
   }
}



/*
==============================================================================
Name : send_tx
==============================================================================
*/

void  DocumentServer::send_tx (const std::string & to, const archi::UByte * tx_data_ptr, archi::UInt32 tx_size, ohm::archi::Int64 tx_id, ohm::archi::Int64 rev_id)
{
   docs_TRACE1 ("send_tx: %1%", to.c_str ());
   
   assert (tx_data_ptr != 0);
   assert (tx_size > 0);
   
   std::vector <archi::UByte> data;
   
   util::BinaryStreamOutput bso (data);
   bso.write (archi::UByte (ComLayerMsgType_SC_TX_EXT));
   
   bso.write (tx_id);
   bso.write (rev_id);
   bso.write (archi::UInt32 (tx_size));
   bso.write (tx_data_ptr, tx_size);
   
   _com_layer.send (to, &data [0], data.size ());
}



/*
==============================================================================
Name : broadcast_ctl_msg
==============================================================================
*/

void  DocumentServer::broadcast_ctl_msg (archi::UInt32 type, const archi::UByte * ctl_data_ptr, archi::UInt32 ctl_size)
{
   docs_TRACE ("broadcast_ctl_msg");
   
#if defined (OHM_FLIP_DOCUMENT_SERVER_INTERNAL_BROADCAST)
   NameMap::iterator it = _int_ext_map.begin ();
   const NameMap::iterator it_end = _int_ext_map.end ();
   
   for (; it != it_end ; ++it)
   {
      const std::string & to = it->second;
      
      send_ctl_msg (to, type, ctl_data_ptr, ctl_size);
   }
   
#else
   if (ctl_size == 0)
   {
      assert (ctl_data_ptr == 0);
      
      static const size_t buf_size
         = sizeof (archi::UByte) + sizeof (type) + sizeof (ctl_size);
      
      archi::UByte buf [buf_size];
      
      util::BinaryStreamOutputNa bso (buf, buf_size);
      
      bso.write (archi::UByte (ComLayerMsgType_SC_CTL));
      
      bso.write (type);
      bso.write (ctl_size);
      
      _com_layer.broadcast (buf, buf_size);
   }
   else
   {
      assert (ctl_data_ptr != 0);
      
      std::vector <archi::UByte> data;
      
      util::BinaryStreamOutput bso (data);
      bso.write (archi::UByte (ComLayerMsgType_SC_CTL));
      
      bso.write (type);
      bso.write (ctl_size);
      bso.write (ctl_data_ptr, ctl_size);
      
      _com_layer.broadcast (&data [0], data.size ());
   }

#endif
}



/*
==============================================================================
Name : broadcast_ctl_msg
==============================================================================
*/

void  DocumentServer::broadcast_ctl_msg (const std::string & except, archi::UInt32 type, const archi::UByte * ctl_data_ptr, archi::UInt32 ctl_size)
{
   docs_TRACE1 ("broadcast_ctl_msg except: %1%", except.c_str ());
   
#if defined (OHM_FLIP_DOCUMENT_SERVER_INTERNAL_BROADCAST)
   NameMap::iterator it = _int_ext_map.begin ();
   const NameMap::iterator it_end = _int_ext_map.end ();
   
   for (; it != it_end ; ++it)
   {
      const std::string & to = it->second;
      
      if (to != except)
      {
         send_ctl_msg (to, type, ctl_data_ptr, ctl_size);
      }
   }
   
#else
   if (ctl_size == 0)
   {
      assert (ctl_data_ptr == 0);
      
      static const size_t buf_size
         = sizeof (archi::UByte) + sizeof (type) + sizeof (ctl_size);
      
      archi::UByte buf [buf_size];
      
      util::BinaryStreamOutputNa bso (buf, buf_size);
      
      bso.write (archi::UByte (ComLayerMsgType_SC_CTL));
      
      bso.write (type);
      bso.write (ctl_size);
      
      _com_layer.broadcast (except, buf, buf_size);
   }
   else
   {
      assert (ctl_data_ptr != 0);
      
      std::vector <archi::UByte> data;
      
      util::BinaryStreamOutput bso (data);
      bso.write (archi::UByte (ComLayerMsgType_SC_CTL));
      
      bso.write (type);
      bso.write (ctl_size);
      bso.write (ctl_data_ptr, ctl_size);
      
      _com_layer.broadcast (except, &data [0], data.size ());
   }
#endif
}



/*
==============================================================================
Name : send_ctl_msg
==============================================================================
*/

void  DocumentServer::send_ctl_msg (const std::string & to, archi::UInt32 type, const archi::UByte * ctl_data_ptr, archi::UInt32 ctl_size)
{
   docs_TRACE3 ("send_ctl_msg: %1% type:%2% size:%3%", to.c_str (), type, ctl_size);
   
   if (ctl_size == 0)
   {
      assert (ctl_data_ptr == 0);
      
      static const size_t buf_size
         = sizeof (archi::UByte) + sizeof (type) + sizeof (ctl_size);
      
      archi::UByte buf [buf_size];
      
      util::BinaryStreamOutputNa bso (buf, buf_size);
      
      bso.write (archi::UByte (ComLayerMsgType_SC_CTL));
      
      bso.write (type);
      bso.write (ctl_size);
      
      _com_layer.send (to, buf, buf_size);
   }
   else
   {
      assert (ctl_data_ptr != 0);
      
      std::vector <archi::UByte> data;
      
      util::BinaryStreamOutput bso (data);
      bso.write (archi::UByte (ComLayerMsgType_SC_CTL));
      
      bso.write (type);
      bso.write (ctl_size);
      bso.write (ctl_data_ptr, ctl_size);
      
      _com_layer.send (to, &data [0], data.size ());
   }
}



/*
==============================================================================
Name : increment_rev_id
==============================================================================
*/

void  DocumentServer::increment_rev_id ()
{
   ++_rev_id;
   
   if (!_cached_doc_data.empty ())
   {
      // release '_cached_doc_data' memory

      Data ().swap (_cached_doc_data);
   }
}



/*
==============================================================================
Name : post_document_change_task
==============================================================================
*/

void  DocumentServer::post_document_change_task ()
{
   enable_post (true);
   
   post_reorder_containers ();
   pre_sync ();
   
   use_object_manager ().empty_recycle_bin ();
   
   Object * root_ptr = use_object_manager ().get_root ();
   
   if (get_controller () == Controller_DOC_RELEASE)
   {
   }
   else
   {
      assert (root_ptr != 0);
      
      root_ptr->synchronize ();
   }
   
   enable_post (false);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
