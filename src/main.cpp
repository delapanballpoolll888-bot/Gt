#include <jni.h>
#include <string>
#include <dlfcn.h>
#include <thread>
#include <unistd.h>
#include "zygisk.hpp"

using namespace zygisk;

void InstallHooks(); // Fungsi dari hooks.cpp

class MyModule : public ModuleBase {
public:
    void onLoad(Api *api, JNIEnv *env) override {
        this->api = api;
        this->env = env;
    }
    void preAppSpecialize(AppSpecializeArgs *args) override {
        const char *name = env->GetStringUTFChars(args->nice_name, nullptr);
        if (std::string(name) == "com.kakaogames.gdungeon") {
            shouldInject = true;
        }
        env->ReleaseStringUTFChars(args->nice_name, name);
        if (!shouldInject) {
            api->setOption(DLCLOSE_MODULE_LIBRARY);
        }
    }
    void postAppSpecialize(const AppSpecializeArgs *) override {
        if (shouldInject) {
            std::thread([]() {
                // Tunggu game load library
                for(int i=0; i<20; i++) {
                    if (dlopen("libil2cpp.so", RTLD_NOW)) {
                        sleep(3);
                        InstallHooks(); // Panggil cheat
                        break;
                    }
                    sleep(1);
                }
            }).detach();
        }
    }
private:
    Api *api;
    JNIEnv *env;
    bool shouldInject = false;
};

REGISTER_ZYGISK_MODULE(MyModule)
