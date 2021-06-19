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

// Left encoder:
// Rotate - set beats for active channel
// Press - switch channel

// Right encoder:
// Rotate - set rotation for active channel
// Press - switch channel

// CV In - override rotation? beats? -0v - beats +0v rotation?

// Something with the buttons...

#include "HSApplication.h"
#include "HSMIDI.h"
#include "bjorklund.h"

#define DISPLAY_TIMEOUT 60

class Popcorn : public HSApplication, public SystemExclusiveHandler {
public:
	void Start() {
        for (int8_t i = 0; i < 4; i++)
        {
            display_timeout[i] = DISPLAY_TIMEOUT;
        }
        

        currentChannel = 0;

        ForEachChannelAll(ch)
        {
            length[ch] = 16;
            beats[ch] = 4 + (ch * 2); // shortened
            pattern[ch] = EuclideanPattern(length[ch] - 1, beats[ch], 0);
        }
        step = 0;
	}
	
	void Resume() {
	}

    void Controller() {
        if (Clock(0)) {
            ForEachChannelAll(ch)
            {
                pattern[ch] = EuclideanPattern(length[ch] - 1, beats[ch], rotation[ch]);
                int sb = step % length[ch];
                if ((pattern[ch] >> sb) & 0x01) {
                    ClockOut(ch);
                }
            }

            // Keeps this going forever?
            if (++step >= length[0] * length[1] * length[2] * length[3]) step = 0;
        }
    }

    void View() {
        gfxHeader("Popcorn");
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
        // Toggle between channel selection and settings per channel
        active[currentChannel] = !active[currentChannel];
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
        // If the channel isn't active it can be switched
        // Once it's active, set the number of beats for it
        if(!active[currentChannel]){
            currentChannel += direction;

            if(currentChannel < 0){
                currentChannel = 3;
            }
            else if(currentChannel > 3){
                currentChannel = 0;
            }
        }
        else{
            beats[currentChannel] += direction;

            if(beats[currentChannel] < 0){
                beats[currentChannel] = 0;
            }
            else if(beats[currentChannel] > length[currentChannel]-1){
                beats[currentChannel] = length[currentChannel];
            }
        }        
    }

    void OnRightEncoderMove(int direction) {
        // If the channel is active, set the rotation for it
        if(active[currentChannel]){
            rotation[currentChannel] += direction;

            if(rotation[currentChannel] < 0){
                rotation[currentChannel] = 0;
            }
            else if(rotation[currentChannel] > length[currentChannel]-1){
                rotation[currentChannel] = length[currentChannel];
            }
        }  
    }

private:
    int step;
    uint32_t pattern[4];
    
    // Settings
    int length[4];
    int beats[4];
    int rotation[4];

    // Used to swich UI modes
    bool active[4];
    
    // Keep track of current channel for settings
    int8_t currentChannel;
    uint32_t display_timeout[4];

    void DrawInterface() {

        //
        ForEachChannelAll(ch){

            // TODO: Only show if enabled
            if(ch == currentChannel){
                //gfxRect ((ch * 32) + constrain(beats[ch]-1, 0, 16) * 2, BottomAlign(12), 2, 2);
                gfxLine (ch * 32, BottomAlign(12), (ch * 32) + constrain(beats[ch], 0, 16) * 2, BottomAlign(12));
                gfxDottedLine (ch * 32, BottomAlign(6), (ch * 32) + constrain(rotation[ch], 0, 16) * 2, BottomAlign(6));
                //gfxRect ((ch * 32) + constrain(rotation[ch]-1, 0, 16) * 2, BottomAlign(6), 2, 2);
            }
            
            //gfxDottedLine (ch * 32, BottomAlign(12), (ch * 32) + 30, BottomAlign(12));
            //gfxDottedLine (ch * 32, BottomAlign(6), (ch * 32) + 30, BottomAlign(6));

            // Visualization
            if(ViewOut(ch) > 0){   
                display_timeout[ch] = DISPLAY_TIMEOUT;
            }

            if (display_timeout[ch] > 0){
                --display_timeout[ch];
                gfxRect(9 + (ch * 32), 32, 10, 10);
            } 
            else{
                gfxFrame(11 + (ch * 32), 34, 6, 6);
            }
            
            // UI framing of selected channel
            if(ch == currentChannel){
               // gfxRect(11 + (ch * 32), BottomAlign(0), 6, 2);
                gfxDottedLine (11 + ch * 32, BottomAlign(0), 11 + (ch * 32) + 6, BottomAlign(0));
                if(active[currentChannel]) gfxRect(11 + (ch * 32), BottomAlign(0), 6, 2);
            } 
        }
    }
};

Popcorn Popcorn_instance;

// App stubs
void Popcorn_init() {
    Popcorn_instance.BaseStart();
}

// Not using O_C Storage
size_t Popcorn_storageSize() {return 0;}
size_t Popcorn_save(void *storage) {return 0;}
size_t Popcorn_restore(const void *storage) {return 0;}

void Popcorn_isr() {
	return Popcorn_instance.BaseController();
}

void Popcorn_handleAppEvent(OC::AppEvent event) {
    if (event ==  OC::APP_EVENT_RESUME) {
        Popcorn_instance.Resume();
    }
    if (event == OC::APP_EVENT_SUSPEND) {
        Popcorn_instance.OnSendSysEx();
    }
}

void Popcorn_loop() {} // Deprecated

void Popcorn_menu() {
    Popcorn_instance.BaseView();
}

void Popcorn_screensaver() {} // Deprecated

void Popcorn_handleButtonEvent(const UI::Event &event) {
    // For left encoder, handle press and long press
    if (event.control == OC::CONTROL_BUTTON_L) {
        if (event.type == UI::EVENT_BUTTON_LONG_PRESS) Popcorn_instance.OnLeftButtonLongPress();
        else Popcorn_instance.OnLeftButtonPress();
    }

    // For right encoder, only handle press (long press is reserved)
    if (event.control == OC::CONTROL_BUTTON_R && event.type == UI::EVENT_BUTTON_PRESS) Popcorn_instance.OnRightButtonPress();

    // For up button, handle only press (long press is reserved)
    if (event.control == OC::CONTROL_BUTTON_UP) Popcorn_instance.OnUpButtonPress();

    // For down button, handle press and long press
    if (event.control == OC::CONTROL_BUTTON_DOWN) {
        if (event.type == UI::EVENT_BUTTON_PRESS) Popcorn_instance.OnDownButtonPress();
        if (event.type == UI::EVENT_BUTTON_LONG_PRESS) Popcorn_instance.OnDownButtonLongPress();
    }
}

void Popcorn_handleEncoderEvent(const UI::Event &event) {
    // Left encoder turned
    if (event.control == OC::CONTROL_ENCODER_L) Popcorn_instance.OnLeftEncoderMove(event.value);

    // Right encoder turned
    if (event.control == OC::CONTROL_ENCODER_R) Popcorn_instance.OnRightEncoderMove(event.value);
}
