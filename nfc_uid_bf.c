#include "nfc_uid_bf.h"

/*
    TODO Add all the types of tags


     **************************** Protocol tree structure ***************************

                                                      (Start)
                                                         |
                                +------------------------+-----------+---------+------------+
                                |                        |           |         |            |
                           ISO14443-3A              ISO14443-3B    Felica  ISO15693-3    ST25TB
                                |                        |                     |
                +---------------+-------------+     ISO14443-4B              SLIX
                |               |             |
           ISO14443-4A   Mf Ultralight   Mf Classic
                |
           Mf Desfire

    Tag Type    | UID len (Bytes) | Note
    ------------|-----------------|---------------------------
    ISO14443-3A | 4, 7 or 10      | used behind MiFare Classic
    ISO14443-3B | 4
    Felica      | 8
    ISO15693-3  | 8
    ST25TB      | 8


    ISO14443-3A, ISO14443 4A| 4, 7 or 10 | used behind MiFare Classic
    ISO14443-3B, ISO14443 4B| 4
    Hi | 7
    Mizip | 4
    Microel | 4
    Felica | 8
    ISO15693-3 | 8 | Slix
    ST25TB | 8
    MiFare Plus | 7
    MiFare DESFire | 7
    MiFare Utralight | 7
*/

NfcUidBf* nfc_uid_bf_init() {
    NfcUidBf* app = malloc(sizeof(NfcUidBf));
    nfc_uid_bf_scene_manager_init(app);
    nfc_uid_bf_view_dispatcher_init(app);
    memset(app->text_store, 0, NFC_UID_BF_TEXT_STORE_SIZE + 1);
    app->tmp_str = furi_string_alloc();
    app->pattern = furi_string_alloc();
    app->current_uid = furi_string_alloc();
    app->saving_uid = furi_string_alloc();
    return app;
}

void nfc_uid_bf_free(NfcUidBf* app) {
    nfc_uid_bf_view_dispatcher_free(app);
    nfc_uid_bf_scene_manager_free(app);
    furi_string_free(app->tmp_str);
    furi_string_free(app->pattern);
    furi_string_free(app->current_uid);
    furi_string_free(app->saving_uid);
    free(app);
}

/** go to trace log level in the dev environment */
void nfc_uid_bf_set_log_level() {
    FURI_LOG_W(TAG, "NfcUidBf Setting log level...");
#ifdef FURI_DEBUG
    FURI_LOG_W(TAG, "... to TRACE level");
    furi_log_set_level(FuriLogLevelTrace);
#else
    FURI_LOG_W(TAG, "... to INFO level");
    furi_log_set_level(FuriLogLevelInfo);
#endif
}

int32_t nfc_uid_bf_app(void* p) {
    UNUSED(p);
    nfc_uid_bf_set_log_level();

    // create the app context struct, scene manager, and view dispatcher
    FURI_LOG_I(TAG, "NfcUidBf app starting...");
    NfcUidBf* app = nfc_uid_bf_init();

    // set the scene and launch the main loop
    Gui* gui = furi_record_open(RECORD_GUI);
    view_dispatcher_attach_to_gui(app->view_dispatcher, gui, ViewDispatcherTypeFullscreen);
    scene_manager_next_scene(app->scene_manager, NfcUidBfScene_MainMenu);
    FURI_LOG_D(TAG, "Starting dispatcher...");
    view_dispatcher_run(app->view_dispatcher);

    FURI_LOG_I(TAG, "NfcUidBf app finishing...");
    nfc_uid_bf_free(app);
    return 0;
}
