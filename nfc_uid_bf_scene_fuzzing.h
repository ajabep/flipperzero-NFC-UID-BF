#ifndef NFC_UID_BF_SCENE_FUZZING_HEADER
#define NFC_UID_BF_SCENE_FUZZING_HEADER

#include "nfc_uid_bf.h"

void nfc_uid_bf_scene_on_enter_fuzzing(void* context);
bool nfc_uid_bf_scene_on_event_fuzzing(void* context, SceneManagerEvent event);
void nfc_uid_bf_scene_on_exit_fuzzing(void* context);

#endif // NFC_UID_BF_SCENE_FUZZING_HEADER
