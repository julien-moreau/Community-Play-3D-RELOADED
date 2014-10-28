/*****************************************************************************

        ImageLoaderImpl.h
        Copyright (c) 2009 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
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



#if ! defined (ohm_opal_ImageLoaderImpl_HEADER_INCLUDED)
#define  ohm_opal_ImageLoaderImpl_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/util/SingletonBase.h"

#include "ohm/archi/types.h"

#include <string>



namespace ohm
{
namespace opal
{



class Bitmap;

class ImageLoaderImpl
:  public util::SingletonBase <ImageLoaderImpl>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   virtual        ~ImageLoaderImpl ();
   
   bool           load_from_resource (Bitmap & bitmap, const std::string & path, bool premultiply_flag);
   bool           load_from_path (Bitmap & bitmap, const std::string & path, bool premultiply_flag);
   bool           load_from_memory (Bitmap & bitmap, const archi::UByte * data_ptr, size_t data_size, bool premultiply_flag);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

                  ImageLoaderImpl ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   ohm_util_SingletonBase_INVITE_SINGLETON (ImageLoaderImpl)
   
   bool           load (Bitmap & bitmap, const std::string & path, bool resource_flag);
   bool           load (Bitmap & bitmap, const archi::UByte * data_ptr, size_t data_size);
            


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  ImageLoaderImpl (const ImageLoaderImpl &other);
   ImageLoaderImpl &    operator = (const ImageLoaderImpl &other);
   bool           operator == (const ImageLoaderImpl &other);
   bool           operator != (const ImageLoaderImpl &other);

}; // class ImageLoaderImpl



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_ImageLoaderImpl_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
