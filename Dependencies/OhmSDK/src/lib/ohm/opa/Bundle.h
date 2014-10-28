/*****************************************************************************

        Bundle.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 53796

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



#if ! defined (ohm_opa_Bundle_HEADER_INCLUDED)
#define  ohm_opa_Bundle_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/Frame.h"
#include "ohm/util/Delegate.h"

#include <vector>
#include <set>



namespace ohm
{
namespace opal
{
class View;
}
namespace opak
{
class Primitive;
class PrimitiveContainer;
class View;
}



namespace opa
{



class EventBase;

class Bundle
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  Bundle (const char * name_0);
                  Bundle (Bundle & parent_bundle, const char * name_0);
                  Bundle (opal::View & opal_view, opak::View & opak_view);
   virtual        ~Bundle ();
   
   bool           has_parent () const;
   Bundle &       get_parent ();
   size_t         get_parent_slot_nbr ();
   template <class T>
   T &            get_ancestor ();
   template <class T>
   T *            get_ancestor_ptr ();

   const char *   get_name () const;
   
   void           set_embeddable (opal::Frame & frame);
   void           set_embeddable (opak::Primitive & prim);
   void           set_nbr_slot (size_t nbr_slot);
   void           set_slot (size_t slot_nbr, opal::Frame & frame);
   void           set_slot (size_t slot_nbr, opak::PrimitiveContainer & layer);
   void           unset_all ();
   
   void           embed (Bundle & bundle, size_t slot_nbr = 0);
   void           unembed (Bundle & bundle);
   void           unembed_immediate (Bundle & bundle);
   
   bool           has_embeddable_frame () const;
   opal::Frame &  get_embeddable_frame ();
   const opal::Frame &
                  get_embeddable_frame () const;
   bool           has_embeddable_primitive () const;
   opak::Primitive &
                  get_embeddable_primitive ();
   const opak::Primitive &
                  get_embeddable_primitive () const;

   opal::Frame &  get_slot_frame (size_t slot_nbr = 0);
   opak::PrimitiveContainer &
                  get_slot_layer (size_t slot_nbr = 0);
   
   template <class T, class EventType, void (T::*handle_event) (EventType &)>
   void           bind ();
   void           process_event (EventBase & event);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   typedef std::vector <opal::Frame *> FramePtrArr;
   typedef std::vector <opak::PrimitiveContainer *> LayerPtrArr;
   typedef std::set <Bundle *> BundlePtrSet;
   typedef std::set <util::Delegate> DelegateSet;
   
   void           set_parent (Bundle & parent_bundle, size_t slot_nbr = 0);
   void           set_no_parent ();

   Bundle *       _parent_ptr;
   size_t         _parent_slot_nbr;
   const char *   _name_0;
   
   BundlePtrSet   _bundle_ptr_set;
   
   opal::Frame *  _embeddable_frame_ptr;
   opak::Primitive *
                  _embeddable_prim_ptr;
   
   FramePtrArr    _slot_frame_ptr_arr;
   LayerPtrArr    _slot_layer_ptr_arr;
   
   DelegateSet    _delegate_set;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Bundle ();
                  Bundle (const Bundle &other);
   Bundle &       operator = (const Bundle &other);
   bool           operator == (const Bundle &other);
   bool           operator != (const Bundle &other);

}; // class Bundle



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Bundle.hpp"



#endif   // ohm_opa_Bundle_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
