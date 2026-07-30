/*
 * Copyright (c) 2014-2023 Synthstrom Audible Limited
 *
 * This file is part of The Synthstrom Audible Deluge Firmware.
 *
 * The Synthstrom Audible Deluge Firmware is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once
#include "definitions_cxx.hpp"
#include "gui/l10n/l10n.h"
#include "gui/menu_item/menu_item.h"
#include "gui/ui/sound_editor.h"
#include "hid/display/display.h"
#include "model/clip/instrument_clip.h"
#include "model/song/song.h"

namespace deluge::gui::menu_item::midi::kit {

// One-shot action: copies the active clip's kitMidiOutChannel and kitMidiOutBaseNote to every other
// session clip belonging to the same Output (= every other clip in that grid column).
class ApplyToColumn final : public MenuItem {
public:
	using MenuItem::MenuItem;

	bool shouldEnterSubmenu() override { return false; }

	bool isRelevant(ModControllableAudio* /*modControllable*/, int32_t /*whichThing*/) override {
		return soundEditor.editingKitAffectEntire();
	}

	MenuItem* selectButtonPress() override {
		InstrumentClip* source = getCurrentInstrumentClip();
		if (source == nullptr) {
			return NO_NAVIGATION;
		}

		Output* srcOutput = source->output;
		uint8_t srcChannel = source->kitMidiOutChannel;
		uint8_t srcBaseNote = source->kitMidiOutBaseNote;

		for (int32_t idx = 0; idx < currentSong->sessionClips.getNumElements(); ++idx) {
			Clip* clip = currentSong->sessionClips.getClipAtIndex(idx);
			if (clip == source || clip->output != srcOutput) {
				continue;
			}
			auto* target = static_cast<InstrumentClip*>(clip);
			target->kitMidiOutChannel = srcChannel;
			target->kitMidiOutBaseNote = srcBaseNote;
		}

		display->displayPopup(deluge::l10n::get(deluge::l10n::String::STRING_FOR_APPLIED));
		return NO_NAVIGATION;
	}
};

} // namespace deluge::gui::menu_item::midi::kit
