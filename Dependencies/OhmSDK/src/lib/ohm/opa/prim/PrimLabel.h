/*****************************************************************************

        PrimLabel.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 68945

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



#if ! defined (ohm_opa_PrimLabel_HEADER_INCLUDED)
#define  ohm_opa_PrimLabel_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/prim/PrimWidget.h"
#include "ohm/opak/base/TypeString.h"
#include "ohm/opak/base/TypeFloat.h"
#include "ohm/opak/base/LabelSimpleHelper.h"



namespace ohm
{
namespace opa
{



class PrimLabel
:  public PrimWidget
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  PrimLabel ();

   // Video Thread
   virtual void   init_display ();
   virtual void   restore_display ();
   virtual void   display (opak::GraphicContext & context);
   
   opak::TypeString <PrimLabel>
                  _label;
   opak::TypeFloat <PrimLabel>
                  _width;
   opak::TypeFloat <PrimLabel>
                  _flush_factor;
   opak::TypeBool <PrimLabel>
                  _underline_flag;
   

   
/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   virtual        ~PrimLabel ();
   
   

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           label_changed ();
   void           width_changed ();
   void           flush_factor_changed ();
   void           underline_flag_changed ();
   
   const opal::Font *
                  _font_ptr;
   std::auto_ptr <opak::LabelSimpleHelper>
                  _lsh_aptr;

   

/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PrimLabel (const PrimLabel &other);
   PrimLabel &    operator = (const PrimLabel &other);
   bool           operator == (const PrimLabel &other) const;
   bool           operator != (const PrimLabel &other) const;

}; // class PrimLabel



}  // namespace opa
}  // namespace ohm



//#include  "ohm/opa/PrimLabel.hpp"



#endif   // ohm_opa_PrimLabel_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

