/*****************************************************************************

        ObjectTravelAgency.hpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 36325

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



#if defined (ohm_flip_ObjectTravelAgency_CURRENT_CODEHEADER)
   #error Recursive inclusion of ObjectTravelAgency code header.
#endif
#define  ohm_flip_ObjectTravelAgency_CURRENT_CODEHEADER

#if ! defined (ohm_flip_ObjectTravelAgency_CODEHEADER_INCLUDED)
#define  ohm_flip_ObjectTravelAgency_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/ClassDescription.h"
#include "ohm/flip/private/TravelerObjectCollector.h"



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : travel
==============================================================================
*/

template <class UTF>
void  ObjectTravelAgency::travel (flip::Ref ref, archi::UInt32 flags, UTF & unary_traveler_function)
{
   bool normal_flag = ! (flags & Flag_REVERSE);
   bool follow_containers_flag = (flags & Flag_FOLLOW_CONTAINERS) != 0;
   bool depth_first_flag = (flags & Flag_DEPTH_FIRST) != 0;
   _first_flag = (flags & Flag_SKIP_FIRST) != 0;
   
   assert (! (depth_first_flag && follow_containers_flag));
   
   if (normal_flag)
   {
      if (follow_containers_flag)
      {
         travel_normal_follow_containers (ref, unary_traveler_function);
      }
      else if (depth_first_flag)
      {
         travel_normal_depth_first (ref, unary_traveler_function);
      }
      else
      {
         travel_normal (ref, unary_traveler_function);
      }
   }
   else
   {
      if (follow_containers_flag)
      {
         travel_reverse_follow_containers (ref, unary_traveler_function);
      }
      else if (depth_first_flag)
      {
         travel_reverse_depth_first (ref, unary_traveler_function);
      }
      else
      {
         travel_reverse (ref, unary_traveler_function);
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : travel_normal
==============================================================================
*/

template <class UTF>
void  ObjectTravelAgency::travel_normal (flip::Ref ref, UTF & unary_traveler_function)
{
   Object * obj_ptr = _object_manager.get_object (ref);
   assert (obj_ptr != 0);
   
   if (obj_ptr != 0)
   {
      ClassDescriptionMaster & cdm = obj_ptr->get_cdm ();
      
      archi::Int64 nbr_ref = cdm.get_nbr_ref ();
      
      for (archi::Int64 i = 0 ; i < nbr_ref ; ++i)
      {
         flip::Ref sub_ref = ref + i;
         
         execute (sub_ref, unary_traveler_function);
      }
   }
}



/*
==============================================================================
Name : travel_reverse
==============================================================================
*/

template <class UTF>
void  ObjectTravelAgency::travel_reverse (flip::Ref ref, UTF & unary_traveler_function)
{
   Object * obj_ptr = _object_manager.get_object (ref);
   assert (obj_ptr != 0);
   
   if (obj_ptr != 0)
   {
      ClassDescriptionMaster & cdm = obj_ptr->get_cdm ();
      
      archi::Int64 nbr_ref = cdm.get_nbr_ref ();
      
      for (archi::Int64 i = nbr_ref - 1 ; i >= 0 ; --i)
      {
         flip::Ref sub_ref = ref + i;
         
         execute (sub_ref, unary_traveler_function);
      }
   }
}



/*
==============================================================================
Name : travel_normal_follow_containers
==============================================================================
*/

template <class UTF>
void  ObjectTravelAgency::travel_normal_follow_containers (flip::Ref ref, UTF & unary_traveler_function)
{
   execute (ref, unary_traveler_function);
   
   Object * obj_ptr = _object_manager.get_object (ref);
   assert (obj_ptr != 0);
   
   if (obj_ptr != 0)
   {
      ClassDescriptionMaster & cdm = obj_ptr->get_cdm ();
      
      const ClassDescriptionMaster::VarInfoArr & var_info_arr = cdm.get_var_info_arr ();
      
      ClassDescriptionMaster::VarInfoArr::const_iterator it = var_info_arr.begin ();
      const ClassDescriptionMaster::VarInfoArr::const_iterator it_end = var_info_arr.end ();
      
      for (; it != it_end ; ++it)
      {
         const ClassDescriptionMaster::VarInfo & var_info = *it;
         
         archi::Int64 var_offset = var_info._var_offset;
         flip::Ref sub_ref = ref + var_offset;
         
         if (var_info._type_id == ClassDescManager::BasicTypeId_ARRAY)
         {
            execute (sub_ref, unary_traveler_function);
            
            Object * sub_obj_ptr = _object_manager.get_object (sub_ref);
            
            ArrayMaster * array_ptr = dynamic_cast <ArrayMaster *> (sub_obj_ptr);
            assert (array_ptr != 0);
            
            size_t array_size = array_ptr->size ();
            
            for (size_t i = 0 ; i < array_size ; ++i)
            {
               flip::Ref elem_ref = array_ptr->get_element_ref (i);
               
               travel_normal_follow_containers (elem_ref, unary_traveler_function);
            }
         }
         else if (var_info._type_id == ClassDescManager::BasicTypeId_COLLECTION)
         {
            execute (sub_ref, unary_traveler_function);
            
            Object * sub_obj_ptr = _object_manager.get_object (sub_ref);
            
            CollectionMaster * collection_ptr = dynamic_cast <CollectionMaster *> (sub_obj_ptr);
            assert (collection_ptr != 0);
            
            size_t collection_size = collection_ptr->size ();
            
            for (size_t i = 0 ; i < collection_size ; ++i)
            {
               flip::Ref elem_ref = collection_ptr->get_element_ref (i);
               
               travel_normal_follow_containers (elem_ref, unary_traveler_function);
            }
         }
         else
         {
            travel_normal_follow_containers (sub_ref, unary_traveler_function);
         }
      }
   }
}



/*
==============================================================================
Name : travel_reverse_follow_containers
==============================================================================
*/

template <class UTF>
void  ObjectTravelAgency::travel_reverse_follow_containers (flip::Ref ref, UTF & unary_traveler_function)
{
   TravelerObjectCollector traveller;
   
   travel_normal_follow_containers (ref, traveller);
   
   std::vector <flip::Ref>::reverse_iterator it = traveller._ref_arr.rbegin ();
   const std::vector <flip::Ref>::reverse_iterator it_end = traveller._ref_arr.rend ();
   
   for (; it != it_end ; ++it)
   {
      flip::Ref sub_ref = *it;
      
      execute (sub_ref, unary_traveler_function);
   }
}



/*
==============================================================================
Name : travel_normal_depth_first
==============================================================================
*/

template <class UTF>
void  ObjectTravelAgency::travel_normal_depth_first (flip::Ref ref, UTF & unary_traveler_function)
{
   Object * obj_ptr = _object_manager.get_object (ref);
   assert (obj_ptr != 0);
   
   if (obj_ptr != 0)
   {
      ClassDescriptionMaster & cdm = obj_ptr->get_cdm ();
      
      const ClassDescriptionMaster::VarInfoArr & var_info_arr = cdm.get_var_info_arr ();
      
      ClassDescriptionMaster::VarInfoArr::const_iterator it = var_info_arr.begin ();
      const ClassDescriptionMaster::VarInfoArr::const_iterator it_end = var_info_arr.end ();
      
      for (; it != it_end ; ++it)
      {
         const ClassDescriptionMaster::VarInfo & var_info = *it;
         
         archi::Int64 var_offset = var_info._var_offset;
         flip::Ref sub_ref = ref + var_offset;
         
         travel_normal_depth_first (sub_ref, unary_traveler_function);
      }
   }

   execute (ref, unary_traveler_function);
}



/*
==============================================================================
Name : travel_reverse_depth_first
==============================================================================
*/

template <class UTF>
void  ObjectTravelAgency::travel_reverse_depth_first (flip::Ref ref, UTF & unary_traveler_function)
{
   TravelerObjectCollector traveller;
   
   travel_normal_depth_first (ref, traveller);
   
   std::vector <flip::Ref>::reverse_iterator it = traveller._ref_arr.rbegin ();
   const std::vector <flip::Ref>::reverse_iterator it_end = traveller._ref_arr.rend ();
   
   for (; it != it_end ; ++it)
   {
      flip::Ref sub_ref = *it;
      
      execute (sub_ref, unary_traveler_function);
   }
}



/*
==============================================================================
Name : execute
==============================================================================
*/

template <class UTF>
void  ObjectTravelAgency::execute (flip::Ref ref, UTF & unary_traveler_function)
{
   if (!_first_flag)
   {
      unary_traveler_function (ref);
   }
   else
   {
      _first_flag = false;
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



#endif   // ohm_flip_ObjectTravelAgency_CODEHEADER_INCLUDED

#undef ohm_flip_ObjectTravelAgency_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
