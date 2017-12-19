#include"BinaryData.h"
#include<string>
#include<stdexcept>

namespace cdec{
	BinaryData::BinaryData():_top_ptr(nullptr),_size(0){}

	BinaryData::BinaryData(size_t size) : _top_ptr(new unsigned char[size]), _size(size){}

	BinaryData::BinaryData(unsigned char* tp, size_t s) : _top_ptr(tp), _size(s){}

	BinaryData::BinaryData(const BinaryData& obj):_top_ptr(new unsigned char[obj._size]),_size(obj._size){
		if(_size)memcpy_s(_top_ptr, _size, obj._top_ptr, _size);
	}

	BinaryData::BinaryData(BinaryData&& obj) : _top_ptr(obj._top_ptr), _size(obj._size){
		obj._top_ptr = nullptr;
		obj._size = 0;
	}

	BinaryData::~BinaryData(){
		if (_top_ptr)delete[] _top_ptr;
	}

	const BinaryData& BinaryData::operator=(const BinaryData& obj){
		if (obj._size){
			if (_top_ptr)delete[] _top_ptr;
			_top_ptr = new unsigned char[obj._size];
			_size = obj._size;
			memcpy_s(_top_ptr, _size, obj._top_ptr, _size);
		}
		return *this;
	}

	const BinaryData& BinaryData::operator=(BinaryData&& obj){
		if (_top_ptr)delete[] _top_ptr;
		_top_ptr = obj._top_ptr;
		_size = obj._size;
		obj._top_ptr = nullptr;
		obj._size = 0;
		return *this;
	}

	const unsigned char& BinaryData::operator[](size_t idx)const{
		return _top_ptr[idx];
	}

	unsigned char& BinaryData::operator[](size_t idx){
		return const_cast<unsigned char&>(static_cast<const BinaryData&>(*this)[idx]);
	}

	const unsigned char& BinaryData::at(size_t idx)const{
		if (idx >= _size){ 
			std::string e("assigned out of binary data. file:");
			e += __FILE__;
			/*e += ",line:";
			e += __LINE__;*/
			throw std::out_of_range(e); 
		}
		return (*this)[idx];
	}

	unsigned char& BinaryData::at(size_t idx){
		return const_cast<unsigned char&>(static_cast<const BinaryData&>(*this).at(idx));
	}

	int BinaryData::resize(size_t s){
		unsigned char* np = new unsigned char[s];
		if (_top_ptr){
			for (size_t i = 0; i < _size && i < s; ++i){
				np[i] = _top_ptr[i];
			}
			delete[] _top_ptr;
		}
		_top_ptr = np;
		_size = s;
		return 0;
	}

	void BinaryData::clear(){
		if (_top_ptr)delete[] _top_ptr;
		_top_ptr = nullptr;
		_size = 0;
	}

	size_t BinaryData::size()const{ return _size; }

	const unsigned char* BinaryData::get()const{ return _top_ptr; }

	unsigned char* BinaryData::get(){ return const_cast<unsigned char*>(static_cast<const BinaryData&>(*this).get()); }

	std::string BinaryData::str(int m)const{
		std::string out;
		if (_top_ptr == nullptr){ return out; }
		switch (m)
		{
		case 0:
			for (size_t i = _size - 1; i >= _size; --i){
				char c;
				sprintf_s(&c, 1, "%u", (unsigned int)_top_ptr[i]);
				out.push_back(c);
			}
			break;
		case 1:
			for (size_t i = _size - 1; i >= _size; --i){
				char c;
				sprintf_s(&c, 1, "%x", (unsigned int)_top_ptr[i]);
				out.push_back(c);
			}
			break;
		default:
			break;
		}
		return std::move(out);
	}

	void BinaryData::set(unsigned char* tp,size_t s){
		if (_top_ptr)delete[] _top_ptr;
		_top_ptr = tp;
		_size = s;
	}
}

