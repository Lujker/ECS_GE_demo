#include "Collor.h"

CollorComponent::CollorComponent(const Vector3& rgb, float alpha) :
	RGB(rgb), alpha(alpha)
{}

CollorComponent::CollorComponent(float R, float G, float B, float alpha) :
	RGB(R, G, B), alpha(alpha)
{}

float CollorComponent::getR() const
{
	return RGB.x;
}

float CollorComponent::getG() const
{
	return RGB.y;
}

float CollorComponent::getB() const
{
	return RGB.z;
}

Vector3 CollorComponent::getRGB() const
{
	return RGB;
}

float CollorComponent::getAlpha() const
{
	return alpha;
}
