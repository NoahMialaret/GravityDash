#include "World.h"

World::World(sf::IntRect playableRegion)
    :
    playableRegion(playableRegion)
{
    renderRect = sf::RectangleShape({(float)playableRegion.width, (float)playableRegion.height});
    renderRect.setPosition((float)playableRegion.left, (float)playableRegion.top);
    renderRect.setFillColor(sf::Color(255, 229, 181));
    renderRect.setOutlineThickness(Utility::gameScale);
    renderRect.setOutlineColor(sf::Color(173, 103, 78));

    targetLeft = playableRegion.left;
}

void World::Update()
{  
    if (!isTransitioning)
    {
        return;
    }

    int deltaLeft = (-targetLeft - playableRegion.left) / 5;
    if (deltaLeft == 0)
    {
        deltaLeft = (playableRegion.left > -targetLeft) ? -1 : 1;
    }

    playableRegion.left += deltaLeft;
    playableRegion.width = - playableRegion.left * 2;

    renderRect.setPosition((float)playableRegion.left, (float)playableRegion.top);
    renderRect.setSize({(float)playableRegion.width, (float)playableRegion.height});

    if (playableRegion.left == -targetLeft)
    {
        std::cout << "World has finished transitioning!\n";
        isTransitioning = false;
    }
}

void World::Render(sf::RenderWindow* win) const
{
    win->draw(renderRect);
}

sf::IntRect World::GetRegion() const
{
    return playableRegion;
}

void World::SetTargetLeft(int targetWorldLeft)
{
    targetLeft = targetWorldLeft;
    isTransitioning = true;
}

bool World::FinishedTransitioning() const
{
    return !isTransitioning;
}
