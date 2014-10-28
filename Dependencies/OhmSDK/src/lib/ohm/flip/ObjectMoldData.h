/*****************************************************************************

        ObjectMoldData.h
        Copyright (c) 2009 Ohm Force

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



#if ! defined (ohm_flip_ObjectMoldData_HEADER_INCLUDED)
#define  ohm_flip_ObjectMoldData_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/private/TypeTraits.h"

#include <map>
#include <string>
#include <list>



namespace ohm
{
namespace flip
{



class PasteBoard;

class ObjectMoldData
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  ObjectMoldData (flip::Ref ref, int class_id);
   virtual        ~ObjectMoldData ();
   
   flip::Ref      get_ref () const;
   template <class Type>
   bool           is_of_type () const;
   template <class T>
   archi::Int64   get_value (Int64 T::*var, const char * var_name_0) const;
   template <class T>
   archi::Float64 get_value (Float64 T::*var, const char * var_name_0) const;
   template <class T>
   Blob::Data     get_value (Blob T::*var, const char * var_name_0) const;
   template <class T, class U>
   flip::Ref      get_value (ObjectRef <U> T::*var, const char * var_name_0) const;
   template <class T, class U, archi::Int64 MIN, archi::Int64 NBR>
   U              get_value (Enum <U, MIN, NBR> T::*var, const char * var_name_0) const;
      
   // System access
   //void            set_type_id (int class_id);
   void           add_value (const std::string & name, int type_id, archi::Int64 val);
   void           add_value (const std::string & name, int type_id, archi::Float64 val);
   void           add_value (const std::string & name, int type_id, const Blob::Data & val);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   friend class PasteBoard;
   
   class Value
   {
   public:
      int         _type_id;
      archi::Int64
                  _val_i64;
      archi::Float64
                  _val_f64;
      Blob::Data  _val_blob;
   };
   
   typedef std::map <std::string, Value> NameValueMap;
   
   const flip::Ref
                  _ref;
   const int      _type_id;
   NameValueMap   _name_value_map;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ObjectMoldData ();
                  ObjectMoldData (const ObjectMoldData &other);
   ObjectMoldData &
                  operator = (const ObjectMoldData &other);
   bool           operator == (const ObjectMoldData &other);
   bool           operator != (const ObjectMoldData &other);

}; // class ObjectMoldData

typedef std::list <ObjectMoldData *>   ObjectMoldDataPtrList;



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/flip/ObjectMoldData.hpp"



#endif   // ohm_flip_ObjectMoldData_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
