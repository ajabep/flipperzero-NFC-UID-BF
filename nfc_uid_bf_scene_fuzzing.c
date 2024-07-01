#include "nfc_uid_bf_scene_fuzzing.h"

/* TODO
dolphin_deed(DolphinDeedNfcEmulate);
*/

void nfc_uid_bf_string_to_uppercase(FuriString* string) {
    char c;
    size_t i = 0;
    size_t len_pattern = furi_string_size(string);
    for(i = 0; i < len_pattern; ++i) {
        c = furi_string_get_char(string, i);
        if((c >= 'a' && c <= 'f') || c == 'x') {
            furi_string_set_char(string, i, c - 'a' + 'A');
        }
    }
}

void nfc_uid_bf_fuzzing_dialog_callback(DialogExResult result, void* context) {
    NfcUidBf* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, result);
}

void nfc_uid_bf_current_uid_init(NfcUidBf* app) {
    char c;
    size_t i = 0;
    size_t len_pattern = furi_string_size(app->pattern);
    if(furi_string_empty(app->current_uid)) {
        furi_string_set(app->current_uid, app->pattern);
    }
    for(i = 0; i < len_pattern; ++i) {
        c = furi_string_get_char(app->pattern, i);
        if(c == 'X') {
            // Init the fuzz positions to 0
            furi_string_set_char(app->current_uid, i, '0');
        } else {
            furi_string_set_char(app->current_uid, i, c);
        }
    }
}

const char* nfc_uid_bf_current_uid_get_str(NfcUidBf* app) {
    if(furi_string_empty(app->current_uid)) {
        nfc_uid_bf_current_uid_init(app);
    }
    return furi_string_get_cstr(app->current_uid);
}

void nfc_uid_bf_current_uid_get_intarray(NfcUidBf* app, uint8_t* uid, int uid_len) {
    int i;
    if(furi_string_empty(app->current_uid)) {
        nfc_uid_bf_current_uid_init(app);
    }
    char c1, c2;
    uint8_t c3;
    for(i = 0; i < uid_len; ++i) {
        c1 = furi_string_get_char(app->current_uid, i * 2);
        c2 = furi_string_get_char(app->current_uid, 1 + (i * 2));
        if(c1 < 'A') {
            c3 = c1 - '0';
        } else {
            c3 = c1 - 'A' + 0xa;
        }
        c3 *= 0x10;
        if(c2 < 'A') {
            c3 += c2 - '0';
        } else {
            c3 += c2 - 'A' + 0xa;
        }
        uid[i] = c3;
    }
}

void nfc_uid_bf_current_uid_inc(NfcUidBf* app) {
    char c_c;
    int idx = furi_string_size(app->pattern);
    for(--idx; idx >= 0; --idx) {
        c_c = furi_string_get_char(app->current_uid, idx);
        if(furi_string_get_char(app->pattern, idx) == 'X') {
            if(c_c == '9') {
                furi_string_set_char(app->current_uid, idx, 'A');
            } else if(c_c == 'F') {
                furi_string_set_char(app->current_uid, idx, '0');
                continue; // Let's increase the next hex char
            } else {
                furi_string_set_char(app->current_uid, idx, c_c + 1);
            }
            break;
        }
    }
}

void nfc_uid_bf_current_uid_dec(NfcUidBf* app) {
    char c_c;
    int idx = furi_string_size(app->pattern);
    for(--idx; idx >= 0; --idx) {
        c_c = furi_string_get_char(app->current_uid, idx);
        if(furi_string_get_char(app->pattern, idx) == 'X') {
            if(c_c == 'A') {
                furi_string_set_char(app->current_uid, idx, '9');
            } else if(c_c == '0') {
                furi_string_set_char(app->current_uid, idx, 'F');
                continue; // Let's increase the next hex char
            } else {
                furi_string_set_char(app->current_uid, idx, c_c - 1);
            }
            break;
        }
    }
}

void nfc_uid_bf_current_uid_refresh(NfcUidBf* app) {
    const char* currentUid = nfc_uid_bf_current_uid_get_str(app);

    furi_string_printf(
        app->tmp_str,
        "Pattern:\n"
        "%s\n"
        "Current:\n"
        "%s",
        furi_string_get_cstr(app->pattern),
        currentUid);

    dialog_ex_set_text(
        app->dialog_ex, furi_string_get_cstr(app->tmp_str), 2, 12, AlignLeft, AlignTop);

    view_dispatcher_switch_to_view(app->view_dispatcher, NfcUidBfView_Fuzzing);
}

NfcCommand nfc_uid_bf_listener_ignore_callback(NfcGenericEvent event, void* context) {
    UNUSED(event);
    UNUSED(context);
    return NfcCommandContinue;
}

void nfc_uid_bf_listener_start(NfcUidBf* app) {
    size_t uid_len = 7;
    uint8_t* uid = malloc(uid_len);
    MfDesfireData* blank = mf_desfire_alloc();
    nfc_uid_bf_current_uid_get_intarray(app, uid, uid_len);
    nfc_device_set_data(app->nfc_device, NfcProtocolMfDesfire, blank);
    nfc_device_set_uid(app->nfc_device, uid, uid_len);

    const Iso14443_4aData* iso14443_4a_data =
        nfc_device_get_data(app->nfc_device, NfcProtocolIso14443_4a);

    app->listener = nfc_listener_alloc(app->nfc, NfcProtocolIso14443_4a, iso14443_4a_data);
    nfc_listener_start(app->listener, nfc_uid_bf_listener_ignore_callback, NULL);
    free(uid);
}

void nfc_uid_bf_listener_stop(NfcUidBf* app) {
    nfc_listener_stop(app->listener);
    nfc_listener_free(app->listener);
}

void nfc_uid_bf_listener_update(NfcUidBf* app) {
    nfc_uid_bf_listener_stop(app);
    nfc_uid_bf_listener_start(app);
}

void nfc_uid_bf_scene_on_enter_fuzzing(void* context) {
    NfcUidBf* app = context;
    DialogEx* dialog_ex = app->dialog_ex;
    nfc_uid_bf_string_to_uppercase(app->pattern);

    dialog_ex_reset(app->dialog_ex);
    dialog_ex_set_left_button_text(dialog_ex, "Prev");
    dialog_ex_set_center_button_text(dialog_ex, "Save");
    dialog_ex_set_right_button_text(dialog_ex, "Next");
    dialog_ex_set_header(dialog_ex, "Emulating UID", 64, 0, AlignCenter, AlignTop);

    dialog_ex_set_context(dialog_ex, app);
    dialog_ex_set_result_callback(dialog_ex, nfc_uid_bf_fuzzing_dialog_callback);
    nfc_uid_bf_current_uid_refresh(app);

    nfc_uid_bf_listener_start(app);

    notification_message(app->notifications, &sequence_blink_start_magenta);
}

bool nfc_uid_bf_scene_on_event_fuzzing(void* context, SceneManagerEvent event) {
    NfcUidBf* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == DialogExResultRight) {
            nfc_uid_bf_current_uid_inc(app);
            nfc_uid_bf_listener_update(app);
            nfc_uid_bf_current_uid_refresh(app);
            consumed = true;
        } else if(event.event == DialogExResultLeft) {
            nfc_uid_bf_current_uid_dec(app);
            nfc_uid_bf_listener_update(app);
            nfc_uid_bf_current_uid_refresh(app);
            consumed = true;
        } else if(event.event == DialogExResultCenter) {
            furi_string_set(app->saving_uid, app->current_uid);
            scene_manager_next_scene(app->scene_manager, NfcUidBfScene_SaveUid);
            consumed = true;
        }
    }

    return consumed;
}

void nfc_uid_bf_scene_on_exit_fuzzing(void* context) {
    NfcUidBf* app = context;
    nfc_uid_bf_listener_stop(app);
    dialog_ex_reset(app->dialog_ex);
    notification_message(app->notifications, &sequence_blink_stop);
}

// nfc_scene_emulate_on_enter
