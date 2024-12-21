#pragma once
#include <juce_core/juce_core.h>
enum PARAM {
	GOOPS,
	FEED,
	RATE,
	SPREAD,
	STEREO,
	GOOPAGE,
	DRYWET,
	TOTAL_NUMBER_PARAMETERS
};
static const std::array<juce::Identifier, PARAM::TOTAL_NUMBER_PARAMETERS> PARAMETER_IDS{
	"GOOPS",
	"FEED",
	"RATE",
	"SPREAD",
	"STEREO",
	"GOOPAGE",
	"DRYWET",
};
static const std::array<juce::String, PARAM::TOTAL_NUMBER_PARAMETERS> PARAMETER_NAMES{
	"GOOPS",
	"FEED",
	"RATE",
	"SPREAD",
	"STEREO",
	"GOOPAGE",
	"DRYWET",
};
static const std::array<juce::NormalisableRange<float>, PARAM::TOTAL_NUMBER_PARAMETERS> PARAMETER_RANGES {
	juce::NormalisableRange<float>(0.0f, 6.0f, 1.0f, 1.0f),
	juce::NormalisableRange<float>(-0.95f, 0.95f, 0.01f, 1.0f),
	juce::NormalisableRange<float>(0.01f, 1.0f, 0.01f, 0.4f),
	juce::NormalisableRange<float>(0.0f, 0.5f, 0.01f, 0.75f),
	juce::NormalisableRange<float>(0.0f, 1.0f, 1.0f, 1.0f),
	juce::NormalisableRange<float>(-8.0f, 8.0f, 0.01f, 1.0f),
	juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f, 1.0f),
};
static const std::array<float, PARAM::TOTAL_NUMBER_PARAMETERS> PARAMETER_DEFAULTS {
	0.0f,
	0.0f,
	0.01f,
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
};
static const std::array<juce::String, PARAM::TOTAL_NUMBER_PARAMETERS> PARAMETER_NICKNAMES{
	"Goops",
	"Feed",
	"Rate",
	"Spread",
	"Stereo",
	"Goopage",
	"DryWet",
};
static const std::array<juce::String, PARAM::TOTAL_NUMBER_PARAMETERS> PARAMETER_SUFFIXES {
	"",
	"",
	"Hz",
	"Hz",
	"",
	"Hz",
	"%",
};
static const std::array<juce::String, PARAM::TOTAL_NUMBER_PARAMETERS> PARAMETER_TOOLTIPS {
	"Goops Parameter",
	"Feed Parameter",
	"Rate Parameter",
	"Spread Parameter",
	"Stereo Parameter",
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
		"OFF",
		"ON",
	},
	std::vector<juce::String>{
	},
	std::vector<juce::String>{
	},
};