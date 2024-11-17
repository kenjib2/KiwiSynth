#include "SettingsScreen5.h"

namespace kiwisynth
{

    void SettingsScreen5::Init(KiwiDisplay* display, Patch* patch)
    {
        this->display = display;
        this->patch = patch;
        voiceNumber = 0;
    }

    void SettingsScreen5::Display()
    {
        int intVal1, intVal2;
        bool boolVal1;
        PatchSettings* settings;
        VoiceMode voiceMode;

        voiceMode = (VoiceMode)patch->voice1Settings_->getIntValue(OSC_VOICES);
        if (voiceNumber == 0 || (voiceMode != VOICE_MODE_MULTI && voiceMode != VOICE_MODE_SPLIT)) {
            settings = patch->voice1Settings_;
        } else {
            settings = patch->voice2Settings_;
        }

        display->Fill(false);

        display->SetCursor(0, 0);
        if (voiceMode == VOICE_MODE_MULTI || voiceMode == VOICE_MODE_SPLIT) {
            sprintf(buffer, "Envelope 1-------- V%d", voiceNumber + 1);
        } else {
            sprintf(buffer, "Envelope 1-----------");
        }
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 8);
        intVal1 = settings->getFloatValue(ENV_1_ATTACK) * 1000;
        intVal2 = settings->getFloatValue(ENV_1_DECAY) * 1000;
        sprintf(buffer, "Attack %3d  Decay %3d", intVal1, intVal2);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 16);
        intVal1 = settings->getFloatValue(ENV_1_SUSTAIN) * 1000;
        intVal2 = settings->getFloatValue(ENV_1_RELEASE) * 1000;
        sprintf(buffer, "Sustn %3d  Releas %3d", intVal1, intVal2);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 24);
        boolVal1 = settings->getBoolValue(ENV_1_REVERSE_PHASE_ON);
        sprintf(buffer, "Inverted %s", boolVal1 ? "On" : "Off");
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 32);
        sprintf(buffer, "Envelope 2-----------");
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 40);
        intVal1 = settings->getFloatValue(ENV_2_ATTACK) * 1000;
        intVal2 = settings->getFloatValue(ENV_2_DECAY) * 1000;
        sprintf(buffer, "Attack %3d  Decay %3d", intVal1, intVal2);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 48);
        intVal1 = settings->getFloatValue(ENV_2_SUSTAIN) * 1000;
        intVal2 = settings->getFloatValue(ENV_2_RELEASE) * 1000;
        sprintf(buffer, "Sustn %3d  Releas %3d", intVal1, intVal2);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 56);
        boolVal1 = settings->getBoolValue(ENV_2_REVERSE_PHASE_ON);
        sprintf(buffer, "Inverted %s", boolVal1 ? "On" : "Off");
        display->WriteString(buffer, Font_6x8, true);

        display->Update();
    }

    void SettingsScreen5::Click() {
        if (patch->GetVoiceMode() == VOICE_MODE_MULTI || patch->GetVoiceMode() == VOICE_MODE_SPLIT) {
            voiceNumber = voiceNumber ^ 1;
        } else {
            voiceNumber = 0;
        }
    }

} // namespace kiwisynth
