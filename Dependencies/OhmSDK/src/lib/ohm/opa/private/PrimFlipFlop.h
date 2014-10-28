/*****************************************************************************

        PrimFlipFlop.h
        Copyright (c) 2009 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 38302

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



#if ! defined (ohm_opa_PrimFlipFlop_HEADER_INCLUDED)
#define  ohm_opa_PrimFlipFlop_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/prim/PrimWidget.h"
#include "ohm/opak/base/TypeEnum.h"
#include "ohm/opak/base/TypeBool.h"

#include <vector>



namespace ohm
{
namespace opa
{



class PrimFlipFlop
:  public PrimWidget
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  PrimFlipFlop ();
   
   // Video Thread
   virtual void   display (opak::GraphicContext & context);

   opak::TypeBool <PrimFlipFlop>
                  _flip_flop_flag;
   opak::TypeBool <PrimFlipFlop>
                  _hit_flag;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   virtual        ~PrimFlipFlop ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           flip_flop_flag_changed ();
   void           hit_flag_changed ();



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PrimFlipFlop (const PrimFlipFlop &other);
   PrimFlipFlop & operator = (const PrimFlipFlop &other);
   bool           operator == (const PrimFlipFlop &other) const;
   bool           operator != (const PrimFlipFlop &other) const;

}; // class PrimFlipFlop



}  // namespace opa
}  // namespace ohm



//#include  "ohm/opa/PrimFlipFlop.hpp"



#endif   // ohm_opa_PrimFlipFlop_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

