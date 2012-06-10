#include <XliMedia/ImageReader.h>
#include <XliMedia/Jpeg.h>
#include <XliMedia/Png.h>
#include <Xli/File.h>

#import <UIKit/UIImage.h>

namespace Xli
{
    class UIImageReader : public ImageReader
    {
        Shared<Stream> stream;
        Managed<DataAccessor> buf;

        UIImage* image;
        
    public:
        UIImageReader(Stream* stream)
        {
            this->stream = stream;
            buf = stream->CreateDataAccessor();
            
            NSData* data = [NSData dataWithBytesNoCopy:(void*)buf->GetData() length:buf->GetSizeInBytes() freeWhenDone:NO];
            
            if (data == nil)
            {
                XLI_THROW("Failed to open image: Unable to create NSData object");
            }
            
            image = [UIImage imageWithData:data];
            //[data release];

            if (image == nil)
            {
                XLI_THROW("Failed to open image: Unable to create NSImage object");
            }
        }
        
        virtual ~UIImageReader()
        {
            //[image release];
        }
        
		virtual int GetWidth()
		{
			return CGImageGetWidth(image.CGImage);
		}
		virtual int GetHeight()
		{
			return CGImageGetHeight(image.CGImage);
		}
		virtual int GetDepth()
		{
			return 1;
		}
        int GetComponentCount()
        {
            return CGImageGetBitsPerPixel(image.CGImage) / CGImageGetBitsPerComponent(image.CGImage);
        }
		virtual int GetBufferSize()
		{
            return GetWidth() * GetHeight() * FormatInfo::SizeOf(GetFormat());
		}
		virtual Format GetFormat()
		{
            int cc = GetComponentCount();
            if (cc == 1) return FormatL_8_UInt_Normalize;
            if (cc == 3) return FormatRGB_8_8_8_UInt_Normalize;
            return FormatRGBA_8_8_8_8_UInt_Normalize;
		}
        
		virtual void Read(void* targetBuffer, ProgressCallback* callback)
		{
            int width = CGImageGetWidth(image.CGImage);
            int height = CGImageGetHeight(image.CGImage);
                  
            Format format = GetFormat();
            int cc = FormatInfo::SizeOf(format);
            
            if (format == FormatRGBA_8_8_8_8_UInt_Normalize)
            {
                CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
                CGContextRef imgcontext = CGBitmapContextCreate(targetBuffer, width, height, 8, 4 * width, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
                CGColorSpaceRelease(colorSpace);
                CGContextClearRect(imgcontext, CGRectMake(0, 0, width, height));
                CGContextTranslateCTM(imgcontext, 0, 0);
                CGContextDrawImage(imgcontext, CGRectMake(0, 0, width, height), image.CGImage);
                CGContextRelease(imgcontext);
            }
            else 
            {
                Managed<Buffer> tmp = Buffer::Create(4 * width * height);
                
                CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
                CGContextRef imgcontext = CGBitmapContextCreate(tmp->Data(), width, height, 8, 4 * width, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
                CGColorSpaceRelease(colorSpace);
                CGContextClearRect(imgcontext, CGRectMake(0, 0, width, height));
                CGContextTranslateCTM(imgcontext, 0, 0);
                CGContextDrawImage(imgcontext, CGRectMake(0, 0, width, height), image.CGImage);
                CGContextRelease(imgcontext);
                
                UInt8* src = (UInt8*)tmp->Data();
                UInt8* dst = (UInt8*)targetBuffer;
                
                for (int i = 0; i < width * height; i++)
                {
                    for (int c = 0; c < 4; c++)
                    {
                        if (c < cc) *dst++ = *src;
                        src++;
                    }
                }
            }
		}        
    };
    
	ImageReader* Jpeg::CreateReader(Stream* input)
	{
		return new UIImageReader(input);
	}
    
	Bitmap* Jpeg::Load(Stream* input)
	{
		Managed<ImageReader> r = CreateReader(input);
		return r->ReadBitmap();
	}
	Bitmap* Jpeg::Load(const String& fileName)
	{
		Managed<File> f = new File(fileName, FileModeRead);
		return Load(f);
	}
    
	ImageReader* Png::CreateReader(Stream* input)
	{
		return new UIImageReader(input);
	}
    
	Bitmap* Png::Load(Stream* input)
	{
		Managed<ImageReader> r = CreateReader(input);
		return r->ReadBitmap();
	}
	Bitmap* Png::Load(const String& fileName)
	{
		Managed<File> f = new File(fileName, FileModeRead);
		return Load(f);
	}
}
