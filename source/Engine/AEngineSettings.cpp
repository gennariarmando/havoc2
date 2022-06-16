#include "AEngineSettings.h"
#include "AConsole.h"
#include "AScreen.h"
#include "version.h"

AEngineSettings EngineSettings;

std::string userFilesPath = "ufiles/";
std::string settingsFileName = "settings.xml";

AEngineSettings::AEngineSettings() {
    m_nScreenWidth = 640;
    m_nScreenHeight = 480;
    m_bFullscreen = false;
    m_bVSync = false;
}

void AEngineSettings::Load() {
    pugi::xml_document doc;
    pugi::xml_parse_result file = doc.load_file((userFilesPath + settingsFileName).c_str());

    if (file) {
        auto settings = doc.child("Settings");
        auto version = settings.child("version").attribute("value");

        std::string ver = SETTINGS_VERSION_STRING;
        if (ver.compare(version.as_string())) {
            Console.WriteLine("Settings file is not compatible, default settings restored");
        }
        else {
            if (auto display = settings.child("display")) {
                m_nScreenWidth = display.child("ScreenWidth").attribute("value").as_int(m_nScreenWidth);
                m_nScreenHeight = display.child("ScreenHeight").attribute("value").as_int(m_nScreenHeight);
                m_bFullscreen = display.child("Fullscreen").attribute("value").as_bool(m_bFullscreen);
                m_bVSync = display.child("VSync").attribute("value").as_int(m_bVSync);
            }
        }
    }
}

void AEngineSettings::Save() {
    pugi::xml_document doc;
    auto declarationNode = doc.append_child(pugi::node_declaration);
    declarationNode.append_attribute("version") = "1.0";
    declarationNode.append_attribute("encoding") = "UTF-8";

    auto settings = doc.append_child("Settings");
    settings.append_child("version").append_attribute("value").set_value(SETTINGS_VERSION_STRING);

    if (auto display = settings.append_child("display")) {
        display.append_child("ScreenWidth").append_attribute("value").set_value(m_nScreenWidth);
        display.append_child("ScreenHeight").append_attribute("value").set_value(m_nScreenHeight);
        display.append_child("Fullscreen").append_attribute("value").set_value(m_bFullscreen);
        display.append_child("VSync").append_attribute("value").set_value(m_bVSync);
    }

    std::error_code err = {};
    bool file = doc.save_file((userFilesPath + settingsFileName).c_str());
    if (std::filesystem::exists(userFilesPath.c_str(), err) || std::filesystem::create_directory(userFilesPath.c_str(), err)) {
        if (!file)
            Console.WriteLine(std::format("Error saving settings file: {}", err.message().c_str()));
    }
}
