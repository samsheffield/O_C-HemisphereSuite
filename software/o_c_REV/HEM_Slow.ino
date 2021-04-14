// Based on Slow, Copyright (c) 2018, Jason Justian
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

#define HEM_Slow_MAX_VALUE 1200
#define HEM_Slow_MAX_TICKS 128000

class Slow : public HemisphereApplet {
public:

    const char* applet_name() { // Maximum 10 characters
        return "Slow";
    }

    void Start() {
        ForEachChannel(ch) signal[ch] = 0;
        rise = 50;
        fall = 50;
    }

    void Controller() {

        target = ViewIn(0);
        if (current < target){
            current++;
        }
        else if(current > target){
            current--;
        }
        /*if(abs(target - current) < 50){
            current = target;
        }*/
/*
        ForEachChannel(ch)
        {
            simfloat input = int2simfloat(In(ch));
            if (Gate(ch)) signal[ch] = input; // Defeat Slow when channel's gate is high
            if (input != signal[ch]) {

                int segment = (input > signal[ch]) ? rise : fall;
                simfloat remaining = input - signal[ch];

                // The number of ticks it would take to get from 0 to HEMISPHERE_MAX_CV
                int max_change = Proportion(segment, HEM_Slow_MAX_VALUE, HEM_Slow_MAX_TICKS);

                // The number of ticks it would take to move the remaining amount at max_change
                int ticks_to_remaining = Proportion(simfloat2int(remaining), HEMISPHERE_MAX_CV, max_change);
                if (ticks_to_remaining < 0) ticks_to_remaining = -ticks_to_remaining;

                simfloat delta;
                if (ticks_to_remaining <= 0) {
                    delta = remaining;
                } else {
                    if (ch == 1) ticks_to_remaining /= 2;
                    delta = remaining / ticks_to_remaining;
                }
                signal[ch] += delta;
            }
            Out(ch, simfloat2int(signal[ch]));
        }
        */
    }

    void View() {
        gfxHeader(applet_name());
        //DrawIndicator();
        DrawInterface();
    }

    void OnButtonPress() {
        cursor = 1 - cursor;
    }

    void OnEncoderMove(int direction) {
        if (cursor == 0) {
            rise = constrain(rise += direction, 0, HEM_Slow_MAX_VALUE);
            last_ms_value = Proportion(rise, HEM_Slow_MAX_VALUE, HEM_Slow_MAX_TICKS) / 170;
        }
        else {
            fall = constrain(fall += direction, 0, HEM_Slow_MAX_VALUE);
            last_ms_value = Proportion(fall, HEM_Slow_MAX_VALUE, HEM_Slow_MAX_TICKS) / 1;
        }
        last_change_ticks = OC::CORE::ticks;
    }
        
    uint32_t OnDataRequest() {
        uint32_t data = 0;
        Pack(data, PackLocation {0,8}, rise);
        Pack(data, PackLocation {8,8}, fall);
        return data;
    }

    void OnDataReceive(uint32_t data) {
        rise = Unpack(data, PackLocation {0,8});
        fall = Unpack(data, PackLocation {8,8});
    }

protected:
    void SetHelp() {
        //                               "------------------" <-- Size Guide
        help[HEMISPHERE_HELP_DIGITALS] = "Defeat 1=Ch1 2=Ch2";
        help[HEMISPHERE_HELP_CVS]      = "Input 1=Ch1 2=Ch2";
        help[HEMISPHERE_HELP_OUTS]     = "A=Linear B=Exp";
        help[HEMISPHERE_HELP_ENCODER]  = "Rise/Fall";
        //                               "------------------" <-- Size Guide
    }
    
private:
    int rise; // Time to reach signal level if signal < 5V
    int fall; // Time to reach signal level if signal > 0V
    simfloat signal[2]; // Current signal level for each channel
    int cursor; // 0 = Rise, 1 = Fall
    int last_ms_value;
    int last_change_ticks;

    int target;
    int current;

    void DrawInterface() {
        gfxPrint(1, 15, "Target");
        gfxPrint(1, 25, target);

        gfxPrint(1, 35, "Current");
        gfxPrint(1, 45, current);
    }

    void DrawIndicator() {
        // Rise portion
        int r_x = Proportion(rise, 600, 31);
        gfxLine(0, 62, r_x, 33, cursor == 1);

        // Fall portion
        int f_x = 62 - Proportion(fall, 600, 31);
        gfxLine(f_x, 33, 62, 62, cursor == 0);

        // Center portion, if necessary
        gfxLine(r_x, 33, f_x, 33, 1);

        // Output indicators
        ForEachChannel(ch)
        {
            if (Gate(ch)) gfxFrame(1, 15 + (ch * 8), ProportionCV(ViewOut(ch), 62), 6);
            else gfxRect(1, 15 + (ch * 8), ProportionCV(ViewOut(ch), 62), 6);
        }

        // Change indicator, if necessary
        if (OC::CORE::ticks - last_change_ticks < 20000) {
            gfxPrint(15, 43, last_ms_value);
            gfxPrint("ms");
        }
    }
};


////////////////////////////////////////////////////////////////////////////////
//// Hemisphere Applet Functions
///
///  Once you run the find-and-replace to make these refer to Slow,
///  it's usually not necessary to do anything with these functions. You
///  should prefer to handle things in the HemisphereApplet child class
///  above.
////////////////////////////////////////////////////////////////////////////////
Slow Slow_instance[2];

void Slow_Start(bool hemisphere) {
    Slow_instance[hemisphere].BaseStart(hemisphere);
}

void Slow_Controller(bool hemisphere, bool forwarding) {
    Slow_instance[hemisphere].BaseController(forwarding);
}

void Slow_View(bool hemisphere) {
    Slow_instance[hemisphere].BaseView();
}

void Slow_OnButtonPress(bool hemisphere) {
    Slow_instance[hemisphere].OnButtonPress();
}

void Slow_OnEncoderMove(bool hemisphere, int direction) {
    Slow_instance[hemisphere].OnEncoderMove(direction);
}

void Slow_ToggleHelpScreen(bool hemisphere) {
    Slow_instance[hemisphere].HelpScreen();
}

uint32_t Slow_OnDataRequest(bool hemisphere) {
    return Slow_instance[hemisphere].OnDataRequest();
}

void Slow_OnDataReceive(bool hemisphere, uint32_t data) {
    Slow_instance[hemisphere].OnDataReceive(data);
}
