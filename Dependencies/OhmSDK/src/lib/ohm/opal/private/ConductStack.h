/*****************************************************************************

        ConductStack.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 38817

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



#if ! defined (ohm_opal_ConductStack_HEADER_INCLUDED)
#define  ohm_opal_ConductStack_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <vector>

#include <typeinfo>



namespace ohm
{
namespace opal
{



class ConductBase;

class ConductStack
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  ConductStack ();
   virtual        ~ConductStack ();

   ConductStack & operator = (const ConductStack & other);
   
   void           reset ();
   void           save ();
   void           restore ();
   
   void           invalidate ();
   
   template <class ConductType>
   void           set (ConductType * conduct_ptr);
   template <class ConductType>
   ConductType *  get ();
   
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   class TypeInfoPtr
   {
   public:
      const std::type_info *
                  _info_ptr;
      ConductBase *
                  _conduct_ptr;
   };
   
   class PosFlag
   {
   public:
      size_t      _pos;
      bool        _spatial_flag;
   };
   
   typedef std::vector <TypeInfoPtr> TypeInfoPtrArr;
   typedef std::vector <PosFlag> PosFlagArr;
   
   TypeInfoPtrArr _type_info_ptr_arr;
   PosFlagArr     _pos_flag_arr;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ConductStack (const ConductStack &other);
   bool           operator == (const ConductStack &other);
   bool           operator != (const ConductStack &other);

}; // class ConductStack



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/opal/private/ConductStack.hpp"



#endif   // ohm_opal_ConductStack_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
