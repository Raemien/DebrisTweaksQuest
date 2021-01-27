# Builds a .zip file for loading with BMBF
$NDKPath = Get-Content $PSScriptRoot/ndkpath.txt

$buildScript = "$NDKPath/build/ndk-build"
if (-not ($PSVersionTable.PSEdition -eq "Core")) {
    $buildScript += ".cmd"
}

& $buildScript NDK_PROJECT_PATH=$PSScriptRoot APP_BUILD_SCRIPT=$PSScriptRoot/Android.mk NDK_APPLICATION_MK=$PSScriptRoot/Application.mk
Compress-Archive -Path "./libs/arm64-v8a/libdebris-tweaks.so","./bmbfmod.json", "./cover.png","./libs/arm64-v8a/libbeatsaber-hook_1_0_12.so", "./libs/arm64-v8a/libcodegen_0_6_2.so", "./libs/arm64-v8a/libquestui.so", "./libs/arm64-v8a/libcustom-types.so" -DestinationPath "./DebrisTweaks.zip" -Update
