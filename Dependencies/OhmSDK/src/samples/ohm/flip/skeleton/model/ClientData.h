/*****************************************************************************

        ClientData.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71440

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



#if ! defined (model_ClientData_HEADER_INCLUDED)
#define  model_ClientData_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "model/Selection.h"
#include "model/Tool.h"
#include "model/ViewInfo.h"

#include "ohm/flip/Blob.h"
#include "ohm/flip/Cue.h"
#include "ohm/flip/Enum.h"
#include "ohm/flip/Object.h"
#include "ohm/flip/Scribe.h"
#include "ohm/flip/TxSessionGuard.h"



namespace model
{



class ClientData
:  public ohm::flip::Object
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum Signal
   {
                  Signal_PRESENCE_CHANGED_SESSION = 0,
   };
   
   enum ToolState
   {
                  ToolState_PRIMARY = 0,
                  ToolState_MOMENTARY,
                  
                  NBR_ToolState_TOTAL
   };
   
   static void    declare ();
   static ClientData &
                  use_self (ohm::flip::Object & obj);

                  ClientData (ohm::flip::DocumentBase & document);
   virtual        ~ClientData ();

   bool           operator < (const ClientData & other);

   // flip::Object
   virtual void   ctor (ohm::flip::Args & args);
   virtual void   pre_sync ();
   virtual void   post_sync ();
   
   std::string    get_user_name () const;

   void           tx_set_primary_tool (Tool tool);
   void           tx_set_momentary_tool (Tool tool);
   void           join_set_momentary_tool (Tool tool);
   void           tx_set_primary_tool ();
   void           join_set_primary_tool ();
   void           ext_set_primary_tool ();
   bool           tool_changed () const;
   Tool           get_tool ();
   
   Selection &    use_selection ();
   const Selection &
                  use_selection () const;

   ViewInfo &     use_view_info ();
   const ViewInfo &
                  use_view_info () const;
   
   // convenience functions
   void           tx_delete_selection ();
   void           copy_selection ();
   void           tx_paste ();
   void           undo (std::string & name);
   void           redo (std::string & name);
   void           tx_select_all ();


   
/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           ext_set_primary_tool (Tool tool);
   void           ext_set_momentary_tool (Tool tool);
   void           ext_set_tool_state (ToolState state);
   
   void           tx_paste_spacial_object ();
   void           ext_paste_spacial_object (size_t & taxi_nbr, size_t & pedestrian_nbr);

   void           update () const;

   ohm::flip::Blob
                  _user_name;
   
   ohm::flip::Cue _primary_tool_cue;
   mutable Tool   _primary_tool;
   Tool           _old_primary_tool;
   ohm::flip::Cue _momentary_tool_cue;
   mutable Tool   _momentary_tool;
   Tool           _old_momentary_tool;
   ohm::flip::Cue _tool_state_cue;
   mutable ToolState
                  _tool_state;
   ToolState      _old_tool_state;

   Selection      _selection;
   ViewInfo       _view_info;

   ohm::flip::TxSessionGuard
                  _tx_session_guard;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ClientData ();
                  ClientData (const ClientData &other);
   ClientData &   operator = (const ClientData &other);
   bool           operator == (const ClientData &other);
   bool           operator != (const ClientData &other);

}; // class ClientData



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace model



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "model/ClientData.hpp"



#endif   // model_ClientData_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
