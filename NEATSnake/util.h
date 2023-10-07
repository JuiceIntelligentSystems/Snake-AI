#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <SDL2/SDL.h>

struct Vector2d
{
    int x;
    int y;
};

struct Intersection
{
    Vector2d p;
    Vector2d s1, s2;

    int distance = 0;
};

Intersection findIntersection(Vector2d line_start, Vector2d line_end, Vector2d squarep_a, Vector2d squarep_b, Vector2d squarep_c, Vector2d squarep_d)
{
    // Calculate Slope of the line
    int m = (line_end.y - line_start.y) / (line_end.x - line_start.x);
    int c = line_start.y - m * line_start.x;

    int x5 = squarep_c.x, x6 = squarep_d.x;
    int y1 = squarep_a.y, y2 = squarep_c.y, y3 = squarep_d.y, y4 = squarep_d.y;

    Vector2d p = {0, 0};

    // Line Intersects Top
    if (line_start.y <= y1 && line_end.y >= y1 && m != 0)
    {
        int x = (y1 - c) / m;
        if (x >= x5 && x <= x6)
        {
            p = {x, y1};
        }
    }

    // Line Intersects Left
    if (p.x == 0 && line_start.x <= x5 && line_end.x >= x5 && m != INFINITY)
    {
        int y = m * x5 + c;
        if (y >= y1 && y <= y2)
        {
            p = {x5, y};
        }
    }

    // Line Intersects Right
    if (p.x == 0 && line_start.x <= x6 && line_end.x >= x6 && m != INFINITY)
    {
        int y = m * x6 + c;
        if (y >= y1 && y <= y2)
        {
            p = {x6, y};
        }
    }

    // Line Intersects Bottom
    if (line_start.y >= y3 && line_end.y <= y3 && m != 0)
    {
        int x = (y3 - c) / m;
        if (x >= x5 && x <= x6)
        {
            p = {x, y3};
        }
    }

    // Calculate Segment end Points
    Vector2d s1, s2;
    if (p.x != 0 || p.y != 0)
    {
        s1 = line_start.y < line_end.y ? line_start : line_end;
        s2 = line_start.y < line_end.y ? line_end : line_start;
    }

    int d = sqrt((p.x - line_start.x)*(p.x - line_start.x) + (p.y - line_start.y)*(p.y - line_start.y));

    return {p, s1, s2, d};
}

#endif