#pragma once

#include<unordered_map>

//マトリクスマップ(Tには比較演算子が定義されている必要があります)
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
	//mc0に小さい方、mc1に大きい方をセットする
	if (c0 < c1){
		mc0 = c0;
		mc1 = c1;
	}
	else{
		mc0 = c1;
		mc1 = c0;
	}
	//セット
	auto it0 = _map.find(mc0);
	if (it0 == _map.end()){ //未セットの組み合わせ
		std::unordered_map<T, U> m;
		m.emplace(std::make_pair(mc1, f));
		_map.insert(std::make_pair(mc0, m));
	}
	else{
		auto it1 = (*it0).second.find(mc1);
		if (it1 == (*it0).second.end()){ //未セットの組み合わせ
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
	//mc0に小さい方、mc1に大きい方をセットする
	if (c0 < c1){
		mc0 = c0;
		mc1 = c1;
	}
	else{
		mc0 = c1;
		mc1 = c0;
	}
	//セット
	auto it0 = _map.find(mc0);
	if (it0 == _map.end()){ //未セットの組み合わせ
		throw std::out_of_range("指定されたキーが存在しません。");
	}
	else{
		auto it1 = (*it0).second.find(mc1);
		if (it1 == (*it0).second.end()){ //未セットの組み合わせ
			throw std::out_of_range("指定されたキーが存在しません。");
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
