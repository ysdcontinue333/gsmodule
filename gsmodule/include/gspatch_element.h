/****************************************************************
 * @file    gspatch_element.h
 * @brief   gspatchに含まれるXML要素
 * @version 1.0.0
 * @auther  ysd
 ****************************************************************/
#ifndef GSPATCH_ELEMENT_H
#define GSPATCH_ELEMENT_H

/****************************************************************
 * プリプロセッサ定義
 ****************************************************************/
/* 拡張子 */
#define GSPATCH_PREFIX                          "gspatch"

/* XML要素 */
#define GSPATCH_ELEMENT_GAME_SYNTHP_ATCH        "GameSynthPatch"
#define GSPATCH_ELEMENT_PATCH                   "Patch"
#define GSPATCH_ELEMENT_AUTHOR                  "Author"
#define GSPATCH_ELEMENT_UCS                     "UCS"
#define GSPATCH_ELEMENT_ALGO_PARAMETERS         "AlgoParameters"
#define GSPATCH_ELEMENT_PARAMETERS              "Parameters"
#define GSPATCH_ELEMENT_META_PARAMETERS         "MetaParameters"
#define GSPATCH_ELEMENT_EVENTS                  "Events"
#define GSPATCH_ELEMENT_AUTOMATION_CURVES       "AutomationCurves"
#define GSPATCH_ELEMENT_INPUT_CONTROLS          "InputControls"
#define GSPATCH_ELEMENT_RANDOM_PLAY             "RandomPlay"

/* XMLアトリビュート */
#define GSPATCH_ATTRIBUTE_TOOL_VERSION          "ToolVersion"
#define GSPATCH_ATTRIBUTE_PATCH_NAME            "PatchName"
#define GSPATCH_ATTRIBUTE_PATCH_VERSION         "PatchVersion"
#define GSPATCH_ATTRIBUTE_UCS_CATEGORY          "category"
#define GSPATCH_ATTRIBUTE_UCS_SUB_CATEGORY      "subCategory"
#define GSPATCH_ATTRIBUTE_VALUE                 "value"

#endif /* GSPATCH_ELEMENT_H */
