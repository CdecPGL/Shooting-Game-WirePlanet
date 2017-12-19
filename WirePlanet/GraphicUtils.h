#pragma once

#include"DxLib.h"
#include<string>

namespace GraphicUtils
{
	//文字列から画像を生成(文字列,色,フォントハンドル,縁の色)(3Dカメラの設定は保持されない)
	int CreateGraphFromStringToHandle(const std::string&, int, int, int = 0);
};
