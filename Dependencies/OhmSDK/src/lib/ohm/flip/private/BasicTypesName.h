/*****************************************************************************

        BasicTypesName.h
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 45327

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



#if ! defined (ohm_flip_BasicTypesName_HEADER_INCLUDED)
#define  ohm_flip_BasicTypesName_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace flip
{



class BasicTypesName
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~BasicTypesName () {}
   
   static bool    is_basic_type (const char * name_0);
   
   static const char *
                  _object_ref_0;
   static const char *
                  _array_0;
   static const char *
                  _collection_0;
   static const char *
                  _bool_0;
   static const char *
                  _int64_0;
   static const char *
                  _enum_0;
   static const char *
                  _float64_0;
   static const char *
                  _blob_0;
   static const char *
                  _cue_0;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  BasicTypesName ();
                  BasicTypesName (const BasicTypesName &other);
   BasicTypesName &     operator = (const BasicTypesName &other);
   bool           operator == (const BasicTypesName &other);
   bool           operator != (const BasicTypesName &other);

}; // class BasicTypesName



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/BasicTypesName.hpp"



#endif   // ohm_flip_BasicTypesName_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
