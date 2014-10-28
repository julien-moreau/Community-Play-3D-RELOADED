/*****************************************************************************

        ServerLocal.cpp
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 63183

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

#include "ohm/flip/local/ComLayerServerLocal.h"
#include "ohm/flip/local/ServerLocal.h"
#include "ohm/flip/DocumentLocal.h"

#include "ohm/flip/conv/ConvDocument.h"
#include "ohm/flip/private/DataStreamOutput.h"
#include "ohm/opal/Application.h"
#include "ohm/sys/File.h"
#include "ohm/task/RunLoopManager.h"
#include "ohm/util/trace.h"

#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

ServerLocal::ServerLocal (DocumentLocal & central)
:  _central (central)
,  _document_server_aptr ()
/*,   _model_validator_aptr ()*/
,  _run_loop ()
{
   // force init
   ComLayerServerLocal::use_instance ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ServerLocal::~ServerLocal ()
{
   try
   {
   }
   catch (...)
   {
   }
}



/*
==============================================================================
Name : run
==============================================================================
*/

void  ServerLocal::run (const std::string & document_path)
{
   _document_path = document_path;
   
   ThreadBase::run ();
   
   _run_sync.wait ();
}



/*
==============================================================================
Name : stop
==============================================================================
*/

void  ServerLocal::stop ()
{
   _run_loop.stop ();
   
   ThreadBase::wait_for_death ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_run
==============================================================================
*/

void  ServerLocal::do_run ()
{
   task::RunLoopManager::use_instance ().push_current (_run_loop);
   
   ComLayerServerLocal::use_instance ().bind (_run_loop);

   ComLayerServerLocal & com_layer = ComLayerServerLocal::use_instance ();
   
   _document_server_aptr = std::auto_ptr <DocumentServer> (
      new DocumentServer (com_layer)
   );
   
   /*_model_validator_aptr = std::auto_ptr <model::ModelValidator> (
      new model::ModelValidator
   );
   
   _document_server_aptr->bind (*_model_validator_aptr);*/
   
   // load document if any
   
   load (_document_path);
   
   _run_sync.signal ();
   
   _run_loop.run ();
   
   if (_document_path != "")
   {
      _document_server_aptr->write_disk (_document_path.c_str ());
   }
   
   /*_model_validator_aptr.reset ();*/

   _document_server_aptr.reset ();
   
   ComLayerServerLocal::use_instance ().unbind ();

   task::RunLoopManager::use_instance ().pop_current (_run_loop);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : load
==============================================================================
*/

void  ServerLocal::load (const std::string & document_path)
{
   bool load_flag = false;
   
   if (document_path != "")
   {
      load_flag = sys::File::exist (document_path.c_str ());
   }
   
   if (load_flag)
   {
      int err = _document_server_aptr->read_disk (document_path.c_str ());
      assert (err == 0);
   }
   else
   {
      // init document
      
      DocumentServer & document = *_document_server_aptr;
      ObjectManager & object_manager = document.use_object_manager ();
      Object * obj_ptr = object_manager.get_root ();
      assert (obj_ptr != 0);
      
      document.begin_transaction ();
      
      obj_ptr->ctor (Args::_none);
      
      document.end_transaction ();
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
