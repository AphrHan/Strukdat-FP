#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <random>
#include <iostream>

struct Vec {
    float x, y;
    Vec() : x(0), y(0) {}
    Vec(float x_, float y_) : x(x_), y(y_) {}
    Vec operator+(const Vec& o) const { return Vec(x+o.x, y+o.y); }
    Vec operator-(const Vec& o) const { return Vec(x-o.x, y-o.y); }
    Vec operator*(float s) const { return Vec(x*s, y*s); }
};

inline float dot(const Vec& a, const Vec& b) { return a.x*b.x + a.y*b.y; }
inline float len(const Vec& a) { return std::sqrt(a.x*a.x + a.y*a.y); }

struct Ball {
    Vec pos;
    Vec vel;
    float radius;
    float mass;
    sf::Color color;
};

struct AABB {
    float x, y, w, h;
    bool contains(const Ball* b) const {
        return (b->pos.x >= x - w && b->pos.x <= x + w &&
                b->pos.y >= y - h && b->pos.y <= y + h);
    }
};

struct Quadtree {
    static const int CAPACITY = 6;
    AABB boundary;
    std::vector<const Ball*> points;
    bool divided = false;
    Quadtree *ne, *nw, *se, *sw;

    Quadtree(float x, float y, float w, float h)
        : boundary({x,y,w,h}), ne(nullptr), nw(nullptr), se(nullptr), sw(nullptr) {}

    ~Quadtree() { delete ne; delete nw; delete se; delete sw; }

    void subdivide() {
        ne = new Quadtree(boundary.x + boundary.w/2, boundary.y - boundary.h/2, boundary.w/2, boundary.h/2);
        nw = new Quadtree(boundary.x - boundary.w/2, boundary.y - boundary.h/2, boundary.w/2, boundary.h/2);
        se = new Quadtree(boundary.x + boundary.w/2, boundary.y + boundary.h/2, boundary.w/2, boundary.h/2);
        sw = new Quadtree(boundary.x - boundary.w/2, boundary.y + boundary.h/2, boundary.w/2, boundary.h/2);
        divided = true;
    }

    bool insert(const Ball* b) {
        if (!boundary.contains(b)) return false;
        if (points.size() < CAPACITY) {
            points.push_back(b);
            return true;
        }
        if (!divided) subdivide();
        return (ne->insert(b) || nw->insert(b) || se->insert(b) || sw->insert(b));
    }

    void query(const AABB &range, std::vector<const Ball*> &found) const {
        if (!(range.x + range.w < boundary.x - boundary.w ||
              range.x - range.w > boundary.x + boundary.w ||
              range.y + range.h < boundary.y - boundary.h ||
              range.y - range.h > boundary.y + boundary.h)) {
            for (auto b : points)
                if (range.contains(b)) found.push_back(b);
            if (divided) {
                ne->query(range, found);
                nw->query(range, found);
                se->query(range, found);
                sw->query(range, found);
            }
        }
    }
};


std::mt19937 rng((unsigned)std::random_device{}());
std::uniform_real_distribution<float> uni(0, 1);

std::vector<sf::Color> palette = {
    sf::Color(255, 0, 0),       // merah
    sf::Color(255, 165, 0),     // orange
    sf::Color(255, 105, 180)    // pink
};
std::uniform_int_distribution<int> pidx(0, (int)palette.size() - 1);

void resolveCollision(Ball &a, Ball &b) {
    Vec n = a.pos - b.pos;
    float d = len(n);
    float penetration = (a.radius + b.radius) - d;
    if (penetration > 0 && d != 0) {
        Vec normal = n * (1.f / d);
        float total = a.mass + b.mass;
        a.pos = a.pos + normal * (penetration * (b.mass/total));
        b.pos = b.pos - normal * (penetration * (a.mass/total));

        Vec rv = a.vel - b.vel;
        float velNorm = dot(rv, normal);
        if (velNorm > 0) return;
        float j = -(1 + 1.f) * velNorm / (1/a.mass + 1/b.mass);
        Vec impulse = normal * j;
        a.vel = a.vel + impulse * (1/a.mass);
        b.vel = b.vel - impulse * (1/b.mass);
    }
}

int main(){
    const int W = 1000, H = 700;
    sf::RenderWindow win(sf::VideoMode(W, H), "Brute Force vs Quadtree — Collision C++");
    win.setFramerateLimit(120);

    std::vector<Ball> balls;
    std::uniform_real_distribution<float> rx(60, W-60), ry(60, H-60), rv(-150,150), rr(12,28);

    for(int i=0;i<200;i++){
        float r = rr(rng);
        Ball b;
        b.pos = {rx(rng), ry(rng)};
        b.vel = {rv(rng), rv(rng)};
        b.radius = r;
        b.mass = r * 0.9f;
        b.color = palette[pidx(rng)]; // pilih salah satu dari 3 warna
        balls.push_back(b);
    }

    for (auto &b : balls) b.color = palette[pidx(rng)];

    bool bruteForce = true;
    sf::Font font; font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");

    while(win.isOpen()){
        sf::Event e;
        while(win.pollEvent(e)){
            if(e.type == sf::Event::Closed) win.close();
            if(e.type == sf::Event::KeyPressed){
                if(e.key.code == sf::Keyboard::Escape) win.close();
                if(e.key.code == sf::Keyboard::Q) bruteForce = !bruteForce;
            }
            if(e.type == sf::Event::MouseButtonPressed){
                if(e.mouseButton.button == sf::Mouse::Left){
                    Ball b;
                    b.radius = rr(rng);
                    b.pos = {(float)e.mouseButton.x, (float)e.mouseButton.y};
                    b.vel = {rv(rng), rv(rng)};
                    b.mass = b.radius * 0.9f;
                    b.color = palette[pidx(rng)];
                    balls.push_back(b);
                }
            }
        }

        for(auto &b: balls){
            b.pos.x += b.vel.x * 0.016f;
            b.pos.y += b.vel.y * 0.016f;
            if (b.pos.x < b.radius || b.pos.x > W - b.radius) b.vel.x *= -1;
            if (b.pos.y < b.radius || b.pos.y > H - b.radius) b.vel.y *= -1;
        }

        if(bruteForce){
            for(size_t i=0;i<balls.size();i++)
                for(size_t j=i+1;j<balls.size();j++)
                    resolveCollision(balls[i], balls[j]);
        }
        else {
            Quadtree qt(W/2, H/2, W/2, H/2);
            for(auto &b: balls) qt.insert(&b);

            for(auto &b: balls){
                AABB range{b.pos.x, b.pos.y, b.radius*2, b.radius*2};
                std::vector<const Ball*> found;
                qt.query(range, found);
                for(auto other : found){
                    if(&b != other)
                        resolveCollision(b, *(Ball*)other);
                }
            }
        }

        win.clear(sf::Color(20,20,20));
        for(auto &b: balls){
            sf::CircleShape c(b.radius);
            c.setOrigin(b.radius, b.radius);
            c.setPosition(b.pos.x, b.pos.y);
            c.setFillColor(b.color);
            win.draw(c);
        }

        sf::Text t;
        t.setFont(font);
        t.setCharacterSize(15);
        t.setFillColor(sf::Color::White);
        t.setPosition(8,8);
        t.setString("Mode: " + std::string(bruteForce? "Brute Force" : "Quadtree") +
                    "  |  Balls: " + std::to_string(balls.size()) +
                    "  |  Click: Add Ball   |  Q: Toggle Mode");
        win.draw(t);

        win.display();
    }
    return 0;
}