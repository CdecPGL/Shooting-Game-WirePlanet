#include "DataBlock.h"
#include <cstdarg>

using namespace std;

DataBlock::DataBlock()
{
}

DataBlock::DataBlock(const std::vector<std::vector<std::string>>& dat):datas(dat){

}

DataBlock::DataBlock(std::vector<std::vector<std::string>>&& dat) : datas(move(dat)){

}

DataBlock::DataBlock(const DataBlock& obj):datas(obj.datas),_child_datablocks(obj._child_datablocks){
}

DataBlock::DataBlock(DataBlock&& obj):datas(std::move(obj.datas)),_child_datablocks(std::move(obj._child_datablocks)){
}


DataBlock::~DataBlock()
{
}

const DataBlock& DataBlock::operator=(const DataBlock& obj){
	datas = obj.datas;
	_child_datablocks = obj._child_datablocks;
	return *this;
}

const DataBlock& DataBlock::operator=(DataBlock&& obj){
	datas = std::move(obj.datas);
	_child_datablocks = std::move(obj._child_datablocks);
	return *this;
}

std::shared_ptr<DataBlock> DataBlock::FindChild(const std::string& dbn){
	auto it = _child_datablocks.find(dbn);
	if (it == _child_datablocks.end()){
		return nullptr;
		/*string err("Not exists ChildDataBlock: ");
		err + dbn;
		throw out_of_range(err);*/
	}
	return it->second;
}
std::shared_ptr<const DataBlock> DataBlock::FindChild(const std::string& dbn)const{
	return (const_cast<DataBlock*>(this))->FindChild(dbn);
}

const std::vector<shared_ptr<DataBlock>> DataBlock::FindAllChildren(const std::string& dbn){
	auto its = _child_datablocks.equal_range(dbn);
	if (its.first == _child_datablocks.end()){
		return vector<shared_ptr<DataBlock>>();
		/*string err("Not exists ChildDataBlock: ");
		err + dbn;
		throw out_of_range(err);*/
	}
	std::vector<shared_ptr<DataBlock>> ret;
	for (auto it = its.first; it != its.second; ++it){
		ret.push_back(it->second);
	}
	return std::move(ret);
}

const std::vector<shared_ptr<const DataBlock>> DataBlock::FindAllChildren(const std::string& dbn)const{
	auto its = _child_datablocks.equal_range(dbn);
	if (its.first == _child_datablocks.end()){
		return vector<shared_ptr<const DataBlock>>();
		/*string err("Not exists ChildDataBlock: ");
		err + dbn;
		throw out_of_range(err);*/
	}
	std::vector<shared_ptr<const DataBlock>> ret;
	for (auto it = its.first; it != its.second; ++it){
		ret.push_back(it->second);
	}
	return std::move(ret);
}

int DataBlock::AddChild(const string& dbn, shared_ptr<DataBlock> db){
	_child_datablocks.insert(make_pair(dbn, db));
	return 0;
}
int DataBlock::AddChild(const std::string& dbn, const std::vector<std::vector<std::string>>& dat){
	shared_ptr<DataBlock> ndb = make_shared<DataBlock>();
	ndb->SetData(dat);
	_child_datablocks.insert(make_pair(dbn, ndb));
	return 0;
}
int DataBlock::AddChild(const std::string& dbn, std::vector<std::vector<std::string>>&& dat){
	shared_ptr<DataBlock> ndb = make_shared<DataBlock>();
	ndb->SetData(move(dat));
	_child_datablocks.insert(make_pair(dbn, ndb));
	return 0;
}

std::vector<std::string> DataBlock::GetChildrenList()const{
	vector < string > list;
	for (auto& p : _child_datablocks){
		list.push_back(p.first);
	}
	return move(list);
}

size_t DataBlock::ChildrenCountWithName(const string& dbn)const{
	auto its = _child_datablocks.equal_range(dbn);
	size_t ret=0;
	for (auto it = its.first; it != its.second; ++it){
		++ret;
	}
	return ret;
}

size_t DataBlock::ChildrenCount()const{
	return _child_datablocks.size();
}

int DataBlock::RemoveChild(weak_ptr<DataBlock> db){
	for (auto it = _child_datablocks.begin(); it != _child_datablocks.end(); ++it){
		if (it->second.get() == db.lock().get()){ _child_datablocks.erase(it); return 0; } //一致
	}
	return -1;
}

int DataBlock::RemoveChild(const string& dbn){
	return _child_datablocks.erase(dbn) == 0 ? -1 : 0;
}

//データ
const vector<vector<string>>& DataBlock::GetAllData()const{
	return datas;
}
vector<vector<string>>& DataBlock::GetAllData(){
	return datas;
}

const vector<string>& DataBlock::GetLine(int l)const{
	return datas.at(l);
}
vector<string>& DataBlock::GetLine(int l){
	return datas.at(l);
}

const string& DataBlock::GetData(int l, int r)const{
	return datas.at(l).at(r);
}
string& DataBlock::GetData(int l, int r){
	return datas.at(l).at(r);
}

const vector<string>& DataBlock::operator[](int l)const{
	return datas[l];
}
vector<string>& DataBlock::operator[](int l){
	return datas[l];
}

//互換用
const DataBlock& DataBlock::FindDataBlock(const string& dbn)const{
	auto ptr = FindChild(dbn);
	if (ptr == nullptr){
		string err("Not exists ChildDataBlock: ");
		err + dbn;
		throw out_of_range(err);
	}
	return *ptr;
}

const DataBlock& DataBlock::operator[](const string& dbn)const{
	auto ptr = FindChild(dbn);
	if (ptr == nullptr){
		string err("Not exists ChildDataBlock: ");
		err + dbn;
		throw out_of_range(err);
	}
	return *ptr;
}

vector<string> DataBlock::GetDataBlockList()const{
	return std::move(GetChildrenList());
}

bool DataBlock::IsDataBlockExist(const std::string& dbn)const{
	return _child_datablocks.find(dbn) != _child_datablocks.end();
}

int DataBlock::AddLineToBack(const std::vector<string>& nl)
{
	datas.push_back(nl);
	return 0;
}

int DataBlock::AddLineToBack(std::vector<std::string>&& nl)
{
	datas.push_back(move(nl));
	return 0;
}

int DataBlock::SetData(const std::vector<std::vector<std::string>>& dat)
{
	datas = dat;
	return 0;
}

int DataBlock::SetData(std::vector<std::vector<std::string>>&& dat){
	datas = move(dat);
	return 0;
}


