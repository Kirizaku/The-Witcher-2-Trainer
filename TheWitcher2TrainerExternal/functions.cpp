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

#include "menu.h"
#include "config/config.h"

void Hacks::Functions()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (is_running_)
        {     
            lp_base_[0] = mem::read_pointer(globals::pid, globals::process_mod + offsets::kLocalPlayer, offsets::kLp1_Offsets);
            lp_base_[1] = mem::read_pointer(globals::pid, globals::process_mod + offsets::kLocalPlayer, offsets::kLp2_Offsets);

            gamespeed_addr = mem::read_pointer(globals::pid, globals::process_mod + offsets::kGameSpeed, std::vector<uintptr_t> { 0xA0 });

            uintptr_t read_viger = 0;
            mem::read(globals::pid, (void*)((uintptr_t)lp_base_[0] + offsets::kLpMaxVigorUnchangeable), &read_viger, sizeof(read_viger));

            if (!read_viger) {
                angle_xy[0] = 0;
                continue;
            }

            UnlimitedHealth();
            UnlimitedVigor();
            UnlimitedTalents();
            UnlimitedMedallion();
            UnlimitedAdrenaline();
            FreeCamera();
            //NoToxicity();
            GameSpeedChanger();

            if (!globals::process_mod_hook)
                continue;

            UnlimitedWeight();
            MovementSpeedChanger();
            FovChanger();
        }
    }
}

void Hacks::UnlimitedHealth()
{
    float write_health = 9999.0f;
    if (is_unlimited_health_) {
        mem::write(globals::pid, (void*)((uintptr_t)lp_base_[1] + offsets::kLpMaxHealthChangeable), &write_health, sizeof(write_health));
        mem::write(globals::pid, (void*)((uintptr_t)lp_base_[0] + offsets::kLpCurrentHealth), &write_health, sizeof(write_health));
    }
}

void Hacks::UnlimitedVigor()
{
    float readwrite_viger = 0;
    if (is_unlimited_vigor_) {
        mem::read(globals::pid, (void*)((uintptr_t)lp_base_[0] + offsets::kLpMaxVigorUnchangeable), &readwrite_viger, sizeof(readwrite_viger));
        mem::write(globals::pid, (void*)((uintptr_t)lp_base_[0] + offsets::kLpCurrentVigor), &readwrite_viger, sizeof(readwrite_viger));
    }
}

void Hacks::UnlimitedTalents()
{
    int write_talents = 50;
    if (is_unlimited_talents_) {
        mem::write(globals::pid, (void*)((uintptr_t)lp_base_[0] + offsets::kLpTalents), &write_talents, sizeof(write_talents));
    }
}

void Hacks::UnlimitedMedallion()
{
    byte write_medallion = 1;
    if (is_unlimited_medallion_) {
        mem::write(globals::pid, (void*)((uintptr_t)lp_base_[0] + offsets::kLpMedallion), &write_medallion, sizeof(write_medallion));
    }
}

void Hacks::UnlimitedAdrenaline()
{
    float write_adrenaline = 200.0f;
    if (is_unlimited_adrenaline_) {
        mem::write(globals::pid, (void*)((uintptr_t)lp_base_[0] + offsets::kLpCurrentAdrenaline), &write_adrenaline, sizeof(write_adrenaline));
    }
}

void Hacks::FreeCamera()
{
    if (angle_xy[0] == 0) 
    {
        angle_xy[0] = mem::aob_scan(globals::pid, 
            "\x32\x00\x00\x00\x00\x00\x00\x00\x00\x00\x22\x22\x00\x00\x00\x00\x2B\x00\x00\x00\x36\x00\x00\x00\x01\x00\x00\x00\x22\x22\x22\x00\x00\x00\x00\x00\x00\x00\x80\x3F\x22\x22\x22\x22\x01",
            //32 00 00 00 00 00 00 00 00 00 ? ? 00 00 00 00 2B 00 00 00 36 00 00 00 01 00 00 00 ? ? ? 00 00 00 00 00 00 00 80 3F ? ? ? ? 01
            "xxxxxxxxxx??xxxxxxxxxxxxxxxx???xxxxxxxxx????x", (char*)0x0, (char*)0x7FFFFFFF);
        angle_xy[0] = angle_xy[0] + 40;
        angle_xy[1] = angle_xy[0] - 60;
    }

    if (is_free_camera_) 
    {
        float write_gamespeed = 0.000001f;
        mem::write(globals::pid, (void*)((uintptr_t)gamespeed_addr), &write_gamespeed, sizeof(write_gamespeed));

        mem::nop_ex(globals::pid, (void*)((uintptr_t)globals::process_mod + 0x707055), 4);
        mem::nop_ex(globals::pid, (void*)((uintptr_t)globals::process_mod + 0x707059), 5);
        mem::nop_ex(globals::pid, (void*)((uintptr_t)globals::process_mod + 0x70705E), 5);
        mem::nop_ex(globals::pid, (void*)((uintptr_t)globals::process_mod + 0x70720B), 2);
        mem::nop_ex(globals::pid, (void*)((uintptr_t)globals::process_mod + 0x707211), 3);
        mem::nop_ex(globals::pid, (void*)((uintptr_t)globals::process_mod + 0x70721A), 3);

        float cam_x, cam_y, cam_z;
        float angle_x, angle_y;
        float PI = 3.141592f;

        mem::read(globals::pid, (void*)((uintptr_t)angle_xy[0]), &angle_x, sizeof(angle_x));
        mem::read(globals::pid, (void*)((uintptr_t)angle_xy[1]), &angle_y, sizeof(angle_y));

        mem::read(globals::pid, (void*)((uintptr_t)globals::process_mod + offsets::kCameraCoords), &camera_coords, sizeof(camera_coords));
        mem::read(globals::pid, (void*)((uintptr_t)camera_coords + 0x420), &cam_x, sizeof(cam_x));
        mem::read(globals::pid, (void*)((uintptr_t)camera_coords + 0x424), &cam_y, sizeof(cam_y));
        mem::read(globals::pid, (void*)((uintptr_t)camera_coords + 0x428), &cam_z, sizeof(cam_z));

        //W
        if (GetAsyncKeyState(0x57)) 
        {
            cam_x += sin(angle_x / -180 * PI) * freecamera_speed_;
            cam_y += cos(angle_x / -180 * PI) * freecamera_speed_;
            cam_z += -tan(angle_y / -180 * PI) * freecamera_speed_;

            mem::write(globals::pid, (void*)((uintptr_t)camera_coords + 0x420), &cam_x, sizeof(cam_x));
            mem::write(globals::pid, (void*)((uintptr_t)camera_coords + 0x424), &cam_y, sizeof(cam_y));
            mem::write(globals::pid, (void*)((uintptr_t)camera_coords + 0x428), &cam_z, sizeof(cam_z));
        }

        //S
        if (GetAsyncKeyState(0x53)) 
        {
            cam_x += -sin(angle_x / -180 * PI) * freecamera_speed_;
            cam_y += -cos(angle_x / -180 * PI) * freecamera_speed_;
            cam_z += tan(angle_y / -180 * PI) * freecamera_speed_;

            mem::write(globals::pid, (void*)((uintptr_t)camera_coords + 0x420), &cam_x, sizeof(cam_x));
            mem::write(globals::pid, (void*)((uintptr_t)camera_coords + 0x424), &cam_y, sizeof(cam_y));
            mem::write(globals::pid, (void*)((uintptr_t)camera_coords + 0x428), &cam_z, sizeof(cam_z));
        }

        //A
        if (GetAsyncKeyState(0x41)) 
        {
            cam_x += sin((angle_x + 90) / -180 * PI) * freecamera_speed_;
            cam_y += cos((angle_x + 90) / -180 * PI) * freecamera_speed_;

            mem::write(globals::pid, (void*)((uintptr_t)camera_coords + 0x420), &cam_x, sizeof(cam_x));
            mem::write(globals::pid, (void*)((uintptr_t)camera_coords + 0x424), &cam_y, sizeof(cam_y));
        }

        //D
        if (GetAsyncKeyState(0x44)) 
        {
            cam_x += sin((angle_x - 90) / -180 * PI) * freecamera_speed_;
            cam_y += cos((angle_x - 90) / -180 * PI) * freecamera_speed_;

            mem::write(globals::pid, (void*)((uintptr_t)camera_coords + 0x420), &cam_x, sizeof(cam_x));
            mem::write(globals::pid, (void*)((uintptr_t)camera_coords + 0x424), &cam_y, sizeof(cam_y));
        }
        is_temp_free_camera_ = true;
    }
    else if (!is_free_camera_ && is_temp_free_camera_) 
    {
        float write_gamespeed = 1.0f;
        mem::write(globals::pid, (void*)((uintptr_t)gamespeed_addr), &write_gamespeed, sizeof(write_gamespeed));

        std::vector<std::vector<uint8_t>> cam_write_bytes = {
        { 0xF3, 0x0F, 0x11, 0x06 },
        { 0xF3, 0x0F, 0x11, 0x46, 0x04 },
        { 0xF3, 0x0F, 0x11, 0x46, 0x08 },
        { 0xD9, 0x1B },
        { 0xD9, 0x5B, 0x04 },
        { 0xD9, 0x5B, 0x08 }
        };

        mem::write(globals::pid, (void*)((uintptr_t)globals::process_mod + 0x707055), cam_write_bytes[0].data(), 4);
        mem::write(globals::pid, (void*)((uintptr_t)globals::process_mod + 0x707059), cam_write_bytes[1].data(), 5);
        mem::write(globals::pid, (void*)((uintptr_t)globals::process_mod + 0x70705E), cam_write_bytes[2].data(), 5);
        mem::write(globals::pid, (void*)((uintptr_t)globals::process_mod + 0x70720B), cam_write_bytes[3].data(), 2);
        mem::write(globals::pid, (void*)((uintptr_t)globals::process_mod + 0x707211), cam_write_bytes[4].data(), 3);
        mem::write(globals::pid, (void*)((uintptr_t)globals::process_mod + 0x70721A), cam_write_bytes[5].data(), 3);

        is_temp_free_camera_ = false;
    }
}

//void Hacks::NoToxicity()
//{
//    float write_toxicity = 0.0f;
//    if (is_toxicity_) {
//        mem::write(globals::pid, (void*)((uintptr_t)lp_base_[0] + offsets::kLpToxicity), &write_toxicity, sizeof(write_toxicity));
//    }
//}

void Hacks::GameSpeedChanger()
{
    if (is_game_speed_changer_)
    {
        mem::write(globals::pid, (void*)((uintptr_t)gamespeed_addr), &game_speed_value, sizeof(game_speed_value));
        is_temp_game_speed_changer_ = true;
    }
    else if (!is_game_speed_changer_ && is_temp_game_speed_changer_) 
    {
        float game_speed_value = 1.0f;
        mem::write(globals::pid, (void*)((uintptr_t)gamespeed_addr), &game_speed_value, sizeof(game_speed_value));
        is_temp_game_speed_changer_ = false;
    }
}

/*process_mod_hook*/

void Hacks::UnlimitedWeight()
{
    if (is_unlimited_weight_ && !is_temp_weight_) { Config().SaveHookConfig(); is_temp_weight_ = true; }
    else if (!is_unlimited_weight_ && is_temp_weight_) { Config().SaveHookConfig(); is_temp_weight_ = false; }
}

void Hacks::MovementSpeedChanger()
{
    if (is_movement_changer_) {
        if (!is_temp_movement_) { Config().SaveHookConfig(); is_temp_movement_ = true; }
        mem::write(globals::pid, (void*)((uintptr_t)globals::process_mod_hook + offsets::kKovementSpeed), &movement_value_, sizeof(movement_value_));
    }
    else if (!is_movement_changer_ && is_temp_movement_) { Config().SaveHookConfig(); is_temp_movement_ = false; }
}

void Hacks::FovChanger()
{
    if (is_fov_changer_) {
        if (!is_temp_fov_) { Config().SaveHookConfig(); is_temp_fov_ = true; }
        mem::write(globals::pid, (void*)((uintptr_t)globals::process_mod_hook + offsets::kFovValue), &fov_value_, sizeof(fov_value_));
    }
    else if (!is_fov_changer_ && is_temp_fov_) { Config().SaveHookConfig(); is_temp_fov_ = false; }
}