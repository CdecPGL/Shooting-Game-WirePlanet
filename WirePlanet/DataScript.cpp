#pragma warning(disable : 4996) 

#include"DataScript.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include"boost/algorithm/string.hpp"

#include<time.h>

using namespace std;

namespace{
	const char* PPP_REPLACE("REPLACE");
}

DataScript::DataScript():script_name(""),version(DataScriptVersion::DSVer1_5),root_data_block(make_shared<DataBlock>()),_preproc_valid_flag(false){
	//データロード、文字列化関数セット
	loaddata.insert(pair<DataScriptVersion, int(DataScript::*)(std::string&)>(DataScriptVersion::DSVer1_5, &DataScript::LoadDataV1_5));
	datatostring.insert(pair<DataScriptVersion, string(DataScript::*)()const>(DataScriptVersion::DSVer1_5, &DataScript::DataToStringV1_5));
}

DataScript::DataScript(const string& ext):DataScript(){
	SetExtension(ext);
}

DataScript::~DataScript(){

}

//拡張子設定
void DataScript::SetExtension(const string& ext){
	extension = "." + ext;
}

//指定名データ取得
int DataScript::GetDataAll(const string& dn, vector<vector<string>>& dat)const{
	try{
		auto& it = root_data_block->FindDataBlock(dn);
		dat = it.GetAllData();
		return 0;
	}
	catch (out_of_range&){
		return -1;
	}
}
int DataScript::GetDataLine(const string& dn, int line, vector<string>& dat)const{
	try{
		auto& it = root_data_block->FindDataBlock(dn);
		try{
			dat = it.GetLine(line);
			return 0;
		}
		catch (out_of_range&){
			return -2;
		}
	}
		catch (out_of_range&){
		return -1;
	}
}
int DataScript::GetDataAssigned(const string& dn, int line, int raw, string& dat)const{
	try{
		auto& it = root_data_block->FindDataBlock(dn);
		try{
			dat = it.GetData(line, raw);
			return 0;
		}
		catch (out_of_range&){
			return -2;
		}
	}
	catch (out_of_range&){
		return -1;
	}
}

//指定名のデータ変更
int DataScript::ChangeDataAll(const string& dn, const vector<vector<string>>& dat){
	/*try{
		auto& it = root_data_block.FindDataBlock(dn);
		const_cast<vector<vector<string>>&>(it.GetAllData())=dat;
		return 0;
	}
	catch (out_of_range&){
		return -1;
	}*/
	return -1;
}
int DataScript::ChangeDataLine(const string& dn, int line, const vector<string>& dat){
	/*try{
		auto& it = root_data_block.FindDataBlock(dn);
		try{
			const_cast<vector<string>&>(it.GetLine(line))=dat;
			return 0;
		}
		catch (out_of_range&){
			return -2;
		}
	}
	catch (out_of_range&){
		return -1;
	}*/
	return -1;
}
int DataScript::ChangeDataAssigned(const string& dn, int line, int raw, const string& dat){
	/*try{
		auto& it = root_data_block.FindDataBlock(dn);
		try{
			const_cast<string&>(it.GetData(line, raw)) = dat;
			return 0;
		}
		catch (out_of_range&){
			return -2;
		}
	}
	catch (out_of_range&){
		return -1;
	}*/
	return -1;
}

//指定名のデータ削除
int DataScript::DeleteData(const string& dn){
	/*map<string, vector<vector<string>>>::iterator it = data.find(dn);
	if (it == data.end()){ return -1; }
	data.erase(it);
	return 0;*/
	return -1;
}

//データ名取得
vector<string> DataScript::GetDataName()const{
	return root_data_block->GetDataBlockList();
}

//サイズ取得
int DataScript::GetSize(const string& dn, int& line, int& raw)const{
	try{
		auto& it = root_data_block->FindDataBlock(dn);
		line = it.datas.size();
		if (line == 0){ raw = 0; return 0; }
		raw = it.datas.at(0).size();
		return 0;
	}
	catch (out_of_range&){
		return -1;
	}
}

//指定名のデータサイズ変更
int DataScript::ChangeSize(const string& dn, int line, int raw,string blank){
	//map<string, vector<vector<string>>>::iterator it = data.find(dn);
	//if (it == data.end()){ return -1; }
	////行を合わせる
	//(*it).second.resize(line);
	////列を合わせる
	//for (vector<vector<string>>::iterator rit = (*it).second.begin(); rit != (*it).second.end();++rit){
	//	(*rit).resize(raw,blank);
	//}
	//return 0;
	return -1;
}

//指定名のデータ作成
int DataScript::CreateData(const string& dn, int line, int raw,string blank){
	/*map<string, vector<vector<string>>>::iterator it = data.find(dn);
	if (it != data.end()){ return -1; }
	data.insert(map<string, vector<vector<string>>>::value_type(dn, vector<vector<string>>()));
	ChangeSize(dn, line, raw,blank);
	return 0;*/
	return -1;
}

//削除
void DataScript::Clear(){
	root_data_block->_child_datablocks.clear();
	root_data_block->datas.clear();
//	data.clear();
}

//スクリプトからデータ読み込み
int DataScript::LoadDataFromScript(const string& filename,bool add_flag){
	if (add_flag == false){ Clear(); }
	ifstream ifs(filename + extension);
	string str;
	while (ifs.eof() != true){
		string buf;
		ifs >> buf;
		//コメント行だったら除外
		if (buf.size() > 2){
			if (buf[0] == '/' && buf[1] == '/'){
				continue;
			}
		}
		//行はじめの水平タブ文字を取り除く
		while (buf.size() > 0 && buf[0] == '\t'){
			buf.erase(0);
		}
		str += buf;
	}
	ifs.close();

	int result = process_load(std::move(str));
	if (result<0){ return result; }

	script_name = filename;
	return result;
}

//文字列からデータ読み込み
int DataScript::LoadDataFromString(const string& str, bool add_flag){
	if (add_flag == false){ Clear(); }
	int result = process_load(str);
	if (result){ return result; }

	script_name = "FromString";
	return result;
}

//バイナリデータからデータ読み込み
int DataScript::LoadDataFromBynary(const void* top_ptr,unsigned int size,bool add_flag){
	if (add_flag == false){ Clear(); }
	stringstream ss;
	string str;
	bool skip_flag=false;
	for (unsigned int i = 0; i < size; ++i){
		char w = ((char*)top_ptr)[i];
		//改行、水平タブは除く
		switch (w){
		case '\r':case '\t': //除く
			continue;
		case '\n': //改行だったら無視終了
			skip_flag = false;
			continue;
		case '/': //"//"だったら無視フラグ
			if (i < size - 1){
				if (((char*)top_ptr)[i + 1] == '/'){
					++i;
					skip_flag = true;
					continue;
				}
			}
			break;
		default:
			break;
		}
		if (!skip_flag){ ss << w; }
	}

	int result = process_load(std::move(ss.str()));
	if (result){ return result; }

	script_name = "FromBynary";
	return result;
}

//データ表示
void DataScript::DrawData()const{
	cout << this->ToString() << endl;
}

//マクロ機能の使用設定
void DataScript::SetPreProcessFlag(bool umf){ _preproc_valid_flag = umf; }

//保存
int DataScript::SaveDataToScript(const string& filename, bool add_flag)const
{
	ofstream ofs;
	if (add_flag == false){ ofs.open(filename + extension, ios::out | ios::trunc); }
	if (add_flag == true){ ofs.open(filename + extension, ios::out | ios::app); }
	ofs << this->ToString() << endl;
	ofs.close();
	return 0;
}

//読み込んだスクリプトにに保存
int DataScript::SaveDataToLoadedScript()const{
	if (script_name == "") { return -1; }
	return SaveDataToScript(script_name);
}

//文字列の取得
string DataScript::ToString()const{
	return move((this->*(datatostring.find(version)->second))());
}

//データの存在確認
bool DataScript::CheckDataExistence(const string& dn)const
{
	return root_data_block->IsDataBlockExist(dn);
}

vector<string> DataScript::GetErrorString()const{ return error_strings; }

void DataScript::SetErrorString(const string& err_str){
	error_strings.push_back(err_str);
}

bool DataScript::SJISMultiCheck(unsigned char c){
	if (((c >= 0x81) && (c <= 0x9f)) || ((c >= 0xe0) && (c <= 0xfc)))return true;
	else return false;
}

int DataScript::Test(const string& fn,int cnt){
	int err = 0;
	ifstream ifs(fn + extension);
	string str, buf;
	while (ifs.eof() != true){
		ifs >> buf;
		//コメント行だったら除外
		if (buf.size() > 2){
			if (buf[0] == '/' && buf[1] == '/'){
				continue;
			}
		}
		str += buf;
	}
	ifs.close();
	printf("PPP:%s\n", _preproc_valid_flag ? "true" : "false");
	printf("DataLoaded from %s\n", fn.c_str());
	printf("---Start checking performance.(%d)---\n",cnt);
	clock_t ct = clock();
	for (int i = 0; i < cnt; ++i){
		if ((err = LoadDataFromString(str))){ break; }
	}
	ct = clock() - ct;
	if (err){
		printf("Error!(%d)\n", err);
		vector<string> ess = GetErrorString();
		for (string es : ess){
			std::cout << es << endl;
		}
		return err;
	}
	else{
		printf("---End checking performance.(%lfms)---\n", (double)ct / (double)CLOCKS_PER_SEC*1000.0);
		printf("avarage: %lfms\n", (double)ct / (double)CLOCKS_PER_SEC*1000.0 / (double)cnt);
	}

	printf("---Start tset---.\n");
	printf("===<<<Loaded data.>>>===\n");
	DrawData();
	printf("=======\n");

	return 0;

	string str2 = ToString();
	LoadDataFromString(str2);
	printf("===<<<Reloaded data.>>>===\n");
	DrawData();
	printf("=======\n");
	if (str == ToString()){
		printf("Test sccessed.\n");
	}
	else{
		printf("Test failed.\n");
	}
	printf("---End test---.\n");
	return 0;
}

//バージョン設定
void DataScript::SetVersion(DataScriptVersion v){
	version = v;
	if (version == DataScriptVersion::DSVer1_4){ version = DataScriptVersion::DSVer1_5; }
}

//バージョン取得
DataScriptVersion DataScript::GetVersion()const{
	return version;
}

//Version1~3
//int DataScript::LoadDataV1_3(std::string& str){
//	データブロックの取り出し
//	blockname{}で囲まれた正規表現
//	using namespace boost;
//	vector<string> datablocks; //データブロック
//	データブロック抽出
//	try{
//		regex reg_db("[^,;{}]+\\{[^}]+\\}");
//		if (!regex_grep([&datablocks](const smatch& m)->bool{datablocks.push_back(m.str()); return true; }, str, reg_db)){
//			return -1;
//		}
//	}
//	catch (regex_error::exception e){
//		std::string err = "データブロック抽出でエラー発生 "; 
//		err += e.what();
//		err += "\n";
//		error_strings.push_back(err);
//		OutputDebugString(err.c_str());
//		return -1;
//	}
//	for (string datablock : datablocks){
//		vector<string> lines; //行
//		vector<vector<string>> ndb; //新しいデータブロック
//		regex reg_ln("[^;{}]*;");
//		行抽出
//		if (!regex_grep(
//			[&lines](const smatch& m)->bool{
//			lines.push_back(m.str());
//			return true; 
//		}
//			, datablock, reg_ln)){
//			continue;
//		}
//		for (string line : lines){
//			regex reg_dt("[^,;]*[,;]");
//			vector<string> datas; //データ
//			データ抽出
//			if (!regex_grep(
//				[&datas](const smatch& m)->bool{
//				string mstr = m.str();
//				mstr.pop_back();
//				datas.push_back(mstr);
//				return true;
//			}
//				, line, reg_dt)){
//				continue;
//			}
//			ndb.push_back(move(datas));
//		}
//		regex reg_dbn("^[^{},;]+");
//		smatch sm_dbn;
//		if (regex_search(datablock, sm_dbn, reg_dbn)){
//			data.insert(pair<string, vector<vector<string>>>(sm_dbn.str(), ndb));
//		}
//	}
//	return 0;
//}

//ロード処理関数
int DataScript::process_load(string data){
	int res=0;
	if (_preproc_valid_flag){
		res = pre_process_before_load(data);
		if (res < 0){ return res; }
	}
	res = (this->*(loaddata[version]))(data);
	if (res < 0){ return res * 10; }
	return 0;
}

//プリProcess
int DataScript::pre_process_before_load(string& data){
	//コマンド
	list<vector<string>> commands;
	//置換ペア
	list<pair<string, string>> replace_list;
	//定義を抜き出す
	size_t cur = 0;
	while (cur < data.size()){
		if (data[cur] != '#'){ break; }
		size_t c_cur = 1;
		string buf;
		vector<string> cmnd_string;
		while (cur + c_cur < data.size()){
			if (data[cur + c_cur] == ',' || data[cur + c_cur] == ';'){
				cmnd_string.push_back(move(buf));
				buf.clear();
				if (data[cur + c_cur] == ';'){ break; }
			}else{
				buf.push_back(data[cur + c_cur]);
			}
			++c_cur;
		}
		commands.push_back(move(cmnd_string));
		cur += c_cur + 1;
	}
	//定義部を削除
	data.erase(0, cur);
	//while ((cur = data.find_first_of("#",cur))!=string::npos){
	//	++cur;
	//	//命令、引数を取得
	//	size_t end_pos = data.find_first_of(";", cur); //現在のマクロ命令が終わる位置
	//	if (end_pos == string::npos){ break; } //終了記号がなっかったら終わり
	//	string command = data.substr(cur, end_pos - cur);
	//	std::vector<string> cmnds;
	//	boost::split(command, cmnds, boost::is_any_of(" "));
	//	commands.push_back(move(cmnds));
	//	cur = end_pos + 1;
	//}
	//コマンド処理
	for (auto& v : commands){
		if (v[0]==PPP_REPLACE){
			if (v.size() != 3){ continue; }
			replace_list.push_back(make_pair(move(v[1]), move(v[2])));
		}
	}
	//置換処理
	for (auto& rp : replace_list){
		boost::replace_all(data, rp.first, rp.second);
	}
	return 0;
}

//Version4
int DataScript::LoadDataV1_5(std::string& str){
	root_data_block = LoadDataBlock(str);
	return 0;
}

shared_ptr<DataBlock> DataScript::LoadDataBlock(const std::string& str){
//	printf("In LoadDataBlock().\n");
	shared_ptr<DataBlock> db = make_shared<DataBlock>();
	int db_s_idx=0; //データブロック開始インデックス
	int d_b_idx = -1; //データ境界インデックス
	int depth = 0;
	for (unsigned int i = 0; i < str.size(); ++i){
		//マルチバイトコードだったらカウンタを増やしてスキップ
		if (SJISMultiCheck(str[i])){
			++i;
			continue;
		}
		switch (str[i])
		{
		case '{':
			if (depth <= 0){ db_s_idx = i + 1; }
			++depth;
			break;
		case '}':
			if (depth <= 0){
				throw std::domain_error("DataScript::LoadDataBlock '{'と'}'の個数が一致しません。");
			}else if (depth==1){
				d_b_idx = i;
			}
			--depth;
			if (depth == 0){
				//データブロック抽出
				//ブロック名位置検索
				int dbn_idx = db_s_idx-2;
				for (; dbn_idx >= 0; --dbn_idx){
					if (str[dbn_idx] == ';' || str[dbn_idx] == '}'){
						break;
					}
				}
				db->_child_datablocks.emplace(unordered_multimap<string, std::shared_ptr<DataBlock>>::value_type(move(str.substr(dbn_idx + 1, db_s_idx - dbn_idx - 2)), LoadDataBlock(str.substr(db_s_idx, i - db_s_idx))));
//				db.datablocks.insert(unordered_map<string, DataBlock>::value_type(move(str.substr(dbn_idx + 1, idx - dbn_idx - 2)), move(LoadDataBlock(str.substr(idx, i - idx)))));
			}
			break;
		case ';':
		{
			if (depth > 0){ break; }
			//開始点を探す
			/*int sidx = i-1;
			for (; sidx >= 0; --sidx){

				if (str[sidx] == ';' || str[sidx] == '}'){ break; }
			}*/
			//データ抜粋分割
			vector<string> res;
			boost::split(res, str.substr(d_b_idx + 1, i - d_b_idx - 1), boost::is_any_of(","));
			db->datas.push_back(move(res));
			d_b_idx = i;
		}
			break;
		default:
			break;
		}
	}
//	printf("Finish to search DataBlock.\n");
	return db;
}

string DataScript::DataToStringV1_5()const{	
	return move(DataBlockToString(*root_data_block));
}

string DataScript::DataBlockToString(const DataBlock& db,int depth){
	string out;
	for (auto& l : db.datas){
		for (int i = 0; i < depth; ++i){
			out += "\t";
		}
		for (unsigned int i = 0; i < l.size(); ++i){
			out += l[i];
			if (i < l.size() - 1){ out += ","; }
		}
		out += ";\n";
	}
	for (auto& mdb : db._child_datablocks){
		for (int i = 0; i < depth; ++i){
			out += "\t";
		}
		out += (mdb.first + "{\n");
		out += DataBlockToString(*(mdb.second),depth+1);
		for (int i = 0; i < depth; ++i){
			out += "\t";
		}
		out += "}\n";
	}
	return out;
}

shared_ptr<const DataBlock> DataScript::GetRoot()const{ return root_data_block; }
shared_ptr<DataBlock> DataScript::GetRoot(){ return root_data_block; }

//互換用
const DataBlock& DataScript::GetRootDataBlock()const{ return *root_data_block; }
DataBlock& DataScript::GetRootDataBlock(){ return *root_data_block; }