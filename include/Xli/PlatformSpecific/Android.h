#ifndef __XLI_PLATFORM_SPECIFIC_ANDROID_H__
#define __XLI_PLATFORM_SPECIFIC_ANDROID_H__

#include <android/log.h>
#include <android/native_activity.h>
#include <cstdio>
#include <cstdlib>
#include <jni.h>

// TODO: Correct place for these?
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

// TODO: Get rid of these includes
#include <Xli/String.h>
#include <Xli/Stream.h>
#include <Xli/Window.h>
#include <XliSoundPlayer.h>
#include <XliHttpClient.h>

#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, Xli::PlatformSpecific::AGetAppName(), __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, Xli::PlatformSpecific::AGetAppName(), __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, Xli::PlatformSpecific::AGetAppName(), __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, Xli::PlatformSpecific::AGetAppName(), __VA_ARGS__))
#define LOGF(...) ((void)__android_log_print(ANDROID_LOG_FATAL, Xli::PlatformSpecific::AGetAppName(), __VA_ARGS__))

struct android_app;

namespace Xli
{
    namespace PlatformSpecific
    {
        // TODO: Too much stuff in this header

        /**
            \addtogroup XliCorePlatform
            @{
        */

        extern ANativeActivity* AndroidActivity;

        XLI_INLINE const char* AGetAppName()
        {
            const char* name = getenv("XLI_APP_NAME");
            return name ? name : "XliApp";
        }

        /** @} */

        /**
            \ingroup XliCorePlatform
        */
        class AJniHelper
        {
            JNIEnv* env;
            jclass shim;
            static int shim_loaded;

        public:
            static void Init();

            AJniHelper();

            jclass GetShim();

            jmethodID FindMethod(const char* className, const char* methodName, const char* methodSig);

            jobject CallObjectMethod(jobject inst, const char* name, const char* sig);

            String GetString(jobject str);
            JNIEnv* GetEnv();

            JNIEnv* operator->();

            jmethodID GetInstanceMethod(const char* m_name, const char* m_sig);
            jmethodID GetInstanceMethod(jobject inst, const char* m_name, const char* m_sig);
            jobject GetInstance();
            jobject GetInstance(const char* class_name, const char* constructor_sig, ...);
            jobject GetInstance(jclass cls, const char* constructor_sig, ...);
            jclass GetCustomClass(const char* class_path);
            int PrepareAssetJar(const char* file_name, const char* class_name);
            int PrepareAssetJar(const char* file_name, const char* class_name, int package);
            jclass GetAssetClass(const char* file_name, const char* class_name);
        };

        /**
            \ingroup XliCorePlatform
        */
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
            virtual int Write(const void* dst, int elmSize, int elmCount);

        private:
            jobject javaStream;
            bool atEnd;
            bool closed;
            bool canRead;
            bool canWrite;
            AStreamType streamType;
            virtual bool Init(AStreamType streamType, jobject javaStream);
        };

        /* this has moved */
        /* class CTError : public WindowAction */
        /* {         */
        /*     String message; */
        /*     CTError(String message) { this->message = message; } */
        /*     virtual void Execute() */
        /*     { */
        /*         LOGE("XLI: ", this->message->DataPtr()); */
        /*         XLI_THROW(this->message->DataPtr()); */
        /*     } */
        /* }; */

        /**
            \ingroup XliCorePlatform
        */
        enum AKeyEvent
        {
            ACTION_DOWN = 0,
            ACTION_MULTIPLE = 2,
            ACTION_UP = 1,
            FLAG_CANCELED = 32,
            FLAG_CANCELED_LONG_PRESS = 256,
            FLAG_EDITOR_ACTION = 16,
            FLAG_FALLBACK = 1024,
            FLAG_FROM_SYSTEM = 8,
            FLAG_KEEP_TOUCH_MODE = 4,
            FLAG_LONG_PRESS = 128,
            FLAG_SOFT_KEYBOARD = 2,
            FLAG_TRACKING = 512,
            FLAG_VIRTUAL_HARD_KEY = 64,
            FLAG_WOKE_HERE = 1,
            KEYCODE_0 = 7,
            KEYCODE_1 = 8,
            KEYCODE_2 = 9,
            KEYCODE_3 = 10,
            KEYCODE_3D_MODE = 206,
            KEYCODE_4 = 11,
            KEYCODE_5 = 12,
            KEYCODE_6 = 13,
            KEYCODE_7 = 14,
            KEYCODE_8 = 15,
            KEYCODE_9 = 16,
            KEYCODE_A = 29,
            KEYCODE_ALT_LEFT = 57,
            KEYCODE_ALT_RIGHT = 58,
            KEYCODE_APOSTROPHE = 75,
            KEYCODE_APP_SWITCH = 187,
            KEYCODE_ASSIST = 219,
            KEYCODE_AT = 77,
            KEYCODE_AVR_INPUT = 182,
            KEYCODE_AVR_POWER = 181,
            KEYCODE_B = 30,
            KEYCODE_BACK = 4,
            KEYCODE_BACKSLASH = 73,
            KEYCODE_BOOKMARK = 174,
            KEYCODE_BREAK = 121,
            KEYCODE_BRIGHTNESS_DOWN = 220,
            KEYCODE_BRIGHTNESS_UP = 221,
            KEYCODE_BUTTON_1 = 188,
            KEYCODE_BUTTON_10 = 197,
            KEYCODE_BUTTON_11 = 198,
            KEYCODE_BUTTON_12 = 199,
            KEYCODE_BUTTON_13 = 200,
            KEYCODE_BUTTON_14 = 201,
            KEYCODE_BUTTON_15 = 202,
            KEYCODE_BUTTON_16 = 203,
            KEYCODE_BUTTON_2 = 189,
            KEYCODE_BUTTON_3 = 190,
            KEYCODE_BUTTON_4 = 191,
            KEYCODE_BUTTON_5 = 192,
            KEYCODE_BUTTON_6 = 193,
            KEYCODE_BUTTON_7 = 194,
            KEYCODE_BUTTON_8 = 195,
            KEYCODE_BUTTON_9 = 196,
            KEYCODE_BUTTON_A = 96,
            KEYCODE_BUTTON_B = 97,
            KEYCODE_BUTTON_C = 98,
            KEYCODE_BUTTON_L1 = 102,
            KEYCODE_BUTTON_L2 = 104,
            KEYCODE_BUTTON_MODE = 110,
            KEYCODE_BUTTON_R1 = 103,
            KEYCODE_BUTTON_R2 = 105,
            KEYCODE_BUTTON_SELECT = 109,
            KEYCODE_BUTTON_START = 108,
            KEYCODE_BUTTON_THUMBL = 106,
            KEYCODE_BUTTON_THUMBR = 107,
            KEYCODE_BUTTON_X = 99,
            KEYCODE_BUTTON_Y = 100,
            KEYCODE_BUTTON_Z = 101,
            KEYCODE_C = 31,
            KEYCODE_CALCULATOR = 210,
            KEYCODE_CALENDAR = 208,
            KEYCODE_CALL = 5,
            KEYCODE_CAMERA = 27,
            KEYCODE_CAPS_LOCK = 115,
            KEYCODE_CAPTIONS = 175,
            KEYCODE_CHANNEL_DOWN = 167,
            KEYCODE_CHANNEL_UP = 166,
            KEYCODE_CLEAR = 28,
            KEYCODE_COMMA = 55,
            KEYCODE_CONTACTS = 207,
            KEYCODE_CTRL_LEFT = 113,
            KEYCODE_CTRL_RIGHT = 114,
            KEYCODE_D = 32,
            KEYCODE_DEL = 67,
            KEYCODE_DPAD_CENTER = 23,
            KEYCODE_DPAD_DOWN = 20,
            KEYCODE_DPAD_LEFT = 21,
            KEYCODE_DPAD_RIGHT = 22,
            KEYCODE_DPAD_UP = 19,
            KEYCODE_DVR = 173,
            KEYCODE_E = 33,
            KEYCODE_EISU = 212,
            KEYCODE_ENDCALL = 6,
            KEYCODE_ENTER = 66,
            KEYCODE_ENVELOPE = 65,
            KEYCODE_EQUALS = 70,
            KEYCODE_ESCAPE = 111,
            KEYCODE_EXPLORER = 64,
            KEYCODE_F = 34,
            KEYCODE_F1 = 131,
            KEYCODE_F10 = 140,
            KEYCODE_F11 = 141,
            KEYCODE_F12 = 142,
            KEYCODE_F2 = 132,
            KEYCODE_F3 = 133,
            KEYCODE_F4 = 134,
            KEYCODE_F5 = 135,
            KEYCODE_F6 = 136,
            KEYCODE_F7 = 137,
            KEYCODE_F8 = 138,
            KEYCODE_F9 = 139,
            KEYCODE_FOCUS = 80,
            KEYCODE_FORWARD = 125,
            KEYCODE_FORWARD_DEL = 112,
            KEYCODE_FUNCTION = 119,
            KEYCODE_G = 35,
            KEYCODE_GRAVE = 68,
            KEYCODE_GUIDE = 172,
            KEYCODE_H = 36,
            KEYCODE_HEADSETHOOK = 79,
            KEYCODE_HENKAN = 214,
            KEYCODE_HOME = 3,
            KEYCODE_I = 37,
            KEYCODE_INFO = 165,
            KEYCODE_INSERT = 124,
            KEYCODE_J = 38,
            KEYCODE_K = 39,
            KEYCODE_KANA = 218,
            KEYCODE_KATAKANA_HIRAGANA = 215,
            KEYCODE_L = 40,
            KEYCODE_LANGUAGE_SWITCH = 204,
            KEYCODE_LEFT_BRACKET = 71,
            KEYCODE_M = 41,
            KEYCODE_MANNER_MODE = 205,
            KEYCODE_MEDIA_CLOSE = 128,
            KEYCODE_MEDIA_EJECT = 129,
            KEYCODE_MEDIA_FAST_FORWARD = 90,
            KEYCODE_MEDIA_NEXT = 87,
            KEYCODE_MEDIA_PAUSE = 127,
            KEYCODE_MEDIA_PLAY = 126,
            KEYCODE_MEDIA_PLAY_PAUSE = 85,
            KEYCODE_MEDIA_PREVIOUS = 88,
            KEYCODE_MEDIA_RECORD = 130,
            KEYCODE_MEDIA_REWIND = 89,
            KEYCODE_MEDIA_STOP = 86,
            KEYCODE_MENU = 82,
            KEYCODE_META_LEFT = 117,
            KEYCODE_META_RIGHT = 118,
            KEYCODE_MINUS = 69,
            KEYCODE_MOVE_END = 123,
            KEYCODE_MOVE_HOME = 122,
            KEYCODE_MUHENKAN = 213,
            KEYCODE_MUSIC = 209,
            KEYCODE_MUTE = 91,
            KEYCODE_N = 42,
            KEYCODE_NOTIFICATION = 83,
            KEYCODE_NUM = 78,
            KEYCODE_NUMPAD_0 = 144,
            KEYCODE_NUMPAD_1 = 145,
            KEYCODE_NUMPAD_2 = 146,
            KEYCODE_NUMPAD_3 = 147,
            KEYCODE_NUMPAD_4 = 148,
            KEYCODE_NUMPAD_5 = 149,
            KEYCODE_NUMPAD_6 = 150,
            KEYCODE_NUMPAD_7 = 151,
            KEYCODE_NUMPAD_8 = 152,
            KEYCODE_NUMPAD_9 = 153,
            KEYCODE_NUMPAD_ADD = 157,
            KEYCODE_NUMPAD_COMMA = 159,
            KEYCODE_NUMPAD_DIVIDE = 154,
            KEYCODE_NUMPAD_DOT = 158,
            KEYCODE_NUMPAD_ENTER = 160,
            KEYCODE_NUMPAD_EQUALS = 161,
            KEYCODE_NUMPAD_LEFT_PAREN = 162,
            KEYCODE_NUMPAD_MULTIPLY = 155,
            KEYCODE_NUMPAD_RIGHT_PAREN = 163,
            KEYCODE_NUMPAD_SUBTRACT = 156,
            KEYCODE_NUM_LOCK = 143,
            KEYCODE_O = 43,
            KEYCODE_P = 44,
            KEYCODE_PAGE_DOWN = 93,
            KEYCODE_PAGE_UP = 92,
            KEYCODE_PERIOD = 56,
            KEYCODE_PICTSYMBOLS = 94,
            KEYCODE_PLUS = 81,
            KEYCODE_POUND = 18,
            KEYCODE_POWER = 26,
            KEYCODE_PROG_BLUE = 186,
            KEYCODE_PROG_GREEN = 184,
            KEYCODE_PROG_RED = 183,
            KEYCODE_PROG_YELLOW = 185,
            KEYCODE_Q = 45,
            KEYCODE_R = 46,
            KEYCODE_RIGHT_BRACKET = 72,
            KEYCODE_RO = 217,
            KEYCODE_S = 47,
            KEYCODE_SCROLL_LOCK = 116,
            KEYCODE_SEARCH = 84,
            KEYCODE_SEMICOLON = 74,
            KEYCODE_SETTINGS = 176,
            KEYCODE_SHIFT_LEFT = 59,
            KEYCODE_SHIFT_RIGHT = 60,
            KEYCODE_SLASH = 76,
            KEYCODE_SOFT_LEFT = 1,
            KEYCODE_SOFT_RIGHT = 2,
            KEYCODE_SPACE = 62,
            KEYCODE_STAR = 17,
            KEYCODE_STB_INPUT = 180,
            KEYCODE_STB_POWER = 179,
            KEYCODE_SWITCH_CHARSET = 95,
            KEYCODE_SYM = 63,
            KEYCODE_SYSRQ = 120,
            KEYCODE_T = 48,
            KEYCODE_TAB = 61,
            KEYCODE_TV = 170,
            KEYCODE_TV_INPUT = 178,
            KEYCODE_TV_POWER = 177,
            KEYCODE_U = 49,
            KEYCODE_UNKNOWN = 0,
            KEYCODE_V = 50,
            KEYCODE_VOLUME_DOWN = 25,
            KEYCODE_VOLUME_MUTE = 164,
            KEYCODE_VOLUME_UP = 24,
            KEYCODE_W = 51,
            KEYCODE_WINDOW = 171,
            KEYCODE_X = 52,
            KEYCODE_Y = 53,
            KEYCODE_YEN = 216,
            KEYCODE_Z = 54,
            KEYCODE_ZENKAKU_HANKAKU = 211,
            KEYCODE_ZOOM_IN = 168,
            KEYCODE_ZOOM_OUT = 169,
            MAX_KEYCODE = 84,
            META_ALT_LEFT_ON = 16,
            META_ALT_MASK = 50,
            META_ALT_ON = 2,
            META_ALT_RIGHT_ON = 32,
            META_CAPS_LOCK_ON = 1048576,
            META_CTRL_LEFT_ON = 8192,
            META_CTRL_MASK = 28672,
            META_CTRL_ON = 4096,
            META_CTRL_RIGHT_ON = 16384,
            META_FUNCTION_ON = 8,
            META_META_LEFT_ON = 131072,
            META_META_MASK = 458752,
            META_META_ON = 65536,
            META_META_RIGHT_ON = 262144,
            META_NUM_LOCK_ON = 2097152,
            META_SCROLL_LOCK_ON = 4194304,
            META_SHIFT_LEFT_ON = 64,
            META_SHIFT_MASK = 193,
            META_SHIFT_ON = 1,
            META_SHIFT_RIGHT_ON = 128,
            META_SYM_ON = 4
        };

        /**
            \ingroup XliCorePlatform
        */

        class AShim
        {
        private:
            static jmethodID makeNoise;
            static jmethodID raiseKeyboard;
            static jmethodID hideKeyboard;
            static jmethodID showMessageBox;
            static jmethodID connectedToNetwork;
            static jmethodID newHttpConnection;
            static jmethodID httpGetOutputStream;
            static jmethodID httpGetInputStream;
            static jmethodID httpShowHeaders;
            static jmethodID initDefaultCookieManager;
            static jmethodID getAssetManager;
            static jmethodID hideStatusBar;
            static jmethodID showStatusBar;        
        public:
            static int kbVisible;

            static void CacheMids(JNIEnv *env, jclass shim_class);
            static void MakeNoise();
            static void RaiseSoftKeyboard();
            static void HideSoftKeyboard();
            static bool KeyboardVisible();
            static void ShowStatusBar();
            static void HideStatusBar();
            static int ShowMessageBox(const String& message, const String& caption, int buttons, int hints);            
            static bool ConnectedToNetwork();
            static jobject XliToJavaHeaders(const HttpRequest* req);
            static jobject SendHttpAsync(const HttpRequest* req, void* content, long byteLength);
            static jobject SendHttpAsync(const HttpRequest* req, String content);
            static jobject SendHttpAsync(const HttpRequest* req);
            static void AbortAsyncConnection(jobject connection);
            static jobject HttpNewConnection(const String& uri, const String& method, bool hasPayload);
            static String InputStreamToString(jobject bufferedInputStream);
            static jobject AsyncInputStreamToString(jobject bufferedInputStream, HttpRequest* request);
            static jobject AsyncInputStreamToByteArray(jobject bufferedInputStream, HttpRequest* request);
            static int ReadBytesFromInputStream(jobject bufferedInputStream, int bytesToRead, void* dst);
            static AAssetManager* GetAssetManager();
            static bool RegisterNativeFunctions(JNINativeMethod native_funcs[], int funcCount);
            static Key AndroidToXliKeyEvent(AKeyEvent keyEvent);
            static void HandleSpecialAndroidKeyEvents(AKeyEvent androidKeyCode);
            static void InitDefaultCookieManager();
        };
        class CTKeyAction : public WindowAction
        {
        public:
            Xli::Key KeyEvent;
            bool KeyDown;
            CTKeyAction(AKeyEvent keyEvent, bool keyDown) 
            { 
                this->KeyEvent = AShim::AndroidToXliKeyEvent(keyEvent);
                this->KeyDown = keyDown;
            }
            virtual void Execute();
        };
        class CTTextAction : public WindowAction
        {
        public:
            String Text;
            CTTextAction(String text) { this->Text = text; }
            virtual void Execute();
        };

        /**
            \ingroup XliAudio
        */        
        class SlesAudioEngine : public Object
        {
        public:
            SLObjectItf EngineObject;
            SLEngineItf EngineEngine;
            SLObjectItf OutputMixObject;
            SLObjectItf LedObject;
            SLLEDArrayItf LedArray;
        };
    }
}


#endif
