#include "AnimationDataAdmin.h"
#include"DataScript.h"
#include"boost/lexical_cast.hpp"
#include<fstream>
//#include"lua.hpp"
#include"AnimationData.h"
#include"DSStandardDataStructure.h"

namespace{
	const char* DSSDS_ID("ganim");
}

namespace AnimLib{

//	const int Ghandle_Buf_Size(1000); //画像読み込み時の画像ハンドルバッファサイズ
	const string AnimFile_Identification_String("AnimationFile_Cdec"); //アニメファイルV2以降識別文字列

	AnimationDataAdmin::AnimationDataAdmin():_ref_ghandles_flag(false){
	}


	AnimationDataAdmin::~AnimationDataAdmin(){
	}

	AnimationDataAdmin& AnimationDataAdmin::GetInstace(){
		static AnimationDataAdmin ada;
		return ada;
	}

	//初期化
	int AnimationDataAdmin::Init(){
		Clear();
		_ghandles.clear();
		_ref_ghandles_flag = false;
		return 0;
	}

	void AnimationDataAdmin::SetGHandles(unordered_map<string, int>&& gh){
		_ghandles = move(gh);
		_ref_ghandles_flag = true;
	}
	void AnimationDataAdmin::SetGHandles(const unordered_map<string, int>& gh){
		_ghandles = gh;
		_ref_ghandles_flag = true;
	}

	//ファイル名でポインタを取得
	shared_ptr<AnimationData> AnimationDataAdmin::GetAnimationDataPointer(const string& m_afn,bool auto_load){
		string afn(m_afn);
		unordered_map<string, shared_ptr<AnimationData>>::iterator it = animationdata.find(afn);
		//無かったらロード
		if (it == animationdata.end()){
			if (auto_load){
				shared_ptr<AnimationData> ad = make_shared<AnimationData>();
				int result = LoadAnimationData(afn, ad);
				if (result >= 0){
					animationdata.insert(unordered_map<string, shared_ptr<AnimationData>>::value_type(afn, ad)); //登録
				}
				return ad;
			}
			else{ return nullptr; }
		}
		else{
			return (*it).second;
		}
	}

	//指定されたアニメーションデータを再読み込み
	shared_ptr<AnimationData> AnimationDataAdmin::ReloadAnimationData(const string& m_afn){
		string afn(m_afn);
		auto it = animationdata.find(afn);
		if (it == animationdata.end()){ return nullptr; }
		int ret = LoadAnimationData(afn,it->second);
		if (ret>=0){
			return it->second;
		}
		else{
			animationdata.erase(it);
			return nullptr;
		}
	}

	//掃除
	void AnimationDataAdmin::Clean(){
		for (unordered_map<string, shared_ptr<AnimationData>>::iterator it = animationdata.begin(); it != animationdata.end(); ++it){
			if ((*it).second.use_count() == 1){
				animationdata.erase(it);
			}
		}
	}

	//消去
	void AnimationDataAdmin::Clear(){
		animationdata.clear();
	}

	//アニメーションデータをファイルから読み込み(過去に読み込んだものだったら読み込み直し)
	int AnimationDataAdmin::LoadAnimationData(const std::string& filename,shared_ptr<AnimationData> ad){
		int result = 0;
		//バージョン確認
		switch (_version)
		{
		case AnimLib::AnimationDataAdmin::Version::V1:
			result = SetAnimationDataFromAnimFileV1(ad, filename, _ref_ghandles_flag ? &_ghandles : nullptr);
			break;
		case AnimLib::AnimationDataAdmin::Version::V2_:
		{
			DataScript ds;
			ds.SetVersion(DataScriptVersion::DSVer1_5);
			ds.LoadDataFromScript(filename);
			result = SetAnimationDataFromDataScript(ad, ds, _ref_ghandles_flag ? &_ghandles : nullptr);
		}
			break;
		default:
			result = -1;
			break;
		}
		if (result){
			printf("アニメーションファイルの読み込みでエラーが発生しました。ファイル:%s,エラー番号:%d\n", filename.c_str(), result);
			printfDx("アニメーションファイルの読み込みでエラーが発生しました。ファイル:%s,エラー番号:%d\n", filename.c_str(), result);
			return result;
		}
		else{
			return 0;
		}
	}

	 int AnimationDataAdmin::LoadAnimationData(const string& name,const void* t_ptr,size_t s,shared_ptr<AnimationData> ad){
		//バージョン確認
		 int result = 0;
		switch (_version)
		{
		case AnimLib::AnimationDataAdmin::Version::V1:
			result = -2;
			break;
		case AnimLib::AnimationDataAdmin::Version::V2_:
		{
			DataScript ds;
			ds.SetVersion(DataScriptVersion::DSVer1_5);
			ds.LoadDataFromBynary(t_ptr,s,false);
			result = SetAnimationDataFromDataScript(ad, ds, _ref_ghandles_flag ? &_ghandles : nullptr);
		}
		break;
		default:
			result = -1;
			break;
		}
		if (result){
			printf("アニメーションファイルの読み込みでエラーが発生しました。ファイル:%s,エラー番号:%d\n", name.c_str(), result);
			printfDx("アニメーションファイルの読み込みでエラーが発生しました。ファイル:%s,エラー番号:%d\n", name.c_str(), result);
			return result;
		}
		else{
			return 0;
		}
	}

	 int AnimationDataAdmin::SetAnimationDataFromAnimFileV1(std::shared_ptr<AnimationData> ad, const std::string& filename, const RefGHandleType* ref_ghandles){
		 throw logic_error("AnimationLibraryV1は利用できません。");
		 return -1;
		//lua_State *lua = luaL_newstate(); //luaステートの作成

		//// Luaファイルを開いて読み込み
		//if (luaL_dofile(lua, filename.c_str())) {
		//	printfDx("%s\n", lua_tostring(lua, lua_gettop(lua)));
		//	lua_close(lua);
		//	return -101;
		//}

		//// Luaステートに読み込まれたグローバル変数をスタックに積んで内容を取得
		////sizeに取得
		//lua_getglobal(lua, "size");
		//int ad_size = lua_tointeger(lua, 1);
		//lua_pop(lua, 1);
		////AnimationDataUnitの数を取得
		//lua_getglobal(lua, "ANIMATION_DATA_UNIT_NUM");
		//const int ad_ADU_NUM = lua_tointeger(lua, 1);
		//lua_pop(lua, 1);
		//ad->num = ad_ADU_NUM; //AnimationDataにAnimationDataUnitの数を記録
		////AnimationDataUnitのデータを取得
		//char string[32]; //32文字まで
		//for (int i = 0; i < ad_ADU_NUM; ++i){
		//	sprintf_s(string, "adu%d_tag", i);
		//	lua_getglobal(lua, string);
		//	sprintf_s(string, "adu%d_graphic_filename", i);
		//	lua_getglobal(lua, string);
		//	sprintf_s(string, "adu%d_interval", i);
		//	lua_getglobal(lua, string);
		//	sprintf_s(string, "adu%d_repeat_flag", i);
		//	lua_getglobal(lua, string);
		//	std::string adu_tag = lua_tostring(lua, 1);
		//	std::string adu_gf = lua_tostring(lua, 2);
		//	int adu_interval = lua_tointeger(lua, 3);
		//	bool adu_rf = lua_toboolean(lua, 4) ? true : false;
		//	lua_pop(lua, lua_gettop(lua));

		//	//AnimationDataUnitをインスタンス化
		//	//画像読み込み
		//	int gh = -1;
		//	if (ref_ghandles){ //画像ハンドル参照フラグがonだったら画像ハンドルを参照する
		//		auto it = ref_ghandles->find(adu_gf);
		//		if (it != ref_ghandles->end()){ gh = (*it).second; }
		//		else{ gh = LoadGraph(adu_gf.c_str()); }
		//	}
		//	else{
		//		gh = LoadGraph(adu_gf.c_str());
		//	}
		//	//画像が読み込まれていなかったら設定中止
		//	if (gh < 0){
		//		continue;
		//	}
		//	ad->_ghandles.push_back(gh);
		//	unique_ptr<AnimationDataUnit> adu(make_unique<AnimationDataUnit>());
		//	adu->interval = adu_interval;
		//	adu->repeat_flag = adu_rf;
		//	adu->tag = adu_tag;
		//	adu->size_x = ad_size;
		//	adu->size_y = ad_size;
		//	int g_w, g_h;
		//	GetGraphSize(gh, &g_w, &g_h);
		//	int bh(g_w / ad_size), bw(g_h / ad_size);
		//	adu->num = bh*bw;
		//	for (int i = 0; i < adu->num; ++i){
		//		AnimationDataUnit::FrameInfo fi;
		//		fi.pos_x = (i % bw)*ad_size;
		//		fi.pos_y = (i / bh)*ad_size;
		//		fi.graph_num = (int)ad->_ghandles.size() - 1;
		//		adu->frame_info.push_back(fi);
		//	}
		//	//AnimationDataUnitをAnimationDataに登録
		//	ad->animationdataunit.push_back(move(adu));
		//}
		//ad->SetGhandlesToAnimDataUnit();

		////PrintStack(lua);

		//// スタックを削除
		//int num = lua_gettop(lua);
		//lua_pop(lua, num);

		//lua_close(lua); //luaステートの削除
		//return 0;
	}
	 int AnimationDataAdmin::SetAnimationDataFromDataScript(std::shared_ptr<AnimationData> ad, const DataScript& ds, const RefGHandleType* ref_ghandles){
		//バージョン確認
		try{
			if (DSsds::IsDSSDS(ds)){
				return SetAnimationDataFromDataScript_V4(ad, ds, ref_ghandles);
			}
			else{
				vector<vector<string>> s;
				if (ds.GetDataAll("version", s)){
					return -201;
				}
				if (s.size() == 0){ return -202; }
				if (s[0].size() == 0){ return -203; }
				double version = boost::lexical_cast<double>(s[0][0]);
				if (version < 2.0){
					return -204;
				}
				else if (2.0 <= version && version < 3.0){
					return SetAnimationDataFromDataScript_V2(ad, ds, ref_ghandles);
				}
				else if (3.0 <= version && version < 4.0){
					return SetAnimationDataFromDataScript_V3(ad, ds, ref_ghandles);
				}
				else{
					return -205;
				}
			}
		}
		catch (out_of_range&){
			return -206;
		}
		catch (boost::bad_lexical_cast&){
			return -207;
		}
		return 0;
	}

	struct anim_def{
		int interval = 1;
		bool repeat = false;
		size_t width = 0;
		size_t height = 0;
		struct frame_data{
			frame_data(int mx, int my, int mgn) :x(mx), y(my), graph_num(mgn){}
			int x = 0;
			int y = 0;
			int graph_num = 0;
		};
		vector<frame_data> f_data;
		bool pixel_coordinates_flag = false; //座標をピクセル単位にするか
		int automatic_generation_g_num = -1; //フレーム自動生成画像番号。-1で無効
	};
	int AnimationDataAdmin::SetAnimationDataFromDataScript_V2(shared_ptr<AnimationData> ad, const DataScript& ds, const RefGHandleType* ref_ghandles){
		int aw(0), ah(0); //サイズ
		vector<string> graph_pass; //画像パスリスト
		unordered_map<string, anim_def> anims;

		//データブロック一覧取得
		vector<string> dbs(ds.GetDataName());
		{
			auto it = find(dbs.begin(), dbs.end(), "version");
			if (it != dbs.end()){ dbs.erase(it); }
		}
		
		//サイズ取得
		try{
			auto it = find(dbs.begin(), dbs.end(), "size");
			if (it == dbs.end()){ return -306; }
			else{ dbs.erase(it); }
			vector<vector<string>> s;
			ds.GetDataAll("size", s);
			if (s.size() == 0){ return -307; }
			if (s[0].size() != 2){ return -308; }
			aw = boost::lexical_cast<int>(s[0][0]);
			ah = boost::lexical_cast<int>(s[0][1]);
		}
		catch (boost::bad_lexical_cast&){
			return -309;
		}
		//画像リスト取得
		{
			auto it = find(dbs.begin(), dbs.end(), "graph_pass");
			if (it == dbs.end()){ return -310; }
			else{ dbs.erase(it); }
			vector<vector<string>> s;
			ds.GetDataAll("graph_pass", s);
			//一行に複数あったら初めのものだけ読み込んであとは無視
			if ((int)s.size() == 0){ return -311; }
			for (int i = 0; i < (int)s.size(); ++i){
				//一つでも空行があったらエラー
				if (s[i].size() == 0){ return -312; }
				graph_pass.push_back(s[i][0]);
			}
		}
		//アニメーション定義読み込み
		for (string& db : dbs){
			vector<vector<string>> d;
			ds.GetDataAll(db, d);
			anims.insert(unordered_map<string, anim_def>::value_type(db, anim_def()));
			auto newanim_it = anims.find(db);
			for (auto it = d.begin(); it != d.end(); ++it){
				try{
					if (it->size() == 0){ continue; }
					if ((*it)[0] == "interval"){
						if ((*it).size() < 2){ continue; }
						newanim_it->second.interval = boost::lexical_cast<int>((*it)[1]);
					}
					else if ((*it)[0] == "repeat"){
						if ((*it).size() < 2){ continue; }
						newanim_it->second.repeat = boost::lexical_cast<bool>((*it)[1]);
					}
					else if ((*it)[0] == "FrameDefS"){
						for (auto it2 = it + 1; it2 != d.end(); ++it2){
							if ((*it2)[0] == "FrameDefE"){
								it = it2;
								break;
							}
							int x(0), y(0), gn(0);
							//数が合わない、変換に失敗した時は0,0,0のフレームを作成
							try{
								if ((*it2).size() >= 2){ x = boost::lexical_cast<int>((*it2)[0]); y = boost::lexical_cast<int>((*it2)[1]); }
								if ((*it2).size() == 3){ gn = boost::lexical_cast<int>((*it2)[2]); }
							}
							catch (boost::bad_lexical_cast&){
							}
							newanim_it->second.f_data.push_back(anim_def::frame_data(x, y, gn));
						}
					}
				}
				catch (boost::bad_lexical_cast&){
					continue;
				}
			}
		}

		//画像を読み込み
		vector<int> graphes;
		for (string& gp : graph_pass){
			//画像読み込み
			int gh = -1;
			if (ref_ghandles){ //画像ハンドル参照フラグがonだったら画像ハンドルを参照する
				auto it = ref_ghandles->find(gp);
				if (it != ref_ghandles->end()){ gh = (*it).second; }
				else{ gh = LoadGraph(gp.c_str()); }
			}
			else{
				gh = LoadGraph(gp.c_str());
			}
			//画像が読み込まれていなかったら設定中止
			if (gh < 0){
				continue;
			}
//			int pw, ph; //読み込んだ画像のピクセル幅、高さ
//			GetGraphSize(gh, &pw, &ph);
//			DeleteGraph(gh);
//			int bw(pw / aw + (pw%aw == 0 ? 0 : 1)), bh(ph / ah + (ph%ah == 0 ? 0 : 1)); //読み込んだ画像のブロック幅、高さを求める
			graphes.push_back(gh);
		}

		//定義からデータをセット
		ad->animationdataunit.clear();
		ad->_ghandles = std::move(graphes);
		for (unordered_map<string, anim_def>::value_type& anim : anims){
			ad->animationdataunit.push_back(make_unique<AnimationDataUnit>());
			AnimationDataUnit* adu = ad->animationdataunit[ad->animationdataunit.size() - 1].get();
			adu->tag = anim.first;
			adu->interval = anim.second.interval;
			adu->repeat_flag = anim.second.repeat;
			adu->size_x = aw;
			adu->size_y = ah;
			adu->num = anim.second.f_data.size();
			for (anim_def::frame_data& gp : anim.second.f_data){
				AnimationDataUnit::FrameInfo fi;
				if (gp.graph_num >= (int)graph_pass.size()){
					//エラー画像には-1を入れる
					fi.graph_num = -1;
				}
				else{
					fi.graph_num = gp.graph_num;
				}
				if (anim.second.pixel_coordinates_flag){
					fi.pos_x = gp.x;
					fi.pos_y = gp.y;
				}
				else{
					fi.pos_x = gp.x*aw;
					fi.pos_y = gp.y*ah;
				}
				adu->frame_info.push_back(fi);
			}
		}
		ad->num = anims.size();
		ad->SetGhandlesToAnimDataUnit();
		return 0;
	}

	int AnimationDataAdmin::SetAnimationDataFromDataScript_V3(shared_ptr<AnimationData> ad, const DataScript& ds, const RefGHandleType* ref_ghandles){
		return SetAnimationDataFromDataBlock_V3(ad, ds.GetRoot(), ref_ghandles);
	}

	std::shared_ptr<AnimationData> AnimationDataAdmin::LoadAnimationDataFromBinary(const std::string& name,const void* ptr, unsigned int size,bool reload_flag){
		auto adit = animationdata.find(name);
		std::shared_ptr<AnimationData> ad;
		if (adit != animationdata.end()){
			if (reload_flag){
				ad = adit->second;
			}
			else{
				return adit->second;
			}
		}
		else{
			ad = make_shared<AnimationData>(); 
		}
		if (LoadAnimationData(name, ptr, size, ad) < 0){
			return nullptr;
		}		
		animationdata.insert(make_pair(name, ad));
		return ad;
	}

	shared_ptr<AnimationData> AnimationDataAdmin::LoadAnimationDataFromDataScript(const string& adn,const DataScript& ds,bool reload_flag){
		auto adit = animationdata.find(adn);
		std::shared_ptr<AnimationData> ad;
		if (adit != animationdata.end()){
			if (reload_flag){
				ad = adit->second;
			}
			else{
				return adit->second;
			}
		}
		else{
			ad = make_shared<AnimationData>();
		}
		if (SetAnimationDataFromDataScript(ad, ds, _ref_ghandles_flag ? &_ghandles : nullptr)){
			return nullptr;
		}
		
		animationdata.insert(make_pair(adn, ad));
		return ad;
	}

	shared_ptr<AnimationData> AnimationDataAdmin::LoadAnimationDataFromDataScriptWithoutResist(const DataScript& ds, const LoadSettings& ls){
		shared_ptr<AnimationData> ad = make_shared<AnimationData>();
		if (SetAnimationDataFromDataScript(ad, ds, ls.ref_ghandles_flag ? ls.ref_ghandles : nullptr)){
			return nullptr;
		}
		return ad;
	}

	int AnimationDataAdmin::SetAnimationDataFromDataScript_V4(std::shared_ptr<AnimationData> ad, const DataScript& ds, const RefGHandleType* ref_ghandles)
	{
		auto db = DSsds::GetDataBlockByDataID(ds, DSSDS_ID);
		if (db == nullptr){ return -500; }
		//バージョン確認
		auto vdb = db->FindChild("version");
		if (vdb == nullptr){ return -501; }
		try{
			if (boost::lexical_cast<double>(vdb->GetData(0, 0))<4.0){
				return -503;
			}
		}
		catch (std::out_of_range&){
			return -502;
		}
		catch (boost::bad_lexical_cast&){
			return -504;
		}
		return SetAnimationDataFromDataBlock_V3(ad, db, ref_ghandles);
	}

	int AnimationDataAdmin::SetAnimationDataFromDataBlock_V3(std::shared_ptr<AnimationData> ad, std::shared_ptr<const DataBlock> db, const RefGHandleType* ref_ghandles)
	{
		int default_w(0), default_h(0); //デフォルトサイズ
		const DataBlock& root_db = *db;
		std::unordered_map<int,std::string> graph_pass; //画像パスリスト
		unordered_map<string, anim_def> anims;

		//デフォルトサイズ取得
		try{
			const DataBlock& size_db = root_db.FindDataBlock("size");
			const vector<vector<string>>& s = size_db.GetAllData();
			if (s.size() == 0){ return -407; }
			if (s[0].size() != 2){ return -408; }
			default_w = boost::lexical_cast<int>(s[0][0]);
			default_h = boost::lexical_cast<int>(s[0][1]);
		}
		catch (out_of_range&){
			return -406;
		}
		catch (boost::bad_lexical_cast&){
			return -209;
		}
		//画像リスト取得
		try{
			bool num_specify = false; //番号任意指定フラグ
			const DataBlock& gp_fb = *root_db.FindChild("graph_pass");
			const vector<vector<string>>& s = gp_fb.GetAllData();
			//一行に複数あったら初めのものだけ読み込んであとは無視
			if ((int)s.size() == 0){ return -411; }
			else if (s[0].size() == 1){ num_specify = false; } //最初の定義のサイズが1だったら番号指定なし
			else{ num_specify = true; } //2以上だったら番号指定あり
			for (int i = 0; i < (int)s.size(); ++i){
				//一つでも無効行があったらエラー
				if (num_specify){
					if (s[i].size() != 2){ return -413; }
					graph_pass.insert(make_pair(boost::lexical_cast<int>(s[i][1]),s[i][0]));
				}
				else{
					if (s[i].size() != 1){ return -412; }
					graph_pass.insert(make_pair(i,s[i][0]));
				}
			}
		}
		catch (out_of_range&){
			return -410;
		}
		catch (boost::bad_lexical_cast&){
			return -414;
		}
		//アニメーション定義読み込み
		try{
			//animationデータブロックの取得
			const DataBlock& anim_db = root_db.FindDataBlock("animation");
			vector<string> anim_list = move(anim_db.GetDataBlockList());
			//アニメーションデータの読み込み
			for (string& db : anim_list){
				//アニメデータ固有のデータ取得(なかったら0をセット)
				const DataBlock& anim_unit_db = anim_db.FindDataBlock(db);
				const vector<vector<string>>& d = anim_unit_db.GetAllData();
				anim_def new_anim;
				new_anim.interval = 0;
				new_anim.repeat = 0;
				for (auto& add : d){
					try{
						if (add[0] == "interval"){
							if (add.size() != 2){ continue; }
							new_anim.interval = boost::lexical_cast<int>(add[1]);
						}
						else if (add[0] == "repeat"){
							if (add.size() != 2){ continue; }
							new_anim.repeat = boost::lexical_cast<bool>(add[1]);
						}
						else if (add[0] == "size"){
							if (add.size() != 3){ continue; }
							new_anim.width = boost::lexical_cast<size_t>(add[1]);
							new_anim.height = boost::lexical_cast<size_t>(add[2]);
						}
						else{
							continue;
						}
					}
					catch (boost::bad_lexical_cast&){
						continue;
					}

				}
				//フレーム情報取得
				try{
					const DataBlock& frame_db = anim_unit_db.FindDataBlock("frame");
					const vector<vector<string>>& frame_data = frame_db.GetAllData();
					for (const auto& frame : frame_data){
						if (frame.size() == 2){
							if (frame[0] == "coordinates"){
								if (frame[1] == "pixcel"){ new_anim.pixel_coordinates_flag = true; }
								else if (frame[1] == "block"){ new_anim.pixel_coordinates_flag = false; }
							}
							else if (frame[0]=="AutomaticGeneration"){
								new_anim.automatic_generation_g_num = boost::lexical_cast<int>(frame[1]);
								break;
							}
							else{
								new_anim.f_data.emplace_back(anim_def::frame_data(boost::lexical_cast<int>(frame[0]), boost::lexical_cast<int>(frame[1]), 0));
							}
						}
						else if (frame.size() == 3){
							new_anim.f_data.emplace_back(anim_def::frame_data(boost::lexical_cast<int>(frame[0]), boost::lexical_cast<int>(frame[1]), boost::lexical_cast<int>(frame[2])));
						}
						else{
							break;
						}
					}
					anims.insert(unordered_map<string, anim_def>::value_type(db, move(new_anim)));
				}
				catch (out_of_range&){
					return -430;
				}
				catch (boost::bad_lexical_cast&){
					return -431;
				}
			}
		}
		catch (out_of_range&){
			return -420;
		}

		//画像を分割読み込み
		vector<int> graphes;
		std::unordered_map<int, int> gnum_index_map; //画像番号と配列インデックスの対応マップ
		for (const auto& gp : graph_pass){
			//画像読み込み
			int gh = -1;
			if (ref_ghandles){ //画像ハンドル参照フラグがonだったら画像ハンドルを参照する
				auto it = ref_ghandles->find(gp.second);
				if (it != ref_ghandles->end()){ gh = (*it).second; }
				else{ /*return -421;*/ } //指定IDの参照画像が存在しない
			}
			else{
				gh = LoadGraph(gp.second.c_str());
			}
			//画像が読み込まれていなかったら設定中止
			if (gh < 0){
				return -421;;
			}
			gnum_index_map.insert(make_pair(gp.first, gnum_index_map.size()));
			graphes.push_back(gh);
		}

		//定義からデータをセット
		ad->animationdataunit.clear();
		ad->_ghandles = move(graphes);
		for (unordered_map<string, anim_def>::value_type& anim : anims){
			ad->animationdataunit.push_back(make_unique<AnimationDataUnit>());
			AnimationDataUnit* adu = ad->animationdataunit[ad->animationdataunit.size() - 1].get();
			adu->tag = anim.first;
			adu->interval = anim.second.interval;
			adu->repeat_flag = anim.second.repeat;
			adu->size_x = anim.second.width > 0 ? anim.second.width : default_w;
			adu->size_y = anim.second.height > 0 ? anim.second.height : default_h;
			//フレーム設定
			if (anim.second.automatic_generation_g_num >= 0){ //自動フレーム設定
				int gnum = anim.second.automatic_generation_g_num;
				auto it = gnum_index_map.find(gnum); //画像番号を格納インデックスに変換
				if (it == gnum_index_map.end()){ //画像が存在しない
					adu->num = 0;
				}
				else{
					int gh = ad->_ghandles[it->second];
					int gs_x;
					GetGraphSize(gh, &gs_x, nullptr);
					size_t frame_num = gs_x / adu->size_x;
					adu->num = frame_num;
					for (size_t i = 0; i < frame_num; ++i){
						AnimationDataUnit::FrameInfo fi;
						fi.graph_num = it->second;
						fi.pos_x = adu->size_x*i;
						fi.pos_y = 0;
						adu->frame_info.push_back(fi);
					}
				}
			}
			else{ //データから設定
				//元画像には-1を入れる
				adu->num = anim.second.f_data.size();
				for (anim_def::frame_data& gp : anim.second.f_data){
					AnimationDataUnit::FrameInfo fi;
					auto it = gnum_index_map.find(gp.graph_num); //画像番号を格納インデックスに変換
					if (it == gnum_index_map.end()){
						//エラー画像には-1を入れる
						fi.graph_num = -1;
					}
					else{
						fi.graph_num = it->second;
					}
					if (anim.second.pixel_coordinates_flag){
						fi.pos_x = gp.x;
						fi.pos_y = gp.y;
					}
					else{
						fi.pos_x = gp.x*adu->size_x;
						fi.pos_y = gp.y*adu->size_y;
					}
					adu->frame_info.push_back(fi);
				}
			}
		}
		ad->num = anims.size();
		ad->SetGhandlesToAnimDataUnit();
		return 0;
	}

	void AnimationDataAdmin::SetVersion(Version v)
	{
		_version = v;
	}

};


