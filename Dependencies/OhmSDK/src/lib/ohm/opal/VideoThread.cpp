/*****************************************************************************

        VideoThread.cpp
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



#if defined (_MSC_VER)
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/VideoThread.h"
#include "ohm/opal/ImageLoader.h"
#include "ohm/opal/Cursor.h"
#include "ohm/opa/Point.h"
#include "ohm/opal/Bitmap.h"
#include "ohm/opal/ImageManager.h"
#include "ohm/opal/FontManager.h"
#include "ohm/opal/freetype/FTLibrary.h"
#include "ohm/opal/private/Clock.h"
#include "ohm/opak/private/Animator.h"
#include "ohm/lang/fnc.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
Thread : Main
==============================================================================
*/

VideoThread::~VideoThread ()
{
   try
   {
   }
   catch (...)
   {
   }
}



/*
==============================================================================
Name : run
Thread : Main
Description :
   Start the video thread. The function returns when the thread is actually
   running and ready to process requests.
==============================================================================
*/

void  VideoThread::run ()
{
   _impl.run ();
}



/*
==============================================================================
Name : stop
Thread : Main
Description :
   Stop the video thread. The function returns when the thread is actually
   stopped.
==============================================================================
*/

void  VideoThread::stop ()
{
   _impl.stop ();

   FTLibrary::use_instance ().unregister_user_input_thread ();
}



/*
==============================================================================
Name : set_nbr_skip_frame
Description :
   Set the nbr of skipped frame to reduce power GPU consumption.
   Typical values are :
   0 : run at nominal main screen refresh period
   1 : run at half the nominal main screen refresh period
   3 : run at quarter the nominal main screen refresh period
==============================================================================
*/

void  VideoThread::set_nbr_skip_frame (int nbr)
{
   assert (nbr >= 0);
   
   _impl.set_nbr_skip_frame (nbr);

   // thread-safe, as this is not a critical operation
   opak::Animator::use_instance ()._nbr_skip_frame = nbr;
}



/*
==============================================================================
Name : add
Thread : Main
Description :
   Notify the video thread that a new view need to be inited.
   It will init the view in the video thread.
   The function returns once the view is inited in the video thread.
==============================================================================
*/

void  VideoThread::add (View & view)
{
   _impl.add (view);
}



/*
==============================================================================
Name : deactivate
Thread : Main
Description :
   Notify the video thread that a view need not to be called again.
==============================================================================
*/

void  VideoThread::deactivate (View & view)
{
   _impl.deactivate (view);
   
   _deactivate_sync.wait ();
}



/*
==============================================================================
Name : remove
Thread : Main
Description :
   Notify the video thread that a view need to be restored.
   It will restore the view in the video thread.
   The function returns once the view is restored in the video thread.
==============================================================================
*/

void  VideoThread::remove (View & view)
{
   _impl.remove (view);
}



/*
==============================================================================
Name : update
Thread : Main
Description :
   Notify the video thread that the view needs a context update in the video
   thread. This is triggered on init, and when the view is moved or resized.
==============================================================================
*/

void  VideoThread::update (View & view)
{
   _impl.update (view);
}



/*
==============================================================================
Name : invalidate
Thread : Main
Description :
   Notify the video thread that a part of the view needs to be invalidated,
   coming from an external system related event.
   This notification is only meaningful in the case where the system does
   not have a compositing engine, that is in particular, does not have a
   layerbacked representation of the view.
==============================================================================
*/

void  VideoThread::invalidate (View & view, const opa::Rect & rect)
{
   _impl.invalidate (view, rect);
}



/*
==============================================================================
Name : request_vbr
Thread : Main
Description :
   Notify the video thread that vbr main thread sync is requested for the
   next processed frame.
==============================================================================
*/

void  VideoThread::request_vbr ()
{
   _impl.request_vbr ();
}



/*
==============================================================================
Name : wait_vbr
Thread : Main
==============================================================================
*/

void  VideoThread::wait_vbr ()
{
   _vbr_sync.wait ();
}



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : init
Thread : Video
Description :
   Allocate the ressources needed in the video thread.
==============================================================================
*/

void  VideoThread::init (double time_s)
{
   FontManager::use_instance ().register_video_thread ();
   FTLibrary::use_instance ().register_video_thread ();
   Clock::use_instance ().init (time_s);
            
   opak::Animator::use_instance ().init ();
}



/*
==============================================================================
Name : restore
Thread : Video
Description :
   Deallocate the ressources needed in the video thread.
==============================================================================
*/

void  VideoThread::restore ()
{
   opak::Animator::use_instance ().restore ();
   
   FontManager::use_instance ().release_all ();
   FTLibrary::use_instance ().unregister_video_thread ();
   ImageManager::use_instance ().release_all ();
}



/*
==============================================================================
Name : tick
Thread : Video
Description :
   Receive the tick from '_impl'
==============================================================================
*/

void  VideoThread::tick (double time_s)
{
   // will trigger invalidations
   Clock::use_instance ().tick (time_s);
}



/*
==============================================================================
Name : notify_vbr
Thread : Video
Description :
   The vertical retrace just happened
==============================================================================
*/

void  VideoThread::notify_vbr ()
{
   _vbr_sync.signal ();
}



/*
==============================================================================
Name : notify_deactivate
Thread : Video
Description :
   The view was removed from the view queue.
==============================================================================
*/

void  VideoThread::notify_deactivate ()
{
   _deactivate_sync.signal ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

VideoThread::VideoThread ()
:  _impl (*this)
{
   FontManager::use_instance ().register_user_input_thread ();
   FTLibrary::use_instance ().register_user_input_thread ();
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
