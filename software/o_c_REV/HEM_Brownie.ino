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



class Brownie : public HemisphereApplet {
public:

    const char* applet_name() {
        return "Brownie";
    }

    void Start() {
        // Semitone 128
        // Octave 1536 or 12<<7
        stepSize = 128;
    }

    void Controller() {
        if(Clock(0)){
            // step x, y
            stepX += random(-1,2);
            stepY += random(-1,2);

            // limit cv x, y
            cvPosition[0] = constrain(stepX * stepSize, -HEMISPHERE_3V_CV, HEMISPHERE_MAX_CV);
            cvPosition[1] = constrain(stepY * stepSize, -HEMISPHERE_3V_CV, HEMISPHERE_MAX_CV);

            // rescale steps to screen x, y
            screenPosition[0] = constrain(32 + stepX, 0, 64);
            screenPosition[1] = constrain(39 + stepY, 14, 64);

            // Output
            Out(0, cvPosition[0]);
            Out(1, cvPosition[1]);  
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
        help[HEMISPHERE_HELP_DIGITALS] = "A/C:Clock";
        help[HEMISPHERE_HELP_CVS]      = "CV in help";
        help[HEMISPHERE_HELP_OUTS]     = "A/C:X B/D:Y";
        help[HEMISPHERE_HELP_ENCODER]  = "123456789012345678";
        //                               "------------------" <-- Size Guide
    }
    
private:
    int cursor;
    int stepX, stepY;
    int stepSize;
    int cvPosition[2];
    int screenPosition[2];
    int startCV;

    void DrawInterface() {
        gfxPrint(1, TopAlign(2),"X ");
        gfxPrint(screenPosition[0]);
        gfxPrint(1, TopAlign(22),"Y ");
        gfxPrint(screenPosition[1]);
        gfxPixel(screenPosition[0], screenPosition[1]);

    }

    int ProportionMidCV(int cv_value, int max_pixels) {
        int prop = constrain(Proportion(cv_value, HEMISPHERE_MAX_CV, max_pixels), 0, max_pixels)-max_pixels/2;
        return prop;
    }
};


////////////////////////////////////////////////////////////////////////////////
//// Hemisphere Applet Functions
///
///  Once you run the find-and-replace to make these refer to Brownie,
///  it's usually not necessary to do anything with these functions. You
///  should prefer to handle things in the HemisphereApplet child class
///  above.
////////////////////////////////////////////////////////////////////////////////
Brownie Brownie_instance[2];

void Brownie_Start(bool hemisphere) {Brownie_instance[hemisphere].BaseStart(hemisphere);}
void Brownie_Controller(bool hemisphere, bool forwarding) {Brownie_instance[hemisphere].BaseController(forwarding);}
void Brownie_View(bool hemisphere) {Brownie_instance[hemisphere].BaseView();}
void Brownie_OnButtonPress(bool hemisphere) {Brownie_instance[hemisphere].OnButtonPress();}
void Brownie_OnEncoderMove(bool hemisphere, int direction) {Brownie_instance[hemisphere].OnEncoderMove(direction);}
void Brownie_ToggleHelpScreen(bool hemisphere) {Brownie_instance[hemisphere].HelpScreen();}
uint32_t Brownie_OnDataRequest(bool hemisphere) {return Brownie_instance[hemisphere].OnDataRequest();}
void Brownie_OnDataReceive(bool hemisphere, uint32_t data) {Brownie_instance[hemisphere].OnDataReceive(data);}
