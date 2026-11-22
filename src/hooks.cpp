#include <jni.h>
#include <android/log.h>
#include <dlfcn.h>
#include "dobby.h"

// 1. Hook RawAttack (Damage Asli)
typedef float (*get_RawAttack_t)(void* __this);
get_RawAttack_t original_get_RawAttack = nullptr;

float hooked_get_RawAttack(void* __this) {
    // Paksa Attack jadi 1.000.000
    return 1000000.0f; 
}

// 2. Hook Immortal (God Mode)
typedef bool (*get_Immortal_t)(void* __this);
get_Immortal_t original_get_Immortal = nullptr;

bool hooked_get_Immortal(void* __this) {
    // Paksa selalu kebal
    return true; 
}

void InstallStatsHook() {
    void* handle = dlopen("libil2cpp.so", RTLD_NOW);
    if (!handle) return;
    
    Dl_info info;
    dladdr(dlsym(handle, "il2cpp_init"), &info);
    uintptr_t lib_base = (uintptr_t)info.dli_fbase;

    // RVA dari dump.cs kamu
    uintptr_t rva_raw_attack = 0x8d4045c; 
    uintptr_t rva_immortal = 0x8d44f9c;

    void* func_attack = (void*)(lib_base + rva_raw_attack);
    void* func_immortal = (void*)(lib_base + rva_immortal);

    // Pasang Hook
    if (func_attack) {
        DobbyHook(func_attack, (void*)hooked_get_RawAttack, (void**)&original_get_RawAttack);
        __android_log_print(ANDROID_LOG_INFO, "GT_HACK", "Hook RawAttack Berhasil!");
    }
    
    if (func_immortal) {
        DobbyHook(func_immortal, (void*)hooked_get_Immortal, (void**)&original_get_Immortal);
        __android_log_print(ANDROID_LOG_INFO, "GT_HACK", "Hook Immortal Berhasil!");
    }
}
