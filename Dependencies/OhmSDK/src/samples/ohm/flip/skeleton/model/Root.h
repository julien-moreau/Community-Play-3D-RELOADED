/*****************************************************************************

        Root.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70455

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



#if ! defined (model_Root_HEADER_INCLUDED)
#define  model_Root_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "model/ClientDataColl.h"
#include "model/SpacialObjectColl.h"

#include "ohm/flip/Object.h"
#include "ohm/flip/TxSessionGuard.h"
#include "ohm/flip/Scribable.h"



namespace model
{



class Root
:  public ohm::flip::Object
,  public ohm::flip::Scribable
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum Signal
   {
                  Signal_INIT_GUI = 0,
   };
   
   enum SignalServer
   {
                  SignalServer_DO_SOMETHING = 0,
   };
   
   static void    declare ();

                  Root (ohm::flip::DocumentBase & document);
   virtual        ~Root ();

   void           ext_init ();
   
   ohm::flip::Scribe &
                  use_scribe ();
   void           set_scribe_metadata (const std::string & metadata);
   bool           has_undo () const;
   void           get_undo_annotation (std::string & annotation);
   int            undo (std::string & annotation);
   bool           has_redo () const;
   void           get_redo_annotation (std::string & annotation);
   int            redo (std::string & annotation);
   
   // ohm::flip::Scribable
   virtual void   push_history (const ohm::flip::DataMaster & tx_data, const ohm::flip::AnnotationSPtr & annotation_sptr);
   
   void           update_client_data_cache ();
   
   void           signal_init_gui ();
   void           signal_server_do_something ();

   ClientDataColl::iterator
                  ext_add_client_data ();
   ClientDataColl::iterator
                  find (const std::string & user_name, ohm::flip::FindMask find_mask = ohm::flip::FindMask_NATURAL);
   ClientDataColl::iterator
                  find_from_object (const ohm::flip::Object & object, ohm::flip::FindMask find_mask = ohm::flip::FindMask_NATURAL);
   ClientDataColl::const_iterator
                  find_from_object (const ohm::flip::Object & object, ohm::flip::FindMask find_mask = ohm::flip::FindMask_NATURAL) const;
   ClientDataColl &
                  use_client_data_coll ();
   const ClientDataColl &
                  use_client_data_coll () const;
   ClientData *   use_self_client_data_ptr ();
   const ClientData *
                  use_self_client_data_ptr () const;
   
   SpacialObjectColl::iterator
                  tx_add_taxi (ohm::archi::Float64 x, ohm::archi::Float64 y, ohm::archi::Float64 width, Taxi::Color color);
   SpacialObjectColl::iterator
                  ext_add_taxi (ohm::archi::Float64 x, ohm::archi::Float64 y, ohm::archi::Float64 width, Taxi::Color color);
   SpacialObjectColl::iterator
                  ext_add_taxi (Taxi & taxi);
   SpacialObjectColl::iterator
                  tx_add_pedestrian (ohm::archi::Float64 x, ohm::archi::Float64 y);
   SpacialObjectColl::iterator
                  ext_add_pedestrian (ohm::archi::Float64 x, ohm::archi::Float64 y);
   SpacialObjectColl::iterator
                  ext_add_pedestrian (Pedestrian & pedestrian);
   void           ext_erase (SpacialObjectColl::iterator it);
   SpacialObjectColl &
                  use_spacial_object_coll ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   class PredicateClient
   {
   public:
                  PredicateClient (const std::string & user_name);
      
      bool        equal (const ClientData & client_data) const;
   
   private:
      const std::string
                  _user_name;
   private:
                  PredicateClient ();
                  PredicateClient (const PredicateClient & other);
   };

   class PredicateUserId
   {
   public:
                  PredicateUserId (ohm::archi::Int32 user_id);
      
      bool        equal (const ClientData & client_data) const;
   
   private:
      const ohm::archi::Int32
                  _user_id;
   private:
                  PredicateUserId ();
                  PredicateUserId (const PredicateUserId & other);
   };

   ClientDataColl _client_data_coll;
   SpacialObjectColl
                  _spatial_object_coll;

   mutable ClientData * 
                  _self_client_data_ptr;
   
   ohm::flip::TxSessionGuard
                  _tx_session_guard;
   ohm::flip::Scribe
                  _scribe;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Root ();
                  Root (const Root &other);
   Root &         operator = (const Root &other);
   bool           operator == (const Root &other);
   bool           operator != (const Root &other);

}; // class Root



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace model



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "model/Root.hpp"



#endif   // model_Root_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
