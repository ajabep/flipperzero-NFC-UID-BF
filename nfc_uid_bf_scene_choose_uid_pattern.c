#include "nfc_uid_bf_scene_choose_uid_pattern.h"

/* it does not exists in the firmware :sob: */
size_t strnlen(const char* str, size_t length) {
    size_t i = 0;
    for(i = 0; i < length; ++i) {
        if(!str[i]) {
            return i;
        }
    }
    return -1;
}

bool nfc_uid_bf_validate_pattern(const char* text, FuriString* error, void* context) {
    // TODO Add possibility to add _ as a separator for being easier to read
    UNUSED(context);
#define RETURN_ERROR(msg, ...)                                     \
    do {                                                           \
        if(!error) {                                               \
            error = furi_string_alloc();                           \
        } else {                                                   \
            furi_string_reset(error);                              \
        }                                                          \
        furi_string_printf(error, msg __VA_OPT__(, ) __VA_ARGS__); \
        return false;                                              \
    } while(0)

    size_t text_len = strnlen(text, NFC_UID_BF_TEXT_STORE_SIZE);

    if(text_len < MF_DESFIRE_UID_SIZE * 2) {
        RETURN_ERROR(
            "UID Pattern\n"
            "not long\n"
            "enough.\n"
            "Should be %d",
            MF_DESFIRE_UID_SIZE * 2);
    }
    if(text_len > MF_DESFIRE_UID_SIZE * 2) {
        RETURN_ERROR(
            "UID Pattern\n"
            "too long.\n"
            "Should be %d",
            MF_DESFIRE_UID_SIZE * 2);
    }

    uint8_t i;
    for(i = 0; i < text_len; ++i) {
        if(text[i] >= '0' && text[i] <= '9') {
            continue;
        }
        if(text[i] >= 'A' && text[i] <= 'F') {
            continue;
        }
        if(text[i] >= 'a' && text[i] <= 'f') {
            continue;
        }
        if(text[i] == 'X' || text[i] == 'x') {
            continue;
        }
        RETURN_ERROR(
            "Invalid char:\n"
            "%c. Use only\n"
            "Hex chars or\n"
            "x for fuzz pos",
            text[i]);
    }
    return true;
}

void nfc_uid_bf_load_pattern_from_string(void* context) {
    NfcUidBf* app = context;
    furi_string_set_str(app->pattern, app->text_store);
    furi_string_reset(app->current_uid);
    scene_manager_next_scene(app->scene_manager, NfcUidBfScene_Fuzzing);
}

void nfc_uid_bf_scene_on_enter_choose_uid_pattern(void* context) {
    NfcUidBf* app = context;
    TextInput* text_input = app->text_input;

    bool name_is_empty = furi_string_empty(app->pattern);
    if(name_is_empty) {
        app->text_store[0] = '\0';
    } else {
        strncpy(app->text_store, furi_string_get_cstr(app->pattern), NFC_UID_BF_TEXT_STORE_SIZE);
    }

    text_input_reset(text_input);
    text_input_set_header_text(text_input, "Pattern (hex, X for the fuzz)");
    text_input_set_validator(text_input, nfc_uid_bf_validate_pattern, app);
    text_input_set_result_callback(
        text_input,
        nfc_uid_bf_load_pattern_from_string,
        app,
        app->text_store,
        NFC_UID_BF_TEXT_STORE_SIZE,
        true);

    view_dispatcher_switch_to_view(app->view_dispatcher, NfcUidBfView_ChoosingUidPattern);
}

bool nfc_uid_bf_scene_on_event_choose_uid_pattern(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    bool consumed = false;
    return consumed;
}

void nfc_uid_bf_scene_on_exit_choose_uid_pattern(void* context) {
    NfcUidBf* app = context;
    text_input_set_validator(app->text_input, NULL, NULL);
    text_input_reset(app->text_input);
}
