#include "Character.h"

Character::Character(int charID, int playerNum)
  : 
  charID(charID),
  playerNum(playerNum),
  entity("character", {4, NUM_ANIMS}),
  acceleration(0.2f),
  reticle("reticle")
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
      ParticleManager::GetInstance()->CreateParticle(
        Puff(*pos, 
             sf::Vector2f(- (float)horiDir, (isUpright ? -1.0f : 1.0f)),
             (int)Settings::GetInstance()->GetPlayerColour(playerNum)));
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
  ENTITY_SHADER.setUniform("colorID", (int)Settings::GetInstance()->GetPlayerColour(playerNum));
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

  PUSH_EVENT(Event::Type::playerCombo, {.combo = {charID, superBouncesLeft >= 0, 0}});

  if (superBouncesLeft == 0)
  {
    invincibilityTimer = 2000;
    superBouncesLeft = -1;
  }

  Land();
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

  PUSH_EVENT(Event::Type::playerHit, {.value = charID});

  // y = ent.y +- sqrt(radius^2-(this.x-ent.x)^2)
  pos->y = source.y + (isUpright ? -1.0f : 1.0f) 
    * std::sqrt(SPRITE_DIM * SPRITE_DIM
    - (pos->x - source.x) * (pos->x - source.x));

  vel.y = (isUpright ? -1.5f : 1.5f);
  vel.x = (pos->x < source.x ? -2.0f : 2.0f);

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

Utility::LineSegment Character::GetLineHitBox() const
{
  return {prevPos, *pos};
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

  vel.y += 0.8f * (isUpright ? 1.0f : -1.0f);

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

  *reticlePos = *pos + (isUpright ? -3.0f : 3.0f) * SPRITE_DIM 
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

  PUSH_EVENT(Event::Type::playerJump, {.value = charID});
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

  PUSH_EVENT(Event::Type::playerSuper, {.value = charID});
}

void Character::Land()
{
  curState = finalJump ? State::dead : State::idle;

  grounded = true;
  vel = ZERO_VECTOR;
  superBouncesLeft = -1;

  entity.SetAnimation(LAND_ANIM, 100, 0, 300);
  entity.PushAnimation(finalJump ? REST_ANIM : IDLE_ANIM, 150);

  ParticleManager::GetInstance()->CreateParticle(
    Dust(*pos, 
         !isUpright, 
         (int)Settings::GetInstance()->GetPlayerColour(playerNum)));

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

PlayableCharacter::PlayableCharacter(int charID, int playerNum)
  : 
  Character(charID, playerNum)
{}

void PlayableCharacter::Update()
{
  if (curState == State::dead)
  {
    Character::Update();
    return;
  }

  if (Settings::GetInstance()->IsActionOnInitialClick(Controls::Action::jump, charID))
    Jump();

  else if(Settings::GetInstance()->IsActionOnInitialClick(Controls::Action::special, charID))
    SuperJump();

  horiDir = Settings::GetInstance()->IsActionHeld(Controls::Action::right, charID) 
          - Settings::GetInstance()->IsActionHeld(Controls::Action::left, charID);

  Character::Update();
}

// ------------------
// Computer Character
// ------------------

ComputerCharacter::ComputerCharacter(int charID, int playerNum)
  :
  Character(charID, playerNum)
{}

void ComputerCharacter::Update()
{
  if (curState == State::dead)
  {
    Character::Update();
    return;
  }

  decisionTimer -= DELTA_TIME;
  
  while (decisionTimer <= 0)
  {
    decisionTimer += 16;
    std::uniform_int_distribution p(0, 99);
    int rand = p(Utility::GetInstance()->GetRNG());

    if (rand == 99)
      Jump();
    else if (rand / 3 < 3)
      horiDir = -1 + rand / 3;
    // Else retain current movement
  }

  Character::Update();
}