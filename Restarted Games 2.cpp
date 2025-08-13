// Restarted Games 2.cpp: A program using the TL-Engine

#include "TL-Engine11.h" // TL-Engine11 include file and namespace
using namespace tle;

struct vector2D
{
	float x;
	float z;
};

vector2D scalar(float s, vector2D v)
{
	return { s * v.x, s * v.z };
}

vector2D addThree(vector2D v1, vector2D v2, vector2D v3)
{
	return { v1.x + v2.x + v3.x, v1.z + v2.z + v3.z };
}

bool Sphere2Box(IModel* model1, IModel* model2, float mRadius, float cWidth, float cDepth);
bool sphere2sphere(IModel* model1, IModel* model2, float radius1, float radius2);

int main()
{
	// Create a 3D engine (using TL11 engine here) and open a window for it
	TLEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine11\\Media" );

	/**** Set up your scene here ****/
	ICamera* myCamera = myEngine->CreateCamera(kManual, 0.0f, 15.0f, -40.0f);
	myCamera->RotateLocalX(10.0f);

	IMesh* skyBoxMesh = myEngine->LoadMesh("Skybox 07.x");
	IModel* skyBox = skyBoxMesh->CreateModel(0.0f, -960.0f, 0.0f);

	IMesh* groundMesh = myEngine->LoadMesh("ground.x");
	IModel* ground = groundMesh->CreateModel(0.0f, 0.0f, 0.0f);

	IMesh* carMesh = myEngine->LoadMesh("race2.x");
	IModel* car = carMesh->CreateModel(0.0f, 0.0f, -20.0f);

	IFont* myFont = myEngine->LoadFont("Comic Sans MS", 36);

	//--------------CheckPoints Position & Array--------------------
	IMesh* checkPointMesh = myEngine->LoadMesh("Checkpoint.x"); //load in the mesh
	const int checkPointArraySize = 5; //set the size of checkpoints
	IModel* checkPoint[checkPointArraySize]; //put the checkpoints into an array

	for (int i = 0; i < checkPointArraySize; i++)
		checkPoint[i] = checkPointMesh->CreateModel(); //create the models that are in the array

	checkPoint[0]->SetPosition(0.0f, 0.0f, 0.0f); //set each position 
	checkPoint[1]->SetPosition(10.0f, 0.0f, 120.0f);
	checkPoint[1]->RotateY(90.0f);
	checkPoint[2]->SetPosition(25.0f, 0.0f, 56.0f);
	checkPoint[3]->SetPosition(25.0f, 0.0f, -20.0f);
	checkPoint[4]->SetPosition(-25.0f, 0.0f, 20.0f);
	//--------------------------------------------------------------

	//--------------CheckPoints Position & Array--------------------
	IMesh* tankMesh = myEngine->LoadMesh("TankSmall1.x");
	const int tankArraySize = 4;
	IModel* tank[tankArraySize];
	float tankScale = 0.7f;

	for (int i = 0; i < tankArraySize; i++)
	{
		tank[i] = tankMesh->CreateModel();
		tank[i]->Scale(tankScale); // make them smaller
	}

	tank[0]->SetPosition(-5.0f, 0.0f, 20.0f);
	tank[1]->SetPosition(-2.0f, 0.0f, 120.0f);
	tank[2]->SetPosition(35.0f, 2.0f, 65.0f);
	tank[2]->RotateLocalX(90.0f);
	tank[3]->SetPosition(18.0f, 0.0f, 0.0f);
	//--------------------------------------------------------------

	//-------------------Isle Position & Array----------------------
	IMesh* straightIsleMesh = myEngine->LoadMesh("IsleStraight.x");
	const int straightIsleArraySize = 6;
	IModel* straightIsle[straightIsleArraySize];

	for (int i = 0; i < straightIsleArraySize; i++)
		straightIsle[i] = straightIsleMesh->CreateModel();

	straightIsle[0]->SetPosition(-10.0f, 0.0f, 40.0f);
	straightIsle[1]->SetPosition(10.0f, 0.0f, 40.0f);
	straightIsle[2]->SetPosition(-10.0f, 0.0f, 56.0f);
	straightIsle[3]->SetPosition(10.0f, 0.0f, 56.0f);
	straightIsle[4]->SetPosition(-10.0f, 0.0f, 72.0f);
	straightIsle[5]->SetPosition(10.0f, 0.0f, 72.0f);
	//--------------------------------------------------------------

	//-------------------Walls Position & Array---------------------
	IMesh* wallMesh = myEngine->LoadMesh("Wall.x");
	const int wallArraySize = 4;
	IModel* walls[wallArraySize];

	for (int i = 0; i < wallArraySize; i++)
		walls[i] = wallMesh->CreateModel();

	walls[0]->SetPosition(-10.0f, 0.0f, 48.0f);
	walls[1]->SetPosition(10.0f, 0.0f, 48.0f);
	walls[2]->SetPosition(-10.0f, 0.0f, 64.0f);
	walls[3]->SetPosition(10.0f, 0.0f, 64.0f);
	//--------------------------------------------------------------

	//-------------------Dummy Position and Array-------------------
	IMesh* dummyMesh = myEngine->LoadMesh("Dummy.x"); 
	IModel* checkpointLegLeft[checkPointArraySize];
	IModel* checkpointLegRight[checkPointArraySize];

	const float legOffset = 10.0f;
	const float legRotation = 90.0f;
	bool checkRotated[checkPointArraySize] = { false, true, false };

	for (int i = 0; i < checkPointArraySize; i++)
	{
		if (!checkRotated[i]) //if its not the checkpoint we are rotating then do whats in the statement
		{
			checkpointLegLeft[i] = dummyMesh->CreateModel(checkPoint[i]->GetX() - legOffset, 0.0f, checkPoint[i]->GetZ());
			checkpointLegRight[i] = dummyMesh->CreateModel(checkPoint[i]->GetX() + legOffset, 0.0f, checkPoint[i]->GetZ());
		}
		else //just for the checkpoint that is rotated
		{
			checkpointLegLeft[i] = dummyMesh->CreateModel(checkPoint[i]->GetX(), 0.0f, checkPoint[i]->GetZ() - legOffset);
			checkpointLegRight[i] = dummyMesh->CreateModel(checkPoint[i]->GetX(), 0.0f, checkPoint[i]->GetZ() + legOffset);
		}
	}
	//--------------------------------------------------------------

	//--------------------------------------------------------------
	IMesh* crossMesh = myEngine->LoadMesh("Cross.x");
	const int crossArraySize = 5;
	IModel* cross[crossArraySize];
	float crossY = -100.0f;

	for (int i = 0; i < crossArraySize; i++)
	{
		cross[i] = crossMesh->CreateModel();
		cross[i]->SetY(crossY);
	}

	/*cross[0]->SetPosition(0.0f, 0.0f, 0.0f);
	cross[1]->SetPosition(10.0f, 0.0f, 120.0f);
	cross[1]->RotateY(90.0f);
	cross[2]->SetPosition(25.0f, 0.0f, 56.0f);
	cross[3]->SetPosition(25.0f, 0.0f, -20.0f);
	cross[4]->SetPosition(-25.0f, 0.0f, 20.0f);*/
	//--------------------------------------------------------------

	myCamera->AttachToParent(car);

	vector2D momentum = { 0.0f, 0.0f };
	vector2D thrust = { 0.0f, 0.0f };
	vector2D drag = { 0.0f, 0.0f };
	vector2D facingVector = { 0.0f, 1.0f };

	float matrix[4][4]; // place to store the model matrix

	const float thrustFactor = 50.0f;
	const float dragFactor = -0.8f;
	const float fontX = 660.0f;
	const float fontY = 830.0f;
	const float backDropX = 435.0f;
	const float backDropY = 825.0f;
	const float carRadius = 3.0f;
	const float checkPointWidth = 2.0f;
	const float checkpointDepth = 0.5f;
	const float wallWidth = 4.0f;
	const float wallDepth = 2.0f;
	const float turnSpeed = 150.0f;
	const float checkPointLegRadius1 = 0.1f;
	const float tankWidth = 2.0f;
	const float tankDepth = 2.0f;
	const float resetCountDownTimer = 1.0f;
	const float healthDisplayOffset = 200.0f;
	const float cameraResetPosX = 0.0f;
	const float cameraResetPosY = 15.0f;
	const float cameraResetPosZ = -40.0f;
	const float cameraResetRotationX = 10.0f;
	const float checkpointDisplayOffsetX = 120.0f;
	const float raceFinishedTextOffsetX = 120.0f;
	const float boostMultiplier = 2.0f;
	const float maxBoostTime = 3.0f;
	const float overheatMultiplier = 2.0f;
	const float maxCooldownTime = 5.0f;
	const float dragFactorReduction = 2.0f;
	const float halfThrustFactor = 2.0f;
	const float speedReadoutOffsetX = 600.0f;
	const float gameOverReadoutOffsetX = 200.0f;
	const float timeDecrement = 1.0f;

	const int numberOfCheckpoints = 5;

	const EKeyCode fwdThrustKey = Key_W;
	const EKeyCode revThrustKey = Key_S;
	const EKeyCode leftTurnKey = Key_A;
	const EKeyCode rightTurnKey = Key_D;
	const EKeyCode spaceBoost = Key_Space;
	const EKeyCode chaseCamera = Key_2;
	const EKeyCode firstPerson = Key_1;
	const EKeyCode camUp = Key_Up;
	const EKeyCode camDown = Key_Down;
	const EKeyCode camLeft = Key_Left;
	const EKeyCode camRight = Key_Right;

	int health = 100;
	int countDown = 3;
	float countDownTimer = 1.0f;
	int speed = 0;

	float carX = 0.0f;
	float carY = 0.0f;
	float carZ = 0.0f;
	float frameTime = 50.0f;
	const float kGameSpeed = 10.0f;
	float carSpeed = frameTime * kGameSpeed;
	const float kCameraMove = kGameSpeed * 0.01f;
	const float kMouseMove = kGameSpeed * 0.1;
	const float kMouseRotation = kGameSpeed * 0.003f;

	bool mouseCaptureActive = true;
	const EKeyCode toggleMouseCapture = Key_Tab;
	myEngine->StartMouseCapture();

	bool chaseCamActive = false;
	int mouseMoveX = 0;
	int mouseMoveY = 0;

	enum GameState { Ready, Countdown, Racing, Finished };
	GameState gameState = Ready;

	bool checkpointContact[numberOfCheckpoints];
	int currentCheckpoint = 0;
	bool finished = false;
	bool carCollision = false;
	bool checkpointCrossed = false;
	bool boostActive = false;
	bool boostOverheat = false;

	float boostDuration = 0.0f;
	float overheatCooldown = 0.0f;

	for (int i = 0; i < numberOfCheckpoints; i++)
	{
		checkpointContact[i] = false; //automatically  sets collision with checkpoints as false
	}

	ISprite* backdrop = myEngine->CreateSprite("ui_backdrop.jpg", backDropX, backDropY);

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();
		frameTime = myEngine->FrameTime();

		/**** Update your scene each frame here ****/
		if (gameState == Ready)
		{
			myFont->Draw("Press space to start!", fontX, fontY);

			if (myEngine->KeyHit(Key_Space))
			{
				gameState = Countdown;
			}
		}

		if (gameState == Countdown)
		{
			countDownTimer -= frameTime; 

			//if timer is lessthan or equalto then countdown is set to countdown - timedecrement (1.0f)
			if (countDownTimer <= 0.0f)
			{
				countDown = countDown - timeDecrement;
				countDownTimer = resetCountDownTimer; // reset timer
			}

			//if countdown reaches 0.0f then enter racing state
			if (countDown <= 0.0f)
			{
				gameState = Racing;
			}

			//displays what number in countdown its on
			if (countDown == 3)
				myFont->Draw("3", fontX, fontY);
			else if (countDown == 2)
				myFont->Draw("2", fontX, fontY);
			else if (countDown == 1)
				myFont->Draw("1", fontX, fontY);
		}

		if (gameState == Racing)
		{
			//lets the program know so when it gets to 'go!'
			//it knows to then let the player move.
			myFont->Draw("Go!", fontX, fontY);
			myFont->Draw("Health: " + std::to_string(health), fontX - healthDisplayOffset, fontY);//---------------------------------

			const float carPovY = 5.5f;
			//chase camera to first person
			if (myEngine->KeyHit(firstPerson)) myCamera->SetLocalPosition(0.0f, carPovY, 0.0f);

			//change camera to chase camera
			if (myEngine->KeyHit(chaseCamera))
			{
				myCamera->SetLocalPosition(cameraResetPosX, cameraResetPosY, cameraResetPosZ);//----------------------
				myCamera->ResetOrientation();
				myCamera->RotateLocalX(cameraResetRotationX);
			}

			//move the camera up,down,left or right
			if (myEngine->KeyHeld(camUp)) myCamera->MoveLocalZ(kCameraMove);
			if (myEngine->KeyHeld(camDown)) myCamera->MoveLocalZ(-kCameraMove);
			if (myEngine->KeyHeld(camLeft)) myCamera->MoveLocalX(-kCameraMove);
			if (myEngine->KeyHeld(camRight)) myCamera->MoveLocalX(kCameraMove);

			carCollision = false;

			//if the game hasnt finished and ive gone through the first checkpoint 
			//then start outputting what checkpoints im on.
			if (currentCheckpoint > 0 && !finished)
			{
				myFont->Draw("Current Checkpoint: " + std::to_string(currentCheckpoint), fontX + checkpointDisplayOffsetX, fontY);

			}
			//if ive finished the race, output finished!
			else if (finished)
			{
				myFont->Draw("Finished!", fontX + raceFinishedTextOffsetX, fontY);
			}
			
			if (myEngine->KeyHeld(leftTurnKey)) car->RotateY(-turnSpeed * frameTime);
			if (myEngine->KeyHeld(rightTurnKey)) car->RotateY(turnSpeed * frameTime);

			// get facing vector (from model matrix)
			car->GetMatrix(&matrix[0][0]); // get the model matrix

			facingVector = { matrix[2][0], matrix[2][2] }; // local z axis's x and z components

			// calculate thrust (from facing vector AND keyboard input)
			if (myEngine->KeyHeld(fwdThrustKey))
			{
				thrust = scalar(thrustFactor * frameTime, facingVector);

				//if boost pressed and its not overheated then activate boost
				if (myEngine->KeyHeld(spaceBoost) && !boostOverheat)
				{
					boostActive = true;
					boostDuration += frameTime;

					//increase speed by 2X if space bar is pressed and hasnt overheated
					thrust = scalar(thrustFactor * boostMultiplier * frameTime, facingVector);
				}
				else
				{
					boostActive = false;
				}

				//if user holds boost for 3 second or more, it overheats and adds drag to slow it down
				if (boostDuration > maxBoostTime)
				{
					boostOverheat = true;
					boostDuration = 0.0f;
					dragFactor * overheatMultiplier;
				}

				//if its overheated then reduce drag to all the vehicle to go faster
				if (boostOverheat)
				{
					overheatCooldown += frameTime;

					if (overheatCooldown > maxCooldownTime)
					{
						boostOverheat = false;
						overheatCooldown = 0.0f;
						dragFactor / dragFactorReduction;
					}
				}
			}

			//if reverse, go half the speed as forward thrust
			else if (myEngine->KeyHeld(revThrustKey))
			{
				thrust = scalar(-thrustFactor / halfThrustFactor * frameTime, facingVector);
			}

			else
			{
				thrust = { 0.0f, 0.0f };
			}

			// calculate drag (from old momentum)
			drag = scalar(dragFactor * frameTime, momentum);

			// calculate new momentum (from old momentum and drag and thrust)
			momentum = addThree(momentum, drag, thrust);

			// move car 
			car->Move(momentum.x * frameTime, 0.0f, momentum.z * frameTime);

			int speed = sqrt((momentum.x * momentum.x) + (momentum.z * momentum.z)); // multiply by scale const and make it 1.
			myFont->Draw("Speed: " + std::to_string(speed) + "kph", fontX + speedReadoutOffsetX, fontY, kLightGrey); //62.5kph is max

			//contatc on checkpoints
			for (int i = 0; i < checkPointArraySize; i++)
			{
				if (Sphere2Box(car, checkPoint[i], carRadius, checkPointWidth, checkpointDepth))
				{
					//when car 'collides' with any checkpoint IN ORDER, collision is
					//set to true and the currentcheckpoint will be incremented
					if (currentCheckpoint == i)
					{
						checkpointContact[i] = true;
						currentCheckpoint++;
						
						cross[i]->SetY(0.0f);
					}

					//if the currentcheckpoint = 5 (numberofcheckpoints) then finished is true
					if (currentCheckpoint == numberOfCheckpoints) 
					{
						finished = true;
					}
				}
			}

			for (int i = 0; i < wallArraySize; i++)
			{
				if (Sphere2Box(walls[i], car, carRadius, wallWidth, wallDepth))
				{
					carCollision = true;

					//if car collides, then the momentums X and Z are reversed to 
					//make it look like it bounces off.
					momentum.x = -momentum.x;
					momentum.z = -momentum.z;
				}
			}

			for (int i = 0; i < straightIsleArraySize; i++)
			{
				if (Sphere2Box(straightIsle[i], car, carRadius, wallWidth, wallDepth))
				{
					carCollision = true;

					momentum.x = -momentum.x;
					momentum.z = -momentum.z;
				}
			}

			for (int i = 0; i < checkPointArraySize; i++)
			{
				if (sphere2sphere(car, checkpointLegLeft[i], checkPointLegRadius1, carRadius))
				{
					carCollision = true;

					momentum.x = -momentum.x;
					momentum.z = -momentum.z;
				}

				if (sphere2sphere(car, checkpointLegRight[i], checkPointLegRadius1, carRadius))
				{
					carCollision = true;

					momentum.x = -momentum.x ;
					momentum.z = -momentum.z ;
				}
			}

			for (int i = 0; i < tankArraySize; i++)
			{
				if (sphere2sphere(car, tank[i], carRadius, tankWidth))
				{
					carCollision = true;

					momentum.x = -momentum.x;
					momentum.z = -momentum.z;
				}
			}

			//if collision happens, health is decreased
			if (carCollision)
			{
				health--;

				//if health is less than or equal to 0 then games finished
				if (health <= 0)
				{
					gameState = Finished;
				}
			}
		}

		if (gameState == Finished)
		{
			myFont->Draw("Health: Game Over!", fontX - gameOverReadoutOffsetX, fontY);

			myFont->Draw("Press space to restart!", fontX, fontY);

			if (myEngine->KeyHit(Key_Space))
			{
				gameState = Ready;
			}

		}

		// Stop if the Escape key is pressed
		if (myEngine->KeyHit( Key_Escape ))
		{
			myEngine->Stop();
		}

		if (mouseCaptureActive && !chaseCamActive)
		{
			//add mouse movement to get mousemovementX&Y
			mouseMoveX += myEngine->GetMouseMovementX();
			mouseMoveY += myEngine->GetMouseMovementY();

			//resets the orientation of camera
			myCamera->ResetOrientation();
			myCamera->RotateLocalY(mouseMoveX* kMouseRotation);
			myCamera->RotateLocalX(mouseMoveY* kMouseRotation);

			float mouseMove = myEngine->GetMouseWheelMovement();
			myCamera->MoveLocalZ(mouseMove* kMouseMove);
		}

		//if tab pressed then activate mouse capture
		if (myEngine->KeyHit(toggleMouseCapture))
		{
			//allows to active mouse movement
			mouseCaptureActive = !mouseCaptureActive;
			if (mouseCaptureActive)
			{
				myEngine->StartMouseCapture();
			}
			else
			{
				myEngine->StopMouseCapture();
			}
		}
	}
	// Delete the 3D engine now we are finished with it
	myEngine->Delete();


}
// MyComment
bool Sphere2Box(IModel* model1, IModel* model2, float caRadius, float chWidth, float chDepth)
{
	float minX = model1->GetX() - (chWidth / 2) - caRadius; //subtracts half the width then subtracts the radius to find the minX
	float maxX = model1->GetX() + (chWidth / 2) + caRadius; //adds half the width then adds the radius to find the maxX etc etc 
	float minZ = model1->GetZ() - (chDepth / 2) - caRadius;
	float maxZ = model1->GetZ() + (chWidth / 2) + caRadius;

	return (model2->GetX() > minX && model2->GetX() < maxX // works out if the marble is in the cube
		&& model2->GetZ() > minZ && model2->GetZ() < maxZ);
}

bool sphere2sphere(IModel* model1, IModel* model2, float radius1, float radius2)
{
	float distX = model2->GetX() - model1->GetX(); //distance between the marbles X taken from the cubes X, **
	float distZ = model2->GetZ() - model1->GetZ(); //**
	float  dist = sqrt((distX * distX) + (distZ * distZ));

	return dist < (radius1 + radius2);
}