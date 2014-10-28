/*****************************************************************************

        DocumentLocal.h
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 63180

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



#if ! defined (ohm_flip_DocumentLocal_HEADER_INCLUDED)
#define  ohm_flip_DocumentLocal_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/local/ServerLocal.h"
#include "ohm/flip/local/ClientLocal.h"

#include "ohm/archi/types.h"



namespace ohm
{
namespace flip
{



class DocumentObserverMaster;
class DocumentClientMasterSlot;
class Object;
class DataMaster;

class DocumentLocal
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class AutoUserInputs
   {
   public:
                  AutoUserInputs (DocumentLocal & document);
      virtual     ~AutoUserInputs ();
   
   private:
      DocumentLocal &
                  _document;
   };
   
                  DocumentLocal (DocumentObserverMaster & observer);
   virtual        ~DocumentLocal ();
   
   void           init (const std::string & path);
   void           restore ();
   
   DocumentClientMasterSlot &
                  create (DocumentClientMaster & document_master);
   void           pre_release ();
   void           release ();
   
   void           notify_pre_user_inputs_tasks ();
   void           notify_post_user_inputs_tasks ();
   
   template <class T, class U>
   static void    signal (U & receiver, bool & continue_flag, Object * obj_ptr, archi::UInt32 type, const DataMaster & signal_data);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   friend class AutoUserInputs;
   
   DocumentObserverMaster &
                  _observer;

   std::auto_ptr <ServerLocal>
                  _server_aptr;
   std::auto_ptr <ClientLocal>
                  _client_aptr;
   
   std::auto_ptr <DocumentClientMasterSlot>
                  _slot_aptr;
   flip::DocumentClient *
                  _document_ptr;
   
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DocumentLocal ();
                  DocumentLocal (const DocumentLocal &other);
   DocumentLocal &
                  operator = (const DocumentLocal &other);
   bool           operator == (const DocumentLocal &other);
   bool           operator != (const DocumentLocal &other);

}; // class DocumentLocal



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/flip/DocumentLocal.hpp"



#endif   // ohm_flip_DocumentLocal_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
