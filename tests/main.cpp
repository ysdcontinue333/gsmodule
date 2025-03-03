/****************************************************************
 * @file    main.cpp
 * @brief   gsmoduleのテスト
 * @version 1.0.8
 * @auther  ysd
 ****************************************************************/

/****************************************************************
 * インクルード
 ****************************************************************/
#include <gsapi_commands.h>
#include <gsapi_client.h>
#include <gtest/gtest.h>
#include <iostream>

/****************************************************************
 * プリプロセッサ定義
 ****************************************************************/
/* システムパス取得のテストで使用する文字列 */
#define GS_PATH_PATCH_FOLDER            "PATCH_FOLDER"          /* パッチパス */
#define GS_PATH_BANK_FOLDER             "BANK_FOLDER"           /* バンクパス */
#define GS_PATH_BINARY_FOLDER           "BINARY_FOLDER"         /* バイナリバンクのエクスポートパス */
#define GS_PATH_MIDDLEWARE_FOLDER       "MIDDLEWARE_FOLDER"     /* ミドルウェアへのエクスポートパス */
#define GS_PATH_RENDERING_FOLDER        "RENDERING_FOLDER"      /* オーディオレンダリングのパス */
#define GS_PATH_AUDIOASSETS_FOLDER      "AUDIOASSETS_FOLDER"    /* オーディオアセットのパス */
#define GS_PATH_PICTURES_FOLDER         "PICTURES_FOLDER"       /* (ImpactやFootstepsモデルなどで使用される) 画像ファイルのパス */
#define GS_PATH_VIDEO_FOLDER            "VIDEO_FOLDER"          /* 動画のパス */
#define GS_PATH_SCRIPTS_FOLDER          "SCRIPTS_FOLDER"        /* (Scriptモジュールやスケッチパッドなどで使われる) スクリプトのパス */
#define GS_PATH_REPOSITORY_FOLDER       "REPOSITORY_FOLDER"     /* リポジトリデータが保存されているパス */
#define GS_PATH_RANDOMPATCH_FOLDER      "RANDOMPATCH_FOLDER"    /* ランダムパッチ生成時にオーディオアセットを取得する際のパス */
#define GS_PATH_AUDIOEDITOR_EXE         "AUDIOEDITOR_EXE"       /* オーディオエディタへのパス */

/* サンプリング周波数のテストで使用する数値 */
#define GS_SAPMLERATE_44100             "44100"
#define GS_SAMPLERATE_48000             "48000"

/* パッチ操作のテストで使用するパス */
#define TEST_LOAD_FILE_NAME             "TestPatch.gspatch"
#define TEST_SAVE_FILE_NAME             "TestPatch.gspatch"
#define TEST_RENDER_FILE_NAME           "TestPatch.wav"

 /****************************************************************
  * 変数定義
  ****************************************************************/
std::string source_path;

/****************************************************************
 * クラス定義
 ****************************************************************/
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/**************************************************************************
 * @brief   GoogleTestによる機能テスト
 **************************************************************************/
class GSAPI_TEST : public testing::Test {
protected:
    GSAPI_TEST() {
    }
    virtual ~GSAPI_TEST() {
    }

    /* 最初のテスト前に実行 */
    static void SetUpTestCase() {
        bool res;
        GsApiClientConfig gs_config;
        res = gsapi_client::get_default_config(gs_config);
        EXPECT_EQ(res, true);
#if 0
        /* GameSynth側の接続設定に合わせて変更 */
        gs_config.port_number = 28542;
        gs_config.ip_address = "127.0.0.1";
        gs_config.codec = "UTF-8";
        gs_config.delimiter = "\r";
#endif
        res = gsapi_client::set_default_config(gs_config);
        EXPECT_EQ(res, true);
    }
    /* 最後のテスト後に実行 */
    static void TearDownTestCase() {
    }
    /* 各テストの実行前 */
    virtual void SetUp() {
    }
    /* 各テストの実行後 */
    virtual void TearDown() {
    }
};

/* 何らかのメッセージを送信するテスト */
TEST_F(GSAPI_TEST, TEST_SEND_COMMAND) {
    const std::string command = "get_version";
    std::string response = "";
    const bool res = gsapi_client::send_command(command, response);
    std::cout << response << std::endl;
    EXPECT_EQ(res, true);
};

/* ツールと通信可能か */
TEST_F(GSAPI_TEST, TEST_GS_CONNECT) {
    const bool res = gsapi_client::is_connect();
    EXPECT_EQ(res, true);
};

/* ツールのバージョン */
TEST_F(GSAPI_TEST, TEST_GSAPI_GET_VERSION) {
    std::string version;
    const bool res = gsapi_client::command_get_version(version);
    std::cout << version << std::endl;
    EXPECT_EQ(res, true);
};

/* ツールで使用可能なコマンドのリスト */
TEST_F(GSAPI_TEST, TEST_GSAPI_GET_COMMANDS) {
    std::vector<std::string> commmand_list;
    const bool res = gsapi_client::command_get_commands(commmand_list);
    for (auto command : commmand_list) {
        std::cout << command << std::endl;
    }
    EXPECT_EQ(res, true);
};

/* ツールで使用可能なモデルのリスト */
TEST_F(GSAPI_TEST, TEST_GSAPI_GET_MODELS) {
    std::vector<std::string> model_list;
    const bool res = gsapi_client::command_get_models(model_list);
    for (auto model : model_list) {
        std::cout << model << std::endl;
    }
    EXPECT_EQ(res, true);
};

/* ツールのシステムパス */
TEST_F(GSAPI_TEST, TEST_GSAPI_GET_PATH_) {
    const std::string path_name = GS_PATH_PATCH_FOLDER;
    std::string path_value;
    const bool res = gsapi_client::command_get_path(path_name, path_value);
    std::cout << path_value << std::endl;
    EXPECT_EQ(res, true);
};

/* ツールのサンプリング周波数を取得 */
TEST_F(GSAPI_TEST, TEST_GSAPI_GET_SAMPLERATE) {
    std::string samplerate;
    const bool res = gsapi_client::command_get_samplerate(samplerate);
    std::cout << samplerate << std::endl;
    EXPECT_EQ(res, true);
};

/* ツールにサンプリング周波数を設定 */
TEST_F(GSAPI_TEST, TEST_GSAPI_SET_SAMPLERATE) {
    const std::string samplerate = GS_SAMPLERATE_48000;
    const bool res = gsapi_client::command_set_samplerate(samplerate);
    std::cout << samplerate << std::endl;
    EXPECT_EQ(res, true);
};

/* リポジトリを検索(パッチ名) */
TEST_F(GSAPI_TEST, TEST_GSAPI_QUERY_PATCHNAMES_NAME) {
    const std::string text = "Magic"; /* 検索する文字列 */
    std::vector<std::string> patch_list;
    const bool res = gsapi_client::command_query_patchnames(text, true, false, false, patch_list);
    for (auto patch_name : patch_list) {
        std::cout << patch_name << std::endl;
    }
    EXPECT_EQ(res, true);
};

/* リポジトリを検索(カテゴリ) */
TEST_F(GSAPI_TEST, TEST_GSAPI_QUERY_PATCHNAMES_CATEGORY) {
    const std::string text = "Magic"; /* 検索する文字列 */
    std::vector<std::string> patch_list;
    const bool res = gsapi_client::command_query_patchnames(text, false, true, false, patch_list);
    for (auto patch_name : patch_list) {
        std::cout << patch_name << std::endl;
    }
    EXPECT_EQ(res, true);
};

/* リポジトリを検索(タグ) */
TEST_F(GSAPI_TEST, TEST_GSAPI_QUERY_PATCHNAMES_TAGS) {
    const std::string text = "Magic"; /* 検索する文字列 */
    std::vector<std::string> patch_list;
    const bool res = gsapi_client::command_query_patchnames(text, false, false, true, patch_list);
    for (auto patch_name : patch_list) {
        std::cout << patch_name << std::endl;
    }
    EXPECT_EQ(res, true);
};

/* リポジトリからパッチを取得してツール上で読み込む */
TEST_F(GSAPI_TEST, TEST_GSAPI_QUERY_PATCH) {
    const std::string text = "Ascent"; /* 検索する文字列 */
    const bool res = gsapi_client::command_query_patch(text);
    EXPECT_EQ(res, true);
};

/* リポジトリで使用可能なカテゴリのリスト */
TEST_F(GSAPI_TEST, TEST_GSAPI_QUERY_CATEGORIES) {
    std::vector<std::string> category_list;
    const bool res = gsapi_client::command_query_categories(category_list);
    for (auto category : category_list) {
        std::cout << category << std::endl;
    }
    EXPECT_EQ(res, true);
};

/* リポジトリで使用可能なタグのリスト */
TEST_F(GSAPI_TEST, TEST_GSAPI_QUERY_TAGS) {
    std::vector<std::string> tag_list;
    const bool res = gsapi_client::command_query_categories(tag_list);
    for (auto category : tag_list) {
        std::cout << category << std::endl;
    }
    EXPECT_EQ(res, true);
};

/* パッチを保存する */
TEST_F(GSAPI_TEST, TEST_GSAPI_SAVE_PATCH) {
    {
        /* パッチを保存するため、リポジトリから取得 */
        const std::string text = "Lightsabers"; /* 検索する文字列 */
        const bool res = gsapi_client::command_query_patch(text);
        EXPECT_EQ(res, true);
    }
    std::ostringstream oss;
    oss << CMAKE_TEST_SOURCE_DIR << "/resources/" << TEST_SAVE_FILE_NAME;
    std::string file_path = oss.str();
    const bool res = gsapi_client::command_save_patch(file_path);
    EXPECT_EQ(res, true);
};

/* パッチを読み込む */
TEST_F(GSAPI_TEST, TEST_GSAPI_LOAD_PATCH) {
    {
        /* パッチが切り替わることを確認するため、リポジトリから取得 */
        const std::string text = "Ascent"; /* 検索する文字列 */
        const bool res = gsapi_client::command_query_patch(text);
        EXPECT_EQ(res, true);
    }
    std::ostringstream oss;
    oss << CMAKE_TEST_SOURCE_DIR << "/resources/" << TEST_LOAD_FILE_NAME;
    std::string file_path = oss.str();
    const bool res = gsapi_client::command_load_patch(file_path);
    EXPECT_EQ(res, true);
};

/* パッチを出力する */
TEST_F(GSAPI_TEST, TEST_GSAPI_RENDER_PATCH) {
    std::ostringstream oss;
    oss << CMAKE_TEST_SOURCE_DIR << "/resources/" << TEST_RENDER_FILE_NAME;
    std::string file_path = oss.str();
    const unsigned int depth = 16;
    const unsigned int channel = 1;
    const unsigned int duration = 10;
    const bool res = gsapi_client::command_render_patch(file_path, depth, channel, duration);
    EXPECT_EQ(res, true);
};

/* 現在のモデル名を取得する */
TEST_F(GSAPI_TEST, TEST_GSAPI_GET_MODELNAME) {
    std::string model_name;
    const bool res = gsapi_client::command_get_modelname(model_name);
    std::cout << model_name << std::endl;
    EXPECT_EQ(res, true);
};

/* 現在のパッチ名を取得する */
TEST_F(GSAPI_TEST, TEST_GSAPI_GET_PATCHNAME) {
    std::string patch_name;
    const bool res = gsapi_client::command_get_patchname(patch_name);
    std::cout << patch_name << std::endl;
    EXPECT_EQ(res, true);
};

/* ランダムバリエーションを取得する */
TEST_F(GSAPI_TEST, TEST_GSAPI_GET_VARIATION) {
    float variation;
    const bool res = gsapi_client::command_get_variation(variation);
    std::cout << variation << std::endl;
    EXPECT_EQ(res, true);
};

/* ランダムバリエーションを設定する */
TEST_F(GSAPI_TEST, TEST_GSAPI_SET_VARIATION) {
    const float variation = 76.54f;
    const bool res = gsapi_client::command_set_variation(variation);
    std::cout << variation << std::endl;
    EXPECT_EQ(res, true);
};

/* スケッチパッドの曲線の情報を取得する */
TEST_F(GSAPI_TEST, TEST_GSAPI_GET_DRAWING) {
    std::vector<GsDrawingData> drawing_data;
    const bool res = gsapi_client::command_get_drawing(0, drawing_data);
    for (auto point : drawing_data) {
        std::cout << '(' << point.t << ',' << point.x << ',' << point.y << ',' << point.p << ')' << ',';
    }
    std::cout << std::endl;
    EXPECT_EQ(res, true);
};

/* スケッチパッドに曲線の情報を設定する */
TEST_F(GSAPI_TEST, TEST_GSAPI_SET_DRAWING) {
    const std::vector<GsDrawingData> drawing_data = {
        {0.f, 0.f, 0.f, 0.f},
        {0.5f, 0.5f, 0.5f, 1.f}
    };
    const bool res = gsapi_client::command_set_drawing(drawing_data);
    EXPECT_EQ(res, true);
};

/* メタパラメータの数を取得する */
TEST_F(GSAPI_TEST, TEST_GSAPI_GET_METACOUNT) {
    unsigned int meta_count;
    const bool res = gsapi_client::command_get_metacount(meta_count);
    std::cout << meta_count << std::endl;
    EXPECT_EQ(res, true);
};
/* メタパラメータ名のリストを取得する */
TEST_F(GSAPI_TEST, TEST_GSAPI_GET_METNAMES) {
    std::vector<std::string> meta_names;
    const bool res = gsapi_client::command_get_metanames(meta_names);
    for (auto patch_name : meta_names) {
        std::cout << patch_name << std::endl;
    }
    EXPECT_EQ(res, true);
};
/* インデックスで指定したメタパラメータの名前を取得する */
TEST_F(GSAPI_TEST, TEST_GSAPI_GET_METANAME) {
    const unsigned int index = 0; /* パッチで定義されているメタパラメータの番号 */
    std::string meta_name;
    const bool res = gsapi_client::command_get_metaname(index, meta_name);
    std::cout << meta_name << std::endl;
    EXPECT_EQ(res, true);
};

/* インデックスで指定したメタパラメータの値を取得する */
TEST_F(GSAPI_TEST, TEST_GSAPI_GET_METAVALUE_BY_INDEX) {
    const unsigned int index = 0; /* パッチで定義されているメタパラメータの番号 */
    float meta_value;
    const bool res = gsapi_client::command_get_metavalue(index, meta_value);
    std::cout << meta_value << std::endl;
    EXPECT_EQ(res, true);
};

/* 名前で指定したメタパラメータの値を取得する */
TEST_F(GSAPI_TEST, TEST_GSAPI_GET_METAVALUE_NAME) {
    const std::string name = "meta_param_01"; /* パッチで定義されているメタパラメータ名を指定 */
    float meta_value;
    const bool res = gsapi_client::command_get_metavalue(name, meta_value);
    std::cout << meta_value << std::endl;
    EXPECT_EQ(res, true);
};

/* インデックスで指定したメタパラメータに値を設定する */
TEST_F(GSAPI_TEST, TEST_GSAPI_SET_METAVALUE_BY_INDEX) {
    const unsigned int index = 0; /* パッチで定義されているメタパラメータの番号 */
    float meta_value = 0.67f;
    const bool res = gsapi_client::command_set_metavalue(index, meta_value);
    EXPECT_EQ(res, true);
};

/* 名前で指定したメタパラメータに値を設定する */
TEST_F(GSAPI_TEST, TEST_GSAPI_SET_METAVALUE_BY_NAME) {
    const std::string name = "meta_param_01"; /* パッチで定義されているメタパラメータ名を指定 */
    const float meta_value = 0.78f;
    const bool res = gsapi_client::command_set_metavalue(name, meta_value);
    EXPECT_EQ(res, true);
};

/* オートメーションカーブ値を取得する。 */
TEST_F(GSAPI_TEST, TEST_GSAPI_GET_CURVECOUNT) {
    unsigned int curve_count = 0;
    const bool res = gsapi_client::command_get_curvescount(curve_count);
    std::cout << curve_count << std::endl;
    EXPECT_EQ(res, true);
}

/* オートメーションカーブ名のリストを取得する。 */
TEST_F(GSAPI_TEST, TEST_GSAPI_GET_CURVENAMES) {
    std::vector<std::string> curve_names;
    const bool res = gsapi_client::command_get_curvenames(curve_names);
    for (auto curve_name : curve_names) {
        std::cout << curve_name << std::endl;
    }
    EXPECT_EQ(res, true);
}

/* インデックスで指定したオートメーションカーブの名前を取得する。 */
TEST_F(GSAPI_TEST, TEST_GSAPI_GET_CURVENAME) {
    const unsigned int curve_index = 0; /* パッチで定義されているオートメーションカーブの番号 */
    std::string curve_name;
    const bool res = gsapi_client::command_get_curvename(curve_index, curve_name);
    std::cout << curve_name << std::endl;
    EXPECT_EQ(res, true);
}

/* インデックスで指定したオートメーションカーブの値を取得する。 */
TEST_F(GSAPI_TEST, TEST_GSAPI_GET_CURVEVALUE_BY_INDEX) {
    const unsigned int curve_index = 0; /* パッチで定義されているオートメーションカーブの番号 */
    GsCurveValue curve_value;
    const bool res = gsapi_client::command_get_curvevalue(curve_index, curve_value);
    for (auto point : curve_value.curve) {
        std::cout << '(' << point.x << ',' << point.y << ')' << std::endl;
    }
    std::cout << curve_value.duration << std::endl;
    std::cout << curve_value.is_loop << std::endl;
    EXPECT_EQ(res, true);
}

/* 名前で指定したオートメーションカーブの値を取得する。 */
TEST_F(GSAPI_TEST, TEST_GSAPI_GET_CURVEVALUE_BY_NAME) {
    const std::string curve_name = "Noise Amplitude"; /* パッチで定義されているオートメーションカーブの名前 */
    GsCurveValue curve_value;
    const bool res = gsapi_client::command_get_curvevalue(curve_name, curve_value);
    for (auto point : curve_value.curve) {
        std::cout << '(' << point.x << ',' << point.y << ')' << std::endl;
    }
    std::cout << curve_value.duration << std::endl;
    std::cout << curve_value.is_loop << std::endl;
    EXPECT_EQ(res, true);
}

/* インデックスで指定したオートメーションカーブに値を設定する。 */
TEST_F(GSAPI_TEST, TEST_GSAPI_SET_CURVEVALUE_BY_INDEX) {
    const unsigned int curve_index = 0; /* パッチで定義されているオートメーションカーブの番号 */
    GsCurveValue curve_value;
    curve_value.curve = { {0,0}, {1,1} };
    curve_value.duration = 0.56f;
    curve_value.is_loop = false;
    const bool res = gsapi_client::command_set_curvevalue(curve_index, curve_value);
    EXPECT_EQ(res, true);
}

/* 名前で指定したオートメーションカーブに値を設定する。 */
TEST_F(GSAPI_TEST, TEST_GSAPI_SET_CURVEVALUE_BY_NAME) {
    const std::string curve_name = "Noise Amplitude"; /* パッチで定義されているオートメーションカーブの名前 */
    GsCurveValue curve_value;
    curve_value.curve = { {0,0}, {1,1} };
    curve_value.duration = 0.67f;
    curve_value.is_loop = true;
    const bool res = gsapi_client::command_set_curvevalue(curve_name, curve_value);
    EXPECT_EQ(res, true);
}

/* パッチを再生する */
TEST_F(GSAPI_TEST, TEST_GSAPI_PLAY) {
    const bool res = gsapi_client::command_play();
    EXPECT_EQ(res, true);
}

/* パッチを停止する */
TEST_F(GSAPI_TEST, TEST_GSAPI_STOP) {
    const bool res = gsapi_client::command_stop();
    EXPECT_EQ(res, true);
}

/* パッチが再生中か */
TEST_F(GSAPI_TEST, TEST_GSAPI_IS_PLAYING) {
    bool is_playing;
    const bool res = gsapi_client::command_is_playing(is_playing);
    std::cout << is_playing << std::endl;
    EXPECT_EQ(res, true);
}

/* パッチはループ再生されるか */
TEST_F(GSAPI_TEST, TEST_GSAPI_IS_INFINITE) {
    bool is_inifinite;
    const bool res = gsapi_client::command_is_infinite(is_inifinite);
    std::cout << is_inifinite << std::endl;
    EXPECT_EQ(res, true);
}

/* パッチはランダム性があるか */
TEST_F(GSAPI_TEST, TEST_GSAPI_IS_RANDOMIZED) {
    bool is_randomized;
    const bool res = gsapi_client::command_is_randomized(is_randomized);
    std::cout << is_randomized << std::endl;
    EXPECT_EQ(res, true);
}

/* ツールのイベント発火を有効にする */
TEST_F(GSAPI_TEST, TEST_GSAPI_ENABLE_EVENTS) {
    const bool flag = true;
    std::string event_name;
    const bool res = gsapi_client::command_enable_events(flag);
    EXPECT_EQ(res, true);
}

/* ツールを背面に移動する。 */
TEST_F(GSAPI_TEST, TEST_GS_WINDOW_BACK) {
    const bool res = gsapi_client::command_window_back();
    EXPECT_EQ(res, true);
};

/* ツールを前面に移動する。 */
TEST_F(GSAPI_TEST, TEST_GS_WINDOW_FRONT) {
    const bool res = gsapi_client::command_window_front();
    EXPECT_EQ(res, true);
};

/* ツールでメッセージボックスを表示する。 */
TEST_F(GSAPI_TEST, TEST_GS_WINDOW_MESSAGE) {
    const std::string message = "MessageBox";
    const GsWindowButton button = GS_WINDOW_BUTTON_OK;
    const bool res = gsapi_client::command_window_message(message, button);
    EXPECT_EQ(res, true);
};

/* ツールでパラメータ設定のダイアログを表示する。 */
TEST_F(GSAPI_TEST, TEST_GS_WINDOW_PARAMETERS) {
    std::vector<GsParameter> params;
    GsNumber param01;
    param01.name = "Number";
    params.push_back(param01);
    GsBool param02;
    param02.name = "Bool";
    params.push_back(param02);
    GsString param03;
    param03.name = "String";
    params.push_back(param03);
    GsEnum param04;
    param04.name = "Enum";
    param04.choices = {"Item01", "Item02"};
    params.push_back(param04);
    GsLabel param05;
    param05.text = "Label";
    params.push_back(param05);

    const bool res = gsapi_client::command_window_parameters(params);
    EXPECT_EQ(res, true);
};

/* ツールでレンダリング設定のダイアログを表示する。 */
TEST_F(GSAPI_TEST, TEST_GS_WINDOW_RENDERING) {
    const bool show_duration = true;
    const bool show_variations = true;
    const bool res = gsapi_client::command_window_rendering(show_duration, show_variations);
    EXPECT_EQ(res, true);
};

/* ツールでテスト用ダイアログを表示する。 */
TEST_F(GSAPI_TEST, TEST_GS_WINDOW_TEST) {
    const bool res = gsapi_client::command_window_test();
    EXPECT_EQ(res, true);
};
