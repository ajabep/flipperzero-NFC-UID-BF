#include "nfc_uid_bf_scene_save_uid.h"

/* TODO
dolphin_deed(DolphinDeedNfcEmulate);
*/

void nfc_uid_bf_scene_on_enter_save_uid(void* context) {
    NfcUidBf* app = context;
    UNUSED(app);
}

bool nfc_uid_bf_scene_on_event_save_uid(void* context, SceneManagerEvent event) {
    NfcUidBf* app = context;
    UNUSED(app);
    UNUSED(event);
    bool consumed = false;
    return consumed;
}

void nfc_uid_bf_scene_on_exit_save_uid(void* context) {
    NfcUidBf* app = context;
    if(app->saving_uid) {
        furi_string_free(app->saving_uid);
    }
}

// nfc_scene_save_name_on_enter
