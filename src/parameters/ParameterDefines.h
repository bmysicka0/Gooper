#pragma once
#include <juce_core/juce_core.h>
enum PARAM {
	GAIN,
	GOOPS,
	FEED,
	RATE,
	SPREAD,
	STEREO,
	SHEPARD,
	GOOPAGE,
	DRYWET,
	TOTAL_NUMBER_PARAMETERS
};
static const std::array<juce::Identifier, PARAM::TOTAL_NUMBER_PARAMETERS> PARAMETER_IDS{
	"GAIN",
	"GOOPS",
	"FEED",
	"RATE",
	"SPREAD",
	"STEREO",
	"SHEPARD",
	"GOOPAGE",
	"DRYWET",
};
static const std::array<juce::String, PARAM::TOTAL_NUMBER_PARAMETERS> PARAMETER_NAMES{
	"GAIN",
	"GOOPS",
	"FEED",
	"RATE",
	"SPREAD",
	"STEREO",
	"SHEPARD",
	"GOOPAGE",
	"DRYWET",
};
static const std::array<juce::NormalisableRange<float>, PARAM::TOTAL_NUMBER_PARAMETERS> PARAMETER_RANGES {
	juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f, 1.0f),
	juce::NormalisableRange<float>(1.0f, 7.0f, 1.0f, 1.0f),
	juce::NormalisableRange<float>(-0.95f, 0.95f, 0.01f, 1.0f),
	juce::NormalisableRange<float>(0.01f, 1.0f, 0.01f, 0.4f),
	juce::NormalisableRange<float>(0.0f, 0.5f, 0.01f, 0.75f),
	juce::NormalisableRange<float>(0.0f, 1.0f, 1.0f, 1.0f),
	juce::NormalisableRange<float>(0.0f, 1.0f, 1.0f, 1.0f),
	juce::NormalisableRange<float>(-10.0f, 10.0f, 0.01f, 1.0f),
	juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f, 1.0f),
};
static const std::array<float, PARAM::TOTAL_NUMBER_PARAMETERS> PARAMETER_DEFAULTS {
	50.0f,
	1.0f,
	0.0f,
	0.01f,
	0.0f,
	0.0f,
	0.0f,
	0.0f,
	50.0f,
};
static const std::array<bool, PARAM::TOTAL_NUMBER_PARAMETERS> PARAMETER_AUTOMATABLE {
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
};
static const std::array<juce::String, PARAM::TOTAL_NUMBER_PARAMETERS> PARAMETER_NICKNAMES{
	"Gain",
	"Goops",
	"Feed",
	"Rate",
	"Spread",
	"Stereo",
	"Shepard",
	"Goopage",
	"DryWet",
};
static const std::array<juce::String, PARAM::TOTAL_NUMBER_PARAMETERS> PARAMETER_SUFFIXES {
	"%",
	"",
	"",
	"Hz",
	"Hz",
	"",
	"",
	"Hz",
	"%",
};
static const std::array<juce::String, PARAM::TOTAL_NUMBER_PARAMETERS> PARAMETER_TOOLTIPS {
	"Gain Parameter",
	"Goops Parameter",
	"Feed Parameter",
	"Rate Parameter",
	"Spread Parameter",
	"Stereo Parameter",
	"Shepard Parameter",
	"Goopage Parameter",
	"DryWet Parameter",
};
static const std::array<std::vector<juce::String>, PARAM::TOTAL_NUMBER_PARAMETERS> PARAMETER_TO_STRING_ARRS {
	std::vector<juce::String>{
	},
	std::vector<juce::String>{
	},
	std::vector<juce::String>{
	},
	std::vector<juce::String>{
	},
	std::vector<juce::String>{
	},
	std::vector<juce::String>{
		"OFF",
		"ON",
	},
	std::vector<juce::String>{
		"OFF",
		"ON",
	},
	std::vector<juce::String>{
	},
	std::vector<juce::String>{
	},
};