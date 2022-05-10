/*
* Copyright (c) 2022 Rudeus Kirigaya <https://github.com/kirizaku>
*
* This program is free software: you can redistribute itand /or modify it under the terms of the GNU
* General Public License as published by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
* even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.

* You should have received a copy of the GNU General Public License along with this program. If not, see
* <https://www.gnu.org/licenses/>.
*/

#pragma once

#include "memory/memory.h"

namespace globals {
	inline mem::mem_pid_t pid							= 0;
	inline uintptr_t process_mod						= 0;
	inline uintptr_t process_mod_hook					= 0;
	inline mem::string_t process_check					= "";
	const inline mem::string_t kProcessName				= "witcher2.exe";
	const inline mem::string_t kModuleWitcher			= "witcher2.EXE";
	const inline mem::string_t kModuleHook				= "TheWitcher2TrainerHook.dll";
}

namespace offsets {
	constexpr uintptr_t kCameraCoords					= 0x02409BA0;
	constexpr uintptr_t kLocalPlayer					= 0x022ADC4C;
	constexpr uintptr_t kGameSpeed						= 0x1004950;

	//local_player_1
	const inline std::vector<uintptr_t> kLp1_Offsets	{ 0x14, 0x14, 0x0 };

	constexpr uintptr_t kLpCurrentHealth				= 0x8;
	constexpr uintptr_t kLpMaxHealthUnchangeable		= 0x0C;
	constexpr uintptr_t kLpCurrentVigor					= 0x10;
	constexpr uintptr_t kLpMaxVigorUnchangeable			= 0x14;
	constexpr uintptr_t kLpLevel						= 0x54;
	constexpr uintptr_t kLpTalents						= 0x2B0;
	constexpr uintptr_t kLpMedallion					= 0x29C;
	constexpr uintptr_t kLpToxicity						= 0x2A4;
	constexpr uintptr_t kLpCurrentAdrenaline			= 0x2A8;

	//local_player_2
	const inline std::vector<uintptr_t> kLp2_Offsets	{ 0x14, 0x158, 0x50, 0x0 };

	constexpr uintptr_t kLpWeightLimit					= 0x2C;
	constexpr uintptr_t kLpMaxHealthChangeable			= 0x40;

	//kModuleHook
	constexpr uintptr_t kKovementSpeed					= 0x16018;
	constexpr uintptr_t kFovValue						= 0x1601C;
}