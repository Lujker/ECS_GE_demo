#pragma once
#include "Vector.h"

class ColorComponent
{
public:
	ColorComponent(const Vector3& rgb = { 1.f, 1.f, 1.f }, float alpha = 1.f);
	ColorComponent(float R, float G, float B, float alpha = 1.f);
	float getR() const;
	float getG() const;
	float getB() const;
	Vector3 getRGB() const;
	float  getAlpha() const;
private:
	Vector3 RGB;
	float alpha = 1.f;
};