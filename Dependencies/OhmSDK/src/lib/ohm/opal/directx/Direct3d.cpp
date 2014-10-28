/*****************************************************************************

        Direct3d.cpp
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59383

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
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/directx/Direct3d.h"
#include "ohm/opal/directx/DirectXEngine.h"
#include "ohm/opal/Bitmap.h"
#include "ohm/opal/def.h"
#include "ohm/opal/PixArgb.h"
#include "ohm/opal/private/PngLoader.h"
#include "ohm/opal/win32/ApplicationImpl.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

Direct3d::~Direct3d ()
{
   assert (_direct_3d_ptr == 0);
}



/*
==============================================================================
Name : create
==============================================================================
*/

void  Direct3d::create ()
{
   assert (_direct_3d_ptr == 0);
   
   _direct_3d_ptr = Direct3DCreate9 (D3D_SDK_VERSION);
   assert (_direct_3d_ptr != 0);
}



/*
==============================================================================
Name : release
==============================================================================
*/

void  Direct3d::release ()
{
   assert (_direct_3d_ptr != 0);
   
   if (_direct_3d_ptr != 0)
   {
      _direct_3d_ptr->Release ();
      _direct_3d_ptr = 0;
   }
}



/*
==============================================================================
Name : get
==============================================================================
*/

::IDirect3D9 * Direct3d::get () const
{
   assert (_direct_3d_ptr != 0);
   
   return _direct_3d_ptr;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

Direct3d::Direct3d ()
:  _direct_3d_ptr (0)
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

