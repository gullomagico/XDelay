/*
  ==============================================================================

    PanDelayGraph.h
    Created: 21 Jan 2022 4:27:30pm
    Author:  MULETTO

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#define WIDTH 272
#define HEIGHT 272
#define LINE_THICKNESS 2
#define CIRCLE_RADIUS 10

using namespace juce;
typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

class PanDelayGraph : public juce::Component, public juce::Slider::Listener
{
public:
    PanDelayGraph(XDelayAudioProcessor& p) : audioProcessor(p)
    {
        PanSlider.reset(new juce::Slider("PanSlider"));
        PanSlider->addListener(this);
        DelaySlider.reset(new juce::Slider("DelaySlider"));
        DelaySlider->addListener(this);

        panAttachment.reset(new SliderAttachment(audioProcessor.ppvts, NAME_PAN, *PanSlider));
        delayAttachment.reset(new SliderAttachment(audioProcessor.ppvts, NAME_DELAY, *DelaySlider));

        DelaySlider->setValue(DEFAULT_DELAY);

    };
    ~PanDelayGraph() 
    {
        panAttachment = nullptr;
        delayAttachment = nullptr;
        DelaySlider = nullptr;
        PanSlider = nullptr;
    };

    void paint(juce::Graphics& g) override
    {        
        g.fillAll(Colours::black);

        //disegno i contorni
        g.setColour(Colours::green);
        g.drawLine(0, 0, 0, HEIGHT, LINE_THICKNESS);
        g.drawLine(0, 0, WIDTH, 0, LINE_THICKNESS);
        g.drawLine(0, HEIGHT, WIDTH, HEIGHT, LINE_THICKNESS);
        g.drawLine(WIDTH, 0, WIDTH, HEIGHT, LINE_THICKNESS);

        //disegno la croce
        g.drawLine(WIDTH / 2, 0, WIDTH / 2, HEIGHT, LINE_THICKNESS);
        
        g.drawLine(WIDTH / 2, HEIGHT, WIDTH * 0.25, 0, LINE_THICKNESS);
        g.drawLine(WIDTH / 2, HEIGHT, WIDTH * 0.75, 0, LINE_THICKNESS);

        g.drawLine(WIDTH / 2, HEIGHT, 0, 0, LINE_THICKNESS);
        g.drawLine(WIDTH / 2, HEIGHT, WIDTH, 0, LINE_THICKNESS);

        g.drawLine(WIDTH / 2, HEIGHT, WIDTH / 0.25, 0, LINE_THICKNESS);
        g.drawLine(WIDTH / 2, HEIGHT, WIDTH / 0.75, 0, LINE_THICKNESS);

        g.drawLine(WIDTH / 2, HEIGHT, WIDTH - (WIDTH / 0.25), 0, LINE_THICKNESS);
        g.drawLine(WIDTH / 2, HEIGHT, WIDTH - (WIDTH / 0.75), 0, LINE_THICKNESS);

        //disegno il cursore
        g.setColour(Colours::yellow);
        g.fillEllipse(cursor_x - (CIRCLE_RADIUS / 2), cursor_y - (CIRCLE_RADIUS / 2), CIRCLE_RADIUS, CIRCLE_RADIUS);
    }

    void mouseDrag(const MouseEvent& event) override
    {
        updateValues(event);
    }

    void mouseDown(const MouseEvent& event) override
    {
        updateValues(event);
    }

    void updateValues(const MouseEvent& event) 
    {
        int x = event.getPosition().getX();
        int y = event.getPosition().getY();

        //per settare i bound
        if (x >= 0 && x <= WIDTH)
            cursor_x = x;
        else if (x < 0)
            cursor_x = 0;
        else if (x > WIDTH)
            cursor_x = WIDTH;

        if (y >= 0 && y <= HEIGHT)
            cursor_y = y;
        else if (y < 0)
            cursor_y = 0;
        else if (y > WIDTH)
            cursor_y = WIDTH;

        // normalizzo valori dal canvas
        PanSlider->setValue((2.0f / 272.0f) * ((float)x) - 1);
        DelaySlider->setValue((MAX_DELAY_TIME / 272.0f) * ((float)(272 - y)));

        repaint();
    }

    void PanDelayGraph::sliderValueChanged(juce::Slider* sliderThatWasMoved) override
    {
    }


private:
    //AudioProcessorValueTreeState& vts;
    XDelayAudioProcessor& audioProcessor;

    std::unique_ptr<SliderAttachment> panAttachment;
    std::unique_ptr<SliderAttachment> delayAttachment;

    //slider nascosti
    std::unique_ptr<juce::Slider> PanSlider;
    std::unique_ptr<juce::Slider> DelaySlider;

    int cursor_x = WIDTH / 2;
    int cursor_y = HEIGHT;
};
