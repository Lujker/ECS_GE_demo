#pragma once
#include "glm/vec2.hpp"
#include "Point.h"

template<class _T> class TRect
{
public:
	_T mX;
	_T mY;
	_T mWidth;
	_T mHeight;

public:
	TRect(_T theX, _T theY, _T theWidth, _T theHeight) :
		mX(theX), mY(theY), mWidth(theWidth), mHeight(theHeight)
	{
	}

	TRect(const TRect<_T>& theTRect) :
		mX(theTRect.mX), mY(theTRect.mY), mWidth(theTRect.mWidth), mHeight(theTRect.mHeight)
	{
	}

	TRect() :
		mX(0), mY(0), mWidth(0), mHeight(0)
	{
	}

	TRect(Point tLeftTop, Point tRightBottom) :
		mX(tLeftTop.mX), mY(tLeftTop.mY), mWidth(tRightBottom.mX - tLeftTop.mX), mHeight(tRightBottom.mY - tLeftTop.mY)
	{
	}
	
	bool Intersects(const TRect<_T>& theTRect) const
	{
		return !((theTRect.mX + theTRect.mWidth <= mX) ||
			(theTRect.mY + theTRect.mHeight <= mY) ||
			(theTRect.mX >= mX + mWidth) ||
			(theTRect.mY >= mY + mHeight));
	}
	
	TRect<_T> Intersection(const TRect<_T>& theTRect)	const
	{
		_T x1 = (std::max)(mX, theTRect.mX);
		_T x2 = (std::min)(mX + mWidth, theTRect.mX + theTRect.mWidth);
		_T y1 = (std::max)(mY, theTRect.mY);
		_T y2 = (std::min)(mY + mHeight, theTRect.mY + theTRect.mHeight);
		if (((x2 - x1) < 0) || ((y2 - y1) < 0))
			return TRect<_T>(0, 0, 0, 0);
		else
			return TRect<_T>(x1, y1, x2 - x1, y2 - y1);
	}
	
	TRect<_T>	Union(const TRect<_T>& theTRect)
	{
		_T x1 = std::min(mX, theTRect.mX);
		_T x2 = std::max(mX + mWidth, theTRect.mX + theTRect.mWidth);
		_T y1 = std::min(mY, theTRect.mY);
		_T y2 = std::max(mY + mHeight, theTRect.mY + theTRect.mHeight);
		return TRect<_T>(x1, y1, x2 - x1, y2 - y1);
	}
	
	bool	Contains(_T theX, _T theY) const
	{
		return ((theX >= mX) && (theX < mX + mWidth) &&
			(theY >= mY) && (theY < mY + mHeight));
	}
	
	bool	Contains(const TPoint<_T>& thePoint) const
	{
		return ((thePoint.mX >= mX) && (thePoint.mX < mX + mWidth) &&
			(thePoint.mY >= mY) && (thePoint.mY < mY + mHeight));
	}
	
	void	Offset(_T theX, _T theY)
	{
		mX += theX;
		mY += theY;
	}
	
	void	Offset(const TPoint<_T>& thePoint)
	{
		mX += thePoint.mX;
		mY += thePoint.mY;
	}
	
	TRect	Inflate(_T theX, _T theY)
	{
		mX -= theX;
		mWidth += theX * 2;
		mY -= theY;
		mHeight += theY * 2;

		return *this;
	}

	bool operator==(const TRect<_T>& theRect) const
	{
		return (mX == theRect.mX) && (mY == theRect.mY) && (mWidth == theRect.mWidth) && (mHeight == theRect.mHeight);
	}

	TRect<_T>& operator+=(const TRect<_T>& theRect)
	{
		mX += theRect.mX; mY += theRect.mY; mWidth += theRect.mWidth; mHeight += theRect.mHeight;
		return *this;
	}

	TRect<_T>& operator+=(const FPoint& point)
	{
		mX += point.mX; mY += point.mY;
		return *this;
	}
};

typedef TRect<int> Rect;
typedef TRect<double> FRect;
