#include "AStream.h"
#include "AXliJ.h"

namespace Xli
{
    bool AStream::midsCached = false;
    jmethodID AStream::closeReadMid = 0;
    jmethodID AStream::closeWriteMid = 0;
    jmethodID AStream::readByteMid = 0;
    jmethodID AStream::readBufferMid = 0;
    jmethodID AStream::resetMid = 0;
    jmethodID AStream::flushMid = 0;
    jmethodID AStream::writeBufferMid = 0;
    jmethodID AStream::writeBufferDetailedMid = 0;

    AStream::AStream()
    {
        this->javaStream = 0;
        this->canRead = false;
        this->canWrite = false;
        this->streamType = AStream::INVALID;
        this->atEnd = true;
        this->closed = true;
    }

    AStream::AStream(AStreamType streamType, jobject javaStream)
    {
        this->javaStream = 0;
        this->canRead = false;
        this->canWrite = false;
        this->streamType = AStream::INVALID;
        this->atEnd = false;
        this->closed = false;

        Init(streamType, javaStream);
    }

    AStream::AStream(jobject javaStream)
    {
        this->javaStream = 0;
        this->canRead = false;
        this->canWrite = false;
        this->streamType = AStream::INVALID;
        this->atEnd = false;
        this->closed = false;

        JniHelper jni;
        jclass clsIS = jni->FindClass("java/io/InputStream");
        jclass clsOS = jni->FindClass("java/io/OutputStream");
        if (clsIS && clsOS)
        {
            if (jni->IsInstanceOf(javaStream, clsIS))
            {
                Init(AStream::READ, javaStream);
            } 
            else if (jni->IsInstanceOf(javaStream, clsOS)) 
            {
                Init(AStream::WRITE, javaStream);
            }
            else
            {
                LOGE("Object passed as argument isnt a type of java stream");
            }
        } 
        else
        {
            LOGE("Unable to get java classes to perform verification on stream");
        }
    }

    bool AStream::Init(AStreamType streamType, jobject javaStream)
    {
        JniHelper jni;
        bool result = false;
        switch (streamType)
        {
        case AStream::INVALID:
            LOGE("Invalid streamType");
            return false;
            break;
        case AStream::READ:
            this->javaStream = javaStream;
            jni->NewGlobalRef(this->javaStream);
            this->streamType = AStream::READ;
            canRead = true;
            break;
        case AStream::WRITE:
            this->javaStream = javaStream;
            jni->NewGlobalRef(this->javaStream);
            this->streamType = AStream::WRITE;
            canWrite = true;
            break;
        };
        if (!midsCached) return CacheMids();
        return true;
    }

    AStream::~AStream()
    {
        JniHelper jni;
        if (this->javaStream) jni->DeleteGlobalRef(this->javaStream);        
    }
    
    void AStream::Flush()
    {
        JniHelper jni;
        if (CanRead() && (streamType == AStream::WRITE))
        {
            jni->CallObjectMethod(javaStream, flushMid);
        }
        else
        {
            XLI_THROW_STREAM_CANT_WRITE;
        }
    }

    void AStream::Close()
    {
        JniHelper jni;
        if (IsClosed()) return;
        switch(streamType) 
        {
        case AStream::INVALID:
            XLI_THROW_STREAM_CLOSED;
            break;
        case AStream::READ:
            jni->CallObjectMethod(javaStream, closeReadMid);
            closed = true;
            break;
        case AStream::WRITE:
            jni->CallObjectMethod(javaStream, closeWriteMid);
            closed = true;
            break;
        };
    }

    bool AStream::IsClosed() const
    {
        return closed;
    }

    bool AStream::CanRead() const
    {
        return canRead;
    }

    bool AStream::CanWrite() const
    {
        return canWrite;
    }

    bool AStream::AtEnd() const
    {
        return this->atEnd;
    }

    int AStream::Read(void* dst, int elmSize, int elmCount)
    {
		if (!CanRead()) XLI_THROW_STREAM_CANT_READ;
		if (IsClosed()) XLI_THROW_STREAM_CLOSED;

        JniHelper jni;
        
        int fetchBytes = elmCount * elmSize;

        jbyteArray arr = jni->NewByteArray(fetchBytes);

        jobject jrecievedBytes = jni->CallObjectMethod(javaStream, readBufferMid, arr, 0, fetchBytes);
        
        int recievedBytes = (int)jrecievedBytes;
        if (jni->ExceptionCheck()) {
            LOGE("Index out of bounds for dst");
            return -1;
        }
        if (recievedBytes >= 0)
        {
             jni->GetByteArrayRegion(arr, 0, recievedBytes, (jbyte*)dst);
             jni->DeleteLocalRef(arr);
        } else {
            atEnd = true;
            jni->DeleteLocalRef(jrecievedBytes);
            jni->DeleteLocalRef(arr);
            return -1;
        }

        jni->DeleteLocalRef(jrecievedBytes);
        return (recievedBytes / elmSize);
    }

//[TODO] memoverflow?
    int AStream::Write(const void* data, int elmSize, int elmCount)
    {
		if (!CanWrite()) XLI_THROW_STREAM_CANT_WRITE;
		if (IsClosed()) XLI_THROW_STREAM_CLOSED;

        JniHelper jni;

        int pushBytes = elmCount * elmSize;
        jbyteArray arr = jni->NewByteArray(pushBytes);

        jni->SetByteArrayRegion(arr, 0, pushBytes, (jbyte *)data);
        jni->CallObjectMethod(javaStream, writeBufferDetailedMid, arr, 0, pushBytes);
        if (jni->ExceptionCheck()) {
            LOGE("Index out of bounds for data");
            return 0;
        }

        jni->GetByteArrayRegion(arr, 0, pushBytes, (jbyte*)data);
        jni->DeleteLocalRef(arr);

        return elmCount;
    }

    bool AStream::CacheMids()
    {
        JniHelper jni;
        
        jclass rcls = jni->FindClass("java/io/InputStream");
        if (rcls) 
        {
            closeReadMid = jni->GetMethodID(rcls, "close", "()V");
            readByteMid = jni->GetMethodID(rcls, "read", "()I");
            readBufferMid = jni->GetMethodID(rcls, "read", "([BII)I");
            resetMid = jni->GetMethodID(rcls, "reset", "()V");
            if (closeReadMid && readByteMid && readBufferMid && resetMid)
            {
                AStream::midsCached = 1;
            } else {
                LOGE("Some read mids were not found");
                return false;
            }
        }
        else
        {
            LOGE("Could not get java cls InputStream to cache mids");
            return false;
        }

        jclass wcls = jni->FindClass("java/io/OutputStream");
        if (wcls) 
        {
            closeWriteMid = jni->GetMethodID(wcls, "close", "()V");
            flushMid = jni->GetMethodID(wcls, "flush", "()V");
            writeBufferMid = jni->GetMethodID(wcls, "write", "([B)V");
            writeBufferDetailedMid = jni->GetMethodID(wcls, "write", "([BII)V");
            if (closeWriteMid && flushMid && writeBufferMid && writeBufferDetailedMid)
            {
                AStream::midsCached = 1;
            } else {
                LOGE("Some write mids were not found");
                return false;
            }
        }
        else
        {
            LOGE("Could not get java cls OutputStream to cache mids");
            return false;
        }
        return true;
    }
}
