class Switch : public HemisphereApplet {
public:

    const char* applet_name() {
        return "Switch";
    }

    void Start() {
        active[0] = 1;
        active[1] = 1;
    }

    void Controller() {
        // Channel 1 is the gated switch - When gate is high, use channel 2, otherwise channel 1
        if (Gate(0)) {
            active[0] = 2;
            Out(0, In(1));
        } else {
            active[0] = 1;
            Out(0, In(0));
        }

        // Channel 2 is the sequential switch - When clocked, step between channel 1 and 2
        if (Clock(1)) {
            step = 1 - step;
            active[1] = step + 1;
        }
        Out(1, In(step));
    }

    void View() {
        gfxHeader(applet_name());
        gfxPrint(1, 15, "Gate");
        gfxPrint(44, 15, "Seq");
        DrawIndicator();
        gfxSkyline();
    }

    void ScreensaverView() {
        gfxSkyline();
    }

    void OnButtonPress() {
    }

    void OnEncoderMove(int direction) {
    }

    uint32_t OnDataRequest() {
        uint32_t data = 0;
        return data;
    }

    void OnDataReceive(uint32_t data) {
    }

protected:
    void SetHelp() {
        // Each help section can have up to 18 characters. Be concise!
        help[HEMISPHERE_HELP_DIGITALS] = "1=Gate 2=Advance";
        help[HEMISPHERE_HELP_CVS] = "1,2=CV";
        help[HEMISPHERE_HELP_OUTS] = "A=Gated Out B=Seq";
        help[HEMISPHERE_HELP_ENCODER] = "";
    }

private:
    int step = 0;
    int active[2];

    void DrawIndicator()
    {
        ForEachChannel(ch)
        {
            // Selected input indicator
            gfxPrint(5 + (46 * ch), 40, active[ch]);
        }
    }
};


////////////////////////////////////////////////////////////////////////////////
//// Hemisphere Applet Functions
///
///  Once you run the find-and-replace to make these refer to Switch,
///  it's usually not necessary to do anything with these functions. You
///  should prefer to handle things in the HemisphereApplet child class
///  above.
////////////////////////////////////////////////////////////////////////////////
Switch Switch_instance[2];

void Switch_Start(int hemisphere) {
    Switch_instance[hemisphere].BaseStart(hemisphere);
}

void Switch_Controller(int hemisphere, bool forwarding) {
    Switch_instance[hemisphere].BaseController(forwarding);
}

void Switch_View(int hemisphere) {
    Switch_instance[hemisphere].BaseView();
}

void Switch_Screensaver(int hemisphere) {
    Switch_instance[hemisphere].BaseScreensaverView();
}

void Switch_OnButtonPress(int hemisphere) {
    Switch_instance[hemisphere].OnButtonPress();
}

void Switch_OnEncoderMove(int hemisphere, int direction) {
    Switch_instance[hemisphere].OnEncoderMove(direction);
}

void Switch_ToggleHelpScreen(int hemisphere) {
    Switch_instance[hemisphere].HelpScreen();
}

uint32_t Switch_OnDataRequest(int hemisphere) {
    return Switch_instance[hemisphere].OnDataRequest();
}

void Switch_OnDataReceive(int hemisphere, uint32_t data) {
    Switch_instance[hemisphere].OnDataReceive(data);
}
