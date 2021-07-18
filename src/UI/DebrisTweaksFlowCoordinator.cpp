#include "main.hpp"
#include "DTConfig.hpp"
#include "UI/SharedUIManager.hpp"
#include "UI/DebrisTweaksMainView.hpp"
#include "UI/DebrisTweaksPhysicsView.hpp"
#include "UI/DebrisTweaksCosmeticView.hpp"
#include "UI/DebrisTweaksFlowCoordinator.hpp"

#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp" 
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "beatsaber-hook/shared/config/config-utils.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

#include "HMUI/ViewController.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/FlowCoordinator.hpp"
#include "UnityEngine/Object.hpp"
#include "GlobalNamespace/MainFlowCoordinator.hpp"

using namespace DebrisTweaks;
DEFINE_TYPE(DebrisTweaks, DebrisTweaksFlowCoordinator);

void DebrisTweaksFlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    DebrisTweaksFC = (HMUI::FlowCoordinator*)this;
    if (!this->dtMainView) this->dtMainView = QuestUI::BeatSaberUI::CreateViewController<DebrisTweaksMainView*>();
    if (!this->dtPhysicsView) this->dtPhysicsView = QuestUI::BeatSaberUI::CreateViewController<DebrisTweaksPhysicsView*>();
    if (!this->dtCosmeticView) this->dtCosmeticView = QuestUI::BeatSaberUI::CreateViewController<DebrisTweaksCosmeticView*>();
    if (firstActivation) 
    {
        this->SetTitle(il2cpp_utils::createcsstr("Debris Tweaks"), (int)1);
        this->showBackButton = true;
        SetInitialValues();
        DebrisTweaksFlowCoordinator::ProvideInitialViewControllers(this->dtMainView, this->dtPhysicsView, this->dtCosmeticView, nullptr, nullptr);
    }
}

void DebrisTweaksFlowCoordinator::ReloadUIElements()
{
    DebrisTweaksPhysicsView* physVC = (DebrisTweaksPhysicsView*)this->dtPhysicsView;
    DebrisTweaksCosmeticView* cosmeticVC = (DebrisTweaksCosmeticView*)this->dtCosmeticView;
    physVC->ReloadUIValues();
    cosmeticVC->ReloadUIValues();
}

void DebrisTweaksFlowCoordinator::UpdateSettingsVisibility(bool visible)
{
    DebrisTweaksPhysicsView* physVC = (DebrisTweaksPhysicsView*)this->dtPhysicsView;
    DebrisTweaksCosmeticView* cosmeticVC = (DebrisTweaksCosmeticView*)this->dtCosmeticView;
    physVC->OnChangeVisibility(visible);
    cosmeticVC->OnChangeVisibility(visible);
}

void DebrisTweaksFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topView)
{
    getConfig().Write();
    HMUI::FlowCoordinator* settingsFC = QuestUI::GetModSettingsFlowCoordinator();
    settingsFC->DismissFlowCoordinator(this, (int)0, nullptr, false);
}