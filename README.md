# HuyJIT-ModMenu
Huy JIT Mod Menu is a template menu for iOS that supported patching offsets/hexes for Non-jailbreak with JIT and fix patch for unc0ver/Xina/palera1n/Dopamine jailbreak using IMGUI.

<div style="text-align: center;">
<b>IMGUI Template Preview</b><br>

<img src="https://raw.githubusercontent.com/34306/HuyJIT-ModMenu/main/Preview.PNG">
</div>


# About
- I'm using vm_writeData.h to patch the offsets/hexes
- Kopycat some code from [joeyjurjens](https://github.com/joeyjurjens/iOS-Mod-Menu-Template-for-Theos)
- Also bring encryption from joeyjurjens template too
- Hook by DobbyHook
- Fan boi of 五等分の花嫁

# Installation
- Using theos for compilation
- Add ```THEOS_PACKAGE_SCHEME = rootless``` to support rootless/Dopamine

# Feature
- On/Off switch for patching offsets
- Support Hooking (by DobbyHook)
- Added getRealOffset(), you can now use it to read the address data
- Added auto update for Unity3D games, it's now auto find the offsets addresses

# Usage
**3 fingers double tap to screen to open menu, 2 fingers double tap to disable menu**

Editing these in `ImGuiDrawView.mm`

- Patching offset on default binary `NULL`
```obj-c
patch_NULL("0x10517A154", "0xC0035FD6");
```

- Patching offset on `UnityFramework`
```obj-c
patch("0x517A154", "0x360080D2");
```
You can change this to anything you want to patch on the line where I noted in `5Toubun/NakanoYotsuba.h`. Normally it's `UnityFramework` but some games like LoL WildRift is `FEProj`

- Hooking a static address (both `NULL` and `UnityFramework`)
```obj-c
void (*_LActorRoot_Visible)(void *instance, int camp, bool bVisible, const bool forceSync);
void LActorRoot_Visible(void *instance, int camp, bool bVisible, const bool forceSync = false) {
    if (instance != nullptr && show_s0) {
        if(camp == 1 || camp == 2 || camp == 110 || camp == 255) {
            bVisible = true;
        }
    } 
 return _LActorRoot_Visible(instance, camp, bVisible, forceSync);
}

DobbyHook((void *)getRealOffset(methodOffset), (void *)LActorRoot_Visible, (void **)&_LActorRoot_Visible);
```
- Auto update for Unity3D games (initial_setup function) using `il2cpp.h`
```
void initial_setup(){
    Il2CppAttach();   //<- This one is required, otherwise crash
    Il2CppMethod& getClass(const char* namespaze, const char* className);
    uint64_t getMethod(const char* methodName, int argsCount);
    Il2CppMethod methodAccess("Project.Plugins_d.dll"); //<- Image to get method
    methodOffset = methodAccess.getClass("NucleusDrive.Logic", "LVActorLinker").getMethod("SetVisible", 3); //<- Method offset address
}
```
#### To use more il2cpp function like `Fields`, `Strings` please read `il2cpp.h` file in `5Toubun/il2cpp.h`
- Font using for this menu is Zenless Zone Zero font (**Vietnamese and English only**)

### Pull request button is on the top, you can contribute to this project if you want

# Credits
- Huy Nguyen (it's me) [34306](https://github.com/34306)
- [x2nios](https://github.com/x2niosvn) for [IMGUI Mod Menu](https://github.com/x2niosvn/iOS-IMGUI-Mod-Menu-Templates)
- [joeyjurjens](https://github.com/joeyjurjens) for [iOS Mod Menu](https://github.com/joeyjurjens/iOS-Mod-Menu-Template-for-Theos)
- [Dobby](https://github.com/jmpews/Dobby) by [jmpews](https://github.com/jmpews) (Apache-2.0 license)
- [il2cpp](https://iosgods.com/topic/183784-il2cpp-resolver-for-theos-v03/) from iOSGods
- Special thanks to: Red16, tuancc, YeetDisDude, [modfs] AloH, HappySecret and Lavochka (H5GG Discord)
