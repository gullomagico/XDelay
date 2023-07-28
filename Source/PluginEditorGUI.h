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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
#include "PanDelayGraph.h"
#include "PluginProcessor.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

using namespace juce;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PluginEditorGUI  : public juce::AudioProcessorEditor,
                         public juce::Slider::Listener
{
public:
    //==============================================================================
    PluginEditorGUI (XDelayAudioProcessor& p, AudioProcessorValueTreeState& ppvts);
    ~PluginEditorGUI() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    XDelayAudioProcessor& audioProcessor;
    AudioProcessorValueTreeState& vts;

    std::unique_ptr<SliderAttachment> feedbackAttachment;
    std::unique_ptr<SliderAttachment> mixAttachment;
    std::unique_ptr<SliderAttachment> toneAttachment;

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::GroupComponent> juce__groupComponent;
    std::unique_ptr<juce::Slider> FeedbackSlider;
    std::unique_ptr<juce::Slider> MixSlider;
    std::unique_ptr<juce::Slider> ToneSlider;
    std::unique_ptr<juce::Label> FeedbackLabel;
    std::unique_ptr<juce::Label> MixLabel;
    std::unique_ptr<juce::Label> ToneLabel;
    std::unique_ptr<PanDelayGraph> PanDelayComponent;
    std::unique_ptr<juce::Label> EchoPanLabel;
    std::unique_ptr<juce::Label> DelayLabel;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditorGUI)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

