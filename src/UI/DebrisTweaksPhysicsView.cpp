#include "main.hpp"
#include "UI/SharedUIManager.hpp"
#include "UI/DebrisTweaksPhysicsView.hpp"

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
DEFINE_TYPE(DebrisTweaks, DebrisTweaksPhysicsView);

DebrisTweaksPhysicsView* PhysicsView;

void OnChangeVelocity(float newval)
{
    newval = std::round(newval * 100) / 100;
    auto& modcfg = getConfig().config;
    auto* element = PhysicsView->debrisVelMulSetting;
    auto* decrButton = element->GetComponentsInChildren<UnityEngine::UI::Button*>()->values[0];
    auto* incrButton = element->GetComponentsInChildren<UnityEngine::UI::Button*>()->values[1];
    decrButton->set_interactable(true);
    incrButton->set_interactable(true);
    if (newval < -3 + element->Increment)
    {
        decrButton->set_interactable(false);
    }
    if (newval > 3 - element->Increment)
    {
        incrButton->set_interactable(false);
    }
    modcfg["velocityMultiplier"].SetFloat(newval);
}

void OnChangeDrag(float newval)
{
    newval = std::round(newval * 100) / 100;
    auto& modcfg = getConfig().config;
    auto* element = PhysicsView->debrisDragSetting;
    auto* decrButton = element->GetComponentsInChildren<UnityEngine::UI::Button*>()->values[0];
    decrButton->set_interactable(true);
    if (newval < 0 + element->Increment * 0.8)
    {
        decrButton->set_interactable(false);
    }
    modcfg["drag"].SetFloat(newval);
}

void OnChangeGravity(bool newval)
{
    auto& modcfg = getConfig().config;
    modcfg["enableGravity"].SetBool(newval);
}

void OnChangeFreezeRot(bool newval)
{
    auto& modcfg = getConfig().config;
    modcfg["freezeRotations"].SetBool(newval);
}

void DebrisTweaksPhysicsView::OnChangeVisibility(bool newval)
{
    this->container->get_gameObject()->SetActive(newval);
}

void DebrisTweaksPhysicsView::ReloadUIValues()
{
    SetInitialValues();
    // Set increment values
    this->debrisVelMulSetting->CurrentValue = UIValues->velocity;
    this->debrisDragSetting->CurrentValue = UIValues->drag;
    //Update UI elements
    this->enableGravityToggle->set_isOn(UIValues->gravity);
    this->freezeRotToggle->set_isOn(UIValues->freezerot);
    this->debrisVelMulSetting->UpdateValue();
    this->debrisDragSetting->UpdateValue();
}

// void DebrisTweaksPhysicsView::ToggleInteractables(bool clickable)
// {
//     PhysicsView->debrisVelMulSetting->get_gameObject()->GetComponentsInChildren<UnityEngine::UI::Selectable*>()->values[0]->set_interactable(clickable);
//     PhysicsView->debrisVelMulSetting->get_gameObject()->GetComponentsInChildren<UnityEngine::UI::Selectable*>()->values[1]->set_interactable(clickable);

//     PhysicsView->debrisDragSetting->get_gameObject()->GetComponentsInChildren<UnityEngine::UI::Selectable*>()->values[0]->set_interactable(clickable);
//     PhysicsView->debrisDragSetting->get_gameObject()->GetComponentsInChildren<UnityEngine::UI::Selectable*>()->values[1]->set_interactable(clickable);
// }

void DebrisTweaksPhysicsView::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    PhysicsView = this;
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

        auto* titletext = QuestUI::BeatSaberUI::CreateText(titlecontainer->get_rectTransform(), "Physics", false);
        titletext->set_alignment(TMPro::TextAlignmentOptions::Center);
        titletext->set_fontSize(10);

        // Config Panel
        this->configcontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(container->get_rectTransform());
        configcontainer->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
        configcontainer->set_childForceExpandHeight(false);
        configcontainer->set_childControlHeight(true);

        this->debrisVelMulSetting = QuestUI::BeatSaberUI::CreateIncrementSetting(configcontainer->get_rectTransform(), "Velocity Multiplier", 2, 0.1f, UIValues->velocity, OnChangeVelocity);
        this->debrisDragSetting = QuestUI::BeatSaberUI::CreateIncrementSetting(configcontainer->get_rectTransform(), "Drag", 2, 0.2f, UIValues->drag, OnChangeDrag);
        this->enableGravityToggle = QuestUI::BeatSaberUI::CreateToggle(configcontainer->get_rectTransform(), "Enable Gravity", UIValues->gravity, UnityEngine::Vector2(0, 0), OnChangeGravity);
        this->freezeRotToggle = QuestUI::BeatSaberUI::CreateToggle(configcontainer->get_rectTransform(), "Prevent Rotations", UIValues->freezerot, UnityEngine::Vector2(0, 0), OnChangeFreezeRot);
    }
    this->ReloadUIValues();
}

void DebrisTweaksPhysicsView::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling)
{
    getConfig().Write();
}