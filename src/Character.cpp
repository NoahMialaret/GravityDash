#include "Character.hpp"

Character::Character(Texture* tex)
  : 
  acceleration(0.2f * Utility::gameScale)
{
  // if (!tex.loadFromFile(spritePath)) 
  // {
  //     std::cout << "Character texture could not be loaded!\n";
  // }

  sprite.SetTexture(tex);
  sprite.SetOrigin(CENTRED_ORIGIN);

  sprite.SetScale({Utility::gameScale, -Utility::gameScale});

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
  
  prevPos = sprite.GetTranslation();

  switch (curState)
  {
  case State::idle:
    if (move == 0)
    {
      break;
    }

    curState = State::moving;

    anim.ChangeAnimation((int)curState, (isLastStand ? 3.0f : 1.0f) * 0.100);
    sprite.SetScale({move * Utility::gameScale, sprite.GetScale().y});

    nextRunParticle = (isLastStand ? 4.0f : 1.0f) * 0.150 + CUR_TIME;
    break;

  case State::moving:
    if (vel == ZERO_VECTOR && move == 0)
    {
      curState = State::idle;
      anim.ChangeAnimation((int)curState, (isLastStand ? 2.0f : 1.0f) * 0.150);
      break;
    }

    if (move != 0 && move != Utility::GetSign(sprite.GetScale().x))
    {
      sprite.Scale({-1.0f, 1.0f});
    }

    if (nextRunParticle < CUR_TIME)
    {
      fVec2 partVel(-move * 0.2f * Utility::gameScale, (isUpright ? -0.1f : 0.1f) * Utility::gameScale);
      Particle* temp = new Particle(Particle::Type::walkDust, partVel, sprite.GetTranslation() - 
        fVec2(move * 0.5f * SCALED_DIM, 0.0f));
      Particle::CreateNewParticle(temp);
      nextRunParticle += (isLastStand ? 4.0f : 1.0f) * 0.150;
    }
    break;

  case State::airborne:
    break;
  
  default:
    std::cout << "Could not determine Character state (" << (int)curState << ")\n";
    break;
  }

  sprite.Translate(vel * (isLastStand ? 0.5f : 1.0f));
  anim.Update();
}

void Character::Render() const
{
  //win->draw(sprite, &Utility::shaderTest);
  sprite.Render();

  Point::RenderPoints(points);
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

  anim.ChangeAnimation((int)State::airborne + 1, 0.100, 0, (int)curState, 0.150, 0.300);
  sprite.Scale({1.0f, -1.0f});

  fVec2 partVel = {Utility::gameScale * 0.3f, 0.0f};
  fVec2 offset = {0.5f * SCALED_DIM, 0.0f};

  Particle* temp = new Particle(Particle::Type::landingImpact, partVel, sprite.GetTranslation() + offset, {fabsf(sprite.GetScale().x), sprite.GetScale().y});
  Particle::CreateNewParticle(temp);
  temp = new Particle(Particle::Type::landingImpact, partVel * -1.0f, sprite.GetTranslation() - offset, {-fabsf(sprite.GetScale().x), sprite.GetScale().y});
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

	fVec2 avPos = Point::GetAveragePosition(points);

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

fVec2 Character::GetPosition() const
{
    return sprite.GetTranslation();
}

void Character::SetPosition(fVec2& newPos)
{
    vel.x = 0.0f;
    move = 0;
    sprite.SetTranslation(newPos);
}

FRect Character::GetHitBox() const
{
    return sprite.GetBoundingRectangle();
}

std::pair<fVec2, fVec2> Character::GetLineHitBox() const
{
    return std::pair<fVec2, fVec2>(prevPos, sprite.GetTranslation());
}

void Character::IncrementComboCount()
{
  comboCount++;
}

void Character::AddNewPoint(fVec2 pos, fVec2 vel)
{
  int value = 50 * (int)pow(2, comboCount);
  if (value >= 25600) // The score from hitting 10th target in a row
  {
    value = 25600;
    // Goldenpoint++; a record of how many times this value was reached
  }

  AddNewPoint(value, pos, vel);
}

void Character::AddNewPoint(int value, fVec2 pos, fVec2 vel)
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

PlayableCharacter::PlayableCharacter(Texture* tex, std::unique_ptr<Controls>& controls)
    : 
    Character(tex)
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

ComputerCharacter::ComputerCharacter(Texture* tex)
  :
  Character(tex)
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