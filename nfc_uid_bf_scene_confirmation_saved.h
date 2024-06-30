#ifndef NFC_UID_BF_SCENE_CONFIRMATION_SAVED_HEADER
#define NFC_UID_BF_SCENE_CONFIRMATION_SAVED_HEADER

#include "nfc_uid_bf.h"

void nfc_uid_bf_scene_on_enter_confirmation_saved(void* context);
bool nfc_uid_bf_scene_on_event_confirmation_saved(void* context, SceneManagerEvent event);
void nfc_uid_bf_scene_on_exit_confirmation_saved(void* context);

#endif // NFC_UID_BF_SCENE_CONFIRMATION_SAVED_HEADER
