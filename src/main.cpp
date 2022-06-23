#include "Collision.h"
#include "Engine.h"
#include <Components/Position.h>

#include "CollisionSystem.h"
#include "DisplayString.h"
#include "LogSystem.h"
#include "RenderSystem.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "UpdateSystem.h"

int main(int argc, char** argv)
{
	Engine engine;
    if (!engine.Init(argv[0]))
        return -1;
	const PositionComponent pos_img{ 100, 100 };
    const CollisionComponent col_img{ 100, 100, false };
    const PositionComponent pos_sprite{ 350, 100 };
    const CollisionComponent size_sprite{ 120, 80, false };
    const CollisionComponent col_sprite{ 40, 40, 30, 0, false };

    const CollisionComponent col_form{ std::vector<FPoint>{ {30, 0},{15, 25}, {25, 30},{35, 25} ,{35, 10} }, false, 2 };
    const PositionComponent string_pos{ 300,300, 2 };
    const auto atlas = RES.getFrameAtlas("res/atlas/_AttackNoMovement");
    const auto imaga_atlas = std::make_shared<RenderEngine::Image2D>(atlas);
    imaga_atlas->SetSubTexture(atlas->getFrame("1")->second);
    const auto image = RES.getSharedImage("res/images/tank.png");
    const auto sprite = RES.getSprite("res/sprites/test/Kinght");
    sprite->mirror(false, true);
    const auto string = std::make_shared<DisplayString>("Can't");
    string->mirror(false, false);
    //sprite->setAnimation("idle");
    for (auto i : sprite->getAnimationsName())
        std::cout << i << std::endl;
    /* Loop until the user closes the window */

    if (COLLISION.intersect2D(col_form, { 300,300 }, col_form, { 320,300 }))
        LOG("Intersect2D!");
    else
        LOG("Not intersect2D");

    UPDATE.GlobalUpdate(true);
    while (!engine.DispatchEvents())
    {
        UPDATE.GlobalUpdate();
        UPDATE.Update(sprite);
        /* Render here */
        RENDER.clear();
        RENDER.Render(sprite, pos_sprite, size_sprite);
        RENDER.Render(image, pos_img + FPoint{ 100, 100 }, col_img);
        RENDER.Render(col_form, { 300,300 });
        RENDER.Render(col_form, { 320,300 });
        RENDER.Render(FRect{ pos_sprite.getPosition().mX + col_sprite.getXOffset(), pos_sprite.getPosition().mY + col_sprite.getYOffset(), col_sprite.getWidth(), col_sprite.getHeight() });
        RENDER.Render(string, string_pos, 1.f, { 0.3f,1.f,0.5f, 0.6f });
        RENDER.Render(imaga_atlas, pos_img, size_sprite);
        engine.SwapBuffers();
    }
    return 0;
}
