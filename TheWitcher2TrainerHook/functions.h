/*
* Copyright (c) 2022 Rudeus Kirigaya <https://github.com/kirizaku>
*
* This file is part of TheWitcher2TrainerExternal.
* TheWitcher2TrainerExternal is free software: you can redistribute it and/or modify it under the terms of the GNU General
* Public License as published by the Free Software Foundation, either version 3 of the License, or (at your
* option) any later version.
*
* TheWitcher2TrainerExternal is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
* the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with TheWitcher2TrainerExternal. If not, see
<https://www.gnu.org/licenses/>.
*/

#pragma once

#include <Windows.h>
#include <thread>
#include <string>
#include "config.h"

class Hacks
{
public:
	void Functions();

private:
	static inline DWORD jmp_back_movement_a_;
	static inline DWORD jmp_back_movement_b_;
	static inline DWORD jmp_back_weight_;
	static inline DWORD jmp_back_fov_;

	static inline float fov_value_ = 0.5f;
	static inline float movement_value_ = 1.0f;

	static inline bool is_hooking_ = false;
	static inline bool is_movement_changer_ = false;
	static inline bool is_unlimited_weight_ = false;
	static inline bool is_fov_changer_ = false;

	static void EnableMovementSpeedChangerA();
	static void EnableMovementSpeedChangerB();
	static void EnableUnlimitedWeight();
	static void EnableFovChanger();
};

