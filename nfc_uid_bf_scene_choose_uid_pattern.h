#ifndef NFC_UID_BF_SCENE_CHOOSE_UID_PATTERN_HEADER
#define NFC_UID_BF_SCENE_CHOOSE_UID_PATTERN_HEADER

#include "nfc_uid_bf.h"

void nfc_uid_bf_scene_on_enter_choose_uid_pattern(void* context);
bool nfc_uid_bf_scene_on_event_choose_uid_pattern(void* context, SceneManagerEvent event);
void nfc_uid_bf_scene_on_exit_choose_uid_pattern(void* context);

#endif // NFC_UID_BF_SCENE_CHOOSE_UID_PATTERN_HEADER
