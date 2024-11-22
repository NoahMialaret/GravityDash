#include "Attachment.h"

Attachment::Attachment(sf::Vector2f pos)
  :
  pos(pos)
{}

Attachment::Attachment(sf::Vector2f pos, std::function<void(sf::Vector2f)>& updateFunction)
  :
  pos(pos),
  isAttached(true),
  updateAttached(updateFunction)
{}

void Attachment::Attach(std::function<void(sf::Vector2f)> &updateFunction)
{
  updateAttached = updateFunction;
  isAttached = true;
  updateAttached(pos);
}

void Attachment::UpdatePos(sf::Vector2f newPos)
{
  pos = newPos;
  if (isAttached)
    updateAttached(pos);
}

void Attachment::Move(sf::Vector2f offset)
{
  pos += offset;
  if (isAttached)
    updateAttached(pos);
}
