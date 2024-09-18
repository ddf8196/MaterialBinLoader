#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cstdlib>

#include <string>

#include "frida-gum.h"
#include "Util.h"
#include "material.h"


//==========================================================================================================================================

typedef struct _LoaderInvocationListener LoaderInvocationListener;
struct _LoaderInvocationListener {
    GObject parent;
};

static void loader_invocation_listener_iface_init(gpointer g_iface, gpointer iface_data);
static void loader_invocation_listener_on_enter(GumInvocationListener* listener, GumInvocationContext* ic);
static void loader_invocation_listener_on_leave(GumInvocationListener* listener, GumInvocationContext* ic);

#define LOADER_TYPE_INVOCATION_LISTENER (loader_invocation_listener_get_type())
G_DECLARE_FINAL_TYPE(LoaderInvocationListener, loader_invocation_listener, LOADER, INVOCATION_LISTENER, GObject)
G_DEFINE_TYPE_EXTENDED(LoaderInvocationListener,
                        loader_invocation_listener,
                        G_TYPE_OBJECT,
                        0,
                        G_IMPLEMENT_INTERFACE(GUM_TYPE_INVOCATION_LISTENER, loader_invocation_listener_iface_init))

//==========================================================================================================================================

enum class HookId : int {
    RESOURCE_PACK_MANAGER,
    READ_ASSET_FILE
};

struct InvocationState {
    bool redirect;
    std::string* filename;
    std::string* retstr;
};

GumAddress minecraftpeBaseAddr;
GumAddress ResourcePackManager_ResourcePackManager;
GumAddress AppPlatform_readAssetFile;

gpointer resourcePackManager = nullptr;

GumInterceptor* interceptor;
GumInvocationListener* listener;

void __attribute__((constructor)) init() {
    #ifdef DEBUG
        printf("[init]\n");
    #endif

    gum_init();

    minecraftpeBaseAddr = gum_module_find_base_address("libminecraftpe.so");

    GumModuleMap* moduleMap = gum_module_map_new();
    const GumModuleDetails* minecraftpeDetails = gum_module_map_find(moduleMap, minecraftpeBaseAddr);

    #if __arm__
        //ResourcePackManager::ResourcePackManager
        ResourcePackManager_ResourcePackManager = FindSignatures(minecraftpeDetails,
            //1.21.00.03	//0x5E64AC0  
            "F0 B5 03 AF 2D E9 00 07 90 B0 05 46 AE 48 98 46 92 46 78 44 00 68 00 68 0F 90 08 69"
        );

        //AppPlatform::readAssetFile
        AppPlatform_readAssetFile = FindSignatures(minecraftpeDetails,
            //1.21.00.03	//0x38A4C54
            "F0 B5 03 AF 4D F8 04 8D A0 B0 04 46 75 48 78 44 00 68 00 68 1F 90 00 20 CD E9 09 00"
        );

        if (ResourcePackManager_ResourcePackManager) {
            ResourcePackManager_ResourcePackManager += 1;
        }
        if (AppPlatform_readAssetFile) {
            AppPlatform_readAssetFile += 1;
        }
    #elif __aarch64__
        //ResourcePackManager::ResourcePackManager
        ResourcePackManager_ResourcePackManager = FindSignatures(minecraftpeDetails,
            //1.20.81.01	//0x8D83E80
            //1.21.00.03	//0x8F567E0
            "FF 03 03 D1 FD 7B 07 A9 FD C3 01 91 F9 43 00 F9 F8 5F 09 A9 F6 57 0A A9 F4 4F 0B A9 59 D0 3B D5 F6 03 03 2A 28 17 40 F9 F5 03 02 AA F3 03 00 AA A8 83 1F F8 28 10 40 F9"
        );
        
        //AppPlatform::readAssetFile
        AppPlatform_readAssetFile = FindSignatures(minecraftpeDetails,
            //1.20.81.01	//0x5C12B88
            //1.21.00.03	//0x5CA7724
            "FF 83 04 D1 FD 7B 0E A9 FD 83 03 91 FC 5F 0F A9 F6 57 10 A9 F4 4F 11 A9 57 D0 3B D5 F3 03 08 AA E8 16 40 F9 A8 83 1F F8 FF FF 02 A9"
        );
    #else
        #error unsupported architecture
    #endif

    #ifdef DEBUG
        if (!ResourcePackManager_ResourcePackManager) {
            printf("ResourcePackManager::ResourcePackManager not found\n");
        }
        if (!AppPlatform_readAssetFile) {
            printf("AppPlatform::readAssetFile not found\n");
        }
    #endif

    g_object_unref(moduleMap);
    
    interceptor = gum_interceptor_obtain();
    listener = (GumInvocationListener*) g_object_new(LOADER_TYPE_INVOCATION_LISTENER, NULL);

    if (!ResourcePackManager_ResourcePackManager || !AppPlatform_readAssetFile) {
        return;
    }

    gum_interceptor_begin_transaction(interceptor);

    gum_interceptor_attach(interceptor,
        GSIZE_TO_POINTER(ResourcePackManager_ResourcePackManager),
        listener,
        GSIZE_TO_POINTER(HookId::RESOURCE_PACK_MANAGER));

    gum_interceptor_attach(interceptor,
        GSIZE_TO_POINTER(AppPlatform_readAssetFile),
        listener,
        GSIZE_TO_POINTER(HookId::READ_ASSET_FILE));

    gum_interceptor_end_transaction(interceptor);
}

void __attribute__((destructor)) dispose() {
    #ifdef DEBUG
        printf("[dispose]\n");
    #endif

    gum_interceptor_detach(interceptor, listener);

    g_object_unref(listener);
    g_object_unref(interceptor);

    gum_deinit();
}

struct ResourceLocation {
    int32_t mFileSystem = 0;
    std::string mPath;
    uint64_t mPathHash = 0;
    uint64_t mFullHash = 0;

    ResourceLocation() {}
    ResourceLocation(const std::string& path) : mPath(path) {}
};

//==========================================================================================================================================

static void loader_invocation_listener_class_init(LoaderInvocationListenerClass* klass) {
    (void) LOADER_IS_INVOCATION_LISTENER;
    (void) glib_autoptr_cleanup_LoaderInvocationListener;
}

static void loader_invocation_listener_iface_init(gpointer g_iface, gpointer iface_data) {
    GumInvocationListenerInterface* iface = (GumInvocationListenerInterface*) g_iface;

    iface->on_enter = loader_invocation_listener_on_enter;
    iface->on_leave = loader_invocation_listener_on_leave;
}

static void loader_invocation_listener_init(LoaderInvocationListener* self) {
}

static void loader_invocation_listener_on_enter(GumInvocationListener* listener, GumInvocationContext* ic) {
    HookId hookId = (HookId) GUM_IC_GET_FUNC_DATA(ic, uintptr_t);

    switch (hookId) {
        //ResourcePackManager::ResourcePackManager(ResourcePackManager* this, std::function<Core::PathBuffer<std::string>()> getPath, const Bedrock::NotNullNonOwnerPtr<const IContentTierManager>& contentTierManager, bool needsToInitialize)
        case HookId::RESOURCE_PACK_MANAGER: {
            gpointer needsToInitialize = gum_invocation_context_get_nth_argument(ic, 3);

            if (resourcePackManager == nullptr && needsToInitialize) {
                resourcePackManager = gum_invocation_context_get_nth_argument(ic, 0);
            }
            break;
        }
        //std::string AppPlatform::readAssetFile(AppPlatform* this, const Core::Path& filename)
        case HookId::READ_ASSET_FILE: {
            InvocationState* state = GUM_IC_GET_INVOCATION_DATA(ic, InvocationState);
            state->redirect = false;
            state->filename = NULL;
            state->retstr = NULL;

            std::string* filename = nullptr;
            #if __arm__
                filename = (std::string*)gum_invocation_context_get_nth_argument(ic, 2);
            #elif __aarch64__
                filename = (std::string*)gum_invocation_context_get_nth_argument(ic, 1);
            #else
                #error unsupported architecture
            #endif

            size_t size = filename->size();
            if (size < 32) {
                return;
            }

            const char* data = filename->data();
            if ((strncmp(data, "renderer/materials/", 19) == 0 || strncmp(data, "assets/renderer/materials/", 26) == 0) && strncmp(data + size - 13, ".material.bin", 13) == 0) {
                #ifdef DEBUG
                    printf("filename=%s\n", data);
                #endif
                
                state->redirect = true;
                state->filename = filename;
                #if __arm__
                    state->retstr = (std::string*)gum_invocation_context_get_nth_argument(ic, 0);
                #elif __aarch64__
                    state->retstr = (std::string*)ic->cpu_context->x[8];
                #else
                    #error unsupported architecture
                #endif

            }

            break;
        }
    }
}

static void loader_invocation_listener_on_leave(GumInvocationListener* listener, GumInvocationContext* ic) {
    HookId hookId = (HookId) GUM_IC_GET_FUNC_DATA(ic, uintptr_t);

    switch (hookId) {
        //ResourcePackManager::ResourcePackManager(ResourcePackManager* this, std::function<Core::PathBuffer<std::string>()> getPath, const Bedrock::NotNullNonOwnerPtr<const IContentTierManager>& contentTierManager, bool needsToInitialize)
        case HookId::RESOURCE_PACK_MANAGER: {
            break;
        }
        //std::string AppPlatform::readAssetFile(AppPlatform* this, const Core::Path& filename)
        case HookId::READ_ASSET_FILE: {
            InvocationState* state = GUM_IC_GET_INVOCATION_DATA(ic, InvocationState);

            if (state->redirect) {
                #ifdef DEBUG
                    printf("state->redirect == true\n");
                #endif
                if (resourcePackManager != NULL) {
                    #ifdef DEBUG
                        printf("resourcePackManager != NULL\n");
                    #endif

                    void (**vptr)() = *(void (***)())resourcePackManager;
                    //bool ResourcePackManager::load(ResourcePackManager* this, const ResourceLocation& resourceLocation, std::string& resourceStream)
                    bool (*load)(void*, void*, void*) = (bool (*)(void*, void*, void*))*(vptr + 2); 

                    const char* data = state->filename->data();
                    if (strncmp(data, "assets/", 7) == 0) {
                        data += 7;
                    }

                    ResourceLocation location(data);
                    std::string resourceStream;
                    bool result = load(resourcePackManager, &location, &resourceStream);

                    if (result && !resourceStream.empty()) {
                        bool successful_update = true;

                        struct Buffer outbufdata = {0,0};
                        if (update_file(resourceStream.length(), (const uint8_t*)resourceStream.c_str(), &outbufdata) != 0) {
                            puts("Updating failed!");
                            successful_update = false;
                            free_buf(outbufdata);
                        }
                        
                        #ifdef DEBUG
                            printf("ResourcePackManager::load returned true\n");
                        #endif
                        if (!successful_update) {
                            state->retstr->assign(resourceStream);
                        } else {
                            state->retstr->assign((const char*)outbufdata.data, outbufdata.len);
                            free_buf(outbufdata);
                        }
                    } else {
                        #ifdef DEBUG
                            printf("ResourcePackManager::load returned false\n");
                        #endif
                    }
                    
                    #ifdef DEBUG
                        printf("\n");
                    #endif
                }
            }
            break;
        }
    }
}
