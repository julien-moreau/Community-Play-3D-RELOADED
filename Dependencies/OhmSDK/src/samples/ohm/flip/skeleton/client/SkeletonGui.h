/*****************************************************************************

        SkeletonGui.h
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



#if ! defined (client_SkeletonGui_HEADER_INCLUDED)
#define  client_SkeletonGui_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "client/BundleGui.h"
#include "client/MenuManager.h"

#include "model/Root.h"

#include "ohm/flip/DocumentClientMaster.h"
#include "ohm/flip/DocumentObserver.h"
#include "ohm/opa/WidgetDialog.h"
#include "ohm/opal/MenuRoot.h"
#include "ohm/opal/MenuEvent.h"
#include "ohm/opal/private/PostActionListener.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/task/Observer.h"

#if defined (SKELETON_LOCAL)
   #include "client/local/ClientLocal.h"
   #include "client/local/ServerLocal.h"

#elif defined (SKELETON_NET)
   #include "client/net/ClientNet.h"

#else
   #error

#endif




namespace ohm
{
namespace opa
{
class BundleView;
}
}



namespace client
{



class LayerAspectBrightnessContrast;

class SkeletonGui
:  public ohm::flip::DocumentObserver <model::Root>
,  public ohm::opal::PostActionListener
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  SkeletonGui (ohm::opa::BundleView & view);
   virtual        ~SkeletonGui ();
   
   ohm::opa::BundleView &
                  use_view ();
   
   ohm::flip::DocumentClientMasterSlot &
                  create (ohm::flip::DocumentClientMaster & document_master, size_t thread_nbr);
   void           pre_release ();
   void           release ();
   ohm::flip::DocumentClient &
                  use_document_client ();
         
   // ohm::ohm::flip::DocumentObserver <model::Root>
   virtual void   document_changed (model::Root & root);
   virtual void   signal (ohm::flip::Object * obj_ptr, ohm::archi::UInt32 type, const ohm::flip::DataMaster & signal_data);

   // ohm::opal::PostActionListener
   virtual void   notify_pre_action ();
   virtual void   notify_post_action ();

   // view options
   void           set_brightness (float brightness);
   void           set_contrast (float contrast);
   void           toggle_inval_rect_display ();
   
   static void    get_transition (double & tt, ohm::opak::TransitionFunction & tf, ohm::flip::Object & object);
                                 


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   enum EventType
   {
   };
   
   void           init_service ();
   void           restore_service ();

   void           init_gui ();
   void           restore_gui ();
   
   // delegates
   void           process_menu_pre ();
   void           process_menu_post ();
   
   void           enter_process ();
   void           exit_process ();
   void           process (ohm::task::Event & event);
   
   void           signal (model::Root & root, ohm::archi::UInt32 type, const ohm::flip::DataMaster & signal_data);
   void           signal_init_gui (model::Root & root, const ohm::flip::DataMaster & signal_data);

   void           init_document ();
   

   ohm::opa::BundleView &
                  _view;
   MenuManager    _menu_manager;
   
#if defined (SKELETON_LOCAL)
   std::auto_ptr <local::ServerLocal>
                  _server_local_aptr;
   std::auto_ptr <local::ClientLocal>
                  _client_local_aptr;

#elif defined (SKELETON_NET)
   std::auto_ptr <net::ClientNet>
                  _client_net_aptr;

#endif

   ohm::flip::DocumentClientMaster *
                  _document_master_ptr;
   ohm::flip::DocumentClientMasterSlot *
                  _slot_ptr;
   size_t         _thread_nbr;
   ohm::flip::DocumentClient *
                  _document_ptr;
   
   std::auto_ptr <BundleGui>
                  _bundle_gui_aptr;
   
   LayerAspectBrightnessContrast *
                  _layer_aspect_bc_ptr;
   
   ohm::task::Observer
                  _observer;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  SkeletonGui ();
                  SkeletonGui (const SkeletonGui &other);
   SkeletonGui &  operator = (const SkeletonGui &other);
   bool           operator == (const SkeletonGui &other);
   bool           operator != (const SkeletonGui &other);

}; // class SkeletonGui



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "client/SkeletonGui.hpp"



#endif   // client_SkeletonGui_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
