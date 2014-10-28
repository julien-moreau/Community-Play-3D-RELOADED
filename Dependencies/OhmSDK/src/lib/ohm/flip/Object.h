/*****************************************************************************

        Object.h
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70515

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



#if ! defined (ohm_flip_Object_HEADER_INCLUDED)
#define  ohm_flip_Object_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

//#include "ohm/archi/def.h"
//#include "ohm/archi/types.h"
#include "ohm/flip/Args.h"
#include "ohm/flip/Data.h"
#include "ohm/flip/Ref.h"
#include "ohm/flip/TxSessionGuard.h"
#include "ohm/flip/private/IdPtrMapSmall.h"

#include <vector>

#undef OHM_FLIP_OBJECT_ENFORCE_CORRECT_ANCESTORS_FLAG



namespace ohm
{
namespace flip
{



class DocumentBase;
class DocumentClient;
class ObjectManager;
class ClassDescriptionMaster;
class Args;
class ArrayMaster;
template <class T> class
      Array;
class CollectionMaster;
template <class T> class
      Collection;
class ObjectMold;

class Object
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  Object (DocumentBase & document);
   virtual        ~Object ();
   
   virtual void   ctor (Args & args);
   
   ObjectMold &   copy ();

   bool           send_signal (archi::UInt32 type, DataSPtr data_sptr = DataSPtr ()) const;
   bool           send_signal_server (archi::UInt32 type, DataSPtr data_sptr = DataSPtr ()) const;
   void           send_signal_client (const std::string & to, archi::UInt32 type, const archi::UByte * data_ptr, archi::UInt32 data_size) const;
   
   void           init ();
   void           restore ();
   void           change_order (size_t order);
   void           change_parent (Object * parent_ptr);
   void           set_conflict ();
   
   bool           is_invalid () const;
   bool           has_conflict () const;
   
   bool           is_inited () const;
   bool           was_inited () const;
   bool           was_restored () const;
   bool           did_order_change () const;
   bool           did_parent_change () const;
   size_t         get_order () const;
   size_t         get_old_order () const;

   virtual void   backup ();
   virtual void   synchronize ();
   virtual void   check_synchronized () const;
   virtual void   check_inner_invalidation_cnt (size_t invalid_cnt) const;

   Ref            get_ref () const;
   archi::Int32   get_user_id () const;
   bool           is_self_created () const;
   
   DocumentBase & use_document () const;
   
   void           erase_from_parent ();
   
   Object *       get_parent_ptr ();
   const Object * get_parent_ptr () const;
   Object *       get_old_parent_ptr ();
   const Object * get_old_parent_ptr () const;
   
   template <class T>
   T &            get_parent ();
   template <class T>
   const T &      get_parent () const;
   template <class T>
   T &            get_old_parent ();
   template <class T>
   const T &      get_old_parent () const;

   template <class T>
   T *            get_ancestor_ptr ();
   template <class T>
   const T *      get_ancestor_ptr () const;
   template <class T>
   T &            get_ancestor ();
   template <class T>
   const T &      get_ancestor () const;
   template <class T>
   T *            get_old_ancestor_ptr ();
   template <class T>
   T &            get_old_ancestor ();

   template <class T>
   T *            get_ancestor_level_ptr (int level);
   template <class T>
   T &            get_ancestor_level (int level);
   template <class T>
   T *            get_old_ancestor_level_ptr (int level);
   template <class T>
   T &            get_old_ancestor_level (int level);
   
   template <class T>
   bool           is_ancestor (Object & obj) const;

   ClassDescriptionMaster &
                  get_cdm () const;
   
   template <class T>
   void           reset_data (T * ptr = 0);
   template <class T>
   T *            get_data () const;
   
   virtual void   trace () const;
   
   void           require_sync_operations ();
   virtual void   pre_sync ();
   virtual void   post_sync ();
   
   std::string    who () const;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   void           invalidate ();
   void           propagate_invalidate (size_t cnt);
   void           validate ();
   void           propagate_validate (size_t cnt);
   
   void           conflict ();
   void           propagate_conflict (size_t cnt);
   void           propagate_resolve (size_t cnt);
   
   ObjectManager &
                  use_object_manager () const;
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   template <class> friend class Array;
   friend class Array <class T>;

   template <class> friend class Collection;
   friend class Collection <class T>;
   
   typedef std::pair <Object *, size_t>   ParentOrder;
   

   void           debug_check ();

   DocumentBase & _document;
   
   Ref            _ref;

   ClassDescriptionMaster *
                  _cdm_ptr;
   
   ParentOrder    _old_parent_order;
   ParentOrder    _parent_order;
   
   bool           _old_inited_flag;
   bool           _inited_flag;
   
   size_t         _inner_invalid_cnt;
   size_t         _total_invalid_cnt;

   size_t         _inner_conflict_cnt;
   size_t         _total_conflict_cnt;
   
   IdPtrMapSmall  _id_ptr_map;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Object ();
                  Object (const Object &other);
   Object &       operator = (const Object &other);
   bool           operator == (const Object &other);
   bool           operator != (const Object &other);

}; // class Object



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/Object.hpp"



#endif   // ohm_flip_Object_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
