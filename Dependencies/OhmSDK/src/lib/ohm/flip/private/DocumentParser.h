/*****************************************************************************

        DocumentParser.h
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



#if ! defined (ohm_flip_DocumentParser_HEADER_INCLUDED)
#define  ohm_flip_DocumentParser_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/flip/Err.h"
#include "ohm/flip/Ref.h"
#include "ohm/flip/private/DataStreamInput.h"
#include "ohm/flip/private/Codec.h"

#include <vector>
#include <map>
#include <string>



namespace ohm
{
namespace flip
{



class BinaryKey;
class EnumDescriptionMaster;

class DocumentParser
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class Object
   {
   public:
      enum Type
      {
                  Type_VOID = 0,
                  Type_OBJECT,
                  Type_BOOL,
                  Type_INT64,
                  Type_ENUM,
                  Type_FLOAT64,
                  Type_BLOB,
                  Type_CUE,
                  Type_ARRAY,
                  Type_COLLECTION,
                  Type_OBJECT_REF,
                  Type_BINARY_KEY,
                  Type_REF,
      };
      
                  Object ();
      virtual     ~Object ();
      
      DocumentParser &
                  get_document_parser ();
      
      int         read_class_name (std::string & class_name);
      int         read_attribute_name (std::string & attribute_name);
      int         read_type (Type & type);
      int         read_sub_type (Type & type);
      int         read_sub_enum_name (std::string & enum_name);
      
      int         read_binary_key (BinaryKey & key);
      int         read_ref (flip::Ref & ref);
      int         read_blob (bool & inline_flag, bool & empty_flag, flip::Ref & ref, std::vector <Codec::Encoder> & codec_arr, archi::UByte * & data_ptr, size_t & data_length);
      int         read_bool (bool & val);
      int         read_int64 (archi::Int64 & val);
      int         read_enum (archi::Int64 & val, EnumDescriptionMaster & edm);
      int         read_enum (std::string & val);
      int         read_float64 (archi::Float64 & val);
   
   private:
      friend class DocumentParser;
      DocumentParser *
                  _dp_ptr;
      DataStreamInput *
                  _ds_ptr;
      size_t      _pos;
      Type        _cur_type;
      Type        _cur_sub_type;
      Type        _expected_type;
   };
   
   typedef std::map <std::string, archi::Int32> UserNameIdMap;

                  DocumentParser (DataStreamInput & data_stream);
   virtual        ~DocumentParser ();
   
   void           print ();
   
   int            init ();
   
   flip::Ref      get_root_ref () const;
   archi::Int64   get_rev_id () const;
   const std::string &
                  get_format_version () const;
   const UserNameIdMap &
                  get_user_name_id_map () const;
   
   int            seek_object (Object & object, flip::Ref ref);
   int            read_blob_properties (size_t & length, std::vector <Codec::Encoder> & codec_arr, bool space_term_flag = false);
   int            read_blob (std::vector <archi::UByte> & data, flip::Ref ref);
   int            read_blob (std::vector <archi::UByte> & data, std::vector <Codec::Encoder> & codec_arr, flip::Ref ref);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   typedef std::map <archi::Int64, size_t> RefPosMap;
   
   int            read_file_end ();
   
   DataStreamInput & _ds;
   
   flip::Ref      _root_ref;
   archi::Int64   _rev_id;
   std::string    _format_version;
   UserNameIdMap  _user_name_id_map;
   RefPosMap      _ref_pos_map;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DocumentParser ();
                  DocumentParser (const DocumentParser &other);
   DocumentParser &
                  operator = (const DocumentParser &other);
   bool           operator == (const DocumentParser &other);
   bool           operator != (const DocumentParser &other);

}; // class DocumentParser



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/DocumentParser.hpp"



#endif   // ohm_flip_DocumentParser_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
