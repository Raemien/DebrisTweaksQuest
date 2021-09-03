#include "UI/DebrisTweaksMainView.hpp"
#include "UI/DebrisTweaksPhysicsView.hpp"
#include "UI/DebrisTweaksCosmeticView.hpp"
#include "UI/DebrisTweaksFlowCoordinator.hpp"

#include "DTConfig.hpp"
using namespace DebrisTweaks;

#include <unordered_set>
#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp" 
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"

#include "questui/shared/QuestUI.hpp"
#include "custom-types/shared/register.hpp"

#include "UnityEngine/Quaternion.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Rigidbody.hpp"
#include "UnityEngine/Renderer.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/MaterialPropertyBlock.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Color.hpp"


#include "GlobalNamespace/ColorType.hpp"
#include "GlobalNamespace/NoteDebris.hpp"
#include "GlobalNamespace/ConditionalMaterialSwitcher.hpp"
#include "GlobalNamespace/MaterialPropertyBlockController.hpp"

using namespace GlobalNamespace;
using namespace UnityEngine;

static ModInfo modInfo;

Logger& getLogger() {
    static Logger* logger = new Logger(modInfo, LoggerOptions(false, true));
    return *logger;
}

Configuration& getConfig() {
    static Configuration config(modInfo);
    return config;
}

MAKE_HOOK_MATCH(NoteDebris_Init, &GlobalNamespace::NoteDebris::Init, void, NoteDebris* self, ColorType color, Vector3 pos, Quaternion rot, Vector3 moveVec, Vector3 scale, Vector3 posoff, Quaternion rotoff,
Vector3 cpoint, Vector3 cnorm, Vector3 force, Vector3 torque, float lifeTime)
{
    // Regular NoteDebris parameters
    auto& modcfg = getConfig().config;
    if (modcfg["enabled"].GetBool())
    {
        float vmul = modcfg["velocityMultiplier"].GetFloat();
        bool overrideLifetime = modcfg["overrideLifetime"].GetBool();
        lifeTime = overrideLifetime ? modcfg["debrisLifetime"].GetFloat() : lifeTime;
        scale = UnityEngine::Vector3().get_one() * modcfg["debrisScale"].GetFloat();
        force.x *= vmul;
    }
    NoteDebris_Init(self, color, pos, rot, moveVec, scale, posoff, rotoff, cpoint, cnorm, force, torque, lifeTime);
    // Transform + Rigidbody parameters
    if (modcfg["enabled"].GetBool() && self)
    {
        UnityEngine::Rigidbody* rbody = self->GetComponent<UnityEngine::Rigidbody*>();

        static auto set_freezeRotation = reinterpret_cast<function_ptr_t<void, UnityEngine::Rigidbody*, bool>>(il2cpp_functions::resolve_icall("UnityEngine.Rigidbody::set_freezeRotation"));
        set_freezeRotation(rbody, modcfg["freezeRotations"].GetBool());

        static auto set_drag = reinterpret_cast<function_ptr_t<void, UnityEngine::Rigidbody*, float>>(il2cpp_functions::resolve_icall("UnityEngine.Rigidbody::set_drag"));
        set_drag(rbody, modcfg["drag"].GetFloat());

        UnityEngine::Transform* tform = self->get_transform();
        rbody->set_useGravity(modcfg["enableGravity"].GetBool());

        UnityEngine::Renderer* rend = tform->get_gameObject()->GetComponentInChildren<UnityEngine::Renderer*>();
        if (rend)
        {
            auto* matSwitcher = tform->GetComponentInChildren<GlobalNamespace::ConditionalMaterialSwitcher*>();
            if (modcfg["enablePCDebris"].GetBool() && matSwitcher) rend->SetMaterial(matSwitcher->material1);

            if (modcfg["monochromeDebris"].GetBool())
            {
                self->materialPropertyBlockController->materialPropertyBlock->SetColor(self->_get__colorID(), UnityEngine::Color::get_gray());
                self->materialPropertyBlockController->ApplyChanges();
            }
        }

    }
}


extern "C" void setup(ModInfo& info) {
    info.id = "DebrisTweaks";
    info.version = "1.1.1";
    modInfo = info;
    getConfig().Load();
}

extern "C" void load() {
    if (!LoadConfig()) SetupConfig();
    il2cpp_functions::Init();
    QuestUI::Init();
    INSTALL_HOOK(getLogger(), NoteDebris_Init); 
    custom_types::Register::AutoRegister();
    QuestUI::Register::RegisterModSettingsFlowCoordinator<::DebrisTweaksFlowCoordinator*>(modInfo);
}
