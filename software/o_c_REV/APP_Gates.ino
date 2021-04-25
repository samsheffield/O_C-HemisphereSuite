// Uses code from Annular Fusion, Copyright (c) 2018, Jason Justian
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "HSApplication.h"
#include "HSMIDI.h"
#include "bjorklund.h"

class Gates : public HSApplication, public SystemExclusiveHandler {
public:
	void Start() {
        ForEachChannelAll(ch)
        {
            length[ch] = 16;
            beats[ch] = 4 + (ch * 4);
            pattern[ch] = EuclideanPattern(length[ch] - 1, beats[ch], 0);;
        }
        step = 0;
	}
	
	void Resume() {
	}

    void Controller() {

        if (Clock(0)) {
            ForEachChannelAll(ch)
            {
                int rotation = Proportion(DetentedIn(ch), HSAPPLICATION_5V, length[ch]);

                pattern[ch] = EuclideanPattern(length[ch] - 1, beats[ch], rotation);
                int sb = step % length[ch];
                if ((pattern[ch] >> sb) & 0x01) {
                    ClockOut(ch);
                }
            }

            if (++step >= length[0] * length[1] * length[2] * length[3]) step = 0;
        }
    }

    void View() {
        gfxHeader("Gates");
        //DrawInterface();
    }

    void OnSendSysEx() {
    }

    void OnReceiveSysEx() {
    }

    /////////////////////////////////////////////////////////////////
    // Control handlers
    /////////////////////////////////////////////////////////////////
    void OnLeftButtonPress() {
    }

    void OnLeftButtonLongPress() {
    }

    void OnRightButtonPress() {
    }

    void OnUpButtonPress() {
    }

    void OnDownButtonPress() {
    }

    void OnDownButtonLongPress() {
    }

    void OnLeftEncoderMove(int direction) {
    }

    void OnRightEncoderMove(int direction) {
    }

private:
    int8_t cursor;
    int step;
    uint32_t pattern[4];
    
    // Settings
    int length[4];
    int beats[4];
    /* Example private screen-drawing method
    void DrawInterface() {
    }
    */
};

Gates Gates_instance;

// App stubs
void Gates_init() {
    Gates_instance.BaseStart();
}

// Not using O_C Storage
size_t Gates_storageSize() {return 0;}
size_t Gates_save(void *storage) {return 0;}
size_t Gates_restore(const void *storage) {return 0;}

void Gates_isr() {
	return Gates_instance.BaseController();
}

void Gates_handleAppEvent(OC::AppEvent event) {
    if (event ==  OC::APP_EVENT_RESUME) {
        Gates_instance.Resume();
    }
    if (event == OC::APP_EVENT_SUSPEND) {
        Gates_instance.OnSendSysEx();
    }
}

void Gates_loop() {} // Deprecated

void Gates_menu() {
    Gates_instance.BaseView();
}

void Gates_screensaver() {} // Deprecated

void Gates_handleButtonEvent(const UI::Event &event) {
    // For left encoder, handle press and long press
    if (event.control == OC::CONTROL_BUTTON_L) {
        if (event.type == UI::EVENT_BUTTON_LONG_PRESS) Gates_instance.OnLeftButtonLongPress();
        else Gates_instance.OnLeftButtonPress();
    }

    // For right encoder, only handle press (long press is reserved)
    if (event.control == OC::CONTROL_BUTTON_R && event.type == UI::EVENT_BUTTON_PRESS) Gates_instance.OnRightButtonPress();

    // For up button, handle only press (long press is reserved)
    if (event.control == OC::CONTROL_BUTTON_UP) Gates_instance.OnUpButtonPress();

    // For down button, handle press and long press
    if (event.control == OC::CONTROL_BUTTON_DOWN) {
        if (event.type == UI::EVENT_BUTTON_PRESS) Gates_instance.OnDownButtonPress();
        if (event.type == UI::EVENT_BUTTON_LONG_PRESS) Gates_instance.OnDownButtonLongPress();
    }
}

void Gates_handleEncoderEvent(const UI::Event &event) {
    // Left encoder turned
    if (event.control == OC::CONTROL_ENCODER_L) Gates_instance.OnLeftEncoderMove(event.value);

    // Right encoder turned
    if (event.control == OC::CONTROL_ENCODER_R) Gates_instance.OnRightEncoderMove(event.value);
}
