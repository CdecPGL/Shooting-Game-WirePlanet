#ifndef DEF_INTERPOLITESTRATEGY_H
#define DEF_INTERPOLITESTRATEGY_H

#include<tuple>
#include<cassert>

namespace otd{
	//補間ストラテジクラス
	template<class VAL, typename DIS = double, typename WHT = double>
	class InterpoliteStrategy{
	protected:
		using TRIO = std::tuple < VAL, DIS, WHT >;
	public:
		/**
		* @brief デストラクタ
		*/
		virtual ~InterpoliteStrategy() = default;
		/**
		* @brief 必要な点の数を取得
		*/
		virtual size_t needPointNum() = 0;
		/**
		* @brief 補間値を計算
		* @param 補間結果,補間割合
		*/
		virtual bool calc(VAL*, const DIS&) = 0;
		/**
		* @brief 点を登録する配列を取得
		* @return 点を登録する配列を取得のポインタ
		*/
		virtual TRIO** getPtrAry() = 0;
	};

	//線形補間クラス
	template<class VAL, typename DIS = double, typename WHT = double>
	class LinearInterpolite : InterpoliteStrategy < VAL, DIS, WHT> {
	public:
		/**
		* @brief コンストラクタ
		*/
		LinearInterpolite();
		/**
		* @brief デストラクタ
		*/
		~LinearInterpolite()override;
		/**
		* @brief 必要な点の数を取得
		*/
		virtual size_t needPointNum()override;
		/**
		* @brief 線形補間値を計算
		* @param 補間結果,補間割合
		*/
		bool calc(VAL*, const DIS&)override;
		/**
		* @brief 点を登録する配列を取得
		* @return 点を登録する配列を取得のポインタ
		*/
		TRIO** getPtrAry()override;
	protected:
		TRIO* points[2]; //キー配列ポインタ
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
		return 2; //線形補間には2つの値が必要
	}

	template<class VAL, typename DIS, typename WHT>
	bool LinearInterpolite<VAL, DIS, WHT>::calc(VAL* pOut,const DIS& t){
		// 線形補間値を計算
		*pOut = (std::get<0>(*points[0]))*(1 - t) + (std::get<0>(*points[1]))*t;
		return true;
	}

	template<class VAL, typename DIS, typename WHT>
	typename LinearInterpolite<VAL, DIS, WHT>::TRIO** LinearInterpolite<VAL, DIS, WHT>::getPtrAry(){
		return (TRIO**)&points;
	}

	//スプライン補間クラス
	template<class VAL, typename DIS = double, typename WHT = double>
	class SplineInterpolite : InterpoliteStrategy < VAL, DIS, WHT> {
	public:
		/**
		* @brief コンストラクタ
		*/
		SplineInterpolite();
		/**
		* @brief デストラクタ
		*/
		~SplineInterpolite()override;
		/**
		* @brief 必要な点の数を取得
		*/
		virtual size_t needPointNum()override;
		/**
		* @brief 線形補間値を計算
		* @param 補間結果,補間割合
		*/
		bool calc(VAL*, const DIS&)override;
		/**
		* @brief 点を登録する配列を取得
		* @return 点を登録する配列を取得のポインタ
		*/
		TRIO** getPtrAry()override;
	protected:
		TRIO* points[3]; //キー配列ポインタ
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
		return 3; //スプライン補間には3つの値が必要
	}

	template<class VAL, typename DIS, typename WHT>
	bool SplineInterpolite<VAL, DIS, WHT>::calc(VAL* pOut, const DIS& t){
		// スプライン補間値を計算
		assert(false); //未実装なので使えない
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
