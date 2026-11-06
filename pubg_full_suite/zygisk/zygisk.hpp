// Zygisk API Header
// This is a minimal implementation for Zygisk modules
// Full header should be obtained from Magisk repository

#ifndef ZYGISK_HPP
#define ZYGISK_HPP

#include <jni.h>

namespace zygisk {

// Module options
enum ModuleOption {
    DLCLOSE_MODULE_LIBRARY = 1,
    FORCE_DENYLIST = 2,
};

// App specialize arguments
struct AppSpecializeArgs {
    JNIEnv *env;
    jclass app_class;
    jstring nice_name;
    jstring app_data_dir;
    jstring lib_dir;
    jintArray uid;
    jintArray gid;
    jintArray runtime_gid;
    jintArray runtime_uid;
    jstring se_info;
    jstring se_name;
    jstring instruction_set;
    jstring data_dir;
};

// API class
class Api {
public:
    virtual ~Api() = default;
    
    // Set module options
    virtual void setOption(ModuleOption option) = 0;
    
    // Get JNI environment
    virtual JNIEnv* getJniEnv() = 0;
};

// Module base class
class ModuleBase {
public:
    virtual ~ModuleBase() = default;
    
    // Called when module is loaded
    virtual void onLoad(Api *api, JNIEnv *env) = 0;
    
    // Called before app specialization
    virtual void preAppSpecialize(AppSpecializeArgs *args) = 0;
    
    // Called after app specialization
    virtual void postAppSpecialize(const AppSpecializeArgs *args) = 0;
};

// Module registration macro
#define REGISTER_ZYGISK_MODULE(ModuleClass) \
    extern "C" __attribute__((visibility("default"))) ModuleBase* zygisk_module_entry() { \
        return new ModuleClass(); \
    }

} // namespace zygisk

#endif // ZYGISK_HPP
