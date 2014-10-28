/*****************************************************************************

        SharedPtr.h
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 45315

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



#if defined (ohm_mem_SharedPtr_CURRENT_HEADER)
   #error Recursive inclusion of ohm/mem/SharedPtr header.
#endif
#define  ohm_mem_SharedPtr_CURRENT_HEADER

#if ! defined (ohm_mem_SharedPtr_HEADER_INCLUDED)
#define  ohm_mem_SharedPtr_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/conc/AtomicInt.h"



namespace ohm
{
namespace mem
{



template <class T>
class SharedPtr
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef  T  DataType;

                  SharedPtr ();
                  SharedPtr (const SharedPtr <T> &other);
   explicit       SharedPtr (T *ptr);
                  ~SharedPtr ();

   SharedPtr <T> &operator = (const SharedPtr <T> &other);
   void           swap (SharedPtr<T> &other);
   
   template <class U>
   void           assign_static (const SharedPtr <U> & other);
   template <class U>
   void           assign_dynamic (const SharedPtr <U> & other);
   template <class U>
   void           assign_reinterpret (const SharedPtr <U> & other);
   
                  operator SharedPtr <const T> () const;

   T *            get () const;
   T *            operator -> () const;
   T &            operator * () const;
   void           destroy ();

   long           get_count () const;

   bool           is_valid () const;
   


/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   // Do not use those functions (for implementation only)
                  SharedPtr (T *other_ptr, conc::AtomicInt <long> * count_ptr);
   conc::AtomicInt <long> *
                  get_counter_ref () const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   bool           is_consistent (const SharedPtr <T> &other) const;

   void           add_ref ();
   void           destroy_complete ();

   DataType *     _obj_ptr;      // 0 if no associated object
   conc::AtomicInt <long> *
                  _count_ptr;    // 0 if no associated object



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

}; // class SharedPtr



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : operator ==
==============================================================================
*/

template <class T, class U>
bool  operator == (const SharedPtr<T> &l_op, const SharedPtr<U> &r_op)
{
   return (l_op.get () == r_op.get ());
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

template <class T, class U>
bool  operator != (const SharedPtr<T> &l_op, const SharedPtr<U> &r_op)
{
   return (l_op.get () != r_op.get ());
}



/*
==============================================================================
Name : operator <
==============================================================================
*/

template <class T, class U>
bool  operator < (const SharedPtr<T> &l_op, const SharedPtr<U> &r_op)
{
   return (l_op.get () < r_op.get ());
}



/*
==============================================================================
Name : shared_static_cast
==============================================================================
*/

template <class T, class U>
SharedPtr <T>  shared_static_cast (SharedPtr <U> const &r_op)
{
   return (SharedPtr <T> (
      static_cast <T *> (r_op.get ()),
      r_op.get_counter_ref ()
   ));
}



/*
==============================================================================
Name : shared_dynamic_cast
==============================================================================
*/

template <class T, class U>
SharedPtr <T>  shared_dynamic_cast (SharedPtr <U> const &r_op)
{
   T *            obj_ptr = dynamic_cast <T *> (r_op.get ());

   if (obj_ptr == 0)
   {
      SharedPtr <T> shared_ptr;
      return (shared_ptr);
   }

   else
   {
      SharedPtr <T> shared_ptr (obj_ptr, r_op.get_counter_ref ());
      return (shared_ptr);
   }
}



/*
==============================================================================
Name : shared_reinterpret_cast
==============================================================================
*/

template <class T, class U>
SharedPtr <T>  shared_reinterpret_cast (SharedPtr <U> const &r_op)
{
   return (SharedPtr <T> (
      reinterpret_cast <T *> (r_op.get ()),
      r_op.get_counter_ref ()
   ));
}



}  // namespace mem
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/mem/SharedPtr.hpp"



#endif   // ohm_mem_SharedPtr_HEADER_INCLUDED

#undef ohm_mem_SharedPtr_CURRENT_HEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
