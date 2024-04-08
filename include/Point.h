#ifndef POINT_H
#define POINT_H

#define NUM_TEX_WIDTH 5
#define NUM_TEX_HEIGHT 6

#include <SFML/Graphics.hpp>

#include "Utility.h"

#include <string>
#include <vector>

// Represents a mini score from defeating an enemy, when the player lands, Score accumulates these points into the total score
class Point
{    
public:
    Point() = delete;
    // Constructor that takes a point value to determine which digits should be rendered, as well as a position and velocity
    Point(int value, sf::Vector2f pos, sf::Vector2f vel);
    // Constructor that takes a point value, a position,
    Point(int value, sf::Vector2f pos, sf::Int32 waitToRender, sf::Int32 dur);
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
    static void RenderPoints(Point* points, sf::RenderWindow* win);
    // Gets the total score of all the points currently in the list
    static unsigned int GetTotalScore(Point* points);
    // Gets the average position of all points currently in the list
    static sf::Vector2f GetAveragePosition(Point* points);
    // Sets a position that all points currently in the list will move to
    static void SetPointsDestination(Point* points, sf::Vector2f dest, sf::Int32 dur);

private:
    // Updates this and the next in line point's position, and determines whether it has reached the end of its lifespan
    void Update();
    // Renders this and the next in line point's current sprite
    void Render(sf::RenderWindow* win) const;
    // Gets the point's value
    void AddValue(unsigned int* total) const;
    // Adds the point's current position to a given position point
    void AddPosition(sf::Vector2f* pos, int* count) const;
    // Sets the velocity of the point so that it will move to a given destination for a set duration
    void SetDestination(sf::Vector2f& dest, sf::Int32& dur);

public:
    static sf::Texture tex; // The texture used by all the points

private:
    Point* nextPoint = nullptr;   // A pointer to the next point in the list

    int value = 0;  // The value of this point

    std::vector<sf::Sprite> sprites;    // The point's sprites, each one represents a digit
    sf::Vector2f pos;                   // The overall centre position of the point
    sf::Vector2f vel;                   // The point's velocity
    sf::Int32 waitToRender = 0;         // The time at which the point should begin rendering

    bool hasFinished = false;   // Whether or not the point has reached the end of its lifespan
    sf::Int32 EndOfLifespan;    // The global time at which the point will reach the end of its lifespan

    bool isMovingToDestination = false; // Whether a call has been made to begin moving the point to a destination
    sf::Int32 waitTime = 0.0f;          // The amount of time to wait before moving the point to its destination
};

#endif