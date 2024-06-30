/*
```mermaid
stateDiagram-v2
    [*] --> MainMenu

    MainMenu --> ChooseUidPattern
    MainMenu --> ListSavedUid

    ChooseUidPattern --> Fuzzing

    ListSavedUid --> Fuzzing: Load the UID as a pattern without position to fuzz

    Fuzzing --> Fuzzing: Next UID
    Fuzzing --> Fuzzing: Previous UID
    Fuzzing --> SaveUid
    SaveUid --> ConfirmationSaved

    ConfirmationSaved --> Fuzzing
```
*/

ADD_SCENE(main_menu, MainMenu)
ADD_SCENE(choose_uid_pattern, ChooseUidPattern)
ADD_SCENE(list_saved_uid, ListSavedUid)
ADD_SCENE(fuzzing, Fuzzing)
ADD_SCENE(save_uid, SaveUid)
ADD_SCENE(confirmation_saved, ConfirmationSaved)
