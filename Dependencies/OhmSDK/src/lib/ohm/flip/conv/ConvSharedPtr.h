/*****************************************************************************

        ConvSharedPtr.h
        Copyright (c) 2011 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 64596

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



#if ! defined (ohm_flip_ConvSharedPtr_HEADER_INCLUDED)
#define  ohm_flip_ConvSharedPtr_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace flip
{



template <class T>
class ConvSharedPtr
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef  T  DataType;

                  ConvSharedPtr ();
                  ConvSharedPtr (const ConvSharedPtr <T> &other);
   explicit       ConvSharedPtr (T *ptr);
                  ~ConvSharedPtr ();

   ConvSharedPtr <T> &operator = (const ConvSharedPtr <T> &other);
   void           swap (ConvSharedPtr<T> &other);
   
   template <class U>
   void           assign_static (const ConvSharedPtr <U> & other);
   template <class U>
   void           assign_dynamic (const ConvSharedPtr <U> & other);
   template <class U>
   void           assign_reinterpret (const ConvSharedPtr <U> & other);
   
                  operator ConvSharedPtr <const T> () const;

   T *            get () const;
   T *            operator -> () const;
   T &            operator * () const;
   void           destroy ();

   long           get_count () const;

   bool           is_valid () const;
   


/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   // Do not use those functions (for implementation only)
                  ConvSharedPtr (T *other_ptr, long * count_ptr);
   long *         get_counter_ref () const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   bool           is_consistent (const ConvSharedPtr <T> &other) const;

   void           add_ref ();
   void           destroy_complete ();

   DataType *     _obj_ptr;      // 0 if no associated object
   long *
                  _count_ptr;    // 0 if no associated object



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

}; // class ConvSharedPtr



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : operator ==
==============================================================================
*/

template <class T, class U>
bool  operator == (const ConvSharedPtr<T> &l_op, const ConvSharedPtr<U> &r_op)
{
   return (l_op.get () == r_op.get ());
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

template <class T, class U>
bool  operator != (const ConvSharedPtr<T> &l_op, const ConvSharedPtr<U> &r_op)
{
   return (l_op.get () != r_op.get ());
}



/*
==============================================================================
Name : operator <
==============================================================================
*/

template <class T, class U>
bool  operator < (const ConvSharedPtr<T> &l_op, const ConvSharedPtr<U> &r_op)
{
   return (l_op.get () < r_op.get ());
}



/*
==============================================================================
Name : shared_static_cast
==============================================================================
*/

template <class T, class U>
ConvSharedPtr <T> shared_static_cast (ConvSharedPtr <U> const &r_op)
{
   return (ConvSharedPtr <T> (
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
ConvSharedPtr <T> shared_dynamic_cast (ConvSharedPtr <U> const &r_op)
{
   T *            obj_ptr = dynamic_cast <T *> (r_op.get ());

   if (obj_ptr == 0)
   {
      ConvSharedPtr <T> shared_ptr;
      return (shared_ptr);
   }

   else
   {
      ConvSharedPtr <T> shared_ptr (obj_ptr, r_op.get_counter_ref ());
      return (shared_ptr);
   }
}



/*
==============================================================================
Name : shared_reinterpret_cast
==============================================================================
*/

template <class T, class U>
ConvSharedPtr <T> shared_reinterpret_cast (ConvSharedPtr <U> const &r_op)
{
   return (ConvSharedPtr <T> (
      reinterpret_cast <T *> (r_op.get ()),
      r_op.get_counter_ref ()
   ));
}



}  // namespace flip
}  // namespace ohm



#include "ohm/flip/conv/ConvSharedPtr.hpp"



#endif   // ohm_flip_ConvSharedPtr_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
