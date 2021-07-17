#include "questui/shared/BeatSaberUI.hpp"
#include "custom-types/shared/macros.hpp"
#include "questui/shared/CustomTypes/Components/Settings/IncrementSetting.hpp"
#include "HMUI/ViewController.hpp"

DECLARE_CLASS_CODEGEN(DebrisTweaks, DebrisTweaksCosmeticView, HMUI::ViewController,

    // Parented to ViewController
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Toggle*, masterEnabledToggle);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::VerticalLayoutGroup*, container);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::VerticalLayoutGroup*, configcontainer);

    // Increments
    DECLARE_INSTANCE_FIELD(QuestUI::IncrementSetting*, debrisLifetimeSetting);
    DECLARE_INSTANCE_FIELD(QuestUI::IncrementSetting*, debrisScaleSetting);

    // Toggles
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Toggle*, overrideLifetimeToggle);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Toggle*, monochromeToggle);

    // Methods
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, DidDeactivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidDeactivate", 2), bool removedFromHierarchy, bool systemScreenDisabling);
    DECLARE_METHOD(void, ReloadUIValues);
    DECLARE_METHOD(void, OnChangeVisibility, bool);

    REGISTER_FUNCTION(
        REGISTER_FIELD(debrisLifetimeSetting);
        REGISTER_FIELD(debrisScaleSetting);
        REGISTER_METHOD(ReloadUIValues);
        REGISTER_METHOD(OnChangeVisibility);
        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD(DidDeactivate);
    )
)