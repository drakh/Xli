#ifndef __XLI_PLATFORM_SPECIFIC_IOS_H__
#define __XLI_PLATFORM_SPECIFIC_IOS_H__

#include <sys/sysctl.h>
#include <stdlib.h>
#include <Xli/String.h>

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
            IDeviceType_UNKNOWN,
            IDeviceType_IPHONE_1G,
            IDeviceType_IPHONE_3G,
            IDeviceType_IPHONE_3GS,
            IDeviceType_IPHONE_4,
            IDeviceType_VERIZON_IPHONE_4,
            IDeviceType_IPHONE_4S,
            IDeviceType_IPHONE_5_GSM,
            IDeviceType_IPHONE_5_GSM_CDMA,
            IDeviceType_IPHONE_5C_GSM,
            IDeviceType_IPHONE_5C_GSM_CDMA,
            IDeviceType_IPHONE_5S_GSM,
            IDeviceType_IPHONE_5S_GSM_CDMA,
            IDeviceType_IPOD_TOUCH_1G,
            IDeviceType_IPOD_TOUCH_2G,
            IDeviceType_IPOD_TOUCH_3G,
            IDeviceType_IPOD_TOUCH_4G,
            IDeviceType_IPOD_TOUCH_5G,
            IDeviceType_IPAD,
            IDeviceType_IPAD_2_WIFI,
            IDeviceType_IPAD_2_GSM,
            IDeviceType_IPAD_2_CDMA,
            IDeviceType_IPAD_MINI_WIFI,
            IDeviceType_IPAD_MINI_GSM,
            IDeviceType_IPAD_MINI_GSM_CDMA,
            IDeviceType_IPAD_3_WIFI,
            IDeviceType_IPAD_3_GSM_CDMA,
            IDeviceType_IPAD_3_GSM,
            IDeviceType_IPAD_4_WIFI,
            IDeviceType_IPAD_4_GSM,
            IDeviceType_IPAD_4_GSM_CDMA,
            IDeviceType_IPAD_AIR_WIFI,
            IDeviceType_IPAD_AIR_CELLULAR,
            IDeviceType_IPAD_MINI_2G_WIFI,
            IDeviceType_IPAD_MINI_2G_CELLULAR,
            IDeviceType_SIMULATOR
        };

        static IDeviceType GetIDeviceType()
        {
            size_t size;
            sysctlbyname("hw.machine", NULL, &size, NULL, 0);
            char* machine = (char*)malloc(size);
            sysctlbyname("hw.machine", machine, &size, NULL, 0);
            String platform(machine,size);
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

        static float GetIDeviceDensity(IDeviceType type)
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
