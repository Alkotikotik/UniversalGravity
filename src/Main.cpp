#include <iostream>
#include <cmath>
#include "raylib.h"
#include <cassert>
#include <utility>
#include <vector>
#include <string>
#include <array>
using namespace std;

//for matrices
using Matrix2x2 = std::vector<std::vector<double>>;


// The whole system class
class NbodySimulation {
public:
    //Constructor to set start masses
    NbodySimulation(const vector<vector<double>>& Xi,
        const vector<double>& masses,
        const vector<int>& radii,
        const vector<array<string, 2>>& colors,
        const double G = 6.674e-11)
        : Xi(Xi), masses(masses), G(G) {
        assert(Xi.size() == masses.size() && masses.size() == radii.size() && radii.size() == colors.size());  // Validate input
    }

    //Useful variables
    const int Nnumber = Xi.size();
    int totalCombs = Nnumber - 1;
    const int dt = GetFrameTime();

    //Calculating pairs
    vector<pair<int, int>> combinations(int Nnumber, int totalCombs)
    {
        vector<pair<int, int>> pairs;
        if (totalCombs > Nnumber || totalCombs <= 0) return pairs;
        vector<int> indices(totalCombs);
        for (int i = 0; i < totalCombs; ++i) indices[i] = i;

        while (true) {
            pairs.emplace_back(indices);  // Save current combination
            int i = totalCombs - 1;
            while (i >= 0 && indices[i] == i + Nnumber - totalCombs) i--;
            if (i < 0) break;
            indices[i]++;
            for (int j = i + 1; j < totalCombs; ++j) {
                indices[j] = indices[j - 1] + 1;
            }
        }
        return pairs;
    }

    //Getting state derivative function 
    Matrix2x2 StateDir(const vector<vector<double>>& Xi, vector<std::pair<int, int>>& pairs)
    {
        const int D = 2; //D = dimensions
        const int Nnumber = Xi.size(); //Number of objects
        Matrix2x2 Xdot;
        Matrix2x2 V; // Velocity submatrix
        Matrix2x2 R; //Position submatrix

        // Extract positions (R = X[:, :D])
        R.resize(Nnumber, std::vector<double>(D));
        for (int i = 0; i < Nnumber; ++i) {
            for (int j = 0; j < D; ++j) {
                R[i][j] = Xi[i][j];
            }
        }

        // Extract velocities (V = X[:, D:])
        V.resize(Nnumber, std::vector<double>(D));
        for (int i = 0; i < Nnumber; ++i) {
            for (int j = 0; j < D; ++j) {
                V[i][j] = Xi[i][D + j];
            }
        }

        // Initialize Xdot (np.zeros_like(X))
        Xdot = Matrix2x2(Nnumber, std::vector<double>(2 * D, 0.0));

        // Fill velocity part (Xdot[:, :D] = V)
        for (int i = 0; i < Nnumber; ++i) {
            for (int j = 0; j < D; ++j) {
                Xdot[i][j] = V[i][j]; // Copy velocities to position derivatives
            }
        }


        for (const auto& [body_i, body_j] : pairs)
        {
            Vector2 ipos = { Xi[0][body_i], Xi[1][body_i] };
            Vector2 jpos = { Xi[0][body_j], Xi[1][body_j] };

            Vector2 r_vec = { ipos.x - jpos.x, ipos.y - jpos.y };

            double r = sqrtf((r_vec.x * r_vec.x) + (r_vec.y * r_vec.y));

            Vector2 Force = { G * ((masses[body_i] * masses[body_j] * r_vec.x) / (r * r * r)),
                G * ((masses[body_i] * masses[body_j] * r_vec.y) / (r * r * r)) };

            vector<double> iAccel = { (Force.x / masses[body_i]), (Force.y / masses[body_i]) };
            vector<double> jAccel = { (Force.x / masses[body_j]), (Force.y / masses[body_j]) };

            for (int k = 0; k < 2; ++k) {
                Xdot[body_i][D + k] += jAccel[k];  // Manually iterate over columns D onward
            }
            for (int k = 0; k < 2; ++k) {
                Xdot[body_j][D + k] += jAccel[k];  // Manually iterate over columns D onward
            }

            return Xdot;
        }
    }


    //rk itegral, basically the heart of calculations
    Matrix2x2 rk4(vector<vector<double>>& Xi, const int dt, vector<pair<int, int>> pairs)
    {

        //rk4 formula: (1/6.)*(k1 + 2*k2 + 2*k3 + k4) 

        int D = 2;
        Matrix2x2 k1 = StateDir(Xi, pairs);
        Matrix2x2 k2;
        Matrix2x2 k3;
        Matrix2x2 k4;


        //k2 calcs based on k2 = Xi + 0.5*k1*dt formula
        for (int i = 0; i < k1.size(); ++i) 
        {
            for (int j = 0; j < k1.size(); ++j) 
            {
                k2[i][j] = (Xi[i][j]+(k1[i][j] * 0.5 * dt));
                k2 = StateDir(k2, pairs);
            }
        }


        //k3 calcs based on k3 = Xi + 0.5*k2*dt formula
        for (int i = 0; i < k2.size(); ++i) 
        {
            for (int j = 0; j < k2.size(); ++j) 
            {
                k3[i][j] = (Xi[i][j] + (k2[i][j] * 0.5 * dt));
                k3 = StateDir(k3, pairs);
            }
        }
        //k4 calcs based on k4 = Xi + k3*dt formula
        for (int i = 0; i < k3.size(); ++i) 
        {
            for (int j = 0; j < k3.size(); ++j) 
            {
                k4[i][j] = (Xi[i][j] + (k3[i][j] * dt));
                k4 = StateDir(k4, pairs);
            }
        }


        //rk4 result calculatons based on X_prime = (1/6.)*(k1 + 2*k2 + 2*k3 + k4) formula

        Matrix2x2 XPrime;

        for (int i = 0; i < k3.size(); ++i)
        {
            for (int j = 0; j < k3.size(); ++j)
            {
                XPrime[i][j] = (1 / 6) * (k1[i][j] + 2 * k2[i][j] + 2 * k3[i][j] + k4[i][j]);

                Xi[i][j] += XPrime[i][j];
            }
        }
        //The slowest thing ever, this is bullshit speed


        //Return new updated Xi(current state)
        return Xi;
    }

    void draw_sys(vector<vector<double>>& Xi, const vector<int> &radii, const vector<array<string, 2>>& colors)
    {
        const int ScreenWidth = 1920;
        const int ScreenHight = 1080;

        SetConfigFlags(FLAG_MSAA_4X_HINT);
        InitWindow(ScreenWidth, ScreenHight, "Universe");

        SetTargetFPS(60);

        float rotation = 0.0f;

        int D = 2;
        while (!WindowShouldClose())
        {
            BeginDrawing();

            for(int i = 0; i<Nnumber; ++i)
            {
                for (int j = 0; j < D; ++j)
                {
                    DrawCircleGradient(Xi[i][j], Xi[i][j+1], radii[i], /*colors[i][j]*/ RED, /*colors[i][j]*/ BLUE);
                }
            }

            /*
            * Just testing for loop, might be useful or otherwise
            DrawCircleGradient(Xi[0][1], Xi[0][2], radii[0], RED, ORANGE);
            DrawCircleGradient(Xi[1][1], Xi[1][2], radii[1], RED, ORANGE);
            DrawCircleGradient(Xi[2][1], Xi[2][2], radii[2], RED, ORANGE);
            DrawCircleGradient(Xi[3][1], Xi[3][2], radii[3], RED, ORANGE);
            DrawCircleGradient(Xi[4][1], Xi[4][2], radii[4], RED, ORANGE);
            */


            EndDrawing();
        }
           
    }




private:
    vector<vector<double>> Xi;
    vector<double> masses;
    double G;
    std::vector<std::pair<int, int>> pairs;
};

int main()
{
  
      NbodySimulation rng_sys(
          {
              //Xi or current state
              {960.0, 540.0, 0.0, 0.0},
              {80.0, 60.0, 0.0, 0.0},
              {960.0, 360.0, 0.0, 0.0},
              {480.0, 540.0, 0.0, 0.0},
              {360.0, 120.0, 0.0, 0.0}

          },
          //Masses
          { 1000.0, 100.0, 50.0, 25.0, 12.5 },
          //Radii
          { 30, 15, 12, 10, 5, 3 },
          //Colors scheme for CircleGradient
          {
              {RED, ORANGE},
              {BLUE, DARKBLUE},
              {BROWN, DARKBROWN},
              {GREEN, DARKGREEN},
              {VIOLET, DARKPURPLE}
          
          },
          //G can be change to control the gravity effect
          100

      );

}
