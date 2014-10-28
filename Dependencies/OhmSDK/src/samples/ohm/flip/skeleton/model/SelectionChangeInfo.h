/*****************************************************************************

        SelectionChangeInfo.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70455

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



#if ! defined (model_SelectionChangeInfo_HEADER_INCLUDED)
#define  model_SelectionChangeInfo_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/util/BinaryStreamInput.h"
#include "ohm/util/BinaryStreamOutputBase.h"

#include <typeinfo>



namespace model
{



class SelectionChangeInfo
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  SelectionChangeInfo ();
   virtual        ~SelectionChangeInfo ();

   void           write (ohm::util::BinaryStreamOutputBase & bso) const;
   int            read (ohm::util::BinaryStreamInput & bsi);
   
   template <class T>
   void           set (int type, const T & data);
   template <class T>
   const T &      use_data () const;
   template <class T>
   bool           is_data_of_type () const;
   int            get_change_type () const;
   
   

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   typedef  void  (*DtorFnc) (SelectionChangeInfo & obj);
   
   void           destroy_data ();
   
   template <class T>
   static void    data_destructor (SelectionChangeInfo & obj);
   
   ohm::archi::Int32
                  _type;
   const std::type_info *
                  _data_type_info_ptr;
   std::vector <ohm::archi::UByte>
                  _data;
   DtorFnc        _dtor_fnc;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  SelectionChangeInfo (const SelectionChangeInfo &other);
   SelectionChangeInfo &   operator = (const SelectionChangeInfo &other);
   bool           operator == (const SelectionChangeInfo &other) const;
   bool           operator != (const SelectionChangeInfo &other) const;

}; // class SelectionChangeInfo



}  // namespace model



#include "model/SelectionChangeInfo.hpp"



#endif   // model_SelectionChangeInfo_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

