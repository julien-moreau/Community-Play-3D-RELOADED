/*****************************************************************************

        Root.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70462

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

#include "model/Root.h"

#include "ohm/flip/ClassDescription.h"
#include "ohm/util/BinaryStreamInput.h"
#include "ohm/util/BinaryStreamOutput.h"

#include <cassert>

// To be sync in model/conv/VersionTree.cpp
#define SKELETON_MODEL_VERSION "Skeleton.Rev0"



namespace model
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : declare
==============================================================================
*/

void  Root::declare ()
{
   using namespace ohm::flip;
   
   ClassDescription <Root>::use ().set_name ("skeleton.Root");
   ClassDescription <Root>::use ().push_var_desc (&Root::_client_data_coll, "_client_data_coll");
   ClassDescription <Root>::use ().push_var_desc (&Root::_spatial_object_coll, "_spatial_object_coll");
   
   ClassDescription <Root>::use ().enable_root (SKELETON_MODEL_VERSION);
   
   ClassDescManager::declare (ClassDescription <Root>::use ());
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Root::Root (ohm::flip::DocumentBase & document)
:  ohm::flip::Object (document)
,  _client_data_coll (document)
,  _spatial_object_coll (document)

,  _self_client_data_ptr (0)

,  _tx_session_guard (document)
,  _scribe (*this)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Root::~Root ()
{
}



/*
==============================================================================
Name : ext_init
Note :
   Called in server only when creating the document.
==============================================================================
*/

void  Root::ext_init ()
{
   // add a few taxis
   
   ext_add_taxi (100, 200, Taxi::_min_width, Taxi::Color_WHITE);
   ext_add_taxi (200, 100, Taxi::_min_width, Taxi::Color_YELLOW);

   // add a few pedestrians
   
   ext_add_pedestrian (500, 150);
   ext_add_pedestrian (150, 500);
}




/*
==============================================================================
Name : use_scribe
==============================================================================
*/

ohm::flip::Scribe &  Root::use_scribe ()
{
   return _scribe;
}



/*
==============================================================================
Name : set_scribe_metadata
==============================================================================
*/

void  Root::set_scribe_metadata (const std::string & metadata)
{
   _scribe.annotate ("main", metadata);
}



/*
==============================================================================
Name : has_undo
==============================================================================
*/

bool  Root::has_undo () const
{
   return _scribe.has_undo ();
}



/*
==============================================================================
Name : get_undo_annotation
==============================================================================
*/

void  Root::get_undo_annotation (std::string & annotation)
{
   using namespace ohm;
   
   assert (has_undo ());
   
   flip::AnnotationSPtr annotation_sptr;
   
   _scribe.get_undo_annotation (annotation_sptr);
   
   flip::Annotation::iterator it = annotation_sptr->find ("main");
      
   assert (it != annotation_sptr->end ());
   
   if (it != annotation_sptr->end ())
   {
      annotation = it->second;
   }
   else
   {
      ohm_util_TRACE_ERROR ("missing undo annotation");
   }
}



/*
==============================================================================
Name : undo
==============================================================================
*/

int   Root::undo (std::string & annotation)
{
   using namespace ohm;
   
   flip::AnnotationSPtr annotation_sptr;
   
   int err = _scribe.undo_history (annotation_sptr);
   
   if (err == 0)
   {
      flip::Annotation::iterator it = annotation_sptr->find ("main");
      
      assert (it != annotation_sptr->end ());
      
      if (it != annotation_sptr->end ())
      {
         annotation = it->second;
      }
      else
      {
         ohm_util_TRACE_ERROR ("missing undo annotation");
      }
   }
   
   return err;
}



/*
==============================================================================
Name : has_redo
==============================================================================
*/

bool  Root::has_redo () const
{
   return _scribe.has_redo ();
}



/*
==============================================================================
Name : get_redo_annotation
==============================================================================
*/

void  Root::get_redo_annotation (std::string & annotation)
{
   using namespace ohm;
   
   assert (has_redo ());
   
   flip::AnnotationSPtr annotation_sptr;
   
   _scribe.get_redo_annotation (annotation_sptr);
   
   flip::Annotation::iterator it = annotation_sptr->find ("main");
      
   assert (it != annotation_sptr->end ());
   
   if (it != annotation_sptr->end ())
   {
      annotation = it->second;
   }
   else
   {
      ohm_util_TRACE_ERROR ("missing redo annotation");
   }
}



/*
==============================================================================
Name : redo
==============================================================================
*/

int   Root::redo (std::string & annotation)
{
   using namespace ohm;

   flip::AnnotationSPtr annotation_sptr;
   
   int err = _scribe.redo_history (annotation_sptr);
   
   if (err == 0)
   {
      flip::Annotation::iterator it = annotation_sptr->find ("main");
      
      assert (it != annotation_sptr->end ());
      
      if (it != annotation_sptr->end ())
      {
         annotation = it->second;
      }
      else
      {
         ohm_util_TRACE_ERROR ("missing redo annotation");
      }
   }
   
   return err;
}



/*
==============================================================================
Name : push_history
==============================================================================
*/

void  Root::push_history (const ohm::flip::DataMaster & tx_data, const ohm::flip::AnnotationSPtr & annotation_sptr)
{
   _scribe.write_history (tx_data, annotation_sptr);
}



/*
==============================================================================
Name: update_client_data_cache
==============================================================================
*/

void  Root::update_client_data_cache ()
{
   using namespace ohm;
   
   if (is_invalid ())
   {
      SpacialObjectColl::iterator it = _spatial_object_coll.begin (flip::FindMask_ALL);
      const SpacialObjectColl::iterator it_end = _spatial_object_coll.end (flip::FindMask_ALL);
      
      for (; it != it_end ; ++it)
      {
         SpacialObject & spacial_object = *it;
         
         spacial_object.update_client_data_cache ();
      }
   }
}



/*
==============================================================================
Name : signal_init_gui
==============================================================================
*/

void  Root::signal_init_gui ()
{
   send_signal (Signal_INIT_GUI);
}



/*
==============================================================================
Name: signal_server_do_something
==============================================================================
*/

void  Root::signal_server_do_something ()
{
   using namespace ohm;
   
   flip::DataSPtr data_sptr (new flip::Data);
   
   util::BinaryStreamOutput bso (*data_sptr);
   
   bso.write (std::string ("some data"));
   
   send_signal_server (SignalServer_DO_SOMETHING, data_sptr);
}



/*
==============================================================================
Name : ext_add_client_data
==============================================================================
*/

ClientDataColl::iterator   Root::ext_add_client_data ()
{
   using namespace ohm;
   
   // force search
   use_self_client_data_ptr ();
   
   ClientDataColl::iterator it = _client_data_coll.end ();
   
   if (_self_client_data_ptr == 0)
   {
      const std::string & user_name
         = use_document ().use_user_manager ().get_self_user_name ();
      
      it = _client_data_coll.insert (
         flip::Args ()
            .push (user_name)
      );

      _self_client_data_ptr = &*it;
   }
   else
   {
      it = _client_data_coll.find (*_self_client_data_ptr);
   }
   
   assert (it != _client_data_coll.end ());

   return it;
}



/*
==============================================================================
Name : find
==============================================================================
*/

ClientDataColl::iterator   Root::find (const std::string & user_name, ohm::flip::FindMask find_mask)
{
   PredicateClient predicate (user_name);
   
   return _client_data_coll.find (predicate, find_mask);
}



/*
==============================================================================
Name: find
Note: returns the ClientData that matches the object's user id
==============================================================================
*/

ClientDataColl::iterator Root::find_from_object (const ohm::flip::Object & object, ohm::flip::FindMask find_mask)
{
   PredicateUserId predicate (object.get_user_id ());
   
   return _client_data_coll.find (predicate, find_mask);
}



/*
==============================================================================
Name: find_from_object
==============================================================================
*/

ClientDataColl::const_iterator Root::find_from_object (const ohm::flip::Object & object, ohm::flip::FindMask find_mask) const
{
   PredicateUserId predicate (object.get_user_id ());
   
   return _client_data_coll.find (predicate, find_mask);
}



/*
==============================================================================
Name : use_client_data_coll
==============================================================================
*/

ClientDataColl &  Root::use_client_data_coll ()
{
   return _client_data_coll;
}



/*
==============================================================================
Name: use_client_data_coll
==============================================================================
*/

const ClientDataColl & Root::use_client_data_coll () const
{
   return _client_data_coll;
}



/*
==============================================================================
Name : use_self_client_data_ptr
==============================================================================
*/

ClientData *   Root::use_self_client_data_ptr ()
{
   using namespace ohm;
   
   // we can cache the client data ptr because it is not going to change
   
   if (_self_client_data_ptr == 0)
   {
      archi::Int32 user_id = use_document ().get_user_id ();
   
      ClientDataColl::iterator it = _client_data_coll.begin ();
      const ClientDataColl::iterator it_end = _client_data_coll.end ();
      
      for (; it != it_end ; ++it)
      {
         ClientData & client_data = *it;
         
         archi::Int32 sub_user_id = client_data.get_user_id ();
         
         if (sub_user_id == user_id)
         {
            _self_client_data_ptr = &client_data;
            break;
         }
      }
   }

   return _self_client_data_ptr;
}



/*
==============================================================================
Name: use_self_client_data
==============================================================================
*/

const ClientData *   Root::use_self_client_data_ptr () const
{
   using namespace ohm;
   
   // we can cache the client data ptr because it is not going to change
   
   if (_self_client_data_ptr == 0)
   {
      archi::Int32 user_id = use_document ().get_user_id ();
   
      ClientDataColl::const_iterator it = _client_data_coll.begin ();
      const ClientDataColl::const_iterator it_end = _client_data_coll.end ();
      
      for (; it != it_end ; ++it)
      {
         const ClientData & client_data = *it;
         
         archi::Int32 sub_user_id = client_data.get_user_id ();
         
         if (sub_user_id == user_id)
         {
            _self_client_data_ptr = const_cast <ClientData *> (&client_data);
            break;
         }
      }
   }

   return _self_client_data_ptr;
}



/*
==============================================================================
Name : tx_add_taxi
==============================================================================
*/
   
SpacialObjectColl::iterator   Root::tx_add_taxi (ohm::archi::Float64 x, ohm::archi::Float64 y, ohm::archi::Float64 width, Taxi::Color color)
{
   _tx_session_guard.prepare_record (_scribe);
   
   if (!_tx_session_guard.start ())
   {
      return _spatial_object_coll.end ();
   }

   SpacialObjectColl::iterator it = ext_add_taxi (x, y, width, color);
   
   set_scribe_metadata ("Add Taxi");
   
   _tx_session_guard.commit ();
   
   return it;
}



/*
==============================================================================
Name : ext_add_taxi
==============================================================================
*/
   
SpacialObjectColl::iterator   Root::ext_add_taxi (ohm::archi::Float64 x, ohm::archi::Float64 y, ohm::archi::Float64 width, Taxi::Color color)
{
   SpacialObjectColl::iterator it = _spatial_object_coll.insert <Taxi> ();
   
   Taxi & taxi = dynamic_cast <Taxi &> (*it);
   
   taxi.ext_set_position (x, y);
   taxi.ext_set_width (width);
   taxi.ext_set_color (color);
   
   return it;
}



/*
==============================================================================
Name : ext_add_taxi
==============================================================================
*/
   
SpacialObjectColl::iterator   Root::ext_add_taxi (Taxi & taxi)
{
   using namespace ohm;
   
   flip::ObjectMold mold (taxi);
   
   SpacialObjectColl::iterator it = _spatial_object_coll.paste (mold);
   
   return it;
}



/*
==============================================================================
Name : tx_add_pedestrian
==============================================================================
*/
   
SpacialObjectColl::iterator   Root::tx_add_pedestrian (ohm::archi::Float64 x, ohm::archi::Float64 y)
{
   _tx_session_guard.prepare_record (_scribe);
   
   if (!_tx_session_guard.start ())
   {
      return _spatial_object_coll.end ();
   }

   SpacialObjectColl::iterator it = ext_add_pedestrian (x, y);
   
   set_scribe_metadata ("Add Pedestrian");
   
   _tx_session_guard.commit ();
   
   return it;
}



/*
==============================================================================
Name : ext_add_pedestrian
==============================================================================
*/
   
SpacialObjectColl::iterator   Root::ext_add_pedestrian (ohm::archi::Float64 x, ohm::archi::Float64 y)
{
   SpacialObjectColl::iterator it = _spatial_object_coll.insert <Pedestrian> ();
   
   Pedestrian & pedestrian = dynamic_cast <Pedestrian &> (*it);
   
   pedestrian.ext_set_position (x, y);
   
   return it;
}



/*
==============================================================================
Name : ext_add_pedestrian
==============================================================================
*/
   
SpacialObjectColl::iterator   Root::ext_add_pedestrian (Pedestrian & pedestrian)
{
   using namespace ohm;
   
   flip::ObjectMold mold (pedestrian);
   
   SpacialObjectColl::iterator it = _spatial_object_coll.paste (mold);
   
   return it;
}



/*
==============================================================================
Name : ext_erase
==============================================================================
*/
   
void  Root::ext_erase (SpacialObjectColl::iterator it)
{
   _spatial_object_coll.erase (it);
}



/*
==============================================================================
Name : use_spacial_object_coll
==============================================================================
*/
   
SpacialObjectColl &  Root::use_spacial_object_coll ()
{
   return _spatial_object_coll;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: PredicateClient
==============================================================================
*/

Root::PredicateClient::PredicateClient (const std::string & user_name)
:  _user_name (user_name)
{
   assert (&user_name != 0);
}



/*
==============================================================================
Name: equal
==============================================================================
*/

bool Root::PredicateClient::equal (const ClientData & client_data) const
{
   return (client_data.get_user_name () == _user_name);
}



/*
==============================================================================
Name: PredicateUserId
==============================================================================
*/

Root::PredicateUserId::PredicateUserId (ohm::archi::Int32 user_id)
:  _user_id (user_id)
{
}



/*
==============================================================================
Name: equal
==============================================================================
*/

bool Root::PredicateUserId::equal (const ClientData & client_data) const
{
   return (client_data.get_user_id () == _user_id);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace model



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
