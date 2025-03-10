/****************************************************************
 * @file    gsapi_client.cpp
 * @brief   GameSynth Tool APIを呼び出す
 * @version 1.0.8
 * @auther  ysd
 ****************************************************************/

/****************************************************************
 * インクルード
 ****************************************************************/
#include "../include/gsapi_client.h"
#include "../include/gsapi_commands.h"
#if (_WIN32)
    #include <WinSock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#endif
#include <algorithm>
#include <sstream>
#include <iostream>

/****************************************************************
 * プリプロセッサ定義
 ****************************************************************/
/* ツールとの通信時に待機する時間 */
#define WAIT_TO_READY_READ_MSEC         (100)
#define WAIT_TO_RECEIVE_MESSAGE_SEC     (1)
#define MAX_RETRY_COUNT                 (10)

/* ツールから受信するメッセージサイズ */
#define MAX_RECEIVE_MESSAGE_SIZE        (4096)

/* ツールから受信するメッセージに含まれるデリミタ */
#define MESSAGE_DELIMITER_SPACE         ' '
#define MESSAGE_DELIMITER_COMMA         ','

/* メタパラメータを取得するときに使用する文字列 */
#define GS_METAVALUE_BY_INDEX       "BY_INDEX"
#define GS_METAVALUE_BY_NAME        "BY_NAME"
/* オートメーションカーブを取得するときに使用する文字列 */
#define GS_CURVE_BY_INDEX           "BY_INDEX"
#define GS_CURVE_BY_NAME            "BY_NAME"

/****************************************************************
 * 構造体宣言
 ****************************************************************/

/****************************************************************
 * 変数宣言
 ****************************************************************/

/****************************************************************
 * 変数定義
 ****************************************************************/
GsApiClientConfig gsapi_client::gs_config;

/****************************************************************
 * 関数宣言
 ****************************************************************/
static bool string_split(const std::string& commands, const char delimiter, std::vector<std::string>& command_list);
static bool enum_to_string(const GsWindowButton type, std::string& text);
static bool enum_to_string(const GsDataType type, std::string& text);
static bool enum_to_string(const GsNumberSubType type, std::string& text);
static bool enum_to_string(const GsStringSubType type, std::string& text);
static bool enum_to_string(const GsEnumSubType type, std::string& text);
static bool enum_to_string(const GsLabelSubType type, std::string& text);
static bool enum_to_string(const GsLabelAlignment type, std::string& text);

/****************************************************************
 * 関数定義
 ****************************************************************/
bool string_split(const std::string& commands, const char delimiter, std::vector<std::string>& command_list)
{
    std::stringstream ss(commands);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
        command_list.push_back(item);
    }
    return true;
}

static bool enum_to_string(const GsWindowButton type, std::string& text)
{
    switch (type) {
    case GS_WINDOW_BUTTON_OK:
        text = "OK";
        break;
    case GS_WINDOW_BUTTON_OK_CANCEL:
        text = "OK_CANCEL";
        break;
    case GS_WINDOW_BUTTON_YES_NO:
        text = "YES_NO";
        break;
    case GS_WINDOW_BUTTON_RETRY_EXIT:
        text = "RETRY_EXIT";
        break;
    default:
        return false;
    }
    return true;
}

static bool enum_to_string(const GsDataType type, std::string& text)
{
    switch (type) {
    case GS_DATA_TYPE_NUMBER:
        text = "NUMBER";
        break;
    case GS_DATA_TYPE_BOOL:
        text = "BOOL";
        break;
    case GS_DATA_TYPE_STRING:
        text = "STRING";
        break;
    case GS_DATA_TYPE_ENUM:
        text = "ENUM";
        break;
    case GS_DATA_TYPE_LABEL:
        text = "LABEL";
        break;
    default:
        return false;
    }
    return true;
}

static bool enum_to_string(const GsNumberSubType type, std::string& text)
{
    switch (type) {
    case GS_NUMBER_SUB_TYPE_INTEGER:
        text = "INTEGER";
        break;
    case GS_NUMBER_SUB_TYPE_FLOAT:
        text = "FLOAT";
        break;
    default:
        return false;
    }
    return true;
}

static bool enum_to_string(const GsStringSubType type, std::string& text)
{
    switch (type) {
    case GS_STRING_SUB_TYPE_NORMAL:
        text = "NORMAL";
        break;
    case GS_STRING_SUB_TYPE_FILELOAD:
        text = "FILELOAD";
        break;
    case GS_STRING_SUB_TYPE_FILESAVE:
        text = "FILESAVE";
        break;
    case GS_STRING_SUB_TYPE_FOLDER:
        text = "FOLDER";
        break;
    default:
        return false;
    }
    return true;
}

static bool enum_to_string(const GsEnumSubType type, std::string& text)
{
    switch (type) {
    case GS_ENUM_SUB_TYPE_LIST:
        text = "LIST";
        break;
    case GS_ENUM_SUB_TYPE_COMBO:
        text = "COMBO";
        break;
    default:
        return false;
    }
    return true;
}

static bool enum_to_string(const GsLabelSubType type, std::string& text)
{
    switch (type) {
    case GS_LABEL_SUB_TYPE_TEXT:
        text = "TEXT";
        break;
    case GS_LABEL_SUB_TYPE_HEADER:
        text = "HEADER";
        break;
    default:
        return false;
    }
    return true;
}

static bool enum_to_string(const GsLabelAlignment type, std::string& text)
{
    switch (type) {
    case GS_LABEL_SUB_ALIGNMENT_LEFT:
        text = "LEFT";
        break;
    case GS_LABEL_SUB_ALIGNMENT_RIGHT:
        text = "RIGHT";
        break;
    case GS_LABEL_SUB_ALIGNMENT_CENTER:
        text = "CENTER";
        break;
    default:
        return false;
    }
    return true;
}

/****************************************************************
 * クラス定義
 ****************************************************************/
bool gsapi_client::set_default_config(const GsApiClientConfig& config)
{
    gs_config = config;
    return true;
}

bool gsapi_client::get_default_config(GsApiClientConfig& config)
{
    config = gs_config;
    return true;
}

bool gsapi_client::send_command(const std::string& message, std::string& response)
{
#if (_WIN32)
    WSADATA         wsaData;
    SOCKET          sock;
    sockaddr_in     server;
    timeval         recv_tv;
    char            buffer[MAX_RECEIVE_MESSAGE_SIZE] = { 0 };
    int             len = 0;
    int             result = 0;

    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        perror("[gsmodule]failed to call WSAStartup.\n");
        return false;
    }

    /* ソケット作成 */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        perror("[gsmodule]failed to create socket.\n");
        WSACleanup();
        return false;
    }

    /* 接続先のアドレス設定 */
    server.sin_family = AF_INET;
    server.sin_port = htons(gs_config.port_number);
    inet_pton(AF_INET, gs_config.ip_address.c_str(), &server.sin_addr);

    /* 接続 */
    result = connect(sock, (sockaddr*)&server, sizeof(server));
    if (result < 0) {
        perror("[gsmodule]failed to connect server.\n");
        closesocket(sock);
        WSACleanup();
        return false;
    }

    /* メッセージの長さを取得し、メッセージ全体が送られるようにする */
    size_t total_sent = 0;
    size_t message_len = strlen(message.c_str());
    while (total_sent < message_len) {
        size_t sent = send(sock, message.c_str() + total_sent, message_len - total_sent, 0);
        if (sent == -1) {
            if (errno == EINTR) {
                // シグナルによる中断。再試行
                continue;
            }
            perror("[gsmodule]failed to send message.\n");
            closesocket(sock);
            WSACleanup();
            return false;
        }
        total_sent += sent;
    }
    if (total_sent != message_len) {
        perror("[gsmodule]failed to send entire message.\n");
        closesocket(sock);
        WSACleanup();
        return false;
    }

    /* タイムアウト設定 */
    recv_tv.tv_sec = WAIT_TO_RECEIVE_MESSAGE_SEC;
    recv_tv.tv_usec = 0;
    result = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&recv_tv, sizeof(recv_tv));
    if (result < 0) {
        perror("[gsmodule]failed to set socket option.\n");
        return false;
    }

    /* メッセージを正しく受け取れているか確認する */
    int retry_count = 0;
    while (1) {
        len = recv(sock, buffer, sizeof(buffer), 0);
        if (len < 0) {
            //if (errno == EINTR) {
            //    continue;
            //} else if (errno == EAGAIN) {
            //    perror("[gsmodule]failed to receive message due to timeout.");
            //} else {
            //    perror("[gsmodule]failed to receive message.");
            //}
            //break;
            if (retry_count > MAX_RETRY_COUNT) {
                perror("[gsmodule]failed to receive message.");
                break;
            }
            else {
                Sleep(WAIT_TO_READY_READ_MSEC);
                retry_count++;
            }
        }
        else {
            response = buffer;
            break;
        }
    }

    closesocket(sock);
    WSACleanup();
#endif

    return true;
}

bool gsapi_client::is_connect()
{
    /* 無効なコマンドでもメッセージが送信できれば良い */
    std::string response;
    const std::string send_message = gs_config.delimiter;
    return send_command(send_message, response);
}

bool gsapi_client::command_get_version(std::string& version)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_GET_VERSION << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    version = response;
    return result;
}

bool gsapi_client::command_get_commands(std::vector<std::string>& commmand_list)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_GET_COMMANDS << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    string_split(response, MESSAGE_DELIMITER_COMMA, commmand_list);
    return result;
}

bool gsapi_client::command_get_models(std::vector<std::string>& model_list)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_GET_MODELS << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    string_split(response, MESSAGE_DELIMITER_COMMA, model_list);
    return result;
}

bool gsapi_client::command_select_model(const std::string& model_name)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_SELECT_MODEL
        << MESSAGE_DELIMITER_SPACE << model_name
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gsapi_client::command_get_path(const std::string& path_name, std::string& path_value)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_GET_PATH
        << MESSAGE_DELIMITER_SPACE << path_name
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    path_value = response;
    return result;
}

bool gsapi_client::command_get_samplerate(std::string& samplerate)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_GET_SAMPLERATE << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    samplerate = response;
    return result;
}

bool gsapi_client::command_set_samplerate(const std::string& samplerate)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_SET_SAMPLERATE
        << MESSAGE_DELIMITER_SPACE << samplerate
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gsapi_client::command_query_patchnames(const std::string& text, const bool name,
    const bool category, const bool tags, std::vector<std::string>& patch_list)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_QUERY_PATCHNAMES
        << MESSAGE_DELIMITER_SPACE << text
        << MESSAGE_DELIMITER_SPACE << (name ? "1" : "0")
        << MESSAGE_DELIMITER_SPACE << (category ? "1" : "0")
        << MESSAGE_DELIMITER_SPACE << (tags ? "1" : "0")
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    string_split(response, MESSAGE_DELIMITER_COMMA, patch_list);
    return result;
}

bool gsapi_client::command_query_patch(const std::string& patch_name)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_QUERY_PATCH
        << MESSAGE_DELIMITER_SPACE << patch_name
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gsapi_client::command_query_categories(std::vector<std::string>& categoryt_list)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_QUERY_CATEGORIES << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    string_split(response, MESSAGE_DELIMITER_COMMA, categoryt_list);
    return result;
}

bool gsapi_client::command_query_tags(std::vector<std::string>& tag_list)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_QUERY_TAGS << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    string_split(response, MESSAGE_DELIMITER_COMMA, tag_list);
    return result;
}

bool gsapi_client::command_load_patch(const std::string& file_path)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_LOAD_PATCH
        << MESSAGE_DELIMITER_SPACE
        << file_path
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gsapi_client::command_save_patch(const std::string& file_path)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_SAVE_PATCH
        << MESSAGE_DELIMITER_SPACE
        << file_path
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gsapi_client::command_render_patch(const std::string& file_path, const unsigned int depth,
    const unsigned int channel, const unsigned int duration)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_RENDER_PATCH
        << MESSAGE_DELIMITER_SPACE << file_path
        << MESSAGE_DELIMITER_SPACE << std::to_string(depth)
        << MESSAGE_DELIMITER_SPACE << std::to_string(channel)
        << MESSAGE_DELIMITER_SPACE << std::to_string(duration)
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gsapi_client::command_get_modelname(std::string& model_name)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_GET_MODELNAME << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    model_name = response;
    return result;
}

bool gsapi_client::command_get_patchname(std::string& patch_name)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_GET_PATCHNAME << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    patch_name = response;
    return result;
}

bool gsapi_client::command_get_variation(float& variation)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_GET_VARIATION << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    variation = static_cast<float>(std::stof(response));
    return result;
}

bool gsapi_client::command_set_variation(const float& variation)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_SET_VARIATION
        << MESSAGE_DELIMITER_SPACE << variation
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gsapi_client::command_get_drawing(const unsigned int index, std::vector<GsDrawingData>& drawing_data)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_GET_DRAWING
        << MESSAGE_DELIMITER_SPACE << index
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);

    response.erase(std::remove(response.begin(), response.end(), '\r'), response.end());
    response.erase(std::remove(response.begin(), response.end(), '('), response.end());
    std::vector<std::string> point_list;
    string_split(response, ')', point_list);
    /* スケッチパッドの曲線の情報をパースする */
    for (auto point : point_list) {
        std::vector<std::string> param_list;
        string_split(point, ',', param_list);
        if (param_list.size() != 4) {
            continue;
        }
        GsDrawingData drawing;
        drawing.t = std::stof(param_list[0]);
        drawing.x = std::stof(param_list[1]);
        drawing.y = std::stof(param_list[2]);
        drawing.p = std::stof(param_list[3]);
        drawing_data.push_back(drawing);
    }
    return result;
}

bool gsapi_client::command_set_drawing(const std::vector<GsDrawingData>& drawing_data)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_SET_DRAWING
        << MESSAGE_DELIMITER_SPACE;
    for (auto it = drawing_data.begin(); it != drawing_data.end(); ++it) {
        if (it != drawing_data.begin()) {
            oss << ',';
        }
        oss << '('
            << it->t << ','
            << it->x << ','
            << it->y << ','
            << it->p << ')';
    }
    oss << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gsapi_client::command_get_metacount(unsigned int& meta_count)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_GET_METACOUNT << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    meta_count = std::stoi(response);
    return result;
}

bool gsapi_client::command_get_metanames(std::vector<std::string>& meta_names)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_GET_METANAMES << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    string_split(response, MESSAGE_DELIMITER_COMMA, meta_names);
    return result;
}

bool gsapi_client::command_get_metaname(const unsigned int& index, std::string& metaname)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_GET_METANAME
        << MESSAGE_DELIMITER_SPACE << index
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    metaname = response;
    return result;
}

bool gsapi_client::command_get_metavalue(const unsigned int& index, float& metavalue)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_GET_METAVALUE
        << MESSAGE_DELIMITER_SPACE << GS_METAVALUE_BY_INDEX
        << MESSAGE_DELIMITER_SPACE << index
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    metavalue = std::stof(response);
    return result;
}

bool gsapi_client::command_get_metavalue(const std::string& name, float& metavalue)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_GET_METAVALUE
        << MESSAGE_DELIMITER_SPACE << GS_METAVALUE_BY_NAME
        << MESSAGE_DELIMITER_SPACE << name
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    metavalue = std::stof(response);
    return result;
}
bool gsapi_client::command_set_metavalue(const unsigned int& index, const float& metavalue)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_SET_METAVALUE
        << MESSAGE_DELIMITER_SPACE << GS_METAVALUE_BY_INDEX
        << MESSAGE_DELIMITER_SPACE << index
        << MESSAGE_DELIMITER_SPACE << metavalue
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gsapi_client::command_set_metavalue(const std::string& name, const float& metavalue)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_SET_METAVALUE
        << MESSAGE_DELIMITER_SPACE << GS_METAVALUE_BY_NAME
        << MESSAGE_DELIMITER_SPACE << name
        << MESSAGE_DELIMITER_SPACE << metavalue
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gsapi_client::command_get_curvescount(unsigned int& curves_count)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_GET_CURVESCOUNT << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    curves_count = std::stoi(response);
    return result;
}

bool gsapi_client::command_get_curvenames(std::vector<std::string>& curve_names)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_GET_CURVENAMES << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    string_split(response, MESSAGE_DELIMITER_COMMA, curve_names);
    return result;
}

bool gsapi_client::command_get_curvename(const unsigned int& curve_index, std::string& curve_name)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_GET_CURVENAME
        << MESSAGE_DELIMITER_SPACE << curve_index
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    curve_name = response;
    return result;
}

bool gsapi_client::command_get_curvevalue(const unsigned int& curve_index, GsCurveValue& curve_value)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_GET_CURVEVALUE
        << MESSAGE_DELIMITER_SPACE << GS_CURVE_BY_INDEX
        << MESSAGE_DELIMITER_SPACE << curve_index
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);

    std::vector<std::string> curve_params;
    response.erase(std::remove(response.begin(), response.end(), '('), response.end());
    string_split(response, ' ', curve_params);
    if (curve_params.size() != 3) {
        /* 想定しているデータではない */
        return false;
    }
    std::vector<std::string> point_list;
    string_split(response, ')', point_list);
    /* オートメーションカーブの情報をパースする */
    for (auto point : point_list) {
        std::vector<std::string> param_list;
        string_split(point, ',', param_list);
        if (param_list.size() != 2) {
            continue;
        }
        GsCurvePoint curve_point;
        curve_point.x = std::stof(param_list[0]);
        curve_point.y = std::stof(param_list[1]);
        curve_value.curve.push_back(curve_point);
    }
    curve_value.duration = std::stof(curve_params[1]);
    curve_value.is_loop = (std::stoi(curve_params[2]) == 1) ? true : false;

    return result;
}

bool gsapi_client::command_get_curvevalue(const std::string& curve_name, GsCurveValue& curve_value)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_GET_CURVEVALUE
        << MESSAGE_DELIMITER_SPACE << GS_CURVE_BY_NAME
        << MESSAGE_DELIMITER_SPACE << "\"" << curve_name << "\""
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);

    std::vector<std::string> curve_params;
    response.erase(std::remove(response.begin(), response.end(), '('), response.end());
    string_split(response, ' ', curve_params);
    if (curve_params.size() != 3) {
        /* 想定しているデータではない */
        return false;
    }
    std::vector<std::string> point_list;
    string_split(response, ')', point_list);
    /* オートメーションカーブの情報をパースする */
    for (auto point : point_list) {
        std::vector<std::string> param_list;
        string_split(point, ',', param_list);
        if (param_list.size() != 2) {
            continue;
        }
        GsCurvePoint curve_point;
        curve_point.x = std::stof(param_list[0]);
        curve_point.y = std::stof(param_list[1]);
        curve_value.curve.push_back(curve_point);
    }
    curve_value.duration = std::stof(curve_params[1]);
    curve_value.is_loop = (std::stoi(curve_params[2]) == 1) ? true : false;

    return result;
}

bool gsapi_client::command_set_curvevalue(const unsigned int& curve_index, const GsCurveValue& curve_value)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_SET_CURVEVALUE
        << MESSAGE_DELIMITER_SPACE << GS_CURVE_BY_INDEX
        << MESSAGE_DELIMITER_SPACE << curve_index
        << MESSAGE_DELIMITER_SPACE << "\"";
    for (auto it = curve_value.curve.begin(); it != curve_value.curve.end(); ++it) {
        if (it != curve_value.curve.begin()) {
            oss << ',';
        }
        oss << '(' << it->x << ',' << it->y << ')';
    }
    oss << "\""
        << MESSAGE_DELIMITER_SPACE << curve_value.duration
        << MESSAGE_DELIMITER_SPACE << ((curve_value.is_loop == true) ? 1 : 0)
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gsapi_client::command_set_curvevalue(const std::string& curve_name, const GsCurveValue& curve_value)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_SET_CURVEVALUE
        << MESSAGE_DELIMITER_SPACE << GS_CURVE_BY_NAME
        << MESSAGE_DELIMITER_SPACE << "\"" << curve_name << "\""
        << MESSAGE_DELIMITER_SPACE << "\"";
    for (auto it = curve_value.curve.begin(); it != curve_value.curve.end(); ++it) {
        if (it != curve_value.curve.begin()) {
            oss << ',';
        }
        oss << '(' << it->x << ',' << it->y << ')';
    }
    oss << "\""
        << MESSAGE_DELIMITER_SPACE << curve_value.duration
        << MESSAGE_DELIMITER_SPACE << ((curve_value.is_loop == true) ? 1 : 0)
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gsapi_client::command_play()
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_PLAY << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gsapi_client::command_stop()
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_STOP << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gsapi_client::command_is_playing(bool& is_playing)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_IS_PLAYING << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    is_playing = (std::stoi(response) == 1) ? true : false;
    return result;
}

bool gsapi_client::command_is_infinite(bool& is_infinite)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_IS_INFINITE << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    is_infinite = (std::stoi(response) == 1) ? true : false;
    return result;
}

bool gsapi_client::command_is_randomized(bool& is_randomized)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_IS_RANDOMIZED << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    is_randomized = (std::stoi(response) == 1) ? true : false;
    return result;
}

bool gsapi_client::command_enable_events(const bool is_notification)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_ENABLE_EVENTS
        << MESSAGE_DELIMITER_SPACE << ((is_notification == true) ? 1 : 0)
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gsapi_client::command_window_back()
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_WINDOW_BACK
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gsapi_client::command_window_front()
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_WINDOW_FRONT
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gsapi_client::command_window_message(const std::string& message, const GsWindowButton& button)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_WINDOW_MESSAGE
        << MESSAGE_DELIMITER_SPACE << message;
    std::string button_setting;
    if (!enum_to_string(button, button_setting)) {
        return false;
    }
    oss << MESSAGE_DELIMITER_SPACE << button_setting
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gsapi_client::command_window_parameters(const std::vector<GsParameter>& params)
{
    if (params.size() == 0) {
        return false;
    }

    std::string response;
    std::ostringstream oss;
    std::string message;
    oss << GSAPI_WINDOW_PARAMETERS;
    for (auto param : params) {
        if (std::holds_alternative<GsNumber>(param)) {
            /* 数値パラメーター {NUMBER, "Name", Type, "Unit", Min, Max, Def, Decimals} */
            const GsNumber& gsNumber = std::get<GsNumber>(param);
            std::string type;
            std::string sub_type;
            if (!enum_to_string(gsNumber.type, type)) {
                continue;
            }
            if (!enum_to_string(gsNumber.sub_type, sub_type)) {
                continue;
            }
            oss << MESSAGE_DELIMITER_SPACE << "{" << type 
                << ",\""<< gsNumber.name << "\"," << sub_type << ",\""<< gsNumber.unit << "\"," 
                << gsNumber.min_value <<  "," << gsNumber.max_value << "," << gsNumber.default_value << "," << gsNumber.decimals << "}";
        } else if (std::holds_alternative<GsBool>(param)) {
            /* 真偽値パラメーター {BOOL, "Name", Def} */
            const GsBool& gsBool = std::get<GsBool>(param);
            std::string type;
            if (!enum_to_string(gsBool.type, type)) {
                continue;
            }
            oss << MESSAGE_DELIMITER_SPACE << "{" << type 
            <<",\"" << gsBool.name << "\"," << ((gsBool.default_value)?"TRUE":"FALSE") << "}";
        } else if (std::holds_alternative<GsString>(param)) {
            /* 文字列パラメーター {STRING, "Name", Type, "Def" */
            const GsString& gsString = std::get<GsString>(param);
            std::string type;
            std::string sub_type;
            if (!enum_to_string(gsString.type, type)) {
                continue;
            }
            if (!enum_to_string(gsString.sub_type, sub_type)) {
                continue;
            }
            oss << MESSAGE_DELIMITER_SPACE << "{" << type 
            << ",\"" << gsString.name << "\"," << sub_type << ",\"" << gsString.default_value << "\"}";
        } else if (std::holds_alternative<GsEnum>(param)) {
            /* 列挙パラメーター {ENUM, "Name", Type, "[Choices]", Def} */
            const GsEnum& gsEnum = std::get<GsEnum>(param);
            std::string type;
            std::string sub_type;
            if (gsEnum.choices.size() == 0) {
                continue;
            }
            if (!enum_to_string(gsEnum.type, type)) {
                continue;
            }
            if (!enum_to_string(gsEnum.sub_type, sub_type)) {
                continue;
            }
            oss << MESSAGE_DELIMITER_SPACE << "{" << type 
            << ",\"" << gsEnum .name << "\"," << sub_type << ",\"";
            for (auto it = gsEnum.choices.begin(); it != gsEnum.choices.end(); it++) {
                if (it != gsEnum.choices.begin()) {
                    oss << "," << MESSAGE_DELIMITER_SPACE;
                }
                oss << *it;
            }
            oss <<"\"," << gsEnum.choices.at(gsEnum.default_choice) << "}";
        } else if (std::holds_alternative<GsLabel>(param)) {
            /* ラベルパラメーター {LABEL, "Text", Type, Alignment} */
            const GsLabel& gsLabel = std::get<GsLabel>(param);
            std::string type;
            std::string sub_type;
            std::string alignment;
            if (!enum_to_string(gsLabel.type, type)) {
                continue;
            }
            if (!enum_to_string(gsLabel.sub_type, sub_type)) {
                continue;
            }
            if (!enum_to_string(gsLabel.alignment, alignment)) {
                continue;
            }
            oss << MESSAGE_DELIMITER_SPACE << "{" << type 
            << ",\"" << gsLabel.text << "\"," << sub_type << "," << alignment << "}";
        }
    }
    oss << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gsapi_client::command_window_rendering(const bool& show_duration, const bool& show_variations)
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_WINDOW_RENDERING
        << MESSAGE_DELIMITER_SPACE << ((show_duration) ? 1 : 0)
        << MESSAGE_DELIMITER_SPACE << ((show_variations) ? 1 : 0)
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gsapi_client::command_window_test()
{
    std::string response;
    std::ostringstream oss;
    oss << GSAPI_WINDOW_TEST
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}
