/*****************************************************************************

        Blob.cpp
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 65772

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



#if defined (_MSC_VER)
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/Blob.h"
#include "ohm/util/Base64.h"
#include "ohm/util/trace.h"
#include "ohm/flip/ClassDescription.h"
#include "ohm/flip/ClassDescManager.h"
#include "ohm/flip/private/BasicTypesName.h"
#include "ohm/flip/private/DocumentBase.h"
#include "ohm/txt/utf8/Codec8.h"
#include "ohm/flip/private/MachineCode.h"

#include "zlib.h"

#include <stdexcept>

#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : declare
==============================================================================
*/

void  Blob::declare ()
{
   ClassDescription <Blob>::use ().set_name (BasicTypesName::_blob_0);
   
   ClassDescManager::declare (ClassDescription <Blob>::use ());
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

Blob::Blob (DocumentBase & document)
:  BasicType (document)
,  _data ()
,  _old_data ()
,  _encoder_arr ()
,  _old_cur_equal_flag (true)
,  _inline_flag (false)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Blob::~Blob ()
{
   // destruction (undefine) is handled in base class Object
}



/*
==============================================================================
Name : operator <
Note :
   does not implement lexicographical order, only total order through internal
   ref.
==============================================================================
*/

bool  Blob::operator < (const Blob & other) const
{
   return get_ref () < other.get_ref ();
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

void  Blob::ctor (ohm::flip::Args & args)
{
   if (!args.empty ())
   {
      args.pop (*this);
   }
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

void  Blob::ctor (const archi::UByte * data_ptr, size_t data_size)
{
   make_transaction_and_set (data_ptr, data_size);
}



/*
==============================================================================
Name : reset_encoder_arr
==============================================================================
*/

void  Blob::reset_encoder_arr ()
{
   _encoder_arr.clear ();
}



/*
==============================================================================
Name : push_encoder
==============================================================================
*/

void  Blob::push_encoder (Encoder encoder)
{
   assert (encoder >= Encoder_BASE64);
   assert (encoder <= Encoder_HEXADECIMAL);
   
   _encoder_arr.push_back (static_cast <Codec::Encoder> (encoder));
}



/*
==============================================================================
Name : push_encoder_base64
==============================================================================
*/

void  Blob::push_encoder_base64 ()
{
   _encoder_arr.push_back (Codec::Encoder_BASE64);
}



/*
==============================================================================
Name : push_encoder_deflate
==============================================================================
*/

void  Blob::push_encoder_deflate ()
{
   _encoder_arr.push_back (Codec::Encoder_DEFLATE);
}



/*
==============================================================================
Name : push_encoder_escaped_ascii
==============================================================================
*/

void  Blob::push_encoder_escaped_ascii ()
{
   _encoder_arr.push_back (Codec::Encoder_ESCAPED_ASCII);
}



/*
==============================================================================
Name : push_encoder_hexadecimal
==============================================================================
*/

void  Blob::push_encoder_hexadecimal ()
{
   _encoder_arr.push_back (Codec::Encoder_HEXADECIMAL);
}



/*
==============================================================================
Name : get_encoder_arr
==============================================================================
*/

const std::vector <Codec::Encoder> &   Blob::get_encoder_arr () const
{
   return _encoder_arr;
}



/*
==============================================================================
Name : set_inline
==============================================================================
*/

void  Blob::set_inline (bool inline_flag)
{
   _inline_flag = inline_flag;
}



/*
==============================================================================
Name : is_inline
==============================================================================
*/

bool  Blob::is_inline () const
{
   return _inline_flag;
}



/*
==============================================================================
Name : set
==============================================================================
*/

void  Blob::set (const archi::UByte * data_ptr, size_t data_size)
{
   make_transaction_and_set (data_ptr, data_size);
}



/*
==============================================================================
Name : set
==============================================================================
*/

void  Blob::set (const std::string & str)
{
   set (reinterpret_cast <const archi::UByte *> (str.c_str ()), str.size ());
}



/*
==============================================================================
Name : subset
==============================================================================
*/

void  Blob::subset (size_t data_pos, const archi::UByte * data_ptr, size_t data_size)
{
   bool ok_flag = data_pos + data_size <= _data.size ();
   assert (ok_flag);
   
   if (ok_flag)
   {
      make_transaction_and_subset (data_pos, data_ptr, data_size);
   }
}



/*
==============================================================================
Name : get
==============================================================================
*/

const Blob::Data &   Blob::get () const
{
   return _data;
}



/*
==============================================================================
Name : to_string
==============================================================================
*/

std::string Blob::to_string () const
{
   std::string ret_val;
   
   ret_val.assign (_data.begin (), _data.end ());
   
   return ret_val;
}



/*
==============================================================================
Name : clear
==============================================================================
*/

void  Blob::clear ()
{
   make_transaction_and_set (0, 0);
}



/*
==============================================================================
Name : empty
==============================================================================
*/

bool  Blob::empty () const
{
   return _data.empty ();
}



/*
==============================================================================
Name : size
==============================================================================
*/

size_t   Blob::size () const
{
   return _data.size ();
}



/*
==============================================================================
Name : did_data_change
==============================================================================
*/

bool  Blob::did_data_change () const
{
   return !_old_cur_equal_flag;
}



/*
==============================================================================
Name : get_old_data
==============================================================================
*/

const Blob::Data &   Blob::get_old_data () const
{
   return _old_data;
}



/*
==============================================================================
Name : backup
==============================================================================
*/

void  Blob::backup ()
{
   make_transaction_and_set (0, 0);
}



/*
==============================================================================
Name : synchronize
==============================================================================
*/

void  Blob::synchronize ()
{
   _old_data = _data;
   _old_cur_equal_flag = true;
   
   Object::synchronize ();
}



/*
==============================================================================
Name : synchronize
==============================================================================
*/

void  Blob::check_synchronized () const
{
   assert (_old_data == _data);
   assert (_old_cur_equal_flag);
   
   Object::check_synchronized ();
}



/*
==============================================================================
Name : check_inner_invalidation_cnt
==============================================================================
*/

void  Blob::check_inner_invalidation_cnt (size_t invalid_cnt) const
{
   if (!_old_cur_equal_flag)
   {
      ohm_util_TRACE_DEBUG ("  BLO");
      assert (invalid_cnt > 0);
      --invalid_cnt;
   }
   
   Object::check_inner_invalidation_cnt (invalid_cnt);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : set_blob
==============================================================================
*/

void  Blob::set_blob (const archi::UByte * data_ptr, archi::UInt32 data_size)
{
#if ! defined (NDEBUG)
   if (_data.size () == data_size)
   {
      bool diff_flag = false;
      
      for (size_t i = 0 ; i < data_size ; ++i)
      {
         diff_flag = _data [i] != data_ptr [i];
         
         if (diff_flag)
         {
            break;
         }
      }
      
      assert (diff_flag);
   }
#endif
   
   if (_old_cur_equal_flag)
   {
      _old_cur_equal_flag = false;
      
      invalidate ();
   }
   else
   {
      if (_old_data.size () == data_size)
      {
         bool diff_flag = false;
      
         for (size_t i = 0 ; i < data_size ; ++i)
         {
            diff_flag = _old_data [i] != data_ptr [i];
            
            if (diff_flag)
            {
               break;
            }
         }
         
         if (!diff_flag)
         {
            _old_cur_equal_flag = true;
      
            validate ();
         }
      }
   }
   
   // ok when data_ptr is null
   _data.assign (data_ptr, data_ptr + data_size);
}



/*
==============================================================================
Name : make_transaction_and_set
==============================================================================
*/

void  Blob::make_transaction_and_set (const archi::UByte * data_ptr, archi::UInt32 data_size)
{
   if (data_size == _data.size ())
   {
      if (std::equal (_data.begin (), _data.end (), data_ptr))
      {
         return;
      }
   }
   
   using namespace std;
   
   Data & transaction = use_document ().use_transaction ();

   Ref this_ref = get_ref ();
   assert (this_ref != 0);

   if (_data.size () > 0)
   {
      MachineCode::build_blob_set (
         transaction,
         this_ref,
         &_data [0], _data.size (),
         data_ptr, data_size
      );
   }
   else
   {
      MachineCode::build_blob_set (
         transaction,
         this_ref,
         0, 0,
         data_ptr, data_size
      );
   }
   
   set_blob (data_ptr, data_size);
}



/*
==============================================================================
Name : subset_blob
==============================================================================
*/

void  Blob::subset_blob (size_t data_pos, const archi::UByte * data_ptr, archi::UInt32 data_size)
{
#if ! defined (NDEBUG)
   {
      assert (data_pos + data_size <= _data.size ());
   
      bool diff_flag = false;
      
      for (size_t i = 0 ; i < data_size ; ++i)
      {
         diff_flag = _data [data_pos + i] != data_ptr [i];
         
         if (diff_flag)
         {
            break;
         }
      }
      
      assert (diff_flag);
   }
#endif
   
   if (_old_cur_equal_flag)
   {
      _old_cur_equal_flag = false;
      
      invalidate ();
   }
   else
   {
      bool diff_flag = ! (_old_data.size () >= data_pos + data_size);
      
      if (!diff_flag)
      {
         for (size_t i = 0 ; i < data_size ; ++i)
         {
            diff_flag = _old_data [data_pos + i] != data_ptr [i];
            
            if (diff_flag)
            {
               break;
            }
         }
      }
      
      if (!diff_flag)
      {
         _old_cur_equal_flag = true;
   
         validate ();
      }
   }
   
   bool ok_flag = data_pos + data_size <= _data.size ();
   
   if (ok_flag)
   {
      using namespace std;
      memcpy (&_data [data_pos], data_ptr, data_size);
   }
}



/*
==============================================================================
Name : make_transaction_and_subset
==============================================================================
*/

void  Blob::make_transaction_and_subset (size_t data_pos, const archi::UByte * data_ptr, archi::UInt32 data_size)
{
   if (std::equal (_data.begin () + data_pos, _data.begin () + data_pos + data_size, data_ptr))
   {
      return;
   }
   
   using namespace std;
   
   Data & transaction = use_document ().use_transaction ();

   Ref this_ref = get_ref ();
   assert (this_ref != 0);

   MachineCode::build_blob_subset (
      transaction,
      this_ref,
      data_pos, data_size,
      &_data [data_pos], data_ptr
   );
   
   subset_blob (data_pos, data_ptr, data_size);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
