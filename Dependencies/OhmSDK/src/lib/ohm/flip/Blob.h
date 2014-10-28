/*****************************************************************************

        Blob.h
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 63856

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



#if ! defined (ohm_flip_Blob_HEADER_INCLUDED)
#define  ohm_flip_Blob_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/private/BasicType.h"
#include "ohm/flip/private/Codec.h"

#include "ohm/archi/types.h"

#include <vector>



namespace ohm
{
namespace flip
{



class DocumentBase;
class UByteProxy;

class Blob
:  public BasicType
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum Encoder
   {
                  Encoder_NONE = Codec::Encoder_NONE,
                  Encoder_BASE64 = Codec::Encoder_BASE64,
                  Encoder_DEFLATE = Codec::Encoder_DEFLATE,
                  Encoder_ESCAPED_ASCII = Codec::Encoder_ESCAPED_ASCII,
                  Encoder_HEXADECIMAL = Codec::Encoder_HEXADECIMAL,
   };
   
   typedef std::vector <archi::UByte>  Data;
   
   static void    declare ();
   
                  Blob (DocumentBase & document);
   virtual        ~Blob ();
   
   bool           operator < (const Blob & other) const;

   // Object
   virtual void   ctor (ohm::flip::Args & args);
   void           ctor (const archi::UByte * data_ptr, size_t data_size);
   
   void           reset_encoder_arr ();
   void           push_encoder (Encoder encoder);
   void           push_encoder_base64 ();
   void           push_encoder_deflate ();
   void           push_encoder_escaped_ascii ();
   void           push_encoder_hexadecimal ();
   const std::vector <Codec::Encoder> &
                  get_encoder_arr () const;

   void           set_inline (bool inline_flag = true);
   bool           is_inline () const;
   
   void           set (const archi::UByte * data_ptr, size_t data_size);
   void           set (const std::string & str);
   void           subset (size_t data_pos, const archi::UByte * data_ptr, size_t data_size);
   const Data &   get () const;
   std::string    to_string () const;
   void           clear ();
   bool           empty () const;
   size_t         size () const;
   
   bool           did_data_change () const;
   const Data &   get_old_data () const;
   
   virtual void   backup ();
   virtual void   synchronize ();
   virtual void   check_synchronized () const;
   virtual void   check_inner_invalidation_cnt (size_t invalid_cnt) const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   friend class VirtualMachine;
   friend class UByteProxy;
   
   void           set_blob (const archi::UByte * data_ptr, archi::UInt32 data_size);
   void           make_transaction_and_set (const archi::UByte * data_ptr, archi::UInt32 data_size);

   void           subset_blob (size_t data_pos, const archi::UByte * data_ptr, archi::UInt32 data_size);
   void           make_transaction_and_subset (size_t data_pos, const archi::UByte * data_ptr, archi::UInt32 data_size);
   
   Data           _data;
   Data           _old_data;
   std::vector <Codec::Encoder>
                  _encoder_arr;
   bool           _old_cur_equal_flag;
   bool           _inline_flag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Blob ();
                  Blob (const Blob &other);
   Blob &         operator = (const Blob &other);
   bool           operator == (const Blob &other);
   bool           operator != (const Blob &other);

}; // class Blob



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_flip_Blob_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
