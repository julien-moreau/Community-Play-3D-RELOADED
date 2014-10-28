/*****************************************************************************

        Debugger.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 48853

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



#if ! defined (ohm_opa_Debugger_HEADER_INCLUDED)
#define  ohm_opa_Debugger_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/util/SingletonBase.h"
#include "ohm/opal/Event.h"

#include <list>

#if defined (OHM_OPA_DEBUGGER)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      // OHM_OPA_DEBUGGER is made explicitely incompatible with OHM_SHIPPING_LEVEL_CODE
      #error OHM_OPA_DEBUGGER is defined in shipping level code !
   #endif
#endif



namespace ohm
{
namespace opal
{
class Frame;
}
namespace opak
{
class Primitive;
class PrimitiveContainer;
class GraphicContext;
}



namespace opa
{



class Debugger
:  public util::SingletonBase <Debugger>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~Debugger ();
   
   static void    enable_frame_break (bool enable_flag = true);
   static void    break_on (opal::Frame & frame, void (opal::Frame::*method) ());
   static void    break_on (opal::Frame & frame, void (opal::Frame::*method) (opal::Frame &));
   static void    break_on (opal::Frame & frame, void (opal::Frame::*method) (opal::Event &));
   
   bool           has_break (opal::Frame & frame, void (opal::Frame::*method) ()) const;
   bool           has_break (opal::Frame & frame, void (opal::Frame::*method) (opal::Frame &)) const;
   bool           has_break (opal::Frame & frame, void (opal::Frame::*method) (opal::Event &)) const;
   
   static void    enable_events_trace (bool enable_flag = true);
   bool           is_tracing_events () const;
   
   static void    enable_frame_trace (bool enable_flag = true);
   static void    trace (opal::Event::Type type, bool enable_flag = true);
   bool           is_tracing (opal::Event::Type type) const;

   static void    enable_prim_break (bool enable_flag = true);
   static void    break_on (opak::Primitive & primitive, void (opak::PrimitiveContainer::*method) (opak::Primitive *));
   static void    break_on (opak::Primitive & primitive, void (opak::PrimitiveContainer::*method) (opak::Primitive *, bool));
   static void    break_on (opak::Primitive & primitive, void (opak::PrimitiveContainer::*method) (opak::GraphicContext &));
   static void    break_on (opak::Primitive & primitive, void (opak::Primitive::*method) ());
   static void    break_on (opak::Primitive & primitive, void (opak::Primitive::*method) (bool));
   static void    break_on (opak::Primitive & primitive, void (opak::Primitive::*method) (opak::Primitive &));

   bool           has_break (opak::Primitive & primitive, void (opak::PrimitiveContainer::*method) (opak::Primitive *)) const;
   bool           has_break (opak::Primitive & primitive, void (opak::PrimitiveContainer::*method) (opak::Primitive *, bool)) const;
   bool           has_break (opak::Primitive & primitive, void (opak::PrimitiveContainer::*method) (opak::GraphicContext &)) const;
   bool           has_break (opak::Primitive & primitive, void (opak::Primitive::*method) ()) const;
   bool           has_break (opak::Primitive & primitive, void (opak::Primitive::*method) (bool)) const;
   bool           has_break (opak::Primitive & primitive, void (opak::Primitive::*method) (opak::Primitive &)) const;

   static void    enable_prim_trace (bool enable_flag = true);
   static void    trace_prim_display (bool enable_flag = true);
   bool           is_tracing_prim_display () const;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  Debugger ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   ohm_util_SingletonBase_INVITE_SINGLETON (Debugger)
      
   typedef std::pair <opal::Frame *, void (opal::Frame::*) ()> FramePtrMethodVoidPair;
   typedef std::pair <opal::Frame *, void (opal::Frame::*) (opal::Frame &)> FramePtrMethodFramePair;
   typedef std::pair <opal::Frame *, void (opal::Frame::*) (opal::Event &)> FramePtrMethodEventPair;
   
   typedef std::list <FramePtrMethodVoidPair> FramePtrMethodVoidPairList;
   typedef std::list <FramePtrMethodFramePair> FramePtrMethodFramePairList;
   typedef std::list <FramePtrMethodEventPair> FramePtrMethodEventPairList;

   typedef std::pair <opak::Primitive *, void (opak::PrimitiveContainer::*) (opak::Primitive *)> PrimPtrContainerMethodPrimPair;
   typedef std::pair <opak::Primitive *, void (opak::PrimitiveContainer::*) (opak::Primitive *, bool)> PrimPtrContainerMethodPrimBoolPair;
   typedef std::pair <opak::Primitive *, void (opak::PrimitiveContainer::*) (opak::GraphicContext &)> PrimPtrContainerMethodGraphicContextPair;
   typedef std::pair <opak::Primitive *, void (opak::Primitive::*) ()> PrimPtrMethodVoidPair;
   typedef std::pair <opak::Primitive *, void (opak::Primitive::*) (bool)> PrimPtrMethodBoolPair;
   typedef std::pair <opak::Primitive *, void (opak::Primitive::*) (opak::Primitive &)> PrimPtrMethodPrimPair;

   typedef std::list <PrimPtrContainerMethodPrimPair> PrimPtrContainerMethodPrimPairList;
   typedef std::list <PrimPtrContainerMethodPrimBoolPair> PrimPtrContainerMethodPrimBoolPairList;
   typedef std::list <PrimPtrContainerMethodGraphicContextPair> PrimPtrContainerMethodGraphicContextPairList;
   typedef std::list <PrimPtrMethodVoidPair> PrimPtrMethodVoidPairList;
   typedef std::list <PrimPtrMethodBoolPair> PrimPtrMethodBoolPairList;
   typedef std::list <PrimPtrMethodPrimPair> PrimPtrMethodPrimPairList;
   
   void           do_enable_events_trace (bool enable_flag);

   void           do_enable_frame_break (bool enable_flag);
   void           do_break_on (opal::Frame & frame, void (opal::Frame::*method) ());
   void           do_break_on (opal::Frame & frame, void (opal::Frame::*method) (opal::Frame &));
   void           do_break_on (opal::Frame & frame, void (opal::Frame::*method) (opal::Event &));

   void           do_enable_frame_trace (bool enable_flag);
   void           do_trace (opal::Event::Type type, bool enable_flag);

   void           do_enable_prim_break (bool enable_flag);
   void           do_break_on (opak::Primitive & primitive, void (opak::PrimitiveContainer::*method) (opak::Primitive *));
   void           do_break_on (opak::Primitive & primitive, void (opak::PrimitiveContainer::*method) (opak::Primitive *, bool));
   void           do_break_on (opak::Primitive & primitive, void (opak::PrimitiveContainer::*method) (opak::GraphicContext &));
   void           do_break_on (opak::Primitive & primitive, void (opak::Primitive::*method) ());
   void           do_break_on (opak::Primitive & primitive, void (opak::Primitive::*method) (bool));
   void           do_break_on (opak::Primitive & primitive, void (opak::Primitive::*method) (opak::Primitive &));

   void           do_enable_prim_trace (bool enable_flag);
   void           do_trace_prim_display (bool enable_flag);
   
   bool           _breaking_frame_flag;
   FramePtrMethodVoidPairList
                  _frame_ptr_method_void_pair_list;
   FramePtrMethodFramePairList
                  _frame_ptr_method_frame_pair_list;
   FramePtrMethodEventPairList
                  _frame_ptr_method_event_pair_list;
   
   bool           _tracing_events_flag;
   bool           _tracing_frame_flag;
   bool           _tracing_event_type_arr [opal::Event::Type_NBR_ELT];

   bool           _breaking_prim_flag;
   PrimPtrContainerMethodPrimPairList
                  _prim_ptr_container_method_prim_pair_list;
   PrimPtrContainerMethodPrimBoolPairList
                  _prim_ptr_container_method_prim_bool_pair_list;
   PrimPtrContainerMethodGraphicContextPairList
                  _prim_ptr_container_method_graphic_context_pair_list;
   PrimPtrMethodVoidPairList
                  _prim_ptr_method_void_pair_list;
   PrimPtrMethodBoolPairList
                  _prim_ptr_method_bool_pair_list;
   PrimPtrMethodPrimPairList
                  _prim_ptr_method_prim_pair_list;
   
   bool           _tracing_prim_flag;
   bool           _tracing_prim_display_flag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Debugger (const Debugger &other);
   Debugger &     operator = (const Debugger &other);
   bool           operator == (const Debugger &other);
   bool           operator != (const Debugger &other);

}; // class Debugger



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opa_Debugger_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
