#include "Health.h"


HealthComponent::HealthComponent(unsigned maxHealth):
	max_health(maxHealth), health(maxHealth)
{}

void HealthComponent::addDamage(unsigned damage)
{
	damage_queue.push(damage);
	if (health - damage > 0)
		health -= damage;
	else
		health = 0;
}

void HealthComponent::addHeal(unsigned heal)
{
	health_queue.push(heal);
	if (health + heal < max_health)
		health += heal;
	else
		health = max_health;
}

void HealthComponent::addFullHeal()
{
	if(isFull())
		return;
	const auto needHeal = max_health - health;
	health_queue.push(needHeal);
	health = max_health;
}

void HealthComponent::addFullDamage()
{
	if(isDead())
		return;
	const auto needDamage = health;
	damage_queue.push(needDamage);
	health = 0;
}

unsigned HealthComponent::getLastDamage() const
{
	return damage_queue.front();
}

unsigned HealthComponent::popLastDamage()
{
	const auto d = getLastDamage(); damage_queue.pop();
	return d;
}

void HealthComponent::undoLastDamage()
{
	addHeal(popLastDamage());
}

unsigned HealthComponent::getLastHeal() const
{
	return health_queue.front();
}

unsigned HealthComponent::popLastHeal()
{
	const auto h = getLastHeal(); health_queue.pop();
	return h;
}

void HealthComponent::undoLastHeal()
{
	addDamage(popLastHeal());
}
