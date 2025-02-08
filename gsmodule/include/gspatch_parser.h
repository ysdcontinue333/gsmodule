/****************************************************************
 * @file    gspatch_parser.h
 * @brief   gspatchを解釈する
 * @version 1.0.0
 * @auther  ysd
 ****************************************************************/
#ifndef GSPATCH_PARSER_H
#define GSPATCH_PARSER_H

/****************************************************************
 * インクルード
 ****************************************************************/
#include <string>

/****************************************************************
 * 構造体宣言
 ****************************************************************/
typedef struct GameSynthPatchDataStruct {
    std::string filepath            = "";   /* ファイルパス */
    std::string tool_version        = "";   /* パッチを作成したツールバージョン */
    std::string patch_name          = "";   /* パッチの種類 */
    std::string patch_version       = "";   /* パッチのバージョン */
    std::string author              = "";   /* パッチの製作者 */
    std::string ucs_category        = "";   /* UCS規格のカテゴリ */
    std::string ucs_sub_catebory    = "";   /* UCS規格のサブカテゴリ */
} GameSynthPatchData;

/****************************************************************
 * クラス宣言
 ****************************************************************/
class gspatch_parser
{
public:
    /***************************************************************************
     * @brief   gspatchファイルをパースして構造体で受け取る。
     * @param   text_data : gspatchファイルのテキストデータへのポインタ
     * @param   gspatch_data : gspatchファイルのデータを格納するポインタ
     * @return  パースに成功するとtrueを返す。それ以外のときにfalseを返す。
     ***************************************************************************/
    static bool parse(const std::string& text_data, GameSynthPatchData& gspatch_data);
};

#endif /* GSPATCH_PARSER_H */