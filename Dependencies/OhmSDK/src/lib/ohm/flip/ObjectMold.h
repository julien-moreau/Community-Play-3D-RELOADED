/*****************************************************************************

        ObjectMold.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70989

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



#if ! defined (ohm_flip_ObjectMold_HEADER_INCLUDED)
#define  ohm_flip_ObjectMold_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/ObjectMoldData.h"
#include "ohm/mem/SharedPtr.h"
#include "ohm/flip/private/IdPtrMapSmall.h"

#include <list>



namespace ohm
{
namespace flip
{



class PasteBoard;
class DocumentBase;

class ObjectMold
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  ObjectMold (Object & object);
   virtual        ~ObjectMold ();

   template <class Type>
   bool           is_of_type () const;
   ClassDescriptionMaster &
                  get_cdm ();
   
   template <class Type>
   ObjectMoldData &
                  get () const;
   
   template <class T>
   void           reset_data (T * ptr = 0);
   template <class T>
   T *            get_data () const;
   
   // Container access
   void           execute (DocumentBase & document, flip::Ref ref);
   void           execute (DocumentBase & document, flip::Ref ref, RefRefArr & ref_ref_arr);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   typedef std::pair <flip::Ref, archi::Int64>  RefOffset;
   typedef std::map <flip::Ref, flip::Ref>   RefMapping;

   class Instruction
   {
   public:
      virtual     ~Instruction () {}
      
      virtual void
                  execute (DocumentBase & document, RefMapping & ref_mapping) = 0;
      virtual void
                  post_execute (DocumentBase & document, RefMapping & ref_mapping) {}
      void        make_mapping (RefMapping & ref_mapping, flip::Ref src_ref, flip::Ref dst_ref);
      flip::Ref   translate (const RefMapping & ref_mapping, const RefOffset & ref_offset);
      virtual void
                  push_translation (RefRefArr & ref_ref_arr, RefMapping & ref_mapping);

      RefOffset   _ref_offset;
   };
   
   typedef std::list <Instruction *> InstructionPtrList;
   
   class InstructionCollectionInsert
   :  public Instruction
   {
   public:
      virtual     ~InstructionCollectionInsert () {}
      
      virtual void
                  execute (DocumentBase & document, RefMapping & ref_mapping);
      virtual void
                  push_translation (RefRefArr & ref_ref_arr, RefMapping & ref_mapping);

      flip::Ref   _sub_ref;
      flip::Ref   _new_sub_ref;
      
      ClassDescriptionMaster *
                  _cdm_ptr;
   };
   
   class InstructionArrayInsert
   :  public Instruction
   {
   public:
      virtual     ~InstructionArrayInsert () {}
      
      virtual void
                  execute (DocumentBase & document, RefMapping & ref_mapping);
      virtual void
                  push_translation (RefRefArr & ref_ref_arr, RefMapping & ref_mapping);

      flip::Ref   _sub_ref;
      flip::Ref   _new_sub_ref;
      
      ClassDescriptionMaster *
                  _cdm_ptr;
   };
   
   class InstructionSetBool
   :  public Instruction
   {
   public:
      virtual     ~InstructionSetBool () {}

      virtual void
                  execute (DocumentBase & document, RefMapping & ref_mapping);

      bool        _val;
   };

   class InstructionSetInt64
   :  public Instruction
   {
   public:
      virtual     ~InstructionSetInt64 () {}

      virtual void
                  execute (DocumentBase & document, RefMapping & ref_mapping);

      archi::Int64
                  _val;
   };

   class InstructionSetEnum
   :  public Instruction
   {
   public:
      virtual     ~InstructionSetEnum () {}

      virtual void
                  execute (DocumentBase & document, RefMapping & ref_mapping);

      archi::Int64
                  _val;
   };

   class InstructionSetFloat64
   :  public Instruction
   {
   public:
      virtual     ~InstructionSetFloat64 () {}

      virtual void
                  execute (DocumentBase & document, RefMapping & ref_mapping);

      archi::Float64
                  _val;
   };

   class InstructionSetObjectRef
   :  public Instruction
   {
   public:
      virtual     ~InstructionSetObjectRef () {}

      virtual void
                  execute (DocumentBase & document, RefMapping & ref_mapping);
      virtual void
                  post_execute (DocumentBase & document, RefMapping & ref_mapping);

      flip::Ref   _pointed_ref;
   };

   class InstructionSetBlob
   :  public Instruction
   {
   public:
      virtual     ~InstructionSetBlob () {}

      virtual void
                  execute (DocumentBase & document, RefMapping & ref_mapping);

      Blob::Data  _val;
   };
   
   class InstructionSetCue
   :  public Instruction
   {
   public:
      virtual     ~InstructionSetCue () {}

      virtual void
                  execute (DocumentBase & document, RefMapping & ref_mapping);

      Blob::Data  _val;
   };
   
   void           parse (ObjectMoldData & data, Object & object, const std::string & prefix);
   void           parse_properties (Object & object);
   void           parse_properties (Object & object, flip::Ref base_ref, archi::Int64 offset);
   void           parse_basic_type (Object & object, flip::Ref base_ref, archi::Int64 offset);
   
   const flip::Ref
                  _ref;
   ClassDescriptionMaster &
                  _cdm;
   ObjectMoldDataPtrList
                  _data_ptr_list;
   InstructionPtrList
                  _instruction_ptr_list;
   
   IdPtrMapSmall  _id_ptr_map;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ObjectMold ();
                  ObjectMold (const ObjectMold &other);
   ObjectMold &   operator = (const ObjectMold &other);
   bool           operator == (const ObjectMold &other);
   bool           operator != (const ObjectMold &other);

}; // class ObjectMold

typedef mem::SharedPtr <ObjectMold> ObjectMoldSPtr;
typedef std::list <ObjectMoldSPtr>  ObjectMoldSPtrList;



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/flip/ObjectMold.hpp"



#endif   // ohm_flip_ObjectMold_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
