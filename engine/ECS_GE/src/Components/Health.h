#pragma once
#include "IComponent.h"
#include <queue>

class HealthComponent: public IComponent
{
	HealthComponent(unsigned maxHealth);
	~HealthComponent() = default;
	bool isDead() const { return health == 0; }
	bool isFull() const { return health == max_health; }
	unsigned getHealth() const { return health; }
	unsigned getMaxHealth() const { return max_health; }
	void setHealth(unsigned health) { this->health = health; }
	void setMaxHealth(unsigned health) { this->max_health = health; }
	void addDamage(unsigned damage);
	void addHeal(unsigned heal);
	void addFullHeal();
	void addFullDamage();
	unsigned getLastDamage() const;
	unsigned popLastDamage();
	void undoLastDamage();
	unsigned getLastHeal() const;
	unsigned popLastHeal();
	void undoLastHeal();
private:
	std::queue<unsigned> damage_queue;
	std::queue<unsigned> health_queue;
	unsigned max_health;
	unsigned health;
};
