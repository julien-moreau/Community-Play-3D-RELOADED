/*****************************************************************************

        CollectionMaster.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71329

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



#if ! defined (ohm_flip_CollectionMaster_HEADER_INCLUDED)
#define  ohm_flip_CollectionMaster_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/Object.h"
#include "ohm/flip/Err.h"
#include "ohm/flip/FindMask.h"

#include <map>



namespace ohm
{
namespace flip
{



class BinaryKey;

class CollectionMaster
:  public Object
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  CollectionMaster (DocumentBase & document);
   virtual        ~CollectionMaster ();

   // Object
   virtual void   ctor (ohm::flip::Args & args);
   virtual void   init ();
   virtual void   restore ();

   virtual size_t size (FindMask find_mask = FindMask_NATURAL) const;
   virtual void   clear ();
   virtual void   erase (Ref ref);
   
   // PasteBoard access
   virtual Ref    insert (ClassDescriptionMaster & cdm);



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   friend class VirtualMachine;
   friend class DocumentWriter;
   friend class ObjectTravelAgency;
   friend class ObjectMold;
   friend class DocumentBase;
   
   // VirtualMachine private access
   virtual int    insert_no_notification (Ref ref, int class_id, bool tolerant_flag);
   virtual int    erase_no_notification (Ref ref, bool tolerant_flag);
   virtual int    move_no_notification (CollectionMaster * dst_ptr, Object * obj_ptr, bool tolerant_flag);

   // DocumentBase private access
   virtual int    broadcast_order_change ();
   
   // DocumentWriter & ObjectMold private access
   virtual Ref    get_element_ref (size_t i) const;
   virtual int    get_element_class_id () const;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  CollectionMaster ();
                  CollectionMaster (const CollectionMaster &other);
   CollectionMaster &   operator = (const CollectionMaster & other);
   bool           operator == (const CollectionMaster &other);
   bool           operator != (const CollectionMaster &other);

};



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_flip_CollectionMaster_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
