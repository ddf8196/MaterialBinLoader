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

#ifdef V1_18_32_02
    #define RESOURCE_PACK_MANAGER_ADDRESS 0x3935E2C
    #define READ_ASSET_FILE_ADDRESS 0x34B10A4
#endif
#ifdef V1_19_2_02
    #define RESOURCE_PACK_MANAGER_ADDRESS 0
    #define READ_ASSET_FILE_ADDRESS 0
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
GumAddress I18n_mResourcePackManager;
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

    I18n_mResourcePackManager = gum_module_find_export_by_name("libminecraftpe.so", "_ZN4I18n20mResourcePackManagerE");

    ResourcePackManager_ResourcePackManager = minecraftpeBaseAddr + RESOURCE_PACK_MANAGER_ADDRESS + 1;
    AppPlatform_android23_readAssetFile = minecraftpeBaseAddr + READ_ASSET_FILE_ADDRESS + 1;
    ResourceLocation_ResourceLocation = gum_module_find_export_by_name("libminecraftpe.so", "_ZN16ResourceLocationC1ERKN4Core4PathE");

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
        //std::string* AppPlatform::readAssetFile(std::string* retstr, AppPlatform* this, Core::Path* filename)
        case READ_ASSET_FILE: {
            InvocationState* state = GUM_IC_GET_INVOCATION_DATA(ic, InvocationState);
            state->redirect = false;
            state->filename = NULL;
            state->retstr = NULL;

            void* filename = gum_invocation_context_get_nth_argument(ic, 2);
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
                state->retstr = gum_invocation_context_get_nth_argument(ic, 0);
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
        //std::string* AppPlatform::readAssetFile(std::string* retstr, AppPlatform* this, Core::Path* filename)
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
