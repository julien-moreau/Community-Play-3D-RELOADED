/*****************************************************************************

        VersionTree.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70457

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
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "model/conv/VersionTree.h"

//#include "model/conv/Converter53843_54534.h"

#include "ohm/flip/Err.h"
#include "ohm/flip/conv/ConvDocument.h"
#include "ohm/flip/private/DataStreamInput.h"
#include "ohm/flip/private/DataStreamOutput.h"
#include "ohm/util/trace.h"

#include <cassert>



namespace model
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: dtor
==============================================================================
*/

VersionTree::~VersionTree ()
{
}



/*
==============================================================================
Name: convert
Description :
   Convert from old document format to another document format, generally
   the latest one.
   Implicitely the document version conversion algo makes an oriented non-cyclic
   graph and the path to a particular version to the latest one is directly
   resolved by using a depth layer algo which resolved dependencies.
   Returns 0 iff successful.
==============================================================================
*/

int   VersionTree::convert (ohm::flip::DataStreamOutput & dst, ohm::flip::DataStreamInput & src, bool force_flag)
{
   using namespace ohm;
   
   int err = 0;
   
   flip::ConvDocument document;
   
   if (err == 0)
   {
      err = document.read (src);
      assert (err == 0);
   }
   
   if (err == 0)
   {
      err = do_convert (document);
      assert (err == 0);
   }

   if (err == 0)
   {
      err = document.write (dst);
      assert (err == 0);
   }
   
   return err;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: ctor
==============================================================================
*/

VersionTree::VersionTree ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: do_convert
==============================================================================
*/

int   VersionTree::do_convert (ohm::flip::ConvDocument & document)
{
   using namespace ohm;
   
   // by default the document won't be converted
   
   int err = 0;
   
   bool converted_flag = false;
   
   try 
   {
      /*if ((err == 0) && (document._format_version == "OhmStudio.Alpha.Rev53843"))
      {
         ohm_util_TRACE_INFO ("Converter53843_54534");
         
         // remove orphan automations and bad samples
         
         Converter53843_54534 converter;
         
         err = converter.process (document);
         
         converted_flag = true;
      }*/
      
      if (err == 0)
      {
         ohm_util_TRACE_INFO ("OK");
      }

      if (converted_flag)
      {
         document._rev_id += 1;
      }
   }
   catch (...) 
   {
      assert (false);
      ohm_util_TRACE_PANIC ("Document convertion failed - an exception occured.");
      
      err = flip::Err_EXCEPTION;
   }
   
   return err;
}



}  // namespace model



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

