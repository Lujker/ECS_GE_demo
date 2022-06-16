#include "Collor.h"

CollorComponent::CollorComponent(const Vector3& rgb):
	RGB(rgb)
{}

CollorComponent::CollorComponent(float R, float G, float B) :
	RGB(R, G, B)
{}

unsigned CollorComponent::getR() const
{
	return RGB.x;
}

unsigned CollorComponent::getG() const
{
	return RGB.y;
}

unsigned CollorComponent::getB() const
{
	return RGB.z;
}

Vector3 CollorComponent::getRGB() const
{
	return RGB;
}
