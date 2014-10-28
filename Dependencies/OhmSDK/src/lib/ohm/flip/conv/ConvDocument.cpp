/*****************************************************************************

        ConvDocument.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70008

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

#include "ohm/flip/conv/ConvDocument.h"
#include "ohm/flip/private/DataStreamOutput.h"
#include "ohm/flip/private/DocumentParser.h"
#include "ohm/flip/private/DocumentFormatter.h"

#include "ohm/txt/out/Print.h"

#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

ConvDocument::ConvDocument ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ConvDocument::~ConvDocument ()
{
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   ConvDocument::read (DataStreamInput & data_stream)
{
   int err = 0;
   
   DocumentParser dp (data_stream);
   
   if (err == 0)
   {
      err = dp.init ();
   }
   
   if (err == 0)
   {
      _rev_id = dp.get_rev_id ();
      _format_version = dp.get_format_version ();
      _user_name_id_map = dp.get_user_name_id_map ();
      
      flip::Ref root_ref = dp.get_root_ref ();
      
      _root.read (dp, root_ref);
   }
   
   if (err == 0)
   {
      assert (!has_ref_collision ());
   }
   
   return err;
}



/*
==============================================================================
Name : write
==============================================================================
*/

int   ConvDocument::write (DataStreamOutput & dst)
{
   assert (!has_ref_collision ());
   
   if (has_ref_collision ())
   {
      // abort
      
      return -1;
   }
   
   //
   
   DocumentFormatter::write_header (dst);
   
   std::set <size_t> obj_offset_set;
   
   std::list <ConvObject *> obj_ptr_arr;
   
   _root.collect (obj_ptr_arr);
   
   std::list <ConvObject *>::iterator it = obj_ptr_arr.begin ();
   const std::list <ConvObject *>::iterator it_end = obj_ptr_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      // object in the list are writable
      
      ConvObject * obj_ptr = *it;
      
      obj_offset_set.insert (dst.tell ());
      
      obj_ptr->write (dst);
   }
   
   size_t startxref = 0;
   
   DocumentFormatter::write_xref (dst, startxref, obj_offset_set);
   
   DocumentFormatter::write_trailer (
      dst,
      _root._ref,
      _rev_id,
      _format_version,
      _user_name_id_map
   );
   
   DocumentFormatter::write_start_xref (dst, startxref);
   
   return 0;
}



/*
==============================================================================
Name : write
==============================================================================
*/

int   ConvDocument::write_xml (std::string & xml)
{
   assert (!has_ref_collision ());
   
   if (has_ref_collision ())
   {
      // abort
      
      return -1;
   }
   
   txt::out::Print printer;
   
   xml += "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
   
   printer
      .print ("<document revision=\"%1%\" version=\"%2%\">\n")
      .arg (_rev_id).arg (_format_version.c_str ());
   
   xml += printer;
   
   xml += "\t<users>\n";
   
   {
      UserNameIdMap::const_iterator it = _user_name_id_map.begin ();
      const UserNameIdMap::const_iterator it_end = _user_name_id_map.end ();
      
      for (; it != it_end ; ++it)
      {
         const std::string & user_name = it->first;
         const archi::Int32 user_id = it->second;
         
         printer
            .print ("\t\t<user name=\"%1%\" id=\"%2%\"/>\n")
            .arg (user_name.c_str ()).arg (user_id);
      }
   }
   
   xml += "\t</users>\n";
   
   ConvObject::ContextXml ctx (xml);
   
   ctx.indent ();
   _root.write_xml (ctx);
   ctx.dedent ();
   
   xml += "</document>\n";
   
   return 0;
}



/*
==============================================================================
Name : has_ref_collision
==============================================================================
*/

bool  ConvDocument::has_ref_collision () const
{
   std::set <flip::Ref> ref_set;
   
   return _root.has_ref_collision (ref_set);
}



/*
==============================================================================
Name : remap_refs
==============================================================================
*/

void  ConvDocument::remap_refs ()
{
   UserIdThreadIdRefMap user_id_thread_id_ref_map;
   RefConvMap ref_conv_map;
   
   _root.remap (user_id_thread_id_ref_map, ref_conv_map);
   
   _root.rebind (ref_conv_map);
   
#if defined _DEBUG
// _root.trace ();
#endif
   
   assert (!has_ref_collision ());
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
