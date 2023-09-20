#include "frida-gum.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

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

#ifdef V1_19_20_02
    #define RESOURCE_PACK_MANAGER_ADDRESS (minecraftpeBaseAddr + 0x5624220)
    #define READ_ASSET_FILE_ADDRESS       (minecraftpeBaseAddr + 0x490044C)
    #define RESOURCE_LOCATION_ADDRESS     (gum_module_find_export_by_name("libminecraftpe.so", "_ZN16ResourceLocationC2ERKN4Core4PathE"))
#endif
#ifdef V1_19_21_01
    #define RESOURCE_PACK_MANAGER_ADDRESS (minecraftpeBaseAddr + 0x56255DC)
    #define READ_ASSET_FILE_ADDRESS       (minecraftpeBaseAddr + 0x490044C)
    #define RESOURCE_LOCATION_ADDRESS     (gum_module_find_export_by_name("libminecraftpe.so", "_ZN16ResourceLocationC2ERKN4Core4PathE"))
#endif
#ifdef V1_19_40_02
    #define RESOURCE_PACK_MANAGER_ADDRESS (minecraftpeBaseAddr + 0x6D5EBF0)
    #define READ_ASSET_FILE_ADDRESS       (minecraftpeBaseAddr + 0x4C6B63C)
    #define RESOURCE_LOCATION_ADDRESS     (gum_module_find_export_by_name("libminecraftpe.so", "_ZN16ResourceLocationC2ERKN4Core4PathE"))
#endif
#ifdef V1_19_51_01
    #define RESOURCE_PACK_MANAGER_ADDRESS (minecraftpeBaseAddr + 0x68FF3D0)
    #define READ_ASSET_FILE_ADDRESS       (minecraftpeBaseAddr + 0x4C174FC)
    #define RESOURCE_LOCATION_ADDRESS     (gum_module_find_export_by_name("libminecraftpe.so", "_ZN16ResourceLocationC2ERKN4Core4PathE"))
#endif
#ifdef V1_19_60_03
    #define RESOURCE_PACK_MANAGER_ADDRESS (minecraftpeBaseAddr + 0x8950318)
    #define READ_ASSET_FILE_ADDRESS       (minecraftpeBaseAddr + 0x776669C)
    #define RESOURCE_LOCATION_ADDRESS     (gum_module_find_export_by_name("libminecraftpe.so", "_ZN16ResourceLocationC2ERKN4Core4PathE"))
#endif
#ifdef V1_19_62_01
    #define RESOURCE_PACK_MANAGER_ADDRESS (minecraftpeBaseAddr + 0x894BDDC)
    #define READ_ASSET_FILE_ADDRESS       (minecraftpeBaseAddr + 0x776333C)
    #define RESOURCE_LOCATION_ADDRESS     (gum_module_find_export_by_name("libminecraftpe.so", "_ZN16ResourceLocationC2ERKN4Core4PathE"))
#endif
#ifdef V1_19_63_01
    #define RESOURCE_PACK_MANAGER_ADDRESS (minecraftpeBaseAddr + 0x894D058)
    #define READ_ASSET_FILE_ADDRESS       (minecraftpeBaseAddr + 0x7763820)
    #define RESOURCE_LOCATION_ADDRESS     (gum_module_find_export_by_name("libminecraftpe.so", "_ZN16ResourceLocationC2ERKN4Core4PathE"))
#endif
#ifdef V1_19_70_22
    #define RESOURCE_PACK_MANAGER_ADDRESS (minecraftpeBaseAddr + 0x7AC02B0)
    #define READ_ASSET_FILE_ADDRESS       (minecraftpeBaseAddr + 0x7A74764)
    #define RESOURCE_LOCATION_ADDRESS     (gum_module_find_export_by_name("libminecraftpe.so", "_ZN16ResourceLocationC2ERKN4Core4PathE"))
#endif
#ifdef V1_19_70_02
    #define RESOURCE_PACK_MANAGER_ADDRESS (minecraftpeBaseAddr + 0x791FE24)
    #define READ_ASSET_FILE_ADDRESS       (minecraftpeBaseAddr + 0x78D641C)
    #define RESOURCE_LOCATION_ADDRESS     (gum_module_find_export_by_name("libminecraftpe.so", "_ZN16ResourceLocationC2ERKN4Core4PathE"))
#endif
#ifdef V1_19_71_02
    #define RESOURCE_PACK_MANAGER_ADDRESS (minecraftpeBaseAddr + 0x7920BF8)
    #define READ_ASSET_FILE_ADDRESS       (minecraftpeBaseAddr + 0x78D71F0)
    #define RESOURCE_LOCATION_ADDRESS     (gum_module_find_export_by_name("libminecraftpe.so", "_ZN16ResourceLocationC2ERKN4Core4PathE"))
#endif
#ifdef V1_19_73_02
    #define RESOURCE_PACK_MANAGER_ADDRESS (minecraftpeBaseAddr + 0x7920DC4)
    #define READ_ASSET_FILE_ADDRESS       (minecraftpeBaseAddr + 0x78D73BC)
    #define RESOURCE_LOCATION_ADDRESS     (gum_module_find_export_by_name("libminecraftpe.so", "_ZN16ResourceLocationC2ERKN4Core4PathE"))
#endif
#ifdef V1_19_80_02
    #define RESOURCE_PACK_MANAGER_ADDRESS (minecraftpeBaseAddr + 0x7C99A24)
    #define READ_ASSET_FILE_ADDRESS       (minecraftpeBaseAddr + 0x7C4C224)
    #define RESOURCE_LOCATION_ADDRESS     (gum_module_find_export_by_name("libminecraftpe.so", "_ZN16ResourceLocationC2ERKN4Core4PathE"))
#endif
#ifdef V1_19_81_01
    #define RESOURCE_PACK_MANAGER_ADDRESS (minecraftpeBaseAddr + 0x7C997DC)
    #define READ_ASSET_FILE_ADDRESS       (minecraftpeBaseAddr + 0x7C4BFDC)
    #define RESOURCE_LOCATION_ADDRESS     (gum_module_find_export_by_name("libminecraftpe.so", "_ZN16ResourceLocationC2ERKN4Core4PathE"))
#endif
#ifdef V1_19_83_01
    #define RESOURCE_PACK_MANAGER_ADDRESS (minecraftpeBaseAddr + 0x7C9D48C)
    #define READ_ASSET_FILE_ADDRESS       (minecraftpeBaseAddr + 0x7C4FC8C)
    #define RESOURCE_LOCATION_ADDRESS     (gum_module_find_export_by_name("libminecraftpe.so", "_ZN16ResourceLocationC2ERKN4Core4PathE"))
#endif
#ifdef V1_20_0_20
    #define RESOURCE_PACK_MANAGER_ADDRESS (minecraftpeBaseAddr + 0x870D984)
    #define READ_ASSET_FILE_ADDRESS       (minecraftpeBaseAddr + 0x86561C4)
    #define RESOURCE_LOCATION_ADDRESS     (gum_module_find_export_by_name("libminecraftpe.so", "_ZN16ResourceLocationC2ERKN4Core4PathE"))
#endif
#ifdef V1_20_0_01
    #define RESOURCE_PACK_MANAGER_ADDRESS (minecraftpeBaseAddr + 0x83BAF14)
    #define READ_ASSET_FILE_ADDRESS       (minecraftpeBaseAddr + 0x8302340)
    #define RESOURCE_LOCATION_ADDRESS     (gum_module_find_export_by_name("libminecraftpe.so", "_ZN16ResourceLocationC2ERKN4Core4PathE"))
#endif
#ifdef V1_20_1_02
    #define RESOURCE_PACK_MANAGER_ADDRESS (minecraftpeBaseAddr + 0x83BB978)
    #define READ_ASSET_FILE_ADDRESS       (minecraftpeBaseAddr + 0x8302DA4)
    #define RESOURCE_LOCATION_ADDRESS     (gum_module_find_export_by_name("libminecraftpe.so", "_ZN16ResourceLocationC2ERKN4Core4PathE"))
#endif
#ifdef V1_20_10_01
    #define RESOURCE_PACK_MANAGER_ADDRESS (minecraftpeBaseAddr + 0x82B5050)
    #define READ_ASSET_FILE_ADDRESS       (minecraftpeBaseAddr + 0x80A54A4)
    #define RESOURCE_LOCATION_ADDRESS     (gum_module_find_export_by_name("libminecraftpe.so", "_ZN16ResourceLocationC2ERKN4Core4PathE"))
#endif
#ifdef V1_20_12_01
    #define RESOURCE_PACK_MANAGER_ADDRESS (minecraftpeBaseAddr + 0x82B5674)
    #define READ_ASSET_FILE_ADDRESS       (minecraftpeBaseAddr + 0x80A5AC8)
    #define RESOURCE_LOCATION_ADDRESS     (gum_module_find_export_by_name("libminecraftpe.so", "_ZN16ResourceLocationC2ERKN4Core4PathE"))
#endif
#ifdef V1_20_30_20
    #define RESOURCE_PACK_MANAGER_ADDRESS (minecraftpeBaseAddr + 0x865EF24)
    #define READ_ASSET_FILE_ADDRESS       (minecraftpeBaseAddr + 0x844ABA4)
    #define RESOURCE_LOCATION_ADDRESS     (gum_module_find_export_by_name("libminecraftpe.so", "_ZN16ResourceLocationC2ERKN4Core4PathE"))
#endif
#ifdef V1_20_30_21
    #define RESOURCE_PACK_MANAGER_ADDRESS (minecraftpeBaseAddr + 0x86E9DBC)
    #define READ_ASSET_FILE_ADDRESS       (minecraftpeBaseAddr + 0x84D5CAC)
    #define RESOURCE_LOCATION_ADDRESS     (gum_module_find_export_by_name("libminecraftpe.so", "_ZN16ResourceLocationC2ERKN4Core4PathE"))
#endif
#ifdef V1_20_15_01
    #define RESOURCE_PACK_MANAGER_ADDRESS (minecraftpeBaseAddr + 0x82B5610)
    #define READ_ASSET_FILE_ADDRESS       (minecraftpeBaseAddr + 0x80A5A64)
    #define RESOURCE_LOCATION_ADDRESS     (gum_module_find_export_by_name("libminecraftpe.so", "_ZN16ResourceLocationC2ERKN4Core4PathE"))
#endif
#ifdef V1_20_30_22
    #define RESOURCE_PACK_MANAGER_ADDRESS (minecraftpeBaseAddr + 0x873B960)
    #define READ_ASSET_FILE_ADDRESS       (minecraftpeBaseAddr + 0x851B86C)
    #define RESOURCE_LOCATION_ADDRESS     (gum_module_find_export_by_name("libminecraftpe.so", "_ZN16ResourceLocationC2ERKN4Core4PathE"))
#endif
#ifdef V1_20_30_25
    #define RESOURCE_PACK_MANAGER_ADDRESS (minecraftpeBaseAddr + 0x8760310)
    #define READ_ASSET_FILE_ADDRESS       (minecraftpeBaseAddr + 0x85495CC)
    #define RESOURCE_LOCATION_ADDRESS     (gum_module_find_export_by_name("libminecraftpe.so", "_ZN16ResourceLocationC2ERKN4Core4PathE"))
#endif
#ifdef V1_20_40_20
    #define RESOURCE_PACK_MANAGER_ADDRESS (minecraftpeBaseAddr + 0x882FB80)
    #define READ_ASSET_FILE_ADDRESS       (minecraftpeBaseAddr + 0x861A92C)
    #define RESOURCE_LOCATION_ADDRESS     (gum_module_find_export_by_name("libminecraftpe.so", "_ZN16ResourceLocationC2ERKN4Core4PathE"))
#endif
#ifdef V1_20_30_02
    #define RESOURCE_PACK_MANAGER_ADDRESS (minecraftpeBaseAddr + 0x845D7A4)
    #define READ_ASSET_FILE_ADDRESS       (minecraftpeBaseAddr + 0x8256A3C)
    #define RESOURCE_LOCATION_ADDRESS     (gum_module_find_export_by_name("libminecraftpe.so", "_ZN16ResourceLocationC2ERKN4Core4PathE"))
#endif
typedef enum _HookId HookId;
enum _HookId {
    RESOURCE_PACK_MANAGER,
    READ_ASSET_FILE
};

typedef struct _InvocationState InvocationState;
struct _InvocationState {
    bool redirect;
    void* filename;
    void* retstr;
};

GumAddress minecraftpeBaseAddr;
GumAddress ResourcePackManager_ResourcePackManager;
GumAddress AppPlatform_android23_readAssetFile;
GumAddress ResourceLocation_ResourceLocation;

gpointer resourcePackManager = NULL;

GumInterceptor* interceptor;
GumInvocationListener* listener;

void __attribute__((constructor)) init() {
    #ifdef DEBUG
        printf("[init]\n");
    #endif

    gum_init();

    minecraftpeBaseAddr = gum_module_find_base_address("libminecraftpe.so");

    ResourcePackManager_ResourcePackManager = RESOURCE_PACK_MANAGER_ADDRESS;
    AppPlatform_android23_readAssetFile = READ_ASSET_FILE_ADDRESS;
    ResourceLocation_ResourceLocation = RESOURCE_LOCATION_ADDRESS;

    interceptor = gum_interceptor_obtain();
    listener = g_object_new(LOADER_TYPE_INVOCATION_LISTENER, NULL);

    gum_interceptor_begin_transaction(interceptor);

    gum_interceptor_attach(interceptor,
        GSIZE_TO_POINTER(ResourcePackManager_ResourcePackManager),
        listener,
        GSIZE_TO_POINTER(RESOURCE_PACK_MANAGER));

    gum_interceptor_attach(interceptor,
        GSIZE_TO_POINTER(AppPlatform_android23_readAssetFile),
        listener,
        GSIZE_TO_POINTER(READ_ASSET_FILE));

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

bool std_string_is_short(void* str) {
    return (*(char*)str & 1) == 0;
}

size_t std_string_size(void* str) {
    if (std_string_is_short(str)) {
        return *(uint8_t*)str >> 1;
    } else {
        return *((size_t*)str + 1);
    }
}

char* std_string_data(void* str) {
    if (std_string_is_short(str)) {
        return (char*)str + 1;
    } else {
        return *((char**)str + 2);
    }
}

bool std_string_is_empty(void* str) {
    return std_string_size(str) == 0;
}

//==========================================================================================================================================

static void loader_invocation_listener_class_init(LoaderInvocationListenerClass* klass) {
    (void) LOADER_IS_INVOCATION_LISTENER;
    (void) glib_autoptr_cleanup_LoaderInvocationListener;
}

static void loader_invocation_listener_iface_init(gpointer g_iface, gpointer iface_data) {
    GumInvocationListenerInterface* iface = g_iface;

    iface->on_enter = loader_invocation_listener_on_enter;
    iface->on_leave = loader_invocation_listener_on_leave;
}

static void loader_invocation_listener_init(LoaderInvocationListener* self) {
}


static void loader_invocation_listener_on_enter(GumInvocationListener* listener, GumInvocationContext* ic) {
    HookId hookId = GUM_IC_GET_FUNC_DATA(ic, HookId);

    switch (hookId) {
        case RESOURCE_PACK_MANAGER: {
            gpointer needsToInitialize = gum_invocation_context_get_nth_argument(ic, 3);

            if (resourcePackManager == NULL && needsToInitialize) {
                resourcePackManager = gum_invocation_context_get_nth_argument(ic, 0);
            }
            break;
        }
        //std::string* AppPlatform::readAssetFile(AppPlatform* this, Core::Path* filename)
        case READ_ASSET_FILE: {
            InvocationState* state = GUM_IC_GET_INVOCATION_DATA(ic, InvocationState);
            state->redirect = false;
            state->filename = NULL;
            state->retstr = NULL;

            void* filename = gum_invocation_context_get_nth_argument(ic, 1);
            char* data = std_string_data(filename);

            size_t size = std_string_size(filename);
            if (size < 32) {
                return;
            }

            if (strncmp(data, "renderer/materials/", 19) == 0 && strncmp(data + size - 13, ".material.bin", 13) == 0) {
                #ifdef DEBUG
                    printf("filename=%s\n", data);
                #endif
                state->redirect = true;
                state->filename = filename;
                state->retstr = (void*)ic->cpu_context->x[8];
            }

            break;
        }
    }
}

static void loader_invocation_listener_on_leave(GumInvocationListener* listener, GumInvocationContext* ic) {
    HookId hookId = GUM_IC_GET_FUNC_DATA(ic, HookId);
    
    switch (hookId) {
        case RESOURCE_PACK_MANAGER: {
            break;
        }
        //std::string* AppPlatform::readAssetFile(AppPlatform* this, Core::Path* filename)
        case READ_ASSET_FILE: {
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
                    //bool ResourcePackManager::load(ResourcePackManager* this, ResourceLocation* resourceLocation, std::string* resourceStream)
                    bool (*load)(void*, void*, void*) = (bool (*)(void*, void*, void*))*(vptr + 2); 

                    //void ResourceLocation::ResourceLocation(ResourceLocation* this, Core::Path* path)
                    void (*ResourceLocation)(void*, void*) = (void (*)(void*, void*))ResourceLocation_ResourceLocation;

                    void* location = malloc(0x50);
                    memset(location, 0, 0x50);
                    ResourceLocation(location, state->filename);

                    void* resourceStream = malloc(3 * sizeof(size_t));
                    memset(resourceStream, 0, 3 * sizeof(size_t));
                    bool result = load(resourcePackManager, location, resourceStream);

                    if (result) {
                        #ifdef DEBUG
                            printf("ResourcePackManager::load returned true\n");
                        #endif
                        if (!std_string_is_empty(resourceStream)) {
                            if (!std_string_is_short(state->retstr) && std_string_data(state->retstr) != NULL) {
                                free(std_string_data(state->retstr));
                            }

                            memcpy(state->retstr, resourceStream, 3 * sizeof(size_t));
                        }
                    } else {
                        #ifdef DEBUG
                            printf("ResourcePackManager::load returned false\n");
                        #endif
                    }
                    
                    #ifdef DEBUG
                        printf("\n");
                    #endif
                    
                    free(resourceStream);
                    free(location);
                }
            }
            break;
        }
    }
}
