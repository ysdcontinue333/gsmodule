/****************************************************************
 * @file    gs_api_interface.h
 * @brief   GameSynth Tool APIを呼び出す
 * @version 1.0.1
 * @auther  ysd
 ****************************************************************/
#ifndef GS_API_INTERFACE_H
#define GS_API_INTERFACE_H

/****************************************************************
 * インクルード
 ****************************************************************/
#include <string>
#include <vector>

/****************************************************************
 * プリプロセッサ定義
 ****************************************************************/
#define GS_API_INTERFACE_DEFAULT_PORT_NUMBER    (28542)         /* デフォルトのポート番号 */
#define GS_API_INTERFACE_DEFAULT_IP_ADDRESS     "127.0.0.1"     /* デフォルトのIPアドレス */
#define GS_API_INTERFACE_DEFAULT_CODEC          "UTF-8"         /* デフォルトの圧縮形式 */
#define GS_API_INTERFACE_DEFAULT_DELIMITER      "\r"            /* デフォルトのデリミタ(10進コードで13) */

/* ツールとの通信に使用されるパラメータの構造体 */
typedef struct GsApiInterfaceConfigStruct {
    unsigned int    port_number     = GS_API_INTERFACE_DEFAULT_PORT_NUMBER;     /* 接続先:ポート番号 */
    std::string     ip_address      = GS_API_INTERFACE_DEFAULT_IP_ADDRESS;      /* 接続先:IPアドレス*/
    std::string     codec           = GS_API_INTERFACE_DEFAULT_CODEC;           /* メッセージ:圧縮形式 */
    std::string     delimiter       = GS_API_INTERFACE_DEFAULT_DELIMITER;       /* メッセージ:デリミタ */
} GsApiInterfaceConfig;

/****************************************************************
 * クラス宣言
 ****************************************************************/
class gs_api_interface
{
public:
    /**************************************************************************
     * @brief   ツールとの通信方法を更新する。
     * @param   config : 新しい通信設定の参照
     * @return  更新完了でtrueを返す。それ以外の場合にfalseを返す。
     **************************************************************************/
    static bool set_default_config(const GsApiInterfaceConfig& config);
    /**************************************************************************
     * @brief   ツールとの通信方法を取得する。
     * @param   config : 現在の通信設定を格納する参照
     * @return  取得完了でtrueを返す。それ以外の場合にfalseを返す。
     **************************************************************************/
    static bool get_default_config(GsApiInterfaceConfig& config);

    /**************************************************************************
     * @brief   起動中のツールに対してメッセージを送る。
     * @param   message : 送信するメッセージの参照
     * @param   response : 受信するメッセージを格納する参照
     * @return  ツールにメッセージを送信できればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool send_command(const std::string& message, std::string& response);

    /**************************************************************************
     * @brief   ツールとTCP通信できるか。
     * @return  通信可能であればtrueを返す。それ以外の場合にfalseを返す。
     **************************************************************************/
    static bool is_connect();

public:
    /* GameSynth Tool APIを利用する関数 */

    /**************************************************************************
     * @brief   バージョンを調べる。
     * @param   version : ツールバージョンを格納する参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_get_version(std::string& version);
    /**************************************************************************
     * @brief   利用可能なコマンドを調べる。
     * @param   commmand_list : コマンド一覧を格納する配列の参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_get_commands(std::vector<std::string>& commmand_list);
    /**************************************************************************
     * @brief   利用可能なモデルを調べる。
     * @param   model_list : モデル一覧を格納する配列の参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_get_models(std::vector<std::string>& model_list);
    /**************************************************************************
     * @brief   名前で指定したモデルをツール上で選択する。
     * @param   model_name : モデル名を格納する参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_select_model(const std::string& model_name);
    /**************************************************************************
     * @brief   ツールのシステムパスを取得する。
     * @param   path_name : システムパスの名称
     * @param   path_value : パスを格納する参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_get_path(const std::string& path_name, std::string& path_value);
    /**************************************************************************
     * @brief   ツールのサンプリング周波数を取得する。
     * @param   samplerate : ツールに設定された値を格納する参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_get_samplerate(std::string& samplerate);
    /**************************************************************************
     * @brief   ツールにサンプリング周波数を設定する。
     * @param   samplerate : ツールに設定する数値への参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_set_samplerate(const std::string& samplerate);

    /**************************************************************************
     * @brief   リポジトリから一致するパッチ名を返す。
     * @param   text : 検索したい文字列
     * @param   name : パッチ名の検索を有効にする
     * @param   category : カテゴリ名の検索を有効にする
     * @param   tags : タグ名の検索を有効にする
     * @param   patch_list : パッチ名一覧を格納する配列の参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_query_patchnames(const std::string& text, const bool name,
        const bool category, const bool tags, std::vector<std::string>& patch_list);
    /**************************************************************************
     * @brief   リポジトリからパッチを取得し、ツール上で読み込む。
     * @param   patch_name : リポジトリから取得するパッチ名の参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_query_patch(const std::string& patch_name);
    /**************************************************************************
     * @brief   リポジトリからカテゴリ一覧を取得する。
     * @param   categoryt_list : カテゴリ一覧を格納する配列の参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_query_categories(std::vector<std::string>& categoryt_list);
    /**************************************************************************
     * @brief   リポジトリからタグ一覧を取得する。
     * @param   tag_list : タグ一覧を格納する配列の参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_query_tags(std::vector<std::string>& tag_list);

private:
    static GsApiInterfaceConfig gs_config;
};

#endif /* GS_API_INTERFACE_H */
