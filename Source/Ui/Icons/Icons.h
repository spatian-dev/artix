/*
  ==============================================================================

    Icons.h
    Created: 15 Dec 2024 10:30:16pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace Artix::Ui {
    using DrawablePtr = std::shared_ptr<const juce::Drawable>;
    class Icons {
        public:
        static const Icons& get() {
            static Icons instance;
            return instance;
        }

        const DrawablePtr caretLeftOutline = juce::Drawable::createFromImageData(
                BinaryData::caretleftoutline_svg, BinaryData::caretleftoutline_svgSize
        );
        const DrawablePtr caretLeftSolid = juce::Drawable::createFromImageData(
                BinaryData::caretleftsolid_svg, BinaryData::caretleftsolid_svgSize
        );
        const DrawablePtr caretRightOutline = juce::Drawable::createFromImageData(
                BinaryData::caretrightoutline_svg, BinaryData::caretrightoutline_svgSize
        );
        const DrawablePtr caretRightSolid = juce::Drawable::createFromImageData(
                BinaryData::caretrightsolid_svg, BinaryData::caretrightsolid_svgSize
        );
        const DrawablePtr floppyOutline = juce::Drawable::createFromImageData(
                BinaryData::floppyoutline_svg, BinaryData::floppyoutline_svgSize
        );
        const DrawablePtr floppySolid = juce::Drawable::createFromImageData(
                BinaryData::floppysolid_svg, BinaryData::floppysolid_svgSize
        );
        const DrawablePtr folderDuplicateOutline = juce::Drawable::createFromImageData(
                BinaryData::folderduplicateoutline_svg, BinaryData::folderduplicateoutline_svgSize
        );
        const DrawablePtr folderDuplicateSolid = juce::Drawable::createFromImageData(
                BinaryData::folderduplicatesolid_svg, BinaryData::folderduplicatesolid_svgSize
        );
        const DrawablePtr folderOpenOutline = juce::Drawable::createFromImageData(
                BinaryData::folderopenoutline_svg, BinaryData::folderopenoutline_svgSize
        );
        const DrawablePtr folderOpenSolid = juce::Drawable::createFromImageData(
                BinaryData::folderopensolid_svg, BinaryData::folderopensolid_svgSize
        );
        const DrawablePtr github = juce::Drawable::createFromImageData(
                BinaryData::github_svg, BinaryData::github_svgSize
        );
        const DrawablePtr globeOutline = juce::Drawable::createFromImageData(
                BinaryData::globeoutline_svg, BinaryData::globeoutline_svgSize
        );
        const DrawablePtr globeSolid = juce::Drawable::createFromImageData(
                BinaryData::globesolid_svg, BinaryData::globesolid_svgSize
        );

        private:
        Icons() = default;
        ~Icons() = default;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Icons)
    };
}