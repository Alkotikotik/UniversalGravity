#include <iostream>
#include <cmath>
#include "raylib.h"
#include <cassert>
#include <utility>
#include <vector>
#include <string>
#include <array>
#include <raymath.h>
using namespace std;

//for matrices
using Matrix2x2 = std::vector<std::vector<double>>;

// The whole system class
class NbodySimulation {
public:
    //Constructor to set start objects
    NbodySimulation(const vector<vector<double>>& Xi,
        const vector<double>& masses,
        const vector<int>& radii,
        const vector<Color>& colors,
        double G = 6.674e-11)
        : Xi(Xi), masses(masses), radii(radii), colors(colors), G(G),
        pairs(combinations(Xi.size())),
        trails(Xi.size())
    {
        assert(Xi.size() == masses.size() && masses.size() == radii.size() && radii.size() == colors.size());
        for (const auto& x : Xi) {
            assert(x.size() == 6);
        }
    }

    //Useful variables
    int getN() const { return Xi.size(); }
    int totalCombs = Xi.size() - 1;

    // NOTE: This variable needs to be updated from main
    bool TwoD = false;

    Color AllColors[21] = {
       DARKGRAY, MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE, DARKBROWN,
       GRAY, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, PINK, YELLOW,
       GREEN, SKYBLUE, PURPLE, BEIGE };


    //Calculating pairs
    vector<pair<int, int>> combinations(int N)
    {
        vector<pair<int, int>> pairs;
        for (int i = 0; i < N; ++i) {
            for (int j = i + 1; j < N; ++j) {
                pairs.emplace_back(i, j);
            }
        }
        return pairs;
    }

    //Getting state derivative function 
    Matrix2x2 StateDir(const vector<vector<double>>& Xi, vector<std::pair<int, int>>& pairs)
    {
        const int D = 3;
        const int N = Xi.size();
        Matrix2x2 Xdot(N, vector<double>(6, 0.0));

        for (int i = 0; i < N; ++i) {
            Xdot[i][0] = Xi[i][3];
            Xdot[i][1] = Xi[i][4];
            Xdot[i][2] = Xi[i][5];
        }

        for (const auto& [i, j] : pairs)
        {
            double dx = Xi[j][0] - Xi[i][0];
            double dy = Xi[j][1] - Xi[i][1];
            double dz = Xi[j][2] - Xi[i][2];
            double r_squared = dx * dx + dy * dy + dz * dz;
            double r = sqrt(r_squared);
            double force_magnitude = G * masses[i] * masses[j] / r_squared;

            double fx = force_magnitude * dx / r;
            double fy = force_magnitude * dy / r;
            double fz = force_magnitude * dz / r;

            Xdot[i][3] += fx / masses[i];
            Xdot[i][4] += fy / masses[i];
            Xdot[i][5] += fz / masses[i];

            Xdot[j][3] -= fx / masses[j];
            Xdot[j][4] -= fy / masses[j];
            Xdot[j][5] -= fz / masses[j];
        }

        return Xdot;
    }


    //rk4 itegral
    Matrix2x2 rk4(vector<vector<double>>& Xi, const float dt, vector<pair<int, int>> pairs)
    {
        const int D = 3;
        int N = Xi.size();

        Matrix2x2 k1 = StateDir(Xi, pairs);
        Matrix2x2 temp = Xi;

        for (size_t i = 0; i < Xi.size(); ++i) {
            for (size_t j = 0; j < Xi[i].size(); ++j) {
                temp[i][j] = Xi[i][j] + k1[i][j] * (dt / 2);
            }
        }
        Matrix2x2 k2 = StateDir(temp, pairs);

        for (size_t i = 0; i < Xi.size(); ++i) {
            for (size_t j = 0; j < Xi[i].size(); ++j) {
                temp[i][j] = Xi[i][j] + k2[i][j] * (dt / 2);
            }
        }
        Matrix2x2 k3 = StateDir(temp, pairs);

        for (size_t i = 0; i < Xi.size(); ++i) {
            for (size_t j = 0; j < Xi[i].size(); ++j) {
                temp[i][j] = Xi[i][j] + k3[i][j] * dt;
            }
        }
        Matrix2x2 k4 = StateDir(temp, pairs);

        for (size_t i = 0; i < Xi.size(); ++i) {
            for (size_t j = 0; j < Xi[i].size(); ++j) {
                Xi[i][j] += (k1[i][j] + 2 * k2[i][j] + 2 * k3[i][j] + k4[i][j]) * (dt / 6);
            }
        }
        return Xi;
    }

    void Add_On_Click(vector<vector<double>>& Xi)
    {
        int N = Xi.size();
        // Change high and low bound to change range for possible masses for spawnes objects
        int low_bound = -50, high_bound = 50;
        int range = (high_bound - low_bound) + 1;

        Xi.push_back({ (double)GetMouseX(), (double)GetMouseY(), low_bound + double(range * rand() /
            (RAND_MAX + 1.0)), low_bound + double(range * rand() /
            (RAND_MAX + 1.0)), low_bound + double(range * rand() / (RAND_MAX + 1.0)),  low_bound + double(range * rand() /
            (RAND_MAX + 1.0)) });
        masses.push_back(rand() % 100);
        radii.push_back(rand() % 15);
        colors.push_back({ AllColors[rand() % 21] });
        pairs = combinations(Xi.size());
        trails.resize(Xi.size());
    }

    void Draw_Trails()
    {
        int N = Xi.size();
        vector<Color> trail_color(getN());
        for (int i = 0; i < getN(); ++i)
        {
            trail_color[i] = colors[i];
        }

        trails.resize(N);

        const float dt = 0.1f;
        rk4(Xi, dt, pairs);

        int maxTrailLength = 15;

        for (int i = 0; i < Xi.size(); ++i)
        {
            trails[i].push_back({ (float)Xi[i][0], (float)Xi[i][1] ,(float)Xi[i][2] });
            if (trails[i].size() > maxTrailLength)
                trails[i].erase(trails[i].begin());

            for (int j = 0; j < trails[i].size(); ++j)
            {
                if (masses[i] > 198900)
                    continue;
                float t = (float)j / trails[i].size();
                float radius = (1.0f + 3.0f / (1.0f - t));

                Color c = trail_color[i];
                c.a = (unsigned char)(255 * (1.0f - t));

                if (TwoD) {
                    DrawCircleV({ trails[i][j].x, trails[i][j].y }, radius, c);
                }
                else {
                    DrawSphereEx(trails[i][j], radius, 5, 10, c);
                }
            }
        }
    }

    void draw2d()
    {
        int N = Xi.size();

        if (TwoD) {
            for (auto& body : Xi) {
                body[2] = 0.0;
                body[5] = 0.0;
            }
        }

        Draw_Trails();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Add_On_Click(Xi);
        }

        for (int i = 0; i < Xi.size(); ++i)
        {
            DrawCircle(Xi[i][0], Xi[i][1], radii[i], colors[i]);
        }
    }

    void draw3d()
    {
        int N = Xi.size();
        float dt = 0.1;


        Draw_Trails();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Add_On_Click(Xi);
        }

        for (int i = 0; i < Xi.size(); ++i)
        {
            Vector3 pos_i = { (float)Xi[i][0], (float)Xi[i][1], (float)Xi[i][2] };
            DrawSphere(pos_i, radii[i], colors[i]);
        }
    }

private:
    vector<vector<double>> Xi;
    vector<double> masses;
    vector<int> radii;
    vector<Color> colors;
    vector<vector<Vector3>> trails;
    double G;
    std::vector<std::pair<int, int>> pairs;
};

int main()
{
    bool isTwoDMode = false; 

    NbodySimulation rng_sys
    (
        {
             { 0, 0, 0, 0, 0, 0 }, // Sun
             { 31.95, 0, 0, 0, 95.8, 0 }, // Mercury
             { 54.1, 0, 0, 0, 70, 0 }, // Venus
             { 74.3, 0, 0, 0, 59.6, 0 }, // Earth
             { 113.95, 0, 0, 0, 48.2, 0 }, // Mars
             { 389.25, 0, 0, 0, 26.2, 0 }, // Jupiter
             { 716.5, 0, 0, 0, 19.4, 0 }, // Saturn
             { 1436, 0, 0, 0, 13.6, 0 }, // Uranus
             { 2247.5, 0, 0, 0, 10.8, 0 }  // Neptune

        },
        //Masses
        { 1989000, 0.330, 4.87, 5.97, 0.642, 1900, 568, 86.8, 102 },
        //Radii
        { 50, 5, 5, 5, 5, 20, 12, 10, 10 },
        //Colors scheme
        {
            {YELLOW}, {DARKGRAY}, {BEIGE}, {BLUE}, {RED},
            {ORANGE}, {GOLD}, {SKYBLUE}, {DARKBLUE}
        },
        0.1f
    );

    const int ScreenWidth = 1920;
    const int ScreenHight = 1080;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(ScreenWidth, ScreenHight, "Universe");

    Camera3D camera3D = { 0 };
    camera3D.position = { 1000.0f, 1000.0f, 1000.0f };
    camera3D.target = { 0.0, 0.0, 0.0 };
    camera3D.up = { 0.0f, 1.0f, 0.0f };
    camera3D.fovy = 90.0f;
    camera3D.projection = CAMERA_PERSPECTIVE;


    Camera2D camera2D = { 0 };

    camera2D.target = { 0.0f, 0.0f };

    camera2D.offset = { ScreenWidth / 2.0f, ScreenHight / 2.0f };
    camera2D.rotation = 0.0f;
    camera2D.zoom = 1.0f;

    SetTargetFPS(120);

    DisableCursor();

    float radius = 2000.0f;

    while (!WindowShouldClose())
    {

        float wheelMove = GetMouseWheelMove();

        if (IsKeyPressed(KEY_SPACE))
        {
            isTwoDMode = !isTwoDMode;
            rng_sys.TwoD = isTwoDMode; 
        }

        if (isTwoDMode)
        {

            camera2D.zoom += wheelMove * 0.1f;


            if (camera2D.zoom < 0.1f) camera2D.zoom = 0.1f;
            if (camera2D.zoom > 5.0f) camera2D.zoom = 5.0f;
        }
        else
        {

            static float yaw = 0.0f, pitch = 0.0f;
            Vector2 d = GetMouseDelta();
            yaw += d.x * 0.005f;
            pitch = Clamp(pitch + d.y * 0.005f, -PI / 2 + 0.1f, PI / 2 - 0.1f);

            camera3D.position.x = radius * cosf(pitch) * sinf(yaw);
            camera3D.position.y = radius * sinf(pitch);
            camera3D.position.z = radius * cosf(pitch) * cosf(yaw);
            camera3D.target = { 0.0, 0.0, 0.0 };

            float zoomSpeed = 50.0f;
            radius -= wheelMove * zoomSpeed;
            radius = Clamp(radius, 100.0f, 5000.0f);

            UpdateCamera(&camera3D, CAMERA_CUSTOM);
        }

        BeginDrawing();

        ClearBackground(BLACK);

        if (isTwoDMode)
        {
            BeginMode2D(camera2D);
            rng_sys.draw2d();
            EndMode2D();
        }
        else
        {
            BeginMode3D(camera3D);
            rng_sys.draw3d();
            EndMode3D();
        }

        DrawFPS(10, 10);

        // Optional: Draw instructions
        if (isTwoDMode) DrawText("Mode: 2D", 10, 40, 20, WHITE);
        else DrawText("Mode: 3D", 10, 40, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
