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
  , knob(juce::ImageFileFormat::loadFrom(BinaryData::KNB_Pitt_L_png, BinaryData::KNB_Pitt_L_pngSize), 55, 55, 101)
  , gain(paramState, "gain", "Gain", &knob)
  , lowStack(paramState, "bass", "Guitar", &knob)
  , midStack(paramState, "medium", "Medium", &knob)
  , highStack(paramState, "high", "High", &knob)
  , volume(paramState, "volume", "Volume", &knob)
  , drywet(paramState, "drywet", "Dry/Wet", &knob)

{
  addAndMakeVisible(gain);
  addAndMakeVisible(lowStack);
  addAndMakeVisible(midStack);
  addAndMakeVisible(highStack);
  addAndMakeVisible(volume);
  addAndMakeVisible(drywet);

  bckgndImage = juce::ImageFileFormat::loadFrom(BinaryData::background_jpg, BinaryData::background_jpgSize);

  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(380, 253);
}

ATKGuitarPreampAudioProcessorEditor::~ATKGuitarPreampAudioProcessorEditor() = default;

void ATKGuitarPreampAudioProcessorEditor::paint(juce::Graphics& g)
{
  g.drawImageAt(bckgndImage, 0, 0);
  g.setFont(juce::Font("Times New Roman", 30.0f, juce::Font::bold | juce::Font::italic));
  g.setColour(juce::Colours::whitesmoke);
  g.drawText("Guitar Preamp", 20, 10, 200, 30, juce::Justification::verticallyCentred);
  g.setFont(juce::Font("Times New Roman", 12.0f, juce::Font::bold));
  g.drawText(
      "Gain", 20, 120, 50, 30, juce::Justification::horizontallyCentred | juce::Justification::verticallyCentred);
  g.drawText(
      "Low", 120, 120, 50, 30, juce::Justification::horizontallyCentred | juce::Justification::verticallyCentred);
  g.drawText(
      "Mid", 220, 120, 50, 30, juce::Justification::horizontallyCentred | juce::Justification::verticallyCentred);
  g.drawText(
      "High", 320, 120, 50, 30, juce::Justification::horizontallyCentred | juce::Justification::verticallyCentred);
  g.drawText(
      "Volume", 120, 220, 50, 30, juce::Justification::horizontallyCentred | juce::Justification::verticallyCentred);
  g.drawText(
      "Dry/Wet", 220, 220, 50, 30, juce::Justification::horizontallyCentred | juce::Justification::verticallyCentred);
}

void ATKGuitarPreampAudioProcessorEditor::resized()
{
  gain.setBounds(20, 60, 55, 55);
  lowStack.setBounds(120, 60, 55, 55);
  midStack.setBounds(220, 60, 55, 55);
  highStack.setBounds(320, 60, 55, 55);
  volume.setBounds(120, 160, 55, 55);
  drywet.setBounds(220, 160, 55, 55);
}
