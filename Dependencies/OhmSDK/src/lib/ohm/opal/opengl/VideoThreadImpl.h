/*****************************************************************************

        VideoThreadImpl.h
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 62309

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



#if ! defined (ohm_opal_VideoThreadImpl_HEADER_INCLUDED)
#define  ohm_opal_VideoThreadImpl_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/def.h"
#include "ohm/opa/Rect.h"
#include "ohm/opal/opengl/OpenGLContext.h"
#include "ohm/sys/ThreadMgr.h"
#include "ohm/task/Observer.h"
#include "ohm/task/RunLoopOnce.h"

#include <set>

#include <CoreVideo/CoreVideo.h>



namespace ohm
{
namespace opal
{



class VideoThread;
class View;

class VideoThreadImpl
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  VideoThreadImpl (VideoThread & parent);
   virtual        ~VideoThreadImpl ();
   
   void           run ();
   void           stop ();
   
   void           set_nbr_skip_frame (int nbr);

   void           add (View & view);
   void           deactivate (View & view);
   void           remove (View & view);
   void           update (View & view);
   void           invalidate (View & view, const opa::Rect & rect);
   void           request_vbr ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum EventType
   {
                  EventType_ADD_VIEW = 0,
                  // View *

                  EventType_DEACTIVATE_VIEW,
                  // View *

                  EventType_REMOVE_VIEW,
                  // View *

                  EventType_UPDATE_VIEW,
                  // View *
                  // opa::Size

                  EventType_REQ_VBR,
                  // nothing
   };
   
   enum State
   {
                  State_STOPPED = 0,
                  State_RUNNING,
                  State_STOPPING,
   };
   
   typedef std::set <View *> ViewPtrSet;
   
   void           process (task::Event & event);
   void           process_add_view (task::Event & event);
   void           process_deactivate_view (task::Event & event);
   void           process_remove_view (task::Event & event);
   void           process_update_view (task::Event & event);
   void           process_request_vbr (task::Event & event);
   
   void           skip_frames ();
   
   static CVReturn
                  handle_display_link_proc (CVDisplayLinkRef display_link_ref, const CVTimeStamp * now_ptr, const CVTimeStamp * due_time_ptr, CVOptionFlags in_flags, CVOptionFlags * out_flags_ptr, void * user_info);
   CVReturn       handle_display_link (CVDisplayLinkRef display_link_ref, const CVTimeStamp & now, const CVTimeStamp & due_time, CVOptionFlags in_flags, CVOptionFlags & out_flags);
   
   VideoThread &  _parent;
   CVDisplayLinkRef
                  _display_link_ref;
   sys::ThreadMgr::IdType
                  _thread_id;
   
   // Video Thread
   State          _state;
   sys::ThreadSync
                  _run_sync;
   sys::ThreadSync
                  _stop_sync;
   task::RunLoopOnce
                  _run_loop;
   task::Observer _observer;
   
   ViewPtrSet     _view_ptr_set;
   
   int            _nbr_skip_frame;
   double         _refresh_period;
   
   int            _nbr_req_vbr;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  VideoThreadImpl ();
                  VideoThreadImpl (const VideoThreadImpl &other);
   VideoThreadImpl &
                  operator = (const VideoThreadImpl &other);
   bool           operator == (const VideoThreadImpl &other);
   bool           operator != (const VideoThreadImpl &other);

}; // class VideoThreadImpl



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_VideoThreadImpl_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
