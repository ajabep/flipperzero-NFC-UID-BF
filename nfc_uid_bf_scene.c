#include "nfc_uid_bf.h"
#include "nfc_uid_bf_scene_main_menu.h"
#include "nfc_uid_bf_scene_fuzzing.h"
#include "nfc_uid_bf_scene_list_saved_uid.h"
#include "nfc_uid_bf_scene_save_uid.h"
#include "nfc_uid_bf_scene_choose_uid_pattern.h"
#include "nfc_uid_bf_scene_confirmation_saved.h"

// Generate scene on_enter handlers array
#define ADD_SCENE(name, id) nfc_uid_bf_scene_on_enter_##name,
void (*const nfc_uid_bf_scene_on_enter_handlers[])(void*) = {
#include "nfc_scene_config.h"
};
#undef ADD_SCENE

// Generate scene on_event handlers array
#define ADD_SCENE(name, id) nfc_uid_bf_scene_on_event_##name,
bool (*const nfc_uid_bf_scene_on_event_handlers[])(void* context, SceneManagerEvent event) = {
#include "nfc_scene_config.h"
};
#undef ADD_SCENE

// Generate scene on_exit handlers array
#define ADD_SCENE(name, id) nfc_uid_bf_scene_on_exit_##name,
void (*const nfc_uid_bf_scene_on_exit_handlers[])(void* context) = {
#include "nfc_scene_config.h"
};
#undef ADD_SCENE

const SceneManagerHandlers nfc_uid_bf_scene_event_handlers = {
    .on_enter_handlers = nfc_uid_bf_scene_on_enter_handlers,
    .on_event_handlers = nfc_uid_bf_scene_on_event_handlers,
    .on_exit_handlers = nfc_uid_bf_scene_on_exit_handlers,
    .scene_num = NfcUidBfScene_count};

void nfc_uid_bf_scene_manager_free(NfcUidBf* app) {
    scene_manager_free(app->scene_manager);
    app->scene_manager = NULL;
}

void nfc_uid_bf_scene_manager_init(NfcUidBf* app) {
    app->scene_manager = scene_manager_alloc(&nfc_uid_bf_scene_event_handlers, app);
}
