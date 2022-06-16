#pragma once
#include "Vector.h"

class CollorComponent
{
public:
	CollorComponent(const Vector3& rgb = { 1.f, 1.f, 1.f }, float alpha = 1.f);
	CollorComponent(float R = 1.f, float G = 1.f, float B = 1.f, float alpha = 1.f);
	float getR() const;
	float getG() const;
	float getB() const;
	Vector3 getRGB() const;
	float  getAlpha() const;
private:
	Vector3 RGB;
	float alpha = 1.f;
};