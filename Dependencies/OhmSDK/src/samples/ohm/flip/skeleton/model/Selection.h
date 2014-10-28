/*****************************************************************************

        Selection.h
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



#if ! defined (model_Selection_HEADER_INCLUDED)
#define  model_Selection_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "model/ObjectRefColl.h"
#include "model/Range.h"
#include "model/Taxi.h"

#include "ohm/flip/TxSessionGuard.h"

#define OHM_STUDIO_MODEL_SELECTION_OPTIMIZED_SELECT
#define OHM_STUDIO_MODEL_SELECTION_PROPAGATE_CHANGE



namespace model
{



class Selection
:  public ohm::flip::Object
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum Signal
   {
                  Signal_START = 0,
                  Signal_END, 
                  Signal_CHANGE,
   };
   
   enum Change
   {
                  Change_COPY = 0,
                  Change_OBJ_MOVE_X,
                  Change_OBJ_MOVE_Y,
                  Change_TAXI_WIDTH,
                  Change_TAXI_COLOR,
   };
   
   static void    declare ();

                  Selection (ohm::flip::DocumentBase & document);
   virtual        ~Selection ();
   
   // ohm::flip::Object
   virtual void   ctor (ohm::flip::Args & args);
   virtual void   pre_sync ();

   void           tx_clear ();
   void           ext_clear ();
   void           tx_delete_content ();
   void           ext_delete_content ();
   void           copy_content ();
   
   void           tx_select (ohm::flip::Object & object, bool toggle_flag);
   template <class InputPtrIterator>
   void           tx_select (InputPtrIterator it, InputPtrIterator it_end, bool toggle_flag);
   template <class InputPtrIterator>
   void           ext_select (InputPtrIterator it, InputPtrIterator it_end, bool toggle_flag);
   template <class InputPtrIterator>
   void           tx_replace (InputPtrIterator it, InputPtrIterator it_end);
   template <class InputPtrIterator>
   void           ext_replace (InputPtrIterator it, InputPtrIterator it_end);
   void           tx_replace (const ohm::flip::Object & object);
   void           ext_replace (const ohm::flip::Object & object);
   template <class UnaryPredicate>
   void           ext_remove_if (UnaryPredicate predicate);
   
   ObjectRefColl::iterator
                  find (const ohm::flip::Object & object, ohm::flip::FindMask find_mask = ohm::flip::FindMask_NATURAL);
   ObjectRefColl::const_iterator
                  find (const ohm::flip::Object & object, ohm::flip::FindMask find_mask = ohm::flip::FindMask_NATURAL) const;
   template <class T>
   ObjectRefColl::iterator
                  find_first_of (ohm::flip::FindMask find_mask = ohm::flip::FindMask_NATURAL);
   template <class T>
   ObjectRefColl::const_iterator
                  find_first_of (ohm::flip::FindMask find_mask = ohm::flip::FindMask_NATURAL) const;

   bool           contains (const ohm::flip::Object & object, ohm::flip::FindMask find_mask = ohm::flip::FindMask_NATURAL) const;
   template <class T>
   bool           contains () const;

   template <class T>
   void           get_object_ptr_arr (std::vector <T *> & ptr_arr);

   bool           match (ohm::flip::Object & object, ohm::flip::Object & other) const;

   bool           accepts (ohm::flip::Object & object) const;
   template <class T>
   bool           accepts () const;

   template <class T>
   size_t         count (ohm::flip::FindMask find_mask = ohm::flip::FindMask_NATURAL) const;

   ObjectRefColl &
                  use ();
   

   void           tx_taxi_move (ohm::archi::Float64 & position_x_delta);
   void           tx_taxi_change_width (ohm::archi::Float64 & width_delta);
   void           tx_taxi_set_color (Taxi::Color color);
   void           tx_pedestrian_move (ohm::archi::Float64 & position_x_delta, ohm::archi::Float64 & position_y_delta);
   
   // ** session **

   bool           can_start_session () const;
   bool           start_session ();
   void           commit_session ();
   void           revert_session ();
   void           session_copy ();

   // instances

   void           session_taxi_move (ohm::archi::Float64 & position_x_delta);
   void           session_taxi_change_width (ohm::archi::Float64 & width_delta);
   void           session_taxi_set_color (Taxi::Color color);
   void           session_pedestrian_move (ohm::archi::Float64 & position_x_delta, ohm::archi::Float64 & position_y_delta);
   
   // ** signals **

   void           signal_start ();
   void           signal_end ();
   void           signal_copy (bool copy_flag);
   
   void           signal_taxi_move (ohm::archi::Float64 & position_x_delta);
   void           signal_taxi_change_width (ohm::archi::Float64 & width_delta);
   void           signal_taxi_set_color (Taxi::Color color);
   void           signal_pedestrian_move (ohm::archi::Float64 & position_x_delta, ohm::archi::Float64 & position_y_delta);

   // ** ranges **
   
   RangeFloat64   get_taxi_width_delta_range () const;

   // ** getters **
   
   Taxi::Color    get_taxi_color () const;

   

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   template <class T>
   class PredicateType
   {
   public:
                  PredicateType ();
      virtual     ~PredicateType () {}
      
      bool        equal (const ObjectRef & object_ref) const;
   };

   class Predicate
   {
   public:
                  Predicate (const ohm::flip::Object & object);
      virtual     ~Predicate () {}
      
      bool        equal (const ObjectRef & object_ref) const;
      
   private:
      const ohm::flip::Object *
                  _object_ptr;
   
   private:
                  Predicate ();
   };

   class ObjPtrIt
   {
   public:
      inline bool operator < (const ObjPtrIt & other) {return _obj_ptr < other._obj_ptr;}
      inline operator
                  ohm::flip::Object * () {return _obj_ptr;}
      
      ohm::flip::Object *
                  _obj_ptr;
      ObjectRefColl::iterator
                  _it;
   };
   
   ObjectRefColl  _coll;
                  
   ohm::flip::TxSessionGuard
                  _tx_session_guard;
   
   // 'contains' / 'accepts' cache
   size_t         _taxi_cnt;
   size_t         _pedestrian_cnt;
   
   bool           _obj_move_flag;
   bool           _taxi_width_change_flag;
   bool           _taxi_color_change_flag;
   bool           _obj_copy_flag;


   
/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Selection ();
                  Selection (const Selection &other);
   Selection &    operator = (const Selection &other);
   bool           operator == (const Selection &other) const;
   bool           operator != (const Selection &other) const;

}; // class Selection



}  // namespace model



#include "model/Selection.hpp"



#endif   // model_Selection_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

