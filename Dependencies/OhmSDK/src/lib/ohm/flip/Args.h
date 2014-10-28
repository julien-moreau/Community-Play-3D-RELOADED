/*****************************************************************************

        Args.h
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



#if ! defined (ohm_flip_Args_HEADER_INCLUDED)
#define  ohm_flip_Args_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"

#include <string>
#include <vector>



namespace ohm
{
namespace flip
{



class Bool;
class Int64;
class EnumMaster;
class Float64;
class Blob;
class ObjectRefMaster;
class Cue;
class Object;
class ArrayMaster;
class CollectionMaster;

class Args
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  Args ();
   virtual        ~Args ();
   
   Args &         push (bool val);
   Args &         push (archi::Int64 val);
   Args &         push (archi::Float64 val);
   Args &         push (const std::string & str);
   Args &         push (const std::vector <archi::UByte> & val);
   Args &         push (const archi::UByte * val_ptr, size_t val_size);
   Args &         push_cue ();
   Args &         push_cue (const archi::UByte * val_ptr, size_t val_size);
   Args &         push (Object & object);
   /*template <class InputIterator>
   Args &         push (InputIterator it, InputIterator it_end);*/
   
   bool           empty ();
   
   void           pop (Bool & val);
   void           pop (Int64 & val);
   void           pop (EnumMaster & val);
   void           pop (Float64 & val);
   void           pop (Blob & val);
   void           pop (ObjectRefMaster & val);
   void           pop (Cue & val);
   /*void         pop (ArrayMaster & val);
   void           pop (CollectionMaster & val);*/
   
   void           pop (bool & val);
   void           pop (archi::Int64 & val);
   void           pop (archi::Float64 & val);
   void           pop (const archi::UByte * & data_ptr, size_t & data_size);
   void           pop (std::string & str);
   
   void           end ();
   
   static Args    _none;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum ElemType
   {
                  ElemType_VOID = 0,
                  ElemType_BOOL,
                  ElemType_INT64,
                  ElemType_FLOAT64,
                  ElemType_BLOB,
                  ElemType_CUE,
                  /*ElemType_LIST,*/
   };
   
   class Elem
   {
   public:
      ElemType    _type;
      
      union
      {
         archi::Int64
                  _val_int64;
         archi::Float64
                  _val_float64;
      };
      
      std::vector <archi::UByte>
                  _val_blob;
   };
   
   typedef std::vector <Elem> ElemArr;
   
   ElemArr        _elem_arr;
   ElemArr::size_type
                  _pop_index;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Args (const Args &other);
   Args &         operator = (const Args &other);
   bool           operator == (const Args &other);
   bool           operator != (const Args &other);

}; // class Args



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/Args.hpp"



#endif   // ohm_flip_Args_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
