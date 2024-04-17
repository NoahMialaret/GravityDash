#ifndef POINT_HPP
#define POINT_HPP

#define NUM_TEX_WIDTH 5
#define NUM_TEX_HEIGHT 6

#include "Sprite.hpp"
#include "Texture.hpp"
#include "Utility.hpp"

#include <string>
#include <vector>

// Represents a mini score from defeating an enemy, when the player lands, Score accumulates these points into the total score
class Point
{    
public:
    Point() = delete;
    // Constructor that takes a point value to determine which digits should be rendered, as well as a position and velocity
    Point(int value, fVec2 pos, fVec2 vel);
    // Constructor that takes a point value, a position,
    Point(int value, fVec2 pos, double waitToRender, double dur);
    // Frees up dynamic memory by deleting the point once the points after this have also been deleted
    void Delete();

    // Returns whether the point has reached the end of its lifespan
    bool HasFinished() const;
    // Returns a pointer to the next point in the list
    Point* GetNextPoint() const;

    // Appends a new point at the end of the list
    static void CreateNewPoint(Point*& points, Point* newPoint);
    // Function to be called outside this class to call Delete() for all points
    static void DeletePoints(Point*& points);
    // Function to be called outside this class to call Update() for all points
    static void UpdatePoints(Point*& points);
    // Function to be called outside this class to call Render() for all points
    static void RenderPoints(Point* points);
    // Gets the total score of all the points currently in the list
    static unsigned int GetTotalScore(Point* points);
    // Gets the average position of all points currently in the list
    static fVec2 GetAveragePosition(Point* points);
    // Sets a position that all points currently in the list will move to
    static void SetPointsDestination(Point* points, fVec2 dest, double dur);

private:
    // Updates this and the next in line point's position, and determines whether it has reached the end of its lifespan
    void Update();
    // Renders this and the next in line point's current sprite
    void Render() const;
    // Gets the point's value
    void AddValue(unsigned int* total) const;
    // Adds the point's current position to a given position point
    void AddPosition(fVec2* pos, int* count) const;
    // Sets the velocity of the point so that it will move to a given destination for a set duration
    void SetDestination(fVec2& dest, double& dur);

public:
    static Texture tex; // The texture used by all the points

private:
    Point* nextPoint = nullptr;   // A pointer to the next point in the list

    int value = 0;  // The value of this point

    std::vector<Sprite> sprites;    // The point's sprites, each one represents a digit
    fVec2 pos;                   // The overall centre position of the point
    fVec2 vel;                   // The point's velocity
    double waitToRender = 0;         // The time at which the point should begin rendering

    bool hasFinished = false;   // Whether or not the point has reached the end of its lifespan
    double EndOfLifespan;    // The global time at which the point will reach the end of its lifespan

    bool isMovingToDestination = false; // Whether a call has been made to begin moving the point to a destination
    double waitTime = 0.0f;          // The amount of time to wait before moving the point to its destination
};

#endif