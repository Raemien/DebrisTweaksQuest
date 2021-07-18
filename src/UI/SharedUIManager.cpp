#include "main.hpp"
#include "UI/SharedUIManager.hpp"
#include "UI/DebrisTweaksFlowCoordinator.hpp"

using namespace DebrisTweaks;

InitalValues_t* UIValues = new InitalValues_t();

HMUI::FlowCoordinator* DebrisTweaksFC;

void SetInitialValues()
{
    UIValues->enabled = getConfig().config["enabled"].GetBool();
    UIValues->overridetime = getConfig().config["overrideLifetime"].GetBool();
    UIValues->pcdebris = getConfig().config["enablePCDebris"].GetBool();
    UIValues->gravity = getConfig().config["enableGravity"].GetBool();
    UIValues->monochrome = getConfig().config["monochromeDebris"].GetBool();
    UIValues->lifetime = getConfig().config["debrisLifetime"].GetFloat();
    UIValues->scale = getConfig().config["debrisScale"].GetFloat();
    UIValues->freezerot = getConfig().config["freezeRotations"].GetBool();
    UIValues->velocity = getConfig().config["velocityMultiplier"].GetFloat();
    UIValues->drag = getConfig().config["drag"].GetFloat();
}

void ReloadAllElements()
{
    if (DebrisTweaksFC != nullptr)
    {
        DebrisTweaksFlowCoordinator* targetFC = (DebrisTweaksFlowCoordinator*)DebrisTweaksFC;
        targetFC->ReloadUIElements();
    }
}

void ApplyPanelVisibility(bool visible)
{
    if (DebrisTweaksFC != nullptr)
    {
        DebrisTweaksFlowCoordinator* targetFC = (DebrisTweaksFlowCoordinator*)DebrisTweaksFC;
        targetFC->UpdateSettingsVisibility(visible);
    }
}