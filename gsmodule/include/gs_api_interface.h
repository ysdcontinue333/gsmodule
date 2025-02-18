/****************************************************************
 * @file    gs_api_interface.h
 * @brief   GameSynth Tool APIを呼び出す
 * @version 1.0.5
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

/*スケッチパッドに描かれている曲線を格納する構造体 */
typedef struct GsDrawingDataStruct {
    float t = 0.f;                                                              /* 秒単位の時間 [0….60] */
    float x = 0.f;                                                              /* 水平位置 [0…1] */
    float y = 0.f;                                                              /* 垂直位置 [0…1] */
    float p = 0.f;                                                              /* 筆圧量[0…1] */
} GsDrawingData;

/* オートメーションカーブの座標情報を格納する */
typedef struct GsCurvePointStruct {
    float x = 0.f;                                                              /* X値 [0-1] */
    float y = 0.f;                                                              /* Y値 [0-1] */
} GsCurvePoint;

/* オートメーションカーブの曲線を格納する構造体 */
typedef struct GsCurveValueStruct {
    std::vector<GsCurvePoint>   curve;                                          /* 連続した座標 */
    float                       duration = 0;                                   /* 持続時間 [0-60] */
    bool                        is_loop = false;                                /* ループ情報 [0,1] */
} GsCurveValue;

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

    /**************************************************************************
     * @brief   ファイルパスにあるパッチファイルをツール上で開く。
     * @param   file_path : パッチファイルのパスの参照
     * @return  GameSynthから応答があればtrueを返す。それ以外はfalseを返す
     **************************************************************************/
    static bool command_load_patch(const std::string& file_path);
    /**************************************************************************
     * @brief   パッチをファイルパスに保存する。
     * @param   file_path : パッチファイルのパスの参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_save_patch(const std::string& file_path);
    /**************************************************************************
     * @brief   パッチをファイルパスにレンダリングする。
     * @param   file_path : 音源ファイルのパスの参照
     * @param   depth : ビット深度
     * @param   channel : チャンネル数
     * @param   duration : デュレーション
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_render_patch(const std::string& file_path, const unsigned int depth,
        const unsigned int channel, const unsigned int duration);
    /**************************************************************************
     * @brief   パッチのモデル名を取得する。
     * @param   model_name : モデル名を格納する参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_get_modelname(std::string& model_name);
    /**************************************************************************
     * @brief   パッチのパッチ名を取得する。
     * @param   patch_name : パッチ名を格納する参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_get_patchname(std::string& patch_name);
    /**************************************************************************
     * @brief   パッチのバリエーションを取得する。
     * @param   variation : パッチのに設定された値を格納する参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_get_variation(float& variation);
    /**************************************************************************
     * @brief   パッチにバリエーションを設定する。
     * @param   variation : パッチに設定したい値への参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_set_variation(const float& variation);
    /**************************************************************************
     * @brief   パッチに設定された曲線を取得する。
     * @param   index : パッチに設定された曲線の番号
     * @param   drawing_data : パッチに設定された曲線の情報を格納する参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_get_drawing(const unsigned int index, std::vector<GsDrawingData>& drawing_data);
    /**************************************************************************
     * @brief   パッチに曲線を設定する。
     * @param   drawing_data : パッチに設定したい曲線の情報への参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_set_drawing(const std::vector<GsDrawingData>& drawing_data);

    /**************************************************************************
     * @brief   パッチのメタパラメータ数を取得する。
     * @param   meta_count : メタパラメータ数を格納する参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_get_metacount(unsigned int& meta_count);
    /**************************************************************************
     * @brief   パッチのメタパラメータの一覧を取得する。
     * @param   meta_names : メタパラメータの一覧を格納する参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_get_metanames(std::vector<std::string>& meta_names);
    /**************************************************************************
     * @brief   パッチのメタパラメータの名前を取得する。
     * @param   index : メタパラメータのインデックスの数値
     * @param   metaname : メタパラメータの名前を格納する参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_get_metaname(const unsigned int& index, std::string& metaname);
    /**************************************************************************
     * @brief   パッチのメタパラメータの値を取得する。
     * @param   index : メタパラメータのインデックスの数値
     * @param   metavalue : ツールから返されたメタパラメータの数値を格納する参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_get_metavalue(const unsigned int& index, float& metavalue);
    /**************************************************************************
     * @brief   パッチのメタパラメータの値を取得する。
     * @param   name : メタパラメータの名前
     * @param   metavalue : ツールから返されたメタパラメータの数値を格納する参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_get_metavalue(const std::string& name, float& metavalue);
    /**************************************************************************
     * @brief   パッチのメタパラメータに値を設定する。
     * @param   index : メタパラメータのインデックスの数値
     * @param   metavalue : ツールに設定したいメタパラメータの数値への参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_set_metavalue(const unsigned int& index, const float& metavalue);
    /**************************************************************************
     * @brief   パッチのメタパラメータに値を設定する。
     * @param   name : メタパラメータの名前
     * @param   metavalue : ツールに設定したいメタパラメータの数値への参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_set_metavalue(const std::string& name, const float& metavalue);

    /**************************************************************************
     * @brief   パッチのオートメーションカーブ数を取得する。
     * @param   curves_count : オートメーションカーブ数を格納する参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_get_curvescount(unsigned int& curves_count);
    /**************************************************************************
     * @brief   パッチのオートメーションカーブの一覧を取得する。
     * @param   curve_names : オートメーションカーブの一覧を格納する参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_get_curvenames(std::vector<std::string>& curve_names);
    /**************************************************************************
     * @brief   パッチのオートメーションカーブの名前を取得する。
     * @param   curve_index : オートメーションカーブのインデックスの数値
     * @param   curve_name : オートメーションカーブの名前を格納する参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_get_curvename(const unsigned int& curve_index, std::string& curve_name);
    /**************************************************************************
     * @brief   パッチのオートメーションカーブの値を取得する。
     * @param   curve_index : オートメーションカーブのインデックスの数値
     * @param   curve_value : オートーメーションカーブの値を格納する参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_get_curvevalue(const unsigned int& curve_index, GsCurveValue& curve_value);
    /**************************************************************************
     * @brief   パッチのオートメーションカーブの値を取得する。
     * @param   curve_name : オートメーションカーブの名前への参照
     * @param   curve_value : オートーメーションカーブの値を格納する参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_get_curvevalue(const std::string& curve_name, GsCurveValue& curve_value);
    /**************************************************************************
     * @brief   パッチのオートメーションカーブに値を設定する。
     * @param   curve_index : オートメーションカーブのインデックスの数値
     * @param   curve_value : オートーメーションカーブの値への参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_set_curvevalue(const unsigned int& curve_index, const GsCurveValue& curve_value);
    /**************************************************************************
     * @brief   パッチのオートメーションカーブに値を設定する。
     * @param   curve_name : オートメーションカーブの名前への参照
     * @param   curve_value : オートーメーションカーブの値への参照
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_set_curvevalue(const std::string& curve_name, const GsCurveValue& curve_value);

private:
    static GsApiInterfaceConfig gs_config;
};

#endif /* GS_API_INTERFACE_H */
