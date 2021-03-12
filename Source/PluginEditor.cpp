/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
ATKGuitarPreampAudioProcessorEditor::ATKGuitarPreampAudioProcessorEditor(ATKGuitarPreampAudioProcessor& p,
    juce::AudioProcessorValueTreeState& paramState)
  : AudioProcessorEditor(&p)
  , processor(p)
  , paramState(paramState)
  , gain(paramState, "gain", "Gain")
  , stack(paramState, "bass", "medium", "high")
  , volume(paramState, "volume", "Volume")
  , drywet(paramState, "drywet")

{
  addAndMakeVisible(gain);
  addAndMakeVisible(stack);
  addAndMakeVisible(volume);
  addAndMakeVisible(drywet);

  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(900, 200);
}

ATKGuitarPreampAudioProcessorEditor::~ATKGuitarPreampAudioProcessorEditor() = default;

void ATKGuitarPreampAudioProcessorEditor::paint(juce::Graphics& g)
{
  // (Our component is opaque, so we must completely fill the background with a solid colour)
  g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
  g.setFont(juce::Font("Times New Roman", 30.0f, juce::Font::bold | juce::Font::italic));
  g.setColour(juce::Colours::whitesmoke);
  g.drawText("Guitar Preamp", 20, 10, 200, 30, juce::Justification::verticallyCentred);
}

void ATKGuitarPreampAudioProcessorEditor::resized()
{
  gain.setBoundsRelative(0, 1. / 4, 1. / 6, 3. / 4);
  stack.setBoundsRelative(1. / 6, 1. / 4, 3. / 6, 3. / 4);
  volume.setBoundsRelative(4. / 6, 1. / 4, 1. / 6, 3. / 4);
  drywet.setBoundsRelative(5. / 6, 1. / 4, 1. / 6, 3. / 4);
}
