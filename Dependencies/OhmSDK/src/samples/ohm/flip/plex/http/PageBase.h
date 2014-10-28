/*****************************************************************************

        PageBase.h
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



#if ! defined (plex_PageBase_HEADER_INCLUDED)
#define  plex_PageBase_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace plex
{



class HttpCentral;

class PageBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  PageBase (HttpCentral & central, const std::string & title);
   virtual        ~PageBase ();
   
   HttpCentral &  use_central ();
   
   void           init (const std::string & action, const std::map <std::string, std::string> & args);
   
   void           process_html ();
   
   virtual void   process_body () = 0;
   virtual void   process_xhr () = 0;
   
   const std::string &
                  get_html () const;
   const std::string &
                  get_js () const;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   void           line (const std::string & str);
   void           flush_script (const std::string & div);

   void           process_tr (const std::string & key, int value);
   void           process_tr (const std::string & key, double value);
   void           process_tr (const std::string & key, const std::string & value);
   
   const std::map <std::string, std::string> &
                  get_args () const;
   bool           need_refresh (time_t ts) const;
   
   
   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           add_styles ();
   void           add_scripts ();

   void           add_banner_menu (const std::string & action, const std::string & sub_action, const std::string & name);

   void           escape_html (std::string & str);

   HttpCentral &  _central;
   const std::string
                  _title;
   std::string    _action;
   std::map <std::string, std::string>
                  _args;
   time_t         _ts;
   
   bool           _html_flag;
   std::string    _html;
   std::string    _js;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PageBase ();
                  PageBase (const PageBase &other);
   PageBase &  operator = (const PageBase &other);
   bool           operator == (const PageBase &other);
   bool           operator != (const PageBase &other);

}; // class PageBase



}  // namespace plex



//#include  "PageBase.hpp"



#endif   // plex_PageBase_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

