/*****************************************************************************

        DocumentServer.h
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



#if ! defined (ohm_flip_DocumentServer_HEADER_INCLUDED)
#define  ohm_flip_DocumentServer_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/ComLayerServer.h"
#include "ohm/flip/private/DocumentBase.h"

#include <map>



namespace ohm
{
namespace flip
{



class DocumentValidatorMaster;
class DocumentSignal;
class ValidationReport;
class DocumentTweakerMaster;

class DataStreamOutput;
class DataStreamInput;

class DocumentServer
:  public DocumentBase
,  public ComLayerServerListener
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef int (*ConverterProc) (DataStreamOutput &, DataStreamInput &, bool);
   
                  DocumentServer (ComLayerServer & com_layer);
   virtual        ~DocumentServer ();
   
   void           bind (DocumentValidatorMaster & validator);
   void           bind (DocumentSignal & signal);
   void           bind (ConverterProc converter_proc);
   void           bind (DocumentTweakerMaster & tweaker);
   
   int            read_mem (const archi::UByte * data_ptr, size_t data_size);
   int            read_disk (const char * path_0);

   void           write_mem (std::vector <archi::UByte> & data);
   void           write_mem_no_comp (std::vector <archi::UByte> & data);
   void           write_mem (std::string & document);
   void           write_disk (const char * path_0);
   
   // DocumentBase
   virtual archi::Int64
                  get_rev_id ();
   
   // ComLayerServerListener
   virtual int    receive (const std::string & from, util::BinaryStreamInput & bsi);
   virtual void   notify_disconnected (const std::string & from);
   
   void           send_signal_client (const std::string & to, archi::Int64 ref, archi::UInt32 type, const archi::UByte * data_ptr, archi::UInt32 data_size);
   
   void           begin_transaction ();
   void           end_transaction ();
   void           commit_transaction ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   typedef std::map <std::string, std::string>  NameMap;
   
   int            uncompress (Data & alt_data, const archi::UByte * & data_ptr, size_t & data_size);
   int            read (const archi::UByte * data_ptr, size_t data_size);
   
   int            incoming_transaction (const std::string & from, util::BinaryStreamInput & bsi);
   int            incoming_ctl_msg (const std::string & from, util::BinaryStreamInput & bsi);
   int            incoming_ctl_msg_hello (const std::string & from, util::BinaryStreamInput & bsi);
   int            incoming_ctl_msg_signal (const std::string & from, util::BinaryStreamInput & bsi);
   
   void           broadcast_user_id (const std::string & user_name, archi::Int32 user_id);
   
   void           broadcast_tx (const std::string & except, const archi::UByte * tx_data_ptr, archi::UInt32 tx_size, ohm::archi::Int64 tx_id, ohm::archi::Int64 rev_id);
   void           broadcast_tx (const archi::UByte * tx_data_ptr, archi::UInt32 tx_size, ohm::archi::Int64 tx_id, ohm::archi::Int64 rev_id);
   void           send_tx_approval (const std::string & to, ohm::archi::Int64 tx_id, ohm::archi::Int64 rev_id);
   void           send_tx_refusal (const std::string & to, ohm::archi::Int64 tx_id, const archi::UByte * report_data_ptr, archi::UInt32 report_size);
   void           send_tx (const std::string & to, const archi::UByte * tx_data_ptr, archi::UInt32 tx_size, ohm::archi::Int64 tx_id, ohm::archi::Int64 rev_id);
   void           broadcast_ctl_msg (archi::UInt32 type, const archi::UByte * ctl_data_ptr, archi::UInt32 ctl_size);
   void           broadcast_ctl_msg (const std::string & except, archi::UInt32 type, const archi::UByte * ctl_data_ptr, archi::UInt32 ctl_size);
   void           send_ctl_msg (const std::string & to, archi::UInt32 type, const archi::UByte * ctl_data_ptr, archi::UInt32 ctl_size);
   void           increment_rev_id ();
   
   void           post_document_change_task ();
   
   typedef std::map <std::string, archi::Int32> ClientNameIdMap;
   
   ComLayerServer &
                  _com_layer;
   
   archi::Int64   _rev_id;
   TxIdGenerator  _tx_id_generator;
   
   ConverterProc  _converter_proc;
   
   DocumentValidatorMaster *
                  _document_validator_ptr;
   DocumentSignal *
                  _document_signal_ptr;
   NameMap        _int_ext_map;

   DocumentTweakerMaster *
                  _document_tweaker_ptr;
   
   Data           _cached_doc_data;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DocumentServer ();
                  DocumentServer (const DocumentServer &other);
   DocumentServer &
                  operator = (const DocumentServer &other);
   bool           operator == (const DocumentServer &other);
   bool           operator != (const DocumentServer &other);

}; // class DocumentServer



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/DocumentServer.hpp"



#endif   // ohm_flip_DocumentServer_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
