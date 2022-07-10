#include "Collor.h"

ColorComponent::ColorComponent(const Vector3& rgb, float alpha) :
	RGB(rgb), alpha(alpha)
{}

ColorComponent::ColorComponent(float R, float G, float B, float alpha) :
	RGB(R, G, B), alpha(alpha)
{}

float ColorComponent::getR() const
{
	return RGB.x;
}

float ColorComponent::getG() const
{
	return RGB.y;
}

float ColorComponent::getB() const
{
	return RGB.z;
}

Vector3 ColorComponent::getRGB() const
{
	return RGB;
}

float ColorComponent::getAlpha() const
{
	return alpha;
}
