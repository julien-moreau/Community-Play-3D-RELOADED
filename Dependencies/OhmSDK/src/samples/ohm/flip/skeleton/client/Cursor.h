/*****************************************************************************

        Cursor.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71268

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



#if ! defined (client_Cursor_HEADER_INCLUDED)
#define  client_Cursor_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/util/SingletonBase.h"
#include "ohm/opa/Point.h"

#include <bitset>



namespace client
{



class Cursor
:  public ohm::util::SingletonBase <Cursor>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~Cursor () {}
   
   static Cursor &use ();
   
   // base
   Cursor &       arrow ();
   Cursor &       resize ();
   Cursor &       hand ();
   Cursor &       pen ();
   Cursor &       eraser ();
   
   // options
   Cursor &       move ();
   Cursor &       copy (bool copy_flag = true);
   Cursor &       right ();
   Cursor &       open ();
   Cursor &       close (bool close_flag = true);
   
   // end
   void           set ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  Cursor ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   ohm_util_SingletonBase_INVITE_SINGLETON (Cursor)
   
   enum Base
   {
                  Base_NONE = 0,
                  
                  Base_ARROW,
                  Base_RESIZE,
                  Base_HAND,
                  Base_PEN,
                  Base_ERASER,
   };
   
   enum Opt
   {
                  Opt_MOVE = 0,
                  Opt_COPY,
                  Opt_RIGHT,
                  Opt_OPEN,
                  Opt_CLOSE,
                  
                  Opt_NBR_ELT
   };
   
   typedef std::bitset <Opt_NBR_ELT>   Options;
   
   enum Type
   {
                  Type_ARROW = 0,
                  Type_ARROW_MOVE,
                  Type_ARROW_MOVE_COPY,

                  Type_RESIZE_HORI_RIGHT,

                  Type_HAND_OPENED,
                  Type_HAND_CLOSED,

                  Type_PEN,
      
                  Type_ERASER,
   };
   
   void           reset ();
   void           push (Opt opt);
   bool           test () const;
   bool           test (Opt opt1) const;
   bool           test (Opt opt1, Opt opt2) const;

   void           get_arrow (std::string & path, ohm::opa::Point & hot_point);
   void           get_resize (std::string & path, ohm::opa::Point & hot_point);
   void           get_hand (std::string & path, ohm::opa::Point & hot_point);
   void           get_pen (std::string & path, ohm::opa::Point & hot_point);
   void           get_eraser (std::string & path, ohm::opa::Point & hot_point);
   void           get (std::string & path, ohm::opa::Point & hot_point, const std::string & type, const ohm::opa::Point & point, bool offset_flag = false);
   
   Base           _base;
   Options        _options;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Cursor (const Cursor &other);
   Cursor &       operator = (const Cursor &other);
   bool           operator == (const Cursor &other);
   bool           operator != (const Cursor &other);

}; // class Cursor



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "client/Cursor.hpp"



#endif   // client_Cursor_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
