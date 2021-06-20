// 
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
#define HEM_MINIPOP_MAX_LENGTH 16

class MiniPop : public HemisphereApplet {
public:

    const char* applet_name() {
        return "MiniPop";
    }

    void Start() {
        selected = 0;

        ForEachChannel(ch)
        {
            length[ch] = HEM_MINIPOP_MAX_LENGTH;
            beats[ch] = 4 + (ch * 4);
            pattern[ch] = EuclideanPattern(length[ch] - 1, beats[ch], 0);
        }
        step = 0;
    }

    void Controller() {
        
        // CV control over active notes from 0-5v
        if(DetentedIn(0) > 0)
        {
            beats[0] = ProportionCV(In(0), HEM_MINIPOP_MAX_LENGTH);
        }
        if(DetentedIn(1) > 0)
        {
            beats[1] =ProportionCV(In(1), HEM_MINIPOP_MAX_LENGTH);
        }

        // Generate patterns when clock on 1 is present
        if (Clock(0)) {
            ForEachChannel(ch)
            {
                pattern[ch] = EuclideanPattern(HEM_MINIPOP_MAX_LENGTH-1, beats[ch], rotation[ch]);
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
        gfxHeader(applet_name());
        DrawInterface();
    }

    void OnButtonPress() {
        // Cycle through settings
        if(selected < 3){
            selected++;
        }
        else{
            selected = 0;
        }
    }

    void OnEncoderMove(int direction) {

        // Settings: 1 active, 1 rotation, 2 action, 2 rotation
        switch (selected)
        {
        case 0:
            beats[0] += direction;
            beats[0] = constrain(beats[0], 1, HEM_MINIPOP_MAX_LENGTH);
            break;
        case 1:
            rotation[0] += direction;
            rotation[0] = constrain(rotation[0], 0, HEM_MINIPOP_MAX_LENGTH);
            break;
        case 2:
            beats[1] += direction;
            beats[1] = constrain(beats[1], 1, HEM_MINIPOP_MAX_LENGTH);
            break;
        case 3:
            rotation[1] += direction;
            rotation[1] = constrain(rotation[1], 0, HEM_MINIPOP_MAX_LENGTH);
            break;
        default:
            break;
        }            
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
        help[HEMISPHERE_HELP_DIGITALS] = "1=Clock";
        help[HEMISPHERE_HELP_CVS]      = "1/2=CV In Active";
        help[HEMISPHERE_HELP_OUTS]     = "A/B=Patterns Out";
        help[HEMISPHERE_HELP_ENCODER]  = "Btn=Next Enc=Set";
        //                               "------------------" <-- Size Guide
    }
    
private:
    int step;
    int selected;
    uint32_t pattern[2];
    
    // Settings
    int length[2];
    int beats[2];
    int rotation[2];

    // Used to swich UI modes
    bool active[2];
    
    // Keep track of current channel for settings
    // Wy does removing this crash things?
    uint32_t display_timeout[2];

    void DrawInterface() {
        //
        gfxPrint(2, 22, "A");
        gfxPrint(2, 34, "R");

        gfxPrint(14, 22, beats[0]);
        gfxPrint(14, 34, rotation[0]);

        gfxPrint(38, 22, "B");
        gfxPrint(38, 34, "R");

        gfxPrint(50, 22, beats[1]);
        gfxPrint(50, 34, rotation[1]);
        
        switch (selected)
        {
        case 0:
            gfxCursor(14, 30, 14);
            break;
        case 1:
            gfxCursor(14, 42, 14);
            break;
        case 2:
            gfxCursor(50, 30, 14);
            break;
        case 3:
            gfxCursor(46, 42, 14);
            break;
        default:
            break;
        }
    }
};


////////////////////////////////////////////////////////////////////////////////
//// Hemisphere Applet Functions
///
///  Once you run the find-and-replace to make these refer to MiniPop,
///  it's usually not necessary to do anything with these functions. You
///  should prefer to handle things in the HemisphereApplet child class
///  above.
////////////////////////////////////////////////////////////////////////////////
MiniPop MiniPop_instance[2];

void MiniPop_Start(bool hemisphere) {MiniPop_instance[hemisphere].BaseStart(hemisphere);}
void MiniPop_Controller(bool hemisphere, bool forwarding) {MiniPop_instance[hemisphere].BaseController(forwarding);}
void MiniPop_View(bool hemisphere) {MiniPop_instance[hemisphere].BaseView();}
void MiniPop_OnButtonPress(bool hemisphere) {MiniPop_instance[hemisphere].OnButtonPress();}
void MiniPop_OnEncoderMove(bool hemisphere, int direction) {MiniPop_instance[hemisphere].OnEncoderMove(direction);}
void MiniPop_ToggleHelpScreen(bool hemisphere) {MiniPop_instance[hemisphere].HelpScreen();}
uint32_t MiniPop_OnDataRequest(bool hemisphere) {return MiniPop_instance[hemisphere].OnDataRequest();}
void MiniPop_OnDataReceive(bool hemisphere, uint32_t data) {MiniPop_instance[hemisphere].OnDataReceive(data);}
