#include "Point.hpp"

Texture Point::tex;

Point::Point(int value, fVec2 pos, fVec2 vel)
    :
    value(value),
    pos(pos),
    vel(vel)
{
  bool isNegative = value < 0;

    std::string digits = Utility::IntToString(abs(value));
    IRect texRect(0, 0, NUM_TEX_WIDTH, NUM_TEX_HEIGHT);
    fVec2 spritePos = pos;
    float posOffset = (Utility::gameScale / 2) * (NUM_TEX_WIDTH - 1);
    spritePos.x = pos.x - (digits.size() - 1 + isNegative) * (posOffset / 2.0f);

    if (isNegative)
    {
        Sprite temp;

        temp.SetTexture(&tex);
        texRect.left = tex.GetSize().x - NUM_TEX_WIDTH;
        temp.SetTextureBounds(texRect);

        temp.SetOrigin(CENTRED_ORIGIN);
        temp.SetTranslation(spritePos);
        temp.SetScale(DEFAULT_SCALE * 0.5f);

        sprites.push_back(temp);

        spritePos.x += posOffset;
    }

    for (auto& d : digits)
    {
        Sprite temp;

        temp.SetTexture(&tex);
        texRect.left = (d - 48) * NUM_TEX_WIDTH;
        temp.SetTextureBounds(texRect);

        temp.SetOrigin(CENTRED_ORIGIN);
        temp.SetTranslation(spritePos);
        temp.SetScale(DEFAULT_SCALE * 0.5f);

        sprites.push_back(temp);

        spritePos.x += posOffset;
    }
}

Point::Point(int value, fVec2 pos, double waitToRender, double dur)
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

void Point::RenderPoints(Point* points)
{
    if (points != nullptr)
    {
        points->Render();
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

fVec2 Point::GetAveragePosition(Point* points)
{
    if (points == nullptr)
    {
        return {0.0f, 0.0f};
    }

    fVec2 accumulatedPosition = ZERO_VECTOR;
    int numPoints = 0;

    points->AddPosition(&accumulatedPosition, &numPoints);

    fVec2 averagePos = accumulatedPosition * (1.0f / (float)numPoints);

    return averagePos;
}

void Point::SetPointsDestination(Point* points, fVec2 dest, double dur)
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
        s.Translate(vel);
    }

    pos += vel;

    if (!isMovingToDestination)
    {
        //vel *= 0.99f;
    }
}

void Point::Render() const
{
    if (waitToRender < CUR_TIME)
    {
        for (auto& s : sprites)
        {
            s.Render();
        }
    }

    if (nextPoint != nullptr)
    {
        nextPoint->Render();
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

void Point::AddPosition(fVec2* pos, int* count) const
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

void Point::SetDestination(fVec2& dest, double& dur)
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

    fVec2 difference = dest - pos;
    vel = difference * (1.0f / (float)dur) * 17;
    EndOfLifespan = dur + 500 + CUR_TIME;
    isMovingToDestination = true;
}