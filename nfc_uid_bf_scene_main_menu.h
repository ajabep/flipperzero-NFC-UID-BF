#ifndef NFC_UID_BF_SCENE_MENU_HEADER
#define NFC_UID_BF_SCENE_MENU_HEADER

#include "nfc_uid_bf.h"

typedef enum {
    NfcUidBfMenuSelection_ListingSaved,
    NfcUidBfMenuSelection_Choose
} NfcUidBfMenuSelection;

void nfc_uid_bf_scene_on_enter_main_menu(void* context);
bool nfc_uid_bf_scene_on_event_main_menu(void* context, SceneManagerEvent event);
void nfc_uid_bf_scene_on_exit_main_menu(void* context);

void nfc_uid_bf_menu_callback_main_menu(void* context, uint32_t index);

#endif // NFC_UID_BF_SCENE_MENU_HEADER
