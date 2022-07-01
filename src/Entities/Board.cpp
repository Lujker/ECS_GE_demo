#include "Board.h"

#include "RenderSystem.h"
#include "ResourceManager.h"

Board::Board(const std::string& name, const PositionComponent& default_position, 
             const CollisionComponent& default_size, const CollisionComponent& default_collision):
	IGameObject(name, default_position, default_size, default_collision)
{
}

void Board::Init()
{
	m_image = RES.getSharedImage("res/images/Layer_0001_8.png");
	isDrawCollision = true;
	isDrawName = true;
	IGameObject::Init();
}

void Board::Draw()
{
	if (m_image)
		RENDER.Render(m_image, m_position, m_size);
	IGameObject::Draw();
}

void Board::Update(const float& delta_time)
{
	IGameObject::Update(delta_time);
}
