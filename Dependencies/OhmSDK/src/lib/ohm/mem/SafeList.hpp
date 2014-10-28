/*****************************************************************************

        SafeList.hpp
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



#if defined (ohm_mem_SafeList_CURRENT_CODEHEADER)
   #error Recursive inclusion of SafeList code header.
#endif
#define  ohm_mem_SafeList_CURRENT_CODEHEADER

#if ! defined (ohm_mem_SafeList_CODEHEADER_INCLUDED)
#define  ohm_mem_SafeList_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/util/trace.h"

#include <cassert>
#include <stdexcept>

#undef ohm_mem_SafeList_ASSERT_ON_THROW
#define ohm_mem_SafeList_LOG_ON_THROW

#if defined ohm_mem_SafeList_ASSERT_ON_THROW
   #define omsl_break assert (false)
#else
   #define omsl_break ((void)0)
#endif

#if defined ohm_mem_SafeList_LOG_ON_THROW
   #define omsl_TRACE(msg_0)  ohm_util_TRACE_WARNING(msg_0)
   #define omsl_TRACE1(fmt_0, arg1) ohm_util_TRACE_WARNING1(fmt_0, arg1)
   #define omsl_TRACE2(fmt_0, arg1, arg2) ohm_util_TRACE_WARNING2(fmt_0, arg1, arg2)
   #define omsl_TRACE3(fmt_0, arg1, arg2, arg3) ohm_util_TRACE_WARNING3(fmt_0, arg1, arg2, arg3)
   #define omsl_TRACE4(fmt_0, arg1, arg2, arg3, arg4) ohm_util_TRACE_WARNING4(fmt_0, arg1, arg2, arg3, arg4)
#else
   #define omsl_TRACE(msg_0)  (void (0))
   #define omsl_TRACE1(fmt_0, arg1) (void (0))
   #define omsl_TRACE2(fmt_0, arg1, arg2) (void (0))
   #define omsl_TRACE3(fmt_0, arg1, arg2, arg3) (void (0))
   #define omsl_TRACE4(fmt_0, arg1, arg2, arg3, arg4) (void (0))
#endif


namespace ohm
{
namespace mem
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: SafeList
==============================================================================
*/

template <typename T, typename Alloc>
SafeList <T, Alloc>::SafeList (const allocator_type & a)
:  _list (a)
{
}



/*
==============================================================================
Name: SafeList
==============================================================================
*/

template <typename T, typename Alloc>
SafeList <T, Alloc>::SafeList (size_type n, const value_type & val, const allocator_type & a)
:  _list (n, val, a)
{
}



/*
==============================================================================
Name: SafeList
==============================================================================
*/

template <typename T, typename Alloc>
SafeList <T, Alloc>::SafeList (size_type n)
:  _list (n)
{
}



/*
==============================================================================
Name: SafeList
==============================================================================
*/

template <typename T, typename Alloc>
SafeList <T, Alloc>::SafeList (const SafeList <T, Alloc> & other)
:  _list (other._list)
{
}



/*
==============================================================================
Name: SafeList
==============================================================================
*/

template <typename T, typename Alloc>
template <typename InputIterator> 
SafeList <T, Alloc>::SafeList (InputIterator first, InputIterator last, const allocator_type & a)
:  _list (first, last, a)
{
}



/*
==============================================================================
Name: ~SafeList
==============================================================================
*/

template <typename T, typename Alloc>
SafeList <T, Alloc>::~SafeList ()
{
}



/*
==============================================================================
Name: operator =
==============================================================================
*/

template <typename T, typename Alloc>
SafeList <T, Alloc> & SafeList <T, Alloc>::operator = (const SafeList <T, Alloc> & other)
{
   _list = other._list;
   
   return *this;
}



/*
==============================================================================
Name: assign
==============================================================================
*/

template <typename T, typename Alloc>
void SafeList <T, Alloc>::assign (size_type n, const value_type & val)
{
   _list.assign (n, val);
}



/*
==============================================================================
Name: assign
==============================================================================
*/

template <typename T, typename Alloc>
template <typename InputIterator> 
void SafeList <T, Alloc>::assign (InputIterator first, InputIterator last)
{
   _list.assign (first, last);
}



/*
==============================================================================
Name: get_allocator
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::allocator_type SafeList <T, Alloc>::get_allocator () const
{
   return _list.get_allocator ();
}



/*
==============================================================================
Name: begin
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::iterator SafeList <T, Alloc>::begin ()
{
   return iterator (this->_list, _list.begin (), _list.end ());
}



/*
==============================================================================
Name: begin
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::const_iterator SafeList <T, Alloc>::begin () const
{
   return const_iterator (this->_list, _list.begin (), _list.end ());
}



/*
==============================================================================
Name: end
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::iterator SafeList <T, Alloc>::end ()
{
   return iterator (this->_list, _list.end (), _list.end ());
}



/*
==============================================================================
Name: end
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::const_iterator SafeList <T, Alloc>::end () const
{
   return const_iterator (this->_list, _list.end (), _list.end ());
}



/*
==============================================================================
Name: rbegin
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::reverse_iterator SafeList <T, Alloc>::rbegin ()
{
   return reverse_iterator (end ());
}



/*
==============================================================================
Name: rbegin
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::const_reverse_iterator SafeList <T, Alloc>::rbegin () const
{
   return const_reverse_iterator (end ());
}



/*
==============================================================================
Name: rend
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::reverse_iterator SafeList <T, Alloc>::rend ()
{
   return reverse_iterator (begin ());
}



/*
==============================================================================
Name: rend
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::const_reverse_iterator SafeList <T, Alloc>::rend () const
{
   return const_reverse_iterator (begin ());
}



/*
==============================================================================
Name: empty
==============================================================================
*/

template <typename T, typename Alloc>
bool SafeList <T, Alloc>::empty () const
{
   return _list.empty ();
}



/*
==============================================================================
Name: size
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::size_type SafeList <T, Alloc>::size () const
{
   return _list.size ();
}



/*
==============================================================================
Name: max_size
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::size_type SafeList <T, Alloc>::max_size () const
{
   return _list._max_size;
}



/*
==============================================================================
Name: resize
==============================================================================
*/

template <typename T, typename Alloc>
void SafeList <T, Alloc>::resize (size_type new_size, const value_type & val)
{
   return _list.resize (new_size, val);
}



/*
==============================================================================
Name: resize
==============================================================================
*/

template <typename T, typename Alloc>
void SafeList <T, Alloc>::resize (size_type new_size)
{
   return _list.resize (new_size);
}



/*
==============================================================================
Name: front
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::reference SafeList <T, Alloc>::front ()
{
   return _list.front ();
}



/*
==============================================================================
Name: front
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::const_reference SafeList <T, Alloc>::front () const
{
   return _list.front ();
}



/*
==============================================================================
Name: back
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::reference SafeList <T, Alloc>::back ()
{
   return _list.back ();
}



/*
==============================================================================
Name: back
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::const_reference SafeList <T, Alloc>::back () const
{
   return _list.back ();
}



/*
==============================================================================
Name: push_front
==============================================================================
*/

template <typename T, typename Alloc>
void SafeList <T, Alloc>::push_front (const value_type & val)
{
   _list.push_front (val);
}



/*
==============================================================================
Name: pop_front
==============================================================================
*/

template <typename T, typename Alloc>
void SafeList <T, Alloc>::pop_front ()
{
   _list.pop_front ();
}



/*
==============================================================================
Name: push_back
==============================================================================
*/

template <typename T, typename Alloc>
void SafeList <T, Alloc>::push_back (const value_type & val)
{
   _list.push_back (val);
}



/*
==============================================================================
Name: pop_back
==============================================================================
*/

template <typename T, typename Alloc>
void SafeList <T, Alloc>::pop_back ()
{
   _list.pop_back ();
}



/*
==============================================================================
Name: insert
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::iterator SafeList <T, Alloc>::insert (iterator pos, const value_type & val)
{
   pos.throw_if_not_inited ();

   typename ListBase::iterator it = _list.insert (pos._it, val);
   
   return iterator (this->_list, it, _list.end ());
}



/*
==============================================================================
Name: insert
==============================================================================
*/

template <typename T, typename Alloc>
void SafeList <T, Alloc>::insert (iterator pos, size_type n, const value_type & val)
{
   pos.throw_if_not_inited ();

   _list.insert (pos._it, n, val);
}



/*
==============================================================================
Name: insert
==============================================================================
*/

template <typename T, typename Alloc>
template <typename InputIterator> 
void SafeList <T, Alloc>::insert (iterator pos, InputIterator first, InputIterator last)
{
   pos.throw_if_not_inited ();

   _list.insert (pos._it, first, last);
}



/*
==============================================================================
Name: erase
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::iterator SafeList <T, Alloc>::erase (iterator pos)
{
   pos.throw_if_not_inited ();

   typename ListBase::iterator lit = _list.erase (pos._it);
   
   return iterator (this->_list, lit, _list.end ());
}



/*
==============================================================================
Name: swap
==============================================================================
*/

template <typename T, typename Alloc>
void SafeList <T, Alloc>::swap (SafeList <T, Alloc> & list)
{
   _list.swap (list._list);
}



/*
==============================================================================
Name: clear
==============================================================================
*/

template <typename T, typename Alloc>
void SafeList <T, Alloc>::clear ()
{
   _list.clear ();
}



/*
==============================================================================
Name: splice
==============================================================================
*/

template <typename T, typename Alloc>
void SafeList <T, Alloc>::splice (iterator pos, SafeList <T, Alloc> & other)
{
   pos.throw_if_not_inited ();

   _list.splice (pos._it, other._list);
}



/*
==============================================================================
Name: splice
==============================================================================
*/

template <typename T, typename Alloc>
void SafeList <T, Alloc>::splice (iterator pos, SafeList <T, Alloc> & other, iterator i)
{
   pos.throw_if_not_inited ();
   i.throw_if_not_inited ();
   
   _list.splice (pos._it, other._list, i._it);
}



/*
==============================================================================
Name: splice
==============================================================================
*/

template <typename T, typename Alloc>
void SafeList <T, Alloc>::splice (iterator pos, SafeList <T, Alloc> & other, iterator first, iterator last)
{
   pos.throw_if_not_inited ();
   first.throw_if_not_inited ();
   last.throw_if_not_inited ();
   
   _list.splice (pos._it, other._list, first._it, last._it);
}



/*
==============================================================================
Name: remove
==============================================================================
*/

template <typename T, typename Alloc>
void SafeList <T, Alloc>::remove (const value_type & val)
{
   _list.remove (val);
}



/*
==============================================================================
Name: remove_if
==============================================================================
*/

template <typename T, typename Alloc>
template <typename Predicate> 
void SafeList <T, Alloc>::remove_if (Predicate predicate)
{
   _list.remove_if (predicate);
}



/*
==============================================================================
Name: unique
==============================================================================
*/

template <typename T, typename Alloc>
void SafeList <T, Alloc>::unique ()
{
   _list.unique ();
}



/*
==============================================================================
Name: unique
==============================================================================
*/

template <typename T, typename Alloc>
template <typename BinaryPredicate> 
void SafeList <T, Alloc>::unique (BinaryPredicate predicate)
{
   _list.unique (predicate);
}



/*
==============================================================================
Name: merge
==============================================================================
*/

template <typename T, typename Alloc>
void SafeList <T, Alloc>::merge (SafeList <T, Alloc> & list)
{
   _list.merge (list._list);
}



/*
==============================================================================
Name: merge
==============================================================================
*/

template <typename T, typename Alloc>
template <typename StrictWeakOrdering> 
void SafeList <T, Alloc>::merge (SafeList <T, Alloc> & other, StrictWeakOrdering ordering)
{
   _list.merge (other._list, ordering);
}



/*
==============================================================================
Name: reverse
==============================================================================
*/

template <typename T, typename Alloc>
void SafeList <T, Alloc>::reverse ()
{
   _list.reverse ();
}



/*
==============================================================================
Name: sort
==============================================================================
*/

template <typename T, typename Alloc>
void SafeList <T, Alloc>::sort ()
{
   _list.sort ();
}



/*
==============================================================================
Name: sort
==============================================================================
*/

template <typename T, typename Alloc>
template <typename StrictWeakOrdering>
void SafeList <T, Alloc>::sort (StrictWeakOrdering ordering)
{
   _list.sort (ordering);
}



/*
==============================================================================
Name: iterator
==============================================================================
*/

template <typename T, typename Alloc>
SafeList <T, Alloc>::iterator::iterator ()
:  _list_ptr (0)
,  _it ()
,  _it_end ()
{
}



/*
==============================================================================
Name: ~iterator
==============================================================================
*/

template <typename T, typename Alloc>
SafeList <T, Alloc>::iterator::~iterator ()
{
}



/*
==============================================================================
Name: operator *
Note: will throw if not inited or internal iterator is equal to list end ()
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::iterator::reference
SafeList <T, Alloc>::iterator::operator * () const
{
   throw_if_not_valid ();
   
   return *_it;
}



/*
==============================================================================
Name: operator ->
Note: will throw if not valid. 
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::iterator::pointer 
SafeList <T, Alloc>::iterator::operator -> () const
{
// return &(*_it);   // would not throw
   return &(operator * ());
}



/*
==============================================================================
Name: operator ++
Note: will throw if the iterator is not inited
=============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::iterator::Self & 
SafeList <T, Alloc>::iterator::operator ++ ()
{
// throw_if_not_valid ();
   throw_if_not_inited ();

   ++_it;

   return *this;
}



/*
==============================================================================
Name: operator ++
Note: will throw if the iterator is not inited
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::iterator::Self 
SafeList <T, Alloc>::iterator::operator ++ (int)
{
   iterator it = *this;
   
   this->operator ++ ();
   
   return it;
}



/*
==============================================================================
Name: operator --
Note: will throw if the iterator is not inited
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::iterator::Self & 
SafeList <T, Alloc>::iterator::operator -- ()
{
// throw_if_not_valid ();
   throw_if_not_inited ();

   --_it;

   return *this;
}



/*
==============================================================================
Name: operator --
Note: will throw if the iterator is not not inited
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::iterator::Self 
SafeList <T, Alloc>::iterator::operator -- (int)
{
   iterator it = *this;
   
   this->operator -- ();
   
   return it;
}



/*
==============================================================================
Name: operator ==
==============================================================================
*/

template <typename T, typename Alloc>
bool SafeList <T, Alloc>::iterator::operator == (const iterator & it) const
{
   // throw if not inited or iterator from different lists
   
   if ((_list_ptr == 0) || (_list_ptr != it._list_ptr))
   {
      omsl_break;
      omsl_TRACE ("SafeList::iterator operator ==");
      throw std::runtime_error ("SafeList::iterator operator ==");
   }
   
   return 
      (_list_ptr == it._list_ptr) &&
      (_it == it._it) &&
      (_it_end == it._it_end);
}



/*
==============================================================================
Name: operator !=
==============================================================================
*/

template <typename T, typename Alloc>
bool SafeList <T, Alloc>::iterator::operator != (const iterator & it) const
{
   return ! ( (*this) == it );
}



/*
==============================================================================
Name: const_iterator
==============================================================================
*/

template <typename T, typename Alloc>
SafeList <T, Alloc>::const_iterator::const_iterator ()
:  _list_ptr (0)
,  _it ()
,  _it_end ()
{
}



/*
==============================================================================
Name: const_iterator
==============================================================================
*/

template <typename T, typename Alloc>
SafeList <T, Alloc>::const_iterator::const_iterator (const iterator & it)
:  _list_ptr (it._list_ptr)
,  _it (it._it)
,  _it_end (it._it_end)
{
}



/*
==============================================================================
Name: ~const_iterator
==============================================================================
*/

template <typename T, typename Alloc>
SafeList <T, Alloc>::const_iterator::~const_iterator ()
{
}



/*
==============================================================================
Name: operator *
Note: will throw if not inited or internal iteraotr is equal to list end ()
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::const_iterator::reference
SafeList <T, Alloc>::const_iterator::operator * () const
{
   throw_if_not_valid ();
   
   return *_it;
}



/*
==============================================================================
Name: operator ->
Note: will not throw
Note: the const_reverse_iterator::operator -> can throw (calls operator --)
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::const_iterator::pointer 
SafeList <T, Alloc>::const_iterator::operator -> () const
{
   return &(*_it);
}



/*
==============================================================================
Name: operator ++
Note: will throw if the iterator is not valid (not inited, or end ())
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::const_iterator::Self & 
SafeList <T, Alloc>::const_iterator::operator ++ ()
{
// throw_if_not_valid ();
   throw_if_not_inited ();

   ++_it;

   return *this;
}



/*
==============================================================================
Name: operator ++
Note: will throw if the iterator is not inited
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::const_iterator::Self 
SafeList <T, Alloc>::const_iterator::operator ++ (int)
{
   const_iterator it = *this;
   
   this->operator ++ ();
   
   return it;
}



/*
==============================================================================
Name: operator --
Note: will throw if the iterator is not inited
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::const_iterator::Self & 
SafeList <T, Alloc>::const_iterator::operator -- ()
{
// throw_if_not_valid ();
   throw_if_not_inited ();

   --_it;

   return *this;
}



/*
==============================================================================
Name: operator --
Note: will throw if the iterator is not inited
==============================================================================
*/

template <typename T, typename Alloc>
typename SafeList <T, Alloc>::const_iterator::Self 
SafeList <T, Alloc>::const_iterator::operator -- (int)
{
   const_iterator it = *this;
   
   this->operator -- ();
   
   return it;
}



/*
==============================================================================
Name: operator ==
==============================================================================
*/

template <typename T, typename Alloc>
bool SafeList <T, Alloc>::const_iterator::operator == (const const_iterator & it) const
{
   // throw if not inited or iterator from different lists
   
   if ((_list_ptr == 0) || (_list_ptr != it._list_ptr))
   {
      omsl_break;
      omsl_TRACE ("SafeList::const_iterator operator ==");
      throw std::runtime_error ("SafeList::const_iterator operator ==");
   }
   
   return 
      (_list_ptr == it._list_ptr) &&
      (_it == it._it) &&
      (_it_end == it._it_end);
}



/*
==============================================================================
Name: operator !=
==============================================================================
*/

template <typename T, typename Alloc>
bool SafeList <T, Alloc>::const_iterator::operator != (const const_iterator & it) const
{
   return ! ( (*this) == it );
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: iterator
==============================================================================
*/

template <typename T, typename Alloc>
SafeList <T, Alloc>::iterator::iterator (const ListBase & list, const typename ListBase::iterator & it, const typename ListBase::iterator & it_end)
:  _list_ptr (&list)
,  _it (it)
,  _it_end (it_end)
{
}



/*
==============================================================================
Name: const_iterator
==============================================================================
*/

template <typename T, typename Alloc>
SafeList <T, Alloc>::const_iterator::const_iterator (const ListBase & list, const typename ListBase::const_iterator & it, const typename ListBase::const_iterator & it_end)
:  _list_ptr (&list)
,  _it (it)
,  _it_end (it_end)
{
}



/*
==============================================================================
Name: throw_if_not_inited
==============================================================================
*/

template <typename T, typename Alloc>
void SafeList <T, Alloc>::iterator::throw_if_not_inited () const
{
   if (_list_ptr == 0)
   {
      omsl_break;
      omsl_TRACE ("SafeList::iterator is not inited");
      throw std::runtime_error ("SafeList::iterator is not inited");
   }
}



/*
==============================================================================
Name: throw_if_not_valid
==============================================================================
*/

template <typename T, typename Alloc>
void SafeList <T, Alloc>::iterator::throw_if_not_valid () const
{
   bool throw_flag = true;
   
   if (_list_ptr != 0)
   {
      if (_it != _it_end)
      {
         T * data_ptr = &(*_it);
         
         if (data_ptr != 0)
         {
            throw_flag = false;
         }
      }
   }
   
   if (throw_flag)
   {
      omsl_break;
      omsl_TRACE ("SafeList::iterator is not valid");
      throw std::runtime_error ("SafeList::iterator is not valid");
   }
}



/*
==============================================================================
Name: throw_if_not_inited
==============================================================================
*/

template <typename T, typename Alloc>
void SafeList <T, Alloc>::const_iterator::throw_if_not_inited () const
{
   if (_list_ptr == 0)
   {
      omsl_break;
      omsl_TRACE ("SafeList::const_iterator is not inited");
      throw std::runtime_error ("SafeList::const_iterator is not inited");
   }
}



/*
==============================================================================
Name: throw_if_not_valid
==============================================================================
*/

template <typename T, typename Alloc>
void SafeList <T, Alloc>::const_iterator::throw_if_not_valid () const
{
   bool throw_flag = true;
   
   if (_list_ptr != 0)
   {
      if (_it != _it_end)
      {
         const T * data_ptr = &(*_it);
         
         if (data_ptr != 0)
         {
            throw_flag = false;
         }
      }
   }
   
   if (throw_flag)
   {
      omsl_break;
      omsl_TRACE ("SafeList::const_iterator is not valid");
      throw std::runtime_error ("SafeList::const_iterator is not valid");
   }
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace mem
}  // namespace ohm



#endif   // ohm_mem_SafeList_CODEHEADER_INCLUDED

#undef ohm_mem_SafeList_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

