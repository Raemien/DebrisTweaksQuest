#include "main.hpp"

#include "questui/shared/CustomTypes/Components/Settings/IncrementSetting.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "custom-types/shared/macros.hpp"
#include "HMUI/FlowCoordinator.hpp"
#include "HMUI/ViewController.hpp"

DECLARE_CLASS_CODEGEN(DebrisTweaks, DebrisTweaksFlowCoordinator, HMUI::FlowCoordinator,
    DECLARE_INSTANCE_FIELD(HMUI::ViewController*, dtMainView);
    DECLARE_INSTANCE_FIELD(HMUI::ViewController*, dtPhysicsView);
    DECLARE_INSTANCE_FIELD(HMUI::ViewController*, dtCosmeticView);

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "FlowCoordinator", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, BackButtonWasPressed, il2cpp_utils::FindMethodUnsafe("HMUI", "FlowCoordinator", "BackButtonWasPressed", 1), HMUI::ViewController* topViewController);
    DECLARE_INSTANCE_METHOD(void, ReloadUIElements);
    DECLARE_INSTANCE_METHOD(void, UpdateSettingsVisibility, bool);
)