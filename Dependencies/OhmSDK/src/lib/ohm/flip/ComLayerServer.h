/*****************************************************************************

        ComLayerServer.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 63111

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



#if ! defined (ohm_flip_ComLayerServer_HEADER_INCLUDED)
#define  ohm_flip_ComLayerServer_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"

#include <string>



namespace ohm
{
namespace util
{
class BinaryStreamInput;
}



namespace flip
{



class ComLayerServerListener
{
public:
   virtual        ~ComLayerServerListener () {}
   
   virtual int    receive (const std::string & from, util::BinaryStreamInput & bsi) = 0;
   virtual void   notify_disconnected (const std::string & user_name) = 0;
};



class ComLayerServer
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~ComLayerServer () {}
   
   virtual void   bind (ComLayerServerListener & listener) = 0;
   
   virtual void   send (const std::string & to, const archi::UByte * data_ptr, size_t data_size) = 0;
   virtual void   broadcast (const archi::UByte * data_ptr, size_t data_size) = 0;
   virtual void   broadcast (const std::string & except, const archi::UByte * data_ptr, size_t data_size) = 0;
   virtual void   receive (const std::string & from, util::BinaryStreamInput & bsi) = 0;

   virtual void   notify_document_need_flush () = 0;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const ComLayerServer &other);
   bool           operator != (const ComLayerServer &other);

}; // class ComLayerServer



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/ComLayerServer.hpp"



#endif   // ohm_flip_ComLayerServer_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
