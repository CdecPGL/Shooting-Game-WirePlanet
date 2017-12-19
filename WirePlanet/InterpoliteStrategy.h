#ifndef DEF_INTERPOLITESTRATEGY_H
#define DEF_INTERPOLITESTRATEGY_H

#include<tuple>
#include<cassert>

namespace otd{
	//��ԃX�g���e�W�N���X
	template<class VAL, typename DIS = double, typename WHT = double>
	class InterpoliteStrategy{
	protected:
		using TRIO = std::tuple < VAL, DIS, WHT >;
	public:
		/**
		* @brief �f�X�g���N�^
		*/
		virtual ~InterpoliteStrategy() = default;
		/**
		* @brief �K�v�ȓ_�̐����擾
		*/
		virtual size_t needPointNum() = 0;
		/**
		* @brief ��Ԓl���v�Z
		* @param ��Ԍ���,��Ԋ���
		*/
		virtual bool calc(VAL*, const DIS&) = 0;
		/**
		* @brief �_��o�^����z����擾
		* @return �_��o�^����z����擾�̃|�C���^
		*/
		virtual TRIO** getPtrAry() = 0;
	};

	//���`��ԃN���X
	template<class VAL, typename DIS = double, typename WHT = double>
	class LinearInterpolite : InterpoliteStrategy < VAL, DIS, WHT> {
	public:
		/**
		* @brief �R���X�g���N�^
		*/
		LinearInterpolite();
		/**
		* @brief �f�X�g���N�^
		*/
		~LinearInterpolite()override;
		/**
		* @brief �K�v�ȓ_�̐����擾
		*/
		virtual size_t needPointNum()override;
		/**
		* @brief ���`��Ԓl���v�Z
		* @param ��Ԍ���,��Ԋ���
		*/
		bool calc(VAL*, const DIS&)override;
		/**
		* @brief �_��o�^����z����擾
		* @return �_��o�^����z����擾�̃|�C���^
		*/
		TRIO** getPtrAry()override;
	protected:
		TRIO* points[2]; //�L�[�z��|�C���^
	};

	template<class VAL, typename DIS, typename WHT>
	LinearInterpolite<VAL, DIS, WHT>::LinearInterpolite(){
		points[0] = NULL;
		points[1] = NULL;
	}

	template<class VAL, typename DIS, typename WHT>
	LinearInterpolite<VAL, DIS, WHT>::~LinearInterpolite(){
		
	}

	template<class VAL, typename DIS, typename WHT>
	size_t LinearInterpolite<VAL, DIS, WHT>::needPointNum(){
		return 2; //���`��Ԃɂ�2�̒l���K�v
	}

	template<class VAL, typename DIS, typename WHT>
	bool LinearInterpolite<VAL, DIS, WHT>::calc(VAL* pOut,const DIS& t){
		// ���`��Ԓl���v�Z
		*pOut = (std::get<0>(*points[0]))*(1 - t) + (std::get<0>(*points[1]))*t;
		return true;
	}

	template<class VAL, typename DIS, typename WHT>
	typename LinearInterpolite<VAL, DIS, WHT>::TRIO** LinearInterpolite<VAL, DIS, WHT>::getPtrAry(){
		return (TRIO**)&points;
	}

	//�X�v���C����ԃN���X
	template<class VAL, typename DIS = double, typename WHT = double>
	class SplineInterpolite : InterpoliteStrategy < VAL, DIS, WHT> {
	public:
		/**
		* @brief �R���X�g���N�^
		*/
		SplineInterpolite();
		/**
		* @brief �f�X�g���N�^
		*/
		~SplineInterpolite()override;
		/**
		* @brief �K�v�ȓ_�̐����擾
		*/
		virtual size_t needPointNum()override;
		/**
		* @brief ���`��Ԓl���v�Z
		* @param ��Ԍ���,��Ԋ���
		*/
		bool calc(VAL*, const DIS&)override;
		/**
		* @brief �_��o�^����z����擾
		* @return �_��o�^����z����擾�̃|�C���^
		*/
		TRIO** getPtrAry()override;
	protected:
		TRIO* points[3]; //�L�[�z��|�C���^
	};

	template<class VAL, typename DIS, typename WHT>
	SplineInterpolite<VAL, DIS, WHT>::SplineInterpolite(){
		points[0] = NULL;
		points[1] = NULL;
		points[2] = NULL;
	}

	template<class VAL, typename DIS, typename WHT>
	SplineInterpolite<VAL, DIS, WHT>::~SplineInterpolite(){

	}

	template<class VAL, typename DIS, typename WHT>
	size_t SplineInterpolite<VAL, DIS, WHT>::needPointNum(){
		return 3; //�X�v���C����Ԃɂ�3�̒l���K�v
	}

	template<class VAL, typename DIS, typename WHT>
	bool SplineInterpolite<VAL, DIS, WHT>::calc(VAL* pOut, const DIS& t){
		// �X�v���C����Ԓl���v�Z
		assert(false); //�������Ȃ̂Ŏg���Ȃ�
//		*pOut = (std::get<0>(*points[0]))*(1 - t) + (std::get<0>(*points[1]))*t;
		VAL p0 = std::get<0>(*points[0]);
		VAL p1 = std::get<0>(*points[1]);
		VAL p2 = std::get<0>(*points[2]);
		VAL v0(0,0);
		VAL v1(p2-p0);
		*pOut = (2 * p0 + v0 - 2 * p1 + v1)*pow(t, 3) + (-3 * p0 - 2 * v0 + 3 * p1 - v1)*pow(t, 2) + v0*t + p0;
		return true;
	}

	template<class VAL, typename DIS, typename WHT>
	typename SplineInterpolite<VAL, DIS, WHT>::TRIO** SplineInterpolite<VAL, DIS, WHT>::getPtrAry(){
		return (TRIO**)&points;
	}
}


#endif
