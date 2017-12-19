//vetsion1.2 2013/12/27
//Version1.3 2014/2/8 Vector2D�̌^�L���X�g�ɑΉ�
//Version1.4 2014/2/18 �����̓����擾����GetLength_Square()��ǉ�
//Version1.5 2014/3/18 ToString()��ǉ�
//Version1.6 2014/4/22 �[�����Z�̗�O������ǉ��B�s�������Z�q�̒ǉ�
//Version2.0 2014/4/23 *�O���[�o�����Z�q�ǉ��B*/���Z�q�őΏۂ̃x�N�^�̌^����Ȃ��X�J���^��������悤�ύX
//Version2.1 2014/4/25 �l�̌ܓ��l�����߂�GetRoundValue�֐���ǉ�
//Version2.2 2015/9/5 *=,/=���Z�q��ǉ�
//Version2.3 2015/12/15 inline�ɏ�������

#pragma once
#define _USE_MATH_DEFINES
#include<math.h>
#include<string>
#include<sstream>
#include<exception>
#include<cmath>

class zero_vector_error : public std::domain_error{
public:
	zero_vector_error::zero_vector_error() = default;
	zero_vector_error::zero_vector_error(char* e) :domain_error(e){};
};

//�{���x���������_�񎟌��x�N�g���N���X

template<typename T> class Vector2D{
	public:
	Vector2D() :x(0), y(0) {};
	Vector2D(const Vector2D<T>& obj){ x = obj.x; y = obj.y; }
	template<typename T2>
	Vector2D(const Vector2D<T2>& obj):x((T)obj.x),y((T)obj.y){}
	Vector2D(T m_x, T m_y) : x(m_x), y(m_y) {};
	Vector2D<T>& operator =(const Vector2D<T>& obj){ x = obj.x; y = obj.y; return *this; }
	template<typename T2>
	Vector2D<T>& operator =(const Vector2D<T2>& obj){ x = (T)obj.x; y = (T)obj.y; return *this; }
	Vector2D<T> operator+(const Vector2D<T>& in)const {
		Vector2D<T> out(x + in.x, y + in.y);
		return out;
	}
	Vector2D<T>operator-(const Vector2D<T>& in)const {
		Vector2D<T> out(x - in.x, y - in.y);
		return out;
	}
	template<typename U>
	Vector2D<T> operator*(U num)const {
		Vector2D<T> out((T)(x*num), (T)(y*num));
		return out;
	}
	template<typename U>
	Vector2D<T> operator*=(U num) {
		x *= num;
		y *= num;
		Vector2D<T> out((T)(x*num), (T)(y*num));
		return *this;
	}
	template<typename U>
	Vector2D<T> operator/(U num)const {
		if (num == 0) {
			throw std::range_error("Divided by zero.");
		}
		Vector2D<T> out(static_cast<T>(static_cast<double>(x) / num), static_cast<T>(static_cast<double>(y) / num));
		return out;
	}
	template<typename U>
	Vector2D<T> operator/=(U num) {
		if (num == 0) {
			throw std::range_error("Divided by zero.");
		}
		x /= num;
		y /= num;
		return *this;
	}
	Vector2D<T> operator-()const {
		Vector2D<T> out(-x, -y);
		return out;
	}
	bool operator==(const Vector2D<T>& in)const {
		if (in.x == x && in.y == y) { return true; }
		return false;
	}
	bool operator!=(const Vector2D<T>& in)const {
		return !(*this == in);
	}
	Vector2D<T>& operator+=(const Vector2D<T>& in) {
		x += in.x;
		y += in.y;
		return *this;
	}
	Vector2D<T>& operator-=(const Vector2D<T>& in) {
		x -= in.x;
		y -= in.y;
		return *this;
	}
	Vector2D<T> operator++(int) {
		Vector2D<T> buf = *this;
		++x;
		++y;
		return buf;
	}
	Vector2D<T>& operator++() {
		++x;
		++y;
		return *this;
	}
	Vector2D<T> operator--(int) {
		Vector2D<T> buf = *this;
		--x;
		--y;
		return buf;
	}
	Vector2D<T>& operator--() {
		--x;
		--y;
		return *this;
	}

//	template <typename T2> operator Vector2D<T2>(); //Vector2D�̃L���X�g

	void Set(T mx, T my) {
		x = mx; y = my;
	}
	double Length()const {
		return Length(*this);
	}

	const std::string ToString()const {
		using namespace std;
		stringstream is;
		is << "(" << x << "," << y << ")";
		return is.str();
	}

	T x, y; //�v�f
public:
	static T Dot(Vector2D<T> v1, Vector2D<T> v2) {
		return v1.x*v2.x + v1.y*v2.y;
	}
	static T Cross(Vector2D<T> v1, Vector2D<T> v2) {
		return v1.x*v2.y - v2.x*v1.y;
	}
	static double Length(Vector2D<T> v) {
		return sqrt(pow(v.x, 2) + pow(v.y, 2));
	}
	static double Length_Square(Vector2D<T> v) {
		return pow(v.x, 2) + pow(v.y, 2);
	}
	//���s�ȒP�ʃx�N�g�������߂�(���������̂���)
	static Vector2D<T> GetParallelUnitVector(Vector2D<T> v) {
		double v_length(Length(v));
		if (v_length == 0) { throw zero_vector_error("tryed to calc unit vector from zero vector"); }
		Vector2D<T>out(v / Length(v));
		return out;
	}
	//�����P�ʃx�N�g�������߂�(���v����90�x��]��������)
	static Vector2D<T> GetVerticalUnitVector(Vector2D<T> v) {
		Vector2D<T> v_vtcl(-v.y, v.x);
		double v_length(Length(v_vtcl));
		Vector2D<T> out(GetParallelUnitVector(v_vtcl));
		return out;
	}
	//v2��v1�ƂȂ��p�̃R�T�C���l�����߂�(���v���肪��)
	static double GetCosin(Vector2D<T> v1, Vector2D<T> v2) {
		double v1_length(Length(v1)), v2_length(Length(v2));
		double v1v2_dot(Dot(v1, v2));
		return v1v2_dot / (v1_length*v2_length);
	}
	//v2��v1�ƂȂ��p�̃T�C���l�����߂�(���v���肪��)
	static double GetSin(Vector2D<T> v1, Vector2D<T> v2) {
		double v1_length(Length(v1)), v2_length(Length(v2));
		double v1v2_cross(Cross(v1, v2));
		return v1v2_cross / (v1_length*v2_length);
	}
	//v2��v1�ƂȂ��p�̊p�x(�x)�����߂�(���v���肪��)
	static double GetDegree(Vector2D<T> v1, Vector2D<T> v2) {
		double v1v2_rad = (GetRadian(v1, v2));
		double v1v2_deg(v1v2_rad / M_PI * 180);
		return v1v2_deg;
	}
	//v2��v1�ƂȂ��p�̊p�x(���W�A��)�����߂�(���v���肪��)
	static double GetRadian(Vector2D<T> v1, Vector2D<T> v2) {
		double v1v2_cos(GetCosin(v1, v2));
		double v1v1_sin(GetSin(v1, v2));
		double v1v2_rad(acos(v1v2_cos));
		if (v1v1_sin < 0) { v1v2_rad = -v1v2_rad; }
		return v1v2_rad;
	}
	//v1�̎l�̌ܓ��l���擾
	static Vector2D<T> GetRoundValue(const Vector2D<T>& v) {
		return Vector2D<T>((T)round((double)v.x), (T)round((double)v.y));
	}
	//v1��v2�ɑ΂��鐳�ˉe�x�N�g�������߂�
	static Vector2D<T> GetOrthogonalProjectionVector(const Vector2D<T>& v1, const Vector2D<T>& v2) {
		return Vector2D<T>::GetParallelUnitVector(v2) * Vector2D<T>::Dot(v1, v2);
	}
	//�w�肵���p�x�����̒P�ʃx�N�g�����擾����
	static Vector2D<T> GetUnitVectorByRadian(double rad) {
		return Vector2D<T>((T)std::cos(rad), (T)std::sin(rad));
	}
};
//�X�J���Ƃ̏�Z���Z�q
template<typename S,typename VT>
Vector2D<VT> operator*(S s, const Vector2D<VT>& v) {
	return Vector2D<VT>(v.x*s, v.y*s);
}