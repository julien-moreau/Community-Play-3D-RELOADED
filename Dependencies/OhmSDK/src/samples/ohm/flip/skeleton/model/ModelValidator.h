/*****************************************************************************

        ModelValidator.h
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



#if ! defined (model_ModelValidator_HEADER_INCLUDED)
#define  model_ModelValidator_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "model/Root.h"

#include "ohm/flip/DocumentValidator.h"



namespace model
{



class ModelValidator
:  public ohm::flip::DocumentValidator <Root>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  ModelValidator ();
   virtual        ~ModelValidator ();
   
   // ohm::flip::DocumentValidator <Root>
   virtual void   validate (ohm::flip::ValidationReportWriter & report, ohm::archi::Int32 user_id, Root & root);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           validate (ohm::flip::ValidationReportWriter & report, ohm::archi::Int32 user_id, ClientData & client_data);

   void           validate (ohm::flip::ValidationReportWriter & report, ohm::archi::Int32 user_id, SpacialObject & spacial_object);
   void           validate (ohm::flip::ValidationReportWriter & report, ohm::archi::Int32 user_id, Taxi & taxi);
   void           validate (ohm::flip::ValidationReportWriter & report, ohm::archi::Int32 user_id, Pedestrian & pedestrian);



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ModelValidator (const ModelValidator &other);
   ModelValidator &
                  operator = (const ModelValidator &other);
   bool           operator == (const ModelValidator &other);
   bool           operator != (const ModelValidator &other);

}; // class ModelValidator



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace model



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "model/ModelValidator.hpp"



#endif   // model_ModelValidator_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
