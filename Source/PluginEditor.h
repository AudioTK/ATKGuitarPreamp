/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "PluginProcessor.h"
#include <JuceHeader.h>

#include <ATKJUCEComponents/EQ/ToneStackFilter.h>
#include <ATKJUCEComponents/Tools/DryWetFilter.h>
#include <ATKJUCEComponents/Tools/VolumeFilter.h>

//==============================================================================
/**
 */
class ATKGuitarPreampAudioProcessorEditor: public juce::AudioProcessorEditor
{
public:
  ATKGuitarPreampAudioProcessorEditor(ATKGuitarPreampAudioProcessor&, juce::AudioProcessorValueTreeState& paramState);
  ~ATKGuitarPreampAudioProcessorEditor() override;

  //==============================================================================
  void paint(juce::Graphics&) override;
  void resized() override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  ATKGuitarPreampAudioProcessor& processor;
  juce::AudioProcessorValueTreeState& paramState;

  ATK::juce::VolumeFilterComponent gain;
  ATK::juce::ToneStackFilterComponent stack;
  ATK::juce::VolumeFilterComponent volume;
  ATK::juce::DryWetFilterComponent drywet;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ATKGuitarPreampAudioProcessorEditor)
};
