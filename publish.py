'''ゲームの公開設定を行う'''

import atexit
import os
import shutil
import subprocess
import sys
import zipfile

TEMP_DIR = "./temp"
SOURCE_RESOURCE_DIRECTORY = "./WirePlanet/resources"
SOURCE_EXECUTABLE_DIRECTORY = "./Release"
SOURCE_TEMPLATE_DIRECTORY = "./publish_sources"
SOURCE_MANUAL_DIRECTORY = "./manual"
DISTRIBUTION_RESOURCE_DIRECTORY = "resources"
DISTRIBUTION_MANUAL_DIRECTORY = "manual"
README_FILE_NAME = "readme.txt"
INI_FILE_NAME = "wireplanet.ini"
ARCHIVER_NAME = "EasyArchiver.exe"
ARCHIVED_FILE_NAME = "data.arc"
PUBLISH_FILE_NAME_BASE = "WirePlanet"
EXECUTABLE_NAME = "WirePlanet.exe"


def list_dir_recursively(target_directory):
    '''ディレクトリの中身を再帰的に探索しリストで出力する'''
    out_list = []
    for root, dirs, files in os.walk(target_directory):
        for directory in dirs:
            out_list.append(os.path.relpath(
                os.path.join(root, directory), target_directory))
        for file in files:
            out_list.append(os.path.relpath(
                os.path.join(root, file), target_directory))
    return out_list


def delete_temp_dir():
    '''一時ディレクトリを削除する'''
    if os.path.exists(TEMP_DIR):
        shutil.rmtree(TEMP_DIR)


def main():
    # コマンドライン引数の読み込み
    if len(sys.argv) > 1:
        version_string = sys.argv[1]
    else:
        sys.stderr.write(
            "コマンドライン引数が足りません。バージョン文字列を指定する必要があります。\n")
        exit(1)

    # 一時ディレクトリの作成
    if os.path.exists(TEMP_DIR):
        shutil.rmtree(TEMP_DIR)
    os.mkdir(TEMP_DIR)
    atexit.register(delete_temp_dir)

    # リソースアーカイブを作成
    archiver_path = os.path.abspath(os.path.join(
        SOURCE_EXECUTABLE_DIRECTORY, ARCHIVER_NAME))
    archive_source_directory = os.path.abspath(
        os.path.join(SOURCE_RESOURCE_DIRECTORY, "data"))
    archived_file_path = os.path.join(TEMP_DIR, ARCHIVED_FILE_NAME)
    print(archiver_path)
    if subprocess.call([archiver_path, "-O" + ARCHIVED_FILE_NAME, "-M2", "-C6", "-K48209", archive_source_directory], cwd=TEMP_DIR, shell=True) != 0:
        sys.stderr.write("リソースアーカイブの作成に失敗しました。\n")
        exit(1)

    # 配布用ファイル作成
    publish_file_name_body = PUBLISH_FILE_NAME_BASE + "_" + version_string
    publish_file_name = publish_file_name_body + ".zip"
    try:
        with zipfile.ZipFile('./' + publish_file_name, 'w', zipfile.ZIP_DEFLATED) as publish_file:
            # Readme
            publish_file.write(os.path.join(
                SOURCE_TEMPLATE_DIRECTORY, README_FILE_NAME), os.path.join(publish_file_name_body, README_FILE_NAME))
            # 設定ファイル
            publish_file.write(os.path.join(
                SOURCE_TEMPLATE_DIRECTORY, INI_FILE_NAME), os.path.join(publish_file_name_body, INI_FILE_NAME))
            # リソースファイル
            publish_file.write(archived_file_path, os.path.join(
                publish_file_name_body, DISTRIBUTION_RESOURCE_DIRECTORY, ARCHIVED_FILE_NAME))
            # 実行ファイル
            publish_file.write(os.path.join(
                SOURCE_EXECUTABLE_DIRECTORY, EXECUTABLE_NAME), os.path.join(publish_file_name_body, EXECUTABLE_NAME))
            # マニュアル
            manual_files = list_dir_recursively(SOURCE_MANUAL_DIRECTORY)
            for manual_file in manual_files:
                if os.path.isfile(os.path.join(SOURCE_MANUAL_DIRECTORY, manual_file)):
                    publish_file.write(os.path.join(
                        SOURCE_MANUAL_DIRECTORY, manual_file), os.path.join(publish_file_name_body, DISTRIBUTION_MANUAL_DIRECTORY, manual_file))
    except FileNotFoundError:
        sys.stderr.write("ファイルが見つかりませんでした。({})\n".format(sys.exc_info()[1]))
        exit(1)

    print("公開用ファイルが{}に出力されました。".format(publish_file_name))


if __name__ == "__main__":
    main()
