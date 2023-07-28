/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "PluginProcessor.h"
#include "PanDelayGraph.h"

using namespace juce;
//[/Headers]

#include "PluginEditorGUI.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
PluginEditorGUI::PluginEditorGUI (XDelayAudioProcessor& p, AudioProcessorValueTreeState& ppvts)
    : AudioProcessorEditor (&p), audioProcessor (p), vts(ppvts)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    juce__groupComponent.reset (new juce::GroupComponent ("new group",
                                                          juce::String()));
    addAndMakeVisible (juce__groupComponent.get());
    juce__groupComponent->setTextLabelPosition (juce::Justification::centredLeft);

    juce__groupComponent->setBounds (9, 392, 480, 128);

    FeedbackSlider.reset (new juce::Slider ("FeedbackSlider"));
    addAndMakeVisible (FeedbackSlider.get());
    FeedbackSlider->setRange (0, 1, 0);
    FeedbackSlider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    FeedbackSlider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    FeedbackSlider->addListener (this);

    FeedbackSlider->setBounds (333, 432, 152, 80);

    MixSlider.reset (new juce::Slider ("MixSlider"));
    addAndMakeVisible (MixSlider.get());
    MixSlider->setRange (0, 1, 0);
    MixSlider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    MixSlider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    MixSlider->addListener (this);

    MixSlider->setBounds (173, 432, 152, 80);

    ToneSlider.reset (new juce::Slider ("ToneSlider"));
    addAndMakeVisible (ToneSlider.get());
    ToneSlider->setRange (-1, 1, 0);
    ToneSlider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    ToneSlider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    ToneSlider->addListener (this);
    ToneSlider->setSkewFactor (0.75);

    ToneSlider->setBounds (13, 432, 152, 80);

    FeedbackLabel.reset (new juce::Label ("FeedbackLabel",
                                          TRANS("Feedback")));
    addAndMakeVisible (FeedbackLabel.get());
    FeedbackLabel->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    FeedbackLabel->setJustificationType (juce::Justification::centred);
    FeedbackLabel->setEditable (false, false, false);
    FeedbackLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    FeedbackLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    FeedbackLabel->setBounds (333, 408, 152, 24);

    MixLabel.reset (new juce::Label ("MixLabel",
                                     TRANS("Mix(Dry/Wet)")));
    addAndMakeVisible (MixLabel.get());
    MixLabel->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    MixLabel->setJustificationType (juce::Justification::centred);
    MixLabel->setEditable (false, false, false);
    MixLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    MixLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    MixLabel->setBounds (173, 408, 152, 24);

    ToneLabel.reset (new juce::Label ("ToneLabel",
                                      TRANS("Tone")));
    addAndMakeVisible (ToneLabel.get());
    ToneLabel->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    ToneLabel->setJustificationType (juce::Justification::centred);
    ToneLabel->setEditable (false, false, false);
    ToneLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    ToneLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    ToneLabel->setBounds (13, 408, 152, 24);

    PanDelayComponent.reset (new PanDelayGraph (p));
    addAndMakeVisible (PanDelayComponent.get());
    PanDelayComponent->setName ("PanDelayComponent");

    EchoPanLabel.reset (new juce::Label ("EchoPanLabel",
                                         TRANS("Echo Pan")));
    addAndMakeVisible (EchoPanLabel.get());
    EchoPanLabel->setFont (juce::Font (14.00f, juce::Font::plain).withTypefaceStyle ("Bold"));
    EchoPanLabel->setJustificationType (juce::Justification::centred);
    EchoPanLabel->setEditable (false, false, false);
    EchoPanLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    EchoPanLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    EchoPanLabel->setBounds (112, 360, 280, 24);

    DelayLabel.reset (new juce::Label ("DelayLabel",
                                       TRANS("D\n"
                                       "e\n"
                                       "l\n"
                                       "a\n"
                                       "y")));
    addAndMakeVisible (DelayLabel.get());
    DelayLabel->setFont (juce::Font (14.00f, juce::Font::plain).withTypefaceStyle ("Bold"));
    DelayLabel->setJustificationType (juce::Justification::centred);
    DelayLabel->setEditable (false, false, false);
    DelayLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    DelayLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    DelayLabel->setBounds (80, 80, 32, 280);


    //[UserPreSize]
    feedbackAttachment.reset(new SliderAttachment(vts, NAME_FEEDBACK, *FeedbackSlider));
    mixAttachment.reset(new SliderAttachment(vts, NAME_MIX, *MixSlider));
    toneAttachment.reset(new SliderAttachment(vts, NAME_TONE, *ToneSlider));
    //[/UserPreSize]

    setSize (500, 550);


    //[Constructor] You can add your own custom stuff here..

    //[/Constructor]
}

PluginEditorGUI::~PluginEditorGUI()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    feedbackAttachment = nullptr;
    mixAttachment = nullptr;
    toneAttachment = nullptr;
    //[/Destructor_pre]

    juce__groupComponent = nullptr;
    FeedbackSlider = nullptr;
    MixSlider = nullptr;
    ToneSlider = nullptr;
    FeedbackLabel = nullptr;
    MixLabel = nullptr;
    ToneLabel = nullptr;
    PanDelayComponent = nullptr;
    EchoPanLabel = nullptr;
    DelayLabel = nullptr;


    //[Destructor]. You can add your own custom destruction code here..

    //[/Destructor]
}

//==============================================================================
void PluginEditorGUI::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff2a3338));

    {
        int x = 0, y = 0, width = 496, height = 72;
        juce::String text (TRANS("X Delay"));
        juce::Colour fillColour1 = juce::Colour (0xff03153f), fillColour2 = juce::Colour (0xff008078);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             392.0f - 0.0f + x,
                                             320.0f - 0.0f + y,
                                             fillColour2,
                                             100.0f - 0.0f + x,
                                             100.0f - 0.0f + y,
                                             true));
        g.setFont (juce::Font (juce::Font::getDefaultMonospacedFontName(), 42.00f, juce::Font::bold));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = getWidth() - 92, y = getHeight() - 19, width = 92, height = 19;
        juce::String text (TRANS("v1.0.0"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (juce::Font::getDefaultSansSerifFontName(), 15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 15, y = getHeight() - 19, width = 328, height = 19;
        juce::String text (TRANS("coded by Cafa and Pre @ LIM"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (juce::Font::getDefaultSansSerifFontName(), 15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PluginEditorGUI::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    PanDelayComponent->setBounds (proportionOfWidth (0.5000f) - (280 / 2), 80, 280, 280);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void PluginEditorGUI::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == FeedbackSlider.get())
    {
        //[UserSliderCode_FeedbackSlider] -- add your slider handling code here..
        //[/UserSliderCode_FeedbackSlider]
    }
    else if (sliderThatWasMoved == MixSlider.get())
    {
        //[UserSliderCode_MixSlider] -- add your slider handling code here..
        //[/UserSliderCode_MixSlider]
    }
    else if (sliderThatWasMoved == ToneSlider.get())
    {
        //[UserSliderCode_ToneSlider] -- add your slider handling code here..
        //[/UserSliderCode_ToneSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PluginEditorGUI" componentName=""
                 parentClasses="public juce::AudioProcessorEditor" constructorParams="XDelayAudioProcessor&amp; p, AudioProcessorValueTreeState&amp; ppvts"
                 variableInitialisers="AudioProcessorEditor (&amp;p), audioProcessor (p), vts(ppvts)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="500" initialHeight="550">
  <BACKGROUND backgroundColour="ff2a3338">
    <TEXT pos="0 0 496 72" fill=" radial: 392 320, 100 100, 0=ff03153f, 1=ff008078"
          hasStroke="0" text="X Delay" fontname="Default monospaced font"
          fontsize="42.0" kerning="0.0" bold="1" italic="0" justification="36"
          typefaceStyle="Bold"/>
    <TEXT pos="0Rr 0Rr 92 19" fill="solid: fff0f8ff" hasStroke="0" text="v1.0.0"
          fontname="Default sans-serif font" fontsize="15.0" kerning="0.0"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="15 0Rr 328 19" fill="solid: fff0f8ff" hasStroke="0" text="coded by Cafa and Pre @ LIM"
          fontname="Default sans-serif font" fontsize="15.0" kerning="0.0"
          bold="0" italic="0" justification="33"/>
  </BACKGROUND>
  <GROUPCOMPONENT name="new group" id="deec32adb724238b" memberName="juce__groupComponent"
                  virtualName="" explicitFocusOrder="0" pos="9 392 480 128" title=""
                  textpos="33"/>
  <SLIDER name="FeedbackSlider" id="f67974cc68240d99" memberName="FeedbackSlider"
          virtualName="" explicitFocusOrder="0" pos="333 432 152 80" min="0.0"
          max="1.0" int="0.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="MixSlider" id="dd96fd0327d144bd" memberName="MixSlider"
          virtualName="" explicitFocusOrder="0" pos="173 432 152 80" min="0.0"
          max="1.0" int="0.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="ToneSlider" id="641a8daa1a6ab101" memberName="ToneSlider"
          virtualName="" explicitFocusOrder="0" pos="13 432 152 80" min="-1.0"
          max="1.0" int="0.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="0.75"
          needsCallback="1"/>
  <LABEL name="FeedbackLabel" id="e7db6f463f9625bd" memberName="FeedbackLabel"
         virtualName="" explicitFocusOrder="0" pos="333 408 152 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Feedback" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="MixLabel" id="71906239077d1f59" memberName="MixLabel" virtualName=""
         explicitFocusOrder="0" pos="173 408 152 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Mix(Dry/Wet)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="ToneLabel" id="1d7a6ef62a273b24" memberName="ToneLabel"
         virtualName="" explicitFocusOrder="0" pos="13 408 152 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Tone" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <GENERICCOMPONENT name="PanDelayComponent" id="1e00799b4f26f6a5" memberName="PanDelayComponent"
                    virtualName="" explicitFocusOrder="0" pos="50%c 80 280 280" class="PanDelayGraph"
                    params="p"/>
  <LABEL name="EchoPanLabel" id="76eabbd45fa5d54c" memberName="EchoPanLabel"
         virtualName="" explicitFocusOrder="0" pos="112 360 280 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Echo Pan" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="14.0"
         kerning="0.0" bold="1" italic="0" justification="36" typefaceStyle="Bold"/>
  <LABEL name="DelayLabel" id="d2901eae0c75a9ec" memberName="DelayLabel"
         virtualName="" explicitFocusOrder="0" pos="80 80 32 280" edTextCol="ff000000"
         edBkgCol="0" labelText="D&#10;e&#10;l&#10;a&#10;y" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="1" italic="0" justification="36"
         typefaceStyle="Bold"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

