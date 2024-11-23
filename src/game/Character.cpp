#include "Character.h"


Character::Character(int charID)
  : 
  charID(charID),
  acceleration(0.2f * ProgramSettings::gameScale)
{
  vel.y = 1000.0f;
  entity = Entity("character");

  entity.CouplePosition(&pos);

  entity.QueueAnimation((int)curState, 150);

  reticle = Entity("reticle", nullptr, (sf::Vector2i)Textures::textures.at("reticle").getSize());
  reticle.CouplePosition(&reticlePos);
}

Character::~Character()
{}

void Character::Update()
{
  particleTimer -= Clock::Delta();
  invincibilityTimer -= Clock::Delta();
  velTimer -= Clock::Delta();
  jumpTimer -= Clock::Delta();

  while (curState <= State::moving && velTimer <= 0)
  {
    UpdateVelocity(move);
  }
  
  prevPos = pos;

  if (superJumpEnabled)
  {
    if (move)
    {
      float m = (float)move * (isUpright ? -1.0f : 1.0f);
      reticleAngle += m * (2.0f - m * reticleAngle) * (float)Clock::Delta() / 500.0f;
      reticleAngle = std::clamp(reticleAngle, -1.2f, 1.2f);
    } 
    else
    {
      float m = - (float)Utility::GetSign(reticleAngle);
      reticleAngle += m * (fabs(reticleAngle)) * (float)Clock::Delta() / 200.0f;
      if (Utility::GetSign(reticleAngle) == (int)m)
      {
        reticleAngle = 0.0f;
      }
    } 

    reticlePos = pos + (isUpright ? -1.0f : 1.0f) * ProgramSettings::gameScale * Utility::spriteDim * 3.0f * sf::Vector2f(std::sin(reticleAngle), std::cos(reticleAngle));
    reticle.Update();
  }

  switch (curState)
  {
  case State::idle:
    if (move == 0)
    {
      break;
    }

    curState = State::moving;

    entity.SetAnimation((int)curState, (finalJump ? 3 : 1) * 100);
    entity.SetXDir(move == 1);

    particleTimer = (finalJump ? 4 : 1) * 150;
    break;

  case State::moving:
    if (vel == ZERO_VECTOR && move == 0)
    {
      curState = State::idle;
      entity.SetAnimation((int)curState, (finalJump ? 2 : 1) * 150);
      break;
    }

    if (move != 0)
    {
      entity.SetXDir(move == 1);
    }

    if (particleTimer <= 0)
    {
      sf::Vector2f partVel(-move * 0.2f * ProgramSettings::gameScale, (isUpright ? -0.1f : 0.1f) * ProgramSettings::gameScale);
      Utility::particles.push_front(std::make_unique<Puff>(pos, sf::Vector2f(- (float)move, (isUpright ? -1.0f : 1.0f))));
      particleTimer = (finalJump ? 4 : 1) * 150;
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
      entity.SetAnimation((int)curState, (finalJump ? 2 : 1) * 150);
      reticleAngle = 0.0f;
      reticle.Update();
    }
    break;

  case State::dead:
    break;
  
  default:
    std::cout << "Could not determine Character state (" << (int)curState << ")\n";
    break;
  }

  if (curState != State::airborne)
  {
    vel.y += (isUpright ? 1.0f : -1.0f) * 0.05f * Clock::Delta();
  }

  pos += (finalJump ? 0.5f : 1.0f) * (Clock::Delta() / 16.0f) * vel;

  entity.Update();
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

void Character::Render(sf::RenderWindow *win) const
{
  Utility::entShad.setUniform("colorID", charID);
  if (invincibilityTimer <= 0 || (Clock::Elapsed() / 64) % 2)
  {
    entity.Render(win);
  }

  if (superJumpEnabled && curState <= State::moving && !finalJump)
    reticle.Render(win);
}

void Character::Jump()
{
  if (curState >= State::airborne)
  {
    return;
  }

  Event event;
  event.type = Event::Type::playerJump;
  event.value = charID;
  Event::events.push(event);

  GameStats::localStats.jumps++;

  vel.y = acceleration * 80.0f * (isUpright ? -1.0f : 1.0f);;
  vel.x = 0.0f;
  
  isUpright = !isUpright;
  reticleAngle = 0.0f;

  curState = State::airborne;
  
  entity.SetAnimation((int)curState, 20);
}

void Character::SuperJump()
{
  if (curState >= State::airborne || !superJumpEnabled)
    return;

  superJumpEnabled = false;

  Event event;
  event.type = Event::Type::playerSuper;
  event.value = charID;
  Event::events.push(event);

  GameStats::localStats.specials++;

  float jumpSpeed = acceleration * 80.0f * (isUpright ? -1.0f : 1.0f);

  bouncesLeft = 5;
  vel.y = std::cos(reticleAngle) * jumpSpeed;
  vel.x = std::sin(reticleAngle) * jumpSpeed;
  
  isUpright = !isUpright;
  reticleAngle = 0.0f;

  curState = State::airborne;
  
  entity.SetAnimation((int)curState, 20);
}

void Character::EnableSuperJump()
{
  superJumpEnabled = true;
}

void Character::Land()
{
  vel.y = 0.0f;
  vel.x = 0.0f;
  curState = State::idle;

  entity.SetAnimation((int)State::airborne + 2, 100, 0, 300);
  entity.QueueAnimation((int)curState, 150);
  entity.FlipY();

  Utility::particles.push_front(std::make_unique<Dust>(pos, !isUpright));

  if (finalJump)
  {
    curState = State::dead;
  }
  else if (queueFinalJump)
  {
    finalJump = true;
    queueFinalJump = false;
  }

  if (comboCount >= 3)
  {
    canCollect = true;
    GameStats::localStats.combos++;
  }
  else 
  {
    timeBoostCollected = 0;
  }

  bouncesLeft = -1;

  comboCount = 0;
}

void Character::FloorCollision(float distance)
{
  pos.y += distance;

  if (curState == State::airborne)
  {
    if (bouncesLeft <= 0)
    {
      Land();
      Event event;
      event.type = Event::Type::playerCombo;
      event.combo.charID = charID;
      event.combo.wasSuperJump = false;
      Event::events.push(event);
      return;
    }

    bouncesLeft--;

    if (bouncesLeft == 0)
    {
      invincibilityTimer = 2000;
      Land();
      Event event;
      event.type = Event::Type::playerCombo;
      event.combo.charID = charID;
      event.combo.wasSuperJump = true;
      Event::events.push(event);
      return;
    }

    pos.y += distance;
    vel.y = -vel.y;

    return;
  }
  else if (curState == State::hit)
  {
    curState = State::stunned;
    stunTimer = 1000;
    vel.x = 0;
  }

  vel.y = 0.0f;
}

void Character::WallCollision(float distance)
{
  pos.x += distance;
  if (bouncesLeft >= 0)
  {
    pos.x += distance;
    vel.x = - vel.x;
  }
  else
  {
    vel.x = 0.0f;
  }
}

bool Character::Hit(sf::Vector2f entPos)
{
  if (invincibilityTimer > 0 || curState >= State::hit || bouncesLeft >= 0 || finalJump)
  {
    return false;
  }

  Event event;
  event.type = Event::Type::playerHit;
  event.value = charID;
  Event::events.push(event); 

  GameStats::localStats.hits++;

  std::cout << "Player has been hit!\n";

  if (curState == State::airborne)
  {
    entity.FlipY();
  }
  if (queueFinalJump)
  {
    queueFinalJump = false;
    finalJump = true;
  }

  // y = ent.y +- sqrt(64*scale-(this.x-ent.x)^2) + for saws on top
  pos.y = entPos.y + (isUpright ? -1.0f : 1.0f) * std::sqrt(64*ProgramSettings::gameScale*ProgramSettings::gameScale - (pos.x - entPos.x) * (pos.x - entPos.x));

  vel.y = (isUpright ? -3.0f : 3.0f);
  vel.x = (pos.x - entPos.x < 0.0f ? -10.0f : 10.0f);

  curState = State::hit;
  entity.SetAnimation((int)curState, 100);

  comboCount = 0;
  timeBoostCollected = 0;
  canCollect = false;

  return true;
}

void Character::Kill()
{
  jumpTimer = 2000;
  if (curState == State::airborne)
  {
    queueFinalJump = true;
    return;
  }
  finalJump = true;
}

Character::State Character::GetCurState() const
{
  return curState;
}

bool Character::IsFinalJump() const
{
  return finalJump;
}

sf::Vector2f Character::GetPosition() const
{
  return pos;
}

void Character::SetPosition(sf::Vector2f& newPos)
{
  pos = newPos;
}

sf::FloatRect Character::GetHitBox() const
{
    return entity.HitBox();
}

std::pair<sf::Vector2f, sf::Vector2f> Character::GetLineHitBox() const
{
  return std::pair<sf::Vector2f, sf::Vector2f>(prevPos, pos);
}

void Character::IncrementComboCount()
{
  if (comboCount < 7)
  {
    comboCount++;
  }
}

void Character::IncrementTimeBoost()
{
  timeBoostCollected++;
}

int Character::GetTimeBoost()
{
  if (canCollect)
  {
    canCollect = false;
    int temp = timeBoostCollected;
    timeBoostCollected = 0;
    return temp;
  }
  return 0;
}

int Character::GetID()
{
  return charID;
}

// ------------------
// Playable Character
// ------------------

PlayableCharacter::PlayableCharacter(int charID, Controls* controls)
  : 
  Character(charID),
  controls(controls)
{}

void PlayableCharacter::Update()
{
  if (curState == State::dead)
  {
    Character::Update();
    return;
  }

  if (controls->IsBindingOnInitialClick(Controls::Binding::jump) && jumpTimer <= 0)
  {
    Jump();
  }
  else if(controls->IsBindingOnInitialClick(Controls::Binding::special) && !finalJump && jumpTimer <= 0)
  {
    SuperJump();
  }

  move = controls->IsBindingHeld(Controls::Binding::right) - controls->IsBindingHeld(Controls::Binding::left);

  Character::Update();
}

// ------------------
// Computer Character
// ------------------

ComputerCharacter::ComputerCharacter(int charID)
  :
  Character(charID)
{}

void ComputerCharacter::Update()
{
  if (curState == State::dead)
  {
    Character::Update();
    return;
  }
  
  std::uniform_int_distribution chance(0, 99);
  int randomNum = chance(Utility::rng);

  if (randomNum == 0)
  {
    Jump();
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