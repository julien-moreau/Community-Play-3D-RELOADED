/*****************************************************************************

        SessCentral.h
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



#if ! defined (plex_SessCentral_HEADER_INCLUDED)
#define  plex_SessCentral_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/ComLayerServerBase.h"

#include "ohm/archi/types.h"



namespace ohm
{
namespace flip
{
class DocumentServer;
}

namespace util
{
class BinaryStreamInput;
}
}



namespace model
{
class ModelValidator;
class ModelTweaker;
}



namespace plex
{



class SessSession;
class Storage;

class SessCentral
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  SessCentral (const std::string & session_id);
   virtual        ~SessCentral ();

   void           run ();
   
   void           status ();
   void           send (const std::string & to, const ohm::archi::UByte * data_ptr, size_t data_size);
   void           broadcast (const ohm::archi::UByte * data_ptr, size_t data_size);
   void           broadcast (const std::string & except, const ohm::archi::UByte * data_ptr, size_t data_size);
   void           document_need_flush ();
   
   void           notify_storage_idle ();
   void           notify_storage_reading (size_t cur);
   void           notify_storage_writing (size_t cur, size_t total);
   void           notify_storage_copying ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   
   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           send_msg (const std::vector <ohm::archi::UByte> & data);
   
   void           process ();
   
   bool           process_command ();
   void           process_tx (ohm::util::BinaryStreamInput & bsi);
   void           process_disconnected (ohm::util::BinaryStreamInput & bsi);
   void           process_flush (ohm::util::BinaryStreamInput & bsi);
   void           process_stop (ohm::util::BinaryStreamInput & bsi);

   bool           read (ohm::archi::UByte * data_ptr, size_t data_size);
   bool           write (const std::vector <ohm::archi::UByte> & data);
   bool           write (const ohm::archi::UByte * data_ptr, size_t data_size);
   
   const std::string
                  _session_id;
   
   std::auto_ptr <SessSession>
                  _session_aptr;
   std::auto_ptr <ohm::flip::DocumentServer>
                  _document_server_aptr;
   std::auto_ptr <model::ModelValidator>
                  _model_validator_aptr;
   std::auto_ptr <model::ModelTweaker>
                  _model_tweaker_aptr;
   std::auto_ptr <Storage>
                  _storage_aptr;
   
   bool           _run_flag;
   bool           _flush_flag;
   double         _flush_ts;
   
   double         _last_sample_time_s;
   size_t         _cpu;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  SessCentral ();
                  SessCentral (const SessCentral &other);
   SessCentral &     operator = (const SessCentral &other);
   bool           operator == (const SessCentral &other);
   bool           operator != (const SessCentral &other);

}; // class SessCentral



}  // namespace plex



//#include  "SessCentral.hpp"



#endif   // plex_SessCentral_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

