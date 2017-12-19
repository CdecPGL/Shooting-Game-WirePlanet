'''依存ライブラリなどの設定を行う'''

import os
import shutil
import subprocess
import urllib.request

TEMP_DIR = "./temp"
DXLIB_DOWNLOAD_URL = "http://dxlib.o.oo7.jp/DxLib/DxLibNoneModel_VC_3_19.exe"
DXLIB_TEMP_FILE_NAME = "dxlib_temp.exe"


def main():
    # 一時ディレクトリの作成
    if os.path.exists(TEMP_DIR):
        shutil.rmtree(TEMP_DIR)
    os.mkdir(TEMP_DIR)

    # DXライブラリのダウンロード
    dxlib_temp_file_path = os.path.join(TEMP_DIR, DXLIB_TEMP_FILE_NAME)
    print("DXライブラリをダウンロードしています……")
    urllib.request.urlretrieve(DXLIB_DOWNLOAD_URL, dxlib_temp_file_path)
    print("DXライブラリを解凍しています……")
    subprocess.call("./" + dxlib_temp_file_path)
    shutil.move("./DxLibNoneModel_VC", "./libraries")
    print("DXライブラリの設定が完了しました。")

    # 一時ディレクトリの削除
    shutil.rmtree(TEMP_DIR)

    print("初期設定が完了しました。BoostLibraryをインストールしていない場合はREADMEを参考に手動で行ってください。")


if __name__ == "__main__":
    main()
