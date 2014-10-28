/*****************************************************************************

        ServerLocal.cpp
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

#include "client/local/ServerLocal.h"

#include "client/Application.h"
#include "client/Constants.h"
#include "client/local/ComLayerServerLocal.h"

#include "model/ModelTweaker.h"
#include "model/ModelSignal.h"
#include "model/ModelValidator.h"
#include "model/conv/VersionTree.h"

#include "ohm/flip/conv/ConvDocument.h"
#include "ohm/flip/private/DataStreamOutput.h"
#include "ohm/opal/Application.h"
#include "ohm/task/RunLoopManager.h"
#include "ohm/util/trace.h"

#include <cassert>



namespace client
{
namespace local
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

ServerLocal::ServerLocal ()
:  _document_server_aptr ()
,  _model_validator_aptr ()
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

void  ServerLocal::run ()
{
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
   using namespace ohm;
   
   task::RunLoopManager::use_instance ().push_current (_run_loop);
   
   ComLayerServerLocal::use_instance ().bind (_run_loop);

   ComLayerServerLocal & com_layer = ComLayerServerLocal::use_instance ();
   
   _document_server_aptr = std::auto_ptr <flip::DocumentServer> (
      new flip::DocumentServer (com_layer)
   );
   
   _model_validator_aptr = std::auto_ptr <model::ModelValidator> (
      new model::ModelValidator
   );
   
   _document_server_aptr->bind (*_model_validator_aptr);

   _model_signal_aptr = std::auto_ptr <model::ModelSignal> (
      new model::ModelSignal
   );
   
   _document_server_aptr->bind (*_model_signal_aptr);
   
   _model_tweaker_aptr = std::auto_ptr <model::ModelTweaker> (
      new model::ModelTweaker
   );
   
   _document_server_aptr->bind (*_model_tweaker_aptr);
   
   // load document if any
   
   load ();
   
   _run_sync.signal ();
   
   _run_loop.run ();
   
   const std::string & path_str
      = Application::use_instance ().get (Constants::_cmd_arg_document_path_0);

   if (path_str != "")
   {
      _document_server_aptr->write_disk (path_str.c_str ());
   }
   
   _model_tweaker_aptr.reset ();
   _model_validator_aptr.reset ();

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

void  ServerLocal::load ()
{
   using namespace ohm;
   
   const std::string & path_str
      = Application::use_instance ().get (Constants::_cmd_arg_document_path_0);
   
   bool load_flag = false;
   
   if (path_str != "")
   {
      using namespace std;
      
      FILE * file_ptr = fopen (path_str.c_str (), "rb");

      if (file_ptr != 0)
      {
         fclose (file_ptr);
         file_ptr = 0;
         
         load_flag = true;
      }
   }
   
   if (load_flag)
   {
      std::string uncomp_path_str;
      
      unzip (uncomp_path_str, path_str);
      
      // make a round trip conversion to test 'conv'
      
      flip::DataStreamInput dsi;
      
      int err = dsi.open (uncomp_path_str.c_str ());
      assert (err == 0);
      
      std::string alt_path_str = path_str + ".converted";
      
      flip::DataStreamOutput dso;
      dso.set_file_output (alt_path_str.c_str ());
      
      model::VersionTree::convert (dso, dsi);
      
      dso.close ();
      
      err = _document_server_aptr->read_disk (alt_path_str.c_str ());
      assert (err == 0);
   }
   else
   {
      // init document
      
      flip::DocumentServer & document = *_document_server_aptr;
      flip::ObjectManager & object_manager = document.use_object_manager ();
      flip::Object * obj_ptr = object_manager.get_root ();
      assert (obj_ptr != 0);
      
      model::Root * root_ptr = dynamic_cast <model::Root *> (obj_ptr);
      assert (root_ptr != 0);
      
      document.begin_transaction ();
      
      root_ptr->ext_init ();
      
      document.end_transaction ();
   }
}



/*
==============================================================================
Name : load
==============================================================================
*/

int ServerLocal::unzip (std::string & dest_path, const std::string & src_path)
{
   int err = 0;
   
   bool zlib_flag = false;
   
   FILE * input_file = ::fopen (src_path.c_str (), "rb");
   
   ohm::archi::Int32 uncomp_size = 0;
   std::vector <ohm::archi::UByte> uncomp_data;
   
   std::vector <ohm::archi::UByte> comp_data;
   
   if (input_file == 0) 
   {
      assert (false);
      err = -1;
   }
   else
   {
      ohm::archi::UByte buf [8];
      
      size_t read_len = fread (buf, sizeof (buf), 1, input_file);
      
      if (read_len == 1)
      {
         zlib_flag
            = (buf [0] == 'z')
            && (buf [1] == 'l')
            && (buf [2] == 'i')
            && (buf [3] == 'b')
         ;

         uncomp_size += (buf [4] << 24);
         uncomp_size += (buf [5] << 16);
         uncomp_size += (buf [6] << 8);
         uncomp_size += (buf [7]);
      }
   }

   if (!zlib_flag)
   {
      // nothing to uncompress
      
      dest_path = src_path;
      
      ::fclose (input_file);

      return err;
   }

   if (uncomp_size <= 0)
   {
      assert (false);
      err = -1;
   }

   if (err == 0)
   {
      ::fseek (input_file, 0, SEEK_END);
      size_t total_len = ::ftell (input_file);
      
      assert (total_len >= 8);

      ::fseek (input_file, 8, SEEK_SET);
      comp_data.resize (total_len - 8);
   }
   
   if (err == 0)
   {
      uncomp_data.resize (uncomp_size);
   }
   
   
   if (err == 0)
   {
      size_t read_len = ::fread (& comp_data [0], comp_data.size (), 1, input_file); 
      
      if (read_len != 1)
      {
         assert (false);
         err = -1;
      }
   }

   if (err == 0)
   {
      ::uLongf       uncomp_size = uncomp_data.size ();

      int z = ::uncompress (
         & uncomp_data [0], 
         &uncomp_size, 
         & comp_data [0], 
         comp_data.size ()
      ); 
      
      if (z != Z_OK)
      {
         assert (false);
         err = -1;
      }
      else if (uncomp_size != uncomp_data.size ())
      {
         assert (false);
         err = -1;
      }
   }
   
   dest_path = src_path + ".uncompressed";

   FILE * out_file = ::fopen (dest_path.c_str (), "wb");
   
   if (out_file == 0)
   {
      assert (false);
      err = -1;
   }
   
   if (err == 0)
   {
      size_t write_len = ::fwrite (
         &uncomp_data [0], uncomp_data.size (), 1, out_file
      );
      
      if (write_len != 1)
      {
         assert (false);
         err = -1;
      }
   }

   ::fclose (out_file);
   ::fclose (input_file);
   
   return err;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace local
}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
