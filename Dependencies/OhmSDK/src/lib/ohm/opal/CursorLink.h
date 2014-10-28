/*****************************************************************************

        CursorLink.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71200

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



#if ! defined (ohm_opal_CursorLink_HEADER_INCLUDED)
#define  ohm_opal_CursorLink_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/def.h"
#include "ohm/opal/StandardMouseCursor.h"

#include <string>



namespace ohm
{
namespace opa
{
class Point;
}



namespace opal
{



class Cursor;

class CursorLink
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
                  CursorLink ();
   virtual        ~CursorLink ();
   
   void           set_depth (int depth);
   
   void           show (bool show_flag);
   void           obscure ();

#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
   void           set (StandardMouseCursor cursor);
   void           set (Cursor & cursor);
   void           set (const std::string & path, const opa::Point & hot_point);
   void           set_default ();
   void           force (StandardMouseCursor cursor);
   void           force (Cursor & cursor);
   void           force (const std::string & path, const opa::Point & hot_point);
   void           force_default ();
   void           unset ();
#endif
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   bool           _set_flag;
   int            _depth;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  CursorLink (const CursorLink &other);
   CursorLink &   operator = (const CursorLink &other);
   bool           operator == (const CursorLink &other);
   bool           operator != (const CursorLink &other);

}; // class CursorLink



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_CursorLink_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
