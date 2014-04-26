#ifndef __XLI_PLATFORM_SPECIFIC_IOS_H__
#define __XLI_PLATFORM_SPECIFIC_IOS_H__

#include <sys/sysctl.h>
#include <stdlib.h>
#include <Xli/String.h>
#include <Xli/Console.h>

namespace Xli
{
    namespace PlatformSpecific
    {
        /**
           \addtogroup XliCorePlatform
           @{
        */

        void iInit();

        /** @} */

        enum IDeviceType
        {
            IDeviceType_UNKNOWN = 0,
            IDeviceType_IPHONE_1G = 1,
            IDeviceType_IPHONE_3G = 2,
            IDeviceType_IPHONE_3GS = 3,
            IDeviceType_IPHONE_4 = 4,
            IDeviceType_VERIZON_IPHONE_4 = 5,
            IDeviceType_IPHONE_4S = 6,
            IDeviceType_IPHONE_5_GSM = 7,
            IDeviceType_IPHONE_5_GSM_CDMA = 8,
            IDeviceType_IPHONE_5C_GSM = 9,
            IDeviceType_IPHONE_5C_GSM_CDMA = 10,
            IDeviceType_IPHONE_5S_GSM = 11,
            IDeviceType_IPHONE_5S_GSM_CDMA = 12,
            IDeviceType_IPOD_TOUCH_1G = 13,
            IDeviceType_IPOD_TOUCH_2G = 14,
            IDeviceType_IPOD_TOUCH_3G = 15,
            IDeviceType_IPOD_TOUCH_4G = 16,
            IDeviceType_IPOD_TOUCH_5G = 17,
            IDeviceType_IPAD = 18,
            IDeviceType_IPAD_2_WIFI = 19,
            IDeviceType_IPAD_2_GSM = 20,
            IDeviceType_IPAD_2_CDMA = 21,
            IDeviceType_IPAD_MINI_WIFI = 22,
            IDeviceType_IPAD_MINI_GSM = 23,
            IDeviceType_IPAD_MINI_GSM_CDMA = 24,
            IDeviceType_IPAD_3_WIFI = 25,
            IDeviceType_IPAD_3_GSM_CDMA = 26,
            IDeviceType_IPAD_3_GSM = 27,
            IDeviceType_IPAD_4_WIFI = 28,
            IDeviceType_IPAD_4_GSM = 29,
            IDeviceType_IPAD_4_GSM_CDMA = 30,
            IDeviceType_IPAD_AIR_WIFI = 31,
            IDeviceType_IPAD_AIR_CELLULAR = 32,
            IDeviceType_IPAD_MINI_2G_WIFI = 33,
            IDeviceType_IPAD_MINI_2G_CELLULAR = 34,
            IDeviceType_SIMULATOR = 35
        };

        static IDeviceType GetIDeviceType()
        {
            size_t size;
            sysctlbyname("hw.machine", NULL, &size, NULL, 0);
            char* machine = (char*)malloc(size);
            sysctlbyname("hw.machine", machine, &size, NULL, 0);
            String platform(machine,size-1);
            free(machine);

            if (platform == "iPhone1,1") return IDeviceType_IPHONE_1G;
            if (platform == "iPhone1,2") return IDeviceType_IPHONE_3G;
            if (platform == "iPhone2,1") return IDeviceType_IPHONE_3GS;
            if (platform == "iPhone3,1") return IDeviceType_IPHONE_4;
            if (platform == "iPhone3,3") return IDeviceType_VERIZON_IPHONE_4;
            if (platform == "iPhone4,1") return IDeviceType_IPHONE_4S;
            if (platform == "iPhone5,1") return IDeviceType_IPHONE_5_GSM;
            if (platform == "iPhone5,2") return IDeviceType_IPHONE_5_GSM_CDMA;
            if (platform == "iPhone5,3") return IDeviceType_IPHONE_5C_GSM;
            if (platform == "iPhone5,4") return IDeviceType_IPHONE_5C_GSM_CDMA;
            if (platform == "iPhone6,1") return IDeviceType_IPHONE_5S_GSM;
            if (platform == "iPhone6,2") return IDeviceType_IPHONE_5S_GSM_CDMA;
            if (platform == "iPod1,1")   return IDeviceType_IPOD_TOUCH_1G;
            if (platform == "iPod2,1")   return IDeviceType_IPOD_TOUCH_2G;
            if (platform == "iPod3,1")   return IDeviceType_IPOD_TOUCH_3G;
            if (platform == "iPod4,1")   return IDeviceType_IPOD_TOUCH_4G;
            if (platform == "iPod5,1")   return IDeviceType_IPOD_TOUCH_5G;
            if (platform == "iPad1,1")   return IDeviceType_IPAD;
            if (platform == "iPad2,1")   return IDeviceType_IPAD_2_WIFI;
            if (platform == "iPad2,2")   return IDeviceType_IPAD_2_GSM;
            if (platform == "iPad2,3")   return IDeviceType_IPAD_2_CDMA;
            if (platform == "iPad2,4")   return IDeviceType_IPAD_2_WIFI;
            if (platform == "iPad2,5")   return IDeviceType_IPAD_MINI_WIFI;
            if (platform == "iPad2,6")   return IDeviceType_IPAD_MINI_GSM;
            if (platform == "iPad2,7")   return IDeviceType_IPAD_MINI_GSM_CDMA;
            if (platform == "iPad3,1")   return IDeviceType_IPAD_3_WIFI;
            if (platform == "iPad3,2")   return IDeviceType_IPAD_3_GSM_CDMA;
            if (platform == "iPad3,3")   return IDeviceType_IPAD_3_GSM;
            if (platform == "iPad3,4")   return IDeviceType_IPAD_4_WIFI;
            if (platform == "iPad3,5")   return IDeviceType_IPAD_4_GSM;
            if (platform == "iPad3,6")   return IDeviceType_IPAD_4_GSM_CDMA;
            if (platform == "iPad4,1")   return IDeviceType_IPAD_AIR_WIFI;
            if (platform == "iPad4,2")   return IDeviceType_IPAD_AIR_CELLULAR;
            if (platform == "iPad4,4")   return IDeviceType_IPAD_MINI_2G_WIFI;
            if (platform == "iPad4,5")   return IDeviceType_IPAD_MINI_2G_CELLULAR;
            if (platform == "i386")      return IDeviceType_SIMULATOR;
            if (platform == "x86_64")    return IDeviceType_SIMULATOR;
            return IDeviceType_UNKNOWN;
        }

        static float GetIDeviceDpi(IDeviceType type)
        {
            switch (type)
            {
            case IDeviceType_UNKNOWN:
                return 0.0f;
            case IDeviceType_IPHONE_1G:
                return 163.0f;
            case IDeviceType_IPHONE_3G:
                return 163.0f;
            case IDeviceType_IPHONE_3GS:
                return 163.0f;
            case IDeviceType_IPHONE_4:
                return 326.0f;
            case IDeviceType_VERIZON_IPHONE_4:
                return 163.0f;
            case IDeviceType_IPHONE_4S:
                return 326.0f;
            case IDeviceType_IPHONE_5_GSM:
                return 326.0f;
            case IDeviceType_IPHONE_5_GSM_CDMA:
                return 326.0f;
            case IDeviceType_IPHONE_5C_GSM:
                return 326.0f;
            case IDeviceType_IPHONE_5C_GSM_CDMA:
                return 326.0f;
            case IDeviceType_IPHONE_5S_GSM:
                return 326.0f;
            case IDeviceType_IPHONE_5S_GSM_CDMA:
                return 326.0f;
            case IDeviceType_IPOD_TOUCH_1G:
                return 163.0f;
            case IDeviceType_IPOD_TOUCH_2G:
                return 163.0f;
            case IDeviceType_IPOD_TOUCH_3G:
                return 163.0f;
            case IDeviceType_IPOD_TOUCH_4G:
                return 326.0f;
            case IDeviceType_IPOD_TOUCH_5G:
                return 326.0f;
            case IDeviceType_IPAD:
                return 132.0f;
            case IDeviceType_IPAD_2_WIFI:
                return 132.0f;
            case IDeviceType_IPAD_2_GSM:
                return 132.0f;
            case IDeviceType_IPAD_2_CDMA:
                return 132.0f;
            case IDeviceType_IPAD_MINI_WIFI:
                return 163.0f;
            case IDeviceType_IPAD_MINI_GSM:
                return 163.0f;
            case IDeviceType_IPAD_MINI_GSM_CDMA:
                return 163.0f;
            case IDeviceType_IPAD_3_WIFI:
                return 264.0f;
            case IDeviceType_IPAD_3_GSM_CDMA:
                return 264.0f;
            case IDeviceType_IPAD_3_GSM:
                return 264.0f;
            case IDeviceType_IPAD_4_WIFI:
                return 264.0f;
            case IDeviceType_IPAD_4_GSM:
                return 264.0f;
            case IDeviceType_IPAD_4_GSM_CDMA:
                return 264.0f;
            case IDeviceType_IPAD_AIR_WIFI:
                return 264.0f;
            case IDeviceType_IPAD_AIR_CELLULAR:
                return 264.0f;
            case IDeviceType_IPAD_MINI_2G_WIFI:
                return 326.0f;
            case IDeviceType_IPAD_MINI_2G_CELLULAR:
                return 326.0f;
            case IDeviceType_SIMULATOR:
                return 163.0f;
            }
            return 0.0f;
        }
    };
};

#endif
