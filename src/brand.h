#pragma once

#include <SFML/Graphics.hpp>

namespace brand {
    // creates a random integer, [min:max]
    int rand_int(int min, int max);

    // creates a random floating point number
    float rand_float(float min = 0.0, float max = 1.0);

    sf::Color rand_color(int a = 255);
}
