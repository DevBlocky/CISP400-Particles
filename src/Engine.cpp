#include "Engine.h"
#include <random>
#include "brand.h"

using namespace sf;

static random_device rngdev;

Engine::Engine() : m_Window(VideoMode(1920, 1080), "Particles") {}

void Engine::run() {
    Clock frameTimeClock;

    // particle unit tests
    cout << "Starting Particle unit tests..." << endl;
    Particle p(m_Window, 4, {(int) m_Window.getSize().x / 2, (int) m_Window.getSize().y / 2});
    p.unitTests();
    cout << "Unit tests complete. Starting engine..." << endl;

    while (m_Window.isOpen()) {
        // find the delta time from last frame
        auto delta = frameTimeClock.restart();
        auto deltaS = delta.asSeconds();

        input();
        update(deltaS);
        draw();
    }
}

void Engine::input() {
    Event ev{};
    while (m_Window.pollEvent(ev)) {
        // click close || escape pressed
        if (ev.type == Event::Closed || (ev.type == Event::KeyPressed && ev.key.code == Keyboard::Escape)) {
            m_Window.close();
        }

        // left click
        if (ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Left) {
            // create 5 particles at mouse position
            for (int i = 0; i < 5; i++) {
                int numPoints = brand::rand_int(25, 50);
                m_particles.emplace_back(
                        Particle(m_Window, numPoints, {ev.mouseButton.x, ev.mouseButton.y})
                );
            }
        }
    }
}

void Engine::update(float dt) {
    for (auto it = m_particles.begin(); it != m_particles.end();) {
        // particle is still allowed to live >:D
        if (it->getTTL() > 0.0) {
            it++->update(dt);
        } else {
            // particle is dead to us
            it = m_particles.erase(it);
        }
    }
}

void Engine::draw() {
    m_Window.clear();
    for (const Particle &p: m_particles) {
        m_Window.draw(p);
    }
    m_Window.display();
}
