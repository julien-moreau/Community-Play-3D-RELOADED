/*****************************************************************************

        SkeletonGui.cpp
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

#include "client/SkeletonGui.h"

#include "client/Application.h"
#include "client/ApplicationWindow.h"
#include "client/Constants.h"
#include "client/LayerAspectBrightnessContrast.h"

#include "ohm/archi/def.h"
#include "ohm/flip/DocumentClient.h"
#include "ohm/flip/private/DocumentClientMasterSlot.h"
#include "ohm/mon/trace.h"
#include "ohm/opa/BundleView.h"
#include "ohm/opa/EventBundleView.h"
#include "ohm/opa/base/LayerAspectWidget.h"
#include "ohm/opak/LayerAspectColorAB.h"
#include "ohm/opal/Application.h"
#include "ohm/opal/Frame.h"
#include "ohm/opal/MenuBar.h"
#include "ohm/opal/MenuItem.h"
#include "ohm/opal/Screens.h"
#include "ohm/opal/VideoThread.h"
#include "ohm/sys/FileRefFactory.h"
#include "ohm/task/RunLoopGui.h"
#include "ohm/txt/utf8/ConvNum.h"
#include "ohm/txt/out/Print.h"
#include "ohm/util/BinaryStreamInput.h"

#include <stdexcept>

#include <cassert>



namespace client
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

SkeletonGui::SkeletonGui (ohm::opa::BundleView & view)
:  _view (view)
,  _menu_manager (view)
,  _document_master_ptr (0)
,  _slot_ptr (0)
,  _thread_nbr (size_t (-1))
,  _document_ptr (0)
,  _layer_aspect_bc_ptr (0)
,  _observer ("client.SkeletonGui")
{
   _observer.bind_process <
      SkeletonGui,
      &SkeletonGui::process
   > (*this);

   _observer.bind_enter_process <
      SkeletonGui,
      &SkeletonGui::enter_process
   > (*this);

   _observer.bind_exit_process <
      SkeletonGui,
      &SkeletonGui::exit_process
   > (*this);
   
   init_service ();
   init_gui ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

SkeletonGui::~SkeletonGui ()
{
   try
   {
      restore_gui ();
      restore_service ();
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : use_view
==============================================================================
*/

ohm::opa::BundleView &  SkeletonGui::use_view ()
{
   return _view;
}



/*
==============================================================================
Name : create
Description :
   Create a gui with only the sequence gui activated
   Called in RunLoopGui thread
==============================================================================
*/

ohm::flip::DocumentClientMasterSlot &  SkeletonGui::create (ohm::flip::DocumentClientMaster & document_master, size_t thread_nbr)
{
   using namespace ohm;
   
   assert (&document_master != 0);
   
   assert (_document_master_ptr == 0);
   assert (_slot_ptr == 0);
   assert (_thread_nbr == size_t (-1));
   assert (_document_ptr == 0);
   
   _document_master_ptr = &document_master;
   _thread_nbr = thread_nbr;
   
   _slot_ptr = flip::DocumentClientMasterSlot::create (document_master, _thread_nbr);

   _document_ptr = _slot_ptr->get_document_ptr ();
   assert (_document_ptr != 0);
   
   _document_ptr->bind (*this);
   
   _bundle_gui_aptr = std::auto_ptr <BundleGui> (new BundleGui (_view));
   
   add (*_bundle_gui_aptr);
   
   return *_slot_ptr;
}



/*
==============================================================================
Name : pre_release
Description :
   Notification, occuring just before the document client master is restored
   that the document is going to be released.
   Therefore we should not be able to access it anymore.
==============================================================================
*/

void  SkeletonGui::pre_release ()
{
   _document_ptr = 0;
}



/*
==============================================================================
Name : release
==============================================================================
*/

void  SkeletonGui::release ()
{
   assert (_document_master_ptr != 0);
   assert (_slot_ptr != 0);
   assert (_thread_nbr != size_t (-1));
   assert (_document_ptr == 0);
   
   delete _slot_ptr;
   _slot_ptr = 0;
   
   _document_master_ptr = 0;
   _thread_nbr = size_t (-1);
   
   remove (*_bundle_gui_aptr);
   
   _bundle_gui_aptr.reset ();
}



/*
==============================================================================
Name: use_document_client
Description:
   use this call only when the GUI is constructed
==============================================================================
*/

ohm::flip::DocumentClient & SkeletonGui::use_document_client ()
{
   assert (_document_ptr != 0);

   return *_document_ptr;
}



/*
==============================================================================
Name : document_changed
==============================================================================
*/

void  SkeletonGui::document_changed (model::Root & root)
{
   using namespace ohm;
   
   assert (&root != 0);
   
   root.update_client_data_cache ();
   
   if (root.was_inited ())
   {
      root.signal_init_gui ();
      
      _menu_manager.bind (root);
   }
   
   flip::DocumentObserver <model::Root>::document_changed (root);
   
   if (root.was_inited ())
   {
      _bundle_gui_aptr->update ();
   }
}



/*
==============================================================================
Name : signal
==============================================================================
*/

void  SkeletonGui::signal (ohm::flip::Object * obj_ptr, ohm::archi::UInt32 type, const ohm::flip::DataMaster & signal_data)
{
   using namespace ohm;
   
   assert (_document_ptr != 0);
   
   flip::DocumentClient::AutoUserInputs auto_ui (*_document_ptr);
   
   bool continue_flag = true;
   
   if (continue_flag)
   {
      model::Root * root_ptr = dynamic_cast <model::Root *> (obj_ptr);
      
      if (root_ptr != 0)
      {
         signal (*root_ptr, type, signal_data);
         
         continue_flag = false;
      }
   }
   
   flip::DocumentObserver <model::Root>::signal (obj_ptr, type, signal_data);
}



/*
==============================================================================
Name : notify_pre_action
Description :
==============================================================================
*/

void  SkeletonGui::notify_pre_action ()
{
   if (_document_ptr != 0)
   {
      _document_ptr->notify_pre_user_inputs_tasks ();
   }
}



/*
==============================================================================
Name : notify_post_action
Description :
==============================================================================
*/

void  SkeletonGui::notify_post_action ()
{
   if (_document_ptr != 0)
   {
      _document_ptr->notify_post_user_inputs_tasks ();
   }
}



/*
==============================================================================
Name : set_brightness
==============================================================================
*/

void  SkeletonGui::set_brightness (float brightness)
{
   if (_layer_aspect_bc_ptr != 0)
   {
      _layer_aspect_bc_ptr->_brightness = brightness;
   }
}



/*
==============================================================================
Name : set_contrast
==============================================================================
*/

void  SkeletonGui::set_contrast (float contrast)
{
   if (_layer_aspect_bc_ptr != 0)
   {
      _layer_aspect_bc_ptr->_contrast = contrast;
   }
}



/*
==============================================================================
Name : toggle_inval_rect_display
==============================================================================
*/

void  SkeletonGui::toggle_inval_rect_display ()
{
   _view.debug_toggle_show_invalid_rect ();
}



/*
==============================================================================
Name : get_transition
==============================================================================
*/

void  SkeletonGui::get_transition (double & tt, ohm::opak::TransitionFunction & tf, ohm::flip::Object & object)
{
   using namespace ohm;
   
   tt = 0.0;
   tf = opak::step;
   
   if (!object.was_inited () && !object.was_restored ())
   {
      switch (object.use_document ().get_controller ())
      {
      case flip::DocumentBase::Controller_NETWORK_EXT:
      case flip::DocumentBase::Controller_NETWORK_DENY:
      case flip::DocumentBase::Controller_LOCAL:
      case flip::DocumentBase::Controller_UNDO_REDO:
         tt = 0.25;
         tf = opak::ease_in_ease_out;
         break;

      case flip::DocumentBase::Controller_DOC_DIF:
         tt = 0.5;
         tf = opak::ease_in_ease_out;
         break;
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : init_service
==============================================================================
*/

#if defined (SKELETON_LOCAL)
void  SkeletonGui::init_service ()
{
   _server_local_aptr
      = std::auto_ptr <local::ServerLocal> (new local::ServerLocal);

   _server_local_aptr->run ();
   
   // main client
   
   _client_local_aptr
      = std::auto_ptr <local::ClientLocal> (new local::ClientLocal (*this));

   _client_local_aptr->connect ();
}
#endif



/*
==============================================================================
Name : init_service
==============================================================================
*/

#if defined (SKELETON_NET)
void  SkeletonGui::init_service ()
{
   using namespace ohm;
   
   Application & app = Application::use_instance ();
   
   const std::string & host = app.get (Application::_key_host_0);
   const std::string & port_str = app.get (Application::_key_port_0);
   const std::string & session = app.get (Application::_key_session_0);
   const std::string & username = app.get (Application::_key_username_0);
   const std::string & secret = app.get (Application::_key_secret_0);
   
   int port = int (txt::utf8::ConvNum::conv_str_to_int64 (port_str.c_str ()));
   
   _client_net_aptr = std::auto_ptr <net::ClientNet> (new net::ClientNet (
      *this, host, port, session, username, secret
   ));

   _client_net_aptr->connect ();
}
#endif



/*
==============================================================================
Name : restore_service
==============================================================================
*/

#if defined (SKELETON_LOCAL)
void  SkeletonGui::restore_service ()
{
   // main client
   
   _client_local_aptr->disconnect ();
   _client_local_aptr.reset ();
   
   // server
   
   _server_local_aptr->stop ();
   _server_local_aptr.reset ();
}
#endif



/*
==============================================================================
Name : restore_service
==============================================================================
*/

#if defined (SKELETON_NET)
void  SkeletonGui::restore_service ()
{
   _client_net_aptr->disconnect ();
   _client_net_aptr.reset ();
}
#endif



/*
==============================================================================
Name : init_gui
==============================================================================
*/

void  SkeletonGui::init_gui ()
{
   using namespace ohm;
   
   opal::PostActionListener * pal_ptr = this;
   _view.add (*pal_ptr);
   
   _view.intercalate <opa::LayerAspectWidget> ("LayerAspectWidget");
   
   {
      opak::LayerAspectColorAB & layer
         = _view.intercalate <opak::LayerAspectColorAB> ("LayerAspectColorAB");

      layer._a = -16;
      layer._b = 93;
   }

   {
      LayerAspectBrightnessContrast & layer
         = _view.intercalate <LayerAspectBrightnessContrast> ("LayerAspectBrightnessContrast");
      
      _layer_aspect_bc_ptr = &layer;
   }
   
   task::RunLoopGui::use_instance ().add (_observer);
   
   opal::MenuRoot & menu_root = opal::MenuBar::use_instance ().use_root ();
   
   menu_root.bind_pre <
      SkeletonGui,
      &SkeletonGui::process_menu_pre
   > (*this);
   
   menu_root.bind_post <
      SkeletonGui,
      &SkeletonGui::process_menu_post
   > (*this);
}



/*
==============================================================================
Name : restore_gui
==============================================================================
*/

void  SkeletonGui::restore_gui ()
{
   using namespace ohm;
   
   task::RunLoopGui::use_instance ().remove (_observer);
   
   _layer_aspect_bc_ptr = 0;
   
   opal::PostActionListener * pal_ptr = this;
   _view.remove (*pal_ptr);
}



/*
==============================================================================
Name : process_menu_pre
==============================================================================
*/

void  SkeletonGui::process_menu_pre ()
{
   if (_document_ptr != 0)
   {
      _document_ptr->notify_pre_user_inputs_tasks ();
   }
}



/*
==============================================================================
Name : process_menu_post
==============================================================================
*/

void  SkeletonGui::process_menu_post ()
{
   if (_document_ptr != 0)
   {
      _document_ptr->notify_post_user_inputs_tasks ();
   }
}



/*
==============================================================================
Name: enter_process
==============================================================================
*/

void SkeletonGui::enter_process ()
{
   const std::string & user_name
      = Application::use_instance ().get (Constants::_cmd_arg_username_0);
}



/*
==============================================================================
Name: exit_process
==============================================================================
*/

void SkeletonGui::exit_process ()
{
}



/*
==============================================================================
Name : process
==============================================================================
*/

void  SkeletonGui::process (ohm::task::Event & event)
{
   EventType type = event.pop <EventType> ();
   
   switch (type)
   {
   default:
      assert (false);
      break;
   }
}



/*
==============================================================================
Name : signal
==============================================================================
*/

void  SkeletonGui::signal (model::Root & root, ohm::archi::UInt32 type, const ohm::flip::DataMaster & signal_data)
{
   switch (type)
   {
   case model::Root::Signal_INIT_GUI:
      signal_init_gui (root, signal_data);
      break;
   }
}



/*
==============================================================================
Name : signal_init_gui
==============================================================================
*/

void  SkeletonGui::signal_init_gui (model::Root & root, const ohm::flip::DataMaster & signal_data)
{
   init_document ();
}



/*
==============================================================================
Name : init_document
Description :
   Function called from a signal just after the first load of the document,
   triggered by the very first call of document_changed.
   It is safe to modify the document from here.
==============================================================================
*/

void  SkeletonGui::init_document ()
{
   assert (_document_ptr != 0);
   
   int err = _document_ptr->start_session ();
   assert (err == 0);
   
   model::Root * root_ptr = dynamic_cast <model::Root *> (
      _document_ptr->use_object_manager ().get_root ()
   );
   
   assert (root_ptr != 0);
   
   root_ptr->ext_add_client_data ();
   
   model::ClientData & client_data = model::ClientData::use_self (*root_ptr);
   
   client_data.ext_set_primary_tool ();
   
   _document_ptr->commit_session ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
