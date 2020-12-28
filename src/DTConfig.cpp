#include "modloader/shared/modloader.hpp"
#include "main.hpp"

#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"

struct Config_t
{
    bool enabled = false;
    bool overrideLifetime = true;
    bool enableGravity = true;
    bool freezeRotations = true;
    bool monochrome = false;
    float drag = 0;
    float debrisLifetime = 2;
    float gravAmmount = 2;
    float debrisScale = 1;
    float velocityMultiplier = 1;
}
Settings;

void SetupConfig()
{
    getConfig().config.RemoveAllMembers();
    getConfig().config.SetObject();
    auto& allocator = getConfig().config.GetAllocator();
    getConfig().config.AddMember("enabled", Settings.enabled, allocator);
    getConfig().config.AddMember("overrideLifetime", Settings.overrideLifetime, allocator);
    getConfig().config.AddMember("enableGravity", Settings.enableGravity, allocator);
    getConfig().config.AddMember("debrisLifetime", Settings.debrisLifetime, allocator);
    getConfig().config.AddMember("debrisScale", Settings.debrisScale, allocator);
    getConfig().config.AddMember("velocityMultiplier", Settings.velocityMultiplier, allocator);
    getConfig().config.AddMember("freezeRotations", Settings.freezeRotations, allocator);
    getConfig().config.AddMember("drag", Settings.drag, allocator);
    getConfig().config.AddMember("monochromeDebris", Settings.monochrome, allocator);
    getConfig().Write();
}

bool LoadConfig()
{
    getConfig().Load();
    if(!getConfig().config.HasMember("enabled") || !getConfig().config["enabled"].IsBool()) return false;
    if(!getConfig().config.HasMember("overrideLifetime") || !getConfig().config["overrideLifetime"].IsBool()) return false;
    if(!getConfig().config.HasMember("enableGravity") || !getConfig().config["enableGravity"].IsBool()) return false;
    if(!getConfig().config.HasMember("debrisLifetime") || !getConfig().config["debrisLifetime"].IsFloat()) return false;
    if(!getConfig().config.HasMember("debrisScale") || !getConfig().config["debrisScale"].IsFloat()) return false;
    if(!getConfig().config.HasMember("velocityMultiplier") || !getConfig().config["velocityMultiplier"].IsFloat()) return false;
    if(!getConfig().config.HasMember("freezeRotations") || !getConfig().config["freezeRotations"].IsBool()) return false;
    if(!getConfig().config.HasMember("monochromeDebris") || !getConfig().config["monochromeDebris"].IsBool()) return false;
    if(!getConfig().config.HasMember("drag") || !getConfig().config["drag"].IsFloat()) return false;
    return true;
}