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

class Hacks
{
public:
	static inline bool is_running_ = false;
	static inline bool is_hooking_ = false;
	static inline bool is_unlimited_health_ = false;
	static inline bool is_unlimited_vigor_ = false;
	static inline bool is_unlimited_talents_ = false;
	static inline bool is_unlimited_medallion_ = false;
	static inline bool is_unlimited_weight_ = false;
	static inline bool is_unlimited_adrenaline_ = false;
	static inline bool is_free_camera_ = false;
	static inline bool is_toxicity_ = false;
	static inline bool is_movement_changer_ = false;
	static inline bool is_fov_changer_ = false;
	static inline bool is_game_speed_changer_ = false;

	static inline std::array<uintptr_t, 2> lp_base_ = { 0,0 };
	static inline std::array<uintptr_t, 2> angle_xy = { 0,0 };

	static inline float freecamera_speed_ = 0.1f;
	static inline float movement_value_ = 1.0f;
	static inline float fov_value_ = 0.5f;
	static inline float game_speed_value = 1.0f;

	void Functions();

private:
	uintptr_t camera_coords = 0;
	uintptr_t gamespeed_addr = 0;

	bool is_temp_weight_ = false;
	bool is_temp_movement_ = false;
	bool is_temp_fov_ = false;
	bool is_temp_game_speed_changer_ = false;
	bool is_temp_free_camera_ = false;

	void UnlimitedHealth();
	void UnlimitedVigor();
	void UnlimitedTalents();
	void UnlimitedMedallion();
	void UnlimitedWeight();
	void UnlimitedAdrenaline();
	void FreeCamera();
	void MovementSpeedChanger();
	void NoToxicity();
	void FovChanger();
	void GameSpeedChanger();
};
