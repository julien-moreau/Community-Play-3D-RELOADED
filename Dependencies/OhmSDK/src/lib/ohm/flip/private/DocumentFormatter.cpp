/*****************************************************************************

        DocumentFormatter.cpp
        Copyright (c) 2010 Ohm Force

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

#include "ohm/flip/private/DocumentFormatter.h"
#include "ohm/flip/private/BinaryKey.h"
#include "ohm/flip/private/DataStreamOutput.h"
#include "ohm/flip/private/DocumentKeywords.h"

#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

DocumentFormatter::~DocumentFormatter ()
{
}



/*
==============================================================================
Name : write_header
==============================================================================
*/

void  DocumentFormatter::write_header (DataStreamOutput & dst)
{
   dst.print ("%1%").arg (DocumentKeywords::_header_0);
   dst.cat ("%1%").arg ("\n\n");
}



/*
==============================================================================
Name : write_header
==============================================================================
*/

void  DocumentFormatter::write_xref (DataStreamOutput & dst, size_t & startxref, const std::set <size_t> & obj_offset_set)
{
   startxref = dst.tell ();
   
   dst.cat ("%1%\n").arg (DocumentKeywords::_xref_0);
   dst.cat ("%1%\n").arg (obj_offset_set.size ());
   
   {
      std::set <size_t>::const_iterator it = obj_offset_set.begin ();
      std::set <size_t>::const_iterator it_end = obj_offset_set.end ();
      
      for (; it != it_end ; ++it)
      {
         size_t pos = *it;
         
         dst.cat ("%1m10rw0%\n").arg (pos);
      }
   }
}



/*
==============================================================================
Name : write_trailer
==============================================================================
*/

void  DocumentFormatter::write_trailer (DataStreamOutput & dst, flip::Ref root_ref, archi::Int64 rev_id, const std::string & format_version, const UserNameIdMap & user_name_id_map)
{
   // root
   {
      dst.cat ("%1% ").arg (DocumentKeywords::_trailer_0);
      dst.cat ("%1%\n\t").arg (DocumentKeywords::_begin_dict_0);
      dst.cat ("%1% ").arg (DocumentKeywords::_root_0);
      dst.cat ("%1%\n").arg (root_ref);
   }
   
   // revision
   {
      if (rev_id != 0)
      {
         assert (rev_id > 0);
         
         dst.cat ("\t%1% ").arg (DocumentKeywords::_revision_0);
         dst.cat ("%1%\n").arg (rev_id);
      }
   }
   
   // version
   {
      assert (format_version != "");
      
      Codec::Data version_data;
      
      Codec::encode_escaped_ascii (
         version_data, (const archi::UByte *) format_version.c_str (), format_version.size ()
      );
      
      std::string encoded_version_str;
      encoded_version_str.assign (&version_data [0], &version_data [0] + version_data.size ());
      
      dst.cat ("\t%1% ").arg (DocumentKeywords::_version_0);
      dst.cat ("%1%\n").arg (encoded_version_str.c_str ());
   }
   
   // users
   {
      UserNameIdMap::const_iterator it = user_name_id_map.begin ();
      const UserNameIdMap::const_iterator it_end = user_name_id_map.end ();
      
      for (; it != it_end ; ++it)
      {
         const std::string & user_name = it->first;
         archi::Int32 user_id = it->second;
         
         dst.cat ("\t%1% ").arg (DocumentKeywords::_user_0);
         dst.cat ("%1% ").arg (user_name.c_str ());
         dst.cat ("%1% ").arg (DocumentKeywords::_id_0);
         dst.cat ("%1%\n").arg (user_id);
      }
   }
   
   dst.cat ("%1%\n").arg (DocumentKeywords::_end_dict_0);
}



/*
==============================================================================
Name : write_start_xref
==============================================================================
*/

void  DocumentFormatter::write_start_xref (DataStreamOutput & dst, size_t startxref)
{
   dst.cat ("%1%\n").arg (DocumentKeywords::_startxref_0);

   dst.cat ("%1%\n").arg (startxref);

   dst.cat ("%1%").arg (DocumentKeywords::_eof_0);
}



/*
==============================================================================
Name : write_begin_obj
==============================================================================
*/

void  DocumentFormatter::write_begin_obj (DataStreamOutput & dst, flip::Ref ref, const std::string & class_name)
{
   dst.cat ("%1% ").arg (ref);
   dst.cat ("%1% ").arg (DocumentKeywords::_obj_0);
   dst.cat ("%1%\n").arg (DocumentKeywords::_begin_dict_0);
   
   dst.cat ("\t%1% ").arg (DocumentKeywords::_class_name_0);
   dst.cat ("%1%\n").arg (class_name.c_str ());
}



/*
==============================================================================
Name : write_end_obj
==============================================================================
*/

void  DocumentFormatter::write_end_obj (DataStreamOutput & dst)
{
   dst.cat ("%1%\n").arg (DocumentKeywords::_end_dict_0);
   dst.cat ("%1%\n\n").arg (DocumentKeywords::_endobj_0);
}



/*
==============================================================================
Name : write_begin_blob
==============================================================================
*/

void  DocumentFormatter::write_begin_blob (DataStreamOutput & dst, flip::Ref ref, const std::vector <archi::UByte> & data, const std::vector <Codec::Encoder> & encoder_arr)
{
   dst.cat ("%1% ").arg (ref);
   dst.cat ("%1% ").arg (DocumentKeywords::_obj_0);
   dst.cat ("%1%\n").arg (DocumentKeywords::_begin_dict_0);

   dst.cat ("\t%1%\n").arg (DocumentKeywords::_blob_0);

   dst.cat ("\t\t%1% ").arg (DocumentKeywords::_length_0);
   dst.cat ("%1%\n").arg (data.size ());
   
   dst.cat ("\t\t%1% ").arg (DocumentKeywords::_filter_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_begin_array_0);
   
   if (encoder_arr.size () > 0)
   {
      for (int i = encoder_arr.size () - 1 ; i >= 0 ; --i)
      {
         Codec::Encoder encoder = encoder_arr [i];
         
         switch (encoder)
         {
         case Codec::Encoder_BASE64:
            dst.cat ("%1% ").arg (DocumentKeywords::_base64_decode_0);
            break;

         case Codec::Encoder_DEFLATE:
            dst.cat ("%1% ").arg (DocumentKeywords::_flate_decode_0);
            break;
         
         case Codec::Encoder_ESCAPED_ASCII:
            dst.cat ("%1% ").arg (DocumentKeywords::_escaped_ascii_decode_0);
            break;
         
         case Codec::Encoder_HEXADECIMAL:
            dst.cat ("%1% ").arg (DocumentKeywords::_hexadecimal_decode_0);
            break;
         
         default:
            assert (false);
            break;
         }
      }
   }
   
   dst.cat ("%1%\n").arg (DocumentKeywords::_end_array_0);
   dst.cat ("%1%\n").arg (DocumentKeywords::_end_dict_0);
}



/*
==============================================================================
Name : write_blob
==============================================================================
*/

void  DocumentFormatter::write_blob (DataStreamOutput & dst, const std::vector <archi::UByte> & data)
{
   dst.cat ("%1%").arg (DocumentKeywords::_begin_stream_0);
   dst.cat ("%1%").arg (DocumentKeywords::_lf_0);
   
   dst.print_raw (data);
   dst.cat ("\n");
   
   dst.cat ("%1%\n").arg (DocumentKeywords::_end_stream_0);
}



/*
==============================================================================
Name : write_end_blob
==============================================================================
*/

void  DocumentFormatter::write_end_blob (DataStreamOutput & dst)
{
   dst.cat ("%1%\n\n").arg (DocumentKeywords::_endobj_0);
}



/*
==============================================================================
Name : write_member
==============================================================================
*/

void  DocumentFormatter::write_member (DataStreamOutput & dst, const std::string & member_name)
{
   dst.cat ("\t\t%1% ").arg (DocumentKeywords::_attribute_name_0);
   dst.cat ("%1% ").arg (member_name.c_str ());
}



/*
==============================================================================
Name : write_member_bool
==============================================================================
*/

void  DocumentFormatter::write_member_bool (DataStreamOutput & dst, bool val)
{
   dst.cat ("%1% ").arg (DocumentKeywords::_type_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_bool_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_value_0);
   write_bool (dst, val);
   dst.cat ("\n");
}



/*
==============================================================================
Name : write_member_int64
==============================================================================
*/

void  DocumentFormatter::write_member_int64 (DataStreamOutput & dst, archi::Int64 val)
{
   dst.cat ("%1% ").arg (DocumentKeywords::_type_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_int64_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_value_0);
   write_int64 (dst, val);
   dst.cat ("\n");
}



/*
==============================================================================
Name : write_member_enum
==============================================================================
*/

void  DocumentFormatter::write_member_enum (DataStreamOutput & dst, const std::string & enum_type_name, const std::string & val)
{
   dst.cat ("%1% ").arg (DocumentKeywords::_type_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_enum_0);
   dst.cat ("%1% ").arg (enum_type_name.c_str ());
   dst.cat ("%1% ").arg (DocumentKeywords::_value_0);
   dst.cat ("%1%").arg (val.c_str ());
   dst.cat ("\n");
}



/*
==============================================================================
Name : write_member_float64
==============================================================================
*/

void  DocumentFormatter::write_member_float64 (DataStreamOutput & dst, archi::Float64 val)
{
   dst.cat ("%1% ").arg (DocumentKeywords::_type_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_float64_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_value_0);
   write_float64 (dst, val);
   dst.cat ("\n");
}



/*
==============================================================================
Name : write_member_float64x
==============================================================================
*/

void  DocumentFormatter::write_member_float64x (DataStreamOutput & dst, archi::Float64 val)
{
   dst.cat ("%1% ").arg (DocumentKeywords::_type_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_float64_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_value_0);
   write_float64x (dst, val);
   dst.cat ("\n");
}



/*
==============================================================================
Name : write_member_cue
==============================================================================
*/

void  DocumentFormatter::write_member_cue (DataStreamOutput & dst)
{
   dst.cat ("%1% ").arg (DocumentKeywords::_type_0);
   dst.cat ("%1%\n").arg (DocumentKeywords::_cue_0);
}



/*
==============================================================================
Name : write_member_blob_empty
==============================================================================
*/

void  DocumentFormatter::write_member_blob_empty (DataStreamOutput & dst, bool inline_flag)
{
   dst.cat ("%1% ").arg (DocumentKeywords::_type_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_blob_0);
   
   if (inline_flag)
   {
      dst.cat ("%1% ").arg (DocumentKeywords::_inline_0);
   }
   
   dst.cat ("%1%\n").arg (DocumentKeywords::_empty_0);
}



/*
==============================================================================
Name : write_member_blob
==============================================================================
*/

void  DocumentFormatter::write_member_blob (DataStreamOutput & dst, flip::Ref ref)
{
   dst.cat ("%1% ").arg (DocumentKeywords::_type_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_blob_0);

   dst.cat ("%1% ").arg (DocumentKeywords::_ref_0);
   dst.cat ("%1%\n").arg (ref);
}



/*
==============================================================================
Name : write_member_blob_inline
==============================================================================
*/

void  DocumentFormatter::write_member_blob_inline (DataStreamOutput & dst, const std::vector <archi::UByte> & data, const std::vector <Codec::Encoder> & encoder_arr)
{
   dst.cat ("%1% ").arg (DocumentKeywords::_type_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_blob_0);

   dst.cat ("%1% ").arg (DocumentKeywords::_inline_0);
   
   dst.cat ("%1% ").arg (DocumentKeywords::_length_0);
   dst.cat ("%1% ").arg (data.size ());

   dst.cat ("%1% ").arg (DocumentKeywords::_filter_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_begin_array_0);
   
   if (encoder_arr.size () > 0)
   {
      for (int i = encoder_arr.size () - 1 ; i >= 0 ; --i)
      {
         Codec::Encoder encoder = encoder_arr [i];
         
         switch (encoder)
         {
         case Codec::Encoder_BASE64:
            dst.cat ("%1% ").arg (DocumentKeywords::_base64_decode_0);
            break;

         case Codec::Encoder_DEFLATE:
            dst.cat ("%1% ").arg (DocumentKeywords::_flate_decode_0);
            break;
         
         case Codec::Encoder_ESCAPED_ASCII:
            dst.cat ("%1% ").arg (DocumentKeywords::_escaped_ascii_decode_0);
            break;
         
         case Codec::Encoder_HEXADECIMAL:
            dst.cat ("%1% ").arg (DocumentKeywords::_hexadecimal_decode_0);
            break;
         
         default:
            assert (false);
            break;
         }
      }
   }
   
   dst.cat ("%1%").arg (DocumentKeywords::_end_array_0);
   dst.cat ("%1%").arg (DocumentKeywords::_space_0);

   dst.print_raw (data);
   dst.cat ("\n");
}



/*
==============================================================================
Name : write_member_object_ref
==============================================================================
*/

void  DocumentFormatter::write_member_object_ref (DataStreamOutput & dst, flip::Ref pointed_ref)
{
   dst.cat ("%1% ").arg (DocumentKeywords::_type_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_object_ref_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_ref_0);
   dst.cat ("%1%\n").arg (pointed_ref);
}



/*
==============================================================================
Name : write_member_object
==============================================================================
*/

void  DocumentFormatter::write_member_object (DataStreamOutput & dst, flip::Ref ref)
{
   dst.cat ("%1% ").arg (DocumentKeywords::_type_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_object_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_ref_0);
   dst.cat ("%1%\n").arg (ref);
}



/*
==============================================================================
Name : write_member_array_bool_begin
==============================================================================
*/

void  DocumentFormatter::write_member_array_bool_begin (DataStreamOutput & dst)
{
   dst.cat ("%1% ").arg (DocumentKeywords::_type_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_array_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_bool_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_begin_array_0);
}



/*
==============================================================================
Name : write_member_array_int64_begin
==============================================================================
*/

void  DocumentFormatter::write_member_array_int64_begin (DataStreamOutput & dst)
{
   dst.cat ("%1% ").arg (DocumentKeywords::_type_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_array_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_int64_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_begin_array_0);
}



/*
==============================================================================
Name : write_member_array_float64_begin
==============================================================================
*/

void  DocumentFormatter::write_member_array_float64_begin (DataStreamOutput & dst)
{
   dst.cat ("%1% ").arg (DocumentKeywords::_type_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_array_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_float64_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_begin_array_0);
}



/*
==============================================================================
Name : write_member_array_blob_begin
==============================================================================
*/

void  DocumentFormatter::write_member_array_blob_begin (DataStreamOutput & dst)
{
   dst.cat ("%1% ").arg (DocumentKeywords::_type_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_array_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_blob_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_begin_array_0);
}



/*
==============================================================================
Name : write_member_array_object_ref_begin
==============================================================================
*/

void  DocumentFormatter::write_member_array_object_ref_begin (DataStreamOutput & dst)
{
   dst.cat ("%1% ").arg (DocumentKeywords::_type_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_array_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_object_ref_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_begin_array_0);
}



/*
==============================================================================
Name : write_member_array_object_begin
==============================================================================
*/

void  DocumentFormatter::write_member_array_object_begin (DataStreamOutput & dst)
{
   dst.cat ("%1% ").arg (DocumentKeywords::_type_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_array_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_ref_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_begin_array_0);
}



/*
==============================================================================
Name : write_member_array_key_ref
==============================================================================
*/

void  DocumentFormatter::write_member_array_key_ref (DataStreamOutput & dst, const BinaryKey & key, flip::Ref ref)
{
   std::string key_str;
   key.write (key_str);
   
   dst.cat ("%1% ").arg (key_str.c_str ());
   
   dst.cat ("%1% ").arg (ref);
}



/*
==============================================================================
Name : write_member_array_end
==============================================================================
*/

void  DocumentFormatter::write_member_array_end (DataStreamOutput & dst)
{
   dst.cat ("%1%\n").arg (DocumentKeywords::_end_array_0);
}



/*
==============================================================================
Name : write_member_collection_bool_begin
==============================================================================
*/

void  DocumentFormatter::write_member_collection_bool_begin (DataStreamOutput & dst)
{
   dst.cat ("%1% ").arg (DocumentKeywords::_type_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_collection_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_bool_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_begin_collection_0);
}



/*
==============================================================================
Name : write_member_collection_int64_begin
==============================================================================
*/

void  DocumentFormatter::write_member_collection_int64_begin (DataStreamOutput & dst)
{
   dst.cat ("%1% ").arg (DocumentKeywords::_type_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_collection_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_int64_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_begin_collection_0);
}



/*
==============================================================================
Name : write_member_collection_float64_begin
==============================================================================
*/

void  DocumentFormatter::write_member_collection_float64_begin (DataStreamOutput & dst)
{
   dst.cat ("%1% ").arg (DocumentKeywords::_type_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_collection_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_float64_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_begin_collection_0);
}



/*
==============================================================================
Name : write_member_collection_blob_begin
==============================================================================
*/

void  DocumentFormatter::write_member_collection_blob_begin (DataStreamOutput & dst)
{
   dst.cat ("%1% ").arg (DocumentKeywords::_type_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_collection_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_blob_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_begin_collection_0);
}



/*
==============================================================================
Name : write_member_collection_object_ref_begin
==============================================================================
*/

void  DocumentFormatter::write_member_collection_object_ref_begin (DataStreamOutput & dst)
{
   dst.cat ("%1% ").arg (DocumentKeywords::_type_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_collection_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_object_ref_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_begin_collection_0);
}



/*
==============================================================================
Name : write_member_collection_object_begin
==============================================================================
*/

void  DocumentFormatter::write_member_collection_object_begin (DataStreamOutput & dst)
{
   dst.cat ("%1% ").arg (DocumentKeywords::_type_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_collection_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_ref_0);
   dst.cat ("%1% ").arg (DocumentKeywords::_begin_collection_0);
}



/*
==============================================================================
Name : write_member_collection_ref
==============================================================================
*/

void  DocumentFormatter::write_member_collection_ref (DataStreamOutput & dst, flip::Ref ref)
{
   dst.cat ("%1% ").arg (ref);
}



/*
==============================================================================
Name : write_member_collection_end
==============================================================================
*/

void  DocumentFormatter::write_member_collection_end (DataStreamOutput & dst)
{
   dst.cat ("%1%\n").arg (DocumentKeywords::_end_collection_0);
}



/*
==============================================================================
Name : write_member_container_bool
==============================================================================
*/

void  DocumentFormatter::write_member_container_bool (DataStreamOutput & dst, bool val)
{
   write_bool (dst, val);
   dst.cat (" ");
}



/*
==============================================================================
Name : write_member_container_int64
==============================================================================
*/

void  DocumentFormatter::write_member_container_int64 (DataStreamOutput & dst, archi::Int64 val)
{
   write_int64 (dst, val);
   dst.cat (" ");
}



/*
==============================================================================
Name : write_member_container_float64
==============================================================================
*/

void  DocumentFormatter::write_member_container_float64 (DataStreamOutput & dst, archi::Float64 val)
{
   write_float64 (dst, val);
   dst.cat (" ");
}



/*
==============================================================================
Name : write_member_container_float64x
==============================================================================
*/

void  DocumentFormatter::write_member_container_float64x (DataStreamOutput & dst, archi::Float64 val)
{
   write_float64x (dst, val);
   dst.cat (" ");
}



/*
==============================================================================
Name : write_member_container_object_ref
==============================================================================
*/

void  DocumentFormatter::write_member_container_object_ref (DataStreamOutput & dst, flip::Ref pointed_ref)
{
   write_ref (dst, pointed_ref);
   dst.cat (" ");
}



/*
==============================================================================
Name : write_member_container_object
==============================================================================
*/

void  DocumentFormatter::write_member_container_object (DataStreamOutput & dst, flip::Ref ref)
{
   write_ref (dst, ref);
   dst.cat (" ");
}



/*
==============================================================================
Name : write_bool
==============================================================================
*/

void  DocumentFormatter::write_bool (DataStreamOutput & dst, bool val)
{
   if (val)
   {
      dst.cat ("%1%").arg (DocumentKeywords::_true_0);
   }
   else
   {
      dst.cat ("%1%").arg (DocumentKeywords::_false_0);
   }
}



/*
==============================================================================
Name : write_int64
==============================================================================
*/

void  DocumentFormatter::write_int64 (DataStreamOutput & dst, archi::Int64 val)
{
   dst.cat ("%1%").arg (val);
}



/*
==============================================================================
Name : write_float64
==============================================================================
*/

void  DocumentFormatter::write_float64 (DataStreamOutput & dst, archi::Float64 val)
{
   dst.cat ("%1ep16%").arg (val);
}



/*
==============================================================================
Name : write_float64x
==============================================================================
*/

void  DocumentFormatter::write_float64x (DataStreamOutput & dst, archi::Float64 val)
{
   archi::UInt64 tval = reinterpret_cast <archi::UInt64 &> (val);
   
   // ("0x%1m16rw0x%") won't work for negative numbers if they were casted to Int64
   
   char buf [17];
   
   for (int i = 0 ; i < 16 ; ++i)
   {
      int d = int (tval & 15);
      tval >>= 4;
      
      char c;
      
      if ((d >= 0) && (d <= 9))
      {
         c = '0' + d;
      }
      else
      {
         c = 'a' + (d - 10);
      }
      
      buf [15 - i] = c;
   }
   
   buf [16] = '\0';
   
   dst.cat ("0x%1%").arg (buf);
}



/*
==============================================================================
Name : write_ref
==============================================================================
*/

void  DocumentFormatter::write_ref (DataStreamOutput & dst, flip::Ref ref)
{
   write_int64 (dst, ref);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

DocumentFormatter::DocumentFormatter ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
