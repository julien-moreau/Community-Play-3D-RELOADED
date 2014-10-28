/*****************************************************************************

        ModelSignal.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71378

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

#include "model/ModelSignal.h"
#include "model/Root.h"

#include "ohm/util/BinaryStreamInput.h"
#include "ohm/util/BinaryStreamOutput.h"

#include <cassert>



namespace model
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

ModelSignal::ModelSignal ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ModelSignal::~ModelSignal ()
{
}



/*
==============================================================================
Name : signal
==============================================================================
*/

void  ModelSignal::signal (const std::string & from, ohm::flip::Object * obj_ptr, ohm::archi::UInt32 type, const ohm::archi::UByte * data_ptr, ohm::archi::UInt32 data_size)
{
   bool continue_flag = true;
   
   if (continue_flag)
   {
      model::Root * root_ptr = dynamic_cast <model::Root *> (obj_ptr);
      
      if (root_ptr != 0)
      {
         signal (from, *root_ptr, type, data_ptr, data_size);
         
         continue_flag = false;
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : signal
==============================================================================
*/

void  ModelSignal::signal (const std::string & from, Root & root, ohm::archi::UInt32 type, const ohm::archi::UByte * data_ptr, ohm::archi::UInt32 data_size)
{
   switch (type)
   {
   case Root::SignalServer_DO_SOMETHING:
      signal_server_do_something (from, root, data_ptr, data_size);
      break;

   default:
      // ignore
      break;
   }
}



/*
==============================================================================
Name : signal_server_do_something
==============================================================================
*/

void  ModelSignal::signal_server_do_something (const std::string & from, Root & root, const ohm::archi::UByte * data_ptr, ohm::archi::UInt32 data_size)
{
   /*std::vector <archi::UByte> data;
   util::BinaryStreamOutput bso (data);
   
   bso.write (sign_data_list.begin (), sign_data_list.end ());
   
   root.send_signal_client (from, Root::Signal_SERVER_SIGN_PUT, &data [0], data.size ());*/
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace model



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
