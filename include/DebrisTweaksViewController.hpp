#include "questui/shared/BeatSaberUI.hpp"
#include "custom-types/shared/macros.hpp"
#include "questui/shared/CustomTypes/Components/Settings/IncrementSetting.hpp"
#include "HMUI/ViewController.hpp"

DECLARE_CLASS_CODEGEN(DebrisTweaks, DebrisTweaksViewController, HMUI::ViewController,

    // Parented to ViewController
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Toggle*, masterEnabledToggle);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::VerticalLayoutGroup*, toggleoptions);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::VerticalLayoutGroup*, incrementoptions);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::VerticalLayoutGroup*, vertpholder);

    // Toggles
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Toggle*, overrideLifetimeToggle);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Toggle*, enableGravityToggle);

    // Increments
    DECLARE_INSTANCE_FIELD(QuestUI::IncrementSetting*, debrisLifetimeSetting);
    DECLARE_INSTANCE_FIELD(QuestUI::IncrementSetting*, debrisScaleSetting);
    DECLARE_INSTANCE_FIELD(QuestUI::IncrementSetting*, debrisVelMulSetting);

    // Methods
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, DidDeactivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidDeactivate", 2), bool removedFromHierarchy, bool systemScreenDisabling);

    REGISTER_FUNCTION(DebrisTweaksViewController,
        REGISTER_FIELD(debrisLifetimeSetting);
        REGISTER_FIELD(debrisScaleSetting);
        REGISTER_FIELD(debrisVelMulSetting);
        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD(DidDeactivate);
    )
)