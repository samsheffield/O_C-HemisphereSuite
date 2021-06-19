// Hemisphere Applet Boilerplate. Follow these steps to add a Hemisphere app:
//
// (1) Save this file as HEM_Curse.ino
// (2) Find and replace "Curse" with the name of your Applet class
// (3) Implement all of the public methods below
// (4) Add text to the help section below in SetHelp()
// (5) Add a declare line in hemisphere_config.h, which looks like this:
//     
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
#define LOFI_PCM2CV(S) ((uint32_t)S << 8) - 32767;

class Curse : public HemisphereApplet {
public:

    const char* applet_name() {
        return "Curse";
    }

    void Start() {
        count = 8;
    }

    void Controller() {
        /*if(count == 0){
        for(int i = 0; i < 64; i ++) { // set up a loop    
            uint32_t z = LOFI_PCM2CV(pcm[i])
            pcm[i] = pcm[i] + pcm[i - 1]; //removes noise and some delay
            //Out(0, z);
            uint32_t s = (In(0) + 32767) >> 8;
            pcm[i] = (char)s;
        }
        count = 5;
        }
        else{
            count--;
        }*/

        if (count == 0){
            c /= 8;
            Out(0, c);
            count = 8;
            c = 0;
        }
        else{
            c += In(0) - (12 << 7);
            count--;
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
    char pcm[64];
    int count;
    int c;
    void DrawInterface() {
        gfxSkyline();
    }
};


////////////////////////////////////////////////////////////////////////////////
//// Hemisphere Applet Functions
///
///  Once you run the find-and-replace to make these refer to Curse,
///  it's usually not necessary to do anything with these functions. You
///  should prefer to handle things in the HemisphereApplet child class
///  above.
////////////////////////////////////////////////////////////////////////////////
Curse Curse_instance[2];

void Curse_Start(bool hemisphere) {Curse_instance[hemisphere].BaseStart(hemisphere);}
void Curse_Controller(bool hemisphere, bool forwarding) {Curse_instance[hemisphere].BaseController(forwarding);}
void Curse_View(bool hemisphere) {Curse_instance[hemisphere].BaseView();}
void Curse_OnButtonPress(bool hemisphere) {Curse_instance[hemisphere].OnButtonPress();}
void Curse_OnEncoderMove(bool hemisphere, int direction) {Curse_instance[hemisphere].OnEncoderMove(direction);}
void Curse_ToggleHelpScreen(bool hemisphere) {Curse_instance[hemisphere].HelpScreen();}
uint32_t Curse_OnDataRequest(bool hemisphere) {return Curse_instance[hemisphere].OnDataRequest();}
void Curse_OnDataReceive(bool hemisphere, uint32_t data) {Curse_instance[hemisphere].OnDataReceive(data);}
