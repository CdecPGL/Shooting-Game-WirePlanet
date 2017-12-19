#include"SystemVariable.h"

namespace SysVar{
	extern const char* GAME_FONT_FILE_NAME = "Jura-Medium.ttf";
	extern const char* GAME_FONT_NAME = "Jura";
	const char* VERSION_STRING("1.1.2");
	const double VERSION_NUMBER(1.12);
	const int DEBUG_LEVEL(0); //0:公開用 1:内部状態表示(define.h内のDEBUG_DRAWが定義されている必要あり) 2:デバッグ用機能 3:デバッグ補助操作(チート的な)
}