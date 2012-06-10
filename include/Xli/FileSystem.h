#ifndef __XLI_FILESYSTEM_H__
#define __XLI_FILESYSTEM_H__

#include <Xli/Stream.h>

namespace Xli
{
	enum FileMode
	{
		/** Open a file for reading. The file must exist. */
		FileModeRead = 1 << 0,
		/** Create an empty file for writing. If a file with the same name already exists its content is erased and the file is treated as a new empty file. */
		FileModeWrite = 1 << 1,
		/** Append to a file. Writing operations append data at the end of the file. The file is created if it does not exist. */
		FileModeAppend = 1 << 2,
		/** Creates a new empty file. */
		FileModeNew = 1 << 4,
		/** Open a file for random access */
		FileModeRandom = 1 << 5,

		/** Open a file for update both reading and writing. The file must exist. */
		FileModeReadWrite = FileModeRead | FileModeWrite,
		/** Create an empty file for both reading and writing. If a file with the same name already exists its content is erased and the file is treated as a new empty file. */
		FileModeReadWriteNew = FileModeRead | FileModeWrite | FileModeNew,
		/** Open a file for reading and appending. All writing operations are performed at the end of the file, protecting the previous content to be overwritten. You can reposition (fseek, rewind) the internal pointer to anywhere in the file for reading, but writing operations will move it back to the end of file. The file is created if it does not exist. */
		FileModeReadAppend = FileModeRead | FileModeAppend,
		/** Open a file for random reading */
		FileModeReadRandom = FileModeRead | FileModeRandom,
	};

	const char* FileModeToString(FileMode mode);

	enum FileFlag
	{
		FileFlagReadOnly = 1 << 0,
		FileFlagDirectory = 1 << 1
	};

	struct FileInfo
	{
		String Name;
		UInt64 Size;
		UInt32 Flags;
		Timestamp CreationTime;
		Timestamp LastAccessTime;
		Timestamp LastWriteTime;
	};

	class FileSystem: public Object
	{
	public:
		virtual ~FileSystem();

		virtual Stream* OpenFile(const String& fileName, FileMode mode = FileModeRead) = 0;
		virtual DataAccessor* OpenFileAsBuffer(const String& fileName);

		virtual void CreateDirectory(const String& name);
		virtual void DeleteDirectory(const String& name);
		virtual void DeleteFile(const String& name);

		virtual void MoveDirectory(const String& oldName, const String& newName);
		virtual void MoveFile(const String& oldName, const String& newName);

		virtual bool FileExists(const String& path);
		virtual FileInfo GetFileInfo(const String& path);
		virtual void GetFiles(const String& path, Array<FileInfo>& list);


		void GetFiles(Array<FileInfo>& list) { GetFiles(".", list); }

		/**
			Makes sure all directories in the path exists.
		*/
		void CreateDirectories(const String& path);

		/**
			Removes directory and all contents.
		*/
		void DeleteDirectoryRecursive(const String& name);

		virtual FileSystem* CreateSubFileSystem(const String& path);
	};
}

#endif
