/****************************************************************
 * @file    gspatch_parser.h
 * @brief   gspatchを解釈する
 * @version 1.0.0
 * @auther  ysd
 ****************************************************************/

/****************************************************************
 * インクルード
 ****************************************************************/
#include "../include/gspatch_parser.h"
#include "../include/gspatch_element.h"
#include <tinyxml2.h>

/****************************************************************
 * クラス定義
 ****************************************************************/
bool gspatch_parser::parse(const std::string &text_data, GameSynthPatchData &gspatch_data)
{
    /* gspatchファイルをXMLとして解釈する */
    tinyxml2::XMLDocument doc;
    auto result = doc.Parse(text_data.c_str());
    if (result != tinyxml2::XML_SUCCESS) {
        return false;
    }
    tinyxml2::XMLElement* element_game_synth_patch = doc.FirstChildElement(GSPATCH_ELEMENT_GAME_SYNTHP_ATCH);
    {
        auto tool_version = element_game_synth_patch->FindAttribute(GSPATCH_ATTRIBUTE_TOOL_VERSION);
        gspatch_data.tool_version = tool_version->Value();
    }
    tinyxml2::XMLElement* element_patch = element_game_synth_patch->FirstChildElement(GSPATCH_ELEMENT_PATCH);
    {
        auto patch_name = element_patch->FindAttribute(GSPATCH_ATTRIBUTE_PATCH_NAME);
        gspatch_data.patch_name = patch_name->Value();
        auto patch_version = element_patch->FindAttribute(GSPATCH_ATTRIBUTE_PATCH_VERSION);
        gspatch_data.patch_version = patch_version->Value();
    }
    tinyxml2::XMLElement* element_author = element_patch->FirstChildElement(GSPATCH_ELEMENT_AUTHOR);
    {
        auto author = element_author->FindAttribute(GSPATCH_ATTRIBUTE_VALUE);
        gspatch_data.author = author->Value();
    }
    return true;
}
