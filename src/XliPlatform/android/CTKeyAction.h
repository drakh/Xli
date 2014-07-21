#ifndef __XLI_CTKEY_ANDROID_H__
#define __XLI_CTKEY_ANDROID_H__

#include "AKeyEvent.h"

namespace Xli
{
    namespace PlatformSpecific
    {
        class CTKeyAction : public WindowAction
        {
        public:
            Xli::Key KeyEvent;
            bool KeyDown;
            CTKeyAction(AKeyEvent keyEvent, bool keyDown) 
            { 
                this->KeyEvent = AndroidToXliKeyEvent(keyEvent);
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
    };
};

#endif
