#ifndef _H_ICP3D_SPY_INCLUDED__
#define _H_ICP3D_SPY_INCLUDED__

#include <irrlicht.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

namespace cp3d {

//! 
class ICP3DSpy {
public:

	ICP3DSpy(irr::io::IFileSystem *fileSystem, irr::core::stringc workingDirectory) {
		FileSystem = fileSystem;
		WorkingDirectory = workingDirectory;
	}

	//! Prepares the spy
	virtual void prepare()
	{ }

	//! Check the changed files by the user
	/*
	Example:
		ISpyFileList files; // Fill list with files you want to check
		ISpyFileList changedFiles = checkFiles(files);

		ISpyFileList::ConstIterator it = changedFiles.begin();
		for (; it != changedFiles.end(); ++it) {
			// Do something with changed files
		}
	*/
	virtual void check() = 0;

	//! Checks the given files
	ISpyFileList &checkFiles(ISpyFileList &files) {
		irr::core::list<irr::core::stringc> f;

		// For each file, verify if it exists and if changed time changed
		ISpyFileList::ConstIterator it = files.begin();
		for (; it != files.end(); ++it) {
			if (!FileSystem->existFile(*it) && !FileSystem->existFile(WorkingDirectory + *it))
				continue;

			time_t time = getChangedTime(*it);

			// If changed, add to the list and update dictionary
			if (Changes[*it] != time) {
				f.push_back(*it);
				Changes[*it] = time;
			}
		}

		return f;
	}

	//! Sets the new spy interval
	//! \param interval: the new spy interval
	void setSpyInterval(irr::u32 interval) {
		Interval = interval;
	}

	//! Returns the check interval
	irr::u32 getInterval() {
		return Interval;
	}

protected:
	//! Members
	irr::u32 Interval;

	irr::io::IFileSystem *FileSystem;
	irr::core::stringc WorkingDirectory;

	//! Returns the changed time of the given file
	//! \param filename: the file name on the file system
	//! \return: the changed time
	time_t getChangedTime(irr::core::stringc filename) {
		struct stat buf;
		int result = stat(filename.c_str(), &buf);

		if (result >= 0)
			return buf.st_mtime;

		return 0;
	}

private:
	irr::core::map<irr::core::stringc, time_t> Changes;

};

typedef irr::core::list<irr::core::stringc> ISpyFileList;

} /// End cp3d namespace

#endif
