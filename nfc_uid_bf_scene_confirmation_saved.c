#include "nfc_uid_bf_scene_confirmation_saved.h"

void nfc_uid_bf_scene_on_enter_confirmation_saved(void* context) {
    NfcUidBf* app = context;
    UNUSED(app);
}

bool nfc_uid_bf_scene_on_event_confirmation_saved(void* context, SceneManagerEvent event) {
    NfcUidBf* app = context;
    UNUSED(app);
    UNUSED(event);
    bool consumed = false;
    return consumed;
}

void nfc_uid_bf_scene_on_exit_confirmation_saved(void* context) {
    NfcUidBf* app = context;
    UNUSED(app);
}

// nfc_scene_save_success_on_enter
