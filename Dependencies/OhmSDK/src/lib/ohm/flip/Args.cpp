/*****************************************************************************

        Args.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 49586

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

#include "ohm/flip/Args.h"
#include "ohm/flip/Bool.h"
#include "ohm/flip/Int64.h"
#include "ohm/flip/Enum.h"
#include "ohm/flip/Float64.h"
#include "ohm/flip/Blob.h"
#include "ohm/flip/ObjectRef.h"
#include "ohm/flip/Cue.h"

#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

Args::Args ()
:  _pop_index (0)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Args::~Args ()
{
}



/*
==============================================================================
Name : push
==============================================================================
*/

Args &   Args::push (bool val)
{
   Elem elem;
   elem._type = ElemType_BOOL;
   elem._val_int64 = val ? 1LL : 0LL;
   
   _elem_arr.push_back (elem);
   
   return *this;
}



/*
==============================================================================
Name : push
==============================================================================
*/

Args &   Args::push (archi::Int64 val)
{
   Elem elem;
   elem._type = ElemType_INT64;
   elem._val_int64 = val;
   
   _elem_arr.push_back (elem);
   
   return *this;
}



/*
==============================================================================
Name : push
==============================================================================
*/

Args &   Args::push (archi::Float64 val)
{
   Elem elem;
   elem._type = ElemType_FLOAT64;
   elem._val_float64 = val;
   
   _elem_arr.push_back (elem);
   
   return *this;
}



/*
==============================================================================
Name : push
==============================================================================
*/

Args &   Args::push (const std::string & str)
{
   Elem elem;
   elem._type = ElemType_BLOB;
   elem._val_blob.assign (str.begin (), str.end ());
   
   _elem_arr.push_back (elem);
   
   return *this;
}



/*
==============================================================================
Name : push
==============================================================================
*/

Args &   Args::push (const std::vector <archi::UByte> & val)
{
   Elem elem;
   elem._type = ElemType_BLOB;
   elem._val_blob = val;
   
   _elem_arr.push_back (elem);
   
   return *this;
}



/*
==============================================================================
Name : push
==============================================================================
*/

Args &   Args::push (const archi::UByte * val_ptr, size_t val_size)
{
   using namespace std;
   
   Elem elem;
   elem._type = ElemType_BLOB;
   elem._val_blob.resize (val_size);

   if (val_size > 0)
   {
      memcpy (&elem._val_blob [0], val_ptr, val_size * sizeof (archi::UByte));
   }
   
   _elem_arr.push_back (elem);
   
   return *this;
}



/*
==============================================================================
Name : push_cue
==============================================================================
*/

Args &   Args::push_cue ()
{
   using namespace std;
   
   Elem elem;
   elem._type = ElemType_CUE;
   
   _elem_arr.push_back (elem);
   
   return *this;
}



/*
==============================================================================
Name : push_cue
==============================================================================
*/

Args &   Args::push_cue (const archi::UByte * val_ptr, size_t val_size)
{
   using namespace std;
   
   Elem elem;
   elem._type = ElemType_CUE;
   elem._val_blob.resize (val_size);

   if (val_size > 0)
   {
      memcpy (&elem._val_blob [0], val_ptr, val_size * sizeof (archi::UByte));
   }
   
   _elem_arr.push_back (elem);
   
   return *this;
}



/*
==============================================================================
Name : push_signal
==============================================================================
*/

Args &   Args::push (Object & object)
{
   return push (object.get_ref ());
}



/*
==============================================================================
Name : empty
==============================================================================
*/

bool  Args::empty ()
{
   return _elem_arr.empty ();
}



/*
==============================================================================
Name : pop
==============================================================================
*/

void  Args::pop (Bool & val)
{
   assert (_pop_index >= 0);
   assert (_pop_index < _elem_arr.size ());
   
   Elem & elem = _elem_arr [_pop_index];
   assert (elem._type == ElemType_BOOL);
   
   val.ctor (elem._val_int64 != 0);
   
   ++_pop_index;
}



/*
==============================================================================
Name : pop
==============================================================================
*/

void  Args::pop (Int64 & val)
{
   assert (_pop_index >= 0);
   assert (_pop_index < _elem_arr.size ());
   
   Elem & elem = _elem_arr [_pop_index];
   assert (elem._type == ElemType_INT64);
   
   val.ctor (elem._val_int64);
   
   ++_pop_index;
}



/*
==============================================================================
Name : pop
==============================================================================
*/

void  Args::pop (EnumMaster & val)
{
   assert (_pop_index >= 0);
   assert (_pop_index < _elem_arr.size ());
   
   Elem & elem = _elem_arr [_pop_index];
   assert (elem._type == ElemType_INT64);
   
   val.ctor (elem._val_int64);
   
   ++_pop_index;
}



/*
==============================================================================
Name : pop
==============================================================================
*/

void  Args::pop (Float64 & val)
{
   assert (_pop_index >= 0);
   assert (_pop_index < _elem_arr.size ());
   
   Elem & elem = _elem_arr [_pop_index];
   assert (elem._type == ElemType_FLOAT64);
   
   val.ctor (elem._val_float64);
   
   ++_pop_index;
}



/*
==============================================================================
Name : pop
==============================================================================
*/

void  Args::pop (Blob & val)
{
   assert (_pop_index >= 0);
   assert (_pop_index < _elem_arr.size ());
   
   Elem & elem = _elem_arr [_pop_index];
   assert (elem._type == ElemType_BLOB);
   
   if (elem._val_blob.size () > 0)
   {
      val.ctor (&elem._val_blob [0], elem._val_blob.size ());
   }
   
   ++_pop_index;
}



/*
==============================================================================
Name : pop
==============================================================================
*/

void  Args::pop (ObjectRefMaster & val)
{
   assert (_pop_index >= 0);
   assert (_pop_index < _elem_arr.size ());
   
   Elem & elem = _elem_arr [_pop_index];
   assert (elem._type == ElemType_INT64);
   
   val.ctor (elem._val_int64);
   
   ++_pop_index;
}



/*
==============================================================================
Name : pop
==============================================================================
*/

void  Args::pop (Cue & val)
{
   assert (_pop_index >= 0);
   assert (_pop_index < _elem_arr.size ());
   
   Elem & elem = _elem_arr [_pop_index];
   assert (elem._type == ElemType_CUE);
   
   if (elem._val_blob.size () > 0)
   {
      val.ctor (&elem._val_blob [0], elem._val_blob.size ());
   }
   else
   {
      val.ctor ();
   }
   
   ++_pop_index;
}



/*
==============================================================================
Name : pop
==============================================================================
*/

void  Args::pop (bool & val)
{
   assert (_pop_index >= 0);
   assert (_pop_index < _elem_arr.size ());
   
   Elem & elem = _elem_arr [_pop_index];
   assert (elem._type == ElemType_BOOL);
   
   val = elem._val_int64 != 0;
   
   ++_pop_index;
}



/*
==============================================================================
Name : pop
==============================================================================
*/

/*void   Args::pop (ArrayMaster & val)
{
   assert (_pop_index >= 0);
   assert (_pop_index < _elem_arr.size ());
   
   Elem & elem = _elem_arr [_pop_index];
   assert (elem._type == ElemType_LIST);
   
   archi::Int64 nbr_item = elem._val_int64;
   
   for (archi::Int64 i = 0 ; i < nbr_item ; ++i)
   {
      
   }
}*/



/*
==============================================================================
Name : pop
==============================================================================
*/

/*void   Args::pop (CollectionMaster & val)
{
}*/



/*
==============================================================================
Name : pop
==============================================================================
*/

void  Args::pop (archi::Int64 & val)
{
   assert (_pop_index >= 0);
   assert (_pop_index < _elem_arr.size ());
   
   Elem & elem = _elem_arr [_pop_index];
   assert (elem._type == ElemType_INT64);
   
   val = elem._val_int64;
   
   ++_pop_index;
}



/*
==============================================================================
Name : pop
==============================================================================
*/

void  Args::pop (archi::Float64 & val)
{
   assert (_pop_index >= 0);
   assert (_pop_index < _elem_arr.size ());
   
   Elem & elem = _elem_arr [_pop_index];
   assert (elem._type == ElemType_FLOAT64);
   
   val = elem._val_float64;
   
   ++_pop_index;
}



/*
==============================================================================
Name : pop
==============================================================================
*/

void  Args::pop (const archi::UByte * & data_ptr, size_t & data_size)
{
   assert (_pop_index >= 0);
   assert (_pop_index < _elem_arr.size ());
   
   Elem & elem = _elem_arr [_pop_index];
   assert (elem._type == ElemType_BLOB);
   
   data_size = elem._val_blob.size ();

   if (data_size > 0)
   {
      data_ptr = &elem._val_blob [0];
   }
   else
   {
      data_ptr = 0;
   }
   
   ++_pop_index;
}



/*
==============================================================================
Name : pop
==============================================================================
*/

void  Args::pop (std::string & str)
{
   assert (_pop_index >= 0);
   assert (_pop_index < _elem_arr.size ());
   
   Elem & elem = _elem_arr [_pop_index];
   assert (elem._type == ElemType_BLOB);
   
   str.assign (elem._val_blob.begin (), elem._val_blob.end ());
   
   ++_pop_index;
}



/*
==============================================================================
Name : end
==============================================================================
*/

void  Args::end ()
{
   assert (_pop_index == _elem_arr.size ());
   
   _pop_index = 0;
}



Args Args::_none;


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
