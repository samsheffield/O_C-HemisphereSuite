// 3 Way by Sam Sheffield, 2021
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

/*
    Channel 1: Controllers (clock, input cv selector)
    Channels 2,3,4: VCA input cv, output cv)
    Encoder 1: Toggle Gate CV Trigger outputs
    Not sure that this will be great for audio without downsampling occuring
    But CV should be totally fine

*/
#ifdef ENABLE_APP_THREESOME

#include "HSApplication.h"
#include "HSMIDI.h"

#define NUMBER_OF_MODES 3

class Threesome : public HSApplication, public SystemExclusiveHandler {
public:
	void Start() {
        selected = 0;
        defaultRectScale = 16;

        ForEachChannelAll(ch){
            Out(ch, 0);
        }
        // Why this way?
        const char * mode_name_list[] = {"CV", "GATE", "TRIG"};
        for(int i = 0; i < NUMBER_OF_MODES; i++)
        {
            mode_name[i] = mode_name_list[i];
        }
    }
	
	void Resume() {
	}

    void Controller() {
        // Divide 5V (7680) by 3 to select the output channels
        if (In(0) < 2560) {
            activeOut = 1;
        }else if (In(0) < 5120) {
            activeOut = 2;
        } else {
            activeOut = 3;
        }  

        // Mode select
        switch (selected)
        {
        case 0:
            // CV mode
            ForEachChannelAll(ch){
                Out(ch, ch == activeOut ? DetentedIn(activeOut) : 0);
            }
            rectScale = RectScaler(activeOut, defaultRectScale-5, 21);
            // Send selected CV through Out 0
            Out(0, DetentedIn(activeOut));
            break;
        case 1:
            ForEachChannelAll(ch){
                GateOut(ch, ch == activeOut ? true : false);
            }
            
            GateOut(0, false);
            // Gate mode
            break;
        case 2:
            // Trigger mode
            break;
        default:
            break;
        }

    }

    void View() {
        gfxHeader("Threesome");
        DrawInterface();
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
        selected += direction;
        if(selected > NUMBER_OF_MODES-1){
            selected = 0;
        }
        else if(selected < 0){
            selected = NUMBER_OF_MODES-1;
        }
    }

    void OnRightEncoderMove(int direction) {
    }

private:
    int8_t cursor;
    int selected;
    int mode;
    int activeOut;
    int rectScale;
    int defaultRectScale;
    const char* mode_name[NUMBER_OF_MODES];

    void DrawInterface() {
        gfxPrint(1, 15, mode_name[selected]);
        gfxPrint(1, 25, activeOut);

        // Graphic

        gfxPrint(20, 43, "B");
        gfxPrint(65, 23, "C");
        gfxPrint(102, 55, "D");
        

        // Set default frames
        if(activeOut != 1){
            gfxFrame(15, 38, defaultRectScale, defaultRectScale);
        }
        if (activeOut != 2){
            gfxFrame(60, 18, defaultRectScale, defaultRectScale);
        }
        if (activeOut != 3){
            gfxFrame(97, 50, defaultRectScale, defaultRectScale);
        }
        
        // Scale frames based on CV input
        switch (activeOut)
        {
        case 1:
            gfxFrame(15 + defaultRectScale/2 - (rectScale/2), 38 + defaultRectScale/2 - (rectScale/2), rectScale, rectScale);
            break;
        case 2:
            gfxFrame(60 + defaultRectScale/2 - (rectScale/2), 18  + defaultRectScale/2 - (rectScale/2), rectScale, rectScale);
            break;
        case 3:
            gfxFrame(97  + defaultRectScale/2 - (rectScale/2), 50  + defaultRectScale/2 - (rectScale/2), rectScale, rectScale);
            break;
        default:
            break;
        }
        
    }

    int RectScaler(int ch, int min, int max){
        return ProportionCV(In(ch), min, max);
    }
    
};

Threesome Threesome_instance;

// App stubs
void Threesome_init() {
    Threesome_instance.BaseStart();
}

// Not using O_C Storage
size_t Threesome_storageSize() {return 0;}
size_t Threesome_save(void *storage) {return 0;}
size_t Threesome_restore(const void *storage) {return 0;}

void Threesome_isr() {
	return Threesome_instance.BaseController();
}

void Threesome_handleAppEvent(OC::AppEvent event) {
    if (event ==  OC::APP_EVENT_RESUME) {
        Threesome_instance.Resume();
    }
    if (event == OC::APP_EVENT_SUSPEND) {
        Threesome_instance.OnSendSysEx();
    }
}

void Threesome_loop() {} // Deprecated

void Threesome_menu() {
    Threesome_instance.BaseView();
}

void Threesome_screensaver() {} // Deprecated

void Threesome_handleButtonEvent(const UI::Event &event) {
    // For left encoder, handle press and long press
    if (event.control == OC::CONTROL_BUTTON_L) {
        if (event.type == UI::EVENT_BUTTON_LONG_PRESS) Threesome_instance.OnLeftButtonLongPress();
        else Threesome_instance.OnLeftButtonPress();
    }

    // For right encoder, only handle press (long press is reserved)
    if (event.control == OC::CONTROL_BUTTON_R && event.type == UI::EVENT_BUTTON_PRESS) Threesome_instance.OnRightButtonPress();

    // For up button, handle only press (long press is reserved)
    if (event.control == OC::CONTROL_BUTTON_UP) Threesome_instance.OnUpButtonPress();

    // For down button, handle press and long press
    if (event.control == OC::CONTROL_BUTTON_DOWN) {
        if (event.type == UI::EVENT_BUTTON_PRESS) Threesome_instance.OnDownButtonPress();
        if (event.type == UI::EVENT_BUTTON_LONG_PRESS) Threesome_instance.OnDownButtonLongPress();
    }
}

void Threesome_handleEncoderEvent(const UI::Event &event) {
    // Left encoder turned
    if (event.control == OC::CONTROL_ENCODER_L) Threesome_instance.OnLeftEncoderMove(event.value);

    // Right encoder turned
    if (event.control == OC::CONTROL_ENCODER_R) Threesome_instance.OnRightEncoderMove(event.value);
}

#endif
