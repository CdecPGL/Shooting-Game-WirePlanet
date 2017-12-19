#ifndef DEF_HERMITECURVE_H
#define DEF_HERMITECURVE_H

/*エルミート曲線
2014/06/24 Version1.0
*/

#include<utility>

namespace otd{

	/**エルミート曲線<位置の型,速度の型>*/
	template<class P, class V = P>
	class HermiteCurve{
	public:
		/**イテレータ*/
		class iterator{
		public:
			iterator();
			iterator(int, HermiteCurve*);
			P operator *()const;
			const iterator& operator ++();
			iterator operator ++(int);
			const iterator& operator --();
			iterator operator --(int);
			bool operator ==(const iterator&)const;
			bool operator !=(const iterator&)const;
		private:
			HermiteCurve* _hCurve;
			int _currentFrame;
			P _currentPos;
			void progressFrame(int);
		};
		/**
		* @brief コンストラクタ
		*/
		HermiteCurve();
		/**
		* @brief コンストラクタ
		* @param 始点,終点[,移動時間]
		*/
		HermiteCurve(const P&, const P&, int = 60);
		/**
		* @brief コンストラクタ
		* @param 始点,始点の速度ベクトル,終点,終点の速度ベクトル[,移動時間]
		*/
		HermiteCurve(const P&, const V&, const P&, const V&, int = 60);
		/**
		* @brief コピーコンストラクタ
		*/
		HermiteCurve(const HermiteCurve&);
		/**
		* @brief ムーブコンストラクタ
		*/
		HermiteCurve(HermiteCurve&&);
		/**
		* @brief デストラクタ
		*/
		~HermiteCurve();
		/**
		* @brief 代入
		*/
		const HermiteCurve& operator=(const HermiteCurve&);
		/**
		* @brief ムーブ
		*/
		const HermiteCurve& operator=(HermiteCurve&&);
		/**
		* @brief 先頭イテレータ取得
		* @return 先頭を指すイテレータ
		*/
		iterator begin();
		/**
		* @brief 末尾イテレータ取得
		* @return 末尾を指すイテレータ
		*/
		iterator end();
	private:
		P* _sPos; //始点位置
		V* _sVel; //始点速度
		P* _ePos; //終点位置
		V* _eVel; //終点速度
		int _totalFrame; //移動時間
	};

	/*HermiteCureve*/

	//コンストラクタ
	template<class P, class V>
	HermiteCurve<P, V>::HermiteCurve() :_sPos(new P), _ePos(new P), _sVel(new V), _eVel(new V), _totalFrame(60){
	}
	template<class P, class V>
	HermiteCurve<P, V>::HermiteCurve(const P& s_pos, const P& e_pos, int total_frame) : _sPos(new P(s_pos)), _ePos(new P(e_pos)), _sVel(new V), _eVel(new V), _totalFrame(total_frame){
	}
	template<class P, class V>
	HermiteCurve<P, V>::HermiteCurve(const P& s_pos, const V& s_vel, const P& e_pos, const V& e_vel, int total_frame) : _sPos(new P(s_pos)), _ePos(new P(e_pos)), _sVel(new V(s_vel)), _eVel(new V(e_vel)), _totalFrame(total_frame){
	}
	//コピーコンストラクタ
	template<class P, class V>
	HermiteCurve<P, V>::HermiteCurve(const HermiteCurve& obj) : _sPos(new P(*obj._sPos)), _ePos(new P(*obj._ePos)), _sVel(new V(*obj._sVel)), _eVel(new V(*obj._eVel)), _totalFrame(obj._totalFrame){
	}
	//ムーブコンストラクタ
	template<class P, class V>
	HermiteCurve<P, V>::HermiteCurve(HermiteCurve&& obj) : _sPos(obj._sPos), _ePos(obj._ePos), _sVel(obj._sVel), _eVel(obj._eVel), _totalFrame(obj._totalFrame){
		delete obj._sPos;
		delete obj._ePos;
		delete obj._sVel;
		delete obj._eVel;
		obj._sPos = new P;
		obj._ePos = new P;
		obj._sVel = new V;
		obj._eVel = new V;
		obj._totalFrame = 60;
	}

	//デストラクタ
	template<class P, class V>
	HermiteCurve<P, V>::~HermiteCurve(){
		delete _sPos;
		delete _ePos;
		delete _sVel;
		delete _eVel;
	}

	//コピー
	template<class P, class V>
	const HermiteCurve<P, V>& HermiteCurve<P, V>::operator=(const HermiteCurve& obj){
		delete _sPos;
		delete _ePos;
		delete _sVel;
		delete _eVel;
		_sPos = new P(*obj._sPos);
		_ePos = new P(*obj._ePos);
		_sVel = new P(*obj._sVel);
		_eVel = new P(*obj._eVel);
		_totalFrame = obj._totalFrame;
	}

	//ムーブ
	template<class P, class V>
	const HermiteCurve<P, V>& HermiteCurve<P, V>::operator=(HermiteCurve&& obj){
		_sPos = obj._sPos;
		_ePos = obj._ePos;
		_sVel = obj._sVel;
		_eVel = obj._eVel;
		_totalFrame = obj._totalFrame;
		delete obj._sPos;
		delete obj._ePos;
		delete obj._sVel;
		delete obj._eVel;
		obj._sPos = new P;
		obj._ePos = new P;
		obj._sVel = new V;
		obj._eVel = new V;
		obj._totalFrame = 60;
	}

	//begin
	template<class P, class V>
	typename HermiteCurve<P, V>::iterator HermiteCurve<P, V>::begin(){
		return std::move(iterator(0, this));
	}

	//end
	template<class P, class V>
	typename HermiteCurve<P, V>::iterator HermiteCurve<P, V>::end(){
		return std::move(iterator(_totalFrame, this));
	}

	/*イテレータ*/

	//コンストラクタ
	template<class P, class V>
	HermiteCurve<P, V>::iterator::iterator() :_currentFrame(0), _hCurve(nullptr){
	}
	template<class P, class V>
	HermiteCurve<P, V>::iterator::iterator(int currentFrame, HermiteCurve* hCurve) :_currentFrame(currentFrame), _hCurve(hCurve){
		progressFrame(0);
	}

	//間接参照演算子
	template<class P, class V>
	P HermiteCurve<P, V>::iterator::operator*()const{
		return std::move(_currentPos);
	}

	//インクリメント,デクリメント
	template<class P, class V>
	const typename HermiteCurve<P, V>::iterator& HermiteCurve<P, V>::iterator::operator++(){
		progressFrame(1);
		return *this;
	}
	template<class P, class V>
	typename HermiteCurve<P, V>::iterator HermiteCurve<P, V>::iterator::operator++(int){
		iterator it = *this;
		progressFrame(-1);
		return std::move(it);
	}
	template<class P, class V>
	const typename HermiteCurve<P, V>::iterator& HermiteCurve<P, V>::iterator::operator--(){
		progressFrame(--_currentFrame);
		return *this;
	}
	template<class P, class V>
	typename HermiteCurve<P, V>::iterator HermiteCurve<P, V>::iterator::operator--(int){
		iterator it = *this;
		progressFrame(--_currentFrame);
		return std::move(it);
	}

	//比較演算子
	template<class P, class V>
	bool HermiteCurve<P, V>::iterator::operator==(const iterator& obj)const{
		return _currentFrame == obj._currentFrame;
	}
	template<class P, class V>
	bool HermiteCurve<P, V>::iterator::operator!=(const iterator& obj)const{
		return _currentFrame != obj._currentFrame;
	}

	//フレームを進める
	template<class P, class V>
	void HermiteCurve<P, V>::iterator::progressFrame(int pf){
		_currentFrame += pf;
		if (_currentFrame >= _hCurve->_totalFrame){ _currentFrame = _hCurve->_totalFrame; }
		if (_currentFrame < 0){ _currentFrame = 0; }
		P& p0(*_hCurve->_sPos);
		P& p1(*_hCurve->_ePos);
		V& v0(*_hCurve->_sVel);
		V& v1(*_hCurve->_eVel);
		double t = (double)_currentFrame / (double)_hCurve->_totalFrame;
		_currentPos = (2 * p0 + v0 - 2 * p1 + v1)*pow(t, 3) + (-3 * p0 - 2 * v0 + 3 * p1 - v1)*pow(t, 2) + v0*t + p0;
	}
}

#endif