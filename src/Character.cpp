#include "Character.h"

Character::Character(const char* spritePath)
  : 
  acceleration(0.2f * Utility::gameScale)
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
  //invincibility -= Utility::clock;
  //std::cout << "Move: " << move << " - State: " << (int)curState << '\n';
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
  
  default:
    std::cout << "Could not determine Character state (" << (int)curState << ")\n";
    break;
  }

  sprite.move((isLastStand ? 0.5f : 1.0f) * vel);
  anim.Update();
}

void Character::Render(sf::RenderWindow *win) const
{
  win->draw(sprite, &Utility::shaderTest);

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

  anim.ChangeAnimation((int)State::airborne + 1, 100, 0, (int)curState, 150, 300);
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

bool Character::Hit()
{
  if (invincibleEnd > CUR_TIME)
  {
    return false;
  }
  invincibleEnd = CUR_TIME + 2000;
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
    vel.x = 0.0f;
    move = 0;
    sprite.setPosition(newPos);
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

PlayableCharacter::PlayableCharacter(const char* spritePath, std::unique_ptr<Controls>& controls)
    : 
    Character(spritePath)
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

ComputerCharacter::ComputerCharacter(const char* spritePath)
  :
  Character(spritePath)
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