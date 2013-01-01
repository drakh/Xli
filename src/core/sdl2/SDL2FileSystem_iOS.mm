#include "SDL2FileSystem.h"
#include <Xli/Unicode.h>

namespace Xli
{
    // TODO: Should return shared dir accessible from global filesystem
    String SDL2FileSystem::GetDocumentsDirectory()
    {
        NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString* documentsDirectory = [paths objectAtIndex:0];
        return Unicode::Utf8To16([documentsDirectory UTF8String]);
    }
    
    String SDL2FileSystem::GetRoamingAppDataDirectory()
    {
        NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString* documentsDirectory = [paths objectAtIndex:0];
        return Unicode::Utf8To16([documentsDirectory UTF8String]);
    }

    String SDL2FileSystem::GetLocalAppDataDirectory()
    {
        NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString* documentsDirectory = [paths objectAtIndex:0];
        return Unicode::Utf8To16([documentsDirectory UTF8String]);
    }
    
    String SDL2FileSystem::GetTempDirectory()
    {
        return Unicode::Utf8To16([NSTemporaryDirectory() UTF8String]);
    }
}
