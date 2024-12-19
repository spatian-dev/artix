/*
  ==============================================================================

    Listener.h
    Created: 13 Jul 2024 3:36:33pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <cstdint>
#include <functional>
#include <limits>
#include <map>
#include <optional>
#include <stdexcept>
#include <vector>

namespace Artix::Utils {
    template <
        typename Argument, typename CriticalSection = juce::DummyCriticalSection, typename Key = uint8_t
    > requires std::is_integral_v<Key>
        class CallbackList {
            public:
            using Identifier = std::optional<Key>;
            using Callback = std::function<void(const Argument)>;

            CallbackList() = default;
            virtual ~CallbackList() = default;

            virtual std::optional<Key> add(const Callback& cb, bool throws = false) {
                ScopedLock lock(mutex);

                const bool indexAtMax = (index == KLimits::max());
                const bool hasVacancy = (vacancies.size() > 0);

                if (indexAtMax && !hasVacancy) {
                    if (throws)
                        throw std::length_error("Maximum storage capacity reached");

                    return {};
                }

                Key i;
                if (hasVacancy) {
                    i = vacancies.back();
                    vacancies.pop_back();
                } else {
                    // indexAtMax should always be false at this point
                    jassert(!indexAtMax);
                    i = ++index;
                }
                jassert(!callbacks.contains(i));
                callbacks.emplace(i, cb);

                return i;
            }

            virtual bool remove(const Key i, bool throws = false) {
                ScopedLock lock(mutex);

                if (!callbacks.contains(i)) {
                    if (throws)
                        throw std::out_of_range("No value exists for the given key");
                    return false;
                }

                vacancies.push_back(i);
                callbacks.extract(i);
                return true;
            }

            virtual bool contains(const Key i) const {
                ScopedLock lock(mutex);
                return callbacks.contains(i);
            }

            virtual inline void callOnMessageThread(const Argument v) {
                ScopedLock lock(mutex);
                juce::MessageManager::callAsync([callbacks = callbacks, v]() {
                    for (const auto& cb : callbacks) {
                        cb.second(v);
                    }
                });
            }

            /* WARNING: This is only synchronous if called from the message thread. */
            virtual inline void callSafely(const Argument v) {
                const auto messageManager = juce::MessageManager::getInstance();
                ScopedLock lock(mutex);

                if (messageManager->isThisTheMessageThread()) {
                    for (const auto& cb : callbacks) {
                        cb.second(v);
                    }
                } else {
                    callOnMessageThread(v);
                }
            }

            private:
            using KLimits = std::numeric_limits<Key>;
            using ScopedLock = CriticalSection::ScopedLockType;

            Key index = 0;
            CriticalSection mutex;
            std::vector<Key> vacancies;
            std::map<Key, Callback> callbacks;

            
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CallbackList)
    };
}