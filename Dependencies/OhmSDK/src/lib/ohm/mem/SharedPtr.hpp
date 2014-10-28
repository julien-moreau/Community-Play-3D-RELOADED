/*****************************************************************************

        SharedPtr.hpp
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 70449

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



#if defined (ohm_mem_SharedPtr_CURRENT_CODEHEADER)
   #error Recursive inclusion of SharedPtr code header.
#endif
#define  ohm_mem_SharedPtr_CURRENT_CODEHEADER

#if ! defined (ohm_mem_SharedPtr_CODEHEADER_INCLUDED)
#define  ohm_mem_SharedPtr_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/conc/AioAdd.h"
#include "ohm/conc/AtomicIntOp.h"

#include <cassert>



namespace ohm
{
namespace mem
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
SharedPtr <T>::SharedPtr ()
:  _obj_ptr (0)
,  _count_ptr (0)
{
   // Nothing
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
SharedPtr <T>::SharedPtr (T *ptr)
:  _obj_ptr (ptr)
,  _count_ptr ((ptr != 0) ? new conc::AtomicInt <long> (1) : 0)
{
   // Nothing
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
SharedPtr <T>::SharedPtr (const SharedPtr <T> &other)
:  _obj_ptr (other._obj_ptr)
,  _count_ptr (other._count_ptr)
{
   assert (other.is_valid ());
   add_ref ();
   assert (is_consistent (other));
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class T>
SharedPtr <T>::~SharedPtr ()
{
   assert (is_valid ());

   destroy_complete ();
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T>
SharedPtr <T> &   SharedPtr <T>::operator = (const SharedPtr <T> &other)
{
   assert (is_valid ());
   assert (is_consistent (other));

   if (other._obj_ptr != _obj_ptr)
   {
      destroy_complete ();
      _obj_ptr = other._obj_ptr;
      _count_ptr = other._count_ptr;
      add_ref ();
   }
   assert (is_consistent (other));

   return (*this);
}



/*
==============================================================================
Name : swap
==============================================================================
*/

template <class T>
void  SharedPtr <T>::swap (SharedPtr <T> &other)
{
   assert (&other != 0);

   DataType *                 tmp_obj_ptr   = _obj_ptr;
   conc::AtomicInt <long> *   tmp_count_ptr = _count_ptr;

   _obj_ptr = other._obj_ptr;
   _count_ptr = other._count_ptr;

   other._obj_ptr = tmp_obj_ptr;
   other._count_ptr = tmp_count_ptr;
}


/*
==============================================================================
Name : assign_static
==============================================================================
*/

template <class T>
template <class U>
void  SharedPtr <T>::assign_static (const SharedPtr <U> & other)
{
   assert (&other != 0);
   
   operator = (
      shared_static_cast <T, U> (other)
   );
}



/*
==============================================================================
Name : assign_dynamic
==============================================================================
*/

template <class T>
template <class U>
void  SharedPtr <T>::assign_dynamic (const SharedPtr <U> & other)
{
   assert (&other != 0);
   
   operator = (
      shared_dynamic_cast <T, U> (other)
   );
}



/*
==============================================================================
Name : assign_reinterpret
==============================================================================
*/

template <class T>
template <class U>
void  SharedPtr <T>::assign_reinterpret (const SharedPtr <U> & other)
{
   assert (&other != 0);
   
   operator = (
      shared_reinterpret_cast <T, U> (other)
   );
}



/*
==============================================================================
Name : operator SharedPtr <const T>
==============================================================================
*/

template <class T>
SharedPtr <T>::operator SharedPtr <const T> () const 
{
   return (reinterpret_cast <const SharedPtr<const T> &> (*this));
}



/*
==============================================================================
Name : get
Description :
   Returns the pointer hold by the shared pointer.
   Return value may be null.
==============================================================================
*/

template <class T>
T  *SharedPtr <T>::get () const
{
   return (_obj_ptr);
}



/*
==============================================================================
Name : operator ->
==============================================================================
*/

template <class T>
T  *SharedPtr <T>::operator -> () const
{
   assert (_obj_ptr != 0);
   
   return (_obj_ptr);
}



/*
==============================================================================
Name : operator *
==============================================================================
*/

template <class T>
T  &SharedPtr <T>::operator * () const
{
   assert (_obj_ptr != 0);
   
   return (*_obj_ptr);
}



/*
==============================================================================
Name : destroy
==============================================================================
*/

template <class T>
void  SharedPtr <T>::destroy ()
{
   assert (is_valid ());

   destroy_complete ();
   _obj_ptr = 0;
   _count_ptr = 0;
}



/*
==============================================================================
Name : get_count
==============================================================================
*/

template <class T>
long  SharedPtr <T>::get_count () const
{
   assert (is_valid ());

   long           count = 0;
   if (_count_ptr != 0)
   {
      count = *_count_ptr;
      assert (count > 0);
   }

   return (count);
}



/*
==============================================================================
Name : is_valid
==============================================================================
*/

template <class T>
bool  SharedPtr <T>::is_valid () const
{
   return (   (   (_obj_ptr == 0 && _count_ptr == 0)
               || (_obj_ptr != 0 && _count_ptr != 0))
           && (_count_ptr == 0 || (*_count_ptr) > 0));
}



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
SharedPtr <T>::SharedPtr (T *other_ptr, conc::AtomicInt <long> *count_ptr)
:  _obj_ptr (other_ptr)
,  _count_ptr (count_ptr)
{
   assert (is_valid ());
   add_ref ();
}



/*
==============================================================================
Name : get_counter_ref
==============================================================================
*/

template <class T>
conc::AtomicInt <long> *   SharedPtr <T>::get_counter_ref () const
{
   return (_count_ptr);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : add_ref
==============================================================================
*/

template <class T>
void  SharedPtr <T>::add_ref ()
{
   if (_count_ptr != 0)
   {
      //++ *_count_ptr;
      
      const conc::AioAdd <long>  ftor_inc (1);
      conc::AtomicIntOp_exec (*_count_ptr, ftor_inc);
   }
}



/*
==============================================================================
Name : is_consistent
==============================================================================
*/

template <class T>
bool  SharedPtr <T>::is_consistent (const SharedPtr <T> &other) const
{
   assert (&other != 0);

   return (   other.is_valid ()
           && (   ((_obj_ptr == other._obj_ptr) && (_count_ptr == other._count_ptr))
               || ((_obj_ptr != other._obj_ptr) && (_count_ptr != other._count_ptr))));
}



/*
==============================================================================
Name : destroy_complete
==============================================================================
*/

template <class T>
void  SharedPtr <T>::destroy_complete ()
{
   assert (is_valid ());

   if (_obj_ptr != 0)
   {
      const conc::AioAdd <long>  ftor_inc (-1);
      long count = conc::AtomicIntOp_exec_new (*_count_ptr, ftor_inc);
      
      if (count == 0)
      {
         delete _obj_ptr;
         _obj_ptr = 0;

         delete _count_ptr;
         _count_ptr = 0;
      }
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace mem
}  // namespace ohm



#endif   // ohm_mem_SharedPtr_CODEHEADER_INCLUDED

#undef ohm_mem_SharedPtr_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
