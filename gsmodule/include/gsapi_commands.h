﻿/****************************************************************
 * @file    gsapi_commands.h
 * @brief   GameSynth Tool APIのコマンド一覧
 * @version 1.0.7
 * @auther  ysd
 ****************************************************************/
#ifndef GSAPI_COMMANDS_H
#define GSAPI_COMMANDS_H

/****************************************************************
 * プリプロセッサ定義
 ****************************************************************/
/* APIジャンル: GameSynth */
#define GSAPI_GET_VERSION              "get_version"           /* ツールバージョンを取得する */
#define GSAPI_GET_COMMANDS             "get_commands"          /* ツール上で利用可能なコマンド一覧を取得する */
#define GSAPI_GET_MODELS               "get_models"            /* ツール上で利用可能なモデル一覧を取得する */
#define GSAPI_SELECT_MODEL             "select_model"          /* ツール上でモデルを選択する。 */
#define GSAPI_GET_PATH                 "get_path"              /* ツールのシステムパスを取得する */
#define GSAPI_GET_SAMPLERATE           "get_samplerate"        /* ツール上のサンプリング周波数を取得する */
#define GSAPI_SET_SAMPLERATE           "set_samplerate"        /* ツールにサンプリング周波数を設定する */

/* APIジャンル: リポジトリ */
#define GSAPI_QUERY_PATCHNAMES         "query_patchnames"      /* リポジトリにあるパッチを検索する。 */
#define GSAPI_QUERY_PATCH              "query_patch"           /* リポジトリからパッチを取得しツール上で読み込む。 */
#define GSAPI_QUERY_CATEGORIES         "query_categories"      /* リポジトリで利用可能なカテゴリ一覧を取得する。 */
#define GSAPI_QUERY_TAGS               "query_tags"            /* リポジトリで利用可能なタグ一覧を取得する。 */

/* APIジャンル: パッチ */
#define GSAPI_LOAD_PATCH               "load_patch"            /* パスで指定したパッチをツールで読み込む。 */
#define GSAPI_SAVE_PATCH               "save_patch"            /* 現在のパッチを指定したパスに保存する。 */
#define GSAPI_RENDER_PATCH             "render_patch"          /* 現在のパッチを指定したパスに出力する。 */
#define GSAPI_GET_MODELNAME            "get_modelname"         /* 現在のパッチのモデル名を取得する。 */
#define GSAPI_GET_PATCHNAME            "get_patchname"         /* 現在のパッチのパッチ名を取得する。 */
#define GSAPI_GET_VARIATION            "get_variation"         /* 現在のパッチのランダムバリエーションを取得する。 */
#define GSAPI_SET_VARIATION            "set_variation"         /* 現在のパッチにランダムバリエーションを設定する。 */
#define GSAPI_GET_DRAWING              "get_drawing"           /* 現在のパッチの曲線の情報を取得する。*/
#define GSAPI_SET_DRAWING              "set_drawing"           /* 現在のパッチに曲線の情報を設定する。*/

/* APIジャンル: メタパラメータ */
#define GSAPI_GET_METACOUNT            "get_metacount"         /* 現在のパッチのメタパラメータ数を取得する。 */
#define GSAPI_GET_METANAMES            "get_metanames"         /* 現在のパッチのメタパラメータ一覧を取得する。 */
#define GSAPI_GET_METANAME             "get_metaname"          /* 現在のパッチのメタパラメータの名前を取得する。 */
#define GSAPI_GET_METAVALUE            "get_metavalue"         /* 現在のパッチのメタパラメータの値を取得する。 */
#define GSAPI_SET_METAVALUE            "set_metavalue"         /* 現在のパッチのメタパラメータに値を設定する。 */

/* APIジャンル: オートメーションカーブ */
#define GSAPI_GET_CURVESCOUNT          "get_curvescount"       /* 現在のパッチのオートメーションカーブ数を取得する。 */
#define GSAPI_GET_CURVENAMES           "get_curvenames"        /* 現在のパッチのオートメーションカーブ一覧を取得する。*/
#define GSAPI_GET_CURVENAME            "get_curvename"         /* 現在のパッチのオートメーションカーブの名前を取得する。 */
#define GSAPI_GET_CURVEVALUE           "get_curvevalue"        /* 現在のパッチのオートメーションカーブの値を取得する。 */
#define GSAPI_SET_CURVEVALUE           "set_curvevalue"        /* 現在のパッチのオートメーションカーブに値を設定する。 */

/* APIジャンル: 再生 */
#define GSAPI_PLAY                     "play"                  /* 現在のパッチを再生する。 */
#define GSAPI_STOP                     "stop"                  /* 現在のパッチを停止する。 */
#define GSAPI_IS_PLAYING               "is_playing"            /* 現在のパッチは再生中か。 */
#define GSAPI_IS_INFINITE              "is_infinite"           /* 現在のパッチは再生時間が無限か。 */
#define GSAPI_IS_RANDOMIZED            "is_randomized"         /* 現在のパッチはランダム性があるか。 */
#define GSAPI_ENABLE_EVENTS            "enable_events"         /* ツールの合成イベントの通知を設定する。 */

/* APIジャンル: ユーザーインタフェース */
#define GSAPI_WINDOW_BACK              "window_back"           /* ツールのメインウィドウを背面に移動させる。 */
#define GSAPI_WINDOW_FRONT             "window_front"          /* ツールのメインウィドウを前面に移動させる。 */
#define GSAPI_WINDOW_MESSAGE           "window_message"        /* ツールのメッセージダイアログを表示する。 */
#define GSAPI_WINDOW_PARAMETERS        "window_parameters"     /* ツールのパラメーター設定ダイアログを表示する。 */
#define GSAPI_WINDOW_RENDERING         "window_rendering "     /* ツールのレンダリング設定ダイアログを表示する。 */
#define GSAPI_WINDOW_TEST              "window_test"           /* ツールのテスト用ダイアログを表示する。*/

#endif /* GSAPI_COMMANDS_H */
