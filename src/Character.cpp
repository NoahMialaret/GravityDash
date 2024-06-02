#include "Character.h"

Character::Character(const char* spritePath, int charID)
  : 
  acceleration(0.2f * Utility::gameScale),
  charID(charID)
{
  if (!tex.loadFromFile(spritePath)) 
  {
    std::cout << "Character texture could not be loaded!\n";
  }

  sprite.setTexture(tex);    
  sprite.setOrigin(CENTRED_ORIGIN);

  sprite.setScale({Utility::gameScale, -Utility::gameScale});

  anim = Animation(&sprite);
  anim.ChangeAnimation((int)curState, 150);
}

Character::~Character()
{
  Point::DeletePoints(points);
}

void Character::Update()
{
  if (curState <= State::moving)
  {
    UpdateVelocity(move);
  }
  
  prevPos = sprite.getPosition();

  switch (curState)
  {
  case State::idle:
    if (move == 0)
    {
      break;
    }

    curState = State::moving;

    anim.ChangeAnimation((int)curState, (isLastStand ? 3.0f : 1.0f) * 100);
    sprite.setScale({move * Utility::gameScale, sprite.getScale().y});

    nextRunParticle = (isLastStand ? 4.0f : 1.0f) * 150 + CUR_TIME;
    break;

  case State::moving:
    if (vel == ZERO_VECTOR && move == 0)
    {
      curState = State::idle;
      anim.ChangeAnimation((int)curState, (isLastStand ? 2.0f : 1.0f) * 150);
      break;
    }

    if (move != 0 && move != Utility::GetSign(sprite.getScale().x))
    {
      sprite.scale({-1.0f, 1.0f});
    }

    if (nextRunParticle < CUR_TIME)
    {
      sf::Vector2f partVel(-move * 0.2f * Utility::gameScale, (isUpright ? -0.1f : 0.1f) * Utility::gameScale);
      Particle* temp = new Particle(Particle::Type::walkDust, partVel, sprite.getPosition() - 
        sf::Vector2f(move * 0.5f * SCALED_DIM, 0.0f));
      Particle::CreateNewParticle(temp);
      nextRunParticle += (isLastStand ? 4.0f : 1.0f) * 150;
    }
    break;

  case State::airborne:
    break;

  case State::hit:
    break;

  case State::stunned:
    if (stunTimer < CUR_TIME)
    {
      curState = State::idle;
      invincibleEnd = CUR_TIME + 3000;
      anim.ChangeAnimation((int)curState, (isLastStand ? 2.0f : 1.0f) * 150);
    }
    break;

  
  default:
    std::cout << "Could not determine Character state (" << (int)curState << ")\n";
    break;
  }

  if (curState != State::airborne)
  {
    vel.y += (isUpright ? 1.0f : -1.0f) * 0.5f;
  }

  sprite.move((isLastStand ? 0.5f : 1.0f) * vel);
  anim.Update();
}

void Character::Render(sf::RenderWindow *win) const
{
  Utility::entShad.setUniform("colorID", charID);
  if (invincibleEnd < CUR_TIME || (CUR_TIME / 64) % 2 == 1)
  {
    win->draw(sprite, &Utility::entShad);
  }

  Point::RenderPoints(points, win);
}

void Character::UpdateVelocity(int dir)
{
  if (dir != 0 && curState != State::airborne)
  {
    vel.x += acceleration * dir;
    vel.x = std::clamp(vel.x, -acceleration * 8.0f, acceleration * 8.0f);
    return;
  }

  if (vel.x > 0.0f)
  {
    vel.x -= acceleration;
  }
  else if (vel.x < 0.0f)
  {
    vel.x += acceleration;
  }

  if (std::fabs(vel.x) < acceleration)
  {
    vel.x = 0.0f;
  }
}

void Character::StartJump()
{
  if (curState >= State::airborne)
  {
      return;
  }
  
  curState = State::airborne;
  vel.x = 0.0f;
  vel.y = acceleration * 80.0f * (isUpright ? -1.0f : 1.0f);
  isUpright = !isUpright;
  anim.ChangeAnimation((int)curState, 100);
}

int Character::Land()
{
  vel.y = 0.0f;
  curState = State::idle;

  anim.ChangeAnimation((int)State::airborne + 2, 100, 0, (int)curState, 150, 300);
  sprite.scale({1.0f, -1.0f});

  sf::Vector2f partVel = {Utility::gameScale * 0.3f, 0.0f};
  sf::Vector2f offset = {0.5f * SCALED_DIM, 0.0f};

  Particle* temp = new Particle(Particle::Type::landingImpact, partVel, sprite.getPosition() + offset, {fabsf(sprite.getScale().x), sprite.getScale().y});
  Particle::CreateNewParticle(temp);
  temp = new Particle(Particle::Type::landingImpact, -partVel, sprite.getPosition() - offset, {-fabsf(sprite.getScale().x), sprite.getScale().y});
  Particle::CreateNewParticle(temp);

  if (isLastStand)
  {
    curState = State::dead;
  }

  if (comboCount == 0)
  {
    return 0;
  }

  comboCount = 0;

	sf::Vector2f avPos = Point::GetAveragePosition(points);

	unsigned int accumulatedPoints = Point::GetTotalScore(points);

	Point* total = new Point(accumulatedPoints, avPos, 700, 500);
	Point::CreateNewPoint(points, total);

	Point::SetPointsDestination(points, avPos, 200);

  return accumulatedPoints;
}

bool Character::Hit(sf::Vector2f entPos)
{
  if (invincibleEnd > CUR_TIME || curState >= State::hit)
  {
    return false;
  }
  std::cout << "Player has been hit!\n";

  if (curState == State::airborne)
  {
    sprite.scale({1.0f, -1.0f});

    sf::Vector2f avPos = Point::GetAveragePosition(points);

    Point::SetPointsDestination(points, avPos, 200);
  }

  // invincibleEnd = CUR_TIME + 2000;

  // y = ent.y +- sqrt(64*scale-(this.x-ent.x)^2) + for saws on top
  sf::Vector2f pos(sprite.getPosition().x, 0.0f);
  pos.y = entPos.y + (isUpright ? -1.0f : 1.0f) * std::sqrt(64*Utility::gameScale*Utility::gameScale - (pos.x - entPos.x) * (pos.x - entPos.x));
  sprite.setPosition(pos);

  vel.y = (isUpright ? -3.0f : 3.0f);
  vel.x = (pos.x - entPos.x < 0.0f ? -10.0f : 10.0f);

  curState = State::hit;
  anim.ChangeAnimation((int)curState, 100);

  return true;

  if (isLastStand)
  {
      return false;
  }
  isLastStand = true;
  std::cout << "Character has been hit, this is their final jump!\n";

  waitToJump = 1000 + CUR_TIME;
  return true;
}

Character::State Character::GetCurState() const
{
    return curState;
}

bool Character::IsLastStand() const
{
    return isLastStand;
}

sf::Vector2f Character::GetPosition() const
{
    return sprite.getPosition();
}

void Character::SetPosition(sf::Vector2f& newPos)
{
  sprite.setPosition(newPos);
}

void Character::SetXVelocity(float xVel)
{
  vel.x = xVel;
}

void Character::SetYVelocity(float yVel)
{
  vel.y = yVel;

  if (curState == State::hit)
  {
    curState = State::stunned;
    stunTimer = CUR_TIME + 1000;
    vel.x = 0;
  }
}

sf::FloatRect Character::GetHitBox() const
{
    return sprite.getGlobalBounds();
}

std::pair<sf::Vector2f, sf::Vector2f> Character::GetLineHitBox() const
{
    return std::pair<sf::Vector2f, sf::Vector2f>(prevPos, sprite.getPosition());
}

void Character::IncrementComboCount()
{
  comboCount++;
}

void Character::AddNewPoint(sf::Vector2f pos, sf::Vector2f vel)
{
  int value = 50 * (int)pow(2, comboCount);
  if (value >= 25600) // The score from hitting 10th target in a row
  {
    value = 25600;
    // Goldenpoint++; a record of how many times this value was reached
  }

  AddNewPoint(value, pos, vel);
}

void Character::AddNewPoint(int value, sf::Vector2f pos, sf::Vector2f vel)
{
  if (value < 0) // Temp code until points and entity link lists are refactored
  {
    vel = ZERO_VECTOR;
  }
  Point* tempPoint = new Point(value, pos, vel);
  Point::CreateNewPoint(points, tempPoint);
}

// ------------------
// Playable Character
// ------------------

PlayableCharacter::PlayableCharacter(const char* spritePath, int charID, std::unique_ptr<Controls>& controls)
    : 
    Character(spritePath, charID)
{
  this->controls = std::move(controls);
}

void PlayableCharacter::Update()
{
  Point::UpdatePoints(points);

  if (curState == State::dead)
  {
    anim.Update();
    return;
  }

  controls.get()->Update();

  if (controls.get()->JumpPressed() && waitToJump < CUR_TIME)
  {
    StartJump();
  }

  move = controls.get()->HeldDirection();

  Character::Update();
}

// ------------------
// Computer Character
// ------------------

ComputerCharacter::ComputerCharacter(const char* spritePath, int charID)
  :
  Character(spritePath, charID)
{}

void ComputerCharacter::Update()
{
  Point::UpdatePoints(points);
  
  std::uniform_int_distribution chance(0, 99);
  int randomNum = chance(Utility::rng);

  if (randomNum == 0)
  {
    StartJump();
  }
  else if (randomNum <= 3)
  {
    move = 0;
  }
  else if (randomNum > 3 && randomNum <= 6)
  {
    move = 1;
  }
  else if (randomNum > 6 && randomNum <= 9)
  {
    move = -1;
  }

  Character::Update();
}