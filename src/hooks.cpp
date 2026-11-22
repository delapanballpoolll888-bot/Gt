#include <android/log.h>
#include <dlfcn.h>
#include "dobby.h"

#define LOG_TAG "GT_HACK"

typedef void(*CalculateDamageOnly_t)(void* damageInfoPtr);
CalculateDamageOnly_t original_CalculateDamageOnly = nullptr;

typedef void(*SetDamage_t)(void* __this, float value, bool hasValue, void* methodInfo);
SetDamage_t set_damage_func = nullptr;

void hooked_CalculateDamageOnly(void* damageInfoPtr) {
    if (!damageInfoPtr) return;
    if (original_CalculateDamageOnly) original_CalculateDamageOnly(damageInfoPtr);

    // Cari fungsi set_damage jika belum ada
    if (!set_damage_func) {
        void* handle = dlopen("libil2cpp.so", RTLD_NOW);
        if (handle) {
            void* domain = ((void*(*)())dlsym(handle, "il2cpp_domain_get"))();
            void* assembly = ((void*(*)(void*, const char*))dlsym(handle, "il2cpp_domain_assembly_open"))(domain, "Assembly-CSharp.dll");
            void* image = ((void*(*)(void*))dlsym(handle, "il2cpp_assembly_get_image"))(assembly);
            void* klass = ((void*(*)(void*, const char*, const char*))dlsym(handle, "il2cpp_class_from_name"))(image, "", "DamageInfo");
            void* method = ((void*(*)(void*, const char*, int))dlsym(handle, "il2cpp_class_get_method_from_name"))(klass, "set_damage", 1);
            
            if (method) {
                // Ambil pointer fungsi dari method
                set_damage_func = (SetDamage_t)(*(void**)((uintptr_t)method + 0x0)); 
            }
        }
    }

    // TIMPA DAMAGE JADI 7 JUTA
    if (set_damage_func) {
        set_damage_func(damageInfoPtr, 7777777.0f, true, nullptr);
    }
}

void InstallHooks() {
    void* handle = dlopen("libil2cpp.so", RTLD_NOW);
    if (!handle) return;
    
    Dl_info info;
    dladdr(dlsym(handle, "il2cpp_init"), &info);
    uintptr_t lib_base = (uintptr_t)info.dli_fbase;

    uintptr_t target_rva = 0x63654C0; // RVA dari dump.cs kamu
    void* target_func = (void*)(lib_base + target_rva);

    DobbyHook(target_func, (void*)hooked_CalculateDamageOnly, (void**)&original_CalculateDamageOnly);
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Hook Installed!");
}
