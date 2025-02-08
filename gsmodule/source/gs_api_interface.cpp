/****************************************************************
 * @file    gs_api_interface.cpp
 * @brief   GameSynth Tool APIを呼び出す
 * @version 1.0.0
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

/****************************************************************
 * プリプロセッサ定義
 ****************************************************************/
/* ツールとの通信時に待機する時間 */
#define WAIT_TO_READY_READ_MSEC         (20)
#define WAIT_TO_RECEIVE_MESSAGE_SEC     (1)

/* ツールから受信するメッセージサイズ */
#define MAX_RECEIVE_MESSAGE_SIZE        (4096)

/* ツールから受信するメッセージに含まれるデリミタ */
#define MESSAGE_DELIMITER_SPACE         ' '
#define MESSAGE_DELIMITER               ','

/****************************************************************
 * 構造体宣言
 ****************************************************************/

/****************************************************************
 * 変数宣言
 ****************************************************************/

/****************************************************************
 * 変数定義
 ****************************************************************/
GsApiInterfaceConfig gs_api_interface::gs_config =
{
    GS_API_INTERFACE_DEFAULT_PORT_NUMBER,
    GS_API_INTERFACE_DEFAULT_IP_ADDRESS,
    GS_API_INTERFACE_DEFAULT_CODEC,
    GS_API_INTERFACE_DEFAULT_DELIMITER,
};

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
    string_split(response, MESSAGE_DELIMITER, commmand_list);
    return result;
}

bool gs_api_interface::command_get_models(std::vector<std::string>& model_list)
{
    std::string response;
    std::ostringstream oss;
    oss << GS_API_GET_MODELS << gs_config.delimiter;
    const std::string send_message = oss.str();
    bool result = send_command(send_message, response);
    string_split(response, MESSAGE_DELIMITER, model_list);
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
