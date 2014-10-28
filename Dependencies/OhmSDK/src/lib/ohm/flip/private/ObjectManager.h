/*****************************************************************************

        ObjectManager.h
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71329

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



#if ! defined (ohm_flip_ObjectManager_HEADER_INCLUDED)
#define  ohm_flip_ObjectManager_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/flip/Object.h"
#include "ohm/flip/Err.h"

#include <string>
#include <map>
#include <list>
#include <set>



namespace ohm
{
namespace flip
{



class Transaction;
class ClassDescriptionMaster;
class Object;
class DocumentBase;

class ObjectManager
{

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   enum ObjectState
   {
                  ObjectState_RESIDENT = 0,
                  ObjectState_RECYCLED,
   };
   
   class ObjectInfo
   {
   public:
      Object *    _object_ptr;
      ObjectState _state;
   };
   
   typedef std::map <flip::Ref, ObjectInfo> RefObjectInfoMap;

   

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class iterator
   {
   public:
                  iterator ();
      virtual     ~iterator ();
      iterator &  operator ++ ();
      bool        operator != (const iterator & other) const;
      flip::Ref   get_ref () const;
      Object *    get_object_ptr () const;
   
   private:
      friend class ObjectManager;
      RefObjectInfoMap::iterator
                  _it;
   };
   
                  ObjectManager (DocumentBase & document, archi::Int32 user_id, archi::Int32 thread_id);
   virtual        ~ObjectManager ();
   
   flip::Ref      assign_ref (Object * & parent_ptr, ClassDescriptionMaster * & cdm_ptr, Object * ptr);
   void           unassign_ref (flip::Ref ref);
   flip::Ref      get_planned_ref ();

   Object *       create_root ();
   Object *       create_root (ClassDescriptionMaster & cdm);
   void           release_root ();
   
   int            check_creation (ClassDescriptionMaster & cdm, flip::Ref ref, bool tolerant_flag);
   Object *       create (ClassDescriptionMaster & cdm, Object * parent_ptr, flip::Ref ref);
   Object *       create (ClassDescriptionMaster & cdm, Object * parent_ptr, flip::Ref ref, Args & args);
   
   bool           can_recycle (Object * object_ptr);
   void           recycle (Object * object_ptr);
   void           empty_recycle_bin ();
   bool           is_recycle_bin_empty ();
   
   Object *       get_object (flip::Ref ref);

   Object *       get_root ();
   
   iterator       begin ();
   iterator       end ();
   
   void           trace () const;
   void           trace_recycle_bin () const;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   class RefInfo
   {
   public:
      flip::Ref   _ref;
      flip::Ref   _parent_ref;
      ClassDescriptionMaster *
                  _cdm_ptr;
   };
   
   typedef std::list <RefInfo> RefInfoList;
   typedef std::list <Object *> ObjectPtrList;
   
   ObjectPtrList::iterator
                  find_recycle_bin (Object * object_ptr);
   void           restore_bin_from_pool ();
   
   void           make_ref_info_pool (ClassDescriptionMaster & cdm, flip::Ref ref, Object * parent_ptr);
   void           make_ref_info_pool_recur (ClassDescriptionMaster * cdm_ptr, flip::Ref & ref, flip::Ref parent_ref);

   int            check_ref_info_pool (ClassDescriptionMaster & cdm, flip::Ref ref, bool tolerant_flag);
   int            check_ref_info_pool_recur (ClassDescriptionMaster * cdm_ptr, flip::Ref & ref, flip::Ref parent_ref, bool tolerant_flag);

   DocumentBase & _document;
   const archi::Int32
                  _user_id;
   const archi::Int32
                  _thread_id;
   
   flip::Ref      _cur_object_ref;
   bool           _in_create_flag;

   RefInfoList    _ref_info_pool;
   
   RefObjectInfoMap
                  _ref_object_info_map;
   ObjectPtrList  _recycle_bin;
   
   Object *       _root_ptr;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ObjectManager ();
                  ObjectManager (const ObjectManager &other);
   ObjectManager &operator = (const ObjectManager &other);
   bool           operator == (const ObjectManager &other);
   bool           operator != (const ObjectManager &other);

}; // class ObjectManager



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/private/ObjectManager.hpp"



#endif   // ohm_flip_ObjectManager_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
