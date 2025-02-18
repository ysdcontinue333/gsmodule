/****************************************************************
 * @file    gs_api_interface.cpp
 * @brief   GameSynth Tool APIを呼び出す
 * @version 1.0.5
 * @auther  ysd
 ****************************************************************/

/****************************************************************
 * インクルード
 ****************************************************************/
#include "../include/gs_api_interface.h"
#include "../include/gs_api_commands.h"
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
#define WAIT_TO_READY_READ_MSEC         (300) /* 備考:読込待機しないと一部コマンドで失敗する */
#define WAIT_TO_RECEIVE_MESSAGE_SEC     (1)

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
GsApiInterfaceConfig gs_api_interface::gs_config;

/****************************************************************
 * 関数宣言
 ****************************************************************/
static bool string_split(const std::string& commands, const char delimiter, std::vector<std::string>& command_list);

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

/****************************************************************
 * クラス定義
 ****************************************************************/
bool gs_api_interface::set_default_config(const GsApiInterfaceConfig& config)
{
    gs_config = config;
    return true;
}

bool gs_api_interface::get_default_config(GsApiInterfaceConfig& config)
{
    config = gs_config;
    return true;
}

bool gs_api_interface::send_command(const std::string& message, std::string& response)
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

    /* 確実に成功させるため少し待機させる */
    Sleep(WAIT_TO_READY_READ_MSEC);

    /* タイムアウト設定 */
    recv_tv.tv_sec = WAIT_TO_RECEIVE_MESSAGE_SEC;
    recv_tv.tv_usec = 0;
    result = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&recv_tv, sizeof(recv_tv));
    if (result < 0) {
        perror("[gsmodule]failed to set socket option.\n");
        return false;
    }

    /* メッセージを正しく受け取れているか確認する */
    while (1) {
        len = recv(sock, buffer, sizeof(buffer), 0);
        if (len < 0) {
            if (errno == EINTR) {
                continue;
            } else if (errno == EAGAIN) {
                perror("[gsmodule]failed to receive message due to timeout.");
            } else {
                perror("[gsmodule]failed to receive message.");
            }
            break;
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

bool gs_api_interface::is_connect()
{
    /* 無効なコマンドでもメッセージが送信できれば良い */
    std::string response;
    const std::string send_message = gs_config.delimiter;
    return send_command(send_message, response);
}

bool gs_api_interface::command_get_version(std::string& version)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_GET_VERSION << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    version = response;
    return result;
}

bool gs_api_interface::command_get_commands(std::vector<std::string>& commmand_list)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_GET_COMMANDS << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    string_split(response, MESSAGE_DELIMITER_COMMA, commmand_list);
    return result;
}

bool gs_api_interface::command_get_models(std::vector<std::string>& model_list)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_GET_MODELS << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    string_split(response, MESSAGE_DELIMITER_COMMA, model_list);
    return result;
}

bool gs_api_interface::command_select_model(const std::string& model_name)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_SELECT_MODEL
        << MESSAGE_DELIMITER_SPACE << model_name
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gs_api_interface::command_get_path(const std::string& path_name, std::string& path_value)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_GET_PATH
        << MESSAGE_DELIMITER_SPACE << path_name
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    path_value = response;
    return result;
}

bool gs_api_interface::command_get_samplerate(std::string& samplerate)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_GET_SAMPLERATE << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    samplerate = response;
    return result;
}

bool gs_api_interface::command_set_samplerate(const std::string& samplerate)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_SET_SAMPLERATE
        << MESSAGE_DELIMITER_SPACE << samplerate
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gs_api_interface::command_query_patchnames(const std::string& text, const bool name,
    const bool category, const bool tags, std::vector<std::string>& patch_list)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_QUERY_PATCHNAMES
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

bool gs_api_interface::command_query_patch(const std::string& patch_name)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_QUERY_PATCH
        << MESSAGE_DELIMITER_SPACE << patch_name
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gs_api_interface::command_query_categories(std::vector<std::string>& categoryt_list)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_QUERY_CATEGORIES << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    string_split(response, MESSAGE_DELIMITER_COMMA, categoryt_list);
    return result;
}

bool gs_api_interface::command_query_tags(std::vector<std::string>& tag_list)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_QUERY_TAGS << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    string_split(response, MESSAGE_DELIMITER_COMMA, tag_list);
    return result;
}

bool gs_api_interface::command_load_patch(const std::string& file_path)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_LOAD_PATCH
        << MESSAGE_DELIMITER_SPACE
        << file_path
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gs_api_interface::command_save_patch(const std::string& file_path)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_SAVE_PATCH
        << MESSAGE_DELIMITER_SPACE
        << file_path
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gs_api_interface::command_render_patch(const std::string& file_path, const unsigned int depth,
    const unsigned int channel, const unsigned int duration)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_RENDER_PATCH
        << MESSAGE_DELIMITER_SPACE << file_path
        << MESSAGE_DELIMITER_SPACE << std::to_string(depth)
        << MESSAGE_DELIMITER_SPACE << std::to_string(channel)
        << MESSAGE_DELIMITER_SPACE << std::to_string(duration)
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gs_api_interface::command_get_modelname(std::string& model_name)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_GET_MODELNAME << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    model_name = response;
    return result;
}

bool gs_api_interface::command_get_patchname(std::string& patch_name)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_GET_PATCHNAME << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    patch_name = response;
    return result;
}

bool gs_api_interface::command_get_variation(float& variation)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_GET_VARIATION << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    variation = static_cast<float>(std::stof(response));
    return result;
}

bool gs_api_interface::command_set_variation(const float& variation)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_SET_VARIATION
        << MESSAGE_DELIMITER_SPACE << variation
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gs_api_interface::command_get_drawing(const unsigned int index, std::vector<GsDrawingData>& drawing_data)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_GET_DRAWING
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

bool gs_api_interface::command_set_drawing(const std::vector<GsDrawingData>& drawing_data)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_SET_DRAWING
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

bool gs_api_interface::command_get_metacount(unsigned int& meta_count)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_GET_METACOUNT << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    meta_count = std::stoi(response);
    return result;
}

bool gs_api_interface::command_get_metanames(std::vector<std::string>& meta_names)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_GET_METANAMES << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    string_split(response, MESSAGE_DELIMITER_COMMA, meta_names);
    return result;
}

bool gs_api_interface::command_get_metaname(const unsigned int& index, std::string& metaname)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_GET_METANAME
        << MESSAGE_DELIMITER_SPACE << index
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    metaname = response;
    return result;
}

bool gs_api_interface::command_get_metavalue(const unsigned int& index, float& metavalue)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_GET_METAVALUE
        << MESSAGE_DELIMITER_SPACE << GS_METAVALUE_BY_INDEX
        << MESSAGE_DELIMITER_SPACE << index
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    metavalue = std::stof(response);
    return result;
}

bool gs_api_interface::command_get_metavalue(const std::string& name, float& metavalue)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_GET_METAVALUE
        << MESSAGE_DELIMITER_SPACE << GS_METAVALUE_BY_NAME
        << MESSAGE_DELIMITER_SPACE << name
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    metavalue = std::stof(response);
    return result;
}
bool gs_api_interface::command_set_metavalue(const unsigned int& index, const float& metavalue)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_SET_METAVALUE
        << MESSAGE_DELIMITER_SPACE << GS_METAVALUE_BY_INDEX
        << MESSAGE_DELIMITER_SPACE << index
        << MESSAGE_DELIMITER_SPACE << metavalue
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gs_api_interface::command_set_metavalue(const std::string& name, const float& metavalue)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_SET_METAVALUE
        << MESSAGE_DELIMITER_SPACE << GS_METAVALUE_BY_NAME
        << MESSAGE_DELIMITER_SPACE << name
        << MESSAGE_DELIMITER_SPACE << metavalue
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    return result;
}

bool gs_api_interface::command_get_curvescount(unsigned int& curves_count)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_GET_CURVESCOUNT << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    curves_count = std::stoi(response);
    return result;
}

bool gs_api_interface::command_get_curvenames(std::vector<std::string>& curve_names)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_GET_CURVENAMES << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    string_split(response, MESSAGE_DELIMITER_COMMA, curve_names);
    return result;
}

bool gs_api_interface::command_get_curvename(const unsigned int& curve_index, std::string& curve_name)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_GET_CURVENAME
        << MESSAGE_DELIMITER_SPACE << curve_index
        << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    curve_name = response;
    return result;
}

bool gs_api_interface::command_get_curvevalue(const unsigned int& curve_index, GsCurveValue& curve_value)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_GET_CURVEVALUE
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

bool gs_api_interface::command_get_curvevalue(const std::string& curve_name, GsCurveValue& curve_value)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_GET_CURVEVALUE
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

bool gs_api_interface::command_set_curvevalue(const unsigned int& curve_index, const GsCurveValue& curve_value)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_SET_CURVEVALUE
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

bool gs_api_interface::command_set_curvevalue(const std::string& curve_name, const GsCurveValue& curve_value)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_SET_CURVEVALUE
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
