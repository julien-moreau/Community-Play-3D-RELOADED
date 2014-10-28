/*****************************************************************************

        ConductMarquee.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70477

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



#if ! defined (client_ConductMarquee_HEADER_INCLUDED)
#define  client_ConductMarquee_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Rect.h"
#include "ohm/opal/base/ConductBase.h"

#include <list>
#include <vector>
#include <set>



namespace ohm
{
namespace opal
{
class Event;
}
}



namespace model
{
class SpacialObject;
}



namespace client
{



class ConductMarquee
:  public ohm::opal::ConductBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  ConductMarquee ();
   virtual        ~ConductMarquee ();
   
   void           begin_collecting (const ohm::opa::Rect & marquee_rect);
   bool           is_collecting () const;
   void           end_collecting ();
   
   bool           is_intersecting (const ohm::opa::Rect & rect);
   void           push_intersection (const ohm::opa::Rect & rect);
   void           pop_intersection ();
   
   void           add (model::SpacialObject & object);
   
   const std::set <model::SpacialObject *> &
                  use_object_ptr_set () const;
   
   // opal::ConductBase
   virtual bool   is_spatial () const;
   void           bind (ohm::opal::Event & event);
   virtual void   invalidate ();
   
   bool           _toggle_flag;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   typedef std::set <model::SpacialObject *>    SpacialObjectPtrSet;
   
   void           update ();
   
   ohm::opal::Event *
                  _event_ptr;
   
   bool           _collecting_flag;
   
   std::vector <ohm::opa::Rect>
                  _view_relative_marquee_rect_arr;
   bool           _marquee_rect_need_update_flag;
   ohm::opa::Rect _marquee_rect;
   
   SpacialObjectPtrSet
                  _object_ptr_set;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ConductMarquee (const ConductMarquee &other);
   ConductMarquee &  operator = (const ConductMarquee &other);
   bool           operator == (const ConductMarquee &other);
   bool           operator != (const ConductMarquee &other);

}; // class ConductMarquee



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // client_ConductMarquee_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
