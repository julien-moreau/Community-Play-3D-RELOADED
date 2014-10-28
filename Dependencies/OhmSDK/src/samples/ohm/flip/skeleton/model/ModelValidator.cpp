/*****************************************************************************

        ModelValidator.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70457

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
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "model/ModelValidator.h"

#include "ohm/flip/ValidationReportWriter.h"

#include <cassert>



#define OHM_STUDIO_MODEL_BREAK_ON_INVALIDATION

#if defined (OHM_STUDIO_MODEL_BREAK_ON_INVALIDATION)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_FLIP_DCM_DEBUG_FLAG is defined in shipping level code !
   #endif

   #define mv_break report.trace (); assert (false)
#else
   #define mv_break ((void) 0)
#endif



namespace model
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

ModelValidator::ModelValidator ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ModelValidator::~ModelValidator ()
{
}



/*
==============================================================================
Name : validate
Note: 
 - check that we cannot add two curves for the same parameter in a given
 pattern class.
==============================================================================
*/

void  ModelValidator::validate (ohm::flip::ValidationReportWriter & report, ohm::archi::Int32 user_id, Root & root)
{
   ClientDataColl & client_data_coll = root.use_client_data_coll ();
   
   if (client_data_coll.is_invalid ())
   {
      ClientDataColl::iterator it = client_data_coll.begin ();
      const ClientDataColl::iterator it_end = client_data_coll.end ();
      
      for (; it != it_end ; ++it)
      {
         ClientData & client_data = *it;
         
         if (client_data.is_invalid ())
         {
            validate (report, user_id, client_data);
         }
      }
   }
   
   SpacialObjectColl & spacial_object_coll = root.use_spacial_object_coll ();
   
   if (spacial_object_coll.is_invalid ())
   {
      SpacialObjectColl::iterator it = spacial_object_coll.begin ();
      const SpacialObjectColl::iterator it_end = spacial_object_coll.end ();
      
      for (; it != it_end ; ++it)
      {
         SpacialObject & spacial_object = *it;
         
         if (spacial_object.is_invalid ())
         {
            validate (report, user_id, spacial_object);
         }
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : validate
==============================================================================
*/

void  ModelValidator::validate (ohm::flip::ValidationReportWriter & report, ohm::archi::Int32 user_id, ClientData & client_data)
{
   using namespace ohm;
   
   archi::Int32 client_data_user_id = client_data.get_user_id ();
   
   if (client_data.was_inited ())
   {
      // ensure that only right client or server can add a client data
      
      if ((user_id != client_data_user_id) && (user_id != 0))
      {
         report.print_severe_error ("ClientData: Cannot add");
         mv_break;
      }
      
      // ensure that he has only inserted it once
      
      Root & root = client_data.get_ancestor <Root> ();
      
      ClientDataColl & client_data_coll = root.use_client_data_coll ();
      
      ClientDataColl::iterator it = client_data_coll.begin ();
      const ClientDataColl::iterator it_end = client_data_coll.end ();
      
      for (; it != it_end ; ++it)
      {
         ClientData & sub_client_data = *it;
         
         if (&sub_client_data != &client_data)
         {
            archi::Int32 sub_client_data_user_id = sub_client_data.get_user_id ();
         
            if (user_id == sub_client_data_user_id)
            {
               report.print_severe_error ("ClientData: Unicity");
               mv_break;
            }
         }
      }
   }
   else if (client_data.was_restored ())
   {
      // no one has the right to remove a client data
      
      report.print_severe_error ("ClientData: Cannot remove");
      mv_break;
   }
   else
   {
      if ((user_id != client_data_user_id) && (user_id != 0))
      {
         report.print_severe_error ("ClientData: Cannot change");
         mv_break;
      }
   }
}



/*
==============================================================================
Name: validate
==============================================================================
*/

void ModelValidator::validate (ohm::flip::ValidationReportWriter & report, ohm::archi::Int32 user_id, SpacialObject & spacial_object)
{
   Taxi * taxi_ptr = dynamic_cast <Taxi *> (&spacial_object);
   Pedestrian * pedestrian_ptr = dynamic_cast <Pedestrian *> (&spacial_object);
   
   if (taxi_ptr != 0)
   {
      validate (report, user_id, *taxi_ptr);
   }
   else if (pedestrian_ptr != 0)
   {
      validate (report, user_id, *pedestrian_ptr);
   }
   else
   {
      report.print_severe_error ("SpacialObject: neither a Taxi or Pedestrian");
      mv_break;
   }
}



/*
==============================================================================
Name : validate
==============================================================================
*/

void  ModelValidator::validate (ohm::flip::ValidationReportWriter & report, ohm::archi::Int32 user_id, Taxi & taxi)
{
   using namespace ohm;
   
   if (!taxi.was_restored ())
   {
      archi::Float64 width = taxi.get_width ();
      
      if ((width < Taxi::_min_width) || (width > Taxi::_max_width))
      {
         report.print_logical_error ("Taxi: width out of bound (%1%)").arg (width);
         mv_break;
      }
   }
}



/*
==============================================================================
Name : validate
==============================================================================
*/

void  ModelValidator::validate (ohm::flip::ValidationReportWriter & report, ohm::archi::Int32 user_id, Pedestrian & pedestrian)
{
   using namespace ohm;
   
   if (!pedestrian.was_restored ())
   {
      // nothing
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace model



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
