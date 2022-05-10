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

#include "language.h"
#include "TheWitcher2TrainerExternal/menu.h"

void Language::LangEng() noexcept
{
    kUnlimitedHealthLabel           = "Unlimited Health";
    KUnlimitedVigorLabel            = "Unlimited Vigor";
    kUnlimitedTalentsLabel          = "Unlimited Talents";
    kUnlimitedWeightLabel           = "Unlimited Weight";
    kUnlimitedMedallionLabel        = "Unlimited Medallion";
    kUnlimitedAdrenalineLabel       = "Unlimited Adrenaline";
    kNoToxicityLabel                = "No Toxicity";
    kFreeCamera                     = "Free Camera";
    kMovementChangerLabel           = "Movement Speed";
    kFovChangerLabel                = "FOV";
    kGameSpeedChangerLabel          = "Game Speed";
    kMaxLvlLabel                    = "Maximum level";
    kWaitForGameLabel               = "Waiting for game...";
    kProcNameLabel                  = "Process Name:   %s";
    kProcIdLabel                    = "Process ID:   %i";
    kSupportProjectLabel            = "Support me on";
    kSourceCode                     = "Source code available on";
    kCheckForUpdatesLabel           = "Check for Updates at startup";
    kTurnOffOnTheSound              = "Sound";
    kThemeLabel                     = "Theme";
    kThemeComboSelector             = "Dark\0Light\0";
    kLangSelectorLabel              = "Language";
    kFunctionsLabel                 = "Functions";
    kSettingsLabel                  = "Settings";
    kAboutLabel                     = "About trainer";
    kVersionLabel                   = "Version %s";
    kReportLevel                    = "\nReport bugs or request enhancements at:";
    kAdditionalCopyrightsLabel      = "\nAdditional Copyrights:";
    kLicenseLabel                   = "License";
}

void Language::LangRus() noexcept
{
    kUnlimitedHealthLabel           = u8"�������������� ��������";
    KUnlimitedVigorLabel            = u8"�������������� �������";
    kUnlimitedTalentsLabel          = u8"�������������� ���� ��������";
    kUnlimitedWeightLabel           = u8"�������������� ���";
    kUnlimitedMedallionLabel        = u8"�������������� ��������";
    kUnlimitedAdrenalineLabel       = u8"�������������� ���������";
    kNoToxicityLabel                = u8"��� �����������";
    kFreeCamera                     = u8"��������� ������";
    kMovementChangerLabel           = u8"�������� ������������";
    kFovChangerLabel                = u8"���� ������";
    kGameSpeedChangerLabel          = u8"�������� ����";
    kMaxLvlLabel                    = u8"������������ �������";
    kWaitForGameLabel               = u8"� �������� ����...";
    kProcNameLabel                  = u8"��� ��������:   %s";
    kProcIdLabel                    = u8"ID ��������:   %i";
    kSupportProjectLabel            = u8"���������� ���� ��";
    kSourceCode                     = u8"�������� ��� �������� ��";
    kCheckForUpdatesLabel           = u8"��������� ������� ���������� ��� �������";
    kTurnOffOnTheSound              = u8"����";
    kThemeLabel                     = u8"����";
    kThemeComboSelector             = u8"������\0�������\0";
    kLangSelectorLabel              = u8"����";
    kFunctionsLabel                 = u8"�������";
    kSettingsLabel                  = u8"���������";
    kAboutLabel                     = u8"� ��������";
    kVersionLabel                   = u8"������ %s";
    kReportLevel                    = u8"\n�������� �� ������ ��� ���������� ��������� �� ���-�����:";
    kAdditionalCopyrightsLabel      = u8"\n�������������� ��������� �����:";
    kLicenseLabel                   = u8"��������";
}

bool Language::ShowLanguageSelector(const char* label)
{
    if (ImGui::Combo(label, &settings::lang_, u8"English\0�������\0"))
    {
        switch (settings::lang_)
        {
            case 0: LangEng(); break;
            case 1: LangRus(); break;
        }
        return true;
    }
    return false;
}