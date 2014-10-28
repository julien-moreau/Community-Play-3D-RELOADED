/*****************************************************************************

        DocumentReader.h
        Copyright (c) 2007 Ohm Force

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



#if ! defined (ohm_flip_DocumentReader_HEADER_INCLUDED)
#define  ohm_flip_DocumentReader_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/flip/Data.h"
#include "ohm/flip/Err.h"
#include "ohm/flip/private/DataStreamInput.h"
#include "ohm/flip/private/DocumentParser.h"
#include "ohm/flip/ClassDescription.h"

#include <fstream>
#include <list>
#include <set>
#include <map>
#include <string>



namespace ohm
{
namespace flip
{



class Object;
class Float64;
class Int64;
class Blob;
class ObjectRefMaster;
class ArrayMaster;
class CollectionMaster;
class DocumentBase;
class BinaryKey;
class UserManager;

class DocumentReader
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  DocumentReader ();
   virtual        ~DocumentReader ();
   
   int            load (const char * path_0);
   int            load_no_copy (const archi::UByte * data_ptr, size_t data_size);
   int            load_no_copy (const std::string & document_str);
   
   archi::Int64   get_rev_id () const;
   const std::string &
                  get_format_version () const;
   void           get_users (UserManager & user_manager);

   int            read ();
   int            exec (DocumentBase & document);
   
   static int     read_object_ref (Data & tx_data, DocumentParser & dp, flip::Ref ref);
   static int     read_object_ref_cdm (ClassDescriptionMaster * & cdm_ptr, DocumentParser & dp, flip::Ref ref);
   
   static int     read_attribute (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, ClassDescriptionMaster * cdm_ptr);
   static int     read_attribute_object (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);
   static int     read_attribute_bool (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);
   static int     read_attribute_int64 (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);
   static int     read_attribute_enum (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);
   static int     read_attribute_float64 (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);
   static int     read_attribute_blob (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);
   static int     read_attribute_cue (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);

   static int     read_attribute_array (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);
   static int     read_attribute_array_object (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);
   static int     read_attribute_array_int64 (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);
   static int     read_attribute_array_float64 (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);

   static int     read_attribute_collection (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);
   static int     read_attribute_collection_object (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);
   static int     read_attribute_collection_int64 (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);
   static int     read_attribute_collection_float64 (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);

   static int     read_attribute_object_ref (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);
   
   static int     read_blob_data_ref (std::vector <archi::UByte> & data, DocumentParser & dp, flip::Ref ref);
   static int     read_blob_ref (Data & tx_data, DocumentParser & dp, flip::Ref ref);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   DataStreamInput
                  _ds;
   DocumentParser _dp;
   
   Data           _tx_data;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DocumentReader (const DocumentReader &other);
   DocumentReader &     operator = (const DocumentReader &other);
   bool           operator == (const DocumentReader &other);
   bool           operator != (const DocumentReader &other);

}; // class DocumentReader



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/DocumentReader.hpp"



#endif   // ohm_flip_DocumentReader_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
