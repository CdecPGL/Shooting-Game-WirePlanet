#include"Archiver.h"
#include<fstream>
#include"zlib_cdec.h"
#include"EXORenckey.h"

using namespace std;
using namespace ArcLib;

Archiver::Archiver():_archiving(false),_exor_enc_key(std::make_unique<EXOREncKey>()){}

Archiver::~Archiver(){}

int Archiver::BeginArchive(const string& fn,Mode mode,unsigned int cl){
	if (_archiving){ return -10; }
	if (cl < 1 || cl>9){ return -4; }
	_comp_level = cl;
	_mode = mode;
	_ofs.open(fn, ios::binary | ios::trunc);
	if (!_ofs){ return -1; }
	if (_ofs.fail()){ return -2; }
	_header.clear();
//	_datas.clear();
	//ヘッダー
	//モード
	unsigned char m = ConvertModeToUnsignedInt(_mode);
	_ofs.write((char*)&m, 1);
	if (mode == Mode::EXOREncrypt){ //データサイズ記載域(4Bytes)分だけEXORキーを進める
		_exor_enc_key->SeekCurrentExorKey(4);
	}
	//データサイズの記載分だけ書き込んでおく
	_ofs.write((char*)&_header.total_data_size, sizeof(_header.total_data_size));
	_archiving = true;
	return 0;
}

int Archiver::EndArchive(){
	if (!_archiving){ return -10; }
	//フッダー出力
	int cmp_tar_footer_size = (NAME_SIZE + 4 + 4 + 4)*_header.num; //圧縮対象のフッターサイズ
	unsigned char* fptr = new unsigned char[cmp_tar_footer_size]; //圧縮対象フッターデータのポインタ
	int idx = 0;
	//ファイル数記録
	_Uint32t data_num = _header.num;
	if (_mode == Mode::EXOREncrypt){
		_exor_enc_key->ExorEncryptData(&data_num, sizeof(data_num));
	}
	_ofs.write((char*)&data_num, sizeof(data_num));
	//fptrにデータを追加していく関数
	auto push_fptr = [&idx, fptr, cmp_tar_footer_size](const void* ptr, unsigned int size)->void{memcpy_s(fptr + idx, cmp_tar_footer_size - idx, ptr, size); idx += size; };

	//ファイル情報を圧縮対象フッターデータに記録
	for (auto& fi : _header.file_infos){
		push_fptr(fi->name, NAME_SIZE);
		push_fptr((char*)&fi->address, sizeof(fi->address));
		push_fptr((char*)&fi->org_size, sizeof(fi->org_size));
		push_fptr((char*)&fi->prc_size, sizeof(fi->prc_size));
	}
	//圧縮対象フッターを処理して保存
	switch (_mode)
	{
	case Mode::SimpleBind:
		write_simplebind(fptr, cmp_tar_footer_size);
		break;
	case Mode::Compress:
		write_compress(fptr, cmp_tar_footer_size);
		break;
	case Mode::EXOREncrypt:
		write_exorenc(fptr, cmp_tar_footer_size);
		break;
	default:
		break;
	}
	delete[] fptr;
	//データサイズ書き換え
	_ofs.seekp(1); //先頭のデータサイズ記録位置に移動(モード記録(1Byte)のつぎ)
	uint32_t data_size = _header.total_data_size;
	if (_mode == Mode::EXOREncrypt){
		_exor_enc_key->ResetCurrentKey();
		_exor_enc_key->ExorEncryptData(&data_size, sizeof(data_size));
	}
	_ofs.write((char*)&data_size, sizeof(data_size));
	//終了
	_ofs.close();
	_archiving = false;
	return 0;
}

int Archiver::InsertData(const string& name, const void* tp, unsigned int s){
	if (!_archiving){ return -10; }
	//名前サイズがNAME_SIZEByte以上だったらエラー
	if (name.size() > NAME_SIZE){ return -1; }
	//データ書き込み
	_Uint32t write_size;
	switch (_mode)
	{
	case Mode::SimpleBind:
		write_size = write_simplebind(tp, s);
		break;
	case Mode::Compress:
		write_size = write_compress(tp, s);
		break;
	case Mode::EXOREncrypt:
		write_size = write_exorenc(tp, s);
		break;
	default:
		break;
	}
	//ヘッダー編集
	std::unique_ptr<Header::FileInfo> fi = make_unique<Header::FileInfo>();
	fi->address = _header.total_data_size;
	_header.total_data_size += write_size;
	fi->org_size = s;
	fi->prc_size = write_size;
	for (int i = 0; i < NAME_SIZE; ++i){
		fi->name[i] = 0;
	}
	strcpy_s(fi->name, NAME_SIZE, name.c_str());
	_header.file_infos.push_back(move(fi));
	++_header.num;
	return 0;
}

_Uint32t Archiver::write_simplebind(const void* tp,unsigned int s){
	_ofs.write((char*)tp, s);
	return s;
}

_Uint32t Archiver::write_compress(const void* tp, unsigned int s){
	uLong write_size = compressBound(s);
	Bytef* wtp = new Bytef[write_size];
	if (compress2(wtp, &write_size, (const Bytef*)tp, s, _comp_level)==Z_MEM_ERROR){
		throw std::runtime_error("Archiver::insert_compress メモリー不足で圧縮に失敗しました。");
	}
	_ofs.write((char*)wtp, write_size);
	delete[] wtp;
	return write_size;
}

_Uint32t Archiver::write_exorenc(const void* tp, unsigned int s){
	uLong write_size = compressBound(s);
	Bytef* wtp = new Bytef[write_size];
	if (compress2(wtp, &write_size, (const Bytef*)tp, s, _comp_level) == Z_MEM_ERROR){
		throw std::runtime_error("Archiver::insert_compress メモリー不足で圧縮に失敗しました。");
	}
	_exor_enc_key->ExorEncryptData(wtp, write_size); //EXOR暗号化
	_ofs.write((char*)wtp, write_size);
	delete[] wtp;
	return write_size;
}

//EXOR暗号化関連
int Archiver::SetEXOREncryptionKey(uint16_t k){
	if (_archiving){ return -1; }
	_exor_enc_key->SetKey(k);
	return 0;
}

void Archiver::Header::clear(){
	num = 0;
	total_data_size = 0;
	file_infos.clear();
}
