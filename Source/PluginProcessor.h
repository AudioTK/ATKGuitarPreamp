/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include <ATK/Core/InPointerFilter.h>
#include <ATK/Core/OutPointerFilter.h>

#include <ATK/Tools/DecimationFilter.h>
#include <ATK/Tools/DryWetFilter.h>
#include <ATK/Tools/OversamplingFilter.h>
#include <ATK/Tools/VolumeFilter.h>

#include <ATK/EQ/ButterworthFilter.h>
#include <ATK/EQ/IIRFilter.h>
#include <ATK/EQ/ToneStackFilter.h>

#include <ATK/Preamplifier/DempwolfTriodeFunction.h>
#include <ATK/Preamplifier/Triode2Filter.h>

//==============================================================================
/**
 */
static const double minVolume = -40;
static const double maxVolume = 40;
static const double minGain = -40;
static const double originGain = -24;
static const double maxGain = 40;

class ATKGuitarPreampAudioProcessor: public AudioProcessor
{
public:
  //==============================================================================
  ATKGuitarPreampAudioProcessor();
  ~ATKGuitarPreampAudioProcessor();

  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

  void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

  //==============================================================================
  AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  const String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  double getTailLengthSeconds() const override;

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const String getProgramName(int index) override;
  void changeProgramName(int index, const String& newName) override;

  //==============================================================================
  void getStateInformation(MemoryBlock& destData) override;
  void setStateInformation(const void* data, int sizeInBytes) override;

private:
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ATKGuitarPreampAudioProcessor)

  ATK::InPointerFilter<float> inFilter;
  ATK::VolumeFilter<double> levelFilter;
  ATK::OversamplingFilter<double, ATK::Oversampling6points5order_4<double>> oversamplingFilter;
  ATK::Triode2Filter<double, ATK::DempwolfTriodeFunction<double>> overdriveFilter;
  ATK::IIRFilter<ATK::ButterworthLowPassCoefficients<double>> lowpassFilter;
  ATK::DecimationFilter<double> decimationFilter;
  ATK::IIRFilter<ATK::ToneStackCoefficients<double>> toneFilter;
  ATK::VolumeFilter<double> volumeFilter;
  ATK::DryWetFilter<double> dryWetFilter;
  ATK::OutPointerFilter<float> outFilter;

  AudioProcessorValueTreeState parameters;

  float old_gain;
  float old_bass;
  float old_medium;
  float old_high;
  float old_volume;
  float old_drywet;
};
