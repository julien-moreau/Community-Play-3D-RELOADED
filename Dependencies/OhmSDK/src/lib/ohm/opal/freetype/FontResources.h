/*****************************************************************************

        FontResources.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 38115

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



#if ! defined (ohm_opal_FontResources_HEADER_INCLUDED)
#define  ohm_opal_FontResources_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/util/SingletonBase.h"

#include "ohm/archi/def.h"

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #define  NOMINMAX
   #define  WIN32_LEAN_AND_MEAN
   #include <windows.h>
#endif

#include <string>
#include <map>



namespace ohm
{
namespace opal
{



class FontResources
:  public util::SingletonBase <FontResources>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class Result
   {
   public:
      bool        _memory_flag;
      std::string _path;
      void *      _data_ptr;
      size_t      _data_size;
   };
   
   virtual        ~FontResources ();
   
   const Result & get (const std::string & name) const;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  FontResources ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   ohm_util_SingletonBase_INVITE_SINGLETON (FontResources);
   
   typedef std::map <std::string, Result> NameResultMap;
   
   NameResultMap  _name_result_map;
         
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   static BOOL CALLBACK
                  enum_resource_name_proc (::HMODULE hinst, ::LPCTSTR type_0, ::LPTSTR name_0, ::LONG_PTR lparam);
   BOOL CALLBACK
                  enum_resource_name (::HMODULE hinst, ::LPCTSTR type_0, ::LPTSTR name_0);
#endif


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  FontResources (const FontResources &other);
   FontResources &      operator = (const FontResources &other);
   bool           operator == (const FontResources &other);
   bool           operator != (const FontResources &other);

}; // class FontResources



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_FontResources_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
