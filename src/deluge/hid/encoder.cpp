/*
 * Copyright © 2014-2023 Synthstrom Audible Limited
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

#include "hid/encoder.h"
#include "processing/engines/audio_engine.h"

using deluge::hid::encoders::Encoder;

extern "C" {
#include "RZA1/gpio/gpio.h"
}

Encoder::Encoder() {
	encPos = 0;
	detentPos = 0;
	doDetents = true;
}

void Encoder::setPins(uint8_t pinA1New, uint8_t pinA2New, uint8_t pinB1New, uint8_t pinB2New) {
	setPinAsInput(pinA1New, pinA2New);
	setPinAsInput(pinB1New, pinB2New);
}

void Encoder::setNonDetentMode() {
	doDetents = false;
}

void Encoder::applyEdges(int16_t edges) {
	if (edges == 0) {
		return;
	}
	if (doDetents) {
		detentPos += edges;
	}
	encPos += edges;
}

int32_t Encoder::getLimitedDetentPosAndReset() {
	int32_t toReturn = (detentPos >= 0) ? 1 : -1;
	detentPos = 0;
	return toReturn;
}
