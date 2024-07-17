#include "Character.h"


Character::Character(int charID, PlayerBoost boost)
  : 
  charID(charID),
  acceleration(0.2f * Utility::gameScale),
  boost(boost)
{
  entity = Entity("character");

  entity.CouplePosition(&pos);

  entity.FlipY();
  entity.QueueAnimation((int)curState, 150);
}

Character::~Character()
{}

void Character::Update()
{
  particleTimer -= Clock::Delta();
  invincibilityTimer -= Clock::Delta();
  velTimer -= Clock::Delta();

  while (curState <= State::moving && velTimer <= 0)
  {
    UpdateVelocity(move);
  }
  
  prevPos = pos;

  switch (curState)
  {
  case State::idle:
    if (move == 0)
    {
      break;
    }

    curState = State::moving;

    entity.SetAnimation((int)curState, (isLastStand ? 3 : 1) * 100);
    entity.SetXDir(move == 1);

    particleTimer = (isLastStand ? 4 : 1) * 150;
    break;

  case State::moving:
    if (vel == ZERO_VECTOR && move == 0)
    {
      curState = State::idle;
      entity.SetAnimation((int)curState, (isLastStand ? 2 : 1) * 150);
      break;
    }

    if (move != 0)
    {
      entity.SetXDir(move == 1);
    }

    if (particleTimer <= 0)
    {
      sf::Vector2f partVel(-move * 0.2f * Utility::gameScale, (isUpright ? -0.1f : 0.1f) * Utility::gameScale);
      Utility::particles.push_front(std::make_unique<Puff>(pos, sf::Vector2f(- (float)move, (isUpright ? -1.0f : 1.0f))));
      particleTimer = (isLastStand ? 4 : 1) * 150;
    }
    break;

  case State::airborne:
    break;

  case State::hit:
    break;

  case State::stunned:
    stunTimer -= Clock::Delta();
    if (stunTimer <= 0)
    {
      curState = State::idle;
      invincibilityTimer = 3000;
      entity.SetAnimation((int)curState, (isLastStand ? 2 : 1) * 150);
    }
    break;

  
  default:
    std::cout << "Could not determine Character state (" << (int)curState << ")\n";
    break;
  }

  if (curState != State::airborne)
  {
    vel.y += (isUpright ? 1.0f : -1.0f) * 0.05f * Clock::Delta();
  }

  boost.Update();

  pos += (isLastStand ? 0.5f : 1.0f) * (Clock::Delta() / 16.0f) * vel;
  entity.Update();
}

void Character::Render(sf::RenderWindow *win) const
{
  Utility::entShad.setUniform("colorID", charID);
  if (invincibilityTimer <= 0 || (Clock::Elapsed() / 64) % 2)
  {
    entity.Render(win);
  }

  for (auto& point : targetPoints)
  {
    point.Render(win);
  }

  boost.Render(win);
}

void Character::UpdateVelocity(int dir)
{
  velTimer += 16;

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
  if (isBoosted)
  {
    boost.Clear();
    isBoosted = false;
    return;
  }
  
  curState = State::airborne;
  vel.x = 0.0f;
  vel.y = acceleration * 80.0f * (isUpright ? -1.0f : 1.0f);
  isUpright = !isUpright;
  entity.SetAnimation((int)curState, 20);
}

void Character::Land()
{
  vel.y = 0.0f;
  curState = State::idle;

  entity.SetAnimation((int)State::airborne + 2, 100, 0, 300);
  entity.QueueAnimation((int)curState, 150);
  entity.FlipY();

  sf::Vector2f partVel = {Utility::gameScale * 0.3f, 0.0f};
  sf::Vector2f offset = {0.5f * SCALED_DIM, 0.0f};

  Utility::particles.push_front(std::make_unique<Dust>(pos, !isUpright));

  if (isLastStand)
  {
    curState = State::dead;
  }

  if (comboCount >= 5)
  {
    boost.Increment();
    if (boost.IsFull())
    {
      isBoosted = true;
    }
  }

  comboCount = 0;

  targetPoints.clear();
}

bool Character::Hit(sf::Vector2f entPos)
{
  if (invincibilityTimer > 0 || curState >= State::hit)
  {
    return false;
  }

  std::cout << "Player has been hit!\n";

  if (curState == State::airborne)
  {
    entity.FlipY();
  }

  targetPoints.clear();
  comboCount = 0;

  // invincibleEnd = CUR_TIME + 2000;

  // y = ent.y +- sqrt(64*scale-(this.x-ent.x)^2) + for saws on top
  pos.y = entPos.y + (isUpright ? -1.0f : 1.0f) * std::sqrt(64*Utility::gameScale*Utility::gameScale - (pos.x - entPos.x) * (pos.x - entPos.x));

  vel.y = (isUpright ? -3.0f : 3.0f);
  vel.x = (pos.x - entPos.x < 0.0f ? -10.0f : 10.0f);

  curState = State::hit;
  entity.SetAnimation((int)curState, 100);

  return true;

  if (isLastStand)
  {
    return false;
  }
  isLastStand = true;
  std::cout << "Character has been hit, this is their final jump!\n";

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
    return pos;
}

void Character::SetPosition(sf::Vector2f& newPos)
{
  pos = newPos;
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
    stunTimer = 1000;
    vel.x = 0;
  }
}

sf::FloatRect Character::GetHitBox() const
{
    return entity.HitBox();
}

std::pair<sf::Vector2f, sf::Vector2f> Character::GetLineHitBox() const
{
    return std::pair<sf::Vector2f, sf::Vector2f>(prevPos, pos);
}

std::forward_list<TargetPoints> Character::GetPoints() const
{
  return targetPoints;
}

void Character::IncrementComboCount()
{
  if (comboCount < 7)
  {
    comboCount++;
  }
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
  targetPoints.emplace_front(TargetPoints(value, pos, vel));
}

// ------------------
// Playable Character
// ------------------

PlayableCharacter::PlayableCharacter(int charID, std::unique_ptr<Controls>& controls, PlayerBoost boost)
  : 
  Character(charID, boost)
{
  this->controls = std::move(controls);
}

void PlayableCharacter::Update()
{
  for (auto& point : targetPoints)
  {
    point.Update();
  }

  if (curState == State::dead)
  {
    return;
  }

  controls.get()->Update();

  if (controls.get()->JumpPressed())
  {
    StartJump();
  }

  move = controls.get()->HeldDirection();

  Character::Update();
}

// ------------------
// Computer Character
// ------------------

ComputerCharacter::ComputerCharacter(int charID, PlayerBoost boost)
  :
  Character(charID, boost)
{}

void ComputerCharacter::Update()
{
  for (auto& point : targetPoints)
  {
    point.Update();
  }
  
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