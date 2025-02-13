/****************************************************************
 * @file    gs_api_commands.h
 * @brief   GameSynth Tool APIのコマンド一覧
 * @version 1.0.2
 * @auther  ysd
 ****************************************************************/
#ifndef GS_API_COMMANDS_H
#define GS_API_COMMANDS_H

/****************************************************************
 * プリプロセッサ定義
 ****************************************************************/
/* APIジャンル: GameSynth */
#define GS_API_GET_VERSION              "get_version"           /* ツールバージョンを取得する */
#define GS_API_GET_COMMANDS             "get_commands"          /* ツール上で利用可能なコマンド一覧を取得する */
#define GS_API_GET_MODELS               "get_models"            /* ツール上で利用可能なモデル一覧を取得する */
#define GS_API_SELECT_MODEL             "select_model"          /* ツール上でモデルを選択する。 */
#define GS_API_GET_PATH                 "get_path"              /* ツールのシステムパスを取得する */
#define GS_API_GET_SAMPLERATE           "get_samplerate"        /* ツール上のサンプリング周波数を取得する */
#define GS_API_SET_SAMPLERATE           "set_samplerate"        /* ツールにサンプリング周波数を設定する */

/* APIジャンル: リポジトリ */
#define GS_API_QUERY_PATCHNAMES         "query_patchnames"      /* リポジトリにあるパッチを検索する。 */
#define GS_API_QUERY_PATCH              "query_patch"           /* リポジトリからパッチを取得しツール上で読み込む。 */
#define GS_API_QUERY_CATEGORIES         "query_categories"      /* リポジトリで利用可能なカテゴリ一覧を取得する。 */
#define GS_API_QUERY_TAGS               "query_tags"            /* リポジトリで利用可能なタグ一覧を取得する。 */

/* APIジャンル: パッチ */
#define GS_API_LOAD_PATCH               "load_patch"            /* パスで指定したパッチをツールで読み込む。 */
#define GS_API_SAVE_PATCH               "save_patch"            /* ツールで開いているパッチを指定したパスに保存する。 */
#define GS_API_RENDER_PATCH             "render_patch"          /* ツールで開いているパッチを指定したパスに出力する。 */
#define GS_API_GET_MODELNAME            "get_modelname"         /* ツールで開いているパッチのモデル名を取得する。 */
#define GS_API_GET_PATCHNAME            "get_patchname"         /* ツールで開いているパッチのパッチ名を取得する。 */
#define GS_API_GET_VARIATION            "get_variation"         /* ツールで開いているパッチのランダムバリエーションを取得する。 */
#define GS_API_SET_VARIATION            "set_variation"         /* ツールで開いているパッチにランダムバリエーションを設定する。 */
#define GS_API_GET_DRAWING              "get_drawing"           /* ツールで開いているパッチの曲線の情報を取得する。*/
#define GS_API_SET_DRAWING              "set_drawing"           /* ツールで開いているパッチに曲線の情報を設定する。*/

/* APIジャンル: メタパラメータ */
#define GS_API_GET_METACOUNT            "get_metacount"
#define GS_API_GET_METANAMES            "get_metanames"
#define GS_API_GET_METANAME             "get_metaname"
#define GS_API_GET_METAVALUE            "get_metavalue"
#define GS_API_SET_METAVALUE            "set_metavalue"
#define GS_API_GET_CURVESCOUNT          "get_curvescount"
#define GS_API_GET_CURVENAMES           "get_curvenames"
#define GS_API_GET_CURVENAME            "get_curvename"
#define GS_API_GET_CURVEVALUE           "get_curvevalue"
#define GS_API_SET_CURVEVALUE           "set_curvevalue"

/* APIジャンル: 再生 */
#define GS_API_PLAY                     "play"
#define GS_API_STOP                     "stop"
#define GS_API_IS_PLAYING               "is_playing"
#define GS_API_IS_INFINITE              "is_infinite"
#define GS_API_IS_RANDOMIZED            "is_randomized"
#define GS_API_ENABLE_EVENTS            "enable_events"

/* APIジャンル: ユーザーインタフェース */
#define GS_API_WINDOW_BACK              "window_back"
#define GS_API_WINDOW_FRONT             "window_front"
#define GS_API_WINDOW_MESSAGE           "window_message"
#define GS_API_WINDOW_PARAMETERS        "window_parameters"
#define GS_API_WINDOW_RENDERING         "window_rendering "
#define GS_API_WINDOW_TEST              "window_test"

#endif /* GS_API_COMMANDS_H */
