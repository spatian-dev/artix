/*
  ==============================================================================

    Metrics.h
    Created: 28 Jun 2024 6:22:31pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

namespace Artix::Ui {
    constexpr float BASE = 10;

    namespace FontSize {
        constexpr float Tiny = BASE * 0.9;
        constexpr float Small = BASE * 1.2;
        constexpr float Medium = BASE * 1.5;
        constexpr float Large = BASE * 1.8;
        constexpr float Huge = BASE * 2.1;
    }

    namespace Spacing {
        constexpr float Small = BASE / 2;
        constexpr float Medium = BASE;
        constexpr float Large = BASE * 2;
    }

    namespace Thickness {
        constexpr float Tiny = BASE / 10;
        constexpr float Small = BASE / 5;
        constexpr float Medium = 3 * (BASE / 10);
        constexpr float Large = 4 * (BASE / 10);
        constexpr float Huge = BASE / 2;
    }

    namespace Rounding {
        constexpr float Tiny = BASE / 2;
        constexpr float Small = BASE;
        constexpr float Medium = 3 * (BASE / 2);
        constexpr float Large = BASE * 2;
        constexpr float Huge = 5 * (BASE * 2);
    }
}