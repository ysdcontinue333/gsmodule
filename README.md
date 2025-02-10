# gsmodule

## 概要

- gsmoduleは、Tsugi®『GameSynth』のリモート制御機能を提供するC++で実装されたオープンソースのライブラリです。

## 使用例

- GameSynthを起動する。
  - GameSynthが起動していれば、TCP通信でGameSynth Tool APIを利用できる。
- GameSynthとの接続設定を確認する。
  - 現在の通信設定を`get_default_config()関数`で受け取ります。
  - 通信設定を変えるときは`set_default_config()関数`で設定します。
- GameSynthの機能が利用できることを確認する。
  - たとえば、`command_get_version()関数`を実行する。
  - GameSynthのツールバージョンが取得できれば成功です。

## 依存ライブラリ

- gsmoduleは、パッチファイルの解析に[tinyxml2](https://github.com/leethomason/tinyxml2)を利用します。
- testsは、ライブラリのテストコードに[googletest](https://github.com/google/googletest)を利用します。
