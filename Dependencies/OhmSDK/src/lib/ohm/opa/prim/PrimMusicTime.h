/*****************************************************************************

        PrimMusicTime.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 66675

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



#if ! defined (ohm_opa_PrimMusicTime_HEADER_INCLUDED)
#define  ohm_opa_PrimMusicTime_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/prim/PrimWidget.h"
#include "ohm/opak/base/PrimitiveContainer.h"
#include "ohm/opak/base/Sizable.h"
#include "ohm/opak/base/TypeInt.h"



namespace ohm
{
namespace opa
{



class PrimMusicTime
:  public PrimWidget
,  public opak::Sizable
,  public opak::PrimitiveContainer
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  PrimMusicTime ();
   virtual        ~PrimMusicTime ();
   
   // Video Thread
   virtual void   display (opak::GraphicContext & context);
   
   opak::TypeInt <PrimMusicTime>
                  _bar;
   opak::TypeInt <PrimMusicTime>
                  _beat;
   opak::TypeInt <PrimMusicTime>
                  _rem;
   opak::TypeInt <PrimMusicTime>
                  _tick;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   // opak::Sizable
   virtual void   do_size_changed ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           something_changed ();



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PrimMusicTime (const PrimMusicTime &other);
   PrimMusicTime &
                  operator = (const PrimMusicTime &other);
   bool           operator == (const PrimMusicTime &other) const;
   bool           operator != (const PrimMusicTime &other) const;

}; // class PrimMusicTime



}  // namespace opa
}  // namespace ohm



//#include  "ohm/opa/PrimMusicTime.hpp"



#endif   // ohm_opa_PrimMusicTime_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

