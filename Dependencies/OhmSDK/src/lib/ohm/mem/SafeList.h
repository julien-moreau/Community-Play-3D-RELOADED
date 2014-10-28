/*****************************************************************************

        SafeList.h
        Copyright (c) 2011 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 64698

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



#if ! defined (ohm_mem_SafeList_HEADER_INCLUDED)
#define  ohm_mem_SafeList_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <list>
#include <stdexcept>



namespace ohm
{
namespace mem
{



template <typename T, typename Allocator = std::allocator <T> >
class SafeList
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef std::list <T, Allocator> ListBase; 
   typedef typename ListBase::value_type value_type;
   typedef typename ListBase::size_type size_type;
   typedef typename ListBase::difference_type difference_type;
   typedef typename ListBase::allocator_type allocator_type;
   typedef typename ListBase::pointer pointer;
   typedef typename ListBase::const_pointer const_pointer;
   typedef typename ListBase::reference reference;
   typedef typename ListBase::const_reference const_reference;
   
   class const_iterator;
   
   class iterator
   {
   public:
      typedef typename ListBase::iterator::difference_type difference_type;
      typedef typename ListBase::iterator::iterator_category iterator_category;
      typedef typename ListBase::iterator::value_type value_type;
      typedef typename ListBase::iterator::pointer pointer;
      typedef typename ListBase::iterator::reference reference;
      typedef iterator Self;
      
                  iterator ();
      virtual     ~iterator ();
            
      reference   operator * () const;
      pointer     operator -> () const;
      Self &      operator ++ ();
      Self        operator ++ (int);
      Self &      operator -- ();
      Self        operator -- (int);
      bool        operator == (const iterator & it) const;
      bool        operator != (const iterator & it) const;
               
   protected:
      friend class SafeList;
      friend class const_iterator;
      friend bool operator == (const iterator & left, const const_iterator & right);
      friend bool operator != (const iterator & left, const const_iterator & right);

                  iterator (const ListBase & list, const typename ListBase::iterator & it, const typename ListBase::iterator & it_end);
      void        throw_if_not_inited () const;
      void        throw_if_not_valid () const;
                  
   private:
      
                  
      const ListBase *  
                  _list_ptr;  // list is never accessed
      typename ListBase::iterator 
                  _it;
      typename ListBase::iterator 
                  _it_end;
   };
   
   class const_iterator
   {
   public:
      typedef typename ListBase::const_iterator::difference_type difference_type;
      typedef typename ListBase::const_iterator::iterator_category iterator_category;
      typedef typename ListBase::const_iterator::value_type value_type;
      typedef typename ListBase::const_iterator::pointer pointer;
      typedef typename ListBase::const_iterator::reference reference;
      typedef const_iterator Self;
   
                  const_iterator ();
                  const_iterator (const iterator & it);
      virtual     ~const_iterator ();
      
      reference   operator * () const;          
      pointer     operator -> () const;
      Self &      operator ++ ();
      Self        operator ++ (int);
      Self &      operator -- ();
      Self        operator -- (int);
      bool        operator == (const const_iterator & it) const;
      bool        operator != (const const_iterator & it) const;
               
   protected:
      friend class SafeList;
      friend bool operator == (const iterator & left, const const_iterator & right) { if ((left._list_ptr == 0) || (left._list_ptr != right._list_ptr)) {throw std::runtime_error ("operator == (SafeList::iterator, SafeList::const_iterator)");}; return (left._list_ptr == right._list_ptr) &&  (left._it == right._it) && (left._it_end == right._it_end);}
      friend bool operator != (const iterator & left, const const_iterator & right) { return !(left == right); }

                  const_iterator (const ListBase & list, const typename ListBase::const_iterator & it, const typename ListBase::const_iterator & it_end);
      void        throw_if_not_inited () const;
      void        throw_if_not_valid () const;
                  
   private:

      const ListBase *  
                  _list_ptr;  // list is never accessed
      typename ListBase::const_iterator 
                  _it;
      typename ListBase::const_iterator 
                  _it_end;
   };
   
   typedef std::reverse_iterator<const_iterator>      const_reverse_iterator;
   typedef std::reverse_iterator<iterator>            reverse_iterator;

   explicit       SafeList (const allocator_type & a = allocator_type ());
                  SafeList (size_type n, const value_type & val, const allocator_type & a = allocator_type ());
   explicit       SafeList (size_type n);
                  SafeList (const SafeList & other);
   template <typename InputIterator>
                  SafeList (InputIterator first, InputIterator last, const allocator_type & a = allocator_type ());
   virtual        ~SafeList ();

   SafeList &     operator = (const SafeList & other);
   void           assign (size_type n, const value_type & val);
   template <typename InputIterator>
   void           assign (InputIterator first, InputIterator last);
   allocator_type get_allocator () const;
   
   iterator       begin ();
   const_iterator begin () const;
   iterator       end ();
   const_iterator end () const;

   reverse_iterator
                  rbegin ();
   const_reverse_iterator
                  rbegin () const;
   reverse_iterator
                  rend ();
   const_reverse_iterator
                  rend () const;

   bool           empty () const;
   size_type      size () const;
   size_type      max_size () const;
   void           resize (size_type new_size, const value_type & val);
   void           resize (size_type new_size);
   
   reference      front ();
   const_reference
                  front () const;
   reference      back ();
   const_reference
                  back () const;
   void           push_front (const value_type & val);
   void           pop_front ();
   void           push_back (const value_type & val);
   void           pop_back ();

   iterator       insert (iterator pos, const value_type & val);
   void           insert (iterator pos, size_type n, const value_type & val);
   template <typename InputIterator>
   void           insert (iterator pos, InputIterator first, InputIterator last);
   
   iterator       erase (iterator pos);
   
   void           swap (SafeList & list);
   
   void           clear ();
   
   void           splice (iterator pos, SafeList & other);
   void           splice (iterator pos, SafeList & other, iterator i);
   void           splice (iterator pos, SafeList & other, iterator first, iterator last);
   
   void           remove (const value_type & val);
   template <typename Predicate>
   void           remove_if (Predicate predicate);
   
   void           unique ();
   template <typename BinaryPredicate>
   void           unique (BinaryPredicate predicate);
   
   void           merge (SafeList & list);
   template <typename StrictWeakOrdering>
   void           merge (SafeList & other, StrictWeakOrdering ordering);
   
   void           reverse ();
   void           sort ();
   template <typename StrictWeakOrdering>
   void           sort (StrictWeakOrdering ordering);
   
   

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   ListBase       _list;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const SafeList &other) const;
   bool           operator != (const SafeList &other) const;

}; // class SafeList




}  // namespace mem
}  // namespace ohm



#include "ohm/mem/SafeList.hpp"



#endif   // ohm_mem_SafeList_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

