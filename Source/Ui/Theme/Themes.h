/*
  ==============================================================================

    Themes.h
    Created: 8 Jul 2024 6:56:01pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <map>
#include <memory>

#include "BaseTheme.h"
#include "PicoDark.h"
#include "PicoLight.h"

namespace Artix::Ui::Theme {
    using ThemePtr = std::shared_ptr<BaseTheme>;
    using ThemeMap = std::map<juce::String, ThemePtr>;

    class Themes final {
        public:
        static Themes& getInstance() {
            static Themes instance;
            jassert(instance.themes.size() > 0);
            return instance;
        }

        ThemeMap::mapped_type& tryFind(
            const ThemeMap::key_type& k, ThemeMap::mapped_type& fallback
        ) noexcept {
            return themes.contains(k) ? themes[k] : fallback;
        }

        //======================================================================
        /** Mirroring std::map container functions */
        inline ThemeMap::mapped_type& at(const ThemeMap::key_type& k) {
            return themes.at(k);
        }
        inline const ThemeMap::mapped_type& at(const ThemeMap::key_type& k) const {
            return themes.at(k);
        }

        inline ThemeMap::mapped_type& operator[](const ThemeMap::key_type& k) {
            return themes.operator[](k);
        }
        inline ThemeMap::mapped_type& operator[](ThemeMap::key_type&& k) {
            return themes.operator[](k);
        }

        inline ThemeMap::iterator begin() noexcept {
            return themes.begin();
        }
        inline ThemeMap::const_iterator begin() const noexcept {
            return themes.begin();
        }
        inline ThemeMap::const_iterator cbegin() const noexcept {
            return themes.cbegin();
        }

        inline ThemeMap::iterator end() noexcept {
            return themes.end();
        }
        inline ThemeMap::const_iterator end() const noexcept {
            return themes.end();
        }
        inline ThemeMap::const_iterator cend() const noexcept {
            return themes.cend();
        }

        inline ThemeMap::iterator find(const ThemeMap::key_type& k) noexcept {
            return themes.find(k);
        }
        inline ThemeMap::const_iterator find(const ThemeMap::key_type& k) const noexcept {
            return themes.find(k);
        }

        inline bool contains(const ThemeMap::key_type& k) const noexcept {
            return themes.contains(k);
        }

        inline bool empty() const noexcept {
            return themes.empty();
        }
        inline ThemeMap::size_type size() const noexcept {
            return themes.size();
        }
        //======================================================================

        private:
        Themes() {
            ThemePtr theme = std::make_shared<PicoLight>();
            themes[theme->getName()] = theme;

            theme = std::make_shared<PicoDark>();
            themes[theme->getName()] = theme;
        };
        ~Themes() {}
        
        ThemeMap themes;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Themes)
    };

    class Themable {
        public:
        virtual ThemePtr getTheme() const noexcept {
            return theme;
        }

        virtual void setTheme(ThemePtr v) noexcept {
            theme = v;
        }

        protected:
        Themable(ThemePtr theme) : theme(theme) {}
        ~Themable() = default;
        ThemePtr theme;

        private:        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Themable)
    };
}