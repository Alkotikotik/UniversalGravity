#include <iostream>
#include <cmath>
#include "raylib.h"
using namespace std;

//Obj class
class Balls {
public:
	Vector2 position;    // float x, y
	Vector2 velocity;    // float x, y
	Vector2 acceleration; // float x, y
	float mass;            // Mass can remain int (unless you need precise fractional mass)
	int radius;
	Vector2 force;
	Vector2 momentum;

	// Constructor with float-based Vector2
	Balls(Vector2 pos = { 0.0f, 0.0f },
		Vector2 vel = { 0.0f, 0.0f },
		Vector2 acc = { 0.0f, 0.0f },
		Vector2 force = { 0.0f, 0.0f },
		Vector2 momnt = {0.0f, 0.0f},

		float m = 1)
		: position(pos), velocity(vel), acceleration(acc), mass(m) {
	}
};

int main()
{
	const int ScreenWidth = 1920;
	const int ScreenHight = 1080;

	const float GravitationalConstant = 6.674 * pow(10, 0);
	const float e = 0.3f;

	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(ScreenWidth, ScreenHight, "Universe");

	Balls Oneball;    // Central black hole
	Oneball.position = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	Oneball.velocity = { 0.0f, 0.0f };
	Oneball.mass = 10000000.0f;  // Huge mass
	Oneball.radius = 20;
	Oneball.force = { 0.0f, 0.0f };
	Oneball.acceleration = { 0.0f, 0.0f };

	Balls Twoball;    // Fast-moving comet
	Twoball.position = { GetScreenWidth() / 2.0f + 200.0f, GetScreenHeight() / 2.0f };
	Twoball.velocity = { 0.0f, 300.0f };  // High initial speed
	Twoball.mass = 10.0f;                  // Tiny mass (easily flung)
	Twoball.radius = 10;
	Twoball.force = { 0.0f, 0.0f };
	Twoball.acceleration = { 0.0f, 0.0f };

	Balls Threeball;  // Rogue planet
	Threeball.position = { GetScreenWidth() / 2.0f - 400.0f, GetScreenHeight() / 2.0f };
	Threeball.velocity = { 0.0f, -150.0f };  // Opposite direction
	Threeball.mass = 50000.0f;                // Significant mass
	Threeball.radius = 25;
	Threeball.force = { 0.0f, 0.0f };
	Threeball.acceleration = { 0.0f, 0.0f };



	//Update frequency??
	float rotation = 0.0f;

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		float deltaTime = GetFrameTime();
		rotation += 0.2f;

		// Calculate displacement vectors between balls (points FROM first ball TO second ball)
		Vector2 OneToTwo = { Twoball.position.x - Oneball.position.x,
							 Twoball.position.y - Oneball.position.y };
		Vector2 OneToThree = { Threeball.position.x - Oneball.position.x,
							   Threeball.position.y - Oneball.position.y };
		Vector2 TwoToThree = { Threeball.position.x - Twoball.position.x,
							   Threeball.position.y - Twoball.position.y };

		// Calculate distances (magnitudes) using Pythagorean theorem
		float OneTwoDistance = sqrtf(OneToTwo.x * OneToTwo.x + OneToTwo.y * OneToTwo.y);
		float ThreeOneDistance = sqrtf(OneToThree.x * OneToThree.x + OneToThree.y * OneToThree.y);
		float TwoThreeDistance = sqrtf(TwoToThree.x * TwoToThree.x + TwoToThree.y * TwoToThree.y);

		// Calculate unit vectors (direction vectors with magnitude = 1)
		Vector2 OneTwoUnitVectOne = { OneToTwo.x / OneTwoDistance,
								OneToTwo.y / OneTwoDistance };
		Vector2 ThreeOneUnitVectOne = { OneToThree.x / ThreeOneDistance,
								  OneToThree.y / ThreeOneDistance, };
		Vector2 TwoThreeUnitVectOne = { TwoToThree.x / TwoThreeDistance,
								  TwoToThree.y / TwoThreeDistance };



		// Calculate gravitational force between Oneball and Twoball
		float OneTwoForceMagnitude = GravitationalConstant * Oneball.mass * Twoball.mass
			/ (OneTwoDistance * OneTwoDistance);
		float ThreeOneForceMagnitude = GravitationalConstant * Oneball.mass * Threeball.mass
			/ (ThreeOneDistance * ThreeOneDistance);
		float TwoThreeForceMagnitude = GravitationalConstant * Threeball.mass * Twoball.mass
			/ (TwoThreeDistance * TwoThreeDistance);

		// Calculate force for Oneball components in x and y directions
		// Calculate force for Oneball (attracted to Twoball and Threeball)
		Oneball.force.x = (OneTwoForceMagnitude * OneTwoUnitVectOne.x)
			+ (ThreeOneForceMagnitude * ThreeOneUnitVectOne.x);
		Oneball.force.y = (OneTwoForceMagnitude * OneTwoUnitVectOne.y)
			+ (ThreeOneForceMagnitude * ThreeOneUnitVectOne.y);

		// Calculate force for Twoball (attracted to Oneball and Threeball)
		Twoball.force.x = (-OneTwoForceMagnitude * OneTwoUnitVectOne.x)
			+ (TwoThreeForceMagnitude * TwoThreeUnitVectOne.x);
		Twoball.force.y = (-OneTwoForceMagnitude * OneTwoUnitVectOne.y)
			+ (TwoThreeForceMagnitude * TwoThreeUnitVectOne.y);

		// Calculate force for Threeball (attracted to Oneball and Twoball)
		Threeball.force.x = (-ThreeOneForceMagnitude * ThreeOneUnitVectOne.x)
			- (TwoThreeForceMagnitude * TwoThreeUnitVectOne.x);
		Threeball.force.y = (-ThreeOneForceMagnitude * ThreeOneUnitVectOne.y)
			- (TwoThreeForceMagnitude * TwoThreeUnitVectOne.y);

		//Now accel
		Oneball.acceleration.x = (Oneball.force.x / Oneball.mass);
		Oneball.acceleration.y = (Oneball.force.y / Oneball.mass);
		Twoball.acceleration.x = (Twoball.force.x / Twoball.mass);
		Twoball.acceleration.y = (Twoball.force.y / Twoball.mass);
		Threeball.acceleration.x = (Threeball.force.x / Threeball.mass);
		Threeball.acceleration.y = (Threeball.force.y / Threeball.mass);
		
		//-----------------------------------------
		//First object calculations
		//-----------------------------------------
		/*if (Oneball.velocity.x >= 0)Oneball.acceleration.x *= -1;
		if (Oneball.velocity.y >= 0)Oneball.acceleration.y *= -1;*/
		Oneball.velocity.x += Oneball.acceleration.x * deltaTime;
		Oneball.velocity.y += Oneball.acceleration.y * deltaTime;
		Oneball.position.x += Oneball.velocity.x * deltaTime;
		Oneball.position.y += Oneball.velocity.y * deltaTime;
		//-----------------------------------------
		//Second object calculations
		//-----------------------------------------
		/*if (Twoball.velocity.x >= 0)Twoball.acceleration.x *= -1;
		if (Twoball.velocity.y >= 0)Twoball.acceleration.y *= -1*/
		Twoball.velocity.x += Twoball.acceleration.x * deltaTime;
		Twoball.velocity.y += Twoball.acceleration.y * deltaTime;
		Twoball.position.x += Twoball.velocity.x * deltaTime;
		Twoball.position.y += Twoball.velocity.y * deltaTime;

		Threeball.velocity.x += Threeball.acceleration.x * deltaTime;
		Threeball.velocity.y += Threeball.acceleration.y * deltaTime;
		Threeball.position.x += Threeball.velocity.x * deltaTime;
		Threeball.position.y += Threeball.velocity.y * deltaTime;
		//-----------------------------------------
		//Collision calculations
		//-----------------------------------------
		if ((Oneball.position.x >= (GetScreenWidth() - Oneball.radius)) || (Oneball.position.x <= Oneball.radius))
		{
			//Oneball.acceleration.x *= -1.0f;
			Oneball.velocity.x *= -0.9f;
			if (Oneball.position.x >= GetScreenWidth() - Oneball.radius)
			{
				Oneball.position.x = GetScreenWidth() - Oneball.radius;
			}
			else if (Oneball.position.x < Oneball.radius)
			{
				Oneball.position.x = Oneball.radius;
			}
		}
		if ((Oneball.position.y >= (GetScreenHeight() - Oneball.radius)) || (Oneball.position.y <= Oneball.radius))
		{
			//Oneball.acceleration.y *= -1.0f;
			Oneball.velocity.y *= -0.9f;
			if (Oneball.position.y >= GetScreenHeight() - Oneball.radius)
			{
				Oneball.position.y = GetScreenHeight() - Oneball.radius;
			}
			else if (Oneball.position.y < Oneball.radius)
			{
				Oneball.position.y = Oneball.radius;
			}
		}

		//TwoBall wall collision 
		if ((Twoball.position.x >= (GetScreenWidth() - Twoball.radius)) || (Twoball.position.x <= Twoball.radius))
		{
			//Twoball.acceleration.x *= -1.0f;
			//Slow decalaration
			Twoball.velocity.x *= -0.9f;

			//Avoid stacking in the wall
			if (Twoball.position.x >= GetScreenWidth() - Twoball.radius)
			{
				Twoball.position.x = GetScreenWidth() - Twoball.radius;
			}
			else if (Twoball.position.x < Twoball.radius)
			{
				Twoball.position.x = Twoball.radius;
			}
		}
		if ((Twoball.position.y >= (GetScreenHeight() - Twoball.radius)) ||(Twoball.position.y <= Twoball.radius))
		{
			// Reverse vertical velocity (perfect elastic collision)
			Twoball.velocity.y *= -0.9f;

			if (Twoball.position.y >= GetScreenHeight() - Twoball.radius) 
			{
				Twoball.position.y = GetScreenHeight() - Twoball.radius;
			}
			else if (Twoball.position.y <= Twoball.radius) 
			{
				Twoball.position.y = Twoball.radius;
			}
		}
		if ((Threeball.position.x >= (GetScreenWidth() - Threeball.radius)) || (Threeball.position.x <= Threeball.radius))
		{
			//Threeball.acceleration.x *= -1.0f;
			//Slow decalaration
			Threeball.velocity.x *= -0.9f;

			//Avoid stacking in the wall
			if (Threeball.position.x >= GetScreenWidth() - Threeball.radius)
			{
				Threeball.position.x = GetScreenWidth() - Threeball.radius;
			}
			else if (Threeball.position.x < Threeball.radius)
			{
				Threeball.position.x = Threeball.radius;
			}
		}
		//ThreeBall wall collision 
		if ((Threeball.position.y >= (GetScreenHeight() - Threeball.radius)) || (Threeball.position.y <= Threeball.radius))
		{
			// Reverse vertical velocity (perfect elastic collision)
			Threeball.velocity.y *= -0.9f;

			if (Threeball.position.y >= GetScreenHeight() - Threeball.radius)
			{
				Threeball.position.y = GetScreenHeight() - Threeball.radius;
			}
			else if (Threeball.position.y <= Threeball.radius)
			{
				Threeball.position.y = Threeball.radius;
			}
		}
	
	
		BeginDrawing();
			ClearBackground(BLACK);
			

			DrawCircleGradient(Oneball.position.x, Oneball.position.y, Oneball.radius, MAROON, ORANGE);
			DrawCircleGradient(Twoball.position.x, Twoball.position.y, Twoball.radius, BLUE, DARKBLUE);
			DrawCircleGradient(Threeball.position.x, Threeball.position.y, Threeball.radius, DARKGREEN, GREEN);

			/*DrawText(
				TextFormat("One Velocity X: %.2f", Oneball.velocity.x), // Formats to 2 decimal places
				40, 40, 20, RED
			);
			DrawText(
				TextFormat("One Velocity Y: %.2f", Oneball.velocity.y), // Formats to 2 decimal places
				40, 80, 20, RED
			);
			DrawText(
				TextFormat("Two Velocity X: %.2f", Twoball.velocity.x), // Formats to 2 decimal places
				360, 40, 20, RED
			);
			DrawText(
				TextFormat("Two Velocity Y: %.2f", Twoball.velocity.y), // Formats to 2 decimal places
				360, 80, 20, RED
			);*/
		EndDrawing();
	}
	CloseWindow();

	return 0;
}

