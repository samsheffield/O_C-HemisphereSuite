// Hemisphere Applet Boilerplate. Follow these steps to add a Hemisphere app:
//
// (1) Save this file as HEM_GateTest.ino
// (2) Find and replace "GateTest" with the name of your Applet class
// (3) Implement all of the public methods below
// (4) Add text to the help section below in SetHelp()
// (5) Add a declare line in hemisphere_config.h, which looks like this:
// (6) Increment HEMISPHERE_AVAILABLE_APPLETS in hemisphere_config.h
// (7) Add your name and any additional copyright info to the block below

// Copyright (c) 2018, __________
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

#include "bjorklund.h"

class GateTest : public HemisphereApplet {
public:

    const char* applet_name() {
        return "GateTest";
    }

    void Start() {
        ForEachChannel(ch)
        {
            length[ch] = 16;
            beats[ch] = 4 + (ch * 4);
            pattern[ch] = EuclideanPattern(length[ch] - 1, beats[ch], 0);;
        }
        step = 0;
    }

    void Controller() {
        // Advance both rings
        if (Clock(0)) {
            ForEachChannel(ch)
            {
                int rotation = Proportion(DetentedIn(ch), HEMISPHERE_MAX_CV, length[ch]);

                // Store the pattern for display
                pattern[ch] = EuclideanPattern(length[ch] - 1, beats[ch], rotation);
                int sb = step % length[ch];
                if ((pattern[ch] >> sb) & 0x01) {
                    ClockOut(ch);
                }
            }

            // Plan for the thing to run forever and ever
            if (++step >= length[0] * length[1]) step = 0;
        }
    }

    void View() {
        gfxHeader(applet_name());
        DrawInterface();
    }

    void OnButtonPress() {
    }

    void OnEncoderMove(int direction) {
    }
        
    uint32_t OnDataRequest() {
        uint32_t data = 0;
        // example: pack property_name at bit 0, with size of 8 bits
        // Pack(data, PackLocation {0,8}, property_name); 
        return data;
    }

    void OnDataReceive(uint32_t data) {
        // example: unpack value at bit 0 with size of 8 bits to property_name
        // property_name = Unpack(data, PackLocation {0,8}); 
    }

protected:
    void SetHelp() {
        //                               "------------------" <-- Size Guide
        help[HEMISPHERE_HELP_DIGITALS] = "Digital in help";
        help[HEMISPHERE_HELP_CVS]      = "CV in help";
        help[HEMISPHERE_HELP_OUTS]     = "Out help";
        help[HEMISPHERE_HELP_ENCODER]  = "123456789012345678";
        //                               "------------------" <-- Size Guide
    }
    
private:
    int step;
    int cursor = 0; // Ch1: 0=Length, 1=Hits; Ch2: 2=Length 3=Hits
    uint32_t pattern[4];
    
    // Settings
    int length[4];
    int beats[4];

    void DrawInterface() {

    }
};


////////////////////////////////////////////////////////////////////////////////
//// Hemisphere Applet Functions
///
///  Once you run the find-and-replace to make these refer to GateTest,
///  it's usually not necessary to do anything with these functions. You
///  should prefer to handle things in the HemisphereApplet child class
///  above.
////////////////////////////////////////////////////////////////////////////////
GateTest GateTest_instance[2];

void GateTest_Start(bool hemisphere) {GateTest_instance[hemisphere].BaseStart(hemisphere);}
void GateTest_Controller(bool hemisphere, bool forwarding) {GateTest_instance[hemisphere].BaseController(forwarding);}
void GateTest_View(bool hemisphere) {GateTest_instance[hemisphere].BaseView();}
void GateTest_OnButtonPress(bool hemisphere) {GateTest_instance[hemisphere].OnButtonPress();}
void GateTest_OnEncoderMove(bool hemisphere, int direction) {GateTest_instance[hemisphere].OnEncoderMove(direction);}
void GateTest_ToggleHelpScreen(bool hemisphere) {GateTest_instance[hemisphere].HelpScreen();}
uint32_t GateTest_OnDataRequest(bool hemisphere) {return GateTest_instance[hemisphere].OnDataRequest();}
void GateTest_OnDataReceive(bool hemisphere, uint32_t data) {GateTest_instance[hemisphere].OnDataReceive(data);}
