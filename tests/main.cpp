/****************************************************************
 * @file    main.cpp
 * @brief   gsmoduleのテスト
 * @version 1.0.2
 * @auther  ysd
 ****************************************************************/

/****************************************************************
 * インクルード
 ****************************************************************/
#include <gs_api_commands.h>
#include <gs_api_interface.h>
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
class GS_API_TEST : public testing::Test {
protected:
    GS_API_TEST() {
    }
    virtual ~GS_API_TEST() {
    }

    /* 最初のテスト前に実行 */
    static void SetUpTestCase() {
        bool res;
        GsApiInterfaceConfig gs_config;
        res = gs_api_interface::get_default_config(gs_config);
        EXPECT_EQ(res, true);
#if 0
        /* GameSynth側の接続設定に合わせて変更 */
        gs_config.port_number = 28542;
        gs_config.ip_address = "127.0.0.1";
        gs_config.codec = "UTF-8";
        gs_config.delimiter = "\r";
#endif
        res = gs_api_interface::set_default_config(gs_config);
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

/* ツールと通信可能か */
TEST_F(GS_API_TEST, TEST_GS_CONNECT) {
    const bool res = gs_api_interface::is_connect();
    EXPECT_EQ(res, true);
};

/* ツールのバージョン */
TEST_F(GS_API_TEST, TEST_GS_API_GET_VERSION) {
    std::string version;
    const bool res = gs_api_interface::command_get_version(version);
    std::cout << version << std::endl;
    EXPECT_EQ(res, true);
};

/* ツールで使用可能なコマンドのリスト */
TEST_F(GS_API_TEST, TEST_GS_API_GET_COMMANDS) {
    std::vector<std::string> commmand_list;
    const bool res = gs_api_interface::command_get_commands(commmand_list);
    for (auto command : commmand_list) {
        std::cout << command << std::endl;
    }
    EXPECT_EQ(res, true);
};

/* ツールで使用可能なモデルのリスト */
TEST_F(GS_API_TEST, TEST_GS_API_GET_MODELS) {
    std::vector<std::string> model_list;
    const bool res = gs_api_interface::command_get_models(model_list);
    for (auto model : model_list) {
        std::cout << model << std::endl;
    }
    EXPECT_EQ(res, true);
};

/* ツールのシステムパス */
TEST_F(GS_API_TEST, TEST_GS_API_GET_PATH_) {
    const std::string path_name = GS_PATH_PATCH_FOLDER;
    std::string path_value;
    const bool res = gs_api_interface::command_get_path(path_name, path_value);
    std::cout << path_value << std::endl;
    EXPECT_EQ(res, true);
};

/* ツールのサンプリング周波数を取得 */
TEST_F(GS_API_TEST, TEST_GS_API_GET_SAMPLERATE) {
    std::string samplerate;
    const bool res = gs_api_interface::command_get_samplerate(samplerate);
    std::cout << samplerate << std::endl;
    EXPECT_EQ(res, true);
};

/* ツールにサンプリング周波数を設定 */
TEST_F(GS_API_TEST, TEST_GS_API_SET_SAMPLERATE) {
    const std::string samplerate = GS_SAMPLERATE_48000;
    const bool res = gs_api_interface::command_set_samplerate(samplerate);
    std::cout << samplerate << std::endl;
    EXPECT_EQ(res, true);
};

/* リポジトリを検索(パッチ名) */
TEST_F(GS_API_TEST, TEST_GS_API_QUERY_PATCHNAMES_NAME) {
    const std::string text = "Magic"; /* 検索する文字列 */
    std::vector<std::string> patch_list;
    const bool res = gs_api_interface::command_query_patchnames(text, true, false, false, patch_list);
    for (auto patch_name : patch_list) {
        std::cout << patch_name << std::endl;
    }
    EXPECT_EQ(res, true);
};

/* リポジトリを検索(カテゴリ) */
TEST_F(GS_API_TEST, TEST_GS_API_QUERY_PATCHNAMES_CATEGORY) {
    const std::string text = "Magic"; /* 検索する文字列 */
    std::vector<std::string> patch_list;
    const bool res = gs_api_interface::command_query_patchnames(text, false, true, false, patch_list);
    for (auto patch_name : patch_list) {
        std::cout << patch_name << std::endl;
    }
    EXPECT_EQ(res, true);
};

/* リポジトリを検索(タグ) */
TEST_F(GS_API_TEST, TEST_GS_API_QUERY_PATCHNAMES_TAGS) {
    const std::string text = "Magic"; /* 検索する文字列 */
    std::vector<std::string> patch_list;
    const bool res = gs_api_interface::command_query_patchnames(text, false, false, true, patch_list);
    for (auto patch_name : patch_list) {
        std::cout << patch_name << std::endl;
    }
    EXPECT_EQ(res, true);
};

/* リポジトリからパッチを取得してツール上で読み込む */
TEST_F(GS_API_TEST, TEST_GS_API_QUERY_PATCH) {
    const std::string text = "Ascent"; /* 検索する文字列 */
    const bool res = gs_api_interface::command_query_patch(text);
    EXPECT_EQ(res, true);
};

/* リポジトリで使用可能なカテゴリのリスト */
TEST_F(GS_API_TEST, TEST_GS_API_QUERY_CATEGORIES) {
    std::vector<std::string> category_list;
    const bool res = gs_api_interface::command_query_categories(category_list);
    for (auto category : category_list) {
        std::cout << category << std::endl;
    }
    EXPECT_EQ(res, true);
};

/* リポジトリで使用可能なタグのリスト */
TEST_F(GS_API_TEST, TEST_GS_API_QUERY_TAGS) {
    std::vector<std::string> tag_list;
    const bool res = gs_api_interface::command_query_categories(tag_list);
    for (auto category : tag_list) {
        std::cout << category << std::endl;
    }
    EXPECT_EQ(res, true);
};

/* パッチを保存する */
TEST_F(GS_API_TEST, TEST_GS_API_SAVE_PATCH) {
    {
        /* パッチを保存するため、リポジトリから取得 */
        const std::string text = "Lightsabers"; /* 検索する文字列 */
        const bool res = gs_api_interface::command_query_patch(text);
        EXPECT_EQ(res, true);
    }
    std::ostringstream oss;
    oss << CMAKE_TEST_SOURCE_DIR << "/resources/" << TEST_SAVE_FILE_NAME;
    std::string file_path = oss.str();
    const bool res = gs_api_interface::command_save_patch(file_path);
    EXPECT_EQ(res, true);
};

/* パッチを読み込む */
TEST_F(GS_API_TEST, TEST_GS_API_LOAD_PATCH) {
    {
        /* パッチが切り替わることを確認するため、リポジトリから取得 */
        const std::string text = "Ascent"; /* 検索する文字列 */
        const bool res = gs_api_interface::command_query_patch(text);
        EXPECT_EQ(res, true);
    }
    std::ostringstream oss;
    oss << CMAKE_TEST_SOURCE_DIR << "/resources/" << TEST_LOAD_FILE_NAME;
    std::string file_path = oss.str();
    const bool res = gs_api_interface::command_load_patch(file_path);
    EXPECT_EQ(res, true);
};

/* パッチを出力する */
TEST_F(GS_API_TEST, TEST_GS_API_RENDER_PATCH) {
    std::ostringstream oss;
    oss << CMAKE_TEST_SOURCE_DIR << "/resources/" << TEST_RENDER_FILE_NAME;
    std::string file_path = oss.str();
    const unsigned int depth = 16;
    const unsigned int channel = 1;
    const unsigned int duration = 10;
    const bool res = gs_api_interface::command_render_patch(file_path, depth, channel, duration);
    EXPECT_EQ(res, true);
};

/* 現在のモデル名を取得する */
TEST_F(GS_API_TEST, TEST_GS_API_GET_MODELNAME) {
    std::string model_name;
    const bool res = gs_api_interface::command_get_modelname(model_name);
    std::cout << model_name << std::endl;
    EXPECT_EQ(res, true);
};

/* 現在のパッチ名を取得する */
TEST_F(GS_API_TEST, TEST_GS_API_GET_PATCHNAME) {
    std::string patch_name;
    const bool res = gs_api_interface::command_get_patchname(patch_name);
    std::cout << patch_name << std::endl;
    EXPECT_EQ(res, true);
};

/* ランダムバリエーションを取得する */
TEST_F(GS_API_TEST, TEST_GS_API_GET_VARIATION) {
    float variation;
    const bool res = gs_api_interface::command_get_variation(variation);
    std::cout << variation << std::endl;
    EXPECT_EQ(res, true);
};

/* ランダムバリエーションを設定する */
TEST_F(GS_API_TEST, TEST_GS_API_SET_VARIATION) {
    const float variation = 76.54f;
    const bool res = gs_api_interface::command_set_variation(variation);
    std::cout << variation << std::endl;
    EXPECT_EQ(res, true);
};

/* スケッチパッドの曲線の情報を取得する */
TEST_F(GS_API_TEST, TEST_GS_API_GET_DRAWING) {
    std::vector<GsDrawingData> drawing_data;
    const bool res = gs_api_interface::command_get_drawing(0, drawing_data);
    for (auto point : drawing_data) {
        std::cout << '(' << point.t << ',' << point.x << ',' << point.y << ',' << point.p << ')' << ',';
    }
    std::cout << std::endl;
    EXPECT_EQ(res, true);
};

/* スケッチパッドに曲線の情報を設定する */
TEST_F(GS_API_TEST, TEST_GS_API_SET_DRAWING) {
    const std::vector<GsDrawingData> drawing_data = {
        {0.f, 0.f, 0.f, 0.f},
        {0.5f, 0.5f, 0.5f, 1.f}
    };
    const bool res = gs_api_interface::command_set_drawing(drawing_data);
    EXPECT_EQ(res, true);
};
