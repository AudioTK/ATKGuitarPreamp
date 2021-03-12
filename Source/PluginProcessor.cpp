/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ATKGuitarPreampAudioProcessor::ATKGuitarPreampAudioProcessor()
  :
#ifndef JucePlugin_PreferredChannelConfigurations
  juce::AudioProcessor(BusesProperties()
#  if !JucePlugin_IsMidiEffect
#    if !JucePlugin_IsSynth
                           .withInput("Input", juce::AudioChannelSet::mono(), true)
#    endif
                           .withOutput("Output", juce::AudioChannelSet::mono(), true)
#  endif
          )
  ,
#endif
  inFilter(nullptr, 1, 0, false)
  , overdriveFilter(ATK::Triode2Filter<double,
        ATK::DempwolfTriodeFunction<double>>::build_standard_filter(220e3, 68e3, 1e6, 1.5e3, 300, 47e-9, 22e-6))
  , toneFilter(ATK::ToneStackCoefficients<double>::buildJCM800Stack())
  , outFilter(nullptr, 1, 0, false)
  , parameters(*this,
        nullptr,
        juce::Identifier("ATKGuitarPreamp"),
        {std::make_unique<juce::AudioParameterFloat>("gain",
             "Gain",
             juce::NormalisableRange<float>(minGain, maxGain),
             originGain,
             " dB"),
            std::make_unique<juce::AudioParameterFloat>("bass", "Bass", -1.0f, 1.0f, .0f),
            std::make_unique<juce::AudioParameterFloat>("medium", "Medium", -1.0f, 1.0f, .0f),
            std::make_unique<juce::AudioParameterFloat>("high", "High", -1.0f, 1.0f, .0f),
            std::make_unique<juce::AudioParameterFloat>("volume",
                "Volume",
                juce::NormalisableRange<float>(minVolume, maxVolume),
                (minVolume + maxVolume) / 2,
                " dB"),
            std::make_unique<juce::AudioParameterFloat>("drywet",
                "Dry/Wet",
                juce::NormalisableRange<float>(0.f, 100.f),
                100.f,
                " dB")})

{
  levelFilter.set_input_port(0, &inFilter, 0);
  oversamplingFilter.set_input_port(0, &levelFilter, 0);
  overdriveFilter.set_input_port(0, &oversamplingFilter, 0);
  lowpassFilter.set_input_port(0, &overdriveFilter, 0);
  decimationFilter.set_input_port(0, &lowpassFilter, 0);
  toneFilter.set_input_port(0, &decimationFilter, 0);
  volumeFilter.set_input_port(0, &toneFilter, 0);
  dryWetFilter.set_input_port(0, &volumeFilter, 0);
  dryWetFilter.set_input_port(1, &inFilter, 0);
  outFilter.set_input_port(0, &dryWetFilter, 0);

  levelFilter.set_volume_db(originGain);
  volumeFilter.set_volume(-1);
  dryWetFilter.set_dry(1);
  lowpassFilter.set_order(4);
  lowpassFilter.set_cut_frequency(20000);

  old_gain = *parameters.getRawParameterValue("gain");
  old_bass = *parameters.getRawParameterValue("bass");
  old_medium = *parameters.getRawParameterValue("medium");
  old_high = *parameters.getRawParameterValue("high");
  old_volume = *parameters.getRawParameterValue("volume");
  old_drywet = *parameters.getRawParameterValue("drywet");
}

ATKGuitarPreampAudioProcessor::~ATKGuitarPreampAudioProcessor()
{
}

//==============================================================================
const juce::String ATKGuitarPreampAudioProcessor::getName() const
{
  return JucePlugin_Name;
}

bool ATKGuitarPreampAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool ATKGuitarPreampAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool ATKGuitarPreampAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double ATKGuitarPreampAudioProcessor::getTailLengthSeconds() const
{
  return 0.0;
}

int ATKGuitarPreampAudioProcessor::getNumPrograms()
{
  return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
            // so this should be at least 1, even if you're not really implementing programs.
}

int ATKGuitarPreampAudioProcessor::getCurrentProgram()
{
  return 0;
}

void ATKGuitarPreampAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String ATKGuitarPreampAudioProcessor::getProgramName(int index)
{
  return {};
}

void ATKGuitarPreampAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void ATKGuitarPreampAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
  auto intsamplerate = std::lround(sampleRate);

  inFilter.set_input_sampling_rate(intsamplerate);
  inFilter.set_output_sampling_rate(intsamplerate);
  levelFilter.set_input_sampling_rate(intsamplerate);
  levelFilter.set_output_sampling_rate(intsamplerate);
  oversamplingFilter.set_input_sampling_rate(intsamplerate);
  oversamplingFilter.set_output_sampling_rate(intsamplerate * 4);
  overdriveFilter.set_input_sampling_rate(intsamplerate * 4);
  overdriveFilter.set_output_sampling_rate(intsamplerate * 4);
  lowpassFilter.set_input_sampling_rate(intsamplerate * 4);
  lowpassFilter.set_output_sampling_rate(intsamplerate * 4);
  decimationFilter.set_input_sampling_rate(intsamplerate * 4);
  decimationFilter.set_output_sampling_rate(intsamplerate);
  toneFilter.set_input_sampling_rate(intsamplerate);
  toneFilter.set_output_sampling_rate(intsamplerate);
  volumeFilter.set_input_sampling_rate(intsamplerate);
  volumeFilter.set_output_sampling_rate(intsamplerate);
  dryWetFilter.set_input_sampling_rate(intsamplerate);
  dryWetFilter.set_output_sampling_rate(intsamplerate);
  outFilter.set_input_sampling_rate(intsamplerate);
  outFilter.set_output_sampling_rate(intsamplerate);
}

void ATKGuitarPreampAudioProcessor::releaseResources()
{
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ATKGuitarPreampAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#  if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#  else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  // Some plugin hosts, such as certain GarageBand versions, will only
  // load plugins that support stereo bus layouts.
  if(layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
      && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

    // This checks if the input layout matches the output layout
#    if !JucePlugin_IsSynth
  if(layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#    endif

  return true;
#  endif
}
#endif

void ATKGuitarPreampAudioProcessor::processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiMessages)
{
  if(*parameters.getRawParameterValue("gain") != old_gain)
  {
    old_gain = *parameters.getRawParameterValue("gain");
    levelFilter.set_volume_db(old_gain);
  }
  if(*parameters.getRawParameterValue("bass") != old_bass)
  {
    old_bass = *parameters.getRawParameterValue("bass");
    toneFilter.set_low((old_bass + 1) / 2);
  }
  if(*parameters.getRawParameterValue("medium") != old_medium)
  {
    old_medium = *parameters.getRawParameterValue("medium");
    toneFilter.set_middle((old_medium + 1) / 2);
  }
  if(*parameters.getRawParameterValue("high") != old_high)
  {
    old_high = *parameters.getRawParameterValue("high");
    toneFilter.set_high((old_high + 1) / 2);
  }
  if(*parameters.getRawParameterValue("volume") != old_volume)
  {
    old_volume = *parameters.getRawParameterValue("volume");
    volumeFilter.set_volume(-std::pow(10., old_volume / 20));
  }

  if(*parameters.getRawParameterValue("drywet") != old_drywet)
  {
    old_drywet = *parameters.getRawParameterValue("drywet");
    dryWetFilter.set_dry(old_drywet / 100);
  }

  const int totalNumInputChannels = getTotalNumInputChannels();
  const int totalNumOutputChannels = getTotalNumOutputChannels();

  assert(totalNumInputChannels == totalNumOutputChannels);
  assert(totalNumOutputChannels == 1);

  inFilter.set_pointer(buffer.getReadPointer(0), buffer.getNumSamples());
  outFilter.set_pointer(buffer.getWritePointer(0), buffer.getNumSamples());

  outFilter.process(buffer.getNumSamples());
}

//==============================================================================
bool ATKGuitarPreampAudioProcessor::hasEditor() const
{
  return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ATKGuitarPreampAudioProcessor::createEditor()
{
  return new ATKGuitarPreampAudioProcessorEditor(*this, parameters);
}

//==============================================================================
void ATKGuitarPreampAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
  juce::MemoryOutputStream store(destData, true);
  store.writeInt(0); // version ID
  store.writeFloat(*parameters.getRawParameterValue("gain"));
  store.writeFloat(*parameters.getRawParameterValue("bass"));
  store.writeFloat(*parameters.getRawParameterValue("medium"));
  store.writeFloat(*parameters.getRawParameterValue("high"));
  store.writeFloat(*parameters.getRawParameterValue("volume"));
  store.writeFloat(*parameters.getRawParameterValue("drywet"));
}

void ATKGuitarPreampAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
  juce::MemoryInputStream store(data, static_cast<size_t>(sizeInBytes), false);
  int version = store.readInt(); // version ID
  *parameters.getRawParameterValue("gain") = store.readFloat();
  *parameters.getRawParameterValue("bass") = store.readFloat();
  *parameters.getRawParameterValue("medium") = store.readFloat();
  *parameters.getRawParameterValue("high") = store.readFloat();
  *parameters.getRawParameterValue("volume") = store.readFloat();
  *parameters.getRawParameterValue("drywet") = store.readFloat();
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
  return new ATKGuitarPreampAudioProcessor();
}
