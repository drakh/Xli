#ifndef __XLI_NATIVE_FILE_SYSTEM_H__
#define __XLI_NATIVE_FILE_SYSTEM_H__

#include <Xli/FileSystem.h>

namespace Xli
{
	class NativeFileSystem: public FileSystem
	{
	public:
		/**
			Generates and returns a unique filename that can be used for creating a temporary file. The temporary file must be deleted manually by the creator.
		*/
		virtual String CreateTempFilename() = 0;

		/**
			Returns the path on the file system where the user keeps his personal documents. E.g. the "My Documents" folder
		*/
		virtual String GetDocumentsDirectory() = 0;

		/**
			Returns the path on the file system where to store roaming app data
		*/
		virtual String GetRoamingAppDataDirectory() = 0;

		/**
			Returns the path on the file system where to store local app data
		*/
		virtual String GetLocalAppDataDirectory() = 0;

		/**
			Returns the working directory
		*/
		virtual String GetCurrentDirectory() = 0;

		/**
			Sets the working directory
		*/
		virtual void ChangeDirectory(const String& dir) = 0;

		static void Init();
		static void Shutdown();
	};

	class DiskAccessor
	{
	public:
		NativeFileSystem* operator ->();
		operator FileSystem*();
	};

	extern DiskAccessor Disk;

	class AssetFileSystem
	{
	public:
		static void Init();
		static void Shutdown();
	};

	class AssetAccessor
	{
	public:
		void SetFilesystem(FileSystem* fs);
		FileSystem* operator ->();
		operator FileSystem*();
	};

	extern AssetAccessor Assets;
}


#endif
