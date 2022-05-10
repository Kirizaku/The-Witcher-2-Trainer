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

#include "functions.h"

BOOL Hook(LPVOID src, LPVOID funct, int size) 
{
	if (size < 5) {
		return FALSE;
	}

	DWORD old_protect;
	VirtualProtect(src, size, PAGE_EXECUTE_READWRITE, &old_protect);
	memset(src, 0x90, size);
	DWORD relative_address = ((DWORD)funct - (DWORD)src) - 5;
	*(BYTE*)src = 0xE9;
	*(DWORD*)((DWORD)src + 1) = relative_address;
	VirtualProtect(src, size, old_protect, &old_protect);

	return TRUE;
}

void Patch(LPVOID src, LPCVOID dst, size_t size)
{
	DWORD old_protect;
	VirtualProtect((LPVOID)src, size, PAGE_EXECUTE_READWRITE, &old_protect);
	memcpy((LPVOID)src, (BYTE*)dst, size);
	VirtualProtect((LPVOID)src, size, old_protect, &old_protect);
}

void __declspec(naked) Hacks::EnableMovementSpeedChangerA() 
{
	__asm {
		movss xmm4, [esp+0x10]
		mulss xmm4, [movement_value_]
		jmp jmp_back_movement_a_
	}
}

void __declspec(naked) Hacks::EnableMovementSpeedChangerB() 
{
	__asm {
		movss xmm0, [esp+0x14]
		mulss xmm0, [movement_value_]
		jmp jmp_back_movement_b_
	}
}

void __declspec(naked) Hacks::EnableUnlimitedWeight() 
{
	__asm {
		mov dword ptr ds:[esp+0x14], 0x0
		fild dword ptr ds:[esp+0x14]
		jmp jmp_back_weight_
	}
}

void __declspec(naked) Hacks::EnableFovChanger() 
{
	__asm {
		mulss xmm0, [fov_value_]
		jmp jmp_back_fov_
	}
}

void Hacks::Functions() {

	char data_dir[MAX_PATH];
	HMODULE hModule = NULL;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR) & "TheWitcher2TrainerHook.dll", &hModule);
	GetModuleFileName(hModule, data_dir, sizeof(data_dir));
	std::string::size_type pos = std::string(data_dir).find_last_of("\\/");
	std::string config_dir = std::string(data_dir).substr(0, pos);
	config_dir += std::string("\\config\\hook.json");

	DWORD module_witcher = (DWORD)GetModuleHandleA("witcher2.exe");

	int movement_size = 6;
	DWORD movement_a = module_witcher + 0x6D43FE;
	DWORD movement_b = module_witcher + 0x6D442F;

	jmp_back_movement_a_ = module_witcher + 0x6D4404;
	jmp_back_movement_b_ = movement_b + movement_size;

	int weight_size = 8;
	DWORD weight = module_witcher + 0x5818E0;
	jmp_back_weight_ = weight + weight_size;

	int fov_size = 8;
	DWORD fov = module_witcher + 0x706A88;
	jmp_back_fov_ = fov + fov_size;

	bool is_temp_movement_ = false;
	bool is_temp_weight_ = false;
	bool is_temp_fov_ = false;

	json j;

	while (true)
	{
		std::ifstream l(config_dir);
		j = json::parse(l, nullptr, false, true);
		Config().ReadBool(j, "hooking", is_hooking_);
		Config().ReadBool(j, "enable_movement_changer", is_movement_changer_);
		Config().ReadBool(j, "enable_unlimited_weight", is_unlimited_weight_);
		Config().ReadBool(j, "enable_fov_changer", is_fov_changer_);

		if (!is_hooking_) break;

		if (is_movement_changer_ && !is_temp_movement_) {
			Hook((LPVOID)movement_a, EnableMovementSpeedChangerA, movement_size);
			Hook((LPVOID)movement_b, EnableMovementSpeedChangerB, movement_size);
			is_temp_movement_ = true;
		}
		else if (!is_movement_changer_ && is_temp_movement_) {
			Patch((LPVOID)movement_a, "\xF3\x0F\x10\x64\x24\x10", movement_size);
			Patch((LPVOID)movement_b, "\xF3\x0F\x10\x44\x24\x14", movement_size);
			is_temp_movement_ = false;
		}

		if (is_unlimited_weight_ && !is_temp_weight_) {
			Hook((LPVOID)weight, EnableUnlimitedWeight, weight_size);
			is_temp_weight_ = true;
		}
		else if (!is_unlimited_weight_ && is_temp_weight_) {
			Patch((LPVOID)weight, "\x89\x4C\x24\x14\xDB\x44\x24\x14", weight_size);
			is_temp_weight_ = false;
		}

		if (is_fov_changer_ && !is_temp_fov_) {
			Hook((LPVOID)fov, EnableFovChanger, fov_size);
			is_temp_fov_ = true;
		}
		else if (!is_fov_changer_ && is_temp_fov_) {
			Patch((LPVOID)fov, "\xF3\x0F\x59\x05\x44\x04\xE3\x00", fov_size);
			is_temp_fov_ = false;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	Patch((LPVOID)movement_a, "\xF3\x0F\x10\x64\x24\x10", movement_size);
	Patch((LPVOID)movement_b, "\xF3\x0F\x10\x44\x24\x14", movement_size);
	Patch((LPVOID)weight, "\x89\x4C\x24\x14\xDB\x44\x24\x14", weight_size);
	Patch((LPVOID)fov, "\xF3\x0F\x59\x05\x44\x04\xE3\x00", fov_size);
}