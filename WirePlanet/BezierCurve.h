#ifndef DEF_BEZIERCURVE_H
#define DEF_BEZIERCURVE_H

/*�x�W�F�Ȑ�
2014/06/24 Version1.0
*/

#include<vector>

namespace otd{

	/**�x�W�F�Ȑ�<�ʒu�̌^>*/
	template<class P>
	class BezierCurve{
	public:
		/**�C�e���[�^*/
		class iterator{
		public:
			iterator();
			iterator(int, BezierCurve*);
			P operator *()const;
			const iterator& operator ++();
			iterator operator ++(int);
			const iterator& operator --();
			iterator operator --(int);
			bool operator ==(const iterator&)const;
			bool operator !=(const iterator&)const;
		private:
			BezierCurve* _bCurve;
			int _currentFrame;
			P _currentPos;
			void progressFrame(int);
		};
		/**
		* @brief �R���X�g���N�^
		* @param [�ړ�����]
		*/
		BezierCurve(int = 60);
		/**
		* @brief �R�s�[�R���X�g���N�^
		*/
		BezierCurve(const BezierCurve&);
		/**
		* @brief ���[�u�R���X�g���N�^
		*/
		BezierCurve(BezierCurve&&);
		/**
		* @brief �f�X�g���N�^
		*/
		~BezierCurve();
		/**
		* @brief ���
		*/
		const BezierCurve& operator=(const BezierCurve&);
		/**
		* @brief ���[�u
		*/
		const BezierCurve& operator=(BezierCurve&&);
		/**
		* @brief �擪�C�e���[�^�擾
		* @return �擪���w���C�e���[�^
		*/
		iterator begin();
		/**
		* @brief �����C�e���[�^�擾
		* @return �������w���C�e���[�^
		*/
		iterator end();
		/**
		* @brief ����_�ǉ�
		* @param ����_
		*/
		void push_back(const P&);
		/**
		* @brief ����_�ǉ�
		* @param ����_
		*/
		void push_back(P&&);
		/**
		* @brief ����_�S�폜
		*/
		void clear();
	private:
		std::vector<P> _points; //����_
		std::vector<int> _combination;
		int _totalFrame; //�ړ�����
	};

	/*BezierCureve*/

	//�R���X�g���N�^
	template<class P>
	BezierCurve<P>::BezierCurve(int totalFrame):_totalFrame(totalFrame){
	}
	//�R�s�[�R���X�g���N�^
	template<class P>
	BezierCurve<P>::BezierCurve(const BezierCurve& obj) :_points(obj._points),_totalFrame(obj._totalFrame),_combination(obj._combination){
	}
	//���[�u�R���X�g���N�^
	template<class P>
	BezierCurve<P>::BezierCurve(BezierCurve&& obj) : _points(std::move(obj._points)),_totalFrame(obj._totalFrame),_combination(std::move(obj._combination)){
		obj._totalFrame = 60;
	}

	//�f�X�g���N�^
	template<class P>
	BezierCurve<P>::~BezierCurve(){
	}

	//�R�s�[
	template<class P>
	const BezierCurve<P>& BezierCurve<P>::operator=(const BezierCurve& obj){
		_points = obj._points;
		_totalFrame = obj._totalFrame;
		_combination = obj._combination;
	}

	//���[�u
	template<class P>
	const BezierCurve<P>& BezierCurve<P>::operator=(BezierCurve&& obj){
		_points = std::move(obj._points);
		_combination = std::move(obj._combination);
		_totalFrame = obj._totalFrame;
		obj._totalFrame = 60;
	}

	//begin
	template<class P>
	typename BezierCurve<P>::iterator BezierCurve<P>::begin(){
		return std::move(iterator(0, this));
	}

	//end
	template<class P>
	typename BezierCurve<P>::iterator BezierCurve<P>::end(){
		return std::move(iterator(_totalFrame, this));
	}

	//push_back
	template<class P>
	void BezierCurve<P>::push_back(const P& point){
		_points.push_back(point);
		//combination�v�Z
		if (_combination.size() == 0){
			_combination.push_back(1);
		}
		while (_combination.size()<_points.size()){
			std::vector<int> n_comb;
			n_comb.push_back(_combination[0]);
			int i;
			for (i = 0; i < (int)_combination.size()-1; ++i){
				n_comb.push_back(_combination[i] + _combination[i+1]);
			}
			n_comb.push_back(_combination[i]);
			_combination = std::move(n_comb);
		}
	}
	template<class P>
	void BezierCurve<P>::push_back(P&& point){
		_points.push_back(std::move(point));
		//combination�v�Z
		if (_combination.size() == 0){
			_combination.push_back(1);
		}
		while (_combination.size()<_points.size()){
			std::vector<int> n_comb;
			n_comb.push_back(_combination[0]);
			int i;
			for (i = 0; i < (int)_combination.size() - 1; ++i){
				n_comb.push_back(_combination[i] + _combination[i + 1]);
			}
			n_comb.push_back(_combination[i]);
			_combination = std::move(n_comb);
		}
	}

	//clear
	template<class P>
	void BezierCurve<P>::clear(){
		_points.clear();
		_combination.clear();
	}

	/*�C�e���[�^*/

	//�R���X�g���N�^
	template<class P>
	BezierCurve<P>::iterator::iterator() :_currentFrame(0), _bCurve(nullptr){
	}
	template<class P>
	BezierCurve<P>::iterator::iterator(int currentFrame, BezierCurve* bCurve) :_currentFrame(currentFrame), _bCurve(bCurve){
		progressFrame(0);
	}

	//�ԐڎQ�Ɖ��Z�q
	template<class P>
	P BezierCurve<P>::iterator::operator*()const{
		return std::move(_currentPos);
	}

	//�C���N�������g,�f�N�������g
	template<class P>
	const typename BezierCurve<P>::iterator& BezierCurve<P>::iterator::operator++(){
		progressFrame(1);
		return *this;
	}
	template<class P>
	typename BezierCurve<P>::iterator BezierCurve<P>::iterator::operator++(int){
		iterator it = *this;
		progressFrame(-1);
		return std::move(it);
	}
	template<class P>
	const typename BezierCurve<P>::iterator& BezierCurve<P>::iterator::operator--(){
		progressFrame(--_currentFrame);
		return *this;
	}
	template<class P>
	typename BezierCurve<P>::iterator BezierCurve<P>::iterator::operator--(int){
		iterator it = *this;
		progressFrame(--_currentFrame);
		return std::move(it);
	}

	//��r���Z�q
	template<class P>
	bool BezierCurve<P>::iterator::operator==(const iterator& obj)const{
		return _currentFrame == obj._currentFrame;
	}
	template<class P>
	bool BezierCurve<P>::iterator::operator!=(const iterator& obj)const{
		return _currentFrame != obj._currentFrame;
	}

	//�t���[����i�߂�
	template<class P>
	void BezierCurve<P>::iterator::progressFrame(int pf){
		_currentFrame += pf;
		if (_currentFrame >= _bCurve->_totalFrame){ _currentFrame = _bCurve->_totalFrame; }
		if (_currentFrame < 0){ _currentFrame = 0; }
		double t = (double)_currentFrame / (double)_bCurve->_totalFrame;
		P res;
		for (int i = 0; i < (int)_bCurve->_points.size(); ++i){
			res += _bCurve->_combination[i] * _bCurve->_points[i] * pow(1.0 - t, _bCurve->_points.size() - 1 - i)*pow(t, i);
		}
		_currentPos = std::move(res);
	}
}

#endif