#pragma once

#include<unordered_map>

//�}�g���N�X�}�b�v(T�ɂ͔�r���Z�q����`����Ă���K�v������܂�)
template <typename T, typename U>
class MatrixMap{
public:
	void SetMap(const T&, const T&, const U&);
	bool CheckMap(const T&, const T&);
	void Clear();
private:
	std::unordered_map<T, std::unordered_map<T, U>> _map;
};

template <typename T, typename U>
void MatrixMap<T, U>::SetMap(const T& c0, const T& c1, const U& f){
	T mc0, mc1;
	//mc0�ɏ��������Amc1�ɑ傫�������Z�b�g����
	if (c0 < c1){
		mc0 = c0;
		mc1 = c1;
	}
	else{
		mc0 = c1;
		mc1 = c0;
	}
	//�Z�b�g
	auto it0 = _map.find(mc0);
	if (it0 == _map.end()){ //���Z�b�g�̑g�ݍ��킹
		std::unordered_map<T, U> m;
		m.emplace(std::make_pair(mc1, f));
		_map.insert(std::make_pair(mc0, m));
	}
	else{
		auto it1 = (*it0).second.find(mc1);
		if (it1 == (*it0).second.end()){ //���Z�b�g�̑g�ݍ��킹
			(*it0).second.insert(std::make_pair(mc1, f));
		}
		else{
			(*it1).second = f;
		}
	}
}

template <typename T, typename U>
bool MatrixMap<T, U>::CheckMap(const T& c0, const T& c1){
	T mc0, mc1;
	//mc0�ɏ��������Amc1�ɑ傫�������Z�b�g����
	if (c0 < c1){
		mc0 = c0;
		mc1 = c1;
	}
	else{
		mc0 = c1;
		mc1 = c0;
	}
	//�Z�b�g
	auto it0 = _map.find(mc0);
	if (it0 == _map.end()){ //���Z�b�g�̑g�ݍ��킹
		throw std::out_of_range("�w�肳�ꂽ�L�[�����݂��܂���B");
	}
	else{
		auto it1 = (*it0).second.find(mc1);
		if (it1 == (*it0).second.end()){ //���Z�b�g�̑g�ݍ��킹
			throw std::out_of_range("�w�肳�ꂽ�L�[�����݂��܂���B");
		}
		else{
			return (*it1).second;
		}
	}
}

template <typename T, typename U>
void MatrixMap<T, U>::Clear(){
	_map.clear();
}
