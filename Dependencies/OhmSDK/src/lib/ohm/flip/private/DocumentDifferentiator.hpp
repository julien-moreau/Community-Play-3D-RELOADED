/*****************************************************************************

        DocumentDifferentiator.hpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 63856

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



#if defined (ohm_flip_DocumentDifferentiator_CURRENT_CODEHEADER)
   #error Recursive inclusion of DocumentDifferentiator code header.
#endif
#define  ohm_flip_DocumentDifferentiator_CURRENT_CODEHEADER

#if ! defined (ohm_flip_DocumentDifferentiator_CODEHEADER_INCLUDED)
#define  ohm_flip_DocumentDifferentiator_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/private/DocumentReader.h"

#undef OHM_FLIP_DOCUMENT_DIFFERENTIATOR_DEBUG_FLAG

#if defined (OHM_FLIP_DOCUMENT_DIFFERENTIATOR_DEBUG_FLAG)
   #define dd_break assert (false)
#else
   #define dd_break ((void) 0)
#endif



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : differentiate
==============================================================================
*/

template <class Actor>
int   DocumentDifferentiator::differentiate (Actor & actor, ContainerObjectItPairList & it_pair_list, ContainerObjectInfoList & src_info_list, ContainerObjectInfoList & dst_info_list)
{
   ContainerObjectItPairList::iterator it = it_pair_list.begin ();
   const ContainerObjectItPairList::iterator it_end = it_pair_list.end ();

   const ContainerObjectInfoList::iterator src_it_end = src_info_list.end ();
   const ContainerObjectInfoList::iterator dst_it_end = dst_info_list.end ();
   
   for (; it != it_end ; ++it)
   {
      ContainerObjectInfoList::iterator src_it = it->first;
      ContainerObjectInfoList::iterator dst_it = it->second;
      
      if ((src_it != src_it_end) && (dst_it != dst_it_end))
      {
         if (src_it->_ref == dst_it->_ref)
         {
            // same object we only differentiate it
            
            int err = actor.differentiate_same_ref (*src_it, *dst_it);
            if (err != Err_OK) return err;
         }
         else
         {
            // arrays :
            // very rare case : the random key was reattributed to some object
            // we generate an error, at this would need some analysis to detect
            // cycles
            
            // collections :
            // not possible
            
            assert (src_it->_array_flag);
            
            return Err_DIF_NOT_COALESCABLE;
         }
      }
      else if (src_it != src_it_end)
      {
         // src may have disappeared, moved accross containers
         // or (only in array case) moved to another key
         
         ContainerObjectItPairList::iterator it2 = it_end;
         
         if (src_it->_array_flag)
         {
            it2 = it;
            ++it2;
         }
         
         ContainerObjectInfoList::iterator dst_it2 = dst_it_end;
         
         for (; it2 != it_end ; ++it2)
         {
            dst_it2 = it2->second;
            
            if (dst_it2 != dst_it_end)
            {
               if (src_it->_ref == dst_it2->_ref)
               {
                  break;
               }
            }
         }
         
         if (it2 != it_end)
         {
            if (dst_it2 == dst_it_end)
            {
               dd_break;
               return Err_DOC_INTERNAL_SYSTEM_ERROR;
            }
            
            // ref moved to another key
            
            int err = actor.ref_moved (*src_it, *dst_it2);
            if (err != Err_OK) return err;
            
            err = actor.differentiate_same_ref (*src_it, *dst_it2);
            if (err != Err_OK) return err;
            
            it_pair_list.erase (it2);
         }
         else
         {
            // ref disappeared or moved accross another container
            
            bool dst_present_flag
               = _dst_container_ref_map.find (src_it->_ref) != _dst_container_ref_map.end ();
            
            if (dst_present_flag)
            {
               int err = actor.ref_moved_accross (*src_it);
               if (err != Err_OK) return err;
            }
            else
            {
               int err = actor.ref_erased (*src_it);
               if (err != Err_OK) return err;
            }
         }
      }
      else if (dst_it != dst_it_end)
      {
         // dst may have appeared, moved accross containers, or move to another key
         
         ContainerObjectItPairList::iterator it2 = it_end;
         
         if (dst_it->_array_flag)
         {
            it2 = it;
            ++it2;
         }
         
         ContainerObjectInfoList::iterator src_it2 = src_it_end;
         
         for (; it2 != it_end ; ++it2)
         {
            src_it2 = it2->first;
            
            if (src_it2 != src_it_end)
            {
               if (dst_it->_ref == src_it2->_ref)
               {
                  break;
               }
            }
         }
         
         if (it2 != it_end)
         {
            if (src_it2 == src_it_end)
            {
               dd_break;
               return Err_DOC_INTERNAL_SYSTEM_ERROR;
            }
            
            // ref moved to another key
            
            int err = actor.ref_moved (*src_it2, *dst_it);
            if (err != Err_OK) return err;
            
            err = actor.differentiate_same_ref (*src_it2, *dst_it);
            if (err != Err_OK) return err;
            
            it_pair_list.erase (it2);
         }
         else
         {
            // ref appeared
            
            bool src_present_flag
               = _src_container_ref_map.find (dst_it->_ref) != _src_container_ref_map.end ();
            
            if (src_present_flag)
            {
               // nothing
            }
            else
            {
               int err = actor.ref_inserted (*dst_it);
               if (err != Err_OK) return err;
            }
         }
      }
      else
      {
         assert (false);
         
         return Err_DOC_INTERNAL_SYSTEM_ERROR;
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : ContainerObjectInfo::is_equal
==============================================================================
*/

bool  DocumentDifferentiator::ContainerObjectInfo::is_equal (const ContainerObjectInfo & other) const
{
   if (_array_flag)
   {
      return _key == other._key;
   }
   else
   {
      return _ref == other._ref;
   }
}



/*
==============================================================================
Name : ContainerObjectInfo::is_less
==============================================================================
*/

bool  DocumentDifferentiator::ContainerObjectInfo::is_less (const ContainerObjectInfo & other) const
{
   if (_array_flag)
   {
      return _key < other._key;
   }
   else
   {
      return _ref < other._ref;
   }
}



/*
==============================================================================
Name : ActorArrayObject::ctor
==============================================================================
*/

DocumentDifferentiator::ActorArrayObject::ActorArrayObject (DocumentDifferentiator & dd, flip::Ref ref)
:  _dd (dd)
,  _ref (ref)
{
}



/*
==============================================================================
Name : ActorArrayObject::differentiate_same_ref
==============================================================================
*/

int   DocumentDifferentiator::ActorArrayObject::differentiate_same_ref (const ContainerObjectInfo & src, const ContainerObjectInfo & dst)
{
   assert (src._ref == dst._ref);
   
   _dd.differentiate_object_ref (src._ref);
   
   return Err_OK;
}



/*
==============================================================================
Name : ActorArrayObject::ref_moved
==============================================================================
*/

int   DocumentDifferentiator::ActorArrayObject::ref_moved (const ContainerObjectInfo & src, const ContainerObjectInfo & dst)
{
   if (_dd._exclude_cnt == 0)
   {
      MachineCode::build_array_move (
         *_dd._tx_data_sptr,
         _ref, src._key,
         _ref, dst._key
      );
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : ActorArrayObject::ref_moved_accross
==============================================================================
*/

int   DocumentDifferentiator::ActorArrayObject::ref_moved_accross (const ContainerObjectInfo & src)
{
   MachineCode::RefContainerElemMap::const_iterator src_it = _dd._src_container_ref_map.find (src._ref);
   assert (src_it != _dd._src_container_ref_map.end ());

   MachineCode::RefContainerElemMap::const_iterator dst_it = _dd._dst_container_ref_map.find (src._ref);
   assert (dst_it != _dd._dst_container_ref_map.end ());
   
   const MachineCode::ContainerElem & src_item = src_it->second;
   const MachineCode::ContainerElem & dst_item = dst_it->second;
   
   assert ((_ref == src_item._parent_ref) || (_ref == dst_item._parent_ref));
   
   if (_dd._exclude_cnt == 0)
   {
      MachineCode::build_array_move (
         *_dd._tx_data_sptr,
         src_item._parent_ref, src_item._key,
         dst_item._parent_ref, dst_item._key
      );
   }
   
   _dd.differentiate_object_ref (src._ref);
   
   return Err_OK;
}



/*
==============================================================================
Name : ActorArrayObject::ref_erased
==============================================================================
*/

int   DocumentDifferentiator::ActorArrayObject::ref_erased (const ContainerObjectInfo & elem)
{
   Data tx_data;
   
   ClassDescriptionMaster * cdm_ptr = 0;
      
   int err = DocumentReader::read_object_ref_cdm (cdm_ptr, _dd._src_dp, elem._ref);
   if (err != Err_OK) return err;

   if (_dd._exclude_cnt == 0)
   {
      MachineCode::build_array_insert (
         tx_data,
         _ref, elem._key, elem._ref, cdm_ptr->get_class_id ()
      );

      DocumentReader::read_object_ref (tx_data, _dd._src_dp, elem._ref);
      
      if (!tx_data.empty ())
      {
         MachineCode code (&tx_data [0], tx_data.size ());
         
         // compile not tolerant
         code.set_mode_no_fault ();
         code.compile ();
         
         Data inv_tx_data;
         code.inverse (inv_tx_data);
         
         _dd._tx_data_sptr->insert (
            _dd._tx_data_sptr->end (),
            inv_tx_data.begin (),
            inv_tx_data.end ()
         );
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : ActorArrayObject::ref_inserted
==============================================================================
*/

int   DocumentDifferentiator::ActorArrayObject::ref_inserted (const ContainerObjectInfo & elem)
{
   ClassDescriptionMaster * cdm_ptr = 0;
      
   int err = DocumentReader::read_object_ref_cdm (cdm_ptr, _dd._dst_dp, elem._ref);
   if (err != Err_OK) return err;

   if (_dd._exclude_cnt == 0)
   {
      MachineCode::build_array_insert (
         *_dd._tx_data_sptr,
         _ref, elem._key, elem._ref, cdm_ptr->get_class_id ()
      );

      DocumentReader::read_object_ref (*_dd._tx_data_sptr, _dd._dst_dp, elem._ref);
   }

   return Err_OK;
}



/*
==============================================================================
Name : ActorArrayInt64::ctor
==============================================================================
*/

DocumentDifferentiator::ActorArrayInt64::ActorArrayInt64 (DocumentDifferentiator & dd, flip::Ref ref)
:  _dd (dd)
,  _ref (ref)
{
}



/*
==============================================================================
Name : ActorArrayInt64::differentiate_same_ref
==============================================================================
*/

int   DocumentDifferentiator::ActorArrayInt64::differentiate_same_ref (const ContainerObjectInfo & src, const ContainerObjectInfo & dst)
{
   assert (src._ref == dst._ref);
   archi::Int64 src_val = src._val_i64;
   archi::Int64 dst_val = dst._val_i64;
   
   if (src_val != dst_val)
   {
      if (_dd._exclude_cnt == 0)
      {
         MachineCode::build_int64_set (
            *_dd._tx_data_sptr,
            src._ref, src_val, dst_val
         );
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : ActorArrayInt64::ref_moved
==============================================================================
*/

int   DocumentDifferentiator::ActorArrayInt64::ref_moved (const ContainerObjectInfo & src, const ContainerObjectInfo & dst)
{
   if (_dd._exclude_cnt == 0)
   {
      MachineCode::build_array_move (
         *_dd._tx_data_sptr,
         _ref, src._key,
         _ref, dst._key
      );
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : ActorArrayInt64::ref_moved_accross
==============================================================================
*/

int   DocumentDifferentiator::ActorArrayInt64::ref_moved_accross (const ContainerObjectInfo & src)
{
   MachineCode::RefContainerElemMap::const_iterator src_it = _dd._src_container_ref_map.find (src._ref);
   assert (src_it != _dd._src_container_ref_map.end ());

   MachineCode::RefContainerElemMap::const_iterator dst_it = _dd._dst_container_ref_map.find (src._ref);
   assert (dst_it != _dd._dst_container_ref_map.end ());
   
   const MachineCode::ContainerElem & src_item = src_it->second;
   const MachineCode::ContainerElem & dst_item = dst_it->second;
   
   assert ((_ref == src_item._parent_ref) || (_ref == dst_item._parent_ref));
   
   if (_dd._exclude_cnt == 0)
   {
      MachineCode::build_array_move (
         *_dd._tx_data_sptr,
         src_item._parent_ref, src_item._key,
         dst_item._parent_ref, dst_item._key
      );
   }
   
   archi::Int64 src_val = src_item._val_i64;
   archi::Int64 dst_val = dst_item._val_i64;

   if (src_val != dst_val)
   {
      if (_dd._exclude_cnt == 0)
      {
         MachineCode::build_int64_set (
            *_dd._tx_data_sptr,
            src._ref, src_val, dst_val
         );
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : ActorArrayInt64::ref_erased
==============================================================================
*/

int   DocumentDifferentiator::ActorArrayInt64::ref_erased (const ContainerObjectInfo & elem)
{
   archi::Int64 sub_val = elem._val_i64;
   
   if (sub_val != 0LL)
   {
      if (_dd._exclude_cnt == 0)
      {
         MachineCode::build_int64_set (
            *_dd._tx_data_sptr,
            elem._ref, sub_val, 0LL
         );
      }
   }

   if (_dd._exclude_cnt == 0)
   {
      MachineCode::build_array_erase (
         *_dd._tx_data_sptr,
         _ref, elem._key, elem._ref, ClassDescManager::BasicTypeId_INT64
      );
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : ActorArrayInt64::ref_inserted
==============================================================================
*/

int   DocumentDifferentiator::ActorArrayInt64::ref_inserted (const ContainerObjectInfo & elem)
{
   archi::Int64 sub_val = elem._val_i64;
   
   if (_dd._exclude_cnt == 0)
   {
      MachineCode::build_array_insert (
         *_dd._tx_data_sptr,
         _ref, elem._key, elem._ref, ClassDescManager::BasicTypeId_INT64
      );
   }

   if (sub_val != 0LL)
   {
      if (_dd._exclude_cnt == 0)
      {
         MachineCode::build_int64_set (
            *_dd._tx_data_sptr,
            elem._ref, 0LL, sub_val
         );
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : ActorArrayFloat64::ctor
==============================================================================
*/

DocumentDifferentiator::ActorArrayFloat64::ActorArrayFloat64 (DocumentDifferentiator & dd, flip::Ref ref)
:  _dd (dd)
,  _ref (ref)
{
}



/*
==============================================================================
Name : ActorArrayFloat64::differentiate_same_ref
==============================================================================
*/

int   DocumentDifferentiator::ActorArrayFloat64::differentiate_same_ref (const ContainerObjectInfo & src, const ContainerObjectInfo & dst)
{
   assert (src._ref == dst._ref);
   archi::Float64 src_val = src._val_f64;
   archi::Float64 dst_val = dst._val_f64;
   
   if (src_val != dst_val)
   {
      if (_dd._exclude_cnt == 0)
      {
         MachineCode::build_float64_set (
            *_dd._tx_data_sptr,
            src._ref, src_val, dst_val
         );
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : ActorArrayFloat64::ref_moved
==============================================================================
*/

int   DocumentDifferentiator::ActorArrayFloat64::ref_moved (const ContainerObjectInfo & src, const ContainerObjectInfo & dst)
{
   if (_dd._exclude_cnt == 0)
   {
      MachineCode::build_array_move (
         *_dd._tx_data_sptr,
         _ref, src._key,
         _ref, dst._key
      );
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : ActorArrayFloat64::ref_moved_accross
==============================================================================
*/

int   DocumentDifferentiator::ActorArrayFloat64::ref_moved_accross (const ContainerObjectInfo & src)
{
   MachineCode::RefContainerElemMap::const_iterator src_it = _dd._src_container_ref_map.find (src._ref);
   assert (src_it != _dd._src_container_ref_map.end ());

   MachineCode::RefContainerElemMap::const_iterator dst_it = _dd._dst_container_ref_map.find (src._ref);
   assert (dst_it != _dd._dst_container_ref_map.end ());
   
   const MachineCode::ContainerElem & src_item = src_it->second;
   const MachineCode::ContainerElem & dst_item = dst_it->second;
   
   assert ((_ref == src_item._parent_ref) || (_ref == dst_item._parent_ref));
   
   if (_dd._exclude_cnt == 0)
   {
      MachineCode::build_array_move (
         *_dd._tx_data_sptr,
         src_item._parent_ref, src_item._key,
         dst_item._parent_ref, dst_item._key
      );
   }

   archi::Float64 src_val = src_item._val_f64;
   archi::Float64 dst_val = dst_item._val_f64;

   if (src_val != dst_val)
   {
      if (_dd._exclude_cnt == 0)
      {
         MachineCode::build_float64_set (
            *_dd._tx_data_sptr,
            src._ref, src_val, dst_val
         );
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : ActorArrayFloat64::ref_erased
==============================================================================
*/

int   DocumentDifferentiator::ActorArrayFloat64::ref_erased (const ContainerObjectInfo & elem)
{
   archi::Float64 sub_val = elem._val_f64;
   
   if (sub_val != 0.0)
   {
      if (_dd._exclude_cnt == 0)
      {
         MachineCode::build_float64_set (
            *_dd._tx_data_sptr,
            elem._ref, sub_val, 0.0
         );
      }
   }

   if (_dd._exclude_cnt == 0)
   {
      MachineCode::build_array_erase (
         *_dd._tx_data_sptr,
         _ref, elem._key, elem._ref, ClassDescManager::BasicTypeId_FLOAT64
      );
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : ActorArrayFloat64::ref_inserted
==============================================================================
*/

int   DocumentDifferentiator::ActorArrayFloat64::ref_inserted (const ContainerObjectInfo & elem)
{
   archi::Float64 sub_val = elem._val_f64;
   
   if (_dd._exclude_cnt == 0)
   {
      MachineCode::build_array_insert (
         *_dd._tx_data_sptr,
         _ref, elem._key, elem._ref, ClassDescManager::BasicTypeId_FLOAT64
      );
   }

   if (sub_val != 0.0)
   {
      if (_dd._exclude_cnt == 0)
      {
         MachineCode::build_float64_set (
            *_dd._tx_data_sptr,
            elem._ref, 0.0, sub_val
         );
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : ActorCollectionObject::ctor
==============================================================================
*/

DocumentDifferentiator::ActorCollectionObject::ActorCollectionObject (DocumentDifferentiator & dd, flip::Ref ref)
:  _dd (dd)
,  _ref (ref)
{
}



/*
==============================================================================
Name : ActorCollectionObject::differentiate_same_ref
==============================================================================
*/

int   DocumentDifferentiator::ActorCollectionObject::differentiate_same_ref (const ContainerObjectInfo & src, const ContainerObjectInfo & dst)
{
   assert (src._ref == dst._ref);
   
   _dd.differentiate_object_ref (src._ref);
   
   return Err_OK;
}



/*
==============================================================================
Name : ActorCollectionObject::ref_moved
==============================================================================
*/

int   DocumentDifferentiator::ActorCollectionObject::ref_moved (const ContainerObjectInfo & src, const ContainerObjectInfo & dst)
{
   // impossible
   assert (false);
   
   return Err_DOC_INTERNAL_SYSTEM_ERROR;
}



/*
==============================================================================
Name : ActorCollectionObject::ref_moved_accross
==============================================================================
*/

int   DocumentDifferentiator::ActorCollectionObject::ref_moved_accross (const ContainerObjectInfo & src)
{
   MachineCode::RefContainerElemMap::const_iterator src_it = _dd._src_container_ref_map.find (src._ref);
   assert (src_it != _dd._src_container_ref_map.end ());

   MachineCode::RefContainerElemMap::const_iterator dst_it = _dd._dst_container_ref_map.find (src._ref);
   assert (dst_it != _dd._dst_container_ref_map.end ());
   
   const MachineCode::ContainerElem & src_item = src_it->second;
   const MachineCode::ContainerElem & dst_item = dst_it->second;
   
   assert ((_ref == src_item._parent_ref) || (_ref == dst_item._parent_ref));
   
   if (_dd._exclude_cnt == 0)
   {
      MachineCode::build_collection_move (
         *_dd._tx_data_sptr,
         src_item._parent_ref, src._ref, dst_item._parent_ref
      );
   }
      
   _dd.differentiate_object_ref (src._ref);
   
   return Err_OK;
}



/*
==============================================================================
Name : ActorCollectionObject::ref_erased
==============================================================================
*/

int   DocumentDifferentiator::ActorCollectionObject::ref_erased (const ContainerObjectInfo & elem)
{
   Data tx_data;
   
   ClassDescriptionMaster * cdm_ptr = 0;
      
   int err = DocumentReader::read_object_ref_cdm (cdm_ptr, _dd._src_dp, elem._ref);
   if (err != Err_OK) return err;

   if (_dd._exclude_cnt == 0)
   {
      MachineCode::build_collection_insert (
         tx_data,
         _ref, elem._ref, cdm_ptr->get_class_id ()
      );

      DocumentReader::read_object_ref (tx_data, _dd._src_dp, elem._ref);
      
      if (!tx_data.empty ())
      {
         MachineCode code (&tx_data [0], tx_data.size ());
         
         // compile not tolerant
         code.set_mode_no_fault ();
         code.compile ();
         
         Data inv_tx_data;
         code.inverse (inv_tx_data);
         
         _dd._tx_data_sptr->insert (
            _dd._tx_data_sptr->end (),
            inv_tx_data.begin (),
            inv_tx_data.end ()
         );
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : ActorCollectionObject::ref_inserted
==============================================================================
*/

int   DocumentDifferentiator::ActorCollectionObject::ref_inserted (const ContainerObjectInfo & elem)
{
   ClassDescriptionMaster * cdm_ptr = 0;
      
   int err = DocumentReader::read_object_ref_cdm (cdm_ptr, _dd._dst_dp, elem._ref);
   if (err != Err_OK) return err;

   if (_dd._exclude_cnt == 0)
   {
      MachineCode::build_collection_insert (
         *_dd._tx_data_sptr,
         _ref, elem._ref, cdm_ptr->get_class_id ()
      );

      DocumentReader::read_object_ref (*_dd._tx_data_sptr, _dd._dst_dp, elem._ref);
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : ActorCollectionInt64::ctor
==============================================================================
*/

DocumentDifferentiator::ActorCollectionInt64::ActorCollectionInt64 (DocumentDifferentiator & dd, flip::Ref ref)
:  _dd (dd)
,  _ref (ref)
{
}



/*
==============================================================================
Name : ActorCollectionInt64::differentiate_same_ref
==============================================================================
*/

int   DocumentDifferentiator::ActorCollectionInt64::differentiate_same_ref (const ContainerObjectInfo & src, const ContainerObjectInfo & dst)
{
   assert (src._ref == dst._ref);
   archi::Int64 src_val = src._val_i64;
   archi::Int64 dst_val = dst._val_i64;
   
   if (src_val != dst_val)
   {
      if (_dd._exclude_cnt == 0)
      {
         MachineCode::build_int64_set (
            *_dd._tx_data_sptr,
            src._ref, src_val, dst_val
         );
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : ActorCollectionInt64::ref_moved
==============================================================================
*/

int   DocumentDifferentiator::ActorCollectionInt64::ref_moved (const ContainerObjectInfo & src, const ContainerObjectInfo & dst)
{
   // impossible
   assert (false);
   
   return Err_DOC_INTERNAL_SYSTEM_ERROR;
}



/*
==============================================================================
Name : ActorCollectionInt64::ref_moved_accross
==============================================================================
*/

int   DocumentDifferentiator::ActorCollectionInt64::ref_moved_accross (const ContainerObjectInfo & src)
{
   MachineCode::RefContainerElemMap::const_iterator src_it = _dd._src_container_ref_map.find (src._ref);
   assert (src_it != _dd._src_container_ref_map.end ());

   MachineCode::RefContainerElemMap::const_iterator dst_it = _dd._dst_container_ref_map.find (src._ref);
   assert (dst_it != _dd._dst_container_ref_map.end ());
   
   const MachineCode::ContainerElem & src_item = src_it->second;
   const MachineCode::ContainerElem & dst_item = dst_it->second;
   
   assert ((_ref == src_item._parent_ref) || (_ref == dst_item._parent_ref));
   
   if (_dd._exclude_cnt == 0)
   {
      MachineCode::build_collection_move (
         *_dd._tx_data_sptr,
         src_item._parent_ref, src._ref, dst_item._parent_ref
      );
   }
   
   archi::Int64 src_val = src_item._val_i64;
   archi::Int64 dst_val = dst_item._val_i64;

   if (src_val != dst_val)
   {
      if (_dd._exclude_cnt == 0)
      {
         MachineCode::build_int64_set (
            *_dd._tx_data_sptr,
            src._ref, src_val, dst_val
         );
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : ActorCollectionInt64::ref_erased
==============================================================================
*/

int   DocumentDifferentiator::ActorCollectionInt64::ref_erased (const ContainerObjectInfo & elem)
{
   archi::Int64 sub_val = elem._val_i64;
   
   if (sub_val != 0LL)
   {
      if (_dd._exclude_cnt == 0)
      {
         MachineCode::build_int64_set (
            *_dd._tx_data_sptr,
            elem._ref, sub_val, 0LL
         );
      }
   }

   if (_dd._exclude_cnt == 0)
   {
      MachineCode::build_collection_erase (
         *_dd._tx_data_sptr,
         _ref, elem._ref, ClassDescManager::BasicTypeId_INT64
      );
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : ActorCollectionInt64::ref_inserted
==============================================================================
*/

int   DocumentDifferentiator::ActorCollectionInt64::ref_inserted (const ContainerObjectInfo & elem)
{
   archi::Int64 sub_val = elem._val_i64;
   
   if (_dd._exclude_cnt == 0)
   {
      MachineCode::build_collection_insert (
         *_dd._tx_data_sptr,
         _ref, elem._ref, ClassDescManager::BasicTypeId_INT64
      );
   }

   if (sub_val != 0LL)
   {
      if (_dd._exclude_cnt == 0)
      {
         MachineCode::build_int64_set (
            *_dd._tx_data_sptr,
            elem._ref, 0LL, sub_val
         );
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : ActorCollectionFloat64::ctor
==============================================================================
*/

DocumentDifferentiator::ActorCollectionFloat64::ActorCollectionFloat64 (DocumentDifferentiator & dd, flip::Ref ref)
:  _dd (dd)
,  _ref (ref)
{
}



/*
==============================================================================
Name : ActorCollectionFloat64::differentiate_same_ref
==============================================================================
*/

int   DocumentDifferentiator::ActorCollectionFloat64::differentiate_same_ref (const ContainerObjectInfo & src, const ContainerObjectInfo & dst)
{
   assert (src._ref == dst._ref);
   archi::Float64 src_val = src._val_f64;
   archi::Float64 dst_val = dst._val_f64;
   
   if (src_val != dst_val)
   {
      if (_dd._exclude_cnt == 0)
      {
         MachineCode::build_float64_set (
            *_dd._tx_data_sptr,
            src._ref, src_val, dst_val
         );
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : ActorCollectionFloat64::ref_moved
==============================================================================
*/

int   DocumentDifferentiator::ActorCollectionFloat64::ref_moved (const ContainerObjectInfo & src, const ContainerObjectInfo & dst)
{
   // impossible
   assert (false);
   
   return Err_DOC_INTERNAL_SYSTEM_ERROR;
}



/*
==============================================================================
Name : ActorCollectionFloat64::ref_moved_accross
==============================================================================
*/

int   DocumentDifferentiator::ActorCollectionFloat64::ref_moved_accross (const ContainerObjectInfo & src)
{
   MachineCode::RefContainerElemMap::const_iterator src_it = _dd._src_container_ref_map.find (src._ref);
   assert (src_it != _dd._src_container_ref_map.end ());

   MachineCode::RefContainerElemMap::const_iterator dst_it = _dd._dst_container_ref_map.find (src._ref);
   assert (dst_it != _dd._dst_container_ref_map.end ());
   
   const MachineCode::ContainerElem & src_item = src_it->second;
   const MachineCode::ContainerElem & dst_item = dst_it->second;
   
   assert ((_ref == src_item._parent_ref) || (_ref == dst_item._parent_ref));
   
   if (_dd._exclude_cnt == 0)
   {
      MachineCode::build_collection_move (
         *_dd._tx_data_sptr,
         src_item._parent_ref, src._ref, dst_item._parent_ref
      );
   }
   
   archi::Float64 src_val = src_item._val_f64;
   archi::Float64 dst_val = dst_item._val_f64;

   if (src_val != dst_val)
   {
      if (_dd._exclude_cnt == 0)
      {
         MachineCode::build_float64_set (
            *_dd._tx_data_sptr,
            src._ref, src_val, dst_val
         );
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : ActorCollectionFloat64::ref_erased
==============================================================================
*/

int   DocumentDifferentiator::ActorCollectionFloat64::ref_erased (const ContainerObjectInfo & elem)
{
   archi::Float64 sub_val = elem._val_f64;
   
   if (sub_val != 0.0)
   {
      if (_dd._exclude_cnt == 0)
      {
         MachineCode::build_float64_set (
            *_dd._tx_data_sptr,
            elem._ref, sub_val, 0.0
         );
      }
   }

   if (_dd._exclude_cnt == 0)
   {
      MachineCode::build_collection_erase (
         *_dd._tx_data_sptr,
         _ref, elem._ref, ClassDescManager::BasicTypeId_FLOAT64
      );
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : ActorCollectionFloat64::ref_inserted
==============================================================================
*/

int   DocumentDifferentiator::ActorCollectionFloat64::ref_inserted (const ContainerObjectInfo & elem)
{
   archi::Float64 sub_val = elem._val_f64;
   
   if (_dd._exclude_cnt == 0)
   {
      MachineCode::build_collection_insert (
         *_dd._tx_data_sptr,
         _ref, elem._ref, ClassDescManager::BasicTypeId_FLOAT64
      );
   }

   if (sub_val != 0.0)
   {
      if (_dd._exclude_cnt == 0)
      {
         MachineCode::build_float64_set (
            *_dd._tx_data_sptr,
            elem._ref, 0.0, sub_val
         );
      }
   }
   
   return Err_OK;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



#undef dd_break

#endif   // ohm_flip_DocumentDifferentiator_CODEHEADER_INCLUDED

#undef ohm_flip_DocumentDifferentiator_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
