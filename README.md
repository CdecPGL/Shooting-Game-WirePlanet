# Shooting-Game-WirePlanet

自作シューティングゲーム「WirePlanet」のリポジトリ。

## 概要

2014年11月9日に公開した自作ゲーム「WirePlanet」のリポジトリです。

C++を触りはじめてあまり経っていない時に作成したもので良いコードとは言えないので、ご了承ください。

## 動作条件

### 開発環境

- VisualStudio2017:  
  ソリューションはVisualStudio2017をターゲットとしていますが、ソースコード自体はVisualStudio2013以降でコンパイル可能だと思われます
- Python3:  
  プロジェクトのセットアップや公開用ファイル作成に用います

### ライブラリ

- BoostLibrary1.56以上: 手動での導入が必要です
- (Lua: すでにリポジトリに含まれています)
- (luabind: すでにリポジトリに含まれています)
- (zlib: すでにリポジトリに含まれています)
- (DXライブラリ: setup_project.pyにより自動でダウンロードされます)

## 各ディレクトリについて

### WirePlanet

WirePlanetのプロジェクトディレクトリ

### ArchiveLibrary

リソースのアーカイブ化を行うためのライブラリ用プロジェクトディレクトリ

### EasyArchiver

リソースのアーカイブ化を行うコマンドラインソフト用プロジェクトライブラリ

### libraries

WirePlanetが使用している外部ライブラリが入ったディレクトリ

### manual

WirePlanetのマニュアルが入ったディレクトリ

### publish_sources

WirePlanetの公開ファイル作成時に使用される、ゲームのReadmeや設定ファイルが入ったディレクトリ

## プロジェクトの初期設定

### Pythonの導入

[Anaconda](https://www.continuum.io/downloads)などを用いて、Pythonをインストールしてください。

### BoostLibraryのインストール

以下のいずれかの方法でBoostLibraryをインストールし設定してください。

#### Vcpkgを用いて導入する

マイクロソフトが公開しているC++用のパッケージ管理ツールVcpkgを[GitHubリポジトリ](https://github.com/Microsoft/vcpkg)からクローンし、READMEに従ってビルドします。
Vcpkgが導入できたら、x86-windows-staticをターゲットとしてBoostLibraryの導入をインストールし、VisualStudioでプロジェクトの設定を行ってください。

#### 自分でビルドする

BoostLibraryを[公式ページ](http://www.boost.org/)からダウンロードし、ビルドしてください。

ビルド後は、プロジェクトの追加のインクルードディレクトリとライブラリディレクトリをVisualStudioで設定する必要があります。

### その他依存ライブラリなどのダウンロード

以下のコマンドを実行してください。

```:bat
python setup_project.py
```

## ゲームの公開用ファイル作成

まずはVisualStudioでソリューションのReleaseビルドを行います。

次に以下のコマンドを実行します。
これにより公開用ファイルのコピーと圧縮が行われ、ゲームの公開用ファイル一式が含まれるWirePlanet_[バージョン文字列].zipが生成されます。

```:bat
python publish.py [任意のバージョン文字列]
```

## 外部のフリー素材について

ゲーム内の音楽、効果音、フォントなどに外部のフリー素材を利用しています。
これらの素材は著作権に配慮して除外してあるので、単純にリポジトリをクローンしてゲームを再生しても一部の音はなりません。ゲーム内で使用したフリー素材を手動でダウンロードする場合は、以下のリストを参照してください。

[使用したフリー素材リスト](free_material_list.md)

## ライセンス

librariesディレクトリ以外のソースコードにはMITライセンスが適用されます。

librariesディレクトリ内のソースコードは、それぞれのライブラリのライセンスが適用されます。

- Lua: MITライセンス
- Luabind: MITライセンス
- zlib: ZLIBライセンス

## 作者

Cdec

- [GitHub](https://github.com/CdecPGL)
- [ホームページ](http://planetagamelabo.com/homepage)
