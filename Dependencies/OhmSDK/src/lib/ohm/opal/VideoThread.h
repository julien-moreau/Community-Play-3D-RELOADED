/*****************************************************************************

        VideoThread.h
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



#if ! defined (ohm_opal_VideoThread_HEADER_INCLUDED)
#define  ohm_opal_VideoThread_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/sys/ThreadSync.h"
#include "ohm/util/SingletonBase.h"

#include "ohm/opal/def.h"

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #include "ohm/opal/directx/VideoThreadImpl.h"

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   #if (ohm_opal_API == ohm_opal_API_OPENGL)
      #include "ohm/opal/opengl/VideoThreadImpl.h"
   
   #endif
   
#else
   #error Operating System not supported

#endif



namespace ohm
{
namespace opal
{



class View;

class VideoThread
:  public util::SingletonBase <VideoThread>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   virtual        ~VideoThread ();
   
   void           run ();
   void           stop ();
   
   void           set_nbr_skip_frame (int nbr);
   
   void           add (View & view);
   void           deactivate (View & view);
   void           remove (View & view);
   void           update (View & view);
   void           invalidate (View & view, const opa::Rect & rect);

   void           request_vbr ();
   void           wait_vbr ();



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   void           init (double time_s);
   void           restore ();
   void           tick (double time_s);
   void           notify_vbr ();
   void           notify_deactivate ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
                  VideoThread ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   ohm_util_SingletonBase_INVITE_SINGLETON (VideoThread)
   
   VideoThreadImpl
                  _impl;
   sys::ThreadSync
                  _deactivate_sync;
   sys::ThreadSync
                  _vbr_sync;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  VideoThread (const VideoThread &other);
   VideoThread &  operator = (const VideoThread &other);
   bool           operator == (const VideoThread &other);
   bool           operator != (const VideoThread &other);

}; // class VideoThread



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_VideoThread_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
