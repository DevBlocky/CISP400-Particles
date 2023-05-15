#include "brand.h"
#include <random>

static std::default_random_engine gen;

int brand::rand_int(int min, int max) {
    std::uniform_int_distribution<int> distr(min, max);
    return distr(gen);
}

float brand::rand_float(float min, float max) {
    std::uniform_real_distribution<float> distr(min, max);
    return distr(gen);
}

sf::Color brand::rand_color(int a) {
    return sf::Color(rand_int(0, 255), rand_int(0, 255), rand_int(0, 255), a);
}
