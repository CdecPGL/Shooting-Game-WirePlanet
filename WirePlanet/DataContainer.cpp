#include "DataContainer.h"
#include<algorithm>

using namespace std;

//�R���X�g���N�^
DataContainer::DataContainer()
{
}

//�f�X�g���N�^
DataContainer::~DataContainer(){
	/*for (unordered_map<string, boost::any>::iterator it = data_info.begin(); it != data_info.end(); ++it){
		delete (*it).second;
	}*/
}

//�R�s�[�R���X�g���N�^
DataContainer::DataContainer(const DataContainer& obj){
	//�����̃f�[�^���R�s�[
	for (unordered_map<string, boost::any>::iterator it = const_cast<DataContainer&>(obj).data_info.begin(); it != obj.data_info.end(); ++it){
		data_info.insert(unordered_map<string, boost::any>::value_type((*it).first,(*it).second));
	}
}

//�w��f�[�^�̍폜
int DataContainer::DeleteData(const string& id){
	unordered_map<string, boost::any>::iterator it = data_info.find(id);
	if (it == data_info.end()){ return -1; } //�w�肳�ꂽID�ɑ��݂��Ȃ�
//	delete (*it).second; //�f�[�^���폜
	data_info.erase(it); //���X�g����폜
	return 0;
}

//�S����
void DataContainer::clear(){
	for (unordered_map<string, boost::any>::iterator it = data_info.begin(); it != data_info.end(); ++it){
//		delete (*it).second;
	}
	data_info.clear();
}

//�T�C�Y�擾
size_t DataContainer::size(){
	return data_info.size();
}

//���Z�q
DataContainer DataContainer::operator + (const DataContainer& obj){
	DataContainer out; //�o�͗p
	out = *this; //�����̃f�[�^����
	//�����̃f�[�^��out�ɂȂ����̂�out�ɃR�s�[
	for (unordered_map<string, boost::any>::iterator it = const_cast<DataContainer&>(obj).data_info.begin(); it != obj.data_info.end(); ++it){
		unordered_map<string, boost::any>::iterator fit = out.data_info.find((*it).first);
		if(fit == out.data_info.end()){
			out.data_info.insert(unordered_map<string, boost::any>::value_type((*it).first, (*it).second));
		}
	}
	return out;
}
DataContainer DataContainer::operator - (const DataContainer& obj){
	DataContainer out; //�o�͗p
	out = *this; //�����̃f�[�^����
	//�����̃f�[�^��out�ɂ�����̂�out����폜
	for (unordered_map<string, boost::any>::iterator it = const_cast<DataContainer&>(obj).data_info.begin(); it != obj.data_info.end(); ++it){
		unordered_map<string, boost::any>::iterator fit = out.data_info.find((*it).first);
		if (fit != out.data_info.end()){
			out.data_info.erase(fit);
		}
	}
	return out;
}
DataContainer DataContainer::operator & (const DataContainer& obj){
	DataContainer out; //�o�͗p
	out = *this; //�����̃f�[�^����
	//out�̃f�[�^�ň����ɂȂ����̂��폜
	for (unordered_map<string, boost::any>::iterator it =out.data_info.begin(); it != out.data_info.end(); ++it){
		unordered_map<string, boost::any>::iterator fit = const_cast<DataContainer&>(obj).data_info.find((*it).first);
		if (fit == obj.data_info.end()){
			out.data_info.erase(it);
		}
	}
	return out;
}
DataContainer DataContainer::operator | (const DataContainer& obj){
	DataContainer out; //�o�͗p
	out = *this; //�����̃f�[�^����
	//�����̃f�[�^��out�ɂȂ����̂�out�ɃR�s�[
	for (unordered_map<string, boost::any>::iterator it = const_cast<DataContainer&>(obj).data_info.begin(); it != obj.data_info.end(); ++it){
		unordered_map<string, boost::any>::iterator fit = out.data_info.find((*it).first);
		if (fit == out.data_info.end()){
			out.data_info.insert(unordered_map<string, boost::any>::value_type((*it).first, (*it).second));
		}
	}
	return out;
}
DataContainer DataContainer::operator ^ (const DataContainer& obj){
	DataContainer out; //�o�͗p
	out = *this; //�����̃f�[�^����
	//�����̃f�[�^��out�ɂȂ����̂�out�ɃR�s�[���Ă�����̂�out����폜
	for (unordered_map<string, boost::any>::iterator it = const_cast<DataContainer&>(obj).data_info.begin(); it != obj.data_info.end(); ++it){
		unordered_map<string, boost::any>::iterator fit = out.data_info.find((*it).first);
		if (fit == out.data_info.end()){
			out.data_info.insert(unordered_map<string, boost::any>::value_type((*it).first, (*it).second));
		}
		else{
			out.data_info.erase(fit);
		}
	}
	return out;
}
DataContainer DataContainer::operator = (const DataContainer& obj){
	DataContainer out; //�o�͗p
	//�����������Ă���f�[�^���폜
	clear();
	//�����̃f�[�^���R�s�[
	for (unordered_map<string, boost::any>::iterator it = const_cast<DataContainer&>(obj).data_info.begin(); it != obj.data_info.end(); ++it){
		data_info.insert(unordered_map<string, boost::any>::value_type((*it).first, (*it).second));
	}
	return out;
}
//bool DataContainer::operator == (const DataContainer& obj){
//	//�f�[�^�����������F
//	if (data_info.size() != obj.data_info.size()){ return false; }
//	//�Ⴄ���̂���������F
//	auto pair = std::mismatch(data_info.begin(), data_info.end(), obj.data_info.begin());
//	if (pair.first != data_info.end()){ return false; }
//	return true;
//}

//bool DataContainer::operator != (const DataContainer& obj){
//	//�f�[�^��������Ă���T
//	if (data_info.size() != obj.data_info.size()){ return true; }
//	//�Ⴄ���̂�����������F
//	auto pair = std::mismatch(data_info.begin(), data_info.end(), obj.data_info.begin());
//	if (pair.first == data_info.end()){ return false; }
//	return true;
//}

bool DataContainer::operator < (const DataContainer& obj){
	if (data_info.size() < const_cast<DataContainer&>(obj).size()){ return true; }
	return false;
}

bool DataContainer::operator > (const DataContainer& obj){
	if (data_info.size() > const_cast<DataContainer&>(obj).size()){ return true; }
	return false;
}

bool DataContainer::operator <= (const DataContainer& obj){
	if (data_info.size() <= const_cast<DataContainer&>(obj).size()){ return true; }
	return false;
}

bool DataContainer::operator >= (const DataContainer& obj){
	if (data_info.size() >= const_cast<DataContainer&>(obj).size()){ return true; }
	return false;
}

DataContainer& DataContainer::operator += (const DataContainer& obj){
	//�����̃f�[�^�Ŏ����ɂȂ����̂������ɃR�s�[
	for (unordered_map<string, boost::any>::iterator it = const_cast<DataContainer&>(obj).data_info.begin(); it != obj.data_info.end(); ++it){
		unordered_map<string, boost::any>::iterator fit = data_info.find((*it).first);
		if (fit == data_info.end()){
			data_info.insert(unordered_map<string, boost::any>::value_type((*it).first, (*it).second));
		}
	}
	return *this;
}
DataContainer& DataContainer::operator -= (const DataContainer& obj){
	//�����̃f�[�^�Ŏ����ɂ�����̂���������폜
	for (unordered_map<string, boost::any>::iterator it = const_cast<DataContainer&>(obj).data_info.begin(); it != obj.data_info.end(); ++it){
		unordered_map<string, boost::any>::iterator fit = data_info.find((*it).first);
		if (fit != data_info.end()){
			data_info.erase(fit);
		}
	}
	return *this;
}
DataContainer& DataContainer::operator &= (const DataContainer& obj){
	//�����̃f�[�^�ň����ɂȂ����̂��폜
	for (unordered_map<string, boost::any>::iterator it = data_info.begin(); it != data_info.end(); ++it){
		unordered_map<string, boost::any>::iterator fit = const_cast<DataContainer&>(obj).data_info.find((*it).first);
		if (fit == obj.data_info.end()){
			data_info.erase(it);
		}
	}
	return *this;
}
DataContainer& DataContainer::operator |= (const DataContainer& obj){
	//�����̃f�[�^�Ŏ����ɂȂ����̂������ɃR�s�[
	for (unordered_map<string, boost::any>::iterator it = const_cast<DataContainer&>(obj).data_info.begin(); it != obj.data_info.end(); ++it){
		unordered_map<string, boost::any>::iterator fit = data_info.find((*it).first);
		if (fit == data_info.end()){
			data_info.insert(unordered_map<string, boost::any>::value_type((*it).first, (*it).second));
		}
	}
	return *this;
}
DataContainer& DataContainer::operator ^= (const DataContainer& obj){
	//�����̃f�[�^�Ŏ����ɂȂ����̂������ɃR�s�[���Ă�����̂���������폜
	for (unordered_map<string, boost::any>::iterator it = const_cast<DataContainer&>(obj).data_info.begin(); it != obj.data_info.end(); ++it){
		unordered_map<string, boost::any>::iterator fit = data_info.find((*it).first);
		if (fit == data_info.end()){
			data_info.insert(unordered_map<string, boost::any>::value_type((*it).first, (*it).second));
		}
		else{
			data_info.erase(fit);
		}
	}
	return *this;
}
