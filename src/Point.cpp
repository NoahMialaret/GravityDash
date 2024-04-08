#include "Point.h"

sf::Texture Point::tex;

Point::Point(int value, sf::Vector2f pos, sf::Vector2f vel)
    :
    value(value),
    pos(pos),
    vel(vel)
{
  bool isNegative = value < 0;

    std::string digits = Utility::IntToString(abs(value));
    sf::IntRect texRect(0, 0, NUM_TEX_WIDTH, NUM_TEX_HEIGHT);
    sf::Vector2f spritePos = pos;
    float posOffset = (Utility::gameScale / 2) * (NUM_TEX_WIDTH - 1);
    spritePos.x = pos.x - (digits.size() - 1 + isNegative) * (posOffset / 2.0f);

    if (isNegative)
    {
        sf::Sprite temp;

        temp.setTexture(tex);
        texRect.left = tex.getSize().x - NUM_TEX_WIDTH;
        temp.setTextureRect(texRect);

        temp.setOrigin(CENTRED_ORIGIN);
        temp.setPosition(spritePos);
        temp.setScale(0.5f * DEFAULT_SCALE);

        sprites.push_back(temp);

        spritePos.x += posOffset;
    }

    for (auto& d : digits)
    {
        sf::Sprite temp;

        temp.setTexture(tex);
        texRect.left = (d - 48) * NUM_TEX_WIDTH;
        temp.setTextureRect(texRect);

        temp.setOrigin(CENTRED_ORIGIN);
        temp.setPosition(spritePos);
        temp.setScale(0.5f * DEFAULT_SCALE);

        sprites.push_back(temp);

        spritePos.x += posOffset;
    }
}

Point::Point(int value, sf::Vector2f pos, sf::Int32 waitToRender, sf::Int32 dur)
    :
    Point(value, pos, ZERO_VECTOR)
{
    isMovingToDestination = true;
    EndOfLifespan = waitToRender + dur + CUR_TIME;
    this->waitToRender = waitToRender + CUR_TIME;
}

void Point::Delete()
{
    if (nextPoint != nullptr)
    {
        nextPoint->Delete();
        delete nextPoint;
    }
}

bool Point::HasFinished() const
{
    return hasFinished;
}

Point* Point::GetNextPoint() const
{
    return nextPoint;
}

// ----------------
// Static functions
// ----------------

void Point::CreateNewPoint(Point*& points, Point* newPoint)
{
    if (points == nullptr)
    {
        points = newPoint;
        return;
    }

    Point* endOfList = points;

    while(true)
    {
        if (endOfList->nextPoint == nullptr)
        {
            endOfList->nextPoint = newPoint;
            break;
        }

        endOfList = endOfList->nextPoint;
    }
}

void Point::DeletePoints(Point*& points)
{
    if (points != nullptr)
    {
        points->Delete();
        delete points;
        points = nullptr;
    }
}

void Point::UpdatePoints(Point*& points)
{
	if (points == nullptr)
	{
        return;
    }

    points->Update();

    if (points->HasFinished())
    {
        Point* temp = points->GetNextPoint();
        delete points;
        points = temp;
    }
}

void Point::RenderPoints(Point* points, sf::RenderWindow* win)
{
    if (points != nullptr)
    {
        points->Render(win);
    }
}

unsigned int Point::GetTotalScore(Point* points)
{
    unsigned int total = 0;

    if (points != nullptr)
    {
        points->AddValue(&total);
    }

    return total;
}

sf::Vector2f Point::GetAveragePosition(Point* points)
{
    if (points == nullptr)
    {
        return {0.0f, 0.0f};
    }

    sf::Vector2f accumulatedPosition = ZERO_VECTOR;
    int numPoints = 0;

    points->AddPosition(&accumulatedPosition, &numPoints);

    sf::Vector2f averagePos = (1.0f / (float)numPoints) * accumulatedPosition;

    return averagePos;
}

void Point::SetPointsDestination(Point* points, sf::Vector2f dest, sf::Int32 dur)
{
    if (points == nullptr)
    {
        return;
    }

    points->SetDestination(dest, dur);
}

// -----------------
// Private Functions
// -----------------

void Point::Update()
{
    if (nextPoint != nullptr)
    {
        nextPoint->Update();

        if (nextPoint->HasFinished())
        {
            Point* temp = nextPoint->GetNextPoint();
            delete nextPoint;
            nextPoint = temp;
        }
    }

    if (isMovingToDestination && EndOfLifespan < CUR_TIME)
    {
        hasFinished = true;
        return;
    }

    if (isMovingToDestination && waitTime > CUR_TIME)
    {
        return;
    }

    for (auto& s : sprites)
    {
        s.move(vel);
    }

    pos += vel;

    if (!isMovingToDestination)
    {
        //vel *= 0.99f;
    }
}

void Point::Render(sf::RenderWindow* win) const
{
    if (waitToRender < CUR_TIME)
    {
        for (auto& s : sprites)
        {
            win->draw(s);
        }
    }

    if (nextPoint != nullptr)
    {
        nextPoint->Render(win);
    }
}

void Point::AddValue(unsigned int* total) const
{
    if (!isMovingToDestination)
    {
        *total += value;
    }

    if (nextPoint != nullptr)
    {
        nextPoint->AddValue(total);
    }
}

void Point::AddPosition(sf::Vector2f* pos, int* count) const
{
    if (!isMovingToDestination)
    {
        *pos += this->pos;
        (*count)++;
    }

    if (nextPoint != nullptr)
    {
        nextPoint->AddPosition(pos, count);
    }
}

void Point::SetDestination(sf::Vector2f& dest, sf::Int32& dur)
{
    if (nextPoint != nullptr)
    {
        nextPoint->SetDestination(dest, dur);
    }

    if (isMovingToDestination)
    {
        return;
    }

    waitTime = 500 + CUR_TIME;

    sf::Vector2f difference = dest - pos;
    vel = (1.0f / (float)dur) * 17 * difference;
    EndOfLifespan = dur + 500 + CUR_TIME;
    isMovingToDestination = true;
}