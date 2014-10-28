/*****************************************************************************

        TrackerBase.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 64317

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



#if ! defined (ohm_opal_TrackerBase_HEADER_INCLUDED)
#define  ohm_opal_TrackerBase_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace opal
{



class Frame;
class Event;

class TrackerBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  TrackerBase (Frame & frame);
   virtual        ~TrackerBase ();
   
   void           enable_anti_bad_move (bool enable_flag = true);
   
   bool           is_tracking () const;

   void           start (Event & event);
   void           track (Event & event);
   void           key_changed (Event & event);
   void           stop (Event & event);
   void           stop_revert ();
   void           mouse_target_lost ();

   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   virtual bool   do_start (Event & event) = 0;
   virtual void   do_track (Event & event) = 0;
   virtual void   do_key_changed (Event & event) {};
   virtual void   do_commit (Event & event) = 0;
   virtual void   do_revert () = 0;
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum State
   {
                  State_VOID = 0,
                  State_BEGIN_TRACKING,
                  State_TRACKING,
                  State_STOPPING,
                  State_REVERTING,
   };
   
   Frame &        _frame;
   State          _state;
   bool           _anti_bad_move_flag;
   double         _start_time;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  TrackerBase ();
                  TrackerBase (const TrackerBase &other);
   TrackerBase &  operator = (const TrackerBase &other);
   bool           operator == (const TrackerBase &other);
   bool           operator != (const TrackerBase &other);

}; // class TrackerBase



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/opal/base/TrackerBase.hpp"



#endif   // ohm_opal_TrackerBase_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
