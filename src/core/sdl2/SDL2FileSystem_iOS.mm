#include "SDL2FileSystem.h"
#include <Xli/Unicode.h>

namespace Xli
{
    // TODO: Should return shared dir accessible from global filesystem
    String SDL2FileSystem::GetSystemDirectory(SystemDirectory dir)
    {
        NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString* documentsDirectory = [paths objectAtIndex:0];
        return [documentsDirectory UTF8String];
    }
    
    String SDL2FileSystem::GetTempDirectory()
    {
        return [NSTemporaryDirectory() UTF8String];
    }
}
