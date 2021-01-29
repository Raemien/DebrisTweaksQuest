#include "modloader/shared/modloader.hpp"

#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "HMUI/FlowCoordinator.hpp"

struct InitalValues_t
{
    bool enabled;
    bool overridetime;
    bool gravity;
    bool monochrome;
    bool lifetime;
    bool scale;
    bool freezerot;
    float velocity;
    float drag;
};

extern InitalValues_t* UIValues;
extern HMUI::FlowCoordinator* DebrisTweaksFC;
extern void SetInitialValues();
extern void ReloadAllElements();
extern void ApplyPanelVisibility(bool visible);