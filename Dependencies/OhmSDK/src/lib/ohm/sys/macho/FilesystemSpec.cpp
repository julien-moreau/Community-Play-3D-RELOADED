/*****************************************************************************

        FilesystemSpec.cpp
        Copyright (c) 2005 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 20445

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
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/sys/Err.h"
#include "ohm/sys/FilesystemSpec.h"

#include <list>



namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



FilesystemSpec::~FilesystemSpec ()
{
   _impl_ptr = 0;
}



bool  FilesystemSpec::is_case_sensitive () const
{
   /*
   
   OSStatus getVRefNumFromPath (unsigned char * utf8Path, short * outVRefNum);

int main (int argc, const char * argv[]) {
OSStatus status;
HIOParam HFSParameterBlock;
GetVolParmsInfoBuffer volParamsInfoBuffer;
short vRefNum;

if (argc < 2)  // If no parameters were given on the command line...
vRefNum = 0;   // take the default volume
else
{
// Get the vRefnum from the paths. 
// There are many other ways to get the vRefNum
// I leave that as an excercise to the reader
status = getVRefNumFromPath((unsigned char *)argv[1], &vRefNum);
if (status != noErr)
return status;
}

HFSParameterBlock.ioNamePtr = NULL;
HFSParameterBlock.ioVRefNum = vRefNum; 
HFSParameterBlock.ioBuffer = (void*)&volParamsInfoBuffer;
HFSParameterBlock.ioReqCount = sizeof(GetVolParmsInfoBuffer);

status = PBHGetVolParmsSync ((HParmBlkPtr)&HFSParameterBlock); 

if (status == noErr)
{
if (volParamsInfoBuffer.vMAttrib & bIsCaseSensitive == bIsCaseSensitive)
puts ("Volume is case-sensitive");
else
puts ("Volume is case-insensitive");
}
else
printf ("Error occured: %d", status);

return status;
}

OSStatus getVRefNumFromPath (unsigned char * utf8Path, short * outVRefNum)
{
OSStatus status;
FSRef myRef;
FSCatalogInfo myCatInfo;
FSSpec myFSSpec;

status = FSPathMakeRef (utf8Path, &myRef, NULL);
if (status == noErr)
status = FSGetCatalogInfo (&myRef, kFSCatInfoVolume, &myCatInfo, NULL, &myFSSpec, NULL);

if (status == noErr && outVRefNum != NULL)
*outVRefNum = myFSSpec.vRefNum; // Or myCatInfo.volume, they are the same

return status;
}
   
   */
   
   
   return (false);
}



// is used to have a buffer that can hold a file path
// in its fopen format

long  FilesystemSpec::get_max_filename_length () const
{
   return (PATH_MAX);
}



// is used to have a buffer that can hold a file path
// in its fopen format

long  FilesystemSpec::get_max_pathname_length () const
{
   return (PATH_MAX);
}



// simplify_path is NOT suitable for comparison
// we only contract on '.' and '..' without checking
// the validity of the path

int   FilesystemSpec::simplify_path (std::string &path) const
{
   assert (&path != 0);
   assert (path [path.size () - 1] != '/');
   
   // cut the string on '/'
   
   std::list <std::string> str_arr;
   
   bool loop_flag = true;
   
   std::string::size_type pos = 0;
   
   while (loop_flag)
   {
      std::string::size_type pos2 = path.find ('/', pos);
      
      if (pos2 != std::string::npos)
      {
         std::string str = path.substr (pos, pos2 - pos);
         
         str_arr.push_back (str);
         
         pos = pos2 + 1;
      }
      else
      {
         std::string str = path.substr (pos);
         
         str_arr.push_back (str);
         
         loop_flag = false;
      }
   }
   
   // remove all '.'
   
   std::list <std::string>::iterator it = str_arr.begin ();
   const std::list <std::string>::iterator it_end= str_arr.end ();
   
   while (it != it_end)
   {
      const std::string & str = *it;
      
      if (str == ".")
      {
         str_arr.erase (it);
         
         it = str_arr.begin ();
      }
      else
      {
         ++it;
      }
   }
   
   // remove '..' and pred
   
   assert (str_arr.front () != "..");
   
   it = str_arr.begin ();
   
   while (it != it_end)
   {
      const std::string & str = *it;
      
      if (str == "..")
      {
         std::list <std::string>::iterator it_pred = it;
         --it_pred;
         
         str_arr.erase (it);
         str_arr.erase (it_pred);
         
         it = str_arr.begin ();
      }
      else
      {
         ++it;
      }
   }
   
   // recompose
   
   it = str_arr.begin ();
   
   path = "";
   
   for (; it != it_end ; ++it)
   {
      path += "/";
      path += *it;
   }
   
   return (Err_OK);
}



/*int FilesystemSpec::simplify_path (std::string &path) const
{
   assert (&path != 0);

   // do nothing
//#* LDS: hein ?
//#* RDI: oui, de toute facon on va s'en passer
   return (Err_OK);
}*/



bool  FilesystemSpec::is_dir_delimiter (const wchar_t c) const
{
   return (c == L'/');
}



char  FilesystemSpec::get_default_dir_delimiter () const
{
   return ('/');
}



FilesystemSpec &  FilesystemSpec::use_instance ()
{
   static FilesystemSpec   instance;

   return (instance);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



FilesystemSpec::FilesystemSpec ()
:  _impl_ptr (0)
{
   // Nothing
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
