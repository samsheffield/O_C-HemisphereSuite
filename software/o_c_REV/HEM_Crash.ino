// Based on DrCrusher applet Copyright (c) 2018, Jason Justian
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

// Crash Hemisphere Applet
// A CV controlled bitcrusher and Crasher
// Audio signal in CV A/C
// Control signal (0~5v) in CV B/D
// Encoder controls bit depth (2-14 bit)
// Crushed audio out OUT A/C, Pass through audio on B/D
// Defeat crushed signal Gate A/D

class Crash : public HemisphereApplet {
public:

    const char* applet_name() {
        return "Crash";
    }

    void Start() {
        selected = 0;
        depth = 13;
    }

    void Controller() {
        
        if (!Gate(0)) {
            
            // 
            if(Gate(1)){
                selected = 0;
                depth = Proportion(In(1), HEMISPHERE_MAX_CV, 14);
                depth = constrain(depth, 1, 13);
                mask = get_mask();
            }
            else{
                selected = 1;
                // 48 is a magic number that I just thought was good as a limit
                crush = Proportion(In(1), HEMISPHERE_MAX_CV, 48);
                crush = constrain(crush, 1, 48);
            }

            if (++count >= crush) {
                count = 0;
                int cv = In(0);
                // Convert CV to positive, 16-bit
                int p_cv = cv + HEMISPHERE_MAX_CV;
                uint16_t p_cv16 = static_cast<uint16_t>(p_cv);
                p_cv16 = p_cv16 & mask;
                cv = static_cast<int>(p_cv16) - HEMISPHERE_MAX_CV;
                Out(0, cv);
            }
        } else {
            Out(0, In(0));
        } 

        Out(1, In(0));
    }

    void View() {
        gfxHeader(applet_name());
        DrawInterface();
    }

    void OnButtonPress() {
    }

    void OnEncoderMove(int direction) {    
        if(selected == 0){
            crush = constrain(crush + direction, 0, 48);
            // I feel like I should be packing, unpacking but I don't understand it so...
        }
        else {
            depth = constrain(depth + direction, 1, 13);
            mask = get_mask();
        }  
    }
        
    uint32_t OnDataRequest() {
        uint32_t data = 0;
        //Pack(data, PackLocation {3,4}, depth);
        return data;
    }

    void OnDataReceive(uint32_t data) {
        //depth = Unpack(data, PackLocation {3,4});
        //mask = get_mask();
    }

protected:
    void SetHelp() {
        //                               "------------------" <-- Size Guide
        help[HEMISPHERE_HELP_DIGITALS] = "1=Disable";
        help[HEMISPHERE_HELP_CVS]      = "1=Input 2=Crush CV";
        help[HEMISPHERE_HELP_OUTS]     = "A=Crush B=Thru";
        help[HEMISPHERE_HELP_ENCODER]  = "Bit Depth";
        //                               "------------------" <-- Size Guide
    }
    
private:
    int8_t depth;

    // which parameter is selected
    int8_t selected;
    int8_t cvSelector;
    int offset;

    // Moved here
    int crush = 1;

    // Housekeeping
    byte count = 0;
    uint16_t mask = 0xffff;
    
    void DrawInterface() {
        gfxPrint(1, 15, "CV");
        // CV graphic indicator
        int w = Proportion(crush, 48, 59);
        w = constrain(w, 1, 56);
        gfxRect(3, 27, w, 6);
        gfxFrame(1, 25, 60, 10);

        gfxPrint(1, 41, "Bit Depth");
        // Bit Depth graphic indicator
        int b = Proportion(depth, 13, 56);
        b = constrain(b, 1, 56);    
        gfxRect(3, 53, b, 6);
        gfxFrame(1, 51, 60, 10);

        // Move cusor based on active paameter
        if (selected == 0){
            gfxCursor(1, 23, 18);
        } 
        else {
            gfxCursor(1, 49, 60);
        }
    }

    uint16_t get_mask() {
        uint16_t mask = 0x0000;
        for (byte b = 0; b <= depth; b++)
        {
            mask = mask << 1;
            mask |= 0x01;
        }
        mask = mask << (13 - depth);
        mask |= 0xc000; // Turn on the high two bits
        return mask;
    }
};


////////////////////////////////////////////////////////////////////////////////
//// Hemisphere Applet Functions
///
///  Once you run the find-and-replace to make these refer to Crash,
///  it's usually not necessary to do anything with these functions. You
///  should prefer to handle things in the HemisphereApplet child class
///  above.
////////////////////////////////////////////////////////////////////////////////
Crash Crash_instance[2];

void Crash_Start(bool hemisphere) {Crash_instance[hemisphere].BaseStart(hemisphere);}
void Crash_Controller(bool hemisphere, bool forwarding) {Crash_instance[hemisphere].BaseController(forwarding);}
void Crash_View(bool hemisphere) {Crash_instance[hemisphere].BaseView();}
void Crash_OnButtonPress(bool hemisphere) {Crash_instance[hemisphere].OnButtonPress();}
void Crash_OnEncoderMove(bool hemisphere, int direction) {Crash_instance[hemisphere].OnEncoderMove(direction);}
void Crash_ToggleHelpScreen(bool hemisphere) {Crash_instance[hemisphere].HelpScreen();}
uint32_t Crash_OnDataRequest(bool hemisphere) {return Crash_instance[hemisphere].OnDataRequest();}
void Crash_OnDataReceive(bool hemisphere, uint32_t data) {Crash_instance[hemisphere].OnDataReceive(data);}
