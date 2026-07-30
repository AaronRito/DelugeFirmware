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
#include "gui/menu_item/integer.h"
#include "gui/ui/sound_editor.h"
#include "model/clip/instrument_clip.h"
#include "model/instrument/kit.h"
#include "model/song/song.h"

namespace deluge::gui::menu_item::midi::kit {

// Sets the base MIDI note used by Kit::noteOnPreKitArp (note = baseNote + drumIndex).
// Hybrid storage, matching OutputMidiChannel: the per-clip current value lives on
// InstrumentClip (kitMidiOutBaseNote), and the preset default lives on Kit
// (outputMidiBaseNote). Writes update both — clip for immediate effect, Kit so the preset
// captures the user's intent when saved.
class OutputBaseNote final : public Integer {
public:
	using Integer::Integer;
	[[nodiscard]] int32_t getMinValue() const override { return 0; }
	[[nodiscard]] int32_t getMaxValue() const override { return 127; }

	bool isRelevant(ModControllableAudio* /*modControllable*/, int32_t /*whichThing*/) override {
		return soundEditor.editingKitAffectEntire();
	}

	void readCurrentValue() override {
		InstrumentClip* clip = getCurrentInstrumentClip();
		this->setValue(clip ? clip->kitMidiOutBaseNote : 36);
	}

	void writeCurrentValue() override {
		InstrumentClip* clip = getCurrentInstrumentClip();
		if (!clip) {
			return;
		}
		uint8_t newBaseNote = (uint8_t)this->getValue();
		clip->kitMidiOutBaseNote = newBaseNote;
		// Mirror to Kit so the preset save path captures the user's current intent.
		if (clip->output && clip->output->type == OutputType::KIT) {
			static_cast<Kit*>(clip->output)->outputMidiBaseNote = newBaseNote;
		}
	}
};

} // namespace deluge::gui::menu_item::midi::kit
