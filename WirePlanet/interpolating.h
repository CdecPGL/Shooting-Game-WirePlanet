#ifndef DEF_INTERPOLITING_H
#define DEF_INTERPOLITING_H

/*区間補間テンプレートクラス(http://marupeke296.com/DXCLS_InterpolatingTemplate.htmlを参照)
* 2014/06/22 Version 1.0
*/

#include"InterPoliteStrategy.h"

#define INTERPOLATING_ENDPTR  ((TRIO*)(-1))

namespace otd{

	//区間補間テンプレートクラス<補間するデータの型,距離の型,重みの型,補間ストラテジクラス>
	template <class VAL, typename DIS = double, typename WHT = double, class STG = LinearInterpolite<VAL, DIS, WHT>>
	class interpoliting{
	public:
		using TRIO = std::tuple < VAL, DIS, WHT >;
		/**
		* @brief イテレータクラス
		*/
		class iterator{
		public:
			/**
			* @brief コンストラクタ
			* @param 点のポインタ,呼び出し元interpolitingのポインタ
			*/
			iterator(TRIO** = NULL, interpoliting* = NULL);
			/**
			* @brief イテレータの更新
			* @param 進める距離
			* @return イテレータ
			*/
			iterator next(DIS&);
			/**
			* @brief 補間間隔変更
			* @param 補間間隔
			*/
			void set_unit(const DIS&);
			/**
			* @brief 間接演算子
			* @return 値
			*/
			const VAL& operator *()const;
			/**
			* @brief 後置インクリメント
			*/
			iterator operator ++(int);
			/**
			* @brief 前置インクリメント
			*/
			iterator operator ++();
			/**
			* @brief 等号
			*/
			bool operator ==(const iterator &src)const;
			/**
			* @brief 不等号
			*/
			bool operator !=(const iterator &src)const;
		protected:
			DIS update(DIS); // 次の位置と標準化距離を算出
			bool setPoint(size_t, TRIO**); // 補間計算に必要な点を格納
			void setTerminate();

		private:
			TRIO** _ppFirst;          // イテレートの最初のポインタ位置
			interpoliting* _pInterp; // アクセス元のinterpolatingオブジェクト
			VAL _val;                // 現在の補間値
			DIS _cx;                   // 現在の距離
			DIS _cum;                  // 累積距離
			DIS _unit;                 // 単位
			size_t _offset;          // オフセット値
			size_t _max;             // 最大格納数
			size_t _curPos;          // 現在の位置
			TRIO *_pCur;           // 現在の点へのポインタ
			bool _finished;         // 終了済みフラグ
			STG _strategy;      // 補間計算方法

		};
		/**
		* @brief コンストラクタ
		*/
		interpoliting();
		/**
		* @brief デストラクタ
		*/
		~interpoliting();
		/**
		* @brief コピーコンストラクタ
		*/
		interpoliting(const interpoliting&);
		/**
		* @brief ムーブコンストラクタ
		*/
		interpoliting(interpoliting&&);
		/**
		* @brief 代入演算子
		*/
		interpoliting& operator =(const interpoliting&);
		/**
		* @brief ムーブ演算子
		*/
		interpoliting& operator =(interpoliting&&);
		/**
		* @brief 末尾に点を追加
		* @param 値、距離、重み
		*/
		void push_back(VAL&, DIS&, WHT&);
		/**
		* @brief 末尾に点を追加
		* @param 値、距離、重み
		*/
		void push_back(VAL&, DIS, WHT);
		/**
		* @brief 末尾に点を追加
		* @param 値、距離、重み
		*/
		void push_back(VAL, DIS, WHT = 1);
		/**
		* @brief 先頭イテレータ取得
		* @return 先頭イテレータ
		*/
		iterator begin();
		/**
		* @brief 末尾イテレータ取得
		* @return 末尾イテレータ
		*/
		iterator end();
		/**
		* @brief 消去
		*/
		void clear();
	protected:
		int _size;
		TRIO** _ppTrio;
		TRIO** _pLast;

		void addArrayMemory(size_t);
		void rewrite(TRIO**, TRIO *); // ポインタの上書き
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
	//コピーコンストラクタ(未テスト)
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
	//代入演算子(未テスト)
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
		// トリオを生成
		TRIO* p = new TRIO(val, dis, weight);
		// 配列を1つ拡張
		addAryMemory(1);
		// 追加した配列部分に値を格納
		rewrite(_pLast, p);
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	void interpoliting<VAL, DIS, WHT, STG>::push_back(VAL& val, DIS dis, WHT weight){
		// トリオを生成
		TRIO* p = new TRIO(val, std::move(dis), std::move(weight));
		// 配列を1つ拡張
		addAryMemory(1);
		// 追加した配列部分に値を格納
		rewrite(_pLast, p);
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	void interpoliting<VAL, DIS, WHT, STG>::push_back(VAL val, DIS dis, WHT weight){
		// トリオを生成
		TRIO* p = new TRIO(std::move(val), std::move(dis), std::move(weight));
		// 配列を1つ拡張
		addArrayMemory(1);
		// 追加した配列部分に値を格納
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
		// 確保したメモリを全てクリアする
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
			// 指定の大きさにポインタ配列を拡張
			_ppTrio = (TRIO**)realloc(_ppTrio, sizeof(TRIO*)*(_size + cont));
//			if (_ppTrio == nullptr){ throw std::domain_error(); }
			// 拡張した部分の値を初期化
			size_t i;
			for (i = 0; i < cont; i++)
				_ppTrio[_size + i] = INTERPOLATING_ENDPTR;
			// サイズを変更
			_size += cont;
			// 最終ポインタ位置の設定を変更
			_pLast = _ppTrio + _size - 1;
		}
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	void interpoliting<VAL, DIS, WHT, STG>::rewrite(TRIO** pPlace, TRIO *val){
		// 上書き部分のポインタ先を削除
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
		// とりあえずエラーの出ない状態にする
		_pInterp = pInterp;
		_offset = 0;
		_max = 0;
		_finished = true;      // 終端に達している状態
		_unit = 1;             // 1単位に初期化
		_cx = 0;               // 初期距離
		_cum = 0;              // 距離を初期化
		_ppFirst = ppTrio;     // 最初のポインタ位置
		_curPos = 0;           // 補間出来ない状態
		_pCur = INTERPOLATING_ENDPTR; // 終端ポインタにする

		// 配列の有効性をチェック
		if (ppTrio != NULL)
		{
			// オフセット数、最大更新数（＝点の数）、現在の点の位置を初期化
			_offset = ((size_t)ppTrio - (size_t)pInterp->getFirstPtr()) / sizeof(TRIO*); // 先頭からのオフセット数
			_max = pInterp->size() - _offset;   // 最大更新数
			_val = std::get<0>(*ppTrio[0]);             // 初期値
			// 点の個数が規定以上であればとりあえず補間可能
			if (_max >= _strategy.needPointNum())
			{
				_curPos = 1;               // 最初の目標点は2点目にあるので1にする
				_pCur = _ppFirst[_curPos]; // 2点目にする
				_finished = false;         // 終端に達していない状態に変更
			}
		}
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	typename interpoliting<VAL, DIS, WHT, STG>::iterator interpoliting<VAL, DIS, WHT, STG>::iterator::next(DIS& dis){
		// 次の距離へ進め、標準化距離を定義
		DIS norm = update(dis);

		// 補間計算に必要な点の数を取得
		size_t pn = _strategy.needPointNum();

		// 指定の点の確保
		TRIO** _ppPoint = _strategy.getPtrAry(); // 補間点ポインタを格納する配列
		if (!setPoint(pn, _ppPoint)){
			// 最終フラグが立っていたら
			// 終端イテレータを返す
			if (_finished || *_ppFirst == INTERPOLATING_ENDPTR)
			{
				// すでに終了している状態
				setTerminate(); // 終端イテレータに設定
				return *this;
			}
			else
			{
				// 最後の点に合わせる
				_finished = true;
				_pCur = _ppFirst[_pInterp->size() - 1];
				_val = std::get<0>(*_pCur);
				return *this;
			}
		}
		// 次の値に更新
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

		if (_pInterp->size() <= 1) // 1点では補間できない
			return 1; // 最終点を指すようにする

		_cx += dis; // 次の位置へ移動
		_cum += dis;
		// 次の距離の所属区間を確定
		while (1)
		{
			// 今の位置が存在するかチェック
			if (_curPos >= _max) // 存在しない
				return 1;
			// 今の位置を通り過ぎていたら次の点へ
			if (_cx >= std::get<1>(*_ppFirst[_offset + _curPos]))
			{
				_cx -= std::get<1>(*_ppFirst[_offset + _curPos]); // 飛び出た差分距離を算出
				_curPos++;
				continue;
			}
			break;
		}

		// ↑-- 現在の区間位置が確定 --↑ //
		_pCur = _ppFirst[_offset + _curPos];

		// 次の点に対する標準化距離を算出
		return _cx / std::get<1>(*_pCur);
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	bool interpoliting<VAL, DIS, WHT, STG>::iterator::setPoint(size_t num, TRIO** ary){
		size_t lastPointNum = (_curPos - 1) + num;
		if (lastPointNum > _max)
			return false; // 必要な点を確保出来ない

		// 前の点を基点に点をコピー
		memcpy(ary, _ppFirst + _curPos - 1, num * sizeof(TRIO*));
		return true;
	}

	template <class VAL, typename DIS, typename WHT, class STG >
	void interpoliting<VAL, DIS, WHT, STG>::iterator::setTerminate(){
		// 終端状態は現在のポインタが終端ポインタになる
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

