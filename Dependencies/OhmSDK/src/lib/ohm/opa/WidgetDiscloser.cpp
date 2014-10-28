/*****************************************************************************

        WidgetDiscloser.cpp
        Copyright (c) 2009 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 50568

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
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/WidgetDiscloser.h"
#include "ohm/opa/WidgetLabel.h"
#include "ohm/opal/base/FrameSignal.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

WidgetDiscloser::~WidgetDiscloser ()
{
   try
   {
      // explicitely call frame reset :
      // if an edit is ungoing, this is going to release
      // the mouse capture which in turn will send an
      // event to this widget - therefore we want primitives
      // and delegate to still exist.
      
      _frame_toggle_aptr.reset ();
   }
   catch (...)
   {
   }
}



/*
==============================================================================
Name : set_label
==============================================================================
*/

void WidgetDiscloser::set_label (const std::string & label)
{
   _label_sptr = WidgetLabel::create ();
   _label_sptr->set_rect (opa::Rect (24, 6, 100, 20));
   _label_sptr->set_label (label);
   embed (*_label_sptr, Slot_DECORATION);
   update_size ();
}



/*
==============================================================================
Name : req_gesture_signal
==============================================================================
*/

void WidgetDiscloser::req_gesture_signal (opal::FrameSignal::Event & event)
{
   const float time = .08f;
   
   _flag_zone_displayed = !_flag_zone_displayed;

   if (_flag_zone_displayed)
   {
      _image_aptr->_image
         .stop ()
         .push ("ohm.widget.discloser.disclose-2.png", 0)
         .push ("ohm.widget.discloser.disclose-3.png", time)
         .push ("ohm.widget.discloser.disclose-4.png", time)
         .push ("ohm.widget.discloser.disclose-5.png", time)
         .push ("ohm.widget.discloser.disclose-6.png", time);
      
      _zone_frame_aptr->activate ();
      _zone_layer_aptr->_visibility = true;
   }
   else 
   {
      _image_aptr->_image
         .stop ()
         .push ("ohm.widget.discloser.disclose-5.png", 0)
         .push ("ohm.widget.discloser.disclose-4.png", time)
         .push ("ohm.widget.discloser.disclose-3.png", time)
         .push ("ohm.widget.discloser.disclose-2.png", time)
         .push ("ohm.widget.discloser.disclose-1.png", time);
         
      _zone_frame_aptr->deactivate ();
      _zone_layer_aptr->_visibility = false;
   }  
   
   update_size ();
}



/*
==============================================================================
Name : req_child_set_bounds
==============================================================================
*/

/*void WidgetDiscloser::req_child_set_bounds (WidgetBase & widget, const Size & bounds)
{
   widget.set_bounds (bounds);
   compute_zone_rect ();
   update_size ();
}*/



/*
==============================================================================
Name : layout
==============================================================================
*/

bool  WidgetDiscloser::layout (const opa::Size & bounds)
{
   /*Point pt;
   box.push (
      pt,
      opa::Rect (0, -11, 11, 13),
      opa::Margin (5, 5, 5, 5),
      0.f
   );
   
   _frame_aptr->set_origin (pt);
   //_prim_aptr->_origin = pt;*/
   
   return false;
}



/*
==============================================================================
Name : get_min_bbox
==============================================================================
*/

void  WidgetDiscloser::get_min_bbox (Rect & min_bbox)
{
}



/*
==============================================================================
Name : get_margin
==============================================================================
*/

void  WidgetDiscloser::get_margin (Margin & margin)
{
   
}



/*
==============================================================================
Name : get_weight
==============================================================================
*/

void  WidgetDiscloser::get_weight (float & weight)
{
   
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

WidgetDiscloser::WidgetDiscloser ()
:  _flag_zone_displayed (true)
,  _zone_rect (Rect::_zero)
{
   _frame_aptr = new opal::Frame ("WidgetDiscloser::frame");
   _frame_aptr->set_bounds (opa::Size::_zero);
   
   _frame_toggle_aptr = new opal::FrameSignal;
   
   _frame_toggle_aptr->bind_gesture_signal <
      WidgetDiscloser, &WidgetDiscloser::req_gesture_signal
   > (*this);
   
   _frame_toggle_aptr->set_bounds (opa::Size (18, 18));
   _frame_toggle_aptr->set_origin (opa::Point (2, 2));
   
   _layer_aptr = new opak::Layer;
   _layer_aptr->set_name ("WidgetDiscloser::layer");
   
   _zone_frame_aptr = new opal::Frame ("WidgetDiscloser::zone_frame");
   _zone_frame_aptr->set_bounds (opa::Size::_zero);
   _zone_frame_aptr->set_origin (opa::Point (2, 22));
   
   _zone_layer_aptr = new opak::Layer;
   _zone_layer_aptr->set_name ("WidgetDiscloser::zone_layer");
   _zone_layer_aptr->_origin = opa::Point (2, 22);
   
   _image_aptr = new opak::Image;
   _image_aptr->_origin = Point (2, 2);
   _image_aptr->_image = "ohm.widget.discloser.disclose-6.png";
   
   _bkg_aptr = new opak::RectFilled;
   _bkg_aptr->_color = opal::Color (.3f, 0.f);
   
   _frame_aptr->insert_top (*_zone_frame_aptr);
   _frame_aptr->insert_top (*_frame_toggle_aptr);
   _layer_aptr->push_back (*_bkg_aptr);
   _layer_aptr->push_back (*_zone_layer_aptr);
   _layer_aptr->push_back (*_image_aptr);

   set_embeddable (*_frame_aptr);
   set_embeddable (*_layer_aptr);
   
   set_nbr_slot (Slot_NBR_ELT);
   set_slot (Slot_ZONE, *_zone_frame_aptr);
   set_slot (Slot_ZONE, *_zone_layer_aptr);
   set_slot (Slot_DECORATION, *_frame_aptr);
   set_slot (Slot_DECORATION, *_layer_aptr); 
}



/*
==============================================================================
Name : do_set_bounds
==============================================================================
*/

void WidgetDiscloser::do_set_bounds (const Size & bounds)
{
   _frame_aptr->set_bounds (bounds);
   _bkg_aptr->_size = bounds;
}



/*
==============================================================================
Name : do_enable
==============================================================================
*/

void WidgetDiscloser::do_enable (bool enabled_flag)
{
}



/*
==============================================================================
Name : do_update
==============================================================================
*/

/*void WidgetDiscloser::do_update (iterator it)
{
   compute_zone_rect ();
   _zone_frame_aptr->set_bounds (_zone_rect._size);
   update_size ();
}*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : compute_zone_rect
==============================================================================
*/

void WidgetDiscloser::compute_zone_rect ()
{
   _zone_rect = opa::Rect::_zero;
   
   /*iterator it = begin ();
   iterator it_end = end ();
   
   for (; it != it_end ; ++it)
   {
      WidgetBase & widget = *it;
      
      _zone_rect |= widget.get_rect ();
   }*/
   
   _zone_rect._origin = _zone_frame_aptr->get_origin ();
}



/*
==============================================================================
Name : update_size
==============================================================================
*/

void WidgetDiscloser::update_size ()
{
   Rect rect (0, 0, 18, 18);
   
   if (_label_sptr.get () != 0)
   {
      Rect label_rect = _label_sptr->get_rect ();
      rect |= label_rect;
   }
   
   if (_flag_zone_displayed)
   {
      rect |= _zone_rect;
   }
   
   //req_set_bounds (rect._size);
   
   invalidate_layout ();
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

