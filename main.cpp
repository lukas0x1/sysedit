#include <iostream>
#include <fstream>
#include "gui.h"
#include <thread>
#include "include/imgui/imgui.h"


int __stdcall WINAPI wWinMain(
        HINSTANCE instance,
        HINSTANCE previousInstance,
        PWSTR arguments,
        int commandShow)
{
    // create gui
    gui::CreateHWindow("Save Editor");
    gui::CreateDevice();
    gui::CreateImGui();

    while (gui::isRunning)
    {
        gui::BeginRender();
        gui::Render();
        gui::EndRender();

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    // destroy gui
    gui::DestroyImGui();
    gui::DestroyDevice();
    gui::DestroyHWindow();
    return EXIT_SUCCESS;
}



/*


void *loadSystemData(const char* path, int &type){



    SystemData *systemdata = new SystemData;
    SystemData_Android *systemdata_android = new SystemData_Android;
    const char* filename = path;
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        return nullptr;
    }
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    int version = 0;
    std::cout << "cc" << version;
    file.read((char *)&version, 4);
    std::cout << "cc" << version;
    if(version == 1){
        file.read(reinterpret_cast<char*>(systemdata_android), sizeof(*systemdata));
        type = save_type::Android;
        file.close();
        return (void *)systemdata_android;
    }
    std::cout << version;
    if(version == 4096){
        file.read(reinterpret_cast<char*>(systemdata), sizeof(*systemdata));
        type = save_type::Steam;
        file.close();
        return (void *)systemdata;
    }

    std::cerr << "Invalid save" << std::endl;

    if (file.bad()) {
        std::cerr << "Error reading the file!" << std::endl;
        delete[] systemdata;
        delete[] systemdata_android;
        return nullptr;
    }
    //std::cout << (int)systemdata->sce_data.GS1_Scenario_enable / 16;
    file.close();
    return (void *)systemdata;
}

*/







int main() {




    return 0;
}
