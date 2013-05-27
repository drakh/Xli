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

	DataAccessor* FileSystem::OpenFileAsBuffer(const String& filename)
	{
		Managed<Stream> f = OpenFile(filename, FileModeRead);
		return f->CreateDataAccessor();
	}

	void FileSystem::CreateDirectory(const String& name) 
	{ 
		XLI_THROW_NOT_SUPPORTED(XLI_FUNC);
	}

	void FileSystem::DeleteDirectory(const String& name)
	{ 
		XLI_THROW_NOT_SUPPORTED(XLI_FUNC);
	}

	void FileSystem::DeleteFile(const String& name)
	{ 
		XLI_THROW_NOT_SUPPORTED(XLI_FUNC);
	}

	void FileSystem::MoveDirectory(const String& oldName, const String& newName)
	{ 
		XLI_THROW_NOT_SUPPORTED(XLI_FUNC);
	}
		
	void FileSystem::MoveFile(const String& oldName, const String& newName)
	{ 
		XLI_THROW_NOT_SUPPORTED(XLI_FUNC);
	}

	bool FileSystem::GetFileInfo(const String& path, FileInfo& result) 
	{ 
		//XLI_THROW_NOT_SUPPORTED(XLI_FUNC);
		return false;
	}

	void FileSystem::GetFiles(const String& path, Array<FileInfo>& list) 
	{ 
		//XLI_THROW_NOT_SUPPORTED(XLI_FUNC);
	}

	void FileSystem::GetFiles(Array<FileInfo>& list) 
	{ 
		GetFiles(".", list); 
	}

	bool FileSystem::Exists(const String& path)
	{
		FileInfo result;
		return GetFileInfo(path, result);
	}

	bool FileSystem::IsFile(const String& path)
	{
		FileInfo result;
		return GetFileInfo(path, result) && (result.Flags & FileFlagDirectory) == 0;
	}

	bool FileSystem::IsDirectory(const String& path)
	{
		FileInfo result;
		return GetFileInfo(path, result) && (result.Flags & FileFlagDirectory) == FileFlagDirectory;
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

		virtual Stream* OpenFile(const String& filename, FileMode mode = FileModeRead);
		virtual DataAccessor* OpenFileAsBuffer(const String& filename);

		virtual void CreateDirectory(const String& name);
		virtual void DeleteDirectory(const String& name);
		virtual void DeleteFile(const String& name);

		virtual void MoveDirectory(const String& oldName, const String& newName);
		virtual void MoveFile(const String& oldName, const String& newName);

		virtual bool GetFileInfo(const String& path, FileInfo& result);
		virtual void GetFiles(const String& path, Array<FileInfo>& list);

		virtual void GetFiles(Array<FileInfo>& list);

		virtual bool Exists(const String& path);
		virtual bool IsFile(const String& path);
		virtual bool IsDirectory(const String& path);

		virtual void CreateDirectories(const String& path);

		virtual void DeleteDirectoryRecursive(const String& name);

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

	Stream* SubFileSystem::OpenFile(const String& filename, FileMode mode)
	{
		return fs->OpenFile(path + filename, mode);
	}

	DataAccessor* SubFileSystem::OpenFileAsBuffer(const String& filename)
	{
		return fs->OpenFileAsBuffer(path + filename);
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

	bool SubFileSystem::Exists(const String& path)
	{
		return fs->Exists(this->path + path);
	}

	bool SubFileSystem::IsFile(const String& path)
	{
		return fs->IsFile(this->path + path);
	}

	bool SubFileSystem::IsDirectory(const String& path)
	{
		return fs->IsDirectory(this->path + path);
	}
	
	bool SubFileSystem::GetFileInfo(const String& path, FileInfo& result)
	{
		FileInfo info;
		if (fs->GetFileInfo(this->path + path, info))
		{
			info.Name = info.Name.Substring(this->path.Length(), info.Name.Length() - this->path.Length());
			return true;
		}
		return false;
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

	void SubFileSystem::GetFiles(Array<FileInfo>& list)
	{
		int s = list.Length();
		fs->GetFiles(this->path + path, list);
		for (int i = s; i < list.Length(); i++)
		{
			list[i].Name = list[i].Name.Substring(this->path.Length(), list[i].Name.Length() - this->path.Length());
		}
	}

	void SubFileSystem::CreateDirectories(const String& path)
	{
		fs->CreateDirectories(this->path + path);
	}

	void SubFileSystem::DeleteDirectoryRecursive(const String& name)
	{
		fs->DeleteDirectoryRecursive(this->path + path);
	}

	FileSystem* SubFileSystem::CreateSubFileSystem(const String& path)
	{
		return new SubFileSystem(fs.Get(), this->path + path);
	}
}
