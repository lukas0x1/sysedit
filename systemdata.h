//
// Created by ciel on 18/11/2023.
//

#ifndef SAVEEDITOR_SYSTEMDATA_H
#define SAVEEDITOR_SYSTEMDATA_H
#include <vector>







namespace save{

    struct SaveData{
        char time[41];
        unsigned short title;
        unsigned short scenario;
        unsigned short in_data;
    };

    struct SaveSlotData{
        SaveData save_data[100];
    };

    struct OpenScenarioData{
        std::byte GS1_Scenario_enable;
        std::byte GS2_Scenario_enable;
        std::byte GS3_Scenario_enable;
    };

    struct OptionWork{
        unsigned short bgm_value;
        unsigned short se_value;
        unsigned short skip_type;
        unsigned short shake_type;
        unsigned short vibe_type;
        unsigned short window_type;
        unsigned short language_type;
        std::byte window_mode;
        unsigned int resolution_w;
        unsigned int resolution_h;
        std::byte vsync;
        unsigned short key_config[30];
    };

    struct OptionWork_Android{
        unsigned short bgm_value;
        unsigned short se_value;
        unsigned short skip_type;
        unsigned short shake_type;
        unsigned short vibe_type;
        unsigned short flash_type;
        unsigned short window_type;
        unsigned short language_type;
    };



    struct TrophyWork{
        int message_flag[96];
    };

    struct ReserveWork{
        int reserve[200];
    };

    struct SystemData{
        int save_ver;
        SaveSlotData slot_data;
        OpenScenarioData sce_data;
        OptionWork option_work;
        TrophyWork trophy_work;
        ReserveWork reserve_work;
    };

    struct SystemData_Android{
        int game_ver;
        int save_ver;
        SaveSlotData slot_data;
        OpenScenarioData sce_data;
        OptionWork_Android option_work;
        TrophyWork trophy_work;
        ReserveWork reserve_work;
    };

    enum save_type{
        Invalid,
        Android,
        Steam,
        Switch,
        Ios
    };

    extern char path[260]; //MAX_PATH
    extern int save_type;
    extern SystemData_Android *systemDataAndroid;
    extern SystemData *systemData;


    bool selectFile();
    void loadSave();
    void SaveData();
};

#endif //SAVEEDITOR_SYSTEMDATA_H
