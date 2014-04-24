#include <Xli/Disk.h>
#include <Xli/Path.h>

namespace Xli
{
    FileSystem* CreateBundleFileSystem()
    {
    	String base = Disk->GetBaseDirectory();

        const char* possibleDataSuffixes[] =
        {
            "data",
            "../data",
            "../../data",
            "../../../data",
        };

        for (int i = 0; i < sizeof(possibleDataSuffixes); i++)
        {
        	String bundle = Path::Combine(base, possibleDataSuffixes[i]);

        	if (Disk->IsDirectory(bundle))
        		return Disk->CreateSubFileSystem(bundle);
        }

        return Disk->CreateSubFileSystem("data");
    }
}
