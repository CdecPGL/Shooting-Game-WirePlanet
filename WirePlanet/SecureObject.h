#pragma once

#include"secure_exception.h"

namespace secure_obj{

	//�Í�����B���f�[�^�ƈÍ����f�[�^�̃T�C�Y�͓��������̂Ƃ���
	class Encrypter{
	public:
		Encrypter()=default;
		~Encrypter() = default;
		virtual void Encode(unsigned char*,unsigned int,unsigned char*) = 0;
		virtual void Decode(unsigned char*, unsigned int, unsigned char*) = 0;
	};

	class BitInvertEncrypter : public Encrypter{
	public:
		BitInvertEncrypter() = default;
		~BitInvertEncrypter() = default;
		void Encode(unsigned char* org_dat, unsigned int s, unsigned char* out_dat)override{
			for (unsigned int i = 0; i < s; ++i){
				out_dat[i] = ~org_dat[i];
			}
		}
		void Decode(unsigned char* org_dat, unsigned int s, unsigned char*out_dat)override{
			for (unsigned int i = 0; i < s; ++i){
				out_dat[i] = ~org_dat[i];
			}
		}
	};

	template<typename T, class E = BitInvertEncrypter>
	class SecureObject{
	public:
		SecureObject();
		SecureObject(T*);
		SecureObject(const T&);
		~SecureObject();
		void SetPointer(T*);
		const T& get()const;
		void set(const T&);
		T& instance();
		void synchronize(); //���f�[�^�ƈÍ����f�[�^�̓��������
	private:
		T* _obj;
		Encrypter* _enpr; //�Í�����
		unsigned char* _encrypted_data; //�Í����f�[�^
		mutable unsigned char* _decrypted_data; //�������f�[�^
		unsigned int _size; //�f�[�^�T�C�Y
		bool check()const; //���f�[�^�ƈÍ����f�[�^�̐��������m�F����
	};

	template<typename T,class E>
	SecureObject<T, E>::SecureObject() :_obj(new T), _encrypted_data(new unsigned char[sizeof(T)]) , _decrypted_data(new unsigned char[sizeof(T)]), _size(0), _enpr(new E){
		synchronize();
	}

	template<typename T, class E>
	SecureObject<T, E>::SecureObject(T* obj) : _obj(obj), _encrypted_data(new unsigned char[sizeof(T)]) , _decrypted_data(new unsigned char[sizeof(T)]), _size(sizeof(T)), _enpr(new E){
		synchronize();
	}

	template<typename T, class E>
	SecureObject<T, E>::SecureObject(const T& dat) : _obj(new T(dat)), _encrypted_data(new unsigned char[sizeof(T)]), _decrypted_data(new unsigned char[sizeof(T)]), _size(sizeof(T)), _enpr(new E){
		synchronize();
	}

	template<typename T, class E>
	SecureObject<T, E>::~SecureObject(){
		if (_obj){ delete _obj; }
		if (_encrypted_data){ delete[] _encrypted_data; }
		if (_decrypted_data){ delete[] _decrypted_data; }
		if (_enpr){ delete _enpr; }
	}

	template<typename T, class E>
	void SecureObject<T, E>::SetPointer(T* p){
		if (_obj){ delete _obj; }
		_obj = p;
		synchronize();
	}

	template<typename T, class E>
	const T& SecureObject<T, E>::get()const{
		if (_obj){
			if (check() == false){
				std::string err("SecureObject::get �s�������o����܂����B");
				throw cheat_detected(err);
			}
			return *_obj;
		}
		else{ throw std::domain_error("SecureObject::get �|�C���^���Z�b�g����Ă��܂���"); }
	}

	template<typename T, class E>
	T& SecureObject<T, E>::instance(){
		if (_obj){
			if (check() == false){
				std::string err("SecureObject::instance �s�������o����܂����B");
				throw cheat_detected(err);
			}
			return *_obj;
		}
		else{ throw std::domain_error("SecureObject::instance �|�C���^���Z�b�g����Ă��܂���"); }
	}


	template<typename T, class E>
	void SecureObject<T, E>::set(const T& dat){
		if (_obj){
			if (check() == false){
				std::string err("SecureObject::set �s�������o����܂����B");
				throw cheat_detected(err);
			}
			*_obj = dat;
			synchronize();
		}
		else{ throw std::domain_error("SecureObject::set �|�C���^���Z�b�g����Ă��܂���"); }
	}

	template <typename T,class E>
	void SecureObject<T, E>::synchronize(){
		_enpr->Encode((unsigned char*)(_obj), _size, _encrypted_data);
	}

	template <typename T, class E>
	bool SecureObject<T, E>::check()const{
		_enpr->Decode(_encrypted_data, _size, _decrypted_data);
		unsigned char* _obj_p = (unsigned char*)_obj;
		bool cheat=false;
		for (unsigned int i = 0; i < _size; ++i){
			if (_obj_p[i] != _decrypted_data[i]){
				cheat = true;
				break;
			}
		}
		return !cheat;
	}
}