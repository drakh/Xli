#include <Xli.h>

namespace Xli
{
	const char* FileModeToString(FileMode mode)
	{
		switch (mode)
		{
#define FORMAT(x) case x: return #x
		FORMAT(FileModeRead);
		FORMAT(FileModeWrite);
		FORMAT(FileModeAppend);
		FORMAT(FileModeNew);
		FORMAT(FileModeRandom);
		FORMAT(FileModeReadWrite);
		FORMAT(FileModeReadWriteNew);
		FORMAT(FileModeReadAppend);
		FORMAT(FileModeReadRandom);
		default: return "<Unknown FileMode>";
#undef FORMAT
		}
	}

	FileSystem::~FileSystem()
	{
	}

	DataAccessor* FileSystem::OpenFileAsBuffer(const String& fileName)
	{
		Managed<Stream> f = OpenFile(fileName, FileModeRead);
		return f->CreateDataAccessor();
	}

	void FileSystem::CreateDirectory(const String& name) 
	{ 
		XLI_THROW_NOT_SUPPORTED(__FUNCTION__);
	}

	void FileSystem::DeleteDirectory(const String& name)
	{ 
		XLI_THROW_NOT_SUPPORTED(__FUNCTION__);
	}

	void FileSystem::DeleteFile(const String& name)
	{ 
		XLI_THROW_NOT_SUPPORTED(__FUNCTION__);
	}

	void FileSystem::MoveDirectory(const String& oldName, const String& newName)
	{ 
		XLI_THROW_NOT_SUPPORTED(__FUNCTION__);
	}
		
	void FileSystem::MoveFile(const String& oldName, const String& newName)
	{ 
		XLI_THROW_NOT_SUPPORTED(__FUNCTION__);
	}

	FileInfo FileSystem::GetFileInfo(const String& path) 
	{ 
		XLI_THROW_NOT_SUPPORTED(__FUNCTION__);
	}

	void FileSystem::GetFiles(const String& path, Array<FileInfo>& list) 
	{ 
		XLI_THROW_NOT_SUPPORTED(__FUNCTION__);
	}

	bool FileSystem::FileExists(const String& path)
	{
		XLI_THROW_NOT_SUPPORTED(__FUNCTION__);
	}

	void FileSystem::CreateDirectories(const String& path)
	{
		Array<String> parts;
		path.Split('/', parts);

		String currentDir;
		for (int i = 0; i < parts.Length(); i++)
		{
			currentDir = currentDir + parts[i];

			if (parts[i].Length() > 0 && parts[i] != "." && parts[i] != "..")
			{
				CreateDirectory(currentDir);
			}

			currentDir = currentDir + '/';
		}
	}

	void FileSystem::DeleteDirectoryRecursive(const String& path)
	{
		Array<FileInfo> files;
		GetFiles(path, files);

		for (int i = 0; i < files.Length(); i++)
		{
			if (files[i].Flags & FileFlagDirectory)
			{
				DeleteDirectoryRecursive(files[i].Name);
			}
			else
			{
				DeleteFile(files[i].Name);
			}
		}

		DeleteDirectory(path);
	}

	class SubFileSystem: public FileSystem
	{
		Shared<FileSystem> fs;
		String path;

	public:
		SubFileSystem(FileSystem* fs, const String& path);

		virtual Stream* OpenFile(const String& fileName, FileMode mode);

		virtual void CreateDirectory(const String& name);
		virtual void DeleteDirectory(const String& name);
		virtual void DeleteFile(const String& name);

		virtual void MoveDirectory(const String& oldName, const String& newName);
		virtual void MoveFile(const String& oldName, const String& newName);

		virtual bool FileExists(const String& path);
		virtual FileInfo GetFileInfo(const String& path);
		virtual void GetFiles(const String& path, Array<FileInfo>& list);

		virtual FileSystem* CreateSubFileSystem(const String& path);
	};

	FileSystem* FileSystem::CreateSubFileSystem(const String& path)
	{
		return new SubFileSystem(this, path);
	}

	SubFileSystem::SubFileSystem(FileSystem* fs, const String& path)
	{
		this->fs = fs;
		this->path = path;
		if (path.Length() > 0 && path.Last() != '/') this->path = this->path + '/';
	}

	Stream* SubFileSystem::OpenFile(const String& fileName, FileMode mode)
	{
		return fs->OpenFile(path + fileName, mode);
	}

	void SubFileSystem::CreateDirectory(const String& name)
	{
		fs->CreateDirectory(path + name);
	}

	void SubFileSystem::DeleteDirectory(const String& name)
	{
		fs->DeleteDirectory(path + name);
	}

	void SubFileSystem::DeleteFile(const String& name)
	{
		fs->DeleteFile(path + name);
	}

	void SubFileSystem::MoveDirectory(const String& oldName, const String& newName)
	{
		fs->MoveDirectory(path + oldName, path + newName);
	}

	void SubFileSystem::MoveFile(const String& oldName, const String& newName)
	{
		fs->MoveFile(path + oldName, path + newName);
	}

	bool SubFileSystem::FileExists(const String& path)
	{
		return fs->FileExists(this->path + path);
	}
	
	FileInfo SubFileSystem::GetFileInfo(const String& path)
	{
		FileInfo info = fs->GetFileInfo(this->path + path);
		info.Name = info.Name.Substring(this->path.Length(), info.Name.Length() - this->path.Length());
		return info;
	}

	void SubFileSystem::GetFiles(const String& path, Array<FileInfo>& list)
	{
		int s = list.Length();
		fs->GetFiles(this->path + path, list);
		for (int i = s; i < list.Length(); i++)
		{
			list[i].Name = list[i].Name.Substring(this->path.Length(), list[i].Name.Length() - this->path.Length());
		}
	}

	FileSystem* SubFileSystem::CreateSubFileSystem(const String& path)
	{
		return new SubFileSystem(fs.Get(), this->path + path);
	}
}
