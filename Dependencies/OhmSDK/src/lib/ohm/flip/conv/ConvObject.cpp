/*****************************************************************************

        ConvObject.cpp
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

#include "ohm/flip/conv/ConvObject.h"
#include "ohm/flip/conv/ConvArray.h"
#include "ohm/flip/conv/ConvBlob.h"
#include "ohm/flip/conv/ConvBool.h"
#include "ohm/flip/conv/ConvCollection.h"
#include "ohm/flip/conv/ConvCue.h"
#include "ohm/flip/conv/ConvEnum.h"
#include "ohm/flip/conv/ConvInt64.h"
#include "ohm/flip/conv/ConvFloat64.h"
#include "ohm/flip/conv/ConvObjectRef.h"
#include "ohm/flip/private/BinaryKey.h"
#include "ohm/flip/private/SeedGenerator.h"
#include "ohm/flip/ClassDescManager.h"

#include "ohm/lang/fnc.h"
#include "ohm/odbg/LogMgr.h"
#include "ohm/txt/out/Print.h"
#include "ohm/util/trace.h"

#include <cassert>

#undef OHM_FLIP_CONVOBJECT_DEBUG_FLAG

#if defined (OHM_FLIP_CONVOBJECT_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_FLIP_CONVOBJECT_DEBUG_FLAG is defined in shipping level code !
   #endif
   
   #define ofc_TRACE(msg_0)   ohm_util_TRACE_DEBUG(msg_0)
   #define ofc_TRACE1(fmt_0, arg1)  ohm_util_TRACE_DEBUG1(fmt_0, arg1)
   #define ofc_TRACE2(fmt_0, arg1, arg2)  ohm_util_TRACE_DEBUG2(fmt_0, arg1, arg2)
   #define ofc_TRACE3(fmt_0, arg1, arg2, arg3)  ohm_util_TRACE_DEBUG3(fmt_0, arg1, arg2, arg3)
   #define ofc_TRACE4(fmt_0, arg1, arg2, arg3, arg4)  ohm_util_TRACE_DEBUG4(fmt_0, arg1, arg2, arg3, arg4)
   #define ofc_TRACE5(fmt_0, arg1, arg2, arg3, arg4, arg5)  ohm_util_TRACE_DEBUG5(fmt_0, arg1, arg2, arg3, arg4, arg5)
#else
   #define ofc_TRACE(msg_0)   (void (0))
   #define ofc_TRACE1(fmt_0, arg1)  (void (0))
   #define ofc_TRACE2(fmt_0, arg1, arg2)  (void (0))
   #define ofc_TRACE3(fmt_0, arg1, arg2, arg3)  (void (0))
   #define ofc_TRACE4(fmt_0, arg1, arg2, arg3, arg4)  (void (0))
   #define ofc_TRACE5(fmt_0, arg1, arg2, arg3, arg4, arg5)  (void (0))
#endif



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

ConvObject::ConvObject ()
:  _ref (0)
,  _class_name ()
,  _attribute_arr ()
,  _parent_ptr (0)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ConvObject::~ConvObject ()
{
}



/*
==============================================================================
Name : read
Description :
   Reader for generic (that is not BasicType) object.
==============================================================================
*/

int   ConvObject::read (DocumentParser & dp, flip::Ref ref)
{
   int err = 0;
   
   _ref = ref;
   
   DocumentParser::Object object;
   
   if (err == 0)
   {
      err = dp.seek_object (object, _ref);
   }
   
   if (err == 0)
   {
      err = object.read_class_name (_class_name);
   }
   
   bool loop_flag = true;
   
   flip::Ref sub_ref = ref + 1;
   
   while (loop_flag)
   {
      err = read_attribute (dp, object, sub_ref);
      
      if (err == Err_DOC_NO_MORE_ATTRIBUTES)
      {
         loop_flag = false;
         err = 0;
      }
      else
      {
         if (err != 0)
         {
            break;
         }
      }
   }
   
   return err;
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  ConvObject::write (DataStreamOutput & dst) const
{
   DocumentFormatter::write_begin_obj (dst, _ref, _class_name);
   
   if (!_attribute_arr.empty ())
   {
      std::list <Attribute>::const_iterator it = _attribute_arr.begin ();
      const std::list <Attribute>::const_iterator it_end = _attribute_arr.end ();
      
      for (; it != it_end ; ++it)
      {
         const Attribute & attribute = *it;
         
         DocumentFormatter::write_member (dst, attribute._name);
         
         ConvObject * member_ptr = &*attribute._object_sptr;
         assert (member_ptr != 0);
         
         member_ptr->do_write_member (dst);
      }
   }
   
   DocumentFormatter::write_end_obj (dst);
}



/*
==============================================================================
Name : write_xml
==============================================================================
*/

void  ConvObject::write_xml (ContextXml & ctx) const
{
   txt::out::Print printer;
   
   ctx.print_begin (printer, *this);
   printer.cat (">");
   ctx.add_line (printer);
   
   ctx.indent ();
   
   std::list <Attribute>::const_iterator it = _attribute_arr.begin ();
   const std::list <Attribute>::const_iterator it_end = _attribute_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      const Attribute & attribute = *it;
      
      ConvObject * member_ptr = &*attribute._object_sptr;
      assert (member_ptr != 0);
      
      ctx._attribute_name = attribute._name;
      member_ptr->write_xml (ctx);
   }
   
   ctx.dedent ();
   
   printer.print ("</%1%>").arg (_class_name.c_str ());
   ctx.add_line (printer);
}



/*
==============================================================================
Name : has_ref_collision
==============================================================================
*/

bool  ConvObject::has_ref_collision (std::set <flip::Ref> & ref_set) const
{
   bool collision_flag = ref_set.find (_ref) != ref_set.end ();

   assert (!collision_flag);
   
   if (!collision_flag)
   {
      ref_set.insert (_ref);
      
      // propagate
      
      std::list <Attribute>::const_iterator it = _attribute_arr.begin ();
      const std::list <Attribute>::const_iterator it_end = _attribute_arr.end ();
      
      for (; it != it_end ; ++it)
      {
         const Attribute & attribute = *it;
         
         collision_flag = attribute._object_sptr->has_ref_collision (ref_set);
         
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
Name : remap
==============================================================================
*/

void  ConvObject::remap (UserIdThreadIdRefMap & user_id_thread_id_ref_map, RefConvMap & ref_conv_map)
{
   archi::Int64 & cur_ref = get_cur_ref (user_id_thread_id_ref_map, _ref);
   
   remap (cur_ref, user_id_thread_id_ref_map, ref_conv_map);
}



/*
==============================================================================
Name : propagate_remap
==============================================================================
*/

void  ConvObject::propagate_remap (UserIdThreadIdRefMap & user_id_thread_id_ref_map, RefConvMap & ref_conv_map)
{
   // default impl : nothing. 
   // not a collection - no children
   ofc_TRACE2 ("<> propagate_remap %1% [%2%]", _class_name.c_str (), _ref);
}



/*
==============================================================================
Name : rebind
==============================================================================
*/

void  ConvObject::rebind (RefConvMap & ref_conv_map)
{
   // propagate
   {
      std::list <Attribute>::const_iterator it = _attribute_arr.begin ();
      const std::list <Attribute>::const_iterator it_end = _attribute_arr.end ();
      
      for (; it != it_end ; ++it)
      {
         const Attribute & attribute = *it;
         
         attribute._object_sptr->rebind (ref_conv_map);
      }
   }
}



/*
==============================================================================
Name: collect_max_id
==============================================================================
*/

void ConvObject::collect_max_id (UserIdRefMap & user_id_ref_map)
{
   archi::Int32 user_id = flip::SeedGenerator::to_user_id (_ref);
   
   UserIdRefMap::iterator it = user_id_ref_map.find (user_id);
   
   if (it == user_id_ref_map.end ())
   {
      user_id_ref_map.insert (std::make_pair (user_id, _ref));
   }
   else
   {
      assert (it->second != _ref);
      
      lang::max_in_place (it->second, _ref);
   }
   
   // propagate to attibutes
   {
      std::list <Attribute>::const_iterator it = _attribute_arr.begin ();
      const std::list <Attribute>::const_iterator it_end = _attribute_arr.end ();
      
      for (; it != it_end ; ++it)
      {
         const Attribute & attribute = *it;
         
         attribute._object_sptr->collect_max_id (user_id_ref_map);
      }
   }
   
   // propagate in collections
   
   propagate_collect_max_id (user_id_ref_map);
}



/*
==============================================================================
Name: propagate_collect_max_id
==============================================================================
*/

void ConvObject::propagate_collect_max_id (UserIdRefMap & user_id_ref_map)
{
   // default impl : does nothing
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

void  ConvObject::collect (std::list <ConvObject *> & obj_ptr_arr)
{
   // default implementation "All other objets are writable"
   
   obj_ptr_arr.push_back (this);
   
   // propagate
   
   std::list <Attribute>::iterator it = _attribute_arr.begin ();
   const std::list <Attribute>::iterator it_end = _attribute_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      Attribute & attribute = *it;
      
      attribute._object_sptr->collect (obj_ptr_arr);
   }
}



/*
==============================================================================
Name : write_member_container
==============================================================================
*/

void  ConvObject::write_member_container (DataStreamOutput & dst) const
{
   do_write_member_container (dst);
}



/*
==============================================================================
Name: trace
==============================================================================
*/

void ConvObject::trace ()
{
   ohm_util_TRACE_DEBUG2 (
      ">> trace '%1%' [%2%]", _class_name.c_str (), _ref
   );

   odbg::LogMgr::use_instance ().indent (3);
   
   // propagate to attibutes
   {
      std::list <Attribute>::const_iterator it = _attribute_arr.begin ();
      const std::list <Attribute>::const_iterator it_end = _attribute_arr.end ();
      
      for (; it != it_end ; ++it)
      {
         const Attribute & attribute = *it;
         
         ohm_util_TRACE_DEBUG1 (
            "attribute name %1%",
            attribute._name.c_str ()
         );
         attribute._object_sptr->trace ();
      }
   }
   
   // propagate in collections
   
   propagate_trace ();

   odbg::LogMgr::use_instance ().indent (-3);

   ohm_util_TRACE_DEBUG ("<<");
}



/*
==============================================================================
Name: propagate_trace
==============================================================================
*/

void ConvObject::propagate_trace ()
{
   // default impl : do nothing
}



/*
==============================================================================
Name : get_nbr_ref
==============================================================================
*/

archi::Int64   ConvObject::get_nbr_ref () const
{
   archi::Int64 nbr_ref = 1;
   
   std::list <Attribute>::const_iterator it = _attribute_arr.begin ();
   const std::list <Attribute>::const_iterator it_end = _attribute_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      const Attribute & attribute = *it;
      
      nbr_ref += attribute._object_sptr->get_nbr_ref ();
   }
   
   return nbr_ref;
}



/*
==============================================================================
Name : get_nbr_ref
==============================================================================
*/

archi::Int64 & ConvObject::get_cur_ref (UserIdThreadIdRefMap & user_id_thread_id_ref_map, archi::Int64 ref)
{
   // ref can be 0 (it will be allocated to SERVER_USER_ID, 0)
   archi::Int32 user_id = SeedGenerator::to_user_id (ref);
   archi::Int32 thread_id = SeedGenerator::to_thread_id (ref);
   
   std::pair <archi::Int32, archi::Int32> user_thread (user_id, thread_id);
   
   UserIdThreadIdRefMap::iterator it = user_id_thread_id_ref_map.find (user_thread);
   
   if (it == user_id_thread_id_ref_map.end ())
   {
      it = user_id_thread_id_ref_map.insert (
         std::make_pair (user_thread, SeedGenerator::get (user_id, thread_id))
      ).first;
   }
   
   archi::Int64 & cur_ref = it->second;
   
   return cur_ref;
}



/*
==============================================================================
Name : add_line
==============================================================================
*/

void  ConvObject::ContextXml::add_line (const std::string & xml)
{
   for (size_t i = 0 ; i < _indent ; ++i)
   {
      _xml += "\t";
   }
   
   _xml += xml + "\n";
}



/*
==============================================================================
Name : indent
==============================================================================
*/

void  ConvObject::ContextXml::indent ()
{
   ++_indent;
}



/*
==============================================================================
Name : dedent
==============================================================================
*/

void  ConvObject::ContextXml::dedent ()
{
   assert (_indent > 0);
   --_indent;
}



/*
==============================================================================
Name : print_begin
==============================================================================
*/

void  ConvObject::ContextXml::print_begin (txt::out::Print & printer, const ConvObject & object)
{
   printer.print ("<%1%").arg (object._class_name.c_str ());
   
   if (!_attribute_name.empty ())
   {
      printer.cat (" name=\"%1%\"").arg (_attribute_name.c_str ());
      _attribute_name.clear ();
   }
   
   printer.cat (" ref=\"%1%\"").arg (object._ref);
   
   if (!_key.empty ())
   {
      printer.cat (" key=\"%1%\"").arg (_key.c_str ());
      _key.clear ();
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_write_member
==============================================================================
*/

void  ConvObject::do_write_member (DataStreamOutput & dst) const
{
   DocumentFormatter::write_member_object (dst, _ref);
}



/*
==============================================================================
Name : do_write_member_container
==============================================================================
*/

void  ConvObject::do_write_member_container (DataStreamOutput & dst) const
{
   // nothing
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : read_attribute
==============================================================================
*/

int   ConvObject::read_attribute (DocumentParser & dp, DocumentParser::Object & object, flip::Ref & sub_ref)
{
   int err = 0;
   
   Attribute attribute;
   
   if (err == 0)
   {
      err = object.read_attribute_name (attribute._name);
   }
   
   DocumentParser::Object::Type type = DocumentParser::Object::Type_VOID;
   
   if (err == 0)
   {
      err = object.read_type (type);
   }
   
   if (err == 0)
   {
      switch (type)
      {
      case DocumentParser::Object::Type_OBJECT:
         err = read_attribute_object (dp, object, attribute, sub_ref);
         break;

      case DocumentParser::Object::Type_BOOL:
         err = read_attribute_bool (dp, object, attribute, sub_ref);
         break;

      case DocumentParser::Object::Type_INT64:
         err = read_attribute_int64 (dp, object, attribute, sub_ref);
         break;

      case DocumentParser::Object::Type_ENUM:
         err = read_attribute_enum (dp, object, attribute, sub_ref);
         break;

      case DocumentParser::Object::Type_FLOAT64:
         err = read_attribute_float64 (dp, object, attribute, sub_ref);
         break;

      case DocumentParser::Object::Type_BLOB:
         err = read_attribute_blob (dp, object, attribute, sub_ref);
         break;

      case DocumentParser::Object::Type_CUE:
         err = read_attribute_cue (dp, object, attribute, sub_ref);
         break;

      case DocumentParser::Object::Type_ARRAY:
         err = read_attribute_array (dp, object, attribute, sub_ref);
         break;

      case DocumentParser::Object::Type_COLLECTION:
         err = read_attribute_collection (dp, object, attribute, sub_ref);
         break;

      case DocumentParser::Object::Type_OBJECT_REF:
         err = read_attribute_object_ref (dp, object, attribute, sub_ref);
         break;
      
      default:
         err = Err_DOC_INTERNAL_SYSTEM_ERROR;
         break;
      };
   }
   
   if (err == 0)
   {
      _attribute_arr.push_back (attribute);
   }
   
   return err;
}



/*
==============================================================================
Name : read_attribute_object
==============================================================================
*/

int   ConvObject::read_attribute_object (DocumentParser & dp, DocumentParser::Object & object, Attribute & attribute, flip::Ref & sub_ref)
{
   int err = 0;
   
   if (err == 0)
   {
      flip::Ref ann_ref = 0;
   
      err = object.read_ref (ann_ref);
      
      assert (ann_ref == sub_ref);
   }
   
   if (err == 0)
   {
      ConvObject * sub_object_ptr = new ConvObject;
      sub_object_ptr->_parent_ptr = this;
      
      attribute._object_sptr = ConvSharedPtr <ConvObject> (sub_object_ptr);
      
      err = sub_object_ptr->read (dp, sub_ref);
   }
   
   if (err == 0)
   {
      sub_ref += attribute._object_sptr->get_nbr_ref ();
   }
   
   return err;
}



/*
==============================================================================
Name : read_attribute_bool
==============================================================================
*/

int   ConvObject::read_attribute_bool (DocumentParser & dp, DocumentParser::Object & object, Attribute & attribute, flip::Ref & sub_ref)
{
   int err = 0;
   
   bool val = 0;
   
   if (err == 0)
   {
      err = object.read_bool (val);
   }
   
   if (err == 0)
   {
      ConvBool * sub_object_ptr = new ConvBool;
      sub_object_ptr->_parent_ptr = this;
      
      attribute._object_sptr = ConvSharedPtr <ConvObject> (sub_object_ptr);
      
      sub_object_ptr->_ref = sub_ref;
      sub_object_ptr->_val = val;
   }
   
   sub_ref += 1;
   
   return err;
}



/*
==============================================================================
Name : read_attribute_int64
==============================================================================
*/

int   ConvObject::read_attribute_int64 (DocumentParser & dp, DocumentParser::Object & object, Attribute & attribute, flip::Ref & sub_ref)
{
   int err = 0;
   
   archi::Int64 val = 0;
   
   if (err == 0)
   {
      err = object.read_int64 (val);
   }
   
   if (err == 0)
   {
      ConvInt64 * sub_object_ptr = new ConvInt64;
      sub_object_ptr->_parent_ptr = this;
      
      attribute._object_sptr = ConvSharedPtr <ConvObject> (sub_object_ptr);
      
      sub_object_ptr->_ref = sub_ref;
      sub_object_ptr->_val = val;
   }
   
   sub_ref += 1;
   
   return err;
}



/*
==============================================================================
Name : read_attribute_enum
==============================================================================
*/

int   ConvObject::read_attribute_enum (DocumentParser & dp, DocumentParser::Object & object, Attribute & attribute, flip::Ref & sub_ref)
{
   int err = 0;
   
   std::string enum_name;
   
   if (err == 0)
   {
      err = object.read_sub_enum_name (enum_name);
   }
   
   std::string val;
   
   if (err == 0)
   {
      err = object.read_enum (val);
   }
   
   if (err == 0)
   {
      ConvEnum * sub_object_ptr = new ConvEnum;
      sub_object_ptr->_parent_ptr = this;
      
      attribute._object_sptr = ConvSharedPtr <ConvObject> (sub_object_ptr);
      
      sub_object_ptr->_ref = sub_ref;
      sub_object_ptr->_enum_name = enum_name;
      sub_object_ptr->_val = val;
   }
   
   sub_ref += 1;
   
   return err;
}



/*
==============================================================================
Name : read_attribute_float64
==============================================================================
*/

int   ConvObject::read_attribute_float64 (DocumentParser & dp, DocumentParser::Object & object, Attribute & attribute, flip::Ref & sub_ref)
{
   int err = 0;
   
   archi::Float64 val = 0;
   
   if (err == 0)
   {
      err = object.read_float64 (val);
   }
   
   if (err == 0)
   {
      ConvFloat64 * sub_object_ptr = new ConvFloat64;
      sub_object_ptr->_parent_ptr = this;
      
      attribute._object_sptr = ConvSharedPtr <ConvObject> (sub_object_ptr);
      
      sub_object_ptr->_ref = sub_ref;
      sub_object_ptr->_val = val;
   }
   
   sub_ref += 1;
   
   return err;
}



/*
==============================================================================
Name : read_attribute_blob
==============================================================================
*/

int   ConvObject::read_attribute_blob (DocumentParser & dp, DocumentParser::Object & object, Attribute & attribute, flip::Ref & sub_ref)
{
   int err = 0;
   
   bool inline_flag = false;
   bool empty_flag = false;
   std::vector <Codec::Encoder> codec_arr;
   archi::UByte * data_ptr = 0;
   size_t data_length = 0;
   
   if (err == 0)
   {
      flip::Ref ann_ref = 0;
   
      err = object.read_blob (inline_flag, empty_flag, ann_ref, codec_arr, data_ptr, data_length);
      
      if (err == 0)
      {
         if (!empty_flag)
         {
            if (inline_flag)
            {
               assert (data_ptr != 0);
               assert (data_length != 0);
            }
            else
            {
               assert (ann_ref == sub_ref);
            }
         }
      }
   }
   
   if (err == 0)
   {
      ConvBlob * sub_object_ptr = new ConvBlob;
      sub_object_ptr->_parent_ptr = this;
      
      attribute._object_sptr = ConvSharedPtr <ConvObject> (sub_object_ptr);
      
      sub_object_ptr->_ref = sub_ref;
      sub_object_ptr->_inline_flag = inline_flag;
      
      if (inline_flag)
      {
         sub_object_ptr->_encoder_arr = codec_arr;
         
         if (!empty_flag)
         {
            sub_object_ptr->_data.assign (data_ptr, data_ptr + data_length);
         }
      }
      else
      {
         if (!empty_flag)
         {
            err = dp.read_blob (sub_object_ptr->_data, sub_object_ptr->_encoder_arr, sub_ref);
         }
      }
   }
   
   sub_ref += 1;
   
   return err;
}



/*
==============================================================================
Name : read_attribute_cue
==============================================================================
*/

int   ConvObject::read_attribute_cue (DocumentParser & dp, DocumentParser::Object & object, Attribute & attribute, flip::Ref & sub_ref)
{
   int err = 0;
   
   if (err == 0)
   {
      ConvCue * sub_object_ptr = new ConvCue;
      sub_object_ptr->_parent_ptr = this;
      
      attribute._object_sptr = ConvSharedPtr <ConvObject> (sub_object_ptr);
      
      sub_object_ptr->_ref = sub_ref;
   }
   
   sub_ref += 1;
   
   return err;
}



/*
==============================================================================
Name : read_attribute_object_ref
==============================================================================
*/

int   ConvObject::read_attribute_object_ref (DocumentParser & dp, DocumentParser::Object & object, Attribute & attribute, flip::Ref & sub_ref)
{
   int err = 0;
   
   flip::Ref pointed_ref = 0;
   
   if (err == 0)
   {
      err = object.read_ref (pointed_ref);
   }
   
   if (err == 0)
   {
      ConvObjectRef * sub_object_ptr = new ConvObjectRef;
      sub_object_ptr->_parent_ptr = this;
      
      attribute._object_sptr = ConvSharedPtr <ConvObject> (sub_object_ptr);
      
      sub_object_ptr->_ref = sub_ref;
      sub_object_ptr->_pointed_ref = pointed_ref;
   }
   
   sub_ref += 1;
   
   return err;
}



/*
==============================================================================
Name : read_attribute_array
==============================================================================
*/

int   ConvObject::read_attribute_array (DocumentParser & dp, DocumentParser::Object & object, Attribute & attribute, flip::Ref & sub_ref)
{
   int err = 0;
   
   DocumentParser::Object::Type sub_type = DocumentParser::Object::Type_VOID;
   
   if (err == 0)
   {
      err = object.read_sub_type (sub_type);
   }
   
   if (err == 0)
   {
      ConvArray * sub_object_ptr = new ConvArray;
      sub_object_ptr->_parent_ptr = this;
      
      attribute._object_sptr = ConvSharedPtr <ConvObject> (sub_object_ptr);
      
      sub_object_ptr->_ref = sub_ref;
      
      switch (sub_type)
      {
      case DocumentParser::Object::Type_OBJECT:
         sub_object_ptr->_elem_class_id = -1;   // special for object
         err = read_attribute_array_object (dp, object, *sub_object_ptr);
         break;

      case DocumentParser::Object::Type_INT64:
         sub_object_ptr->_elem_class_id = ClassDescManager::BasicTypeId_INT64;
         err = read_attribute_array_int64 (dp, object, *sub_object_ptr);
         break;

      case DocumentParser::Object::Type_FLOAT64:
         sub_object_ptr->_elem_class_id = ClassDescManager::BasicTypeId_FLOAT64;
         err = read_attribute_array_float64 (dp, object, *sub_object_ptr);
         break;
      
      default:
         err = Err_DOC_UNEXPECTED_TYPE;
         break;
      }
   }
   
   sub_ref += 1;
   
   return err;
}



/*
==============================================================================
Name : read_attribute_array_object
==============================================================================
*/

int   ConvObject::read_attribute_array_object (DocumentParser & dp, DocumentParser::Object & object, ConvArray & array)
{
   int err = 0;
   
   bool loop_flag = true;
   
   while (loop_flag)
   {
      BinaryKey key;
      err = object.read_binary_key (key);
      
      if (err == 0)
      {
         flip::Ref ref = 0;
         err = object.read_ref (ref);
         
         if (ref == 0)
         {
            err = Err_DOC_BAD_REFERENCE;
         }
         
         if (err == 0)
         {
            ConvObject * sub_object_ptr = new ConvObject;
            sub_object_ptr->_parent_ptr = &array;
         
            err = sub_object_ptr->read (dp, ref);
            
            ConvArray::Element element;
            element._key = key;
            element._object_sptr = ConvSharedPtr <ConvObject> (sub_object_ptr);
            
            array._content.push_back (element);
         }
      }
      else
      {
         loop_flag = false;
         
         if (err == Err_DOC_ITERATION_NO_MORE_ITEMS)
         {
            err = 0;
         }
      }
   }
   
   return err;
}



/*
==============================================================================
Name : read_attribute_array_int64
==============================================================================
*/

int   ConvObject::read_attribute_array_int64 (DocumentParser & dp, DocumentParser::Object & object, ConvArray & array)
{
   int err = 0;
   
   bool loop_flag = true;
   
   while (loop_flag)
   {
      BinaryKey key;
      err = object.read_binary_key (key);
      
      if (err == 0)
      {
         flip::Ref ref = 0;
         err = object.read_ref (ref);
         
         if (ref == 0)
         {
            err = Err_DOC_BAD_REFERENCE;
         }
         
         archi::Int64 val = 0;
         
         if (err == 0)
         {
            err = object.read_int64 (val);
         }
         
         if (err == 0)
         {
            ConvInt64 * sub_object_ptr = new ConvInt64;
            sub_object_ptr->_parent_ptr = &array;
         
            sub_object_ptr->_ref = ref;
            sub_object_ptr->_val = val;
            
            ConvArray::Element element;
            element._key = key;
            element._object_sptr = ConvSharedPtr <ConvObject> (sub_object_ptr);
            
            array._content.push_back (element);
         }
      }
      else
      {
         loop_flag = false;
         
         if (err == Err_DOC_ITERATION_NO_MORE_ITEMS)
         {
            err = 0;
         }
      }
   }
   
   return err;
}



/*
==============================================================================
Name : read_attribute_array_float64
==============================================================================
*/

int   ConvObject::read_attribute_array_float64 (DocumentParser & dp, DocumentParser::Object & object, ConvArray & array)
{
   int err = 0;
   
   bool loop_flag = true;
   
   while (loop_flag)
   {
      BinaryKey key;
      err = object.read_binary_key (key);
      
      if (err == 0)
      {
         flip::Ref ref = 0;
         err = object.read_ref (ref);
         
         if (ref == 0)
         {
            err = Err_DOC_BAD_REFERENCE;
         }
         
         archi::Float64 val = 0.0;
         
         if (err == 0)
         {
            err = object.read_float64 (val);
         }
         
         if (err == 0)
         {
            ConvFloat64 * sub_object_ptr = new ConvFloat64;
            sub_object_ptr->_parent_ptr = &array;
         
            sub_object_ptr->_ref = ref;
            sub_object_ptr->_val = val;
            
            ConvArray::Element element;
            element._key = key;
            element._object_sptr = ConvSharedPtr <ConvObject> (sub_object_ptr);
            
            array._content.push_back (element);
         }
      }
      else
      {
         loop_flag = false;
         
         if (err == Err_DOC_ITERATION_NO_MORE_ITEMS)
         {
            err = 0;
         }
      }
   }
   
   return err;
}



/*
==============================================================================
Name : read_attribute_collection
==============================================================================
*/

int   ConvObject::read_attribute_collection (DocumentParser & dp, DocumentParser::Object & object, Attribute & attribute, flip::Ref & sub_ref)
{
   int err = 0;
   
   DocumentParser::Object::Type sub_type = DocumentParser::Object::Type_VOID;
   
   if (err == 0)
   {
      err = object.read_sub_type (sub_type);
   }
   
   if (err == 0)
   {
      ConvCollection * sub_object_ptr = new ConvCollection;
      sub_object_ptr->_parent_ptr = this;
      
      attribute._object_sptr = ConvSharedPtr <ConvObject> (sub_object_ptr);
      
      sub_object_ptr->_ref = sub_ref;
      
      switch (sub_type)
      {
      case DocumentParser::Object::Type_OBJECT:
         sub_object_ptr->_elem_class_id = -1;   // special for object
         err = read_attribute_collection_object (dp, object, *sub_object_ptr);
         break;

      case DocumentParser::Object::Type_INT64:
         sub_object_ptr->_elem_class_id = ClassDescManager::BasicTypeId_INT64;
         err = read_attribute_collection_int64 (dp, object, *sub_object_ptr);
         break;

      case DocumentParser::Object::Type_FLOAT64:
         sub_object_ptr->_elem_class_id = ClassDescManager::BasicTypeId_FLOAT64;
         err = read_attribute_collection_float64 (dp, object, *sub_object_ptr);
         break;
      
      default:
         err = Err_DOC_UNEXPECTED_TYPE;
         break;
      }
   }
   
   sub_ref += 1;
   
   return err;
}



/*
==============================================================================
Name : read_attribute_collection_object
==============================================================================
*/

int   ConvObject::read_attribute_collection_object (DocumentParser & dp, DocumentParser::Object & object, ConvCollection & collection)
{
   int err = 0;
   
   bool loop_flag = true;
   
   while (loop_flag)
   {
      flip::Ref ref = 0;
      err = object.read_ref (ref);
      
      if (err == 0)
      {
         if (ref == 0)
         {
            err = Err_DOC_BAD_REFERENCE;
         }
         
         if (err == 0)
         {
            ConvObject * sub_object_ptr = new ConvObject;
            sub_object_ptr->_parent_ptr = &collection;
         
            err = sub_object_ptr->read (dp, ref);
            
            ConvCollection::Element element;
            element._object_sptr = ConvSharedPtr <ConvObject> (sub_object_ptr);
            
            collection._content.push_back (element);
         }
      }
      else
      {
         loop_flag = false;
         
         if (err == Err_DOC_ITERATION_NO_MORE_ITEMS)
         {
            err = 0;
         }
      }
   }
   
   return err;
}



/*
==============================================================================
Name : read_attribute_collection_int64
==============================================================================
*/

int   ConvObject::read_attribute_collection_int64 (DocumentParser & dp, DocumentParser::Object & object, ConvCollection & collection)
{
   int err = 0;
   
   bool loop_flag = true;
   
   while (loop_flag)
   {
      flip::Ref ref = 0;
      err = object.read_ref (ref);
         
      if (err == 0)
      {
         if (ref == 0)
         {
            err = Err_DOC_BAD_REFERENCE;
         }
         
         archi::Int64 val = 0;
         
         if (err == 0)
         {
            err = object.read_int64 (val);
         }
         
         if (err == 0)
         {
            ConvInt64 * sub_object_ptr = new ConvInt64;
            sub_object_ptr->_parent_ptr = &collection;
         
            sub_object_ptr->_ref = ref;
            sub_object_ptr->_val = val;
            
            ConvCollection::Element element;
            element._object_sptr = ConvSharedPtr <ConvObject> (sub_object_ptr);
            
            collection._content.push_back (element);
         }
      }
      else
      {
         loop_flag = false;
         
         if (err == Err_DOC_ITERATION_NO_MORE_ITEMS)
         {
            err = 0;
         }
      }
   }
   
   return err;
}



/*
==============================================================================
Name : read_attribute_collection_float64
==============================================================================
*/

int   ConvObject::read_attribute_collection_float64 (DocumentParser & dp, DocumentParser::Object & object, ConvCollection & collection)
{
   int err = 0;
   
   bool loop_flag = true;
   
   while (loop_flag)
   {
      flip::Ref ref = 0;
      err = object.read_ref (ref);
         
      if (err == 0)
      {
         if (ref == 0)
         {
            err = Err_DOC_BAD_REFERENCE;
         }
         
         archi::Float64 val = 0;
         
         if (err == 0)
         {
            err = object.read_float64 (val);
         }
         
         if (err == 0)
         {
            ConvFloat64 * sub_object_ptr = new ConvFloat64;
            sub_object_ptr->_parent_ptr = &collection;
         
            sub_object_ptr->_ref = ref;
            sub_object_ptr->_val = val;
            
            ConvCollection::Element element;
            element._object_sptr = ConvSharedPtr <ConvObject> (sub_object_ptr);
            
            collection._content.push_back (element);
         }
      }
      else
      {
         loop_flag = false;
         
         if (err == Err_DOC_ITERATION_NO_MORE_ITEMS)
         {
            err = 0;
         }
      }
   }
   
   return err;
}



/*
==============================================================================
Name : remap
==============================================================================
*/

void  ConvObject::remap (archi::Int64 & cur_ref, UserIdThreadIdRefMap & user_id_thread_id_ref_map, RefConvMap & ref_conv_map)
{
   ofc_TRACE1 (">> remap %1%", _class_name.c_str ());

   odbg::LogMgr::use_instance ().indent (3);

   remap_self (cur_ref, user_id_thread_id_ref_map, ref_conv_map);
   remap_attributes (cur_ref, user_id_thread_id_ref_map, ref_conv_map);
   remap_children (user_id_thread_id_ref_map, ref_conv_map);

   odbg::LogMgr::use_instance ().indent (-3);

   ofc_TRACE ("<<");
}



/*
==============================================================================
Name: remap_self
==============================================================================
*/

void ConvObject::remap_self (archi::Int64 & cur_ref, UserIdThreadIdRefMap & user_id_thread_id_ref_map, RefConvMap & ref_conv_map)
{
   if (_ref != 0)
   {
      ref_conv_map [_ref] = cur_ref;
   }
   
   ofc_TRACE3 (">> remap_self %1% %2% -> %3%", _class_name.c_str (), _ref, cur_ref);
   ofc_TRACE ("<<");

   _ref = cur_ref;
   
   ++cur_ref;

}



/*
==============================================================================
Name: remap_attributes
==============================================================================
*/

void ConvObject::remap_attributes (archi::Int64 & cur_ref, UserIdThreadIdRefMap & user_id_thread_id_ref_map, RefConvMap & ref_conv_map)
{
   ofc_TRACE2 (">> remap_attributes %1% [%2%]", _class_name.c_str (), _ref);

   odbg::LogMgr::use_instance ().indent (3);

   std::list <Attribute>::const_iterator it 
      = _attribute_arr.begin ();
   const std::list <Attribute>::const_iterator it_end 
      = _attribute_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      const Attribute & attribute = *it;
      
      attribute._object_sptr->remap_self (
         cur_ref, user_id_thread_id_ref_map, ref_conv_map
      );
      
      attribute._object_sptr->remap_attributes (
         cur_ref, user_id_thread_id_ref_map, ref_conv_map
      );
   }

   odbg::LogMgr::use_instance ().indent (-3);

   ofc_TRACE ("<<");
}



/*
==============================================================================
Name: remap_children
==============================================================================
*/

void ConvObject::remap_children (UserIdThreadIdRefMap & user_id_thread_id_ref_map, RefConvMap & ref_conv_map)
{
   ofc_TRACE2 (">> remap_children %1% [%2%]", _class_name.c_str (), _ref);
   
   odbg::LogMgr::use_instance ().indent (3);

   std::list <Attribute>::const_iterator it = _attribute_arr.begin ();
   const std::list <Attribute>::const_iterator it_end = _attribute_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      const Attribute & attribute = *it;
      
      attribute._object_sptr->remap_children (
         user_id_thread_id_ref_map, ref_conv_map
      );
   }
   
   propagate_remap (user_id_thread_id_ref_map, ref_conv_map);
   
   odbg::LogMgr::use_instance ().indent (-3);

   ofc_TRACE ("<<");
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
