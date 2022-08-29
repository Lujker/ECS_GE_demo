#pragma once

template<class _T> class TPoint
{
public:
	_T	mX;
	_T	mY;

public:
	TPoint(_T theX, _T theY) :
		mX(theX),
		mY(theY)
	{
	}

	TPoint(const TPoint<_T>& theTPoint) :
		mX(theTPoint.mX),
		mY(theTPoint.mY)
	{
	}

	TPoint() :
		mX(0),
		mY(0)
	{
	}

	inline bool operator==(const TPoint& p)
	{
		return ((p.mX == mX) && (p.mY == mY));
	}

	inline bool operator!=(const TPoint& p)
	{
		return ((p.mX != mX) || (p.mY != mY));
	}

	TPoint operator+(const TPoint& p) const { return TPoint(mX + p.mX, mY + p.mY); }
	TPoint operator-(const TPoint& p) const { return TPoint(mX - p.mX, mY - p.mY); }
	TPoint operator*(const TPoint& p) const { return TPoint(mX * p.mX, mY * p.mY); }
	TPoint operator/(const TPoint& p) const { return TPoint(mX / p.mX, mY / p.mY); }
	TPoint& operator+=(const TPoint& p) { mX += p.mX; mY += p.mY; return *this; }
	TPoint& operator-=(const TPoint& p) { mX -= p.mX; mY -= p.mY; return *this; }
	TPoint& operator*=(const TPoint& p) { mX *= p.mX; mY *= p.mY; return *this; }
	TPoint& operator/=(const TPoint& p) { mX /= p.mX; mY /= p.mY; return *this; }
	TPoint operator*(_T s) const { return TPoint(mX * s, mY * s); }
	TPoint operator/(_T s) const { return TPoint(mX / s, mY / s); }
};

template<class _T> class T3DPoint
{
public:
	_T	mX;
	_T	mY;
	_T	mZ;
public:
	T3DPoint(_T theX, _T theY, _T theZ) :
		mX(theX),
		mY(theY),
		mZ(theZ)
	{
	}

	T3DPoint(const T3DPoint<_T>& theT3DPoint) :
		mX(theT3DPoint.mX),
		mY(theT3DPoint.mY),
		mZ(theT3DPoint.mZ)
	{
	}

	T3DPoint() :
		mX(0),
		mY(0),
		mZ(0)
	{
	}

	inline bool operator==(const T3DPoint& p)
	{
		return ((p.mX == mX) && (p.mY == mY) && (p.mZ == mZ));
	}

	inline bool operator!=(const T3DPoint& p)
	{
		return ((p.mX != mX) || (p.mY != mY) && (p.mZ != mZ));
	}

	T3DPoint operator+(const T3DPoint& p) const { return T3DPoint(mX + p.mX, mY + p.mY, mZ + p.mZ); }
	T3DPoint operator-(const T3DPoint& p) const { return T3DPoint(mX - p.mX, mY - p.mY, mZ - p.mZ); }
	T3DPoint operator*(const T3DPoint& p) const { return T3DPoint(mX * p.mX, mY * p.mY, mZ * p.mZ); }
	T3DPoint operator/(const T3DPoint& p) const { return T3DPoint(mX / p.mX, mY / p.mY, mZ / p.mZ); }
	T3DPoint& operator+=(const T3DPoint& p) { mX += p.mX; mY += p.mY; mZ += p.mZ; return *this; }
	T3DPoint& operator-=(const T3DPoint& p) { mX -= p.mX; mY -= p.mY; mZ -= p.mZ; return *this; }
	T3DPoint& operator*=(const T3DPoint& p) { mX *= p.mX; mY *= p.mY; mZ *= p.mZ; return *this; }
	T3DPoint& operator/=(const T3DPoint& p) { mX /= p.mX; mY /= p.mY; mZ /= p.mZ; return *this; }
	T3DPoint operator*(_T s) const { return T3DPoint(mX * s, mY * s, mZ * s); }
	T3DPoint operator/(_T s) const { return T3DPoint(mX / s, mY / s, mZ / s); }
};

typedef T3DPoint<int> Point3;
typedef T3DPoint<double> FPoint3;
typedef TPoint<int> Point;
typedef TPoint<double> FPoint;