#pragma once

#include"Vector2D.h"
//#include<initializer_list>

template < typename V, int X, int Y > 
struct Matrix{
	Matrix();
	/*Matrix(std::initializer_list<V>);*/
	V element[X][Y];
	const int raw = X;
	const int line = Y;
};

template < typename V, int X, int Y >
Matrix<V, X, Y>::Matrix(){
	for (int i = 0; i < X; ++i){
		for (int j = 0; j < Y; ++j){
			element[i][j] = 0;
		}
	}
}

//template < typename V, int X, int Y >
//Matrix<V, X, Y>::Matrix(std::initializer_list<V> e){
//	if (e.size != Y){ throw std::invalid_argument(); }
//	element = e;
//}

template<typename T>
Vector2D<T> LinearTransformation(const Matrix<T, 2, 2>& m,const Vector2D<T>& v){
	return Vector2D<T>(m.element[0][0] * v.x + m.element[1][0] * v.y, m.element[0][1] * v.x + m.element[1][1] * v.y);
}

template<typename T>
Vector2D<T> RotationalTransformation(double rota, const Vector2D<T>& v){
	Matrix<T, 2, 2> m;
	m.element[0][0] = (T)cos(rota);
	m.element[1][0] = -(T)sin(rota);
	m.element[0][1] = (T)sin(rota);
	m.element[1][1] = (T)cos(rota);
	return LinearTransformation(m,v);
}
