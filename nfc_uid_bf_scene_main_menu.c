#include "nfc_uid_bf_scene_main_menu.h"

/** main menu callback - sends a custom event to the scene manager based on the menu selection */
void nfc_uid_bf_menu_callback_main_menu(void* context, uint32_t index) {
    NfcUidBf* app = context;
    NfcUidBfMenuSelection selected = index;

    switch(selected) {
    case NfcUidBfMenuSelection_Choose:
        scene_manager_handle_custom_event(app->scene_manager, NfcUidBfEvent_ShowChooseUidPattern);
        break;
    case NfcUidBfMenuSelection_ListingSaved:
        scene_manager_handle_custom_event(app->scene_manager, NfcUidBfEvent_ShowListingSaved);
        break;
    }
}

void nfc_uid_bf_scene_on_enter_main_menu(void* context) {
    NfcUidBf* app = context;
    menu_reset(app->menu);

    menu_add_item(
        app->menu,
        "Set a UID Pattern",
        NULL, // previously: &I_two,
        NfcUidBfMenuSelection_Choose,
        nfc_uid_bf_menu_callback_main_menu,
        app);
    menu_add_item(
        app->menu,
        "Saved",
        NULL, // previously: &I_one,
        NfcUidBfMenuSelection_ListingSaved,
        nfc_uid_bf_menu_callback_main_menu,
        app);

    view_dispatcher_switch_to_view(app->view_dispatcher, NfcUidBfView_Menu);
}

bool nfc_uid_bf_scene_on_event_main_menu(void* context, SceneManagerEvent event) {
    NfcUidBf* app = context;
    bool consumed = false;

    switch(event.type) {
    case SceneManagerEventTypeCustom:
        switch(event.event) {
        case NfcUidBfEvent_ShowChooseUidPattern:
            scene_manager_next_scene(app->scene_manager, NfcUidBfScene_ChooseUidPattern);
            consumed = true;
            break;
        case NfcUidBfEvent_ShowListingSaved:
            scene_manager_next_scene(app->scene_manager, NfcUidBfScene_ListSavedUid);
            consumed = true;
            break;
        }
        break;
    default: // eg. SceneManagerEventTypeBack, SceneManagerEventTypeTick
        consumed = false;
        break;
    }
    return consumed;
}

void nfc_uid_bf_scene_on_exit_main_menu(void* context) {
    NfcUidBf* app = context;
    menu_reset(app->menu);
}
