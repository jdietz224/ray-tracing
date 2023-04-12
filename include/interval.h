#pragma once

#include "rtweekend.h"

namespace raytrace {
    class interval {
    public:
        interval() : min(+infinity), max(-infinity) {} // Default interval is empty

        interval(double _min, double _max) : min(_min), max(_max) {}

        bool contains(double x) const {
            return min <= x && x <= max;
        }

        double clamp(double x) const {
            return (x < min) ? (min) : 
                ((x > max) ? (max) : (x));
        }

        static const interval empty, universe;

    public:
        double min, max;
    };

    const static interval empty   (+infinity, -infinity);
    const static interval universe(-infinity, +infinity);
}