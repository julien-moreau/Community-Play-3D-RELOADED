/*****************************************************************************

        FileOp.cpp
        Copyright (c) 2010 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 59820

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

#include "ohm/txt/out/PrintNa.h"
#include "ohm/util/Err.h"
#include "ohm/util/FileOp.h"
#include "ohm/util/trace.h"
#include "ohm/sys/FileRefFactory.h"
#include "ohm/sys/FileRefOp.h"
#include "ohm/sys/FileSystemspec.h"

#include <cassert>



namespace ohm
{
namespace util
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



// The first returned element can be an empty string, if the path starts with
// a delimiter (root).
// If the last character is a delimiter, the last element is empty.
// If consecutive delimiters are encountered, it generates empty strings too.
/*** To do: make it accept \\NETWORK\blah Windows paths and extra long Windows paths ***/
void  FileOp::split_path (std::vector <std::string> &elt_list, const std::string &path)
{
   assert (&elt_list != 0);

   const sys::FilesystemSpec &   fs_spec = sys::FilesystemSpec::use_instance ();

   elt_list.clear ();
   elt_list.push_back ("");

   const std::string::size_type  len = path.length ();
   for (std::string::size_type pos = 0; pos < len; ++pos)
   {
      const char     c = path [pos];
      if (fs_spec.is_dir_delimiter (c))
      {
         elt_list.push_back ("");
      }
      else
      {
         elt_list.back ().push_back (c);
      }
   }
}



// If there is no delimiter, the whole string is copied to the filename.
// The filename delimiter is copied to the resulting path.
void  FileOp::split_path_fname (std::string &path, std::string &filename, const std::string &fullpath)
{
   assert (&path != 0);
   assert (&filename != 0);
   assert (&fullpath != 0);

   const sys::FilesystemSpec &   fs_spec = sys::FilesystemSpec::use_instance ();

   path.clear ();
   filename = fullpath;

   const std::string::size_type  len = fullpath.length ();
   for (long pos = long (len) - 1; pos >= 0; --pos)
   {
      const char     c = fullpath [pos];
      if (fs_spec.is_dir_delimiter (c))
      {
         filename = fullpath.substr (pos + 1);
         path     = fullpath.substr (0, pos + 1);

         pos = 0; // Stop now
      }
   }
}



// Creates all the required sub-directories and returns the file ref on the
// accessed path (the deepest directory).
// The returned file ref is empty if there was an error.
sys::FileRefSPtr  FileOp::create_dir_and_enter (std::string path)
{
   assert (&path != 0);

   sys::FileRefSPtr     final_sptr;

   sys::FilesystemSpec &   fs_spec = sys::FilesystemSpec::use_instance ();
   int            ret_val = fs_spec.simplify_path (path);

   if (ret_val == Err_OK)
   {
      std::vector <std::string>  elt_list;
      split_path (elt_list, path);

      const size_t      nbr_elt = elt_list.size ();
      assert (nbr_elt > 0);
      const std::string base =
         elt_list [0] + fs_spec.get_default_dir_delimiter ();

      final_sptr = sys::FileRefFactory::from_utf8_path (base);
      if (final_sptr.get () == 0)
      {
         ohm_util_TRACE_ERROR1 (
            "create_dir_and_enter(): Cannot access root directory \"%1%\".",
            base.c_str ()
         );
      }

      for (size_t pos = 1; pos < nbr_elt && final_sptr.get () != 0; ++pos)
      {
         const std::string &  elt_name = elt_list [pos];
         assert (elt_name != ".");  // Because of the simplification
         assert (elt_name != "..");

         if (! elt_name.empty ())
         {
            const sys::FileRefSPtr  prev_sptr (final_sptr);
            final_sptr = sys::FileRefOp::mkdir (*final_sptr, elt_name);
            if (final_sptr.get () == 0)
            {
               const std::string prev_path = prev_sptr->to_utf8_path ();
               ohm_util_TRACE_ERROR2 (
                  "create_dir_and_enter(): Cannot create folder "
                  "\"%1%\" in directory \"%2%\".",
                  elt_name.c_str (),
                  prev_path.c_str ()
               );
            }
         }
      }
   }

   return (final_sptr);
}



std::string FileOp::remove_terminal_delim (const std::string &path)
{
   assert (&path != 0);

   const sys::FilesystemSpec &   fs_spec = sys::FilesystemSpec::use_instance ();

   const std::string::size_type  last_pos = path.length () - 1;
   std::string::size_type  pos = last_pos;

   // We don't erase the first character of the path, because a delimiter
   // here would mean it's the root.
   while (pos > 0 && fs_spec.is_dir_delimiter (path [pos]))
   {
      -- pos;
   }

   std::string    path_new (path);
   if (pos < last_pos)
   {
      path_new = path.substr (0, pos + 1);
   }

   return (path_new);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace util
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
