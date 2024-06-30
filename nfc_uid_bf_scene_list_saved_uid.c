#include "nfc_uid_bf_scene_list_saved_uid.h"

bool nfc_uid_bf_load_file(NfcUidBf* app, FuriString* path, bool show_dialog) {
    furi_assert(app);
    furi_assert(path);
    UNUSED(show_dialog);
    bool result = false;

    // nfc_supported_cards_load_cache(instance->nfc_supported_cards);

    // FuriString* load_path = furi_string_alloc();
    // if(nfc_has_shadow_file_internal(instance, path)) { //-V1051
    //     nfc_set_shadow_file_path(path, load_path);
    // } else if(furi_string_end_with(path, NFC_APP_SHADOW_EXTENSION)) {
    //     size_t path_len = furi_string_size(path);
    //     furi_string_set_n(load_path, path, 0, path_len - 4);
    //     furi_string_cat_printf(load_path, "%s", NFC_APP_EXTENSION);
    // } else {
    //     furi_string_set(load_path, path);
    // }

    // result = nfc_device_load(instance->nfc_device, furi_string_get_cstr(load_path));

    // if(result) {
    //     path_extract_filename(load_path, instance->file_name, true);
    // }

    // if((!result) && (show_dialog)) {
    //     dialog_message_show_storage_error(instance->dialogs, "Cannot load\nkey file");
    // }

    // furi_string_free(load_path);

    return result;
}

bool nfc_uid_bf_load_from_file_select(NfcUidBf* app) {
    furi_assert(app);

    DialogsFileBrowserOptions browser_options;
    dialog_file_browser_set_basic_options(
        &browser_options, NFC_UID_BF_APP_EXTENSION, NULL); // TODO add icon
    browser_options.base_path = NFC_APP_FOLDER;
    browser_options.skip_assets = true;
    browser_options.hide_dot_files = true;

    bool success = false;
    FuriString* path = furi_string_alloc_set(NFC_APP_FOLDER);
    do {
        // Input events and views are managed by file_browser
        if(!dialog_file_browser_show(app->dialogs, path, path, &browser_options)) {
            break;
        }
        success = nfc_uid_bf_load_file(app, path, true);
    } while(!success);

    return success;
}

void nfc_uid_bf_scene_on_enter_list_saved_uid(void* context) {
    NfcUidBf* app = context;

    if(nfc_uid_bf_load_from_file_select(app)) {
        scene_manager_next_scene(app->scene_manager, NfcUidBfScene_Fuzzing);
    } else {
        scene_manager_previous_scene(app->scene_manager);
    }
}

bool nfc_uid_bf_scene_on_event_list_saved_uid(void* context, SceneManagerEvent event) {
    NfcUidBf* app = context;
    UNUSED(app);
    UNUSED(event);
    bool consumed = false;
    return consumed;
}

void nfc_uid_bf_scene_on_exit_list_saved_uid(void* context) {
    NfcUidBf* app = context;
    UNUSED(app);
}

// nfc_scene_file_select_on_enter
// nfc_scene_saved_menu_on_enter
