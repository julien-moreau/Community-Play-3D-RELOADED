/*****************************************************************************

        DocumentValidator.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 45327

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



#if ! defined (ohm_flip_DocumentValidator_HEADER_INCLUDED)
#define  ohm_flip_DocumentValidator_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"



namespace ohm
{
namespace flip
{



class Object;
class ValidationReportWriter;

class DocumentValidatorMaster
{
public:
   virtual        ~DocumentValidatorMaster () {}
   
   virtual void   request_validate (ValidationReportWriter & report, archi::Int32 user_id, Object & obj) = 0;
};

template <class T>
class DocumentValidator
:  public DocumentValidatorMaster
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  DocumentValidator ();
   virtual        ~DocumentValidator ();
   
   virtual void   validate (ValidationReportWriter & report, archi::Int32 user_id, T & root) = 0;

   // DocumentValidatorMaster
   virtual void   request_validate (ValidationReportWriter & report, archi::Int32 user_id, Object & obj);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DocumentValidator (const DocumentValidator &other);
   DocumentValidator &
                  operator = (const DocumentValidator &other);
   bool           operator == (const DocumentValidator &other);
   bool           operator != (const DocumentValidator &other);

}; // class DocumentValidator



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/flip/DocumentValidator.hpp"



#endif   // ohm_flip_DocumentValidator_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
