#import <Foundation/Foundation.h>
#include <Xli/File.h>
#include "../posix/PosixFileSystemBase.h"

namespace Xli
{
	class CocoaFileSystem: public PosixFileSystemBase
	{
	public:
        virtual Stream* Open(const String& filename, FileMode mode)
        {
            if ((mode & FileModeRead) != 0)
            {
                File* result = 0;
                
                @autoreleasepool {
                    NSFileManager* file_manager = [NSFileManager defaultManager];
                    NSString* resource_path = [[NSBundle mainBundle] resourcePath];
                    
                    NSString* ns_string_file_component = [file_manager stringWithFileSystemRepresentation:filename.Data() length:filename.Length()];
                    
                    NSString* full_path_with_file_to_try = [resource_path stringByAppendingPathComponent:ns_string_file_component];
                    if([file_manager fileExistsAtPath:full_path_with_file_to_try])
                    {
                        result = new File([full_path_with_file_to_try fileSystemRepresentation], mode);
                    }
                }
                
                if (result != 0)
                {
                    return result;
                }
            }
        
            return new File(filename, mode);
        }
        
	    // TODO: Should return shared dir accessible from global filesystem
	    virtual String GetSystemDirectory(SystemDirectory dir)
	    {
	        NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	        NSString* documentsDirectory = [paths objectAtIndex:0];
	        return [documentsDirectory UTF8String];
	    }
	    
	    virtual String GetTempDirectory()
	    {
	        return [NSTemporaryDirectory() UTF8String];
	    }
	};

	NativeFileSystem* CreateNativeFileSystem()
	{
		return new CocoaFileSystem();
	}

	FileSystem* CreateAssetFileSystem()
	{
		return Disk->CreateSubFileSystem("data");
	}
}