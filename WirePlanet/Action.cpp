#include "Action.h"
#include<fstream>
#include"SVLoader.h"
#include"FileLoadUtility.h"

using namespace std;

const std::string ACTION_LIST("action_list.csv");

//アクションデータUnitコンストラクタ
ActionDataUnit::ActionDataUnit(const std::string m_tag,int m_gh,int m_intrvl,bool m_rf,int size):tag(m_tag),interval(m_intrvl),repeat_flag(m_rf){
	//printfDx(tag);
	int x,y;
	GetGraphSize(m_gh,&x,&y);
	int w = x/size;
	int h = y/size;
	num = w*h;
	ghandle.push_back(m_gh); //元画像の登録
	for(int j=0;j<h;j++){
		for(int i=0;i<w;i++){
			ghandle.push_back(DerivationGraph(i*size,j*size,size,size,m_gh)); //あぬめーしょん画像の生成と登録
		}
	}
};

//ActionDataUnitデストラクタ
ActionDataUnit::~ActionDataUnit(){
	//画像の削除
	std::vector<int>::iterator it;
	for(it=ghandle.begin() + 1;it<ghandle.end();it++){
		DeleteGraph((*it));
	}
	DeleteGraph((*ghandle.begin()));
	//当たり判定データの消去
	std::vector<HitAreaData*>::iterator hit;
	for(hit=hitarea.begin(); hit!=hitarea.end(); hit++){
		delete *hit;   // メモリー解放
	}
}

//HitAreaDataコンストラクタ
HitAreaData::HitAreaData(int m_kind,int m_x,int m_y,int m_r,int m_sf,int m_ef,int m_ap[2]):attribute(m_kind),x(m_x),y(m_y),radius(m_r),start_frame(m_sf),end_frame(m_ef){
	if (m_ap){
		additional_param[0] = m_ap[0];
		additional_param[1] = m_ap[1];
	}
	else{
		additional_param[0] = 0;
		additional_param[1] = 0;
	}
}

//ActionData構造体
//コンストラクタ
ActionData::ActionData():size(0),num(0){}
ActionData::ActionData(const char* filename):size(32),num(0){
	int error = ReadActionFileFromFile(filename);
	if(error != 0){printfDx("error:ReadActionFileFromFile=%d",error);}
}

//デストラクタ
ActionData::~ActionData(){
	std::vector<ActionDataUnit*>::iterator it;
	for(it=data.begin(); it!=data.end(); it++){
		delete *it;   // メモリー解放
	}
}



//ActionClass

//コンストラクタ(ActionDataの参照を得る)
Action::Action(const ActionData& ad):actiondata(ad),frame(0),counter(0),current_anim(0),repeat(false){
	
}

//デストラクタ
Action::~Action(void){
	
}

//コピーコンストラクタ
//Action::Action(const Action& action){
//	size = action.size;
//	num = action.num;
//	counter = action.counter;
//	frame = action.frame;
//	std::vector<ActionData*>::iterator it;
//	for(it=action.data.begin();it!=action.data.end();it++){
//		data.push_back(new ActionData());
//	}
//}

//更新
void Action::Update() {
	if(actiondata.num==0){return;} //アクションがなかったらスキップ
	if(frame>0 && actiondata.data.at(current_anim)->interval!=0 && counter>=actiondata.data.at(current_anim)->interval){frame++;counter=0;} //フレームの加算(frameが0か現在のアクションがアニメなしかカウンターが不十分なら加算しない)
	//フレームがアニメーション枚数を超えたら
	if(frame > actiondata.data.at(current_anim)->num){
		if(repeat==false){ //繰り返しなしだったら
			frame=0; //0にしてアニメーション終了
		}else{ //繰り返しありだったら
			frame=1; //1にして再びアニメーション
		}
	}

	////有効な当たり判定の数を求める
	//current_hitarea_num=0;
	//std::vector<HitAreaData*>::iterator it;
	//std::vector<HitAreaData*> current_hitarea = data[current_anim]->hitarea;
	//for(it=current_hitarea.begin();it!=current_hitarea.end();it++){
	//	if((*it)->start_frame<=frame && frame<=(*it)->end_frame){current_hitarea_num++;}
	//}
	//アニメーションカウンタの加算
	counter++;
}

//現在の画像の取得
int Action::GetCurrentGraph() const{
	if(actiondata.num==0){return 0;}
	//frameが0だったら最後のフレーム(num)の画像を返す
	if(frame==0){return actiondata.data.at(current_anim)->ghandle[actiondata.data.at(current_anim)->num];}
	return actiondata.data.at(current_anim)->ghandle[frame];
}

//現在のフレーム数を取得
int Action::GetCurrentFrame() const{
	if(actiondata.num==0){return 0;}
	return frame;
}

//現在のアクションのタグと引数を比較
bool Action::CheckCurrentActionTag(const std::string& string) const{
	if(actiondata.num==0){return false;}
	if(string == actiondata.data.at(current_anim)->tag){return true;}
	else{return false;}
}

//タグを用いて任意のアクションをスタート
int Action::StartActionByTag(const std::string& m_tag){
	if(actiondata.num==0){return -1;} //アクションがなかったらスキップ
	bool exist=false;
	for(int i=0;i<actiondata.num;i++){
		if (m_tag == actiondata.data.at(i)->tag){
			current_anim = i;
			frame = 1;
			exist = true; 
			repeat = actiondata.data.at(i)->repeat_flag;
			break;
		}
	}
	if (!exist){ return -1; }
	return 0;
}
//タグを用いて任意のアクションをスタート
int Action::StartActionByTag(const std::string& m_tag,bool r_f){
	if (actiondata.num == 0){ return -1; } //アクションがなかったらスキップ
	bool exist = false;
	for (int i = 0; i<actiondata.num; i++){
		if (m_tag == actiondata.data.at(i)->tag){
			current_anim = i;
			frame = 1;
			exist = true;
			repeat = r_f;
			break;
		}
	}
	if (!exist){ return -1; }
	return 0;
}

//アクションを一コマ進める
int Action::ProgressAnim(){
	if(actiondata.num==0){return 0;} //アクションがなかったらスキップ
	if(frame>0){frame++;} //フレームの加算(frameが0だったら加算しない)
	frame++;
	counter=0;
	//フレームがアニメーション枚数を超えたら
	if(frame > actiondata.data.at(current_anim)->num){
		if(actiondata.data.at(current_anim)->repeat_flag==false){ //繰り返しなしだったら
			frame=0; //0にしてアニメーション終了
		}else{ //繰り返しありだったら
			frame=1; //1にして再びアニメーション
		}
	}
	return 0;
}

//現在有効な当たり判定の数を得る
//int Action::GetCurrentHitAreaNum() const{
//	if(num==0){return 0;} //アクションがなかったらスキップ
//	return current_hitarea_num;
//}

//HitAreaDataポインタのvectorに現在有効な当たり判定データを入れる
int Action::SetCurrentHitAreaToVector(std::vector<HitAreaData*>* m_hitarea) const{
	if(actiondata.num==0){return 0;} //アクションがなかったらスキップ
	//リセット
	m_hitarea->clear();
	std::vector<HitAreaData*>::iterator it;
	std::vector<HitAreaData*> current_hitarea = actiondata.data[current_anim]->hitarea;
	for(it=current_hitarea.begin();it!=current_hitarea.end();it++){
		//現在のフレームがこのhitareaの開始、終了フレーム内だったら追加(hitareaの開始終了がともに0でも追加)
		if(((*it)->start_frame<=frame && frame<=(*it)->end_frame) || ((*it)->start_frame==0 && 0==(*it)->end_frame)){m_hitarea->push_back(*it);}
	}
	return 0;
}

//現在のアクションタグを取得
const std::string Action::GetCurrentActionTag()const{
	if (actiondata.num == 0){ return "None"; }
	if (frame == 0){ return "None;"; }
	return actiondata.data.at(current_anim)->tag;
}

//アクション再生中か確認
bool Action::CheckActionPlaying()const{
	if (actiondata.num == 0){ return false; }
	if (frame == 0){ return false; }
	return true;
}

//画像サイズ所得
int Action::GetGraphSize()const{
	return actiondata.size;
}

//ActionData
ActionData::RefGHandlesType ActionData::_ref_ghandles;
bool ActionData::_ghandle_ref_flag = false;

//ファイルからアクションデータを読み込む
int ActionData::ReadActionFileFromFile(const string& file_name){
	ifstream ifs(file_name);
	if (!ifs){ return -2; }
	if (ifs.fail()){ return -1; }
	int word;
	string buf;
	vector<string> unit;
	while (!ifs.eof()){
		word = ifs.get();
		if (word == EOF){break;} //ファイルの終わりだったらるーぷえお抜ける
		//{}=;だったら分割した文字を保存してバッファリセット
		if (word != (int)(*"\n") && word != (int)(*"\t") && word != (int)(*" ")){ //改行タブスペース
			if (word == (int)(*"{") || word == (int)(*"}") || word == (int)(*";") || word == (int)(*"=")){
				unit.push_back(move(buf)); //代入
				buf = word;
				unit.push_back(move(buf)); //カッコを入れる
				buf.clear(); //バッファの初期化
			}else{
				buf += word; //バッファに文字追加
			}
		}
	}
	ifs.close();
	return ReadActionFileFromSeparatedString(move(unit));
	//FILE* fp;
	//fopen_s(&fp,filename,"rt"); //ファイルを開く
	//if(fp==NULL){return -1;} //ファイルのオープンに失敗した
	//int word; //読み取った文字
	//std::string buf; //バッファ
	//std::vector<std::string> unit; //分解済み文字列
	////構文に沿って分解
	//while(1){
	//	word = fgetc(fp); //一文字読み取り
	//	if (word == EOF){break;} //ファイルの終わりだったらるーぷえお抜ける
	//	//{}=;だったら分割した文字を保存してバッファリセット
	//	if (word != (int)(*"\n") && word != (int)(*"\t") && word != (int)(*" ")){ //改行タブスペース
	//		if (word == (int)(*"{") || word == (int)(*"}") || word == (int)(*";") || word == (int)(*"=")){
	//			unit.push_back(move(buf)); //代入
	//			buf = word;
	//			unit.push_back(move(buf)); //カッコを入れる
	//			buf.clear(); //バッファの初期化
	//		}else{
	//			buf += word; //バッファに文字追加
	//		}
	//	}
	//}
	//fclose(fp); //ファイルを閉じる
}

//データからからアクションデータを読み込む
int ActionData::ReadActionFileFromMem(const void* top_ptr,unsigned int size){
	const char* data = static_cast<const char*>(top_ptr);
	int word;
	string buf;
	vector<string> unit;
	for (int i = 0; i < size;++i){
		word = data[i];
		if (word == EOF){ break; } //ファイルの終わりだったらるーぷえお抜ける
		//\rだったらスキップ
		if (word == '\r'){ continue; }
		//{}=;だったら分割した文字を保存してバッファリセット
		if (word != (int)(*"\n") && word != (int)(*"\t") && word != (int)(*" ")){ //改行タブスペース
			if (word == (int)(*"{") || word == (int)(*"}") || word == (int)(*";") || word == (int)(*"=")){
				unit.push_back(move(buf)); //代入
				buf = word;
				unit.push_back(move(buf)); //カッコを入れる
				buf.clear(); //バッファの初期化
			}
			else{
				buf += word; //バッファに文字追加
			}
		}
	}
	return ReadActionFileFromSeparatedString(move(unit));
}


//分割済み文字列からアクションデータを読み込む
int ActionData::ReadActionFileFromSeparatedString(std::vector<std::string>&& unit){
	//構文チェック(カッコのチェック;はチェックしない)
	std::vector<std::string>::iterator it;
	int kakko(0);
	for(it=unit.begin();it!=unit.end();it++){
		if((*it)=="{"){++kakko;}
		if((*it)=="}"){--kakko;}
	}
	for(it=unit.begin();it!=unit.end();it++){
		if((*it)==";"){unit.erase(it);it=unit.begin();} //;は消す
	}
	if(kakko != 0){;return -2;} //カッコの数が合わない

	/*for(it=unit.begin();it!=unit.end();it++){
		printfDx((*it).c_str());
		printfDx("|");
	}*/
	/*printfDx("\n");*/

	//構文解析
	//階層は3まで
	std::string label[3]={"None","None","None"};
	int error(0);
	int depth(0);
	double pp_version(0);
	int ad_size(32);
	std::string adu_tag;
	std::string adu_gfn;
	int adu_intertval(0);
	bool adu_rf(false);
	std::vector<HitAreaData*> adu_had;
	int had_kind(0);
	int had_x(0),had_y(0);
	int had_r(0);
	int had_sf(0),had_ef(0);
	int had_ap[2] = {0,0};

	for(it=unit.begin();it!=unit.end();++it){
		if((*it)=="{"){
			switch(depth){
			case 0:
				--it;
				label[0] = (*it);
				++it;
				depth = 1;
				break;
			case 1:
				--it;
				label[1] = (*it);
				++it;
				depth = 2;
				break;
			case 2:
				--it;
				label[2] = (*it);
				++it;
				depth = 3;
				break;
			default:
				break;
			}
		}else if((*it)=="}"){
			switch(depth){
			case 1:
				if(label[0]=="Property"){
//					printfDx("version = %lf\n",pp_version);
					label[0]="None";
					depth = 0;
				}else if(label[0]=="ActionData"){
					size = ad_size;
					num = data.size();
					label[0]="None";
					ad_size = 32;
					depth=0;
				}
				break;
			case 2:
				if(label[1]=="ActionDataUnit"){
					//画像読み込みまたは検索
					int adu_gh=-1;
					if (_ghandle_ref_flag){ //画像ハンドル参照
						auto it = _ref_ghandles.find(adu_gfn);
						if (it == _ref_ghandles.end()){
							adu_gh = LoadGraph(adu_gfn.c_str());
						}
						else{
							adu_gh = (*it).second;
						}
					}
					else{ //作成
						adu_gh = LoadGraph(adu_gfn.c_str());
					}
					if (adu_gh < 0){ 
						error = -3;
						printf("ActionData::ReadActionFileFromSeparatedString 画像ファイルを読み込めませんでした。(%s,%s)\n",adu_tag.c_str(), adu_gfn.c_str());
						break;
					}

					ActionDataUnit* ad_adu = new ActionDataUnit(adu_tag,adu_gh,adu_intertval,adu_rf,ad_size);
					data.push_back(ad_adu);
//					std::vector<HitAreaData*>::iterator it_had;
//					for(it_had=adu_had.begin();it_had!=adu_had.end();++it_had){
//						ad_adu->hitarea.push_back(*it_had);
//					}
					ad_adu->hitarea = adu_had;
					label[1]="None";
					adu_tag = "None";
					adu_gfn = "None";
					adu_intertval = 0;
					adu_rf = false;
					adu_had.clear();
					depth=1;
				}
				break;
			case 3:
				if(label[2]=="HitAreaData"){
					adu_had.push_back(new HitAreaData(had_kind,had_x,had_y,had_r,had_sf,had_ef,had_ap));
					label[2]="None";
					had_kind = 0;
					had_x = 0;had_y = 0;
					had_r = 0;
					had_sf = 0;had_ef = 0;
					had_ap[0] = 0; had_ap[1] = 0;
					depth=2;
				}
				break;
			default:
				break;
			}
		}else if(1){
			switch(depth){
			case 1:
				if(label[0]=="Property"){
					if((*it)=="kind"){
						++it;
						if((*it)=="="){
							++it;
							if((*it) != "ActionFile"){error=-3;break;}
						}
					}else if((*it)=="version"){
						++it;
						if((*it)=="="){
							++it;
							pp_version = atof((*it).c_str());
						}
					}
				}else if(label[0]=="ActionData"){
					if((*it)=="size"){
						++it;
						if((*it)=="="){
							++it;
							ad_size = atoi((*it).c_str());
						}
					}
				}
				break;
			case 2:
				if(label[1]=="ActionDataUnit"){
					if((*it)=="tag"){
						++it;
						if((*it)=="="){
							++it;
							adu_tag = (*it);
						}
					}else if((*it)=="graphic_filename"){
						++it;
						if((*it)=="="){
							++it;
							adu_gfn = (*it);
						}
					}else if((*it)=="interval"){
						++it;
						if((*it)=="="){
							++it;
							adu_intertval = atoi((*it).c_str());
						}
					}else if((*it)=="repeat_flag"){
						++it;
						if((*it)=="="){
							++it;
							adu_rf = atoi((*it).c_str());
						}
					}
				}
				break;
			case 3:
				if(label[2]=="HitAreaData"){
					if((*it)=="attribute"){
						++it;
						if((*it)=="="){
							++it;
							had_kind = atoi((*it).c_str());
						}
					}else if((*it)=="x"){
						++it;
						if((*it)=="="){
							++it;
							had_x = atoi((*it).c_str());
						}
					}else if((*it)=="y"){
						++it;
						if((*it)=="="){
							++it;
							had_y = atoi((*it).c_str());
						}
					}else if((*it)=="radius"){
						++it;
						if((*it)=="="){
							++it;
							had_r = atoi((*it).c_str());
						}
					}else if((*it)=="start_frame"){
						++it;
						if((*it)=="="){
							++it;
							had_sf = atoi((*it).c_str());
						}
					}else if((*it)=="end_frame"){
						++it;
						if((*it)=="="){
							++it;
							had_ef = atoi((*it).c_str());
						}
					}else if ((*it) == "additional_param"){
						++it;
						if ((*it) == "="){
							++it;
							had_ap[0] = atoi((*it).c_str());
						}
					}else if ((*it) == "matchless_time"){
						++it;
						if ((*it) == "="){
							++it;
							had_ap[1] = atoi((*it).c_str());
						}
					}
				}
				break;
			default:
				break;
			}
		}
		/*printfDx((*it).c_str());
		printfDx("\n");
		std::string string = "label[0],[1],[2] = "+label[0]+","+label[1]+","+label[2]+"\n";
		printfDx(string.c_str());
		printfDx(adu_tag.c_str());
		ScreenFlip();
		ClearDrawScreen();
		WaitKey();*/
	}
	if(error!=0){return error;}
//	printfDx("data.size = %d\n",data.size());
	return 0;
}

ActionDataAdmin::ActionDataAdmin(){
	printf("ActiondataAdminのインスタンスが作成されました。\n");
}

ActionDataAdmin::~ActionDataAdmin(){
	printf("ActiondataAdminのインスタンスが破棄されました。\n");
}

ActionDataAdmin& ActionDataAdmin::GetInstance(){
	static ActionDataAdmin ada;
	return ada;
}

int ActionDataAdmin::Init(){
	try{
		CSVLoader csvl;
		if (FileLoadUtil::LoadCSVFromFLM(&csvl,ACTION_LIST,true)){
			printf("ActionDataAdminの初期化に失敗しました。アクションファイルリストの読み込みに失敗しました。%s\n", ACTION_LIST.c_str());
		}
		for (auto& it : csvl){
			if (it.size() != 2){ continue; }
			unique_ptr<ActionData> nad = make_unique<ActionData>();
			FileLoadUtil::LoadActionDataFromFLM(nad.get(), it[1], true);
			action_datas.emplace(make_pair(it[0], move(nad)));
		}
	}
	catch (out_of_range& e){
		printf("ActionDataAdminの初期化に失敗しました。初期データが不足しています。(%s)\n", e.what());
		return -1;
	}
	printf("ActiondataAdminが初期化されました。\n");
	return 0;
}

ActionData& ActionDataAdmin::GetActionData(const string& id){
	auto it = action_datas.find(id);
	if (it == action_datas.end()){ throw out_of_range("ActionDataAdmin::GetActionData()"); }
	return *((*it).second);
}

void ActionDataAdmin::Clear(){
	action_datas.clear();
}

void ActionDataAdmin::SetRefGHandles(ActionData::RefGHandlesType&& rgh){
	ActionData::_ref_ghandles = move(rgh);
}

void ActionDataAdmin::SetRefGHandles(const ActionData::RefGHandlesType& rgh){
	ActionData::_ref_ghandles = rgh;
}

void ActionDataAdmin::SetRefGhandleFlag(bool f){
	ActionData::_ghandle_ref_flag = f;
}

