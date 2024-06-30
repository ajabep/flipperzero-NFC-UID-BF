#ifndef NFC_UID_BF_SCENE_SAVING_HEADER
#define NFC_UID_BF_SCENE_SAVING_HEADER

#include "nfc_uid_bf.h"

void nfc_uid_bf_scene_on_enter_save_uid(void* context);
bool nfc_uid_bf_scene_on_event_save_uid(void* context, SceneManagerEvent event);
void nfc_uid_bf_scene_on_exit_save_uid(void* context);

#endif // NFC_UID_BF_SCENE_SAVING_HEADER
