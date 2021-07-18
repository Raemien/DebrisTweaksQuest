#include "main.hpp"
#include "UI/SharedUIManager.hpp"
#include "UI/DebrisTweaksCosmeticView.hpp"

#include <string>

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "questui/shared/CustomTypes/Components/Settings/IncrementSetting.hpp"

#include "UnityEngine/Object.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "HMUI/ViewController.hpp"
#include "UnityEngine/UI/Selectable.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "TMPro/TextMeshProUGUI.hpp"

using namespace DebrisTweaks;
DEFINE_TYPE(DebrisTweaks, DebrisTweaksCosmeticView);

DebrisTweaksCosmeticView* CosmeticView;

void OnChangeLifetime(float newval)
{
    newval = std::abs(std::round(newval * 100)) / 100;
    auto& modcfg = getConfig().config;
    auto* element = CosmeticView->debrisLifetimeSetting;
    auto* decrButton = element->GetComponentsInChildren<UnityEngine::UI::Button*>()->values[0];
    decrButton->set_interactable(true);
    if (newval < element->Increment)
    {
        decrButton->set_interactable(false);
    }
    modcfg["debrisLifetime"].SetFloat(newval);
}

void OnChangeScale(float newval)
{
    newval = std::abs(std::round(newval * 100)) / 100;
    auto& modcfg = getConfig().config;
    auto* element = CosmeticView->debrisScaleSetting;
    auto* decrButton = element->GetComponentsInChildren<UnityEngine::UI::Button*>()->values[0];
    auto* incrButton = element->GetComponentsInChildren<UnityEngine::UI::Button*>()->values[1];
    decrButton->set_interactable(true);
    incrButton->set_interactable(true);
    if (newval < element->Increment * 2)
    {
        decrButton->set_interactable(false);
    }
    if (newval > 1.5 - element->Increment)
    {
        incrButton->set_interactable(false);
    }
    modcfg["debrisScale"].SetFloat(newval);
}

void OnChangeMonochrome(bool newval)
{
    auto& modcfg = getConfig().config;
    modcfg["monochromeDebris"].SetBool(newval);
}

void OnChangeLTimeOverride(bool newval)
{
    auto& modcfg = getConfig().config;
    modcfg["overrideLifetime"].SetBool(newval);
    CosmeticView->ReloadUIValues();
}

void DebrisTweaksCosmeticView::OnChangeVisibility(bool newval)
{
    this->container->get_gameObject()->SetActive(newval);
}

void DebrisTweaksCosmeticView::ReloadUIValues()
{
    SetInitialValues();
    // Set increment values
    this->debrisLifetimeSetting->CurrentValue = UIValues->lifetime;
    this->debrisScaleSetting->CurrentValue = UIValues->scale;
    //Update UI elements
    this->overrideLifetimeToggle->set_isOn(UIValues->overridetime);
    this->monochromeToggle->set_isOn(UIValues->monochrome);
    this->debrisLifetimeSetting->UpdateValue();
    this->debrisScaleSetting->UpdateValue();
    // Set interactables
    this->debrisLifetimeSetting->get_gameObject()->GetComponentsInChildren<UnityEngine::UI::Selectable*>()->values[0]->set_interactable(UIValues->overridetime);
    this->debrisLifetimeSetting->get_gameObject()->GetComponentsInChildren<UnityEngine::UI::Selectable*>()->values[1]->set_interactable(UIValues->overridetime);
}

// void ToggleInteractables(bool clickable)
// {
//     CosmeticView->debrisLifetimeSetting->get_gameObject()->GetComponentsInChildren<UnityEngine::UI::Selectable*>()->values[0]->set_interactable(clickable);
//     CosmeticView->debrisLifetimeSetting->get_gameObject()->GetComponentsInChildren<UnityEngine::UI::Selectable*>()->values[1]->set_interactable(clickable);

//     CosmeticView->debrisScaleSetting->get_gameObject()->GetComponentsInChildren<UnityEngine::UI::Selectable*>()->values[0]->set_interactable(clickable);
//     CosmeticView->debrisScaleSetting->get_gameObject()->GetComponentsInChildren<UnityEngine::UI::Selectable*>()->values[1]->set_interactable(clickable);
// }

void DebrisTweaksCosmeticView::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    CosmeticView = this;
    if(firstActivation && addedToHierarchy) 
    {
        this->container = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_rectTransform());
        container->set_spacing(0.2f);
        container->GetComponent<UnityEngine::UI::LayoutElement*>()->set_minWidth(25.0);

        // Title
        auto* titlecontainer = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(container->get_rectTransform());
        titlecontainer->get_gameObject()->AddComponent<QuestUI::Backgroundable*>()->ApplyBackground(il2cpp_utils::createcsstr("panel-top"));
        titlecontainer->set_padding(UnityEngine::RectOffset::New_ctor(10, 10, 0, 0));
        titlecontainer->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_horizontalFit(2);

        auto* titletext = QuestUI::BeatSaberUI::CreateText(titlecontainer->get_rectTransform(), "Cosmetic", false);
        titletext->set_alignment(TMPro::TextAlignmentOptions::Center);
        titletext->set_fontSize(10);

        // Config Panel
        this->configcontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(container->get_rectTransform());
        configcontainer->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
        configcontainer->set_childForceExpandHeight(false);
        configcontainer->set_childControlHeight(true);

        this->overrideLifetimeToggle = QuestUI::BeatSaberUI::CreateToggle(configcontainer->get_rectTransform(), "Override Debris Lifetime", UIValues->overridetime, OnChangeLTimeOverride);
        this->debrisLifetimeSetting = QuestUI::BeatSaberUI::CreateIncrementSetting(configcontainer->get_rectTransform(), "Lifespan (Seconds)", 2, 0.2, UIValues->lifetime, OnChangeLifetime);
        this->debrisScaleSetting = QuestUI::BeatSaberUI::CreateIncrementSetting(configcontainer->get_rectTransform(), "Scale", 2, 0.1, UIValues->scale, OnChangeScale);
        QuestUI::BeatSaberUI::AddHoverHint(debrisScaleSetting->get_gameObject(), "The relative size of sliced debris.");
        this->monochromeToggle = QuestUI::BeatSaberUI::CreateToggle(configcontainer->get_rectTransform(), "Monochromatic Colors", UIValues->freezerot, UnityEngine::Vector2(0, 0), OnChangeMonochrome);

        this->ReloadUIValues();
    }
}

void DebrisTweaksCosmeticView::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling)
{
    getConfig().Write();
}