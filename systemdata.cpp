//
// Created by ciel on 18/11/2023.
//

#include "systemdata.h"
#include <windows.h>
#include <fstream>

int save::save_type = save::save_type::Invalid;
char save::path[MAX_PATH];

bool save::selectFile() {
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = save::path;
    ofn.lpstrFile[0] = L'\0';
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = "systemdata"; // Add more filters if needed
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Display the file dialog
    return GetOpenFileName(&ofn) == TRUE;
}

std::vector<char> buf;
save::SystemData *save::systemData = new SystemData;
save::SystemData_Android *save::systemDataAndroid = new SystemData_Android;
void save::loadSave(){
    const char* filename = path;
    std::ifstream file(filename, std::ios::binary);
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    buf.reserve(fileSize);
    file.read(buf.data(), fileSize);
    file.close();
    switch(*(int *)buf.data()){
        case 1:
            memcpy(save::systemDataAndroid, buf.data(), sizeof(save::SystemData_Android));
            save::save_type = save::save_type::Android;
            break;
        case 4096:
            memcpy(save::systemData, buf.data(), sizeof(save::SystemData));
            save::save_type = save::save_type::Steam;
            break;
        default:
            save::save_type = save::save_type::Invalid;
            break;
    }
    return;
}

void save::SaveData(){

    std::ofstream file(path, std::ios::out | std::ios::binary | std::ios::in);
    switch(save::save_type){
        case save::save_type::Android:
            file.write((char *)systemDataAndroid, sizeof(SystemData_Android));
            break;
        case save::save_type::Steam:
            file.write((char *)systemData, sizeof(SystemData));
            break;
        default:
            break;
    }

    file.close();
}