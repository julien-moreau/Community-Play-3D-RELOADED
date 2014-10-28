/*****************************************************************************

        Vector.hpp
        Copyright (c) 2007 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 18042

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



#if defined (ohm_mem_mmgr_Vector_CURRENT_CODEHEADER)
   #error Recursive inclusion of Vector code header.
#endif
#define  ohm_mem_mmgr_Vector_CURRENT_CODEHEADER

#if ! defined (ohm_mem_mmgr_Vector_CODEHEADER_INCLUDED)
#define  ohm_mem_mmgr_Vector_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/mem/mmgr/AllocTools.h"

#include <algorithm>

#include <cassert>



namespace ohm
{
namespace mem
{
namespace mmgr
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <class T>
Vector <T>::Vector ()
:  _mem_mgr_ptr (0)
,  _data_ptr (0)
,  _len (0)
,  _capacity (0)
{
   assert (! is_valid ());
}



template <class T>
Vector <T>::Vector (MemMgrInterface &mem_mgr)
:  _mem_mgr_ptr (&mem_mgr)
,  _data_ptr (0)
,  _len (0)
,  _capacity (0)
{
   assert (&mem_mgr != 0);

   assert (is_valid ());
}



template <class T>
Vector <T>::Vector (MemMgrInterface &mem_mgr, long size)
:  _mem_mgr_ptr (&mem_mgr)
,  _data_ptr (0)
,  _len (0)
,  _capacity (0)
{
   assert (&mem_mgr != 0);
   assert (size >= 0);

   assert (is_valid ());

   reserve (size);
   try
   {
      resize (size);
   }
   catch (...)
   {
      kill ();
      throw;
   }
}



template <class T>
Vector <T>::Vector (MemMgrInterface &mem_mgr, long size, long capacity)
:  _mem_mgr_ptr (&mem_mgr)
,  _data_ptr (0)
,  _len (0)
,  _capacity (0)
{
   assert (&mem_mgr != 0);
   assert (size >= 0);
   assert (capacity >= 0);
   assert (size <= capacity);

   assert (is_valid ());

   reserve (capacity);
   try
   {
      resize (size);
   }
   catch (...)
   {
      kill ();
      throw;
   }
}



template <class T>
Vector <T>::Vector (const Vector <T> &other)
:  _mem_mgr_ptr (other._mem_mgr_ptr)
,  _data_ptr (0)
,  _len (0)
,  _capacity (0)
{
   assert (&other != 0);

   if (other._capacity > 0)
   {
      assert (other.is_valid ());
      reserve (other._capacity);

      if (other._len > 0)
      {
         try
         {
            AllocTools <T>::copy_zone (
               _data_ptr,
               other._data_ptr,
               other._len
            );
         }
         catch (...)
         {
            kill ();
            throw;
         }

         _len = other._len;
      }
   }
}



template <class T>
Vector <T>::~Vector ()
{
   kill ();
}



// If both objects have initialised memory managers, it is not specified if
// the memory manager is taken from the copied object or not.
template <class T>
Vector <T> &   Vector <T>::operator = (const Vector <T> &other)
{
   assert (&other != 0);

   if (&other != this)
   {
      if (other._capacity == 0)
      {
         kill ();
         _mem_mgr_ptr = other._mem_mgr_ptr;
      }

      else
      {
         // New allocation and copy
         if (_capacity < other._capacity)
         {
            reserve_and_copy (
               other._capacity,
               *other._mem_mgr_ptr,
               other._data_ptr,
               other._len
            );
            _mem_mgr_ptr = other._mem_mgr_ptr;
         }

         // Just a copy
         // Warning: in case of exception during the copy-construction, the
         // content is erased.
         else
         {
            assert (other._len <= _capacity);
            clear ();

            AllocTools <T>::copy_zone (
               _data_ptr,
               other._data_ptr,
               other._len
            );
            _len = other._len;
         }
      }
   }

   return (*this);
}



template <class T>
Vector <T> &   Vector <T>::swap (const Vector <T> &other)
{
   assert (&other != 0);

   std::swap (_mem_mgr_ptr, other._mem_mgr_ptr);
   std::swap (_data_ptr, other._data_ptr);
   std::swap (_len, other._len);
   std::swap (_capacity, other._capacity);

   return (*this);
}



template <class T>
void  Vector <T>::set_mem_mgr (MemMgrInterface &mem_mgr)
{
   assert (_mem_mgr_ptr == 0 || _capacity == 0);
   assert (&mem_mgr != 0);

   _mem_mgr_ptr = &mem_mgr;
}



template <class T>
MemMgrInterface & Vector <T>::use_mem_mgr () const
{
   assert (is_valid ());

   return (*_mem_mgr_ptr);
}



template <class T>
bool  Vector <T>::is_valid () const
{
   return (_mem_mgr_ptr != 0);
}



template <class T>
long  Vector <T>::get_capacity () const
{
   assert (is_valid ());

   return (_capacity);
}



template <class T>
long  Vector <T>::capacity () const
{
   return (get_capacity ());
}



template <class T>
void  Vector <T>::kill ()
{
   if (_len > 0)
   {
      clear ();
   }

   if (_capacity > 0)
   {
      _mem_mgr_ptr->deallocate_bytes (_data_ptr);
      _data_ptr = 0;
      _capacity = 0;
   }
}



template <class T>
void  Vector <T>::reserve (long nbr_elt)
{
   assert (is_valid ());
   assert (nbr_elt >= 0);

   reserve_and_copy (nbr_elt, *_mem_mgr_ptr, _data_ptr, _len);
}



template <class T>
long  Vector <T>::get_size () const
{
   assert (is_valid ());

   return (_len);
}



template <class T>
long  Vector <T>::size () const
{
   return (get_size ());
}



template <class T>
void  Vector <T>::clear ()
{
   assert (is_valid ());

   if (_len > 0)
   {
      AllocTools <T>::delete_zone (_data_ptr, _len);
      _len = 0;
   }
}



template <class T>
void  Vector <T>::resize (long nbr_elt)
{
   assert (is_valid ());
   assert (nbr_elt >= 0);

   resize_internal (nbr_elt, 0);
}



template <class T>
void  Vector <T>::push_back (const T &val)
{
   assert (is_valid ());
   assert (&val != 0);

   const long     old_nbr_elt = get_size ();
   resize_internal (old_nbr_elt + 1, &val);
}



template <class T>
T &   Vector <T>::use_back ()
{
   return (operator [] (get_size () - 1));
}



template <class T>
const T &   Vector <T>::use_back () const
{
   return (operator [] (get_size () - 1));
}



template <class T>
T &   Vector <T>::back ()
{
   return (use_back ());
}



template <class T>
const T &   Vector <T>::back () const
{
   return (use_back ());
}



template <class T>
T &   Vector <T>::operator [] (long pos)
{
   assert (is_valid ());
   assert (pos >= 0);
   assert (pos < _len);

   return (_data_ptr [pos]);
}



template <class T>
const T &   Vector <T>::operator [] (long pos) const
{
   assert (is_valid ());
   assert (pos >= 0);
   assert (pos < _len);

   return (_data_ptr [pos]);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <class T>
void  Vector <T>::reserve_and_copy (long new_capacity, MemMgrInterface &mem_mgr, T src_ptr [], long copy_len)
{
   assert (is_valid ());
   assert (new_capacity >= 0);
   assert (&mem_mgr != 0);
   assert (src_ptr != 0 || copy_len == 0);
   assert (copy_len == _len || src_ptr != _data_ptr);
   assert (copy_len >= 0);

   if (new_capacity > _capacity)
   {
      T *            zone_ptr = AllocTools <T>::allocate_mem (
         new_capacity,
         mem_mgr
      );

      if (copy_len > 0)
      {
         try
         {
            AllocTools <T>::copy_zone (zone_ptr, src_ptr, copy_len);
         }
         catch (...)
         {
            mem_mgr.deallocate_bytes (zone_ptr);
            throw;
         }
      }

      // Copy has been done, subsequent assignments are secure, we can now
      // safely destroy the current oject.
      kill ();

      _data_ptr = zone_ptr;
      _capacity = new_capacity;
      _len = copy_len;
   }
}



template <class T>
void  Vector <T>::resize_internal (long nbr_elt, const T new_elt_arr [])
{
   assert (is_valid ());
   assert (nbr_elt >= 0);

   if (nbr_elt > _capacity)
   {
      // x1.5 growth factor should be good.
      long        new_cap = nbr_elt;
      new_cap += (nbr_elt + 1) >> 1;   // !! May overflow, should be checked
      assert (new_cap >= nbr_elt);

      reserve (new_cap);
   }

   if (nbr_elt > _len)
   {
      const long     nbr_new_elt = nbr_elt - _len;
      T *            new_zone_ptr = _data_ptr + _len;
      if (new_elt_arr != 0)
      {
         AllocTools <T>::copy_zone (new_zone_ptr, new_elt_arr, nbr_new_elt);
      }
      else
      {
         AllocTools <T>::init_zone (new_zone_ptr, nbr_new_elt);
      }
   }

   else if (nbr_elt < _len)
   {
      AllocTools <T>::delete_zone (_data_ptr + nbr_elt, _len - nbr_elt);
   }

   _len = nbr_elt;
}



}  // namespace mmgr
}  // namespace mem
}  // namespace ohm



#endif   // ohm_mem_mmgr_Vector_CODEHEADER_INCLUDED

#undef ohm_mem_mmgr_Vector_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
