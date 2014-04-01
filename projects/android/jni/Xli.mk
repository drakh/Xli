include $(CLEAR_VARS)
LOCAL_MODULE    := Xli

LOCAL_SRC_FILES := \
    ../../../src/core/3rdparty/ConvertUTF.c \
    ../../../src/core/android/AFileSystem.cpp \
    ../../../src/core/android/AJniHelper.cpp \
    ../../../src/core/android/AMessageBox.cpp \
    ../../../src/core/android/AShim.cpp \
    ../../../src/core/android/AStream.cpp \
    ../../../src/core/android/AWindow.cpp \
    ../../../src/core/generic/GenericDialogs.cpp \
    ../../../src/core/posix/PosixCond.cpp \
    ../../../src/core/posix/PosixFileSystemBase.cpp \
    ../../../src/core/posix/PosixMutex.cpp \
    ../../../src/core/posix/PosixTime.cpp \
    ../../../src/core/posix/PosixThread.cpp \
    ../../../src/core/Application.cpp \
    ../../../src/core/ArrayStream.cpp \
    ../../../src/core/BinaryReader.cpp \
    ../../../src/core/BinaryWriter.cpp \
    ../../../src/core/Buffer.cpp \
    ../../../src/core/BufferStream.cpp \
    ../../../src/core/Bundle.cpp \
    ../../../src/core/Console.cpp \
    ../../../src/core/DataType.cpp \
    ../../../src/core/DateTime.cpp \
    ../../../src/core/Disk.cpp \
    ../../../src/core/Exception.cpp \
    ../../../src/core/File.cpp \
    ../../../src/core/FileSystem.cpp \
    ../../../src/core/Matrix3.cpp \
    ../../../src/core/Matrix4.cpp \
    ../../../src/core/MessageBox.cpp \
    ../../../src/core/Object.cpp \
    ../../../src/core/Path.cpp \
    ../../../src/core/Quaternion.cpp \
    ../../../src/core/Random.cpp \
    ../../../src/core/Stream.cpp \
    ../../../src/core/String.cpp \
    ../../../src/core/StringBuilder.cpp \
    ../../../src/core/Text.cpp \
    ../../../src/core/TextReader.cpp \
    ../../../src/core/TextStream.cpp \
    ../../../src/core/TextWriter.cpp \
    ../../../src/core/Thread.cpp \
    ../../../src/core/Traits.cpp \
    ../../../src/core/Unicode.cpp \
    ../../../src/core/Utf16String.cpp \
    ../../../src/core/Window.cpp \

LOCAL_C_INCLUDES := \
    ../../include \

LOCAL_CFLAGS   := -fexceptions -fno-rtti
LOCAL_LDLIBS   := -lm -llog -landroid

include $(BUILD_SHARED_LIBRARY)
