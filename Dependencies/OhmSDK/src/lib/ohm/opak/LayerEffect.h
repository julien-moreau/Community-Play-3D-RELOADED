/*****************************************************************************

        LayerEffect.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 45538

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



#if ! defined (ohm_opak_LayerEffect_HEADER_INCLUDED)
#define  ohm_opak_LayerEffect_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/base/PrimitiveContainer.h"
#include "ohm/opak/base/TypeBool.h"
#include "ohm/opak/base/TypeFloat.h"
#include "ohm/opal/private/FragmentProgramProc.h"
#include "ohm/opal/Cache.h"

#include <list>



namespace ohm
{
namespace opak
{



class LayerEffect
:  public PrimitiveContainer
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
public:
   
                  LayerEffect ();
   
   // Video Thread
   virtual void   display (GraphicContext & context);

   TypeBool <LayerEffect>
                  _active_flag;
   opal::FragmentProgramProc
                  _proc;
   TypeFloat <LayerEffect>
                  _var_0;
   TypeFloat <LayerEffect>
                  _var_1;
   TypeFloat <LayerEffect>
                  _var_2;
   TypeFloat <LayerEffect>
                  _var_3;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   virtual        ~LayerEffect ();

   virtual void   invalidate_all ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           active_flag_changed ();
   void           var_changed ();



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  LayerEffect (const LayerEffect &other);
   LayerEffect &  operator = (const LayerEffect &other);
   bool           operator == (const LayerEffect &other);
   bool           operator != (const LayerEffect &other);

}; // class LayerEffect



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/opak/LayerEffect.hpp"



#endif   // ohm_opak_LayerEffect_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
