/****************************************************************
 * @file    main.cpp
 * @brief   gsmoduleのテスト
 * @version 1.0.0
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

#define GS_SAPMLERATE_44100             "44100"                 /* 44100Hz */
#define GS_SAMPLERATE_48000             "48000"                 /* 48000Hz */

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
