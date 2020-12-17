#include "main.hpp"
#include "DTConfig.hpp"
#include "DebrisTweaksViewController.hpp"

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
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Events/UnityAction.hpp"

using namespace DebrisTweaks;
DEFINE_CLASS(DebrisTweaksViewController);

HMUI::FlowCoordinator* settingsFC;
UnityEngine::GameObject* debrisTest;

// Inital Values
bool enabled_initval;
bool overridetime_initval;
bool gravity_initvalue;
float lifetime_initval;
float scale_initval;
float velocity_initval;

void OnChangeEnabled(DebrisTweaksViewController* self, bool newval)
{
    auto& modcfg = getConfig().config;
    modcfg["enabled"].SetBool(newval);
    self->toggleoptions->get_gameObject()->SetActive(newval);
    self->incrementoptions->get_gameObject()->SetActive(newval);
    self->vertpholder->get_gameObject()->SetActive(!newval);
}

void OnChangeLifetime(DebrisTweaksViewController* self, float newval)
{
    newval = std::abs(std::round(newval * 100)) / 100;
    auto& modcfg = getConfig().config;
    auto* element = self->debrisLifetimeSetting;
    auto* decrButton = element->GetComponentsInChildren<UnityEngine::UI::Button*>()->values[0];
    decrButton->set_interactable(true);
    if (newval < element->Increment)
    {
        decrButton->set_interactable(false);
    }
    modcfg["debrisLifetime"].SetFloat(newval);
}

void OnChangeScale(DebrisTweaksViewController* self, float newval)
{
    newval = std::abs(std::round(newval * 100)) / 100;
    auto& modcfg = getConfig().config;
    auto* element = self->debrisScaleSetting;
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

void OnChangeVelocity(DebrisTweaksViewController* self, float newval)
{
    newval = std::abs(std::round(newval * 100)) / 100;
    auto& modcfg = getConfig().config;
    auto* element = self->debrisVelMulSetting;
    auto* decrButton = element->GetComponentsInChildren<UnityEngine::UI::Button*>()->values[0];
    auto* incrButton = element->GetComponentsInChildren<UnityEngine::UI::Button*>()->values[1];
    decrButton->set_interactable(true);
    incrButton->set_interactable(true);
    if (newval < element->Increment)
    {
        decrButton->set_interactable(false);
    }
    if (newval > 3 - element->Increment)
    {
        incrButton->set_interactable(false);
    }
    modcfg["velocityMultiplier"].SetFloat(newval);
}

void OnChangeLTimeOverride(DebrisTweaksViewController* self, bool newval)
{
    auto& modcfg = getConfig().config;
    modcfg["overrideLifetime"].SetBool(newval);
    self->debrisLifetimeSetting->get_gameObject()->SetActive(newval);
}

void OnChangeGravity(DebrisTweaksViewController* self, bool newval)
{
    auto& modcfg = getConfig().config;
    modcfg["enableGravity"].SetBool(newval);
}

void SetInitialValues()
{
    enabled_initval = getConfig().config["enabled"].GetBool();
    overridetime_initval = getConfig().config["overrideLifetime"].GetBool();
    gravity_initvalue = getConfig().config["enableGravity"].GetBool();
    lifetime_initval = getConfig().config["debrisLifetime"].GetFloat();
    scale_initval = getConfig().config["debrisScale"].GetFloat();
    velocity_initval = getConfig().config["velocityMultiplier"].GetFloat();
}

void ReloadUIValues(DebrisTweaksViewController* self)
{
    // Set increment values
    self->debrisLifetimeSetting->CurrentValue = lifetime_initval;
    self->debrisScaleSetting->CurrentValue = scale_initval;
    self->debrisVelMulSetting->CurrentValue = velocity_initval;
    //Update UI elements
    self->overrideLifetimeToggle->set_isOn(overridetime_initval);
    self->enableGravityToggle->set_isOn(gravity_initvalue);
    self->debrisLifetimeSetting->UpdateValue();
    self->debrisScaleSetting->UpdateValue();
    self->debrisVelMulSetting->UpdateValue();
}

void ResetValues(DebrisTweaksViewController* self) 
{
    SetupConfig();
    getConfig().config["enabled"].SetBool(true);
    SetInitialValues();
    ReloadUIValues(self);
}

void DebrisTweaksViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    SetInitialValues();
    if (firstActivation && addedToHierarchy)
    {
        settingsFC = QuestUI::getModSettingsFlowCoordinator();

        UnityEngine::UI::VerticalLayoutGroup* container = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_rectTransform());
        container->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
        
        // Enable button
        UnityEngine::UI::VerticalLayoutGroup* enabledcontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(container->get_rectTransform());
        enabledcontainer->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
        enabledcontainer->GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredHeight(30);
        auto onChangeEnabledAction = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>(classof(UnityEngine::Events::UnityAction_1<bool>*), this, OnChangeEnabled);
        this->masterEnabledToggle = QuestUI::BeatSaberUI::CreateToggle(enabledcontainer->get_rectTransform(), "Enable Tweaks", enabled_initval, onChangeEnabledAction);

        // Spacing placeholder
        this->vertpholder = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(container->get_rectTransform());
        this->vertpholder->GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredHeight(74.0);
        this->vertpholder->get_gameObject()->SetActive(!enabled_initval);

        // Toggle container
        this->toggleoptions = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(container->get_rectTransform());
        toggleoptions->get_gameObject()->AddComponent<QuestUI::Backgroundable*>()->ApplyBackground(il2cpp_utils::createcsstr("round-rect-panel"));
        toggleoptions->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
        toggleoptions->set_padding(UnityEngine::RectOffset::New_ctor(2, 2, 2, 2));
        UnityEngine::UI::LayoutElement* togglelayout = toggleoptions->GetComponent<UnityEngine::UI::LayoutElement*>();
        togglelayout->set_preferredWidth(100.0);
        togglelayout->set_preferredHeight(30.0);

        // Toggles
        auto onChangeLTimeOverrideAction = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>(classof(UnityEngine::Events::UnityAction_1<bool>*), this, OnChangeLTimeOverride);
        this->overrideLifetimeToggle = QuestUI::BeatSaberUI::CreateToggle(toggleoptions->get_rectTransform(), "Override Debris Lifetime", overridetime_initval, onChangeLTimeOverrideAction);
        
        auto onChangeGravityAction = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>(classof(UnityEngine::Events::UnityAction_1<bool>*), this, OnChangeGravity);
        this->enableGravityToggle = QuestUI::BeatSaberUI::CreateToggle(toggleoptions->get_rectTransform(), "Enable Gravity", gravity_initvalue, onChangeGravityAction);

        // Increment container
        this->incrementoptions = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(container->get_rectTransform());
        incrementoptions->get_gameObject()->AddComponent<QuestUI::Backgroundable*>()->ApplyBackground(il2cpp_utils::createcsstr("round-rect-panel"));
        incrementoptions->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
        incrementoptions->set_padding(UnityEngine::RectOffset::New_ctor(2, 2, 2, 2));
        UnityEngine::UI::LayoutElement* incrementlayout = incrementoptions->GetComponent<UnityEngine::UI::LayoutElement*>();
        incrementlayout->set_preferredWidth(100.0);
        incrementlayout->set_preferredHeight(40.0);
        
        // Increments
        auto onChangeScaleAction = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(classof(UnityEngine::Events::UnityAction_1<float>*), this, OnChangeScale);
        this->debrisScaleSetting = QuestUI::BeatSaberUI::CreateIncrementSetting(incrementoptions->get_rectTransform(), "Scale", 2, 0.1, scale_initval, onChangeScaleAction);
        QuestUI::BeatSaberUI::AddHoverHint(debrisScaleSetting->get_gameObject(), "The relative size of sliced debris.");

        auto onChangeLifetimeAction = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(classof(UnityEngine::Events::UnityAction_1<float>*), this, OnChangeLifetime);
        this->debrisLifetimeSetting = QuestUI::BeatSaberUI::CreateIncrementSetting(incrementoptions->get_rectTransform(), "Lifespan (Seconds)", 2, 0.2, lifetime_initval, onChangeLifetimeAction);
        this->debrisLifetimeSetting->get_gameObject()->SetActive(overridetime_initval);

        auto onChangeVelocityAction = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(classof(UnityEngine::Events::UnityAction_1<float>*), this, OnChangeVelocity);
        this->debrisVelMulSetting = QuestUI::BeatSaberUI::CreateIncrementSetting(incrementoptions->get_rectTransform(), "Velocity Multiplier", 2, 0.1, velocity_initval, onChangeVelocityAction);
        QuestUI::BeatSaberUI::AddHoverHint(debrisVelMulSetting->get_gameObject(), "Multiply the initial velocity of all debris by this amount.");

        // Footer container
        UnityEngine::UI::HorizontalLayoutGroup* footercontainer = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(container->get_rectTransform());
        footercontainer->set_padding(UnityEngine::RectOffset::New_ctor(1, 1, 2, 2));
        auto onReset = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), this, ResetValues);
        UnityEngine::UI::Button* resetButton = QuestUI::BeatSaberUI::CreateUIButton(footercontainer->get_rectTransform(), "Reset", onReset);

        this->toggleoptions->get_gameObject()->SetActive(enabled_initval);
        this->incrementoptions->get_gameObject()->SetActive(enabled_initval);
    }
    settingsFC->SetTitle(il2cpp_utils::createcsstr("Debris Parameters"), HMUI::ViewController::AnimationType(0));
    ReloadUIValues(this); // Ensure increment limits and hidden layouts are correctly applied.
}

void DebrisTweaksViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling)
{
    getConfig().Write();
}