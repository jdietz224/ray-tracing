#pragma once

namespace raytrace {
    class interval {
    public:
        interval() : min(+infinity), max(-infinity) {} // Default interval is empty

        interval(double _min, double _max) : min(_min), max(_max) {}

        bool contains(double x) const {
            return min <= x && x <= max;
        }

        static const interval empty, universe;

    public:
        double min, max;
    };

    const static interval empty   (+infinity, -infinity);
    const static interval universe(-infinity, +infinity);
}