/*****************************************************************************

        PageBase.cpp
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



#if defined (_MSC_VER)
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "plex/http/PageBase.h"

#include "plex/shared/Configuration.h"

#include "ohm/txt/out/Print.h"
#include "ohm/txt/utf8/ConvNum.h"

#include <cassert>



namespace plex
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

PageBase::PageBase (HttpCentral & central, const std::string & title)
:  _central (central)
,  _title (title)

,  _action ()
,  _args ()
,  _ts (0)

,  _html_flag (false)
,  _html ()
,  _js ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

PageBase::~PageBase ()
{
}



/*
==============================================================================
Name : use_central
==============================================================================
*/

HttpCentral &  PageBase::use_central ()
{
   return _central;
}



/*
==============================================================================
Name : init
==============================================================================
*/

void  PageBase::init (const std::string & action, const std::map <std::string, std::string> & args)
{
   using namespace ohm;
   
   _action = action;
   _args = args;
   
   std::map <std::string, std::string>::const_iterator it = args.find ("ts");
   
   if (it != args.end ())
   {
      const std::string & ts_str = it->second;
      
      _ts = time_t (txt::utf8::ConvNum::conv_str_to_int64 (ts_str.c_str ()));
   }
}



/*
==============================================================================
Name : process_html
==============================================================================
*/

void  PageBase::process_html ()
{
   _html_flag = true;
   
   line ("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"");
   line ("  \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">");
   
   line ("<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\" dir=\"ltr\">");
   
   line ("<head>");
   line ("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />");
   
   line ("<title>" + _title + "</title>");
   line ("<style type=\"text/css\">");
   
   add_styles ();
   
   line ("</style>");
   line ("<script>");

   add_scripts ();

   line ("</script>");
   line ("</head>");
   
   line ("<body>");
   
   line ("<div class=\"dbanner\">");
   line ("<div class=\"logo\"></div>");
   
   add_banner_menu (_action, "status", "Status");
   add_banner_menu (_action, "logs", "Logs");
   add_banner_menu (_action, "sessions", "Sessions");
   //add_banner_menu (_action, "install", "Install");
   add_banner_menu (_action, "restart", "Restart");
   
   line ("</div>");

   process_body ();
   
   line ("</body>");
   line ("</html>");
}



/*
==============================================================================
Name : get_html
==============================================================================
*/

const std::string &  PageBase::get_html () const
{
   return _html;
}



/*
==============================================================================
Name : get_js
==============================================================================
*/

const std::string &  PageBase::get_js () const
{
   return _js;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : line
==============================================================================
*/

void  PageBase::line (const std::string & str)
{
   _html += str + "\r\n";
}



/*
==============================================================================
Name : flush_script
==============================================================================
*/

void  PageBase::flush_script (const std::string & div)
{
   if (_html_flag)
   {
      return;  // ignore
   }
   
   escape_html (_html);

   _js += "document.getElementById(\"" + div + "\").innerHTML=\"" + _html + "\"; ";
   
   _html.clear ();
}



/*
==============================================================================
Name : process_tr
==============================================================================
*/

void  PageBase::process_tr (const std::string & key, int value)
{
   using namespace ohm;
   
   txt::out::Print printer;
   printer.print ("%1%").arg (value);
   
   process_tr (key, printer);
}



/*
==============================================================================
Name : process_tr
==============================================================================
*/

void  PageBase::process_tr (const std::string & key, double value)
{
   using namespace ohm;
   
   txt::out::Print printer;
   printer.print ("%1%").arg (value);
   
   process_tr (key, printer);
}



/*
==============================================================================
Name : process_tr
==============================================================================
*/

void  PageBase::process_tr (const std::string & key, const std::string & value)
{
   line ("<tr class=\"tr_item\" bgcolor=\"#6b6b6b\" onMouseOver=\"this.bgColor='#777777';\" onMouseOut=\"this.bgColor='#6b6b6b';\">");
   line ("<td>" + key + "</td><td>" + value + "</td>");
   line ("</tr>");
}



/*
==============================================================================
Name : get_args
==============================================================================
*/

const std::map <std::string, std::string> &  PageBase::get_args () const
{
   return _args;
}



/*
==============================================================================
Name : need_refresh
==============================================================================
*/

bool  PageBase::need_refresh (time_t ts) const
{
#if 0
   return true;

#elif 1
   if (_ts == 0)
   {
      return true;
   }
   
   return ts + 1 > _ts;
#endif
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : add_styles
==============================================================================
*/

void  PageBase::add_styles ()
{
   line ("* {font-family: arial; color: #eee; margin: 0; padding: 0;}");
   line ("body {background-color: #6b6b6b; font-size: 12px;}");
   
   line (".dbanner {background-color: #000; width: 100%; height: 80px;}");
   line (".logo {width: 53px; height: 26px; margin: 30px 0 0 40px; background:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAADUAAAAaCAYAAAAXHBSTAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAAMpJREFUeNrsVgsOQDAM3cS9cDKcDCejhEXorJONrelLliy1Nl7/egYoD2jAccd0z9+f4NL1+K9xPaDaGzsMSBlyoN6sl0LxQQ1+GHKMVHcmsZ8buU35CirBmLoU2yBqkWetjunt0JHCbGPvsq4p4FNh8twbRYfJCmq+v2ix0SKz19JgaRR+3e+a04nNKWOD05wy6VhyGr4QpMlKippCCeC292UXKaqzudUUX1LB08/Vir+oV0k/ISUQuOfZjxv362VYGoWQShiLAAMAf7H1ZcAKSVwAAAAASUVORK5CYII=) top left no-repeat;  display:inline-block;}");
   line (".bannersec {width:60px; padding: 50px 0 0 40px; display:inline-block;}");
   line (".banneron {text-decoration: none; color: #eee; font-size: 14px;}");
   line (".banneroff {text-decoration: none; color: #aaa; font-size: 14px;}");
   
   line (".section {color: #eee; font-size: 18px; margin: 2em 0 0 1em; }");
   line (".dsection {border-style:solid; border-width:1px; border-color:#999; margin: 0.5em 1em 1em 1em; padding: 1em 1em;}");

   line (".sectionr {color: #f00; font-size: 18px; margin: 2em 0 0 1em; }");
   line (".dsectionr {border-style:solid; border-width:1px; border-color:#f00; margin: 0.5em 1em 1em 1em; padding: 1em 1em;}");
   
   line (".tr_h {color: #ccc; font-size: 12px; width:50px;}");
   line (".tr_item {height:18px;}");

   line ("option {color:black;}");
   line ("[disabled] {color:#999;}");
   
   line (".log_date {color: #ababab; float:right; font-size: 11px;}");
   line (".log_info {color: #ababab; font-size: 12px; margin: 0.25em 2em; background:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAAJZJREFUeNpi/P//PwMpgImBRMACIhgZGRmuXbsmD2Q2AHEAEAtA5T8A8QaQuJaW1kOQaxhBxPXr1/WBggeQFKIDkEYHTU3NizAnTcCjmAEqNwHZDw5EON8Bw9M1NTVYaWQA88N+Imw5APSDI8yGAgKKP8DUwDQkQOkLUElkhQuA2AAYrBfBIiAnAePAnphIg8cDKQAgwADrBzbfLCBsLAAAAABJRU5ErkJggg==) top left no-repeat; text-indent:1.5em;}");
   line (".log_error {color: #c6c6c6; font-size: 12px; margin: 0.25em 2em; background:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAAM1JREFUeNpiYMAC/lxx4AdhbHJMDNhBARRjAEZspgOpB1CuAovOgY+EbCj4//+fAAhjs4URm+l/f38FKWZgZuX+gG4LE4bp//4IRDbLMoAwiI1uCxOa6QW/fyCcDGUXIIcYsg0F//7+Fvj14x1cAMQGiSHbwojs9u+fngj8/vEBxY2sHAIMnHwycL/AbCj4++enwI+vrxj+/v3FkDLFDoxBbLAYUA5mCyPM9C/v7gj8/Poaayyyc4sy8AipQGyB6hQACoAEGfAAsC0AAQYAut9nKtCxcC8AAAAASUVORK5CYII=) top left no-repeat; text-indent:1.5em; }");
   line (".log_incident {color: #ffffff; font-size: 12px; margin: 0.25em 2em; background:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAATpJREFUeNqUks9Kw0AQxr+kCUlMD4tVoYhKRT0J+gb6AB4k4FGvvkaexZMghAo+gN48VtCTQvEkmjbmkJqYPxt3lqSm9uTAlxm+/U12NhsFVTiOsyGSK3QsxCo7FOqT73neKxlKBe+JdLucF+xkkmCl4JL+aKm4sk34WosaD0XTg1K9eXAQxewsjGCXJZoxURRcsDbu2hY17Wu03VKWs/P3QAIcs2EJnfqfeDJ0NtI1V6WZHWHwNJUyBvdztZl8gxhiaQfWiRO5IEfY2oX98jit6yCGWE2OkWcoshR/o+nxXJeZGkJfVdlOmkmDvQ0RdntztWDkZ6Zn/3qti4gXcqygszo9Q13TGjHEUoM7No3wcnMdUclRCKAp8miNGGJnLm7xK2ZHz8P6gBhbJm62ewgWrN+L+++v8SPAAIaErQ09pwBmAAAAAElFTkSuQmCC) top left no-repeat; text-indent:1.5em; }");
}



/*
==============================================================================
Name : add_scripts
==============================================================================
*/

void  PageBase::add_scripts ()
{
   line ("var timestamp = 0;");
   
   // load_execute
   
   line ("function load_execute(url) {");
   line ("  var xmlhttp = new XMLHttpRequest();");

   line ("  xmlhttp.onreadystatechange=function() {");
   line ("     if (xmlhttp.readyState==4 && xmlhttp.status==200) {");
   line ("        var result = xmlhttp.responseText;");
   line ("        eval (result);");
   line ("     }");
   line ("  };");
   
   line ("  xmlhttp.open(\"GET\",url+\"?ts=\"+timestamp,true);");
   line ("  xmlhttp.send();");
   
   line ("}");

   // signal
   
   line ("function signal(url) {");
   line ("  var xmlhttp = new XMLHttpRequest();");

   line ("  xmlhttp.open(\"GET\",url+\"?ts=\"+timestamp,true);");
   line ("  xmlhttp.send();");
   
   line ("}");

   // signal_select
   
   line ("function signal_select(url,id) {");
   line ("  var xmlhttp = new XMLHttpRequest();");
   
   line ("  var e = document.getElementById(id);");
   line ("  if (e.selectedIndex >= 0) {");
   line ("     var value = e.options[e.selectedIndex].value;");
   line ("     var urlf = url+\"?ts=\"+timestamp+\"&rev=\"+value;");

   line ("     xmlhttp.open(\"GET\",urlf,true);");
   line ("     xmlhttp.send();");
   line ("  }");
   
   line ("}");
   
   const Configuration & conf = Configuration::get ();
   
   const std::string url = "/" + _action + ".xhr";
   const std::string div = _action;
   
   line ("setInterval(function(){load_execute('" + url + "')},1000);");
}



/*
==============================================================================
Name : add_banner_menu
==============================================================================
*/

void  PageBase::add_banner_menu (const std::string & action, const std::string & sub_action, const std::string & name)
{
   const Configuration & conf = Configuration::get ();
   std::string base_url = "/";
   
   const std::string style = (action == sub_action) ? "banneron" : "banneroff";
   const std::string resource = base_url + sub_action;
   line ("<div class=\"bannersec\"><a class=\"" + style + "\" href=\"" + resource + "\">" + name + "</a></div>");
}



/*
==============================================================================
Name : escape_html
==============================================================================
*/

void  PageBase::escape_html (std::string & str)
{
   std::string esc;
   
   for (size_t i = 0 ; i < str.size () ; ++i)
   {
      char c = str [i];
      
      if (c == '"')
      {
         esc += "\\\"";
      }
      else if (c == '\'')
      {
         esc += "\\\\'";
      }
      else if ((c == '\r') || (c == '\n'))
      {
         esc += "\\n";
      }
      else
      {
         esc.push_back (c);
      }
   }
   
   str.swap (esc);
}



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

