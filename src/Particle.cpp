#include "Particle.h"
#include "brand.h"

static Vector2f vec2i_to_vec2f(const Vector2i &veci) {
    return {static_cast<float>(veci.x), static_cast<float>(veci.y)};
}

Particle::Particle(sf::RenderTarget &target, int numPoints, sf::Vector2i mouseClickPosition) : m_A(2, numPoints) {
    // setup the view
    m_cartesianPlane.setCenter(0.0, 0.0);
    m_cartesianPlane.setSize(static_cast<float>(target.getSize().x), -static_cast<float>(target.getSize().y));

    // initialize variables
    m_ttl = TTL;
    m_numPoints = numPoints;
    m_radiansPerSec = brand::rand_float(0.0, M_PI);
    m_centerCoordinate = target.mapPixelToCoords(mouseClickPosition, m_cartesianPlane);
    m_vx = brand::rand_float(-500.0, 500.0);
    m_vy = brand::rand_float(100.0, 500.0);

    m_color1 = brand::rand_color();
    m_color2 = brand::rand_color();

    // create vertices
    float theta = brand::rand_float(0.0, static_cast<float>(M_PI / 2.0));
    float dTheta = 2 * M_PI / static_cast<float>(numPoints - 1);
    for (int j = 0; j < numPoints; j++) {
        float r = brand::rand_float(20.0, 80.0);
        float dx = r * cos(theta);
        float dy = r * sin(theta);
        theta += dTheta;

        m_A(0, j) = m_centerCoordinate.x + dx;
        m_A(1, j) = m_centerCoordinate.y + dy;
    }
}

void Particle::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    VertexArray lines(TriangleFan, m_numPoints + 1);
    auto center = vec2i_to_vec2f(target.mapCoordsToPixel(m_centerCoordinate, m_cartesianPlane));

    lines[0].position = center;
    lines[0].color = m_color1;
    for (int j = 1; j <= m_numPoints; j++) {
        float posX = m_A(0, j - 1);
        float posY = m_A(1, j - 1);
        lines[j].position = vec2i_to_vec2f(target.mapCoordsToPixel({posX, posY}, m_cartesianPlane));
        lines[j].color = m_color2;
    }

    target.draw(lines);
}

void Particle::update(float dt) {
    m_ttl -= dt;
    rotate(dt * m_radiansPerSec);
    scale(SCALE);

    m_vy -= G * dt;
    float dx = m_vx * dt;
    float dy = m_vy * dt;
    translate(dx, dy);
}

void Particle::translate(double xShift, double yShift) {
    TranslationMatrix t(xShift, yShift, m_numPoints);
    m_A = t + m_A;
    m_centerCoordinate.x += static_cast<float>(xShift);
    m_centerCoordinate.y += static_cast<float>(yShift);
}

void Particle::rotate(double theta) {
    // make origin around centerCoordinate
    Vector2f temp = m_centerCoordinate;
    translate(-temp.x, -temp.y);

    // rotate
    RotationMatrix r(theta);
    m_A = r * m_A;

    // translate back to original place
    translate(temp.x, temp.y);
}

void Particle::scale(double c) {
    // make origin around centerCoordinate
    Vector2f temp = m_centerCoordinate;
    translate(-temp.x, -temp.y);

    ScalingMatrix s(c);
    m_A = s * m_A;

    // translate back to original place
    translate(temp.x, temp.y);
}

bool Particle::almostEqual(double a, double b, double eps) {
    return fabs(a - b) < eps;
}

void Particle::unitTests() {
    int score = 0;

    cout << "Testing RotationMatrix constructor...";
    double theta = M_PI / 4.0;
    RotationMatrix r(M_PI / 4);
    if (r.getRows() == 2 && r.getCols() == 2 && almostEqual(r(0, 0), cos(theta))
        && almostEqual(r(0, 1), -sin(theta))
        && almostEqual(r(1, 0), sin(theta))
        && almostEqual(r(1, 1), cos(theta))) {
        cout << "Passed.  +1" << endl;
        score++;
    } else {
        cout << "Failed." << endl;
    }

    cout << "Testing ScalingMatrix constructor...";
    ScalingMatrix s(1.5);
    if (s.getRows() == 2 && s.getCols() == 2
        && almostEqual(s(0, 0), 1.5)
        && almostEqual(s(0, 1), 0)
        && almostEqual(s(1, 0), 0)
        && almostEqual(s(1, 1), 1.5)) {
        cout << "Passed.  +1" << endl;
        score++;
    } else {
        cout << "Failed." << endl;
    }

    cout << "Testing TranslationMatrix constructor...";
    TranslationMatrix t(5, -5, 3);
    if (t.getRows() == 2 && t.getCols() == 3
        && almostEqual(t(0, 0), 5)
        && almostEqual(t(1, 0), -5)
        && almostEqual(t(0, 1), 5)
        && almostEqual(t(1, 1), -5)
        && almostEqual(t(0, 2), 5)
        && almostEqual(t(1, 2), -5)) {
        cout << "Passed.  +1" << endl;
        score++;
    } else {
        cout << "Failed." << endl;
    }


    cout << "Testing Particles..." << endl;
    cout << "Testing Particle mapping to Cartesian origin..." << endl;
    if (m_centerCoordinate.x != 0 || m_centerCoordinate.y != 0) {
        cout << "Failed.  Expected (0,0).  Received: (" << m_centerCoordinate.x << "," << m_centerCoordinate.y << ")"
             << endl;
    } else {
        cout << "Passed.  +1" << endl;
        score++;
    }

    cout << "Applying one rotation of 90 degrees about the origin..." << endl;
    Matrix initialCoords = m_A;
    rotate(M_PI / 2.0);
    bool rotationPassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++) {
        if (!almostEqual(m_A(0, j), -initialCoords(1, j)) || !almostEqual(m_A(1, j), initialCoords(0, j))) {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", "
                 << m_A(1, j) << ")" << endl;
            rotationPassed = false;
        }
    }
    if (rotationPassed) {
        cout << "Passed.  +1" << endl;
        score++;
    } else {
        cout << "Failed." << endl;
    }

    cout << "Applying a scale of 0.5..." << endl;
    initialCoords = m_A;
    scale(0.5);
    bool scalePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++) {
        if (!almostEqual(m_A(0, j), 0.5 * initialCoords(0, j)) || !almostEqual(m_A(1, j), 0.5 * initialCoords(1, j))) {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", "
                 << m_A(1, j) << ")" << endl;
            scalePassed = false;
        }
    }
    if (scalePassed) {
        cout << "Passed.  +1" << endl;
        score++;
    } else {
        cout << "Failed." << endl;
    }

    cout << "Applying a translation of (10, 5)..." << endl;
    initialCoords = m_A;
    translate(10, 5);
    bool translatePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++) {
        if (!almostEqual(m_A(0, j), 10 + initialCoords(0, j)) || !almostEqual(m_A(1, j), 5 + initialCoords(1, j))) {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", "
                 << m_A(1, j) << ")" << endl;
            translatePassed = false;
        }
    }
    if (translatePassed) {
        cout << "Passed.  +1" << endl;
        score++;
    } else {
        cout << "Failed." << endl;
    }

    cout << "Score: " << score << " / 7" << endl;
}