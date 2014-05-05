#ifndef __XLI_ASTREAM_ANDROID_H__
#define __XLI_ASTREAM_ANDROID_H__

#include <Xli/Stream.h>
#include "AShim.h"

namespace Xli
{
    namespace PlatformSpecific
    {
        class AStream: public Stream
        {
        public:
            enum AStreamType 
            {
                INVALID = 0,
                READ = 1,
                WRITE = 2
            };

            static bool midsCached;
            static jmethodID closeReadMid;
            static jmethodID closeWriteMid;
            static jmethodID readByteMid;
            static jmethodID readBufferMid;
            static jmethodID resetMid;
            static jmethodID flushMid;
            static jmethodID writeBufferMid;
            static jmethodID writeBufferDetailedMid;
            static bool CacheMids();

            AStream();
            AStream(AStreamType streamType);
            AStream(AStreamType streamType, jobject javaStream);
            AStream(jobject javaStream);
            virtual ~AStream();
        
            virtual void Flush();
            virtual void Close();
            virtual bool IsClosed() const;
            virtual bool CanRead() const;
            virtual bool CanWrite() const;
            virtual bool AtEnd() const;
            virtual int Read(void* dst, int elmSize, int elmCount);
            virtual void Write(const void* dst, int elmSize, int elmCount);

        private:
            jobject javaStream;
            bool atEnd;
            bool closed;
            bool canRead;
            bool canWrite;
            AStreamType streamType;
            virtual bool Init(AStreamType streamType, jobject javaStream);
        };
    };
};

#endif
