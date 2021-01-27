#include "DebrisTweaksViewController.hpp"
#include "DTConfig.hpp"
using namespace DebrisTweaks;

#include <unordered_set>
#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp" 
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include <string>

#include "questui/shared/QuestUI.hpp"
#include "custom-types/shared/register.hpp"

#include "UnityEngine/Quaternion.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Rigidbody.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Color.hpp"
#include "GlobalNamespace/ColorManager.hpp"
#include "UnityEngine/Renderer.hpp"
#include "UnityEngine/Material.hpp"
#include "GlobalNamespace/NoteDebris.hpp"
#include "GlobalNamespace/NoteDebrisSimplePhysics.hpp"
#include "GlobalNamespace/ColorType.hpp"

using namespace GlobalNamespace;

static ModInfo modInfo;

static const Logger& getLogger() {
    static const Logger logger(modInfo);
    return logger;
}

Configuration& getConfig() {
    static Configuration config(modInfo);
    return config;
}
bool isDebris = false;
MAKE_HOOK_OFFSETLESS(NoteDebris_Init, void, NoteDebris* self, ColorType color, Vector3 pos, Quaternion rot, Vector3 posoff, Quaternion rotoff,
Vector3 cpoint, Vector3 cnorm, Vector3 force, Vector3 torque, float lifeTime)
{
    // Regular NoteDebris parameters
    auto& modconfig = getConfig().config;
    if (modconfig["enabled"].GetBool())
    {
        float vmul = modconfig["velocityMultiplier"].GetFloat();
        bool overrideLifetime = modconfig["overrideLifetime"].GetBool();
        lifeTime = overrideLifetime ? modconfig["debrisLifetime"].GetFloat() : lifeTime;
        color = (modconfig["monochromeDebris"].GetBool() || modconfig["enableCustomDebrisColor"].GetBool()) ? (int)ColorType::None : (int)color;
        force.x *= vmul; force.y *= vmul; force.z *= vmul;
    }
    isDebris = true;
    NoteDebris_Init(self, color, pos, rot, posoff, rotoff, cpoint, cnorm, force, torque, lifeTime);
    isDebris = false;
    // Transform + Rigidbody parameters
    if (modconfig["enabled"].GetBool() && self)
    {
        UnityEngine::Rigidbody* rbody = self->GetComponent<UnityEngine::Rigidbody*>();

        static auto set_freezeRotation = reinterpret_cast<function_ptr_t<void, UnityEngine::Rigidbody*, bool>>(il2cpp_functions::resolve_icall("UnityEngine.Rigidbody::set_freezeRotation"));
        set_freezeRotation(rbody, modconfig["freezeRotations"].GetBool());

        static auto set_drag = reinterpret_cast<function_ptr_t<void, UnityEngine::Rigidbody*, float>>(il2cpp_functions::resolve_icall("UnityEngine.Rigidbody::set_drag"));
        set_drag(rbody, modconfig["drag"].GetFloat());

        UnityEngine::Transform* tform = self->get_transform();
        rbody->set_useGravity(modconfig["enableGravity"].GetBool());
        tform->set_localScale(UnityEngine::Vector3().get_one() * modconfig["debrisScale"].GetFloat());
    }
}
//Gray debris 
MAKE_HOOK_OFFSETLESS(ColorManager_ColorForType, UnityEngine::Color, ColorManager* self, ColorType color)
{
    auto& modconfig = getConfig().config;
    if(isDebris && color == ColorType::None)
    {
        UnityEngine::Color col = UnityEngine::Color::get_white();
        if(modconfig["monochromeDebris"].GetBool()) col = UnityEngine::Color::get_gray();
        if(modconfig["enableCustomDebrisColor"].GetBool()) col = UnityEngine::Color(modconfig["customDebrisColorR"].GetFloat()/255.0f, modconfig["customDebrisColorG"].GetFloat()/255.0f, modconfig["customDebrisColorB"].GetFloat()/255.0f, 1.0f);
        return col;
    }
    return ColorManager_ColorForType(self, color);
}

extern "C" void setup(ModInfo& info) {

    info.id = "DebrisTweaks";
    info.version = "0.3.0";
    modInfo = info;
    getConfig().Load();
}

extern "C" void load() {
    if (!LoadConfig()) SetupConfig();
    il2cpp_functions::Init();
    QuestUI::Init();
    INSTALL_HOOK_OFFSETLESS(NoteDebris_Init, il2cpp_utils::FindMethodUnsafe("", "NoteDebris", "Init", 10)); 
    INSTALL_HOOK_OFFSETLESS(ColorManager_ColorForType, il2cpp_utils::FindMethodUnsafe("", "ColorManager", "ColorForType", 1)); 
    custom_types::Register::RegisterType<DebrisTweaksViewController>();
    QuestUI::Register::RegisterModSettingsViewController<DebrisTweaksViewController*>(modInfo);
}
