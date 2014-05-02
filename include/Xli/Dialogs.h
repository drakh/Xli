#ifndef __XLI_PLATFORM_DIALOGS_H__
#define __XLI_PLATFORM_DIALOGS_H__

#include <Xli/Window.h>

namespace Xli
{
    /**
        \ingroup XliCorePlatform
    */
    class Dialogs
    {
    public:
        struct FileDialogOptions
        {
            struct FileExtensionDesc
            {
                String Description;
                String Extension;
            };

            String Caption;
            String DefaultFolder;
            String DefaultFile;
            String DefaultExtension;
            Array<FileExtensionDesc> FileExtensions;

            FileDialogOptions(const String& caption)
                : Caption(caption) 
            {
            }

            void AddFileExtension(const String& ext, const String& desc)
            {
                FileExtensionDesc fed;
                fed.Extension = ext;
                fed.Description = desc;
                FileExtensions.Add(fed);
            }

            void AddFileExtension(const String& ext)
            {
                FileExtensionDesc fed;
                fed.Extension = ext;
                FileExtensions.Add(fed);
            }
        };

        static bool OpenFile(Window* parent, const FileDialogOptions& options, String& result);
        static bool SaveFile(Window* parent, const FileDialogOptions& options, String& result);
    };
}

#endif
