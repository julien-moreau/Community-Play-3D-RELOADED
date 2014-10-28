/*****************************************************************************

        SessSession.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71437

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



#if ! defined (plex_SessSession_HEADER_INCLUDED)
#define  plex_SessSession_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/flip/ComLayerServerBase.h"

#include <vector>
#include <string>
#include <set>



namespace ohm
{
namespace flip
{
class DocumentServer;
}
}



namespace model
{
class ModelValidator;
class ModelSignal;
class ModelTweaker;
}



namespace plex
{



class SessCentral;

class SessSession
:  public ohm::flip::ComLayerServerBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  SessSession (SessCentral & central);
   virtual        ~SessSession ();
   
   void           receive (const std::string & username, const ohm::archi::UByte * data_ptr, size_t data_size);
   
   // flip::ComLayerServerBase
   virtual void   notify_document_need_flush ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   // flip::ComLayerServerBase
   virtual void   do_send (const std::string & to, const ohm::archi::UByte * data_ptr, size_t data_size);
   virtual void   do_broadcast (const ohm::archi::UByte * data_ptr, size_t data_size);
   virtual void   do_broadcast (const std::string & except, const ohm::archi::UByte * data_ptr, size_t data_size);

   
   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   SessCentral &  _central;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  SessSession ();
                  SessSession (const SessSession &other);
   SessSession &     operator = (const SessSession &other);
   bool           operator == (const SessSession &other);
   bool           operator != (const SessSession &other);

}; // class SessSession



}  // namespace plex



//#include  "SessSession.hpp"



#endif   // plex_SessSession_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

