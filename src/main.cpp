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
	Vector2 force;

	// Constructor with float-based Vector2
	Balls(Vector2 pos = { 0.0f, 0.0f },
		Vector2 vel = { 0.0f, 0.0f },
		Vector2 acc = { 0.0f, 0.0f },
		Vector2 force = { 0.0f, 0.0f },
		float m = 1)
		: position(pos), velocity(vel), acceleration(acc), mass(m) {
	}
};

int main()
{
	const int ScreenWidth = 1280;
	const int ScreenHight = 720;

	const float GravitationalConstant = 6.674 * pow(10, 6);

	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(ScreenWidth, ScreenHight, "Universe");

	Balls Oneball;
	Oneball.position = { 740.0f, 690.0f };
	Oneball.velocity = { 0.0f, 0.0f };
	Oneball.acceleration = { 0.0f, 0.0f };
	Oneball.mass = 500000000.0f;
	Oneball.force = { 0.0f, 0.0f };

	Balls Twoball;
	Twoball.position = { 540.0f, 320.0f };
	Twoball.velocity = { 0.0f, 0.0f };
	Twoball.acceleration = { 0.0f, 0.0f };
	Twoball.mass = 900000000.0f;
	Twoball.force = { 0.0f, 0.0f };


	const int radius = 24;

	//Update frequency??
	float rotation = 0.0f;

	float distance = 0.0f;

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		float deltaTime = GetFrameTime();
		rotation += 0.2f;

		//For pythogorous 
		Vector2 WDistance = { (Oneball.position.x - Twoball.position.x), (Oneball.position.y - Twoball.position.y) };
		//Distance calculated by pathagorous
		float distance = sqrtf((WDistance.x * WDistance.x) + (WDistance.y * WDistance.y));
		//Vector distance between two masses
		Vector2 VectorDis = { (Twoball.position.x - Oneball.position.x),(Twoball.position.y - Oneball.position.y) };
		//It is something cool
		Vector2 UnitVectOne = { (Oneball.position.x / distance), (Oneball.position.y / distance) };

		//ForceX calc
		Oneball.force.x = -fabs(((GravitationalConstant * ((Oneball.mass + Twoball.mass) / (distance * distance))) * UnitVectOne.x));
		//ForceY calc
		Oneball.force.y = -fabs(((GravitationalConstant * ((Oneball.mass + Twoball.mass) / (distance * distance))) * UnitVectOne.y));
		//Ugh Done!
		//Now state that forces are equal(almost)
		Twoball.force.x = fabs(Oneball.force.x);
		Twoball.force.y = fabs(Oneball.force.y);

		
		//Now accel
		Oneball.acceleration.x = (Oneball.force.x / Oneball.mass) * deltaTime;
		Oneball.acceleration.y = (Oneball.force.y / Oneball.mass) * deltaTime;
		Twoball.acceleration.x = (Twoball.force.x / Twoball.mass) * deltaTime;
		Twoball.acceleration.y = (Twoball.force.y / Twoball.mass) * deltaTime;


		
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
		//-----------------------------------------
		//Collision calculations
		//-----------------------------------------
		if ((Oneball.position.x >= (GetScreenWidth() - radius)) || (Oneball.position.x <= radius))
		{
			//Oneball.acceleration.x *= -1.0f;
			Oneball.velocity.x *= -1.0f;
		}
		if ((Oneball.position.y >= (GetScreenHeight() - radius)) || (Oneball.position.y <= radius))
		{
			//Oneball.acceleration.y *= -1.0f;
			Oneball.velocity.y *= -1.0f;
		}
		if ((Twoball.position.x >= (GetScreenWidth() - radius)) || (Twoball.position.x <= radius))
		{
			//Twoball.acceleration.x *= -1.0f;
			Twoball.velocity.x *= -1.0f;
		}
		if ((Twoball.position.y >= (GetScreenHeight() - radius)) || (Twoball.position.y <= radius))
		{
			//Twoball.acceleration.y *= -1.0f;
			Twoball.velocity.y *= -1.0f;
		}
	
		BeginDrawing();
			ClearBackground(BLACK);
			
			DrawCircleGradient(Oneball.position.x, Oneball.position.y, radius, RED, BLUE);
			DrawCircleGradient(Twoball.position.x, Twoball.position.y, radius, BLUE, RED);
			DrawText(
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
			);
		EndDrawing();
	}
	CloseWindow();

	return 0;
}

