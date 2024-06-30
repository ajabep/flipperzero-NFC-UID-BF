#include "nfc_uid_bf.h"

bool nfc_uid_bf_scene_manager_custom_event_callback(void* context, uint32_t custom_event) {
    furi_assert(context);
    NfcUidBf* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, custom_event);
}

bool nfc_uid_bf_scene_manager_navigation_event_callback(void* context) {
    furi_assert(context);
    NfcUidBf* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

void nfc_uid_bf_view_dispatcher_init(NfcUidBf* app) {
    app->view_dispatcher = view_dispatcher_alloc();

    app->dialogs = furi_record_open(RECORD_DIALOGS);
    app->notifications = furi_record_open(RECORD_NOTIFICATION);

    view_dispatcher_enable_queue(app->view_dispatcher);

    app->nfc = nfc_alloc();
    app->nfc_device = nfc_device_alloc();

    // allocate each view
    app->menu = menu_alloc();
    app->dialog_ex = dialog_ex_alloc();
    app->text_input = text_input_alloc();

    // assign callback that pass events from views to the scene manager
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_custom_event_callback(
        app->view_dispatcher, nfc_uid_bf_scene_manager_custom_event_callback);
    view_dispatcher_set_navigation_event_callback(
        app->view_dispatcher, nfc_uid_bf_scene_manager_navigation_event_callback);

    // add views to the dispatcher, indexed by their enum value
    view_dispatcher_add_view(app->view_dispatcher, NfcUidBfView_Menu, menu_get_view(app->menu));
    view_dispatcher_add_view(
        app->view_dispatcher, NfcUidBfView_Fuzzing, dialog_ex_get_view(app->dialog_ex));
    view_dispatcher_add_view(
        app->view_dispatcher,
        NfcUidBfView_ChoosingUidPattern,
        text_input_get_view(app->text_input));
}

void nfc_uid_bf_view_dispatcher_free(NfcUidBf* app) {
    view_dispatcher_remove_view(app->view_dispatcher, NfcUidBfView_ChoosingUidPattern);
    view_dispatcher_remove_view(app->view_dispatcher, NfcUidBfView_Fuzzing);
    view_dispatcher_remove_view(app->view_dispatcher, NfcUidBfView_Menu);
    view_dispatcher_free(app->view_dispatcher);
    furi_record_close(RECORD_DIALOGS);
    furi_record_close(RECORD_NOTIFICATION);
    text_input_free(app->text_input);
    dialog_ex_free(app->dialog_ex);
    menu_free(app->menu);
    nfc_device_free(app->nfc_device);
    nfc_free(app->nfc);
}
