#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cstdlib>

#include <string>

#include "frida-gum.h"
#include "Util.h"

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

    #if __aarch64__
	
        //ResourcePackManager::ResourcePackManager
        ResourcePackManager_ResourcePackManager = FindSignatures(minecraftpeDetails,
            //1.20.50.20	//0x9668B40
			//1.20.73.01	//0x88BC608
			//1.20.81.01	//0x8D83E80
			//1.21.10.23	//
			//1.21.00.03	//
            "FF 03 03 D1 FD 7B 07 A9 FD C3 01 91 F9 43 00 F9 F8 5F 09 A9 F6 57 0A A9 F4 4F 0B A9 59 D0 3B D5 F6 03 03 2A 28 17 40 F9 F5 03 02 AA F3 03 00 AA A8 83 1F F8 28 10 40 F9"
        );
		
		/*
            //1.20.50.20 preview
			//1.20.73.01	//sub_88BC608+228
			FF 03 03 D1 FD 7B 07 A9		FD C3 01 91 F9 43 00 F9	
			F8 5F 09 A9 F6 57 0A A9 	F4 4F 0B A9 59 D0 3B D5 
			F6 03 03 2A 28 17 40 F9 	F5 03 02 AA F3 03 00 AA
			A8 83 1F F8 28 10 40 F9
		*/
		
		
        //AppPlatform::readAssetFile
        AppPlatform_readAssetFile = FindSignatures(minecraftpeDetails,
			//1.20.81.01
			//1.20.80.21 preview
			//1.21.10.23
			//1.21.00.03
			"FF 83 04 D1 FD 7B 0E A9 FD 83 03 91 FC 5F 0F A9 F6 57 10 A9 F4 4F 11 A9 57 D0 3B D5 F3 03 08 AA E8 16 40 F9 A8 83 1F F8 FF FF 02 A9"
		);	
		
		
		/*
			//1.20.81.01	//0x5C12B88
			"FF 83 04 D1 FD 7B 0E A9 FD 83 03 91 FC 5F 0F A9 F6 57 10 A9 F4 4F 11 A9 57 D0 3B D5 F3 03 08 AA E8 16 40 F9 A8 83 1F F8 FF FF 02 A9 A8 63 01 D1 E0 A3 00 91 22 04 80 52 E3 03 1F 2A 03 52 D3 95 A0 63 01 D1 BE 17 D2 95 C0 00 00 36 E1 07 04 D0 E0 03 13 AA 21 50 44 F9 D4 41 E5 95 1B 00 00 14"
			
									 FF 83 04 D1 FD 7B 0E A9 
			FD 83 03 91 FC 5F 0F A9  F6 57 10 A9 F4 4F 11 A9 
			57 D0 3B D5 F3 03 08 AA  E8 16 40 F9 A8 83 1F F8 
			FF FF 02 A9 A8 63 01 D1  E0 A3 00 91 22 04 80 52 
			E3 03 1F 2A 03 52 D3 95  A0 63 01 D1 BE 17 D2 95 
			C0 00 00 36 E1 07 04 D0  E0 03 13 AA 21 50 44 F9 
			D4 41 E5 95 1B 00 00 14 
			
			//1.20.50.20 for real	//0xC8E3CC4
			"FF 03 02 D1 FD 7B 02 A9 FD 83 00 91 FB 1B 00 F9 FA 67 04 A9 F8 5F 05 A9 F6 57 06 A9 F4 4F 07 A9 5B D0 3B D5 F3 03 08 AA 69 17 40 F9 A9 83 1F F8 09 40 54 39 E9 04 00 34 C8 E9 00 D0 F4 03 01 AA F5 03 00 AA 00 0D 41 F9 20 0A 00 B4 88 02 40 39 62 00 80 52 89 0A 40 F9 1F 01 00 72 21 15 94 9A C7 49 50 94 40 09 00 B4 F6 03 00 AA C8 49 50 94 F9 03 00 AA 3F 03 00 71 6D 03 00 54 E0 03 16 AA 47 16 52 94 7F FE 00 A9 7F 02 00 F9 A0 06 00 B4 28 7F 60 D3 E9 43 50 B2 E9 FD DF F2 1F 01 09 EB 62 09 00 54 F7 03 00 AA 38 7F 40 93 09 FD 60 D3 3F 5D 00 F1 E2 01 00 54 EA 03 00 91 29 7B 1F 53 59 01 40 B2 E9 03 00 39 48 02 00 B5 15 00 00 14 C1 DD 00 F0 E0 03 13 AA 21 30 45 F9 F0 28 50 94 2F 00 00 14 7F 7E 00 A9 7F 0A 00 F9 1D 00 00 14 08 43 00 91 1A ED 7C 92 E0 03 1A AA D8 28 50 94 48 03 40 B2 F9 03 00 AA F8 83 00 A9 E8 03 00 F9 E0 03 19 AA E1 03 1F 2A E2 03 18 AA F0 2B 50 94 68 02 40 39 3F 6B 38 38 68 00 00 36 60 0A 40 F9 A7 28 50 94 E0 03 C0 3D E1 03 17 AA E8 0B 40 F9 E2 03 18 AA 60 02 80 3D 68 0A 00 F9 68 02 40 39 69 0A 40 F9 1F 01 00 72 20 15 93 9A D8 28 50 94"

		    //1.20.73.01   //0x5A0FEB0
            "FF 03 04 D1 FD 7B 0C A9 FD 03 03 91 FC 5F 0D A9 F6 57 0E A9 F4 4F 0F A9 57 D0 3B D5 F3 03 08 AA E8 16 40 F9 A8 83 1F F8 FF 7F 02 A9"
	
						FF 03 04 D1 	FD 7B 0C A9 FD 03 03 91 
			FC 5F 0D A9 F6 57 0E A9 	F4 4F 0F A9 57 D0 3B D5 
			F3 03 08 AA E8 16 40 F9 	A8 83 1F F8	FF 7F 02 A9
			
			//1.20.80.21 preview	//same as 1.20.81
			"FF 83 04 D1 FD 7B 0E A9 FD 83 03 91 FC 5F 0F A9 F6 57 10 A9 F4 4F 11 A9 57 D0 3B D5 F3 03 08 AA E8 16 40 F9 A8 83 1F F8 FF FF 02 A9"
		*/

		
		
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

                    if (result) {
                        #ifdef DEBUG
                            printf("ResourcePackManager::load returned true\n");
                        #endif
                        if (!resourceStream.empty()) {
                            state->retstr->assign(resourceStream);
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
