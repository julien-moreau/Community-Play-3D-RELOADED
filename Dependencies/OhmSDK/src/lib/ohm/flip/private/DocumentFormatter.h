/*****************************************************************************

        DocumentFormatter.h
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



#if ! defined (ohm_flip_DocumentFormatter_HEADER_INCLUDED)
#define  ohm_flip_DocumentFormatter_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/flip/private/Codec.h"
#include "ohm/flip/Ref.h"

#include <set>
#include <map>



namespace ohm
{
namespace flip
{



class BinaryKey;
class DataStreamOutput;

class DocumentFormatter
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef std::set <size_t> ObjOffsetSet;
   typedef std::map <std::string, archi::Int32> UserNameIdMap; 

   virtual        ~DocumentFormatter ();
   
   static void    write_header (DataStreamOutput & dst);
   static void    write_xref (DataStreamOutput & dst, size_t & startxref, const ObjOffsetSet & obj_offset_set);
   static void    write_trailer (DataStreamOutput & dst, flip::Ref root_ref, archi::Int64 rev_id, const std::string & format_version, const UserNameIdMap & user_name_id_map);
   static void    write_start_xref (DataStreamOutput & dst, size_t startxref);
   
   static void    write_begin_obj (DataStreamOutput & dst, flip::Ref ref, const std::string & class_name);
   static void    write_end_obj (DataStreamOutput & dst);
   
   static void    write_begin_blob (DataStreamOutput & dst, flip::Ref ref, const std::vector <archi::UByte> & data, const std::vector <Codec::Encoder> & encoder_arr);
   static void    write_blob (DataStreamOutput & dst, const std::vector <archi::UByte> & data);
   static void    write_end_blob (DataStreamOutput & dst);
   
   static void    write_member (DataStreamOutput & dst, const std::string & member_name);
   static void    write_member_bool (DataStreamOutput & dst, bool val);
   static void    write_member_int64 (DataStreamOutput & dst, archi::Int64 val);
   static void    write_member_enum (DataStreamOutput & dst, const std::string & enum_type_name, const std::string & val);
   static void    write_member_float64 (DataStreamOutput & dst, archi::Float64 val);
   static void    write_member_float64x (DataStreamOutput & dst, archi::Float64 val);
   static void    write_member_cue (DataStreamOutput & dst);
   static void    write_member_blob_empty (DataStreamOutput & dst, bool inline_flag);
   static void    write_member_blob (DataStreamOutput & dst, flip::Ref ref);
   static void    write_member_blob_inline (DataStreamOutput & dst, const std::vector <archi::UByte> & data, const std::vector <Codec::Encoder> & encoder_arr);
   static void    write_member_object_ref (DataStreamOutput & dst, flip::Ref pointed_ref);
   static void    write_member_object (DataStreamOutput & dst, flip::Ref ref);

   static void    write_member_array_bool_begin (DataStreamOutput & dst);
   static void    write_member_array_int64_begin (DataStreamOutput & dst);
   static void    write_member_array_float64_begin (DataStreamOutput & dst);
   static void    write_member_array_blob_begin (DataStreamOutput & dst);
   static void    write_member_array_object_ref_begin (DataStreamOutput & dst);
   static void    write_member_array_object_begin (DataStreamOutput & dst);
   static void    write_member_array_key_ref (DataStreamOutput & dst, const BinaryKey & key, flip::Ref ref);
   static void    write_member_array_end (DataStreamOutput & dst);

   static void    write_member_collection_bool_begin (DataStreamOutput & dst);
   static void    write_member_collection_int64_begin (DataStreamOutput & dst);
   static void    write_member_collection_float64_begin (DataStreamOutput & dst);
   static void    write_member_collection_blob_begin (DataStreamOutput & dst);
   static void    write_member_collection_object_ref_begin (DataStreamOutput & dst);
   static void    write_member_collection_object_begin (DataStreamOutput & dst);
   static void    write_member_collection_ref (DataStreamOutput & dst, flip::Ref ref);
   static void    write_member_collection_end (DataStreamOutput & dst);

   static void    write_member_container_bool (DataStreamOutput & dst, bool val);
   static void    write_member_container_int64 (DataStreamOutput & dst, archi::Int64 val);
   static void    write_member_container_float64 (DataStreamOutput & dst, archi::Float64 val);
   static void    write_member_container_float64x (DataStreamOutput & dst, archi::Float64 val);
   static void    write_member_container_object_ref (DataStreamOutput & dst, flip::Ref pointed_ref);
   static void    write_member_container_object (DataStreamOutput & dst, flip::Ref ref);
   
   static void    write_bool (DataStreamOutput & dst, bool val);
   static void    write_int64 (DataStreamOutput & dst, archi::Int64 val);
   static void    write_float64 (DataStreamOutput & dst, archi::Float64 val);
   static void    write_float64x (DataStreamOutput & dst, archi::Float64 val);
   static void    write_ref (DataStreamOutput & dst, flip::Ref ref);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  DocumentFormatter ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   

/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DocumentFormatter (const DocumentFormatter &other);
   DocumentFormatter &
                  operator = (const DocumentFormatter &other);
   bool           operator == (const DocumentFormatter &other);
   bool           operator != (const DocumentFormatter &other);

}; // class DocumentFormatter



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/DocumentFormatter.hpp"



#endif   // ohm_flip_DocumentFormatter_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
