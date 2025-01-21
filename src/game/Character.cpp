#include "Character.h"

Character::Character(int charID)
  : 
  charID(charID),
  acceleration(0.2f * ProgramSettings::gameScale),
  entity("character", &Utility::GetInstance()->GetEntityShader(), {4, NUM_ANIMS}),
  reticle("reticle", nullptr)
{
  vel.y = 1000.0f;

  pos = entity.GetPosition();
  entity.PushAnimation(IDLE_ANIM, 150);

  reticlePos = reticle.GetPosition();
}

void Character::Update()
{
  int deltaT = DELTA_TIME;
  if (finalJump)
    deltaT /= 2;

  runParticleTimer -= deltaT;
  invincibilityTimer -= deltaT;
  velTimer -= deltaT;

  while (velTimer <= 0)
    UpdateVelocity(horiDir);

  if (canSuperJump)
    UpdateReticle();

  switch (curState)
  {
  case State::idle:
    if (horiDir == 0)
      break;

    // Transition to moving
    curState = State::moving;

    entity.SetAnimation(WALK_ANIM, (finalJump ? 2 : 1) * 100);
    entity.SetXDir(horiDir == 1);
    break;

  case State::moving:
    if (vel.x == 0.0f && horiDir == 0) // Transition to idle
    {
      curState = State::idle;
      entity.SetAnimation(IDLE_ANIM, (finalJump ? 2 : 1) * 150);
      break;
    }

    if (horiDir != 0)
      entity.SetXDir(horiDir == 1);

    if (runParticleTimer <= 0)
    {
      // TODO: Update when particles are redone
      sf::Vector2f partVel(-horiDir * 0.2f * ProgramSettings::gameScale, (isUpright ? -0.1f : 0.1f) * ProgramSettings::gameScale);
      // Utility::GetInstance()->CreateParticle(std::make_unique<Puff>(*pos, sf::Vector2f(- (float)horiDir, (isUpright ? -1.0f : 1.0f))));
      runParticleTimer = 150;
    }
    break;

  case State::stunned:
    stunTimer -= deltaT;
    if (stunTimer > 0)
      break;

    curState = State::idle;
    entity.SetAnimation(IDLE_ANIM, (finalJump ? 2 : 1) * 150);
    if (!finalJump)
      invincibilityTimer = 3000;
    break;
  
  default:
    break;
  }

  prevPos = *pos;

  if (curState != State::stunned)
    *pos += (deltaT / 16.0f) * vel;

  entity.Update();
}

void Character::Render(sf::RenderWindow* win) const
{
  // TODO: Update when shaders are redone
  Utility::GetInstance()->GetEntityShader().setUniform("colorID", charID);
  if (invincibilityTimer <= 0 || (Clock::GetInstance()->Elapsed() / 64) % 2)
    entity.Render(win);

  if (canSuperJump && curState <= State::moving && !finalJump)
    reticle.Render(win);
}

void Character::FloorCollision(float correction)
{
  pos->y += correction;

  if (curState != State::airborne)
  {
    grounded = true;
    vel.y = 0.0f;
    return;
  }

  if (superBouncesLeft > 0)
  {
    pos->y += correction;
    vel.y = -vel.y;
    superBouncesLeft--;
    return;
  }

  Event event;
  event.type = Event::Type::playerCombo;
  event.combo.charID = charID;
  event.combo.wasSuperJump = false;

  if (superBouncesLeft == 0)
  {
    invincibilityTimer = 2000;
    superBouncesLeft = -1;
    event.combo.wasSuperJump = true;
  }

  Land();
  Event::events.push(event);
}

void Character::WallCollision(float correction)
{
  pos->x += correction;

  if (superBouncesLeft < 0)
  {
    vel.x = 0.0f;
    return;
  }

  pos->x += correction;
  vel.x = - vel.x;
}

bool Character::Hit(sf::Vector2f source)
{
  if (invincibilityTimer > 0 || curState >= State::stunned || superBouncesLeft >= 0 || finalJump)
    return false;

  Stun();

  grounded = false;

  entity.SetAnimation(STUN_ANIM, 100);

  Event event;
  event.type = Event::Type::playerHit;
  event.value = charID;
  Event::events.push(event); 

  // y = ent.y +- sqrt(radius^2-(this.x-ent.x)^2)
  pos->y = source.y + (isUpright ? -1.0f : 1.0f) 
    * std::sqrt(SCALED_DIM * SCALED_DIM
    - (pos->x - source.x) * (pos->x - source.x));

  vel.y = (isUpright ? -3.0f : 3.0f);
  vel.x = (pos->x < source.x? -10.0f : 10.0f);

  if (queueFinalJump)
  {
    queueFinalJump = false;
    finalJump = true;
  }

  return true;
}

void Character::MakeFinalJump()
{
  if (curState == State::airborne)
  {
    queueFinalJump = true;
    return;
  }
  
  Stun();
  finalJump = true;
}

int Character::GetID()
{
  return charID;
}

Character::State Character::GetCurState() const
{
  return curState;
}

bool Character::IsInvincible() const
{
  return invincibilityTimer > 0 || curState >= State::stunned || superBouncesLeft >= 0;
}

sf::Vector2f Character::GetPosition() const
{
  return *pos;
}

sf::FloatRect Character::GetHitBox() const
{
  return entity.HitBox();
}

std::pair<sf::Vector2f, sf::Vector2f> Character::GetLineHitBox() const
{
  return std::pair<sf::Vector2f, sf::Vector2f>(prevPos, *pos);
}

void Character::EnableSuperJump()
{
  canSuperJump = true;
}

void Character::UpdateVelocity(int dir)
{
  velTimer += 16;

  if (curState > State::moving)
    return;

  vel.y += (isUpright ? 1.0f : -1.0f) * 0.05f * DELTA_TIME;

  if (dir != 0 && grounded)
  {
    vel.x += acceleration * dir;
    float maxVel = 8.0f * acceleration;
    vel.x = std::clamp(vel.x, -maxVel, maxVel);
    return;
  }

  if (std::fabs(vel.x) < acceleration)
  {
    vel.x = 0.0f;
    return;
  }

  vel.x += acceleration * (grounded ? 1.0f : 0.2f) * (vel.x > 0 ? -1.0f : 1.0f);
}

void Character::UpdateReticle()
{
  if (horiDir)
  {
    float m = (float)horiDir * (isUpright ? -1.0f : 1.0f);
    reticleAngle += m * (2.0f - m * reticleAngle) * DELTA_TIME / 500.0f;
    reticleAngle = std::clamp(reticleAngle, -1.2f, 1.2f);
  } 
  else
  {
    float m = - (float)Utility::GetInstance()->GetSign(reticleAngle);
    reticleAngle += m * (fabs(reticleAngle)) * DELTA_TIME / 200.0f;
    if (Utility::GetInstance()->GetSign(reticleAngle) == (int)m)
    {
      reticleAngle = 0.0f;
    }
  } 

  *reticlePos = *pos + (isUpright ? -3.0f : 3.0f) * SCALED_DIM 
    * sf::Vector2f(std::sin(reticleAngle), std::cos(reticleAngle));
  reticle.Update();
}

void Character::Jump()
{
  if (!grounded)
    return;

  curState = State::airborne;
  
  grounded = false;
  isUpright = !isUpright;
  entity.FlipY();

  entity.SetAnimation(JUMP_ANIM, 20);

  vel.y = acceleration * 80.0f * (isUpright ? 1.0f : -1.0f);
  vel.x = 0.0f;

  reticleAngle = 0.0f;

  Event event;
  event.type = Event::Type::playerJump;
  event.value = charID;
  Event::events.push(event);
}

void Character::SuperJump()
{
  if (!canSuperJump || finalJump || !grounded)
    return;

  curState = State::airborne;

  grounded = false;
  isUpright = !isUpright;
  entity.FlipY();

  canSuperJump = false;
  superBouncesLeft = 6;
  
  entity.SetAnimation(JUMP_ANIM, 20);

  float jumpSpeed = acceleration * 80.0f * (isUpright ? 1.0f : -1.0f);

  vel.y = std::cos(reticleAngle) * jumpSpeed;
  vel.x = std::sin(reticleAngle) * jumpSpeed;
  
  reticleAngle = 0.0f;

  Event event;
  event.type = Event::Type::playerSuper;
  event.value = charID;
  Event::events.push(event);
}

void Character::Land()
{
  curState = finalJump ? State::dead : State::idle;

  grounded = true;
  vel = ZERO_VECTOR;
  superBouncesLeft = -1;

  entity.SetAnimation(LAND_ANIM, 100, 0, 300);
  entity.PushAnimation(finalJump ? REST_ANIM : IDLE_ANIM, 150);

  // Utility::GetInstance()->CreateParticle(std::make_unique<Dust>(*pos, !isUpright));

  if (queueFinalJump)
  {
    finalJump = true;
    queueFinalJump = false;
    Stun();
  }
}

void Character::Stun()
{
  if (curState == State::stunned)
    return;
    
  entity.SetAnimation(STUN_ANIM, 100);
  curState = State::stunned;
  stunTimer = 1000;
  grounded = false;
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

  if (controls->IsBindingOnInitialClick(Controls::Binding::jump))
    Jump();

  else if(controls->IsBindingOnInitialClick(Controls::Binding::special))
    SuperJump();

  horiDir = controls->IsBindingHeld(Controls::Binding::right) - controls->IsBindingHeld(Controls::Binding::left);

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
  
  std::uniform_int_distribution p(0, 99);
  int rand = p(Utility::GetInstance()->GetRNG());

  if (rand == 99)
    Jump();
  else if (rand / 3 < 3)
    horiDir = -1 + rand / 3;
  // Else retain current movement

  Character::Update();
}