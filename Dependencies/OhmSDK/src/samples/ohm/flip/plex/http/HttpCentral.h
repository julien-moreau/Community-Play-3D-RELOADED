/*****************************************************************************

        HttpCentral.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71330

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



#if ! defined (plex_HttpCentral_HEADER_INCLUDED)
#define  plex_HttpCentral_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"



namespace ohm
{
namespace util
{
class BinaryStreamInput;
}
}



namespace plex
{



class PageBase;

class HttpCentral
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef PageBase * (*CreateProc) (HttpCentral &);
   
                  HttpCentral ();
   virtual        ~HttpCentral ();

   void           run ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   
   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   typedef std::map <std::string, CreateProc>   ActionCreateProcMap;
   
   void           add (const std::string & action, CreateProc create_proc);

   void           send_msg (const std::vector <ohm::archi::UByte> & data);
   
   void           process ();
   
   bool           process_command ();
   void           process_http_request (ohm::util::BinaryStreamInput & bsi);
   void           process_info (ohm::util::BinaryStreamInput & bsi);
   
   void           process_http_request_html (bool & continue_flag, const std::string & action, const std::map <std::string, std::string> & args);
   void           process_http_request_xhr (bool & continue_flag, const std::string & action, const std::map <std::string, std::string> & args);
   void           process_http_request_restart (bool & continue_flag, const std::string & action, const std::map <std::string, std::string> & args);
   void           process_http_request_shutdown (bool & continue_flag, const std::string & action, const std::map <std::string, std::string> & args);

   int            process_html (const std::string & action, CreateProc create_proc, const std::map <std::string, std::string> & args);
   int            process_xhr (const std::string & action, CreateProc create_proc, const std::map <std::string, std::string> & args);

   bool           read (ohm::archi::UByte * data_ptr, size_t data_size);
   bool           write (const std::vector <ohm::archi::UByte> & data);
   bool           write (const ohm::archi::UByte * data_ptr, size_t data_size);

   void           send_compress (const char * str, size_t size);
   void           send_404 ();
   
   bool           _run_flag;

   ActionCreateProcMap
                  _action_html_map;
   ActionCreateProcMap
                  _action_xhr_map;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  HttpCentral (const HttpCentral &other);
   HttpCentral &     operator = (const HttpCentral &other);
   bool           operator == (const HttpCentral &other);
   bool           operator != (const HttpCentral &other);

}; // class HttpCentral



}  // namespace plex



//#include  "HttpCentral.hpp"



#endif   // plex_HttpCentral_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

