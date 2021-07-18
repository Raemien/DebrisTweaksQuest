#include "main.hpp"
#include "DTConfig.hpp"
#include "UI/SharedUIManager.hpp"
#include "UI/DebrisTweaksMainView.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "questui/shared/CustomTypes/Components/Settings/IncrementSetting.hpp"

#include "HMUI/FlowCoordinator.hpp"
#include "HMUI/ViewController.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "GlobalNamespace/SimpleLevelStarter.hpp"
#include "GlobalNamespace/BeatmapLevelSO.hpp"

using namespace DebrisTweaks;
DEFINE_TYPE(DebrisTweaks, DebrisTweaksMainView);

void OnChangeEnabled(bool newval)
{
    auto& modcfg = getConfig().config;
    modcfg["enabled"].SetBool(newval);
    ApplyPanelVisibility(newval);
}

void ResetValues()
{
    SetupConfig();
    getConfig().config["enabled"].SetBool(true);
    SetInitialValues();
    ReloadAllElements();
}

void TestDebris()
{
    Array<GlobalNamespace::SimpleLevelStarter*>* levelStartArray = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::SimpleLevelStarter*>();
    for (int i = 0; i < sizeof(levelStartArray); i++)
    {
        GlobalNamespace::SimpleLevelStarter* lstart = (GlobalNamespace::SimpleLevelStarter*)levelStartArray->values[i];
        if (lstart->get_gameObject()->get_name()->Contains(il2cpp_utils::createcsstr("PerformanceTestLevelButton")))
        {
            getLogger().info("[DebrisTweaks] Starting test level...");
            lstart->level->songName = il2cpp_utils::createcsstr("Debris Test");
            lstart->StartLevel();
            return;
        } 
    }
}

void DebrisTweaksMainView::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    SetInitialValues();
    if (firstActivation && addedToHierarchy)
    {
        UnityEngine::UI::VerticalLayoutGroup* container = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_rectTransform());
        container->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
        
        // Enable button
        UnityEngine::UI::VerticalLayoutGroup* enabledcontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(container->get_rectTransform());
        enabledcontainer->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
        enabledcontainer->GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredHeight(30);
        this->masterEnabledToggle = QuestUI::BeatSaberUI::CreateToggle(enabledcontainer->get_rectTransform(), "Enable Tweaks", UIValues->enabled, OnChangeEnabled);

        // Spacing placeholder
        this->vertpholder = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(container->get_rectTransform());
        this->vertpholder->GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredHeight(74.0);
        
        // Footer container
        UnityEngine::UI::HorizontalLayoutGroup* footercontainer = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(container->get_rectTransform());
        footercontainer->set_padding(UnityEngine::RectOffset::New_ctor(1, 1, 2, 2));

        UnityEngine::UI::Button* testButton = QuestUI::BeatSaberUI::CreateUIButton(footercontainer->get_rectTransform(), "Test Debris!", TestDebris);
        UnityEngine::UI::Button* resetButton = QuestUI::BeatSaberUI::CreateUIButton(footercontainer->get_rectTransform(), "Reset", ResetValues);
    }
}

void DebrisTweaksMainView::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling)
{
    getConfig().Write();
}