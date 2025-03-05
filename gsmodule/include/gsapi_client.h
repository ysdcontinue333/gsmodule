/****************************************************************
 * @file    gsapi_client.h
 * @brief   GameSynth Tool APIを呼び出す
 * @version 1.0.8
 * @auther  ysd
 ****************************************************************/
#ifndef GSAPI_CLIENT_H
#define GSAPI_CLIENT_H

/****************************************************************
 * インクルード
 ****************************************************************/
#include <string>
#include <vector>
#include <variant>

/****************************************************************
 * プリプロセッサ定義
 ****************************************************************/
#define GSAPI_CLIENT_DEFAULT_PORT_NUMBER    (28542)                             /* デフォルトのポート番号 */
#define GSAPI_CLIENT_DEFAULT_IP_ADDRESS     "127.0.0.1"                         /* デフォルトのIPアドレス */
#define GSAPI_CLIENT_DEFAULT_CODEC          "UTF-8"                             /* デフォルトの圧縮形式 */
#define GSAPI_CLIENT_DEFAULT_DELIMITER      "\r"                                /* デフォルトのデリミタ(10進コードで13) */

/****************************************************************
 * 構造体宣言
 ****************************************************************/

/* ツールとの通信に使用されるパラメータの構造体 */
typedef struct GsApiClientConfigStruct {
    unsigned int    port_number     = GSAPI_CLIENT_DEFAULT_PORT_NUMBER;         /* 接続先:ポート番号 */
    std::string     ip_address      = GSAPI_CLIENT_DEFAULT_IP_ADDRESS;          /* 接続先:IPアドレス*/
    std::string     codec           = GSAPI_CLIENT_DEFAULT_CODEC;               /* メッセージ:圧縮形式 */
    std::string     delimiter       = GSAPI_CLIENT_DEFAULT_DELIMITER;           /* メッセージ:デリミタ */
} GsApiClientConfig;

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

/* メッセージボックスのボタン */
typedef enum GsWindowButtonEnum {
    GS_WINDOW_BUTTON_OK = 0,                                                    /* OK */
    GS_WINDOW_BUTTON_OK_CANCEL,                                                 /* OK_CANCEL */
    GS_WINDOW_BUTTON_YES_NO,                                                    /* YES_NO */
    GS_WINDOW_BUTTON_RETRY_EXIT,                                                /* RETRY_EXIT */
} GsWindowButton;

/* ダイアログで選択できるデータ型 */
typedef enum GsDataTypeEnum {
    GS_DATA_TYPE_NUMBER = 0,                                                    /* NUMBER */
    GS_DATA_TYPE_BOOL,                                                          /* BOOL */
    GS_DATA_TYPE_STRING,                                                        /* STRING */
    GS_DATA_TYPE_ENUM,                                                          /* ENUM */
    GS_DATA_TYPE_LABEL,                                                         /* LABEL */
} GsDataType;

/* 数値パラメーターのサブタイプ */
typedef enum GsNumberSubTypeEnum {
    GS_NUMBER_SUB_TYPE_INTEGER = 0,                                             /* INTEGER */
    GS_NUMBER_SUB_TYPE_FLOAT                                                    /* FLOAT */
} GsNumberSubType;

/* 文字列パラメーターのサブタイプ */
typedef enum GsStringSubTypeEnum {
    GS_STRING_SUB_TYPE_NORMAL = 0,                                              /* NORMAL */
    GS_STRING_SUB_TYPE_FILELOAD,                                                /* FILELOAD */
    GS_STRING_SUB_TYPE_FILESAVE,                                                /* FILESAVE */
    GS_STRING_SUB_TYPE_FOLDER                                                   /* FOLDER */
} GsStringSubType;

/* 列挙パラメーターのサブタイプ */
typedef enum GsEnumSubTypeEnum {
    GS_ENUM_SUB_TYPE_LIST = 0,                                                  /* LIST */
    GS_ENUM_SUB_TYPE_COMBO                                                      /* COMBO */
} GsEnumSubType;

/* ラベルパラメーターのサブタイプ */
typedef enum GsLabelSubTypeEnum {
    GS_LABEL_SUB_TYPE_TEXT = 0,                                                 /* TEXT */
    GS_LABEL_SUB_TYPE_HEADER                                                    /* HEADER */
} GsLabelSubType;

/* ラベルパラメーターの水平方向の配置 */
typedef enum GsLabelAlignmentEnum {
    GS_LABEL_SUB_ALIGNMENT_LEFT = 0,                                            /* LEFT */
    GS_LABEL_SUB_ALIGNMENT_RIGHT,                                               /* RIGHT */
    GS_LABEL_SUB_ALIGNMENT_CENTER                                               /* CENTER */
} GsLabelAlignment;

/* 数値パラメーターのフィールド */
typedef struct GsNumberStruct {
    const GsDataType type = GS_DATA_TYPE_NUMBER;                                /* タイプ */
    GsNumberSubType sub_type = GS_NUMBER_SUB_TYPE_INTEGER;                      /* サブタイプ */
    std::string name = "";                                                      /* 名前 */
    std::string unit = "";                                                      /* 単位 */
    float min_value = 0.f;                                                      /* 最小値 */
    float max_value = 1.f;                                                      /* 最大値 */
    float default_value = 0;                                                    /* デフォルト値 */
    unsigned int decimals = 1;                                                  /* 小数点以下の桁数 */
} GsNumber;

/* 真偽値パラメーターのフィールド */
typedef struct GsBoolStruct {
    const GsDataType type = GS_DATA_TYPE_BOOL;                                  /* タイプ */
    std::string name = "";                                                      /* 名前 */
    bool default_value = true;                                                  /* デフォルト値 */
} GsBool;

/* 文字列パラメーターのフィールド */
typedef struct GsStringStruct {
    const GsDataType type = GS_DATA_TYPE_STRING;                                /* タイプ */
    GsStringSubType sub_type = GS_STRING_SUB_TYPE_NORMAL;                       /* サブタイプ */
    std::string name = "";                                                      /* 名前 */
    std::string default_value = "";                                             /* デフォルト値 */
} GsString;

/* 列挙パラメーターのフィールド */
typedef struct GsEnumStruct {
    const GsDataType type = GS_DATA_TYPE_ENUM;                                  /* タイプ */
    GsEnumSubType sub_type = GS_ENUM_SUB_TYPE_LIST;                             /* サブタイプ */
    std::string name = "";                                                      /* 名前 */
    std::vector<std::string> choices;                                           /* 選択肢のリスト */
    unsigned int default_choice = 0;                                            /* デフォルト値 */
} GsEnum;

/* ラベルパラメーターのフィールド */
typedef struct GsLabelStruct {
    const GsDataType type = GS_DATA_TYPE_LABEL;                                 /* タイプ */
    GsLabelSubType sub_type = GS_LABEL_SUB_TYPE_TEXT;                           /* サブタイプ */
    GsLabelAlignment alignment = GS_LABEL_SUB_ALIGNMENT_LEFT;                   /* 水平方向の配置 */
    std::string text = "";                                                      /* テキスト */
} GsLabel;

/* パラメーターの汎用的なデータ型 */
typedef std::variant<GsNumber, GsBool, GsString, GsEnum, GsLabel> GsParameter;

/****************************************************************
 * クラス宣言
 ****************************************************************/
class gsapi_client
{
public:
    /**************************************************************************
     * @brief   ツールとの通信方法を更新する。
     * @param   config : 新しい通信設定の参照
     * @return  更新完了でtrueを返す。それ以外の場合にfalseを返す。
     **************************************************************************/
    static bool set_default_config(const GsApiClientConfig& config);
    /**************************************************************************
     * @brief   ツールとの通信方法を取得する。
     * @param   config : 現在の通信設定を格納する参照
     * @return  取得完了でtrueを返す。それ以外の場合にfalseを返す。
     **************************************************************************/
    static bool get_default_config(GsApiClientConfig& config);

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

    /**************************************************************************
     * @brief   パッチを再生する。
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_play();
    /**************************************************************************
     * @brief   パッチを停止する。
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_stop();
    /**************************************************************************
     * @brief   パッチは再生中か調べる。
     * @param   is_playing :　再生中なら[1]。それ以外なら[0]。
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_is_playing(bool& is_playing);
    /**************************************************************************
     * @brief   パッチは再生時間が無限か調べる。
     * @param   is_infinite :　時間が無限なら[1]。自動的に終了するなら[0]。
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_is_infinite(bool& is_infinite);
    /**************************************************************************
     * @brief   パッチはランダム性があるか調べる。
     * @param   is_randomized :　ランダム性があれば[1]。見つからなければ[0]。
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_is_randomized(bool& is_randomized);
    /**************************************************************************
     * @brief   ツールの合成イベントの通知を設定する。
     * @param   is_notification :　通知を有効なら[1]。通知を無効なら[0]。
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_enable_events(const bool is_notification);

    /**************************************************************************
     * @brief   
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_window_back();
    /**************************************************************************
     * @brief
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_window_front();
    /**************************************************************************
     * @brief
     * @param   XXX :　
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_window_message(const std::string& message, const GsWindowButton& button);
    /**************************************************************************
     * @brief
     * @param   XXX :　
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_window_parameters(const std::vector<GsParameter>& params);
    /**************************************************************************
     * @brief
     * @param   XXX :　
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_window_rendering(const bool& show_duration, const bool& show_variations);
    /**************************************************************************
     * @brief
     * @return  ツールから応答があればtrueを返す。それ以外はfalseを返す。
     **************************************************************************/
    static bool command_window_test();


private:
    static GsApiClientConfig gs_config;
};

#endif /* GSAPI_CLIENT_H */
