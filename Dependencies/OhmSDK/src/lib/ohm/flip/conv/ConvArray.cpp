/*****************************************************************************

        ConvArray.cpp
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

#include "ohm/flip/conv/ConvArray.h"
#include "ohm/flip/private/BasicTypesName.h"
#include "ohm/flip/ClassDescManager.h"

#include "ohm/txt/out/Print.h"
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
==============================================================================
*/

ConvArray::ConvArray ()
:  ConvObject ()
,  _content ()
,  _elem_class_id (-1)
{
   _class_name = get_class_name ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ConvArray::~ConvArray ()
{
}



/*
==============================================================================
Name : has_ref_collision
==============================================================================
*/

bool  ConvArray::has_ref_collision (std::set <flip::Ref> & ref_set) const
{
   bool collision_flag = ConvObject::has_ref_collision (ref_set);
   
   // propagate

   if (!collision_flag)
   {
      ElementList::const_iterator it = _content.begin ();
      const ElementList::const_iterator it_end = _content.end ();
      
      for (; it != it_end ; ++it)
      {
         const Element & element = *it;
         
         collision_flag = element._object_sptr->has_ref_collision (ref_set);
         
         if (collision_flag)
         {
            break;
         }
      }
   }
   
   return collision_flag;
}



/*
==============================================================================
Name : propagate_remap
==============================================================================
*/

void  ConvArray::propagate_remap (UserIdThreadIdRefMap & user_id_thread_id_ref_map, RefConvMap & ref_conv_map)
{
   // propagate to collections sub-objects
   
   ElementList::const_iterator it = _content.begin ();
   const ElementList::const_iterator it_end = _content.end ();
   
   for (; it != it_end ; ++it)
   {
      const Element & element = *it;
      
      element._object_sptr->remap (
         user_id_thread_id_ref_map, ref_conv_map
      );
   }
}



/*
==============================================================================
Name : rebind
==============================================================================
*/

void  ConvArray::rebind (RefConvMap & ref_conv_map)
{
   // propagate

   ElementList::const_iterator it = _content.begin ();
   const ElementList::const_iterator it_end = _content.end ();
   
   for (; it != it_end ; ++it)
   {
      const Element & element = *it;
      
      element._object_sptr->rebind (ref_conv_map);
   }
}



/*
==============================================================================
Name: propagate_collect_max_id
==============================================================================
*/

void ConvArray::propagate_collect_max_id (UserIdRefMap & user_id_ref_map)
{
   // propagate

   ElementList::const_iterator it = _content.begin ();
   const ElementList::const_iterator it_end = _content.end ();
   
   for (; it != it_end ; ++it)
   {
      const Element & element = *it;
      
      element._object_sptr->collect_max_id (user_id_ref_map);
   }
}



/*
==============================================================================
Name : collect
Description :
   collect writable objects :
   - BasicTypes are not writable
   - Blobs are writable iff they are non empty
   - All other objets are writable
==============================================================================
*/

void  ConvArray::collect (std::list <ConvObject *> & obj_ptr_arr)
{
   // "BasicTypes are not writable"
   
   // propagate
   
   ElementList::iterator it = _content.begin ();
   const ElementList::iterator it_end = _content.end ();
   
   for (; it != it_end ; ++it)
   {
      Element & element = *it;
      
      element._object_sptr->collect (obj_ptr_arr);
   }
}



/*
==============================================================================
Name: propagate_trace
==============================================================================
*/

void ConvArray::propagate_trace ()
{
   // propagate
   
   ElementList::iterator it = _content.begin ();
   const ElementList::iterator it_end = _content.end ();
   
   for (; it != it_end ; ++it)
   {
      Element & element = *it;
      
      ohm_util_TRACE_DEBUG ("sub-object");
      element._object_sptr->trace ();
   }
}



/*
==============================================================================
Name : write_xml
==============================================================================
*/

void  ConvArray::write_xml (ContextXml & ctx) const
{
   txt::out::Print printer;
   
   ctx.print_begin (printer, *this);
   
   if (_content.empty ())
   {
      printer.cat ("/>");
      ctx.add_line (printer);
   }
   else
   {
      printer.cat (">");
      ctx.add_line (printer);
      
      ctx.indent ();
      
      ElementList::const_iterator it = _content.begin ();
      const ElementList::const_iterator it_end = _content.end ();
      
      for (; it != it_end ; ++it)
      {
         const Element & element = *it;
         
         std::string key;
         element._key.write (key);
         
         ctx._key = key;
         element._object_sptr->write_xml (ctx);
      }
      
      ctx.dedent ();
      
      printer.print ("</%1%>").arg (_class_name.c_str ());
      ctx.add_line (printer);
   }
}



/*
==============================================================================
Name: get_class_name
==============================================================================
*/

const char * ConvArray::get_class_name ()
{
   return BasicTypesName::_array_0;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_write_member
==============================================================================
*/

void  ConvArray::do_write_member (DataStreamOutput & dst) const
{
   if (_elem_class_id == ClassDescManager::BasicTypeId_BOOL)
   {
      DocumentFormatter::write_member_array_bool_begin (dst);
   }
   else if (_elem_class_id == ClassDescManager::BasicTypeId_INT64)
   {
      DocumentFormatter::write_member_array_int64_begin (dst);
   }
   else if (_elem_class_id == ClassDescManager::BasicTypeId_ENUM)
   {
      // not supported
      assert (false);
   }
   else if (_elem_class_id == ClassDescManager::BasicTypeId_FLOAT64)
   {
      DocumentFormatter::write_member_array_float64_begin (dst);
   }
   else if (_elem_class_id == ClassDescManager::BasicTypeId_BLOB)
   {
      DocumentFormatter::write_member_array_blob_begin (dst);
   }
   else if (_elem_class_id == ClassDescManager::BasicTypeId_OBJECT_REF)
   {
      DocumentFormatter::write_member_array_object_ref_begin (dst);
   }
   else
   {
      DocumentFormatter::write_member_array_object_begin (dst);
   }
   
   ElementList::const_iterator it = _content.begin ();
   const ElementList::const_iterator it_end = _content.end ();
   
   for (; it != it_end ; ++it)
   {
      const Element & element = *it;
      
      DocumentFormatter::write_member_array_key_ref (
         dst,
         element._key,
         element._object_sptr->_ref
      );
      
      element._object_sptr->write_member_container (dst);
   }
   
   DocumentFormatter::write_member_array_end (dst);
}



/*
==============================================================================
Name : do_write_member_container
==============================================================================
*/

void  ConvArray::do_write_member_container (DataStreamOutput & dst) const
{
   // not supported
   
   assert (false);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
