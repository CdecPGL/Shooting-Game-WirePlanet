#pragma once

#include"AnimationData.h"
#include<vector>
#include<unordered_map>

using namespace std;

class DataScript;

namespace AnimLib{

	class AnimationDataAdmin
	{
	public:
		using RefGHandleType = std::unordered_map < std::string, int > ;
		struct LoadSettings{
			bool ref_ghandles_flag = false;
			const RefGHandleType* ref_ghandles = nullptr;
			bool reload_flag = false;
		};

		~AnimationDataAdmin();
		AnimationDataAdmin(const AnimationDataAdmin&) = delete;
		const AnimationDataAdmin& operator=(const AnimationDataAdmin&) = delete;
		static AnimationDataAdmin& GetInstace();
		int Init(); //初期化
		shared_ptr<AnimationData> GetAnimationDataPointer(const std::string&,bool=false); //アニメファイル名からポインタを取得(アニメデータ名、自動読み込みフラグ)(まだ読み込まれていなかったらフラグがTの場合は自動で読み込み。なしで無効なポインタを返す。)(拡張子を省略で.ganimを自動追加)
		shared_ptr<AnimationData> ReloadAnimationData(const std::string&); //指定のアニメーションデータを再読み込み(新しいアニメーションデータを返す。失敗したら英ストから削除し,nullptrを返す)
		shared_ptr<AnimationData> LoadAnimationDataFromBinary(const std::string&,const void*,unsigned int,bool = false); //バイナリデータからからアニメーションデータ読み込み。(データ名、データの先頭ポインタ、データサイズ、存在時の再読み込みフラグ=false)(失敗でnullptr)(V2,3のみ対応)
		shared_ptr<AnimationData> LoadAnimationDataFromDataScript(const std::string&,const DataScript&,bool=false); //データスクリプトからアニメーションデータ読み込み(データ名、データスクリプト、存在時の再読み込みフラグ=false)
		void SetGHandles(RefGHandleType&&); //画像ハンドルセット
		void SetGHandles(const RefGHandleType&); //画像ハンドルセット
		void Clean(); //使われてないデータを掃除
		void Clear(); //全消去
		enum class Version{V1,V2_};
		void SetVersion(Version);

		static std::shared_ptr<AnimationData> LoadAnimationDataFromDataScriptWithoutResist(const DataScript&, const LoadSettings&);
	private:
		AnimationDataAdmin();
		Version _version = Version::V2_;

		unordered_map < string, shared_ptr<AnimationData>> animationdata; //アニメーションデータ(ファイル名、ポインタ)

		RefGHandleType _ghandles; //画像ハンドル
		bool _ref_ghandles_flag; //読み込み時に画像ハンドルを参照するかのフラグ

		int LoadAnimationData(const std::string&,std::shared_ptr<AnimationData>); //読み込み仕分け。_versionによってV1以前とV2以降を仕分ける。
		int LoadAnimationData(const std::string&, const void*, size_t, std::shared_ptr<AnimationData>); //読み込み仕分け。_versionによってV1以前とV2以降を仕分ける。

		static int SetAnimationDataFromAnimFileV1(std::shared_ptr<AnimationData>, const std::string&,const RefGHandleType*); //V1読み込み本体
		static int SetAnimationDataFromDataScript(std::shared_ptr<AnimationData>, const DataScript&, const RefGHandleType*); //V2以降仕分け

		//SetAnimationDataFromDataScript
		static int SetAnimationDataFromDataScript_V2(std::shared_ptr<AnimationData>, const DataScript&, const RefGHandleType*);

		static int SetAnimationDataFromDataScript_V3(std::shared_ptr<AnimationData>, const DataScript&, const RefGHandleType*);
		static int SetAnimationDataFromDataScript_V4(std::shared_ptr<AnimationData>, const DataScript&, const RefGHandleType*);
		static int SetAnimationDataFromDataBlock_V3(std::shared_ptr<AnimationData>, std::shared_ptr<const DataBlock>, const RefGHandleType*);
	};
};
