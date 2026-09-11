#include <JuceHeader.h>

namespace CMS
{
    static const juce::Colour bg = juce::Colour::fromRGB(12, 13, 15);
    static const juce::Colour panel = juce::Colour::fromRGB(24, 25, 29);
    static const juce::Colour panel2 = juce::Colour::fromRGB(32, 34, 39);
    static const juce::Colour panel3 = juce::Colour::fromRGB(42, 44, 50);
    static const juce::Colour line = juce::Colour::fromRGB(60, 63, 70);
    static const juce::Colour text = juce::Colour::fromRGB(235, 237, 242);
    static const juce::Colour subtext = juce::Colour::fromRGB(145, 149, 158);

    static const juce::Colour blue = juce::Colour::fromRGB(45, 170, 255);
    static const juce::Colour cyan = juce::Colour::fromRGB(45, 220, 255);
    static const juce::Colour green = juce::Colour::fromRGB(70, 220, 135);
    static const juce::Colour yellow = juce::Colour::fromRGB(255, 205, 70);
    static const juce::Colour orange = juce::Colour::fromRGB(255, 145, 55);
    static const juce::Colour red = juce::Colour::fromRGB(245, 70, 80);
    static const juce::Colour purple = juce::Colour::fromRGB(175, 100, 255);
}

// ============================================================
// BOTON CMS
// ============================================================

class CMSButton : public juce::TextButton
{
public:
    CMSButton(const juce::String& name,
        juce::Colour normal = CMS::panel3)
        : juce::TextButton(name)
    {
        setColour(juce::TextButton::buttonColourId, normal);
        setColour(juce::TextButton::buttonOnColourId, CMS::blue);
        setColour(juce::TextButton::textColourOffId, CMS::text);
        setColour(juce::TextButton::textColourOnId, CMS::text);
    }
};

// ============================================================
// KNOB DJ
// ============================================================

class CMSKnob : public juce::Slider
{
public:
    CMSKnob(const juce::String& name)
        : knobName(name)
    {
        setSliderStyle(
            juce::Slider::RotaryHorizontalVerticalDrag);

        setTextBoxStyle(
            juce::Slider::NoTextBox,
            false,
            0,
            0);

        setRange(-1.0, 1.0, 0.01);
        setValue(0.0);
    }

    void paint(juce::Graphics& g) override
    {
        auto r = getLocalBounds().toFloat();

        const float labelHeight = 18.0f;
        const float knobSize =
            juce::jmin(r.getWidth() - 8.0f,
                r.getHeight() - labelHeight - 4.0f);

        const float cx = r.getCentreX();
        const float cy =
            labelHeight + knobSize * 0.5f + 2.0f;

        const float radius = knobSize * 0.42f;

        // aro exterior
        g.setColour(CMS::line);
        g.drawEllipse(
            cx - radius - 4.0f,
            cy - radius - 4.0f,
            (radius + 4.0f) * 2.0f,
            (radius + 4.0f) * 2.0f,
            2.0f);

        // cuerpo
        g.setColour(CMS::panel3);
        g.fillEllipse(
            cx - radius,
            cy - radius,
            radius * 2.0f,
            radius * 2.0f);

        // aro indicador
        const double value =
            juce::jlimit(0.0,
                1.0,
                (getValue() - getMinimum())
                / (getMaximum() - getMinimum()));

        const float startAngle =
            juce::MathConstants<float>::pi * 1.25f;

        const float endAngle =
            juce::MathConstants<float>::pi * 2.75f;

        const float angle =
            startAngle
            + (endAngle - startAngle)
            * (float)value;

        juce::Path arc;
        arc.addCentredArc(
            cx,
            cy,
            radius + 5.0f,
            radius + 5.0f,
            0.0f,
            startAngle,
            angle,
            true);

        g.setColour(CMS::blue);
        g.strokePath(
            arc,
            juce::PathStrokeType(3.0f));

        // indicador central
        const float ix =
            cx + std::cos(angle) * (radius - 7.0f);

        const float iy =
            cy + std::sin(angle) * (radius - 7.0f);

        g.setColour(CMS::text);
        g.fillEllipse(
            ix - 2.5f,
            iy - 2.5f,
            5.0f,
            5.0f);

        // nombre
        g.setColour(CMS::subtext);
        g.setFont(
            juce::Font(11.0f));

        g.drawText(
            knobName,
            0,
            0,
            getWidth(),
            (int)labelHeight,
            juce::Justification::centred);
    }

private:
    juce::String knobName;
};

// ============================================================
// WAVEFORM RGB
// ============================================================

class Waveform : public juce::Component
{
public:
    Waveform()
    {
        setOpaque(true);
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(
            juce::Colour::fromRGB(8, 9, 11));

        const float centre =
            getHeight() * 0.5f;

        // línea central
        g.setColour(CMS::line);
        g.drawHorizontalLine(
            (int)centre,
            0.0f,
            (float)getWidth());

        juce::Random random(87321);

        const int bars =
            juce::jmax(1,
                getWidth() / 3);

        for (int i = 0; i < bars; ++i)
        {
            const float x =
                (float)i * 3.0f;

            const float energy =
                0.15f
                + random.nextFloat() * 0.85f;

            const float h =
                8.0f
                + energy
                * ((float)getHeight() * 0.78f);

            /*
                Representación visual de 3 bandas:

                GRAVES  -> azul/cian
                MEDIOS  -> verde
                AGUDOS  -> amarillo/naranja

                Más adelante esto será reemplazado
                por datos reales provenientes del FFT.
            */

            juce::Colour colour;

            const float pos =
                (float)i / (float)bars;

            if (pos < 0.33f)
                colour = CMS::cyan;
            else if (pos < 0.66f)
                colour = CMS::green;
            else
                colour = CMS::yellow;

            g.setColour(
                colour.withAlpha(
                    0.45f + energy * 0.55f));

            g.fillRect(
                x,
                centre - h * 0.5f,
                2.0f,
                h);
        }

        // playhead
        g.setColour(CMS::red);

        g.fillRect(
            getWidth() * 0.25f,
            0.0f,
            2.0f,
            (float)getHeight());

        // beatgrid de demostración
        g.setColour(
            CMS::text.withAlpha(0.16f));

        const int beatSpacing = 80;

        for (int x = beatSpacing;
            x < getWidth();
            x += beatSpacing)
        {
            g.fillRect(
                (float)x,
                0.0f,
                1.0f,
                (float)getHeight());
        }
    }
};

// ============================================================
// LOOP SELECTOR
// ============================================================

class LoopSelector : public juce::Component
{
public:
    LoopSelector()
    {
        addAndMakeVisible(label);

        label.setText(
            "LOOP",
            juce::dontSendNotification);

        label.setColour(
            juce::Label::textColourId,
            CMS::subtext);

        label.setJustificationType(
            juce::Justification::centred);

        addAndMakeVisible(previous);
        addAndMakeVisible(size);
        addAndMakeVisible(next);

        previous.setButtonText("<");
        next.setButtonText(">");

        size.setColour(
            juce::TextButton::buttonColourId,
            CMS::blue);

        previous.setColour(
            juce::TextButton::buttonColourId,
            CMS::panel3);

        next.setColour(
            juce::TextButton::buttonColourId,
            CMS::panel3);

        values =
        {
            0.25, 0.5, 1.0, 2.0, 4.0,
            8.0, 16.0, 32.0, 64.0, 128.0
        };

        updateText();

        previous.onClick = [this]
            {
                index =
                    juce::jmax(0,
                        index - 1);

                updateText();
            };

        next.onClick = [this]
            {
                index =
                    juce::jmin(
                        (int)values.size() - 1,
                        index + 1);

                updateText();
            };
    }

    void resized() override
    {
        auto area =
            getLocalBounds();

        label.setBounds(
            area.removeFromTop(16));

        area.removeFromTop(3);

        previous.setBounds(
            area.removeFromLeft(30));

        area.removeFromLeft(4);

        size.setBounds(
            area.removeFromLeft(70));

        area.removeFromLeft(4);

        next.setBounds(
            area.removeFromLeft(30));
    }

private:
    void updateText()
    {
        const double v =
            values[(size_t)index];

        if (v < 1.0)
            size.setButtonText(
                juce::String(v, 2)
                + " BEAT");
        else
            size.setButtonText(
                juce::String(v, 0)
                + " BEATS");
    }

    juce::Label label;

    CMSButton previous{ "<" };
    CMSButton size{ "" };
    CMSButton next{ ">" };

    std::vector<double> values;
    int index = 4;
};
// ============================================================
// DECK
// ============================================================

class Deck : public juce::Component
{
public:
    Deck(const juce::String& deckName,
        juce::Colour accentColour)
        : name(deckName),
        accent(accentColour)
    {
        // ----------------------------------------------------
        // CABECERA
        // ----------------------------------------------------

        addAndMakeVisible(title);

        title.setText(
            "DECK " + name,
            juce::dontSendNotification);

        title.setColour(
            juce::Label::textColourId,
            accent);

        title.setFont(
            juce::Font(18.0f));

        title.setJustificationType(
            juce::Justification::centredLeft);


        addAndMakeVisible(trackInfo);

        trackInfo.setText(
            "No track loaded",
            juce::dontSendNotification);

        trackInfo.setColour(
            juce::Label::textColourId,
            CMS::subtext);

        trackInfo.setJustificationType(
            juce::Justification::centredLeft);


        addAndMakeVisible(bpm);

        bpm.setText(
            "128.00 BPM",
            juce::dontSendNotification);

        bpm.setColour(
            juce::Label::textColourId,
            CMS::text);

        bpm.setFont(
            juce::Font(20.0f));

        bpm.setJustificationType(
            juce::Justification::centredRight);


        addAndMakeVisible(key);

        key.setText(
            "8A",
            juce::dontSendNotification);

        key.setColour(
            juce::Label::textColourId,
            CMS::yellow);

        key.setFont(
            juce::Font(17.0f));

        key.setJustificationType(
            juce::Justification::centredRight);


        // ----------------------------------------------------
        // WAVEFORM
        // ----------------------------------------------------

        addAndMakeVisible(waveform);


        // ----------------------------------------------------
        // TIEMPOS
        // ----------------------------------------------------

        addAndMakeVisible(timeElapsed);

        timeElapsed.setText(
            "00:00",
            juce::dontSendNotification);

        timeElapsed.setColour(
            juce::Label::textColourId,
            CMS::text);

        timeElapsed.setJustificationType(
            juce::Justification::centredLeft);


        addAndMakeVisible(timeRemaining);

        timeRemaining.setText(
            "-00:00",
            juce::dontSendNotification);

        timeRemaining.setColour(
            juce::Label::textColourId,
            CMS::subtext);

        timeRemaining.setJustificationType(
            juce::Justification::centredRight);


        // ----------------------------------------------------
        // TRANSPORTE
        // ----------------------------------------------------

        addAndMakeVisible(play);
        play.setButtonText("PLAY");

        play.setColour(
            juce::TextButton::buttonColourId,
            accent);

        play.setColour(
            juce::TextButton::textColourOffId,
            CMS::bg);


        addAndMakeVisible(cue);
        cue.setButtonText("CUE");


        addAndMakeVisible(sync);
        sync.setButtonText("SYNC");


        addAndMakeVisible(keyLock);
        keyLock.setButtonText("KEY LOCK");


        addAndMakeVisible(load);
        load.setButtonText("LOAD");


        // ----------------------------------------------------
        // PITCH
        // ----------------------------------------------------

        addAndMakeVisible(pitchMinus);
        pitchMinus.setButtonText("-");


        addAndMakeVisible(pitchPlus);
        pitchPlus.setButtonText("+");


        addAndMakeVisible(pitch);

        pitch.setRange(
            -8.0,
            8.0,
            0.01);

        pitch.setValue(0.0);

        pitch.setTextValueSuffix("%");

        pitch.setSliderStyle(
            juce::Slider::LinearHorizontal);

        pitch.setTextBoxStyle(
            juce::Slider::TextBoxLeft,
            false,
            55,
            22);

        pitch.setColour(
            juce::Slider::trackColourId,
            CMS::line);

        pitch.setColour(
            juce::Slider::thumbColourId,
            accent);

        pitch.setColour(
            juce::Slider::textBoxTextColourId,
            CMS::text);

        pitch.setColour(
            juce::Slider::textBoxBackgroundColourId,
            CMS::panel2);


        // ----------------------------------------------------
        // LOOP
        // ----------------------------------------------------

        addAndMakeVisible(loopSelector);


        // ----------------------------------------------------
        // HOT CUES
        // ----------------------------------------------------

        addAndMakeVisible(hotCue1);
        addAndMakeVisible(hotCue2);
        addAndMakeVisible(hotCue3);
        addAndMakeVisible(hotCue4);

        hotCue1.setButtonText("A");
        hotCue2.setButtonText("B");
        hotCue3.setButtonText("C");
        hotCue4.setButtonText("D");

        hotCue1.setColour(
            juce::TextButton::buttonColourId,
            CMS::red);

        hotCue2.setColour(
            juce::TextButton::buttonColourId,
            CMS::purple);

        hotCue3.setColour(
            juce::TextButton::buttonColourId,
            CMS::green);

        hotCue4.setColour(
            juce::TextButton::buttonColourId,
            CMS::orange);


        // ----------------------------------------------------
        // COMPORTAMIENTO
        // ----------------------------------------------------

        play.onClick = [this]
            {
                playing = !playing;

                play.setButtonText(
                    playing ? "PAUSE" : "PLAY");
            };


        cue.onClick = [this]
            {
                cuePressed = !cuePressed;

                cue.setColour(
                    juce::TextButton::buttonColourId,
                    cuePressed
                    ? CMS::yellow
                    : CMS::panel3);
            };


        sync.onClick = [this]
            {
                syncEnabled = !syncEnabled;

                sync.setColour(
                    juce::TextButton::buttonColourId,
                    syncEnabled
                    ? accent
                    : CMS::panel3);
            };


        keyLock.onClick = [this]
            {
                keyLockEnabled = !keyLockEnabled;

                keyLock.setColour(
                    juce::TextButton::buttonColourId,
                    keyLockEnabled
                    ? accent
                    : CMS::panel3);
            };


        pitchMinus.onClick = [this]
            {
                pitch.setValue(
                    pitch.getValue() - 0.1);
            };


        pitchPlus.onClick = [this]
            {
                pitch.setValue(
                    pitch.getValue() + 0.1);
            };
    }


    void resized() override
    {
        auto area =
            getLocalBounds();


        // ----------------------------------------------------
        // CABECERA
        // ----------------------------------------------------

        auto top =
            area.removeFromTop(40);


        title.setBounds(
            top.removeFromLeft(80));


        load.setBounds(
            top.removeFromRight(55)
            .reduced(2));


        key.setBounds(
            top.removeFromRight(48)
            .reduced(2));


        bpm.setBounds(
            top.removeFromRight(92)
            .reduced(2));


        trackInfo.setBounds(
            top.reduced(4, 2));


        // ----------------------------------------------------
        // WAVEFORM
        // ----------------------------------------------------

        waveform.setBounds(
            area.removeFromTop(150)
            .reduced(3));


        // ----------------------------------------------------
        // TIEMPOS
        // ----------------------------------------------------

        auto time =
            area.removeFromTop(22);


        timeElapsed.setBounds(
            time.removeFromLeft(65));


        timeRemaining.setBounds(
            time.removeFromRight(65));


        // ----------------------------------------------------
        // HOT CUES
        // ----------------------------------------------------

        auto hot =
            area.removeFromTop(32);


        hotCue1.setBounds(
            hot.removeFromLeft(38)
            .reduced(2));


        hotCue2.setBounds(
            hot.removeFromLeft(38)
            .reduced(2));


        hotCue3.setBounds(
            hot.removeFromLeft(38)
            .reduced(2));


        hotCue4.setBounds(
            hot.removeFromLeft(38)
            .reduced(2));


        // ----------------------------------------------------
        // TRANSPORTE
        // ----------------------------------------------------

        auto controls =
            area.removeFromTop(42);


        play.setBounds(
            controls.removeFromLeft(75)
            .reduced(2));


        cue.setBounds(
            controls.removeFromLeft(55)
            .reduced(2));


        sync.setBounds(
            controls.removeFromLeft(58)
            .reduced(2));


        keyLock.setBounds(
            controls.removeFromLeft(72)
            .reduced(2));


        loopSelector.setBounds(
            controls.removeFromRight(145)
            .reduced(2));


        // ----------------------------------------------------
        // PITCH
        // ----------------------------------------------------

        auto pitchArea =
            area.removeFromTop(40);


        pitchMinus.setBounds(
            pitchArea.removeFromLeft(28)
            .reduced(2));


        pitchPlus.setBounds(
            pitchArea.removeFromRight(28)
            .reduced(2));


        pitch.setBounds(
            pitchArea.reduced(2));
    }


private:

    juce::String name;

    juce::Colour accent;


    // --------------------------------------------------------
    // CABECERA
    // --------------------------------------------------------

    juce::Label title;

    juce::Label trackInfo;

    juce::Label bpm;

    juce::Label key;


    // --------------------------------------------------------
    // WAVEFORM
    // --------------------------------------------------------

    Waveform waveform;


    // --------------------------------------------------------
    // TIEMPOS
    // --------------------------------------------------------

    juce::Label timeElapsed;

    juce::Label timeRemaining;


    // --------------------------------------------------------
    // BOTONES
    // --------------------------------------------------------

    CMSButton load{ "LOAD" };

    CMSButton play{ "PLAY" };

    CMSButton cue{ "CUE" };

    CMSButton sync{ "SYNC" };

    CMSButton keyLock{ "KEY LOCK" };


    // --------------------------------------------------------
    // PITCH
    // --------------------------------------------------------

    CMSButton pitchMinus{ "-" };

    CMSButton pitchPlus{ "+" };

    juce::Slider pitch;


    // --------------------------------------------------------
    // LOOP
    // --------------------------------------------------------

    LoopSelector loopSelector;


    // --------------------------------------------------------
    // HOT CUES
    // --------------------------------------------------------

    CMSButton hotCue1{ "A" };

    CMSButton hotCue2{ "B" };

    CMSButton hotCue3{ "C" };

    CMSButton hotCue4{ "D" };


    // --------------------------------------------------------
    // ESTADO
    // --------------------------------------------------------

    bool playing = false;

    bool cuePressed = false;

    bool syncEnabled = false;

    bool keyLockEnabled = false;
};
// ============================================================
// MIXER CENTRAL
// ============================================================

class ChannelStrip : public juce::Component
{
public:
    ChannelStrip(const juce::String& channelName,
        juce::Colour accentColour)
        : name(channelName),
        accent(accentColour),
        gain("GAIN"),
        high("HIGH"),
        mid("MID"),
        low("LOW"),
        filter("FILTER")
    {
        // ----------------------------------------------------
        // NOMBRE DEL CANAL
        // ----------------------------------------------------

        addAndMakeVisible(channelLabel);

        channelLabel.setText(
            name,
            juce::dontSendNotification);

        channelLabel.setColour(
            juce::Label::textColourId,
            accent);

        channelLabel.setFont(
            juce::Font(14.0f));

        channelLabel.setJustificationType(
            juce::Justification::centred);


        // ----------------------------------------------------
        // KNOBS
        // ----------------------------------------------------

        addAndMakeVisible(gain);
        addAndMakeVisible(high);
        addAndMakeVisible(mid);
        addAndMakeVisible(low);
        addAndMakeVisible(filter);


        // ----------------------------------------------------
        // KILL
        // ----------------------------------------------------

        addAndMakeVisible(highKill);
        addAndMakeVisible(midKill);
        addAndMakeVisible(lowKill);

        highKill.setButtonText("HI KILL");
        midKill.setButtonText("MID KILL");
        lowKill.setButtonText("LOW KILL");


        highKill.setColour(
            juce::TextButton::buttonColourId,
            CMS::panel3);

        midKill.setColour(
            juce::TextButton::buttonColourId,
            CMS::panel3);

        lowKill.setColour(
            juce::TextButton::buttonColourId,
            CMS::panel3);


        highKill.onClick = [this]
            {
                highKilled = !highKilled;

                highKill.setColour(
                    juce::TextButton::buttonColourId,
                    highKilled
                    ? CMS::red
                    : CMS::panel3);
            };


        midKill.onClick = [this]
            {
                midKilled = !midKilled;

                midKill.setColour(
                    juce::TextButton::buttonColourId,
                    midKilled
                    ? CMS::red
                    : CMS::panel3);
            };


        lowKill.onClick = [this]
            {
                lowKilled = !lowKilled;

                lowKill.setColour(
                    juce::TextButton::buttonColourId,
                    lowKilled
                    ? CMS::red
                    : CMS::panel3);
            };


        // ----------------------------------------------------
        // FADER
        // ----------------------------------------------------

        addAndMakeVisible(fader);

        fader.setSliderStyle(
            juce::Slider::LinearVertical);

        fader.setTextBoxStyle(
            juce::Slider::NoTextBox,
            false,
            0,
            0);

        fader.setRange(
            0.0,
            1.0,
            0.001);

        fader.setValue(0.85);

        fader.setColour(
            juce::Slider::trackColourId,
            CMS::line);

        fader.setColour(
            juce::Slider::thumbColourId,
            accent);


        // ----------------------------------------------------
        // VU
        // ----------------------------------------------------

        addAndMakeVisible(vuMeter);
    }


    void resized() override
    {
        auto area =
            getLocalBounds();


        // ----------------------------------------------------
        // NOMBRE
        // ----------------------------------------------------

        channelLabel.setBounds(
            area.removeFromTop(24)
            .reduced(3, 2));


        // ----------------------------------------------------
        // GAIN
        // ----------------------------------------------------

        gain.setBounds(
            area.removeFromTop(58)
            .reduced(2));


        // ----------------------------------------------------
        // EQ
        // ----------------------------------------------------

        auto eq =
            area.removeFromTop(62);

        high.setBounds(
            eq.removeFromLeft(
                eq.getWidth() / 3)
            .reduced(2));

        mid.setBounds(
            eq.removeFromLeft(
                eq.getWidth() / 2)
            .reduced(2));

        low.setBounds(
            eq.reduced(2));


        // ----------------------------------------------------
        // KILL
        // ----------------------------------------------------

        auto kills =
            area.removeFromTop(54);

        highKill.setBounds(
            kills.removeFromTop(17)
            .reduced(2));

        midKill.setBounds(
            kills.removeFromTop(17)
            .reduced(2));

        lowKill.setBounds(
            kills.reduced(2));


        // ----------------------------------------------------
        // FILTER
        // ----------------------------------------------------

        filter.setBounds(
            area.removeFromTop(58)
            .reduced(2));


        // ----------------------------------------------------
        // FADER + VU
        // ----------------------------------------------------

        auto bottom =
            area.reduced(3);

        vuMeter.setBounds(
            bottom.removeFromRight(15));

        fader.setBounds(
            bottom.reduced(3));
    }


private:

    juce::String name;

    juce::Colour accent;


    // --------------------------------------------------------
    // LABEL
    // --------------------------------------------------------

    juce::Label channelLabel;


    // --------------------------------------------------------
    // EQ
    // --------------------------------------------------------

    CMSKnob gain;

    CMSKnob high;

    CMSKnob mid;

    CMSKnob low;

    CMSKnob filter;


    // --------------------------------------------------------
    // KILL
    // --------------------------------------------------------

    CMSButton highKill{ "HI KILL" };

    CMSButton midKill{ "MID KILL" };

    CMSButton lowKill{ "LOW KILL" };


    // --------------------------------------------------------
    // FADER
    // --------------------------------------------------------

    juce::Slider fader;


    // --------------------------------------------------------
    // VU
    // --------------------------------------------------------

    juce::Component vuMeter;


    // --------------------------------------------------------
    // ESTADO
    // --------------------------------------------------------

    bool highKilled = false;

    bool midKilled = false;

    bool lowKilled = false;
};


// ============================================================
// CROSSFADE / MASTER
// ============================================================

class MasterSection : public juce::Component
{
public:
    MasterSection()
    {
        // ----------------------------------------------------
        // MASTER
        // ----------------------------------------------------

        addAndMakeVisible(masterLabel);

        masterLabel.setText(
            "MASTER",
            juce::dontSendNotification);

        masterLabel.setColour(
            juce::Label::textColourId,
            CMS::text);

        masterLabel.setJustificationType(
            juce::Justification::centred);


        addAndMakeVisible(master);

        master.setSliderStyle(
            juce::Slider::LinearVertical);

        master.setTextBoxStyle(
            juce::Slider::NoTextBox,
            false,
            0,
            0);

        master.setRange(
            0.0,
            1.0,
            0.001);

        master.setValue(0.85);

        master.setColour(
            juce::Slider::trackColourId,
            CMS::line);

        master.setColour(
            juce::Slider::thumbColourId,
            CMS::blue);


        // ----------------------------------------------------
        // CROSSFADER
        // ----------------------------------------------------

        addAndMakeVisible(crossfaderLabel);

        crossfaderLabel.setText(
            "CROSSFADER",
            juce::dontSendNotification);

        crossfaderLabel.setColour(
            juce::Label::textColourId,
            CMS::subtext);

        crossfaderLabel.setJustificationType(
            juce::Justification::centred);


        addAndMakeVisible(crossfader);

        crossfader.setSliderStyle(
            juce::Slider::LinearHorizontal);

        crossfader.setTextBoxStyle(
            juce::Slider::NoTextBox,
            false,
            0,
            0);

        crossfader.setRange(
            -1.0,
            1.0,
            0.001);

        crossfader.setValue(0.0);

        crossfader.setColour(
            juce::Slider::trackColourId,
            CMS::line);

        crossfader.setColour(
            juce::Slider::thumbColourId,
            CMS::blue);
    }


    void resized() override
    {
        auto area =
            getLocalBounds();


        masterLabel.setBounds(
            area.removeFromTop(20));


        auto masterArea =
            area.removeFromTop(
                juce::jmin(
                    120,
                    area.getHeight() / 2));


        master.setBounds(
            masterArea.reduced(8, 4));


        crossfaderLabel.setBounds(
            area.removeFromTop(20));


        crossfader.setBounds(
            area.removeFromTop(30)
            .reduced(5, 4));
    }


private:

    juce::Label masterLabel;

    juce::Slider master;


    juce::Label crossfaderLabel;

    juce::Slider crossfader;
};


// ============================================================
// MIXER COMPLETO
// ============================================================

class Mixer : public juce::Component
{
public:
    Mixer()
    {
        addAndMakeVisible(leftChannel);

        addAndMakeVisible(rightChannel);

        addAndMakeVisible(master);


        addAndMakeVisible(mixerLabel);

        mixerLabel.setText(
            "MIXER",
            juce::dontSendNotification);

        mixerLabel.setColour(
            juce::Label::textColourId,
            CMS::text);

        mixerLabel.setFont(
            juce::Font(15.0f));

        mixerLabel.setJustificationType(
            juce::Justification::centred);
    }


    void resized() override
    {
        auto area =
            getLocalBounds();


        mixerLabel.setBounds(
            area.removeFromTop(25));


        const int channelWidth =
            juce::jmax(
                80,
                area.getWidth() / 3);


        auto left =
            area.removeFromLeft(
                channelWidth);


        auto right =
            area.removeFromRight(
                channelWidth);


        leftChannel.setBounds(
            left.reduced(2));


        rightChannel.setBounds(
            right.reduced(2));


        master.setBounds(
            area.reduced(3));
    }


private:

    ChannelStrip leftChannel
    {
        "A",
        CMS::cyan
    };


    ChannelStrip rightChannel
    {
        "B",
        CMS::yellow
    };


    MasterSection master;


    juce::Label mixerLabel;
};
// ============================================================
// FX BANNER
// ============================================================

class FXBanner : public juce::Component
{
public:
    FXBanner()
    {
        addAndMakeVisible(title);

        title.setText(
            "FX",
            juce::dontSendNotification);

        title.setColour(
            juce::Label::textColourId,
            CMS::text);

        title.setFont(
            juce::Font(15.0f));

        title.setJustificationType(
            juce::Justification::centred);


        // ----------------------------------------------------
        // FX 1
        // ----------------------------------------------------

        addAndMakeVisible(fx1);

        fx1.setButtonText("FX 1");

        addAndMakeVisible(fx1Type);

        fx1Type.setText(
            "ECHO",
            juce::dontSendNotification);

        fx1Type.setColour(
            juce::Label::textColourId,
            CMS::subtext);

        fx1Type.setJustificationType(
            juce::Justification::centred);


        // ----------------------------------------------------
        // FX 2
        // ----------------------------------------------------

        addAndMakeVisible(fx2);

        fx2.setButtonText("FX 2");

        addAndMakeVisible(fx2Type);

        fx2Type.setText(
            "FILTER",
            juce::dontSendNotification);

        fx2Type.setColour(
            juce::Label::textColourId,
            CMS::subtext);

        fx2Type.setJustificationType(
            juce::Justification::centred);


        // ----------------------------------------------------
        // FX 3
        // ----------------------------------------------------

        addAndMakeVisible(fx3);

        fx3.setButtonText("FX 3");

        addAndMakeVisible(fx3Type);

        fx3Type.setText(
            "REVERB",
            juce::dontSendNotification);

        fx3Type.setColour(
            juce::Label::textColourId,
            CMS::subtext);

        fx3Type.setJustificationType(
            juce::Justification::centred);


        // ----------------------------------------------------
        // FX 4
        // ----------------------------------------------------

        addAndMakeVisible(fx4);

        fx4.setButtonText("FX 4");

        addAndMakeVisible(fx4Type);

        fx4Type.setText(
            "FLANGER",
            juce::dontSendNotification);

        fx4Type.setColour(
            juce::Label::textColourId,
            CMS::subtext);

        fx4Type.setJustificationType(
            juce::Justification::centred);


        // ----------------------------------------------------
        // WET
        // ----------------------------------------------------

        addAndMakeVisible(wet);

        wet.setRange(
            0.0,
            100.0,
            1.0);

        wet.setValue(35.0);

        wet.setTextValueSuffix("%");

        wet.setTextBoxStyle(
            juce::Slider::TextBoxRight,
            false,
            45,
            20);


        fx1.onClick = [this]
            {
                toggleFX(fx1);
            };

        fx2.onClick = [this]
            {
                toggleFX(fx2);
            };

        fx3.onClick = [this]
            {
                toggleFX(fx3);
            };

        fx4.onClick = [this]
            {
                toggleFX(fx4);
            };
    }


    void resized() override
    {
        auto area =
            getLocalBounds();


        title.setBounds(
            area.removeFromLeft(45));


        auto fxWidth =
            area.getWidth() / 4;


        auto a =
            area.removeFromLeft(fxWidth);

        fx1.setBounds(
            a.removeFromLeft(50)
            .reduced(2));

        fx1Type.setBounds(
            a.reduced(2));


        auto b =
            area.removeFromLeft(fxWidth);

        fx2.setBounds(
            b.removeFromLeft(50)
            .reduced(2));

        fx2Type.setBounds(
            b.reduced(2));


        auto c =
            area.removeFromLeft(fxWidth);

        fx3.setBounds(
            c.removeFromLeft(50)
            .reduced(2));

        fx3Type.setBounds(
            c.reduced(2));


        auto d =
            area.removeFromLeft(fxWidth);

        fx4.setBounds(
            d.removeFromLeft(50)
            .reduced(2));

        fx4Type.setBounds(
            d.reduced(2));


        wet.setBounds(
            getLocalBounds()
            .removeFromRight(100)
            .reduced(3));
    }


private:

    void toggleFX(
        CMSButton& button)
    {
        const bool active =
            button.findColour(
                juce::TextButton::buttonColourId)
            == CMS::purple;

        button.setColour(
            juce::TextButton::buttonColourId,
            active
            ? CMS::panel3
            : CMS::purple);
    }


    juce::Label title;


    CMSButton fx1{ "FX 1" };
    CMSButton fx2{ "FX 2" };
    CMSButton fx3{ "FX 3" };
    CMSButton fx4{ "FX 4" };


    juce::Label fx1Type;
    juce::Label fx2Type;
    juce::Label fx3Type;
    juce::Label fx4Type;


    juce::Slider wet;
};


// ============================================================
// LIBRARY
// ============================================================

class Library : public juce::Component
{
public:
    Library()
    {
        // ----------------------------------------------------
        // TITULO
        // ----------------------------------------------------

        addAndMakeVisible(title);

        title.setText(
            "MUSIC LIBRARY",
            juce::dontSendNotification);

        title.setColour(
            juce::Label::textColourId,
            CMS::text);

        title.setFont(
            juce::Font(16.0f));


        // ----------------------------------------------------
        // SEARCH
        // ----------------------------------------------------

        addAndMakeVisible(search);

        search.setTextToShowWhenEmpty(
            "Search artist, title, album, BPM, key...",
            CMS::subtext);

        search.setColour(
            juce::TextEditor::backgroundColourId,
            CMS::panel2);

        search.setColour(
            juce::TextEditor::textColourId,
            CMS::text);

        search.setColour(
            juce::TextEditor::outlineColourId,
            CMS::line);


        // ----------------------------------------------------
        // CARPETAS
        // ----------------------------------------------------

        addAndMakeVisible(collection);

        collection.setButtonText(
            "COLLECTION");

        addAndMakeVisible(folders);

        folders.setButtonText(
            "FOLDERS");

        addAndMakeVisible(playlists);

        playlists.setButtonText(
            "PLAYLISTS");

        addAndMakeVisible(history);

        history.setButtonText(
            "HISTORY");


        // ----------------------------------------------------
        // TABLA
        // ----------------------------------------------------

        addAndMakeVisible(table);

        table.getHeader().addColumn(
            "ARTIST",
            1,
            170);

        table.getHeader().addColumn(
            "TITLE",
            2,
            230);

        table.getHeader().addColumn(
            "BPM",
            3,
            65);

        table.getHeader().addColumn(
            "KEY",
            4,
            60);

        table.getHeader().addColumn(
            "GENRE",
            5,
            110);

        table.getHeader().addColumn(
            "DURATION",
            6,
            90);

        table.getHeader().addColumn(
            "RATING",
            7,
            70);

        table.getHeader().addColumn(
            "COMMENTS",
            8,
            180);

        table.setColour(
            juce::ListBox::backgroundColourId,
            CMS::panel);

        table.setColour(
            juce::ListBox::outlineColourId,
            CMS::line);

        table.setModel(&model);
    }


    void resized() override
    {
        auto area =
            getLocalBounds();


        // ----------------------------------------------------
        // HEADER
        // ----------------------------------------------------

        auto header =
            area.removeFromTop(38);


        title.setBounds(
            header.removeFromLeft(150));


        search.setBounds(
            header.removeFromLeft(
                juce::jmax(
                    200,
                    header.getWidth() - 260))
            .reduced(3));


        // ----------------------------------------------------
        // FILTROS
        // ----------------------------------------------------

        auto filters =
            area.removeFromTop(32);


        collection.setBounds(
            filters.removeFromLeft(100)
            .reduced(2));


        folders.setBounds(
            filters.removeFromLeft(80)
            .reduced(2));


        playlists.setBounds(
            filters.removeFromLeft(90)
            .reduced(2));


        history.setBounds(
            filters.removeFromLeft(75)
            .reduced(2));


        // ----------------------------------------------------
        // TABLA
        // ----------------------------------------------------

        table.setBounds(
            area.reduced(2));
    }


private:

    // ========================================================
    // MODELO DE TABLA
    // ========================================================

    class LibraryModel
        : public juce::TableListBoxModel
    {
    public:

        int getNumRows() override
        {
            return 8;
        }


        void paintRowBackground(
            juce::Graphics& g,
            int rowNumber,
            int width,
            int height,
            bool rowIsSelected) override
        {
            if (rowIsSelected)
            {
                g.fillAll(
                    CMS::blue.withAlpha(0.25f));
            }
            else if (rowNumber % 2 == 0)
            {
                g.fillAll(
                    CMS::panel2);
            }
        }


        void paintCell(
            juce::Graphics& g,
            int rowNumber,
            int columnId,
            int width,
            int height,
            bool rowIsSelected) override
        {
            juce::String value;


            switch (columnId)
            {
            case 1:
                value = "Artist " +
                    juce::String(rowNumber + 1);
                break;

            case 2:
                value = "Track " +
                    juce::String(rowNumber + 1);
                break;

            case 3:
                value = "128";
                break;

            case 4:
                value = "8A";
                break;

            case 5:
                value = "Electronic";
                break;

            case 6:
                value = "04:32";
                break;

            case 7:
                value = "★★★★★";
                break;

            case 8:
                value = "";
                break;

            default:
                break;
            }


            g.setColour(
                rowIsSelected
                ? CMS::text
                : CMS::subtext);


            g.setFont(
                juce::Font(12.0f));


            g.drawText(
                value,
                6,
                0,
                width - 12,
                height,
                juce::Justification::centredLeft);
        }
    };


    juce::Label title;

    juce::TextEditor search;


    CMSButton collection
    {
        "COLLECTION"
    };

    CMSButton folders
    {
        "FOLDERS"
    };

    CMSButton playlists
    {
        "PLAYLISTS"
    };

    CMSButton history
    {
        "HISTORY"
    };


    juce::TableListBox table;

    LibraryModel model;
};


// ============================================================
// MAIN COMPONENT
// ============================================================

class MainComponent : public juce::Component
{
public:
    MainComponent()
    {
        // ----------------------------------------------------
        // FX
        // ----------------------------------------------------

        addAndMakeVisible(fxBanner);


        // ----------------------------------------------------
        // DECKS
        // ----------------------------------------------------

        addAndMakeVisible(deckA);

        addAndMakeVisible(mixer);

        addAndMakeVisible(deckB);


        // ----------------------------------------------------
        // LIBRARY
        // ----------------------------------------------------

        addAndMakeVisible(library);


        setSize(
            1500,
            900);
    }


    void paint(
        juce::Graphics& g) override
    {
        g.fillAll(
            CMS::bg);


        // Separadores
        g.setColour(
            CMS::line.withAlpha(0.7f));


        g.drawHorizontalLine(
            fxBanner.getBottom(),
            0.0f,
            (float)getWidth());


        g.drawHorizontalLine(
            library.getY(),
            0.0f,
            (float)getWidth());


        // Separación decks/mixer
        g.drawVerticalLine(
            mixer.getX(),
            (float)fxBanner.getBottom(),
            (float)library.getY());


        g.drawVerticalLine(
            mixer.getRight(),
            (float)fxBanner.getBottom(),
            (float)library.getY());
    }


    void resized() override
    {
        auto area =
            getLocalBounds();


        // ----------------------------------------------------
        // FX SUPERIOR
        // ----------------------------------------------------

        auto fxArea =
            area.removeFromTop(52);

        fxBanner.setBounds(
            fxArea.reduced(4, 3));


        // ----------------------------------------------------
        // LIBRARY
        // ----------------------------------------------------

        const int libraryHeight =
            juce::jmax(
                260,
                (int) (area.getHeight() * 0.38f));


        auto libraryArea =
            area.removeFromBottom(
                libraryHeight);


        library.setBounds(
            libraryArea.reduced(3));


        // ----------------------------------------------------
        // PERFORMANCE AREA
        // ----------------------------------------------------

        auto performance =
            area;


        const int mixerWidth =
            juce::jmax(
                270,
                performance.getWidth() / 5);


        // ----------------------------------------------------
        // DECK A | MIXER | DECK B
        // ----------------------------------------------------

        const int deckWidth =
            (performance.getWidth() - mixerWidth)
            / 2;


        auto deckAArea =
            performance
            .removeFromLeft(
                deckWidth);


        auto mixerArea =
            performance
            .removeFromLeft(
                mixerWidth);


        auto deckBArea =
            performance;


        deckA.setBounds(
            deckAArea.reduced(3));


        mixer.setBounds(
            mixerArea.reduced(3));


        deckB.setBounds(
            deckBArea.reduced(3));
    }


private:

    FXBanner fxBanner;


    Deck deckA
    {
        "A",
        CMS::cyan
    };


    Mixer mixer;


    Deck deckB
    {
        "B",
        CMS::yellow
    };


    Library library;
};


// ============================================================
// DOCUMENT WINDOW
// ============================================================

class MainWindow
    : public juce::DocumentWindow
{
public:

    MainWindow(
        juce::String name)
        : DocumentWindow(
            name,
            CMS::bg,
            juce::DocumentWindow::allButtons)
    {
        setUsingNativeTitleBar(true);

        setContentOwned(
            new MainComponent(),
            true);

        centreWithSize(
            1500,
            900);

        setResizable(
            true,
            true);

        setResizeLimits(
            1100,
            700,
            2200,
            1400);

        setVisible(true);
    }


    void closeButtonPressed() override
    {
        juce::JUCEApplication::
            getInstance()
            ->systemRequestedQuit();
    }
};


// ============================================================
// APLICACION
// ============================================================

class CMSPerformanceApplication
    : public juce::JUCEApplication
{
public:

    CMSPerformanceApplication() = default;


    const juce::String getApplicationName()
        override
    {
        return "CMS Performance DJ";
    }


    const juce::String getApplicationVersion()
        override
    {
        return "0.1.0";
    }


    bool moreThanOneInstanceAllowed()
        override
    {
        return true;
    }


    void initialise(
        const juce::String&)
        override
    {
        mainWindow.reset(
            new MainWindow(
                getApplicationName()));
    }


    void shutdown()
        override
    {
        mainWindow = nullptr;
    }


    void systemRequestedQuit()
        override
    {
        quit();
    }


    void anotherInstanceStarted(
        const juce::String&)
        override
    {
    }


private:

    std::unique_ptr<MainWindow> mainWindow;
};


// ============================================================
// START JUCE
// ============================================================

START_JUCE_APPLICATION(
    CMSPerformanceApplication)