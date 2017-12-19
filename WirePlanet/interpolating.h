#ifndef DEF_INTERPOLITING_H
#define DEF_INTERPOLITING_H

/*��ԕ�ԃe���v���[�g�N���X(http://marupeke296.com/DXCLS_InterpolatingTemplate.html���Q��)
* 2014/06/22 Version 1.0
*/

#include"InterPoliteStrategy.h"

#define INTERPOLATING_ENDPTR  ((TRIO*)(-1))

namespace otd{

	//��ԕ�ԃe���v���[�g�N���X<��Ԃ���f�[�^�̌^,�����̌^,�d�݂̌^,��ԃX�g���e�W�N���X>
	template <class VAL, typename DIS = double, typename WHT = double, class STG = LinearInterpolite<VAL, DIS, WHT>>
	class interpoliting{
	public:
		using TRIO = std::tuple < VAL, DIS, WHT >;
		/**
		* @brief �C�e���[�^�N���X
		*/
		class iterator{
		public:
			/**
			* @brief �R���X�g���N�^
			* @param �_�̃|�C���^,�Ăяo����interpoliting�̃|�C���^
			*/
			iterator(TRIO** = NULL, interpoliting* = NULL);
			/**
			* @brief �C�e���[�^�̍X�V
			* @param �i�߂鋗��
			* @return �C�e���[�^
			*/
			iterator next(DIS&);
			/**
			* @brief ��ԊԊu�ύX
			* @param ��ԊԊu
			*/
			void set_unit(const DIS&);
			/**
			* @brief �Ԑډ��Z�q
			* @return �l
			*/
			const VAL& operator *()const;
			/**
			* @brief ��u�C���N�������g
			*/
			iterator operator ++(int);
			/**
			* @brief �O�u�C���N�������g
			*/
			iterator operator ++();
			/**
			* @brief ����
			*/
			bool operator ==(const iterator &src)const;
			/**
			* @brief �s����
			*/
			bool operator !=(const iterator &src)const;
		protected:
			DIS update(DIS); // ���̈ʒu�ƕW�����������Z�o
			bool setPoint(size_t, TRIO**); // ��Ԍv�Z�ɕK�v�ȓ_���i�[
			void setTerminate();

		private:
			TRIO** _ppFirst;          // �C�e���[�g�̍ŏ��̃|�C���^�ʒu
			interpoliting* _pInterp; // �A�N�Z�X����interpolating�I�u�W�F�N�g
			VAL _val;                // ���݂̕�Ԓl
			DIS _cx;                   // ���݂̋���
			DIS _cum;                  // �ݐϋ���
			DIS _unit;                 // �P��
			size_t _offset;          // �I�t�Z�b�g�l
			size_t _max;             // �ő�i�[��
			size_t _curPos;          // ���݂̈ʒu
			TRIO *_pCur;           // ���݂̓_�ւ̃|�C���^
			bool _finished;         // �I���ς݃t���O
			STG _strategy;      // ��Ԍv�Z���@

		};
		/**
		* @brief �R���X�g���N�^
		*/
		interpoliting();
		/**
		* @brief �f�X�g���N�^
		*/
		~interpoliting();
		/**
		* @brief �R�s�[�R���X�g���N�^
		*/
		interpoliting(const interpoliting&);
		/**
		* @brief ���[�u�R���X�g���N�^
		*/
		interpoliting(interpoliting&&);
		/**
		* @brief ������Z�q
		*/
		interpoliting& operator =(const interpoliting&);
		/**
		* @brief ���[�u���Z�q
		*/
		interpoliting& operator =(interpoliting&&);
		/**
		* @brief �����ɓ_��ǉ�
		* @param �l�A�����A�d��
		*/
		void push_back(VAL&, DIS&, WHT&);
		/**
		* @brief �����ɓ_��ǉ�
		* @param �l�A�����A�d��
		*/
		void push_back(VAL&, DIS, WHT);
		/**
		* @brief �����ɓ_��ǉ�
		* @param �l�A�����A�d��
		*/
		void push_back(VAL, DIS, WHT = 1);
		/**
		* @brief �擪�C�e���[�^�擾
		* @return �擪�C�e���[�^
		*/
		iterator begin();
		/**
		* @brief �����C�e���[�^�擾
		* @return �����C�e���[�^
		*/
		iterator end();
		/**
		* @brief ����
		*/
		void clear();
	protected:
		int _size;
		TRIO** _ppTrio;
		TRIO** _pLast;

		void addArrayMemory(size_t);
		void rewrite(TRIO**, TRIO *); // �|�C���^�̏㏑��
		TRIO** getFirstPtr();
		size_t size()const;
	};

	//interpoliting

	template <class VAL, typename DIS, typename WHT, class STG >
	interpoliting<VAL, DIS, WHT, STG>::interpoliting(){
		_size = 0;
		_ppTrio = (TRIO**)malloc(sizeof(TRIO*));
		*_ppTrio = INTERPOLATING_ENDPTR;
		if (_ppTrio){ _pLast = _ppTrio; }
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	interpoliting<VAL, DIS, WHT, STG>::~interpoliting()
	{
		clear();
		free(_ppTrio);
	}
	//�R�s�[�R���X�g���N�^(���e�X�g)
	template <class VAL, typename DIS, typename WHT, class STG >
	interpoliting<VAL, DIS, WHT, STG>::interpoliting(const interpoliting& obj){
		_size = obj._size;
		_ppTrio = (TRIO**)malloc(sizeof(TRIO*)*_size);
		for (size_t i = 0; i < _size; ++i){
			**(_ppTrio + i) = **(obj._ppTrio + i);
		}
		_pLast = _ppTrio + _size - 1;

	}

	template <class VAL, typename DIS, typename WHT, class STG >
	interpoliting<VAL, DIS, WHT, STG>::interpoliting(interpoliting&& obj){
		_size = obj._size;
		_ppTrio = obj._ppTrio;
		_pLast = obj._pLast;

		obj.pTrio = (TRIO**)malloc(sizeof(TRIO*));
		*obj._ppTrio = INTERPOLATING_ENDPTR;
		obj._pLast = obj._ppTrio;
	}
	//������Z�q(���e�X�g)
	template <class VAL, typename DIS, typename WHT, class STG >
	interpoliting<VAL, DIS, WHT, STG>& interpoliting<VAL, DIS, WHT, STG>::operator =(const interpoliting& obj){
		_size = obj._size;
		_ppTrio = (TRIO**)realloc(_ppTrio, sizeof(TRIO*));
		for (size_t i = 0; i < _size; ++i){
			*(_ppTrio + i)=
			**(_ppTrio + i) = **(obj._ppTrio + i);
		}
		_pLast = _ppTrio + _size - 1;

		return *this;
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	interpoliting<VAL, DIS, WHT, STG>& interpoliting<VAL, DIS, WHT, STG>::operator =(interpoliting&& obj){
		clear();
		free(_ppTrio);

		_size = obj._size;
		_ppTrio = obj._ppTrio;
		_pLast = obj._pLast;

		obj.pTrio = (TRIO**)malloc(sizeof(TRIO*));
		*obj._ppTrio = INTERPOLATING_ENDPTR;
		obj._pLast = obj._ppTrio;

		return *this;
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	void interpoliting<VAL, DIS, WHT, STG>::push_back(VAL& val, DIS& dis, WHT& weight){
		// �g���I�𐶐�
		TRIO* p = new TRIO(val, dis, weight);
		// �z���1�g��
		addAryMemory(1);
		// �ǉ������z�񕔕��ɒl���i�[
		rewrite(_pLast, p);
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	void interpoliting<VAL, DIS, WHT, STG>::push_back(VAL& val, DIS dis, WHT weight){
		// �g���I�𐶐�
		TRIO* p = new TRIO(val, std::move(dis), std::move(weight));
		// �z���1�g��
		addAryMemory(1);
		// �ǉ������z�񕔕��ɒl���i�[
		rewrite(_pLast, p);
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	void interpoliting<VAL, DIS, WHT, STG>::push_back(VAL val, DIS dis, WHT weight){
		// �g���I�𐶐�
		TRIO* p = new TRIO(std::move(val), std::move(dis), std::move(weight));
		// �z���1�g��
		addArrayMemory(1);
		// �ǉ������z�񕔕��ɒl���i�[
		rewrite(_pLast, p);
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	typename interpoliting<VAL, DIS, WHT, STG>::iterator interpoliting<VAL, DIS, WHT, STG>::begin(){
		return iterator(_ppTrio, this);
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	typename interpoliting<VAL, DIS, WHT, STG>::iterator interpoliting<VAL, DIS, WHT, STG>::end(){
		return iterator(NULL, this);
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	void interpoliting<VAL, DIS, WHT, STG>::clear(){
		// �m�ۂ�����������S�ăN���A����
		size_t i;
		for (i = 0; i < (unsigned int)_size; i++)
		{
			delete _ppTrio[i];
		}
		_size = 0;
		_ppTrio = (TRIO**)realloc(_ppTrio, sizeof(TRIO*));
		if (_ppTrio){
			*_ppTrio = INTERPOLATING_ENDPTR;
			_pLast = _ppTrio;
		}
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	void interpoliting<VAL, DIS, WHT, STG>::addArrayMemory(size_t cont){
		if (cont > 0){
			// �w��̑傫���Ƀ|�C���^�z����g��
			_ppTrio = (TRIO**)realloc(_ppTrio, sizeof(TRIO*)*(_size + cont));
//			if (_ppTrio == nullptr){ throw std::domain_error(); }
			// �g�����������̒l��������
			size_t i;
			for (i = 0; i < cont; i++)
				_ppTrio[_size + i] = INTERPOLATING_ENDPTR;
			// �T�C�Y��ύX
			_size += cont;
			// �ŏI�|�C���^�ʒu�̐ݒ��ύX
			_pLast = _ppTrio + _size - 1;
		}
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	void interpoliting<VAL, DIS, WHT, STG>::rewrite(TRIO** pPlace, TRIO *val){
		// �㏑�������̃|�C���^����폜
		if (*pPlace != INTERPOLATING_ENDPTR)
			delete *pPlace;
		*pPlace = val;
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	typename interpoliting<VAL, DIS, WHT, STG>::TRIO** interpoliting<VAL, DIS, WHT, STG>::getFirstPtr(){
		return _ppTrio;
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	size_t interpoliting<VAL, DIS, WHT, STG>::size()const{
		return _size;
	}

	//iterator

	template <class VAL, typename DIS, typename WHT, class STG >
	interpoliting<VAL, DIS, WHT, STG>::iterator::iterator(TRIO** ppTrio, interpoliting* pInterp){
		// �Ƃ肠�����G���[�̏o�Ȃ���Ԃɂ���
		_pInterp = pInterp;
		_offset = 0;
		_max = 0;
		_finished = true;      // �I�[�ɒB���Ă�����
		_unit = 1;             // 1�P�ʂɏ�����
		_cx = 0;               // ��������
		_cum = 0;              // ������������
		_ppFirst = ppTrio;     // �ŏ��̃|�C���^�ʒu
		_curPos = 0;           // ��ԏo���Ȃ����
		_pCur = INTERPOLATING_ENDPTR; // �I�[�|�C���^�ɂ���

		// �z��̗L�������`�F�b�N
		if (ppTrio != NULL)
		{
			// �I�t�Z�b�g���A�ő�X�V���i���_�̐��j�A���݂̓_�̈ʒu��������
			_offset = ((size_t)ppTrio - (size_t)pInterp->getFirstPtr()) / sizeof(TRIO*); // �擪����̃I�t�Z�b�g��
			_max = pInterp->size() - _offset;   // �ő�X�V��
			_val = std::get<0>(*ppTrio[0]);             // �����l
			// �_�̌����K��ȏ�ł���΂Ƃ肠������ԉ\
			if (_max >= _strategy.needPointNum())
			{
				_curPos = 1;               // �ŏ��̖ڕW�_��2�_�ڂɂ���̂�1�ɂ���
				_pCur = _ppFirst[_curPos]; // 2�_�ڂɂ���
				_finished = false;         // �I�[�ɒB���Ă��Ȃ���ԂɕύX
			}
		}
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	typename interpoliting<VAL, DIS, WHT, STG>::iterator interpoliting<VAL, DIS, WHT, STG>::iterator::next(DIS& dis){
		// ���̋����֐i�߁A�W�����������`
		DIS norm = update(dis);

		// ��Ԍv�Z�ɕK�v�ȓ_�̐����擾
		size_t pn = _strategy.needPointNum();

		// �w��̓_�̊m��
		TRIO** _ppPoint = _strategy.getPtrAry(); // ��ԓ_�|�C���^���i�[����z��
		if (!setPoint(pn, _ppPoint)){
			// �ŏI�t���O�������Ă�����
			// �I�[�C�e���[�^��Ԃ�
			if (_finished || *_ppFirst == INTERPOLATING_ENDPTR)
			{
				// ���łɏI�����Ă�����
				setTerminate(); // �I�[�C�e���[�^�ɐݒ�
				return *this;
			}
			else
			{
				// �Ō�̓_�ɍ��킹��
				_finished = true;
				_pCur = _ppFirst[_pInterp->size() - 1];
				_val = std::get<0>(*_pCur);
				return *this;
			}
		}
		// ���̒l�ɍX�V
		_strategy.calc(&_val, norm);
		return *this;
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	void interpoliting<VAL, DIS, WHT, STG>::iterator::set_unit(const DIS& u){
		_unit = u;
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	DIS interpoliting<VAL, DIS, WHT, STG>::iterator::update(DIS dis){
		if (*_ppFirst == INTERPOLATING_ENDPTR)
			return 1;

		if (_pInterp->size() <= 1) // 1�_�ł͕�Ԃł��Ȃ�
			return 1; // �ŏI�_���w���悤�ɂ���

		_cx += dis; // ���̈ʒu�ֈړ�
		_cum += dis;
		// ���̋����̏�����Ԃ��m��
		while (1)
		{
			// ���̈ʒu�����݂��邩�`�F�b�N
			if (_curPos >= _max) // ���݂��Ȃ�
				return 1;
			// ���̈ʒu��ʂ�߂��Ă����玟�̓_��
			if (_cx >= std::get<1>(*_ppFirst[_offset + _curPos]))
			{
				_cx -= std::get<1>(*_ppFirst[_offset + _curPos]); // ��яo�������������Z�o
				_curPos++;
				continue;
			}
			break;
		}

		// ��-- ���݂̋�Ԉʒu���m�� --�� //
		_pCur = _ppFirst[_offset + _curPos];

		// ���̓_�ɑ΂���W�����������Z�o
		return _cx / std::get<1>(*_pCur);
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	bool interpoliting<VAL, DIS, WHT, STG>::iterator::setPoint(size_t num, TRIO** ary){
		size_t lastPointNum = (_curPos - 1) + num;
		if (lastPointNum > _max)
			return false; // �K�v�ȓ_���m�ۏo���Ȃ�

		// �O�̓_����_�ɓ_���R�s�[
		memcpy(ary, _ppFirst + _curPos - 1, num * sizeof(TRIO*));
		return true;
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	void interpoliting<VAL, DIS, WHT, STG>::iterator::setTerminate(){
		// �I�[��Ԃ͌��݂̃|�C���^���I�[�|�C���^�ɂȂ�
		_pCur = INTERPOLATING_ENDPTR;
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	const VAL& interpoliting<VAL, DIS, WHT, STG>::iterator::operator*()const{
		return _val;
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	typename interpoliting<VAL, DIS, WHT, STG>::iterator interpoliting<VAL, DIS, WHT, STG>::iterator::operator++(int dummy){
		iterator it = *this;
		next(_unit);
		return _unit;
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	typename interpoliting<VAL, DIS, WHT, STG>::iterator interpoliting<VAL, DIS, WHT, STG>::iterator::operator++(){
		return next(_unit);
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	bool interpoliting<VAL, DIS, WHT, STG>::iterator::operator==(const iterator &src)const{
		return src._pCur == _pCur;
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	bool interpoliting<VAL, DIS, WHT, STG>::iterator::operator!=(const iterator &src)const{
		return src._pCur != _pCur;
	}
}

#endif

