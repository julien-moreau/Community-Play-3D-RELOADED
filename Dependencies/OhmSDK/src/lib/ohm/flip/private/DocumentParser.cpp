/*****************************************************************************

        DocumentParser.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 63856

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

#include "ohm/flip/private/DocumentParser.h"
#include "ohm/flip/Blob.h"
#include "ohm/flip/private/DocumentKeywords.h"
#include "ohm/flip/private/SeedGenerator.h"
#include "ohm/flip/private/EnumDescriptionMaster.h"
#include "ohm/txt/utf8/ConvNum.h"
#include "ohm/util/trace.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <cassert>
#include <cstring>



#define OHM_FLIP_DOCUMENT_READER_DEBUG_FLAG

#if defined (OHM_FLIP_DOCUMENT_READER_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_FLIP_DOCUMENT_READER_DEBUG_FLAG is defined in shipping level code !
   #endif

   #define dr_break assert (false)
#else
   #define dr_break ((void) 0)
#endif



namespace ohm
{
namespace flip
{



/*
==============================================================================
Description :
   This class handles reading of Ohm Document Format.
   It only checks for syntax & grammar errors.
   It is aimed to check for all possible problems, except for class and
   attribute existences verifications.
==============================================================================
*/




/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

DocumentParser::DocumentParser (DataStreamInput & data_stream)
:  _ds (data_stream)
,  _root_ref (0)
,  _rev_id (-1)
,  _ref_pos_map ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

DocumentParser::~DocumentParser ()
{
   
}



/*
==============================================================================
Name : print
Description :
   Print the data stream to std out
==============================================================================
*/

void  DocumentParser::print ()
{
   _ds.print ();
}



/*
==============================================================================
Name : init
==============================================================================
*/

int   DocumentParser::init ()
{
   _root_ref = 0;
   _rev_id = 0;
   _user_name_id_map.clear ();
   _ref_pos_map.clear ();

   // check header
   
   bool ok_flag = true;
   int err = _ds.is_equal (ok_flag, DocumentKeywords::_header_0);
   if (err != Err_OK) return err;
   
   if (!ok_flag)
   {
      dr_break;
      return Err_DOC_BAD_HEADER;
   }
   
   // read xref, root ref, and user name/id pairs
   
   err = read_file_end ();
   if (err != Err_OK) return err;

   return err;
}



/*
==============================================================================
Name : get_root_ref
==============================================================================
*/

flip::Ref   DocumentParser::get_root_ref () const
{
   assert (_root_ref != 0);
   
   return _root_ref;
}



/*
==============================================================================
Name : get_rev_id
==============================================================================
*/

archi::Int64   DocumentParser::get_rev_id () const
{
   // 0 means user side offline
   
   assert (_rev_id >= 0);
   
   return _rev_id;
}



/*
==============================================================================
Name : get_format_version
==============================================================================
*/

const std::string &  DocumentParser::get_format_version () const
{
   assert (_format_version != "");
   
   return _format_version;
}



/*
==============================================================================
Name : get_user_name_id_map
==============================================================================
*/

const DocumentParser::UserNameIdMap &  DocumentParser::get_user_name_id_map () const
{
   return _user_name_id_map;
}



/*
==============================================================================
Name : seek_object
==============================================================================
*/

int DocumentParser::seek_object (Object & object, flip::Ref ref)
{
   RefPosMap::const_iterator it = _ref_pos_map.find (ref);
   
   if (it == _ref_pos_map.end ())
   {
      dr_break;
      return Err_DOC_OBJECT_MISSING;
   }
   
   size_t ref_pos = it->second;

   int err = _ds.seek_beg (ref_pos);
   if (err != Err_OK) return err;

   flip::Ref ref2 = 0;
   err = _ds.read (ref2);
   if (err != Err_OK) return err;
   
   if (ref != ref2)
   {
      return Err_DOC_BAD_REFERENCE;
   }
   
   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;

   err = _ds.require_keyword (DocumentKeywords::_obj_0);
   if (err != Err_OK) return err;
   
   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;

   err = _ds.require_keyword (DocumentKeywords::_begin_dict_0);
   if (err != Err_OK) return err;
   
   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;
   
   object._dp_ptr = this;
   object._ds_ptr = &_ds;
   object._cur_type = Object::Type_VOID;
   object._cur_sub_type = Object::Type_VOID;
   
   // points to /ClassName

   object._pos = _ds.tell ();
   
   return Err_OK;
}



/*
==============================================================================
Name : Object::ctor
==============================================================================
*/

DocumentParser::Object::Object ()
:  _dp_ptr (0)
,  _ds_ptr (0)
,  _pos (size_t (-1))
,  _cur_type (Type_VOID)
,  _cur_sub_type (Type_VOID)
,  _expected_type (Type_VOID)
{
}



/*
==============================================================================
Name : Object::dtor
==============================================================================
*/

DocumentParser::Object::~Object ()
{
}



/*
==============================================================================
Name : Object::get_document_parser
==============================================================================
*/

DocumentParser &  DocumentParser::Object::get_document_parser ()
{
   assert (_dp_ptr != 0);
   
   return *_dp_ptr;
}



/*
==============================================================================
Name : Object::read_class_name
==============================================================================
*/

int DocumentParser::Object::read_class_name (std::string & class_name)
{
   assert (_ds_ptr != 0);
   
   int err = _ds_ptr->seek_beg (_pos);
   if (err != Err_OK) return err;

   // blobs are handled differently
   err = _ds_ptr->require_keyword (DocumentKeywords::_class_name_0);
   if (err != Err_OK) return err;
   
   err = _ds_ptr->find_first_not_of_ws ();
   if (err != Err_OK) return err;

   err = _ds_ptr->read (class_name);
   if (err != Err_OK) return err;
   
   err = _ds_ptr->find_first_not_of_ws ();
   if (err != Err_OK) return err;
   
   // point to either keyword /AttributeName or >>
   
   _pos = _ds_ptr->tell ();
   
   return Err_OK;
}



/*
==============================================================================
Name : Object::read_attribute_name
==============================================================================
*/

int DocumentParser::Object::read_attribute_name (std::string & attribute_name)
{
   assert (_ds_ptr != 0);
   
   int err = _ds_ptr->seek_beg (_pos);
   if (err != Err_OK) return err;

   std::string keyword;
   err = _ds_ptr->read (keyword);
   if (err != Err_OK) return err;
   
   if (keyword == DocumentKeywords::_attribute_name_0)
   {
      err = _ds_ptr->find_first_not_of_ws ();
      if (err != Err_OK) return err;
      
      err = _ds_ptr->read (attribute_name);
      if (err != Err_OK) return err;
      
      err = _ds_ptr->find_first_not_of_ws ();
      if (err != Err_OK) return err;
   }
   else if (keyword == DocumentKeywords::_end_dict_0)
   {
      // don't update pos
      
      return Err_DOC_NO_MORE_ATTRIBUTES;
   }
   else
   {
      dr_break;
      return Err_DOC_UNEXPECTED_KEYWORD;
   }

   // point to keyword /Type

   _pos = _ds_ptr->tell ();
   
   return Err_OK;
}



/*
==============================================================================
Name : Object::read_type
==============================================================================
*/

int DocumentParser::Object::read_type (Type & type)
{
   assert (_ds_ptr != 0);
   
   int err = _ds_ptr->seek_beg (_pos);
   if (err != Err_OK) return err;

   err = _ds_ptr->require_keyword (DocumentKeywords::_type_0);
   if (err != Err_OK) return err;
   
   err = _ds_ptr->find_first_not_of_ws ();
   if (err != Err_OK) return err;
   
   const char * keyword_ptr = 0;
   size_t len;
   err = _ds_ptr->read (keyword_ptr, len);
   if (err != Err_OK) return err;
   
   err = _ds_ptr->find_first_not_of_ws ();
   if (err != Err_OK) return err;
   
   using namespace std;
   
   switch (len)
   {
   case 4:
      if (memcmp (keyword_ptr, DocumentKeywords::_cue_0, len) == 0)
      {
         _cur_type = type = Type_CUE;
         _expected_type = Type_VOID;
      }
      else
      {
         dr_break;
         return Err_DOC_UNEXPECTED_KEYWORD;
      }
      break;

   case 5:
      if (memcmp (keyword_ptr, DocumentKeywords::_bool_0, len) == 0)
      {
         _cur_type = type = Type_BOOL;
         _expected_type = type;
      }
      else if (memcmp (keyword_ptr, DocumentKeywords::_enum_0, len) == 0)
      {
         _cur_type = type = Type_ENUM;
         _expected_type = type;
      }
      else if (memcmp (keyword_ptr, DocumentKeywords::_blob_0, len) == 0)
      {
         _cur_type = type = Type_BLOB;
         _expected_type = Type_VOID;
      }
      else
      {
         dr_break;
         return Err_DOC_UNEXPECTED_KEYWORD;
      }
      break;

   case 6:
      if (memcmp (keyword_ptr, DocumentKeywords::_array_0, len) == 0)
      {
         _cur_type = type = Type_ARRAY;
      }
      else if (memcmp (keyword_ptr, DocumentKeywords::_int64_0, len) == 0)
      {
         _cur_type = type = Type_INT64;
         _expected_type = type;
      }
      else
      {
         dr_break;
         return Err_DOC_UNEXPECTED_KEYWORD;
      }
      break;

   case 7:
      if (memcmp (keyword_ptr, DocumentKeywords::_object_0, len) == 0)
      {
         _cur_type = type = Type_OBJECT;
         _expected_type = Type_REF;
      }
      else
      {
         dr_break;
         return Err_DOC_UNEXPECTED_KEYWORD;
      }
      break;

   case 8:
      if (memcmp (keyword_ptr, DocumentKeywords::_float64_0, len) == 0)
      {
         _cur_type = type = Type_FLOAT64;
         _expected_type = type;
      }
      else
      {
         dr_break;
         return Err_DOC_UNEXPECTED_KEYWORD;
      }
      break;

   case 10:
      if (memcmp (keyword_ptr, DocumentKeywords::_object_ref_0, len) == 0)
      {
         _cur_type = type = Type_OBJECT_REF;
         _expected_type = Type_REF;
      }
      else
      {
         dr_break;
         return Err_DOC_UNEXPECTED_KEYWORD;
      }
      break;

   case 11:
      if (memcmp (keyword_ptr, DocumentKeywords::_collection_0, len) == 0)
      {
         _cur_type = type = Type_COLLECTION;
      }
      else
      {
         dr_break;
         return Err_DOC_UNEXPECTED_KEYWORD;
      }
      break;
   
   default:
      dr_break;
      return Err_DOC_UNEXPECTED_KEYWORD;
      break;
   }
   
   // point to keyword /Value or sub type

   _pos = _ds_ptr->tell ();
   
   return Err_OK;
}



/*
==============================================================================
Name : Object::read_sub_type
==============================================================================
*/

int DocumentParser::Object::read_sub_type (Type & type)
{
   assert (_ds_ptr != 0);
   
   int err = _ds_ptr->seek_beg (_pos);
   if (err != Err_OK) return err;
   
   bool cur_type_ok_flag
      = (_cur_type == Type_ARRAY)
      || (_cur_type == Type_COLLECTION)
   ;
   
   if (!cur_type_ok_flag)
   {
      dr_break;
      return Err_DOC_UNEXPECTED_KEYWORD;
   }
   
   std::string keyword;
   err = _ds_ptr->read (keyword);
   if (err != Err_OK) return err;
   
   err = _ds_ptr->find_first_not_of_ws ();
   if (err != Err_OK) return err;
   
   if (keyword == DocumentKeywords::_ref_0)
   {
      _cur_sub_type = type = Type_OBJECT;
   }
   else if (keyword == DocumentKeywords::_bool_0)
   {
      _cur_sub_type = type = Type_BOOL;
   }
   else if (keyword == DocumentKeywords::_int64_0)
   {
      _cur_sub_type = type = Type_INT64;
   }
   else if (keyword == DocumentKeywords::_enum_0)
   {
      _cur_sub_type = type = Type_ENUM;
   }
   else if (keyword == DocumentKeywords::_float64_0)
   {
      _cur_sub_type = type = Type_FLOAT64;
   }
   else
   {
      dr_break;
      return Err_DOC_UNEXPECTED_KEYWORD;
   }
   
   // point to either [ or (
   
   if (_cur_type == Type_ARRAY)
   {
      err = _ds_ptr->require_keyword (DocumentKeywords::_begin_array_0);
      if (err != Err_OK) return err;
      
      err = _ds_ptr->find_first_not_of_ws ();
      if (err != Err_OK) return err;
      
      _expected_type = Type_BINARY_KEY;
   }
   else if (_cur_type == Type_COLLECTION)
   {
      err = _ds_ptr->require_keyword (DocumentKeywords::_begin_collection_0);
      if (err != Err_OK) return err;
      
      err = _ds_ptr->find_first_not_of_ws ();
      if (err != Err_OK) return err;

      _expected_type = Type_REF;
   }
   
   // point to either reference or first key of the array/collection

   _pos = _ds_ptr->tell ();
   
   return Err_OK;
}



/*
==============================================================================
Name : Object::read_sub_enum_name
==============================================================================
*/

int DocumentParser::Object::read_sub_enum_name (std::string & enum_name)
{
   assert (_ds_ptr != 0);
   
   int err = _ds_ptr->seek_beg (_pos);
   if (err != Err_OK) return err;
   
   bool cur_type_ok_flag = (_cur_type == Type_ENUM);
   
   if (!cur_type_ok_flag)
   {
      dr_break;
      return Err_DOC_UNEXPECTED_KEYWORD;
   }
   
   err = _ds_ptr->read (enum_name);
   if (err != Err_OK) return err;
   
   err = _ds_ptr->find_first_not_of_ws ();
   if (err != Err_OK) return err;
   
   // point to value

   _pos = _ds_ptr->tell ();
   
   return Err_OK;
}



/*
==============================================================================
Name : Object::read_binary_key
==============================================================================
*/

int DocumentParser::Object::read_binary_key (BinaryKey & key)
{
   if (_cur_type != Type_ARRAY)
   {
      dr_break;
      return Err_DOC_INTERNAL_SYSTEM_ERROR;
   }
   
   if (_expected_type != Type_BINARY_KEY)
   {
      dr_break;
      return Err_DOC_INTERNAL_SYSTEM_ERROR;
   }
   
   assert (_ds_ptr != 0);
   
   int err = _ds_ptr->seek_beg (_pos);
   if (err != Err_OK) return err;
   
   std::string keyword;
   err = _ds_ptr->read (keyword);
   if (err != Err_OK) return err;
   
   if (keyword == DocumentKeywords::_end_array_0)
   {
      err = _ds_ptr->find_first_not_of_ws ();
      if (err != Err_OK) return err;
      
      // points to next attribute if any

      _pos = _ds_ptr->tell ();
      _expected_type = Type_VOID;
      
      return Err_DOC_ITERATION_NO_MORE_ITEMS;
   }
   else
   {
      // reseek to begin of keyword
      _ds_ptr->seek_beg (_pos);
      
      err = _ds_ptr->read (key);
      if (err != Err_OK) return err;

      err = _ds_ptr->find_first_not_of_ws ();
      if (err != Err_OK) return err;
      
      // points to a reference
      
      _pos = _ds_ptr->tell ();
      _expected_type = Type_REF;
   
      return Err_OK;
   }

   return Err_OK;
}



/*
==============================================================================
Name : Object::read_ref
==============================================================================
*/

int DocumentParser::Object::read_ref (flip::Ref & ref)
{
   assert (_ds_ptr != 0);
   
   int err = _ds_ptr->seek_beg (_pos);
   if (err != Err_OK) return err;
   
   if (_expected_type != Type_REF)
   {
      dr_break;
      return Err_DOC_INTERNAL_SYSTEM_ERROR;
   }
   
   bool prefix_flag = false;
   
   switch (_cur_type)
   {
   case Type_OBJECT:
   case Type_BLOB:
   case Type_OBJECT_REF:
      prefix_flag = true;
      break;

   case Type_ARRAY:
   case Type_COLLECTION:
      prefix_flag = false;
      break;

   default:
      dr_break;
      return Err_DOC_INTERNAL_SYSTEM_ERROR;
      break;
   }

   if (prefix_flag)
   {
      if (_cur_type == Type_BLOB)
      {
         std::string keyword;
         err = _ds_ptr->read (keyword);
         if (err != Err_OK) return err;
         
         if (keyword == DocumentKeywords::_empty_0)
         {
            err = _ds_ptr->find_first_not_of_ws ();
            if (err != Err_OK) return err;
            
            // points to next attribute if any

            _pos = _ds_ptr->tell ();
            _expected_type = Type_VOID;
         
            return Err_DOC_BLOB_EMPTY;
         }
         else if (keyword == DocumentKeywords::_ref_0)
         {
            err = _ds_ptr->find_first_not_of_ws ();
            if (err != Err_OK) return err;
         }
         else
         {
            return Err_DOC_UNEXPECTED_KEYWORD;
         }
      }
      else
      {
         err = _ds_ptr->require_keyword (DocumentKeywords::_ref_0);
         if (err != Err_OK) return err;
         
         err = _ds_ptr->find_first_not_of_ws ();
         if (err != Err_OK) return err;
      }
   }
   
   if (_cur_type == Type_COLLECTION)
   {
      std::string keyword;
      err = _ds_ptr->read (keyword);
      if (err != Err_OK) return err;
      
      if (keyword == DocumentKeywords::_end_collection_0)
      {
         err = _ds_ptr->find_first_not_of_ws ();
         if (err != Err_OK) return err;
         
         // points to next attribute if any

         _pos = _ds_ptr->tell ();
         _expected_type = Type_VOID;
      
         return Err_DOC_ITERATION_NO_MORE_ITEMS;
      }
      else
      {
         // reseek to begin of keyword
         _ds_ptr->seek_beg (_pos);
         
         err = _ds_ptr->read (ref);
         if (err != Err_OK) return err;
      }
   }
   else
   {
      err = _ds_ptr->read (ref);
      if (err != Err_OK) return err;
   }

   err = _ds_ptr->find_first_not_of_ws ();
   if (err != Err_OK) return err;
   
   // points to a value, the next reference, or end
   
   _pos = _ds_ptr->tell ();

   switch (_cur_type)
   {
   case Type_OBJECT:
   case Type_BLOB:
   case Type_OBJECT_REF:
      _expected_type = Type_VOID;
      break;

   case Type_ARRAY:
      if (_cur_sub_type == Type_OBJECT)
      {
         _expected_type = Type_BINARY_KEY;
      }
      else
      {
         _expected_type = _cur_sub_type;
      }
      break;

   case Type_COLLECTION:
      if (_cur_sub_type == Type_OBJECT)
      {
         _expected_type = Type_REF;
      }
      else
      {
         _expected_type = _cur_sub_type;
      }
      break;

   default:
      assert (false);
      break;
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : Object::read_blob
==============================================================================
*/

int DocumentParser::Object::read_blob (bool & inline_flag, bool & empty_flag, flip::Ref & ref, std::vector <Codec::Encoder> & codec_arr, archi::UByte * & data_ptr, size_t & data_length)
{
   assert (_ds_ptr != 0);
   assert (_dp_ptr != 0);
   
   inline_flag = false;
   empty_flag = false;
   ref = 0;
   codec_arr.clear ();
   data_ptr = 0;
   data_length = 0;
   
   int err = _ds_ptr->seek_beg (_pos);
   if (err != Err_OK) return err;
   
   if (_expected_type != Type_VOID)
   {
      dr_break;
      return Err_DOC_INTERNAL_SYSTEM_ERROR;
   }
   
   if (_cur_type != Type_BLOB)
   {
      dr_break;
      return Err_DOC_INTERNAL_SYSTEM_ERROR;
   }
   
   std::string prop_str;
   err = _ds_ptr->read (prop_str);
   if (err != Err_OK) return err;

   err = _ds_ptr->find_first_not_of_ws ();
   if (err != Err_OK) return err;
   
   inline_flag = prop_str == DocumentKeywords::_inline_0;
   
   size_t pos = _ds_ptr->tell ();
   
   if (inline_flag)
   {
      err = _ds_ptr->read (prop_str);
      if (err != Err_OK) return err;

      err = _ds_ptr->find_first_not_of_ws ();
      if (err != Err_OK) return err;
   }
   
   if (prop_str == DocumentKeywords::_empty_0)
   {
      empty_flag = true;
   }
   else if (prop_str == DocumentKeywords::_ref_0)
   {
      if (inline_flag)
      {
         return Err_DOC_UNEXPECTED_KEYWORD;
      }
      
      err = _ds_ptr->read (ref);
      if (err != Err_OK) return err;
   }
   else if (prop_str == DocumentKeywords::_length_0)
   {
      if (!inline_flag)
      {
         return Err_DOC_UNEXPECTED_KEYWORD;
      }

      _ds_ptr->seek_beg (pos);
      
      _dp_ptr->read_blob_properties (data_length, codec_arr, true);
      
      err = _ds_ptr->read (data_ptr, data_length);
      if (err != Err_OK) return err;
   }
   else
   {
      return Err_DOC_UNEXPECTED_KEYWORD;
   }
   
   err = _ds_ptr->find_first_not_of_ws ();
   if (err != Err_OK) return err;

   _pos = _ds_ptr->tell ();
   
   return Err_OK;
}



/*
==============================================================================
Name : Object::read_bool
==============================================================================
*/

int DocumentParser::Object::read_bool (bool & val)
{
   assert (_ds_ptr != 0);
   
   int err = _ds_ptr->seek_beg (_pos);
   if (err != Err_OK) return err;
   
   if (_expected_type != Type_BOOL)
   {
      dr_break;
      return Err_DOC_INTERNAL_SYSTEM_ERROR;
   }
   
   bool prefix_flag = false;
   
   switch (_cur_type)
   {
   case Type_BOOL:
      prefix_flag = true;
      break;

   case Type_ARRAY:
   case Type_COLLECTION:
      prefix_flag = false;
      break;

   default:
      dr_break;
      return Err_DOC_INTERNAL_SYSTEM_ERROR;
      break;
   }

   if (prefix_flag)
   {
      err = _ds_ptr->require_keyword (DocumentKeywords::_value_0);
      if (err != Err_OK) return err;
      
      err = _ds_ptr->find_first_not_of_ws ();
      if (err != Err_OK) return err;
   }
   
   std::string val_str;
   err = _ds_ptr->read (val_str);
   if (err != Err_OK) return err;
   
   if (val_str == DocumentKeywords::_true_0)
   {
      val = true;
   }
   else if (val_str == DocumentKeywords::_false_0)
   {
      val = false;
   }
   else
   {
      return Err_DOC_UNEXPECTED_KEYWORD;
   }
   
   err = _ds_ptr->find_first_not_of_ws ();
   if (err != Err_OK) return err;
   
   // points to a key or end
   
   _pos = _ds_ptr->tell ();

   switch (_cur_type)
   {
   case Type_BOOL:
      _expected_type = Type_VOID;
      break;

   case Type_ARRAY:
      _expected_type = Type_BINARY_KEY;
      break;

   case Type_COLLECTION:
      _expected_type = Type_REF;
      break;

   default:
      assert (false);
      break;
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : Object::read_int64
==============================================================================
*/

int DocumentParser::Object::read_int64 (archi::Int64 & val)
{
   assert (_ds_ptr != 0);
   
   int err = _ds_ptr->seek_beg (_pos);
   if (err != Err_OK) return err;
   
   if (_expected_type != Type_INT64)
   {
      dr_break;
      return Err_DOC_INTERNAL_SYSTEM_ERROR;
   }
   
   bool prefix_flag = false;
   
   switch (_cur_type)
   {
   case Type_INT64:
      prefix_flag = true;
      break;

   case Type_ARRAY:
   case Type_COLLECTION:
      prefix_flag = false;
      break;

   default:
      dr_break;
      return Err_DOC_INTERNAL_SYSTEM_ERROR;
      break;
   }

   if (prefix_flag)
   {
      err = _ds_ptr->require_keyword (DocumentKeywords::_value_0);
      if (err != Err_OK) return err;
      
      err = _ds_ptr->find_first_not_of_ws ();
      if (err != Err_OK) return err;
   }
   
   err = _ds_ptr->read (val);
   if (err != Err_OK) return err;
   
   err = _ds_ptr->find_first_not_of_ws ();
   if (err != Err_OK) return err;
   
   // points to a key or end
   
   _pos = _ds_ptr->tell ();

   switch (_cur_type)
   {
   case Type_INT64:
      _expected_type = Type_VOID;
      break;

   case Type_ARRAY:
      _expected_type = Type_BINARY_KEY;
      break;

   case Type_COLLECTION:
      _expected_type = Type_REF;
      break;

   default:
      assert (false);
      break;
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : Object::read_enum
==============================================================================
*/

int DocumentParser::Object::read_enum (archi::Int64 & val, EnumDescriptionMaster & edm)
{
   assert (_ds_ptr != 0);
   assert (&edm != 0);
   
   int err = _ds_ptr->seek_beg (_pos);
   if (err != Err_OK) return err;
   
   if (_expected_type != Type_ENUM)
   {
      dr_break;
      return Err_DOC_INTERNAL_SYSTEM_ERROR;
   }
   
   if (_cur_type != Type_ENUM)
   {
      dr_break;
      return Err_DOC_INTERNAL_SYSTEM_ERROR;
   }
   
   err = _ds_ptr->require_keyword (DocumentKeywords::_value_0);
   if (err != Err_OK) return err;
   
   err = _ds_ptr->find_first_not_of_ws ();
   if (err != Err_OK) return err;
   
   std::string val_str;
   err = _ds_ptr->read (val_str);
   if (err != Err_OK) return err;
   
   err = _ds_ptr->find_first_not_of_ws ();
   if (err != Err_OK) return err;
   
   err = edm.get_value_for_name (val, val_str);
   if (err != Err_OK) return err;
   
   // points to a key or end
   
   _pos = _ds_ptr->tell ();
   
   _expected_type = Type_VOID;
   
   return Err_OK;
}



/*
==============================================================================
Name : Object::read_enum
==============================================================================
*/

int DocumentParser::Object::read_enum (std::string & val)
{
   assert (_ds_ptr != 0);
   
   int err = _ds_ptr->seek_beg (_pos);
   if (err != Err_OK) return err;
   
   if (_expected_type != Type_ENUM)
   {
      dr_break;
      return Err_DOC_INTERNAL_SYSTEM_ERROR;
   }
   
   if (_cur_type != Type_ENUM)
   {
      dr_break;
      return Err_DOC_INTERNAL_SYSTEM_ERROR;
   }
   
   err = _ds_ptr->require_keyword (DocumentKeywords::_value_0);
   if (err != Err_OK) return err;
   
   err = _ds_ptr->find_first_not_of_ws ();
   if (err != Err_OK) return err;
   
   err = _ds_ptr->read (val);
   if (err != Err_OK) return err;
   
   err = _ds_ptr->find_first_not_of_ws ();
   if (err != Err_OK) return err;
   
   // points to a key or end
   
   _pos = _ds_ptr->tell ();
   
   _expected_type = Type_VOID;
   
   return Err_OK;
}



/*
==============================================================================
Name : Object::read_float64
==============================================================================
*/

int DocumentParser::Object::read_float64 (archi::Float64 & val)
{
   assert (_ds_ptr != 0);
   
   int err = _ds_ptr->seek_beg (_pos);
   if (err != Err_OK) return err;
   
   if (_expected_type != Type_FLOAT64)
   {
      dr_break;
      return Err_DOC_INTERNAL_SYSTEM_ERROR;
   }
   
   bool prefix_flag = false;
   
   switch (_cur_type)
   {
   case Type_FLOAT64:
      prefix_flag = true;
      break;

   case Type_ARRAY:
   case Type_COLLECTION:
      prefix_flag = false;
      break;

   default:
      dr_break;
      return Err_DOC_INTERNAL_SYSTEM_ERROR;
      break;
   }

   if (prefix_flag)
   {
      err = _ds_ptr->require_keyword (DocumentKeywords::_value_0);
      if (err != Err_OK) return err;
      
      err = _ds_ptr->find_first_not_of_ws ();
      if (err != Err_OK) return err;
   }
   
   err = _ds_ptr->read (val);
   if (err != Err_OK) return err;
   
   err = _ds_ptr->find_first_not_of_ws ();
   if (err != Err_OK) return err;
   
   // points to a key or end
   
   _pos = _ds_ptr->tell ();

   switch (_cur_type)
   {
   case Type_FLOAT64:
      _expected_type = Type_VOID;
      break;

   case Type_ARRAY:
      _expected_type = Type_BINARY_KEY;
      break;

   case Type_COLLECTION:
      _expected_type = Type_REF;
      break;

   default:
      assert (false);
      break;
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : read_blob_properties
Description :
   Read a blob (decode)
==============================================================================
*/

int   DocumentParser::read_blob_properties (size_t & length, std::vector <Codec::Encoder> & codec_arr, bool space_term_flag)
{
   int err = _ds.require_keyword (DocumentKeywords::_length_0);
   if (err != Err_OK) return err;

   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;

   err = _ds.read (length);
   if (err != Err_OK) return err;
   
   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;
   
   err = _ds.require_keyword (DocumentKeywords::_filter_0);
   if (err != Err_OK) return err;

   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;

   err = _ds.require_keyword (DocumentKeywords::_begin_array_0);
   if (err != Err_OK) return err;

   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;
   
   // codec arr is to be used in the reverse way
   // (whatever the decoding order) i.e. the
   // encoding is made from right to left
      
   bool loop_flag = true;
   
   while (loop_flag)
   {
      std::string keyword;
      err = _ds.read (keyword);
      if (err != Err_OK) return err;
      
      if (keyword == DocumentKeywords::_end_array_0)
      {
         loop_flag = false;
      }
      else
      {
         if (keyword == DocumentKeywords::_base64_decode_0)
         {
            codec_arr.insert (codec_arr.begin (), Codec::Encoder_BASE64);
         }
         else if (keyword == DocumentKeywords::_flate_decode_0)
         {
            codec_arr.insert (codec_arr.begin (), Codec::Encoder_DEFLATE);
         }
         else if (keyword == DocumentKeywords::_escaped_ascii_decode_0)
         {
            codec_arr.insert (codec_arr.begin (), Codec::Encoder_ESCAPED_ASCII);
         }
         else if (keyword == DocumentKeywords::_hexadecimal_decode_0)
         {
            codec_arr.insert (codec_arr.begin (), Codec::Encoder_HEXADECIMAL);
         }
         else
         {
            dr_break;
            return Err_CODEC_UNKNOWN;
         }
      }
      
      if (space_term_flag && !loop_flag)
      {
         err = _ds.require_char (DocumentKeywords::_space_0);
         if (err != Err_OK) return err;
      }
      else
      {
         err = _ds.find_first_not_of_ws ();
         if (err != Err_OK) return err;
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : read_blob
Description :
   Read a blob (decode)
==============================================================================
*/

int   DocumentParser::read_blob (std::vector <archi::UByte> & data, flip::Ref ref)
{
   size_t old_pos = _ds.tell ();
   
   RefPosMap::const_iterator it = _ref_pos_map.find (ref);
   
   if (it == _ref_pos_map.end ())
   {
      dr_break;
      return Err_DOC_OBJECT_MISSING;
   }
   
   size_t ref_pos = it->second;

   int err = _ds.seek_beg (ref_pos);
   if (err != Err_OK) return err;

   err = _ds.read (ref);
   if (err != Err_OK) return err;
   
   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;

   err = _ds.require_keyword (DocumentKeywords::_obj_0);
   if (err != Err_OK) return err;

   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;

   err = _ds.require_keyword (DocumentKeywords::_begin_dict_0);
   if (err != Err_OK) return err;

   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;

   err = _ds.require_keyword (DocumentKeywords::_blob_0);
   if (err != Err_OK) return err;

   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;

   size_t length = 0;

   // codec arr is to be used in the reverse way
   // (whatever the decoding order) i.e. the
   // encoding is made from right to left
   std::vector <Codec::Encoder> codec_arr;

   err = read_blob_properties (length, codec_arr);
   if (err != Err_OK) return err;
   
   err = _ds.require_keyword (DocumentKeywords::_end_dict_0);
   if (err != Err_OK) return err;

   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;

   err = _ds.require_keyword (DocumentKeywords::_begin_stream_0);
   if (err != Err_OK) return err;

   err = _ds.require_char (DocumentKeywords::_lf_0);
   if (err != Err_OK) return err;

   archi::UByte * data_ptr = 0;
   err = _ds.read (data_ptr, length);
   if (err != Err_OK) return err;
   
   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;

   err = _ds.require_keyword (DocumentKeywords::_end_stream_0);
   if (err != Err_OK) return err;

   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;

   err = _ds.require_keyword (DocumentKeywords::_endobj_0);
   if (err != Err_OK) return err;
   
   if (length > 0)
   {
      if (codec_arr.size () > 0)
      {
         Codec::decode (data, data_ptr, length, codec_arr);
      }
      else
      {
         using namespace std;
         
         data.resize (length);
         
         memcpy (&data [0], data_ptr, length);
      }
   }
   else
   {
      assert (length == 0);
      
      // nothing
   }
   
   err = _ds.seek_beg (old_pos);
   if (err != Err_OK) return err;

   return Err_OK;
}



/*
==============================================================================
Name : read_blob
Description :
   Read a blob (do not decode)
==============================================================================
*/

int   DocumentParser::read_blob (std::vector <archi::UByte> & data, std::vector <Codec::Encoder> & codec_arr, flip::Ref ref)
{
   size_t old_pos = _ds.tell ();
   
   RefPosMap::const_iterator it = _ref_pos_map.find (ref);
   
   if (it == _ref_pos_map.end ())
   {
      dr_break;
      return Err_DOC_OBJECT_MISSING;
   }
   
   size_t ref_pos = it->second;

   int err = _ds.seek_beg (ref_pos);
   if (err != Err_OK) return err;

   err = _ds.read (ref);
   if (err != Err_OK) return err;
   
   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;

   err = _ds.require_keyword (DocumentKeywords::_obj_0);
   if (err != Err_OK) return err;

   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;

   err = _ds.require_keyword (DocumentKeywords::_begin_dict_0);
   if (err != Err_OK) return err;

   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;

   err = _ds.require_keyword (DocumentKeywords::_blob_0);
   if (err != Err_OK) return err;

   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;

   size_t length = 0;

   err = read_blob_properties (length, codec_arr);
   if (err != Err_OK) return err;
   
   err = _ds.require_keyword (DocumentKeywords::_end_dict_0);
   if (err != Err_OK) return err;

   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;

   err = _ds.require_keyword (DocumentKeywords::_begin_stream_0);
   if (err != Err_OK) return err;

   err = _ds.require_char (DocumentKeywords::_lf_0);
   if (err != Err_OK) return err;

   archi::UByte * data_ptr = 0;
   err = _ds.read (data_ptr, length);
   if (err != Err_OK) return err;
   
   data.assign (data_ptr, data_ptr + length);
   
   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;

   err = _ds.require_keyword (DocumentKeywords::_end_stream_0);
   if (err != Err_OK) return err;

   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;

   err = _ds.require_keyword (DocumentKeywords::_endobj_0);
   if (err != Err_OK) return err;
   
   err = _ds.seek_beg (old_pos);
   if (err != Err_OK) return err;

   return Err_OK;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : read_file_end
==============================================================================
*/

int   DocumentParser::read_file_end ()
{
   int err = _ds.seek_end (0);
   if (err != Err_OK) return err;

   err = _ds.rseek (DocumentKeywords::_eof_0);
   if (err != Err_OK) return err;
   
   if (_ds.tell () + std::string (DocumentKeywords::_eof_0).size () != _ds.size ())
   {
      dr_break;
      return Err_DOC_BAD_EOF;
   }
   
   err = _ds.rseek (DocumentKeywords::_startxref_0);
   if (err != Err_OK) return err;
   
   err = _ds.advance (DocumentKeywords::_startxref_0);
   if (err != Err_OK) return err;
   
   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;
   
   size_t startxref_pos = 0;
   err = _ds.read (startxref_pos);
   if (err != Err_OK) return err;
   
   err = _ds.seek_beg (startxref_pos);
   if (err != Err_OK) return err;
   
   err = _ds.require_keyword (DocumentKeywords::_xref_0);
   if (err != Err_OK) return err;
   
   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;
   
   size_t nbr_ref = 0;
   err = _ds.read (nbr_ref);
   if (err != Err_OK) return err;
   
   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;
   
   for (size_t i = 0 ; i < nbr_ref ; ++i)
   {
      size_t ref_pos = 0;
      err = _ds.read (ref_pos);
      if (err != Err_OK) return err;
      
      size_t pos = _ds.tell ();
      _ds.seek_beg (ref_pos);
      
      flip::Ref ref = 0;
      err = _ds.read (ref);
      if (err != Err_OK) return err;
      
      if (ref == 0)
      {
         dr_break;
         return Err_DOC_BAD_REFERENCE;
      }
      
      err = _ds.seek_beg (pos);
      if (err != Err_OK) return err;
      
      err = _ds.find_first_not_of_ws ();
      if (err != Err_OK) return err;
      
      _ref_pos_map [ref] = ref_pos;
   }
   
   err = _ds.require_keyword (DocumentKeywords::_trailer_0);
   if (err != Err_OK) return err;

   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;
   
   err = _ds.require_keyword (DocumentKeywords::_begin_dict_0);
   if (err != Err_OK) return err;

   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;

   err = _ds.require_keyword (DocumentKeywords::_root_0);
   if (err != Err_OK) return err;

   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;
   
   err = _ds.read (_root_ref);
   if (err != Err_OK) return err;
   
   if (_root_ref == 0)
   {
      dr_break;
      return Err_DOC_BAD_REFERENCE;
   }
   
   if (_ref_pos_map.find (_root_ref) == _ref_pos_map.end ())
   {
      dr_break;
      return Err_DOC_OBJECT_MISSING;
   }
   
   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;
   
   std::string keyword;
   err = _ds.read (keyword);
   if (err != Err_OK) return err;
   
   if (keyword == DocumentKeywords::_revision_0)
   {
      err = _ds.find_first_not_of_ws ();
      if (err != Err_OK) return err;

      err = _ds.read (_rev_id);
      if (err != Err_OK) return err;

      if (_rev_id <= 0)
      {
         dr_break;
         return Err_DOC_BAD_REVISION;
      }
   
      err = _ds.find_first_not_of_ws ();
      if (err != Err_OK) return err;

      err = _ds.read (keyword);
      if (err != Err_OK) return err;
   }
   
   if (keyword == DocumentKeywords::_version_0)
   {
      err = _ds.find_first_not_of_ws ();
      if (err != Err_OK) return err;

      err = _ds.read (_format_version);
      if (err != Err_OK) return err;

      err = _ds.find_first_not_of_ws ();
      if (err != Err_OK) return err;

      err = _ds.read (keyword);
      if (err != Err_OK) return err;
   }

   while (keyword == DocumentKeywords::_user_0)
   {
      err = _ds.find_first_not_of_ws ();
      if (err != Err_OK) return err;
      
      std::string user_name;
      err = _ds.read (user_name);
      if (err != Err_OK) return err;

      err = _ds.find_first_not_of_ws ();
      if (err != Err_OK) return err;
      
      err = _ds.require_keyword (DocumentKeywords::_id_0);
      if (err != Err_OK) return err;

      err = _ds.find_first_not_of_ws ();
      if (err != Err_OK) return err;
      
      archi::Int64 user_id = 0;
      err = _ds.read (user_id);
      if (err != Err_OK) return err;
      
      bool ok_flag
         = (user_id >= SeedGenerator::EDITOR_USER_ID_START_NBR)
         && (user_id <= SeedGenerator::EDITOR_USER_ID_MAX);
      
      if (!ok_flag)
      {
         return Err_DOC_BAD_USER_ID;
      }
      
      assert (user_id >= 0);
      _user_name_id_map [user_name] = archi::Int32 (user_id);
      
      err = _ds.find_first_not_of_ws ();
      if (err != Err_OK) return err;
      
      err = _ds.read (keyword);
      if (err != Err_OK) return err;
   }
   
   if (keyword != DocumentKeywords::_end_dict_0)
   {
      dr_break;
      return Err_DOC_UNEXPECTED_KEYWORD;
   }
   
   err = _ds.find_first_not_of_ws ();
   if (err != Err_OK) return err;
   
   err = _ds.require_keyword (DocumentKeywords::_startxref_0);
   if (err != Err_OK) return err;

   return Err_OK;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



#undef dr_break



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
