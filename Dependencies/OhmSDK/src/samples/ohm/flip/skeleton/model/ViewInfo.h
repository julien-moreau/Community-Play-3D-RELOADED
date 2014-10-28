/*****************************************************************************

        ViewInfo.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70481

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



#if ! defined (model_ViewInfo_HEADER_INCLUDED)
#define  model_ViewInfo_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/Float64.h"
#include "ohm/flip/TxSessionGuard.h"



namespace model
{



class ViewInfo
:  public ohm::flip::Object
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   static void    declare ();

                  ViewInfo (ohm::flip::DocumentBase & document);
   virtual        ~ViewInfo ();
   
   void           tx_set_focus (ohm::archi::Float64 focus_x, ohm::archi::Float64 focus_y);
   void           ext_set_focus (ohm::archi::Float64 focus_x, ohm::archi::Float64 focus_y);
   bool           focus_changed () const;
   ohm::archi::Float64
                  get_focus_x () const;
   ohm::archi::Float64
                  get_focus_y () const;
   
   bool           start_session ();
   void           commit_session ();
   void           revert_session ();
   void           session_set_focus (ohm::archi::Float64 focus_x, ohm::archi::Float64 focus_y);


   
/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   ohm::flip::Float64
                  _focus_x;
   ohm::flip::Float64
                  _focus_y;

   ohm::flip::TxSessionGuard
                  _tx_session_guard;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ViewInfo ();
                  ViewInfo (const ViewInfo &other);
   ViewInfo &  operator = (const ViewInfo &other);
   bool           operator == (const ViewInfo &other);
   bool           operator != (const ViewInfo &other);

}; // class ViewInfo



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace model



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "model/ViewInfo.hpp"



#endif   // model_ViewInfo_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
