// Hemisphere Applet Boilerplate. Follow these steps to add a Hemisphere app:
//
// (1) Save this file as HEM_Nose.ino
// (2) Find and replace "Nose" with the name of your Applet class
// (3) Implement all of the public methods below
// (4) Add text to the help section below in SetHelp()
// (5) Add a declare line in hemisphere_config.h, which looks like this:
//
// (6) Increment HEMISPHERE_AVAILABLE_APPLETS in hemisphere_config.h
// (7) Add your name and any additional copyright info to the block below
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

// TODO:
// Gate muting

class Nose : public HemisphereApplet {
public:

    const char* applet_name() {
        return "Nose";
    }

    void Start() {
    }

    void Controller() {
        Out(0, GenerateNoise());
        Out(1, GenerateCVNoise(1));

        noiseSamples[sampleCount % 8] = ViewOut(0);
        noiseCVSamples[sampleCount % 8] = ViewOut(1);
        sampleCount++;
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
        help[HEMISPHERE_HELP_DIGITALS] = "";
        help[HEMISPHERE_HELP_CVS]      = "2=Noise CV";
        help[HEMISPHERE_HELP_OUTS]     = "1=Noise 2=CV";
        help[HEMISPHERE_HELP_ENCODER]  = "";
        //                               "------------------" <-- Size Guide
    }
    
private:
    int cursor;
    int noiseSamples[8];
    int noiseCVSamples[8];
    int sampleCount;
    
    void DrawInterface() {
        // Static noise visualization
        for (int i = 0; i < 8; i++)
        {
            gfxFrame((4 * i), map(noiseSamples[i], -HEMISPHERE_3V_CV, HEMISPHERE_3V_CV, 63, 10), 2, 2);
            gfxFrame(32 + (4 * i), map(noiseCVSamples[i], 0, HEMISPHERE_3V_CV, 64, 10), 2, 2);
        }      
    }

    int GenerateNoise(){
        return random(0, (12 << 7) * 6) - ((12 << 7) * 3);
    }

    int GenerateCVNoise(int ch){
        return random(0, Proportion(In(ch), HEMISPHERE_MAX_CV, (12 << 7) * 3));
    }
};


////////////////////////////////////////////////////////////////////////////////
//// Hemisphere Applet Functions
///
///  Once you run the find-and-replace to make these refer to Nose,
///  it's usually not necessary to do anything with these functions. You
///  should prefer to handle things in the HemisphereApplet child class
///  above.
////////////////////////////////////////////////////////////////////////////////
Nose Nose_instance[2];

void Nose_Start(bool hemisphere) {Nose_instance[hemisphere].BaseStart(hemisphere);}
void Nose_Controller(bool hemisphere, bool forwarding) {Nose_instance[hemisphere].BaseController(forwarding);}
void Nose_View(bool hemisphere) {Nose_instance[hemisphere].BaseView();}
void Nose_OnButtonPress(bool hemisphere) {Nose_instance[hemisphere].OnButtonPress();}
void Nose_OnEncoderMove(bool hemisphere, int direction) {Nose_instance[hemisphere].OnEncoderMove(direction);}
void Nose_ToggleHelpScreen(bool hemisphere) {Nose_instance[hemisphere].HelpScreen();}
uint32_t Nose_OnDataRequest(bool hemisphere) {return Nose_instance[hemisphere].OnDataRequest();}
void Nose_OnDataReceive(bool hemisphere, uint32_t data) {Nose_instance[hemisphere].OnDataReceive(data);}
