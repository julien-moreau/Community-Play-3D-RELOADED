/*****************************************************************************

        BinaryStreamOutputBase.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70845

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



#if ! defined (ohm_util_BinaryStreamOutputBase_HEADER_INCLUDED)
#define  ohm_util_BinaryStreamOutputBase_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/mem/SharedPtr.h"

#include <string>
#include <vector>



namespace ohm
{
namespace util
{



class BinaryStreamOutputBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  BinaryStreamOutputBase ();
   virtual        ~BinaryStreamOutputBase ();
   
   const archi::UByte *
                  data () const;
   size_t         size () const;
   bool           empty () const;

   void           clear ();

   void           write (bool val);
   void           write (archi::UByte val);
   void           write (archi::SByte val);
   void           write (archi::UInt16 val);
   void           write (archi::Int16 val);
   void           write (archi::UInt32 val);
   void           write (archi::Int32 val);
   void           write (archi::UInt64 val);
   void           write (archi::Int64 val);
   void           write (archi::Float32 val);
   void           write (archi::Float64 val);
   void           write (void * val);
   void           write (const void * val);
   void           write (const std::string & str);
   void           write (const archi::UByte * data_ptr, size_t data_size);
   template <class InputIterator>
   void           write (InputIterator it, InputIterator it_end);
   template <class T, class U>
   void           write (const std::pair <T, U> & val);
   template <class T>
   void           write (const T & val);
   template <class T>
   void           write (const mem::SharedPtr <T> & val_sptr);

   void           write_front (const archi::UByte * data_ptr, size_t data_size);

   void           overwrite (bool val, size_t pos);
   void           overwrite (archi::UByte val, size_t pos);
   void           overwrite (archi::SByte val, size_t pos);
   void           overwrite (archi::UInt16 val, size_t pos);
   void           overwrite (archi::Int16 val, size_t pos);
   void           overwrite (archi::UInt32 val, size_t pos);
   void           overwrite (archi::Int32 val, size_t pos);
   void           overwrite (archi::UInt64 val, size_t pos);
   void           overwrite (archi::Int64 val, size_t pos);
   void           overwrite (archi::Float32 val, size_t pos);
   void           overwrite (archi::Float64 val, size_t pos);
   void           overwrite (const std::string & str, size_t pos);
   void           overwrite (const archi::UByte * data_ptr, size_t data_size, size_t pos);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   virtual const archi::UByte *
                  do_data () const = 0;
   virtual size_t do_size () const = 0;
   virtual bool   do_empty () const = 0;

   virtual void   do_clear () = 0;

   virtual archi::UByte *
                  do_resize_of (size_t diff_size) = 0;
   virtual archi::UByte *
                  do_resize_max_of (size_t pos, size_t diff_size) = 0;
   virtual void   do_resize_write_bytes (const archi::UByte * data_ptr, size_t data_size) = 0;
   virtual void   do_write_front_bytes (const archi::UByte * data_ptr, size_t data_size) = 0;
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  BinaryStreamOutputBase (const BinaryStreamOutputBase &other);
   BinaryStreamOutputBase &
                  operator = (const BinaryStreamOutputBase &other);
   bool           operator == (const BinaryStreamOutputBase &other);
   bool           operator != (const BinaryStreamOutputBase &other);

}; // class BinaryStreamOutputBase



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace util
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/util/BinaryStreamOutputBase.hpp"



#endif   // ohm_util_BinaryStreamOutputBase_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
