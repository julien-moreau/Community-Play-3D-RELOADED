/*****************************************************************************

        SpacialObject.h
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



#if ! defined (model_SpacialObject_HEADER_INCLUDED)
#define  model_SpacialObject_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/Float64.h"
#include "ohm/flip/TxSessionGuard.h"



namespace model
{



class SpacialObject
:  public ohm::flip::Object
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   static void    declare ();

                  SpacialObject (ohm::flip::DocumentBase & document);
   virtual        ~SpacialObject ();

   bool           operator < (const SpacialObject & other);
   
   virtual void   update_client_data_cache ();
   
   void           ext_set_position (ohm::archi::Float64 position_x, ohm::archi::Float64 position_y);
   void           ext_set_position_x (ohm::archi::Float64 position_x);
   void           ext_set_position_y (ohm::archi::Float64 position_y);
   bool           position_changed () const;
   ohm::archi::Float64
                  get_position_x () const;
   ohm::archi::Float64
                  get_position_y () const;


   
/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   ohm::flip::Float64
                  _position_x;
   ohm::flip::Float64
                  _position_y;

   ohm::flip::TxSessionGuard
                  _tx_session_guard;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  SpacialObject ();
                  SpacialObject (const SpacialObject &other);
   SpacialObject &   operator = (const SpacialObject &other);
   bool           operator == (const SpacialObject &other);
   bool           operator != (const SpacialObject &other);

}; // class SpacialObject



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace model



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "model/SpacialObject.hpp"



#endif   // model_SpacialObject_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
