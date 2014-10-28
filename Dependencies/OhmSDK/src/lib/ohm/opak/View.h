/*****************************************************************************

        View.h
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 45537

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



#if ! defined (ohm_opak_View_HEADER_INCLUDED)
#define  ohm_opak_View_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opak/Layer.h"
#include "ohm/opal/Primitive.h"
#include "ohm/sys/ThreadSync.h"
#include "ohm/task/Observer.h"



namespace ohm
{
namespace opal
{
class View;
class Frame;
class Event;
}



namespace opak
{



class View
:  public opak::Primitive
,  public opal::Primitive
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   static View &  create (opal::View & view);
   static void    release (View & view);
   
   void           run ();
   void           clear ();
   
   // opak::Primitive
   virtual void   stop ();
   virtual bool   is_animating ();
   virtual void   display (GraphicContext & context);
   virtual void   invalidate (const opa::Rect & rect, const opa::Margin & margin);
   
   // opal::Primitive
   virtual void   draw (opal::GraphicContextInterface & context);
   virtual void   request_invalidate_all ();
   
   // Root

   AutoPrimPtr <Layer>
                  _layer_aptr;
   
   // Video Thread
   static void    do_create_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr);
   static void    do_clear_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr);
   static void    do_stop_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr);
   
   void           do_create ();
   void           do_clear ();
   virtual void   do_stop ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
                  View (opal::View & view);
   virtual        ~View ();
   
   virtual void   invalidate ();
   virtual void   bounding_box_changed ();
   virtual void   do_propagate_child_bounding_box_changed ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           post_process ();
   
   opal::View &   _view;
   task::Observer _observer;
   
   sys::ThreadSync
                  _clear_sync;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  View ();
                  View (const View &other);
   View &         operator = (const View &other);
   bool           operator == (const View &other);
   bool           operator != (const View &other);

}; // class View



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/opak/View.hpp"



#endif   // ohm_opak_View_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
