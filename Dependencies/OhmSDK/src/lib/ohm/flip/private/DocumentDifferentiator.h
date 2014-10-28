/*****************************************************************************

        DocumentDifferentiator.h
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



#if ! defined (ohm_flip_DocumentDifferentiator_HEADER_INCLUDED)
#define  ohm_flip_DocumentDifferentiator_HEADER_INCLUDED

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
#include "ohm/flip/private/MachineCode.h"
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

class DocumentDifferentiator
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  DocumentDifferentiator ();
   virtual        ~DocumentDifferentiator ();
   
   int            load_src (const char * path_0);
   int            load_src_no_copy (const archi::UByte * data_ptr, int data_size);
   int            load_src_no_copy (const std::string & document_str);

   int            load_dst (const char * path_0);
   int            load_dst_no_copy (const archi::UByte * data_ptr, int data_size);
   int            load_dst_no_copy (const std::string & document_str);
   
   void           exclude (Object & object);
   
   int            differentiate ();
   DataSPtr       get_tx_data ();
   int            exec (DocumentBase & document);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   class ContainerObjectInfo
   {
   public:
                  ContainerObjectInfo () : _ref (0) {}

      inline bool is_equal (const ContainerObjectInfo & other) const;
      inline bool is_less (const ContainerObjectInfo & other) const;
      
      bool        _array_flag;
      BinaryKey   _key;
      archi::Int64
                  _ref;
      union {
         archi::Int64
                  _val_i64;
         archi::Float64
                  _val_f64;
      };
   };

   typedef std::list <ContainerObjectInfo> ContainerObjectInfoList;
   typedef std::list <std::pair <ContainerObjectInfoList::iterator, ContainerObjectInfoList::iterator> > ContainerObjectItPairList;

   class ActorArrayObject
   {
   public:
      inline      ActorArrayObject (DocumentDifferentiator & dd, flip::Ref ref);
      inline int  differentiate_same_ref (const ContainerObjectInfo & src, const ContainerObjectInfo & dst);
      inline int  ref_moved (const ContainerObjectInfo & src, const ContainerObjectInfo & dst);
      inline int  ref_moved_accross (const ContainerObjectInfo & src);
      inline int  ref_erased (const ContainerObjectInfo & elem);
      inline int  ref_inserted (const ContainerObjectInfo & elem);
   
   private:
      DocumentDifferentiator &
                  _dd;
      const archi::Int64
                  _ref;
   };
   
   class ActorArrayInt64
   {
   public:
      inline      ActorArrayInt64 (DocumentDifferentiator & dd, flip::Ref ref);
      inline int  differentiate_same_ref (const ContainerObjectInfo & src, const ContainerObjectInfo & dst);
      inline int  ref_moved (const ContainerObjectInfo & src, const ContainerObjectInfo & dst);
      inline int  ref_moved_accross (const ContainerObjectInfo & src);
      inline int  ref_erased (const ContainerObjectInfo & elem);
      inline int  ref_inserted (const ContainerObjectInfo & elem);
   
   private:
      DocumentDifferentiator &
                  _dd;
      const archi::Int64
                  _ref;
   };
   
   class ActorArrayFloat64
   {
   public:
      inline      ActorArrayFloat64 (DocumentDifferentiator & dd, flip::Ref ref);
      inline int  differentiate_same_ref (const ContainerObjectInfo & src, const ContainerObjectInfo & dst);
      inline int  ref_moved (const ContainerObjectInfo & src, const ContainerObjectInfo & dst);
      inline int  ref_moved_accross (const ContainerObjectInfo & src);
      inline int  ref_erased (const ContainerObjectInfo & elem);
      inline int  ref_inserted (const ContainerObjectInfo & elem);
   
   private:
      DocumentDifferentiator &
                  _dd;
      const archi::Int64
                  _ref;
   };
   
   class ActorCollectionObject
   {
   public:
      inline      ActorCollectionObject (DocumentDifferentiator & dd, flip::Ref ref);
      inline int  differentiate_same_ref (const ContainerObjectInfo & src, const ContainerObjectInfo & dst);
      inline int  ref_moved (const ContainerObjectInfo & src, const ContainerObjectInfo & dst);
      inline int  ref_moved_accross (const ContainerObjectInfo & src);
      inline int  ref_erased (const ContainerObjectInfo & elem);
      inline int  ref_inserted (const ContainerObjectInfo & elem);
   
   private:
      DocumentDifferentiator &
                  _dd;
      const archi::Int64
                  _ref;
   };
   
   class ActorCollectionInt64
   {
   public:
      inline      ActorCollectionInt64 (DocumentDifferentiator & dd, flip::Ref ref);
      inline int  differentiate_same_ref (const ContainerObjectInfo & src, const ContainerObjectInfo & dst);
      inline int  ref_moved (const ContainerObjectInfo & src, const ContainerObjectInfo & dst);
      inline int  ref_moved_accross (const ContainerObjectInfo & src);
      inline int  ref_erased (const ContainerObjectInfo & elem);
      inline int  ref_inserted (const ContainerObjectInfo & elem);
   
   private:
      DocumentDifferentiator &
                  _dd;
      const archi::Int64
                  _ref;
   };
   
   class ActorCollectionFloat64
   {
   public:
      inline      ActorCollectionFloat64 (DocumentDifferentiator & dd, flip::Ref ref);
      inline int  differentiate_same_ref (const ContainerObjectInfo & src, const ContainerObjectInfo & dst);
      inline int  ref_moved (const ContainerObjectInfo & src, const ContainerObjectInfo & dst);
      inline int  ref_moved_accross (const ContainerObjectInfo & src);
      inline int  ref_erased (const ContainerObjectInfo & elem);
      inline int  ref_inserted (const ContainerObjectInfo & elem);
   
   private:
      DocumentDifferentiator &
                  _dd;
      const archi::Int64
                  _ref;
   };
   
   friend class ActorArrayObject;
   friend class ActorArrayInt64;
   friend class ActorArrayFloat64;
   friend class ActorCollectionObject;
   friend class ActorCollectionInt64;
   friend class ActorCollectionFloat64;
   
   typedef std::set <flip::Ref> RefSet;
   
   
   int            differentiate_object_ref (flip::Ref ref);
   int            differentiate_attribute (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, ClassDescriptionMaster * cdm_ptr);
   int            differentiate_attribute_object (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);
   int            differentiate_attribute_bool (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);
   int            differentiate_attribute_int64 (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);
   int            differentiate_attribute_enum (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);
   int            differentiate_attribute_float64 (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);
   int            differentiate_attribute_blob (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);
   int            differentiate_attribute_cue (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);
   
   int            differentiate_attribute_array (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);
   int            differentiate_attribute_array_object (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);
   int            differentiate_attribute_array_int64 (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);
   int            differentiate_attribute_array_float64 (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);

   int            differentiate_attribute_collection (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);
   int            differentiate_attribute_collection_object (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);
   int            differentiate_attribute_collection_int64 (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);
   int            differentiate_attribute_collection_float64 (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);

   int            differentiate_attribute_object_ref (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr);
   
   int            read_attribute_array_object (ContainerObjectInfoList & info_list, DocumentParser::Object & object);
   int            read_attribute_array_int64 (ContainerObjectInfoList & info_list, DocumentParser::Object & object);
   int            read_attribute_array_float64 (ContainerObjectInfoList & info_list, DocumentParser::Object & object);

   int            read_attribute_collection_object (ContainerObjectInfoList & info_list, DocumentParser::Object & object);
   int            read_attribute_collection_int64 (ContainerObjectInfoList & info_list, DocumentParser::Object & object);
   int            read_attribute_collection_float64 (ContainerObjectInfoList & info_list, DocumentParser::Object & object);

   void           merge_ordered (ContainerObjectItPairList & it_pair_list, ContainerObjectInfoList & src_info_list, ContainerObjectInfoList & dst_info_list);
   
   template <class Actor>
   int            differentiate (Actor & actor, ContainerObjectItPairList & it_pair_list, ContainerObjectInfoList & src_info_list, ContainerObjectInfoList & dst_info_list);
   
   DataStreamInput
                  _src_ds;
   DocumentParser _src_dp;
   MachineCode::RefContainerElemMap
                  _src_container_ref_map;

   DataStreamInput
                  _dst_ds;
   DocumentParser _dst_dp;
   MachineCode::RefContainerElemMap
                  _dst_array_ref_map;
   MachineCode::RefContainerElemMap
                  _dst_container_ref_map;
   
   DataSPtr       _tx_data_sptr;
   
   RefSet         _excluded_ref_set;
   int            _exclude_cnt;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DocumentDifferentiator (const DocumentDifferentiator &other);
   DocumentDifferentiator &
                  operator = (const DocumentDifferentiator &other);
   bool           operator == (const DocumentDifferentiator &other);
   bool           operator != (const DocumentDifferentiator &other);

}; // class DocumentDifferentiator



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/flip/private/DocumentDifferentiator.hpp"



#endif   // ohm_flip_DocumentDifferentiator_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
