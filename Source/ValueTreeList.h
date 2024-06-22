/*
  ==============================================================================

	ValueTreeList.h
	Created: 22 Jun 2024 10:39:05am
	Author:  Saad Sidqui

	Inspired heavily by David Rowland's ValueTreeObjectList.
	@see https://github.com/drowaudio

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace Artix::Util {
	template<typename T>
	concept Stateful = requires(T t) {
		{ t.getState()  } -> std::same_as<juce::ValueTree>;
	};

	template<Stateful T, typename Mutex = juce::CriticalSection>
	class ValueTreeList : public juce::ValueTree::Listener {
		using ScopedLockType = Mutex::ScopedLockType;

		public:
		ValueTreeList(const juce::ValueTree& parent) : parent(parent) {
			this->parent.addListener(this);
			buildItems();
		}

		virtual ~ValueTreeList() {
			jassert(items.size() == 0);
			parent.removeListener(this);
			freeItems();
		}

		virtual bool isSuitable(const juce::ValueTree&) const = 0;
		virtual T* createItem(const juce::ValueTree&) = 0;
		virtual void deleteItem(T*) = 0;

		virtual void onItemAdded(T*) = 0;
		virtual void onItemRemoved(T*) = 0;
		virtual void onItemsOrderChanged() = 0;
		virtual void onListRedirected() = 0;

		int compareElements(T* a, T* b) const {
			return parent.indexOf(a->getState()) - parent.indexOf(b->getState());
		}

		int indexOf(const juce::ValueTree& v) const noexcept {
			for (int i = 0; i < items.size(); ++i) {
				if (items.getUnchecked(i)->getState() == v) {
					return i;
				}
			}
			return -1;
		}

		void sort() {
			{
				const ScopedLockType lock(mutex);
				items.sort(*this);
			}
			onItemsOrderChanged();
		}

		protected:
		juce::ValueTree parent;
		juce::Array<T*, Mutex> items;
		Mutex mutex;

		void buildItems() {
			jassert(items.size() == 0);
			ScopedLockType lock(mutex);
			for (const auto& v : parent) {
				if (!isSuitable(v)) {
					continue;
				}

				T* item = createItem(v);
				if (item != nullptr) {
					items.add(item);
				}
			}
		}

		void freeItems() {
			ScopedLockType lock(mutex);
			while (items.size() > 0) {
				deleteItem(items.removeAndReturn(items.size() - 1));
			}
		}

		bool isChildTree(juce::ValueTree& v) const {
			return isSuitable(v) && (v.getParent() == parent);
		}

		void valueTreeChildAdded(juce::ValueTree&, juce::ValueTree& v) override {
			if (!isChildTree(v)) {
				return;
			}

			const int index = parent.indexOf(v);
			jassert(index >= 0);

			T* item = createItem(v);
			if (item == nullptr) {
				jassertfalse;
				return;
			}

			{
				const ScopedLockType lock(mutex);

				if (index == parent.getNumChildren() - 1) {
					items.add(item);
				} else {
					items.addSorted(*this, item);
				}
			}
			onItemAdded(item);
		}

		void valueTreeChildRemoved(juce::ValueTree& exParent, juce::ValueTree& v, int) override {
			if ((parent == exParent) && isSuitable(v)) {
				return;
			}

			const int oldIndex = indexOf(v);
			if (oldIndex >= 0) {
				T* item;
				{
					const ScopedLockType lock(mutex);
					item = items.removeAndReturn(oldIndex);
				}
				onItemRemoved(item);
				deleteItem(item);
			}
		}

		void valueTreeChildOrderChanged(juce::ValueTree& v, int, int) override {
			if (v == parent) {
				sort();
			}
		}

		void valueTreeRedirected(juce::ValueTree& newParent) override {
			// Candidate implementation needs more testing
			jassertfalse;
			/*ScopedLockType lock(mutex);

			parent.removeListener(this);
			while (items.size() > 0) {
				items.remove(items.size() - 1);
			}

			parent = newParent;

			parent.addListener(this);
			for (const auto& v : parent) {
				if (!isSuitable(v)) {
					continue;
				}

				T* item = createItem(v);
				if (item != nullptr) {
					items.add(item);
				}
			}
			onListRedirected();*/
		}

		void valueTreePropertyChanged(juce::ValueTree&, const juce::Identifier&) override {}

		void valueTreeParentChanged(juce::ValueTree&) override {}

		private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueTreeList)
	};
}