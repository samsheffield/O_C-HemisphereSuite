// Hemisphere Applet Boilerplate. Follow these steps to add a Hemisphere app:
//
// (1) Save this file as HEM_LearnClock.ino
// (2) Find and replace "LearnClock" with the name of your Applet class
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

class LearnClock : public HemisphereApplet {
public:

    const char* applet_name() {
        return "Learn Clock";
    }

    void Start() {

    }

    void Controller() {
        ForEachChannel(ch){
            if(Clock(ch)){
                ClockOut(ch);
            }
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
    int cursor;
    
    void DrawInterface() {
        gfxSkyline();
    }
};


////////////////////////////////////////////////////////////////////////////////
//// Hemisphere Applet Functions
///
///  Once you run the find-and-replace to make these refer to LearnClock,
///  it's usually not necessary to do anything with these functions. You
///  should prefer to handle things in the HemisphereApplet child class
///  above.
////////////////////////////////////////////////////////////////////////////////
LearnClock LearnClock_instance[2];

void LearnClock_Start(bool hemisphere) {LearnClock_instance[hemisphere].BaseStart(hemisphere);}
void LearnClock_Controller(bool hemisphere, bool forwarding) {LearnClock_instance[hemisphere].BaseController(forwarding);}
void LearnClock_View(bool hemisphere) {LearnClock_instance[hemisphere].BaseView();}
void LearnClock_OnButtonPress(bool hemisphere) {LearnClock_instance[hemisphere].OnButtonPress();}
void LearnClock_OnEncoderMove(bool hemisphere, int direction) {LearnClock_instance[hemisphere].OnEncoderMove(direction);}
void LearnClock_ToggleHelpScreen(bool hemisphere) {LearnClock_instance[hemisphere].HelpScreen();}
uint32_t LearnClock_OnDataRequest(bool hemisphere) {return LearnClock_instance[hemisphere].OnDataRequest();}
void LearnClock_OnDataReceive(bool hemisphere, uint32_t data) {LearnClock_instance[hemisphere].OnDataReceive(data);}
