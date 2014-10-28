/*****************************************************************************

        Collection.h
        Copyright (c) 2009 Ohm Force

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



#if ! defined (ohm_flip_Collection_HEADER_INCLUDED)
#define  ohm_flip_Collection_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/Object.h"
#include "ohm/flip/FindMask.h"
#include "ohm/flip/private/CollectionIteratorBase.h"
#include "ohm/flip/private/CollectionMaster.h"
#include "ohm/archi/def.h"

#include <map>



namespace ohm
{
namespace flip
{



class ObjectMold;

template <class T>
class Collection
:  public CollectionMaster
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef T value_type;
   typedef T & reference;
   typedef const T & const_reference;

private:
   class ObjectWrap
   {
   public:
      enum State
      {
                  State_RESIDENT = 0,
                  State_ADDED,
                  State_REMOVED,
      };
      
                  ObjectWrap (T * obj_ptr);
      bool        operator < (const ObjectWrap & other) const;
      bool        operator == (const ObjectWrap & other) const;
      T *         get () const;

      mutable State
                  _state;

   private:
                  ObjectWrap ();
      bool        operator != (const ObjectWrap & other) const;
      T *         _obj_ptr;
   };

   typedef std::list <ObjectWrap> List;
   
   typedef typename List::iterator List_iterator;
   typedef typename List::const_iterator List_const_iterator;
   typedef typename List::reverse_iterator List_reverse_iterator;
   typedef typename List::const_reverse_iterator List_const_reverse_iterator;

   friend class CollectionIteratorBase <T, Collection <T>, List_iterator>;
   friend class CollectionIteratorBase <T, const Collection <T>, List_const_iterator>;
   friend class CollectionIteratorBase <T, Collection <T>, List_reverse_iterator>;
   friend class CollectionIteratorBase <T, const Collection <T>, List_const_reverse_iterator>;
   
public:
   typedef CollectionIteratorBase <T, Collection <T>, List_iterator> iterator;
   typedef CollectionIteratorBase <T, const Collection <T>, List_const_iterator> const_iterator;
   typedef CollectionIteratorBase <T, Collection <T>, List_reverse_iterator> reverse_iterator;
   typedef CollectionIteratorBase <T, const Collection <T>, List_const_reverse_iterator> const_reverse_iterator;
   
   static void    declare ();
   
                  Collection (DocumentBase & document);
   virtual        ~Collection ();
   
   void           add_dependency (Object & object);
   void           remove_dependencies ();
   
   // Collection accessors
   
   iterator       begin (FindMask find_mask = FindMask_NATURAL);
   const_iterator begin (FindMask find_mask = FindMask_NATURAL) const;
   iterator       end (FindMask find_mask = FindMask_NATURAL);
   const_iterator end (FindMask find_mask = FindMask_NATURAL) const;

   reverse_iterator
                  rbegin (FindMask find_mask = FindMask_NATURAL);
   const_reverse_iterator
                  rbegin (FindMask find_mask = FindMask_NATURAL) const;
   reverse_iterator
                  rend (FindMask find_mask = FindMask_NATURAL);
   const_reverse_iterator
                  rend (FindMask find_mask = FindMask_NATURAL) const;

   iterator       find (const T & elem, FindMask find_mask = FindMask_NATURAL);
   const_iterator find (const T & elem, FindMask find_mask = FindMask_NATURAL) const;
   template <class Predicate>
   iterator       find (const Predicate & predicate, FindMask find_mask = FindMask_NATURAL);
   template <class Predicate>
   const_iterator find (const Predicate & predicate, FindMask find_mask = FindMask_NATURAL) const;
   
   reverse_iterator
                  rfind (const T & elem, FindMask find_mask = FindMask_NATURAL);
   const_reverse_iterator  
                  rfind (const T & elem, FindMask find_mask = FindMask_NATURAL) const;
   template <class Predicate>
   reverse_iterator
                  rfind (const Predicate & predicate, FindMask find_mask = FindMask_NATURAL);
   template <class Predicate>
   const_reverse_iterator  
                  rfind (const Predicate & predicate, FindMask find_mask = FindMask_NATURAL) const;

   virtual size_t size (FindMask find_mask = FindMask_NATURAL) const;

   bool           empty (FindMask find_mask = FindMask_NATURAL) const;
   
   reference      front ();
   const_reference
                  front () const;
   reference      back ();
   const_reference
                  back () const;

   iterator       insert (Args & args = Args::_none);
   template <class U>
   iterator       insert (Args & args = Args::_none);

   void           erase (iterator it);
   void           erase (iterator begin, iterator end);
   virtual void   erase (Ref ref);
   
   virtual void   clear ();

   iterator       move (Collection & src_collection, iterator src_it);
   
   bool           did_value_change () const;
   bool           did_child_order_change () const;
   
   virtual void   synchronize ();
   virtual void   check_synchronized () const;
   virtual void   check_inner_invalidation_cnt (size_t invalid_cnt) const;

   iterator       paste (ObjectMold & mold);
   iterator       paste (ObjectMold & mold, RefRefArr & ref_ref_arr);
   
   // PasteBoard access
   virtual Ref    insert (ClassDescriptionMaster & cdm);
      


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   friend class VirtualMachine;
   friend class DocumentWriter;
   friend class DocumentBase;

   void           collection_changed ();
   void           collection_changed_invalidate ();

   // VirtualMachine private access
   virtual int    insert_no_notification (Ref ref, int class_id, bool tolerant_flag);
   virtual int    erase_no_notification (Ref ref, bool tolerant_flag);
   virtual int    move_no_notification (CollectionMaster * dst_ptr, Object * obj_ptr, bool tolerant_flag);

   // DocumentBase private access
   virtual int    broadcast_order_change ();

   // DocumentWriter private access
   virtual Ref    get_element_ref (size_t i) const;
   virtual int    get_element_class_id () const;
   
   List           _list;
   bool           _old_cur_equal_flag;
   bool           _child_order_changed_flag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Collection ();
                  Collection (const Collection &other);
   Collection &   operator = (const Collection & other);
   bool           operator == (const Collection &other);
   bool           operator != (const Collection &other);

}; // class Collection



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/flip/Collection.hpp"



#endif   // ohm_flip_Collection_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
