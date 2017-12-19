#pragma once

#include<string>

namespace cdec{
	class BinaryData{
	public:
		BinaryData();
		explicit BinaryData(size_t);
		BinaryData(unsigned char*, size_t);
		BinaryData(const BinaryData&);
		BinaryData(BinaryData&&);
		~BinaryData();
		const BinaryData& operator = (const BinaryData&);
		const BinaryData& operator = (BinaryData&&);
		unsigned char& operator [](size_t);
		const unsigned char& operator [](size_t)const;
		unsigned char& at(size_t);
		const unsigned char& at(size_t)const;
		int resize(size_t);
		void clear();
		size_t size()const;
		unsigned char* get();
		const unsigned char* get()const;
		std::string str(int = 0)const; //•¶š—ñ‰»(0:\i” 1:16i”)
		void set(unsigned char*, size_t);
	private:
		unsigned char* _top_ptr;
		size_t _size;
	};
}