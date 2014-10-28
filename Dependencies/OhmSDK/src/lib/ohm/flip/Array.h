/*****************************************************************************

        Array.h
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



#if ! defined (ohm_flip_Array_HEADER_INCLUDED)
#define  ohm_flip_Array_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/Object.h"
#include "ohm/flip/FindMask.h"
#include "ohm/flip/private/BinaryKey.h"
#include "ohm/flip/private/ArrayIteratorBase.h"
#include "ohm/flip/private/ArrayMaster.h"

#include <map>



namespace ohm
{
namespace flip
{



class ObjectMold;

template <class T>
class Array
:  public ArrayMaster
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef T value_type;
   typedef T & reference;
   typedef const T & const_reference;

private:
   class ObjectPtrState
   {
   public:
      enum State
      {
                  State_RESIDENT = 0,
                  State_ADDED,
                  State_REMOVED,
                  State_MOVED,
      };
      
      Object *    _object_ptr;
      State       _state;
   };
   typedef std::map <BinaryKey, ObjectPtrState> KeyPtrMap;
   
   typedef typename KeyPtrMap::iterator KeyPtrMap_iterator;
   typedef typename KeyPtrMap::const_iterator KeyPtrMap_const_iterator;
   typedef typename KeyPtrMap::reverse_iterator KeyPtrMap_reverse_iterator;
   typedef typename KeyPtrMap::const_reverse_iterator KeyPtrMap_const_reverse_iterator;

   friend class ArrayIteratorBase <T, KeyPtrMap_iterator>;
   friend class ArrayIteratorBase <T, KeyPtrMap_const_iterator>;
   friend class ArrayIteratorBase <T, KeyPtrMap_reverse_iterator>;
   friend class ArrayIteratorBase <T, KeyPtrMap_const_reverse_iterator>;
   
public:
   typedef ArrayIteratorBase <T, KeyPtrMap_iterator> iterator;
   typedef ArrayIteratorBase <T, KeyPtrMap_const_iterator> const_iterator;
   typedef ArrayIteratorBase <T, KeyPtrMap_reverse_iterator> reverse_iterator;
   typedef ArrayIteratorBase <T, KeyPtrMap_const_reverse_iterator> const_reverse_iterator;
   
   static void    declare ();
   
                  Array (DocumentBase & document);
   virtual        ~Array ();
   
   // Array accessors
   
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

   T &            operator [] (size_t index);
   const T &      operator [] (size_t index) const;

   T &            at (size_t index);
   const T &      at (size_t index) const;

   bool           empty (FindMask find_mask = FindMask_NATURAL) const;
   
   reference      front (FindMask find_mask = FindMask_NATURAL);
   const_reference
                  front (FindMask find_mask = FindMask_NATURAL) const;
   reference      back (FindMask find_mask = FindMask_NATURAL);
   const_reference
                  back (FindMask find_mask = FindMask_NATURAL) const;

   iterator       insert (iterator it, Args & args = Args::_none);
   template <class U>
   iterator       insert (iterator it, Args & args = Args::_none);
   
   void           erase (iterator it);
   void           erase (iterator begin, iterator end);
   virtual void   erase (flip::Ref ref);
   
   virtual void   clear ();
   
   iterator       move (iterator src_it, iterator dst_it);
   iterator       move (Array & src_array, iterator src_it, iterator dst_it);
   
   bool           did_value_change () const;
   bool           did_child_order_change () const;
   
   virtual void   synchronize ();
   virtual void   check_synchronized () const;
   virtual void   check_inner_invalidation_cnt (size_t invalid_cnt) const;

   iterator       paste (iterator it, ObjectMold & mold);
   iterator       paste (iterator it, ObjectMold & mold, RefRefArr & ref_ref_arr);

   // PasteBoard access
   virtual Ref    insert (iterator it, ClassDescriptionMaster & cdm);
   virtual Ref    push_back (ClassDescriptionMaster & cdm);
      


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   friend class VirtualMachine;
   friend class DocumentWriter;
   friend class DocumentBase;
   
   void           compute_key (BinaryKey & key, iterator it);
   void           array_changed ();
   void           array_changed_invalidate ();
   
   // VirtualMachine private access
   virtual int    insert_no_notification (const BinaryKey & key, Ref ref, int class_id, bool tolerant_flag);
   virtual int    erase_no_notification (const BinaryKey & key, Ref ref, bool tolerant_flag);
   virtual int    move_no_notification (ArrayMaster * dst_ptr, const BinaryKey & dst_key, const BinaryKey & src_key, bool tolerant_flag);
   
   // DocumentBase private access
   virtual int    broadcast_order_change ();
   
   // DocumentWriter private access
   virtual Ref    get_element_ref (size_t i) const;
   virtual const BinaryKey &
                  get_element_key (size_t i) const;
   virtual int    get_element_class_id () const;
   
   KeyPtrMap      _key_ptr_map;
   bool           _old_cur_equal_flag;
   bool           _child_order_changed_flag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Array ();
                  Array (const Array &other);
   Array &        operator = (const Array & other);
   bool           operator == (const Array &other);
   bool           operator != (const Array &other);

}; // class Array



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/flip/Array.hpp"



#endif   // ohm_flip_Array_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
