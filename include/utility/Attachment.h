#ifndef ATTACHMENT_H
#define ATTACHMENT_H

#include <SFML/Graphics.hpp>

#include <functional>

// Attachments allow the position of one class to be influenced 
// by a reference point associated with another class
class Attachment
{
public:
  Attachment() = default;
  // Constructs an attachment point with a reference position
  Attachment(sf::Vector2f pos);
  // Constructs an attachment point with a reference position, 
  //  and a position update function for the object being attached
  Attachment(sf::Vector2f pos, std::function<void(sf::Vector2f)>& updateFunction);
  // Attaches an object to the attachment point
  void Attach(std::function<void(sf::Vector2f)>& updateFunction);

  // Updates the position of the reference and attached object if applicable
  void UpdatePos(sf::Vector2f newPos);
  // Moves the position of the reference and attached object if applicable
  void Move(sf::Vector2f offset);
  // Forces a call to `updateAttached`
  void ForceUpdate();

private:
  // Whether an object has been attached
  bool isAttached = false;
  // The position of the reference point
  sf::Vector2f pos;
  // The function used to update the position of of the attached object
  std::function<void(sf::Vector2f)> updateAttached;
};

#endif