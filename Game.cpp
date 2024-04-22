// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Class with all of the functions relating to the gameplay
// Is called by main - Rebecca Morris

#include "Game.h"
#include <algorithm>
#include "Renderer.h"
#include "AudioSystem.h"
#include "PhysWorld.h"
#include "Actor.h"
#include "UIScreen.h"
#include "HUD.h"
#include "MeshComponent.h"
#include "FPSActor.h"
#include "BasicFish.h"
#include "PlaneActor.h"
#include "BobberActor.h"
#include "PauseMenu.h"
#include "MainMenu.h"
#include "InventoryMenu.h"
#include "ScreenSaysFishOn.h"
#include "ScreenSaysFishOff.h"
#include "ScreenSaysFishCaught.h"
#include "SkyBox.h"
#include "Skeleton.h"
#include "Animation.h"
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "Font.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include "Hook.h"

Game::Game()
:mRenderer(nullptr)
,mAudioSystem(nullptr)
,mPhysWorld(nullptr)
,mGameState(EGameplay)
,mUpdatingActors(false)
{
	
}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Create the renderer
	mRenderer = new Renderer(this);
	// Changed to make game full screen

	//Rebecca fullscreen = 1600, 880

	int displayIndex = 0;

	SDL_Rect window;

	//Gets the size of the user's computer screen - Rebecca Morris
	SDL_GetDisplayBounds(displayIndex, &window);

	mWindowWidth = window.w;
	mWindowHeight = window.h;



	if (!mRenderer->Initialize((float)mWindowWidth, (float)mWindowHeight))
	{
		SDL_Log("Failed to initialize renderer");
		delete mRenderer;
		mRenderer = nullptr;
		return false;
	}

	// Create the audio system
	mAudioSystem = new AudioSystem(this);
	if (!mAudioSystem->Initialize())
	{
		SDL_Log("Failed to initialize audio system");
		mAudioSystem->Shutdown();
		delete mAudioSystem;
		mAudioSystem = nullptr;
		return false;
	}

	// Create the physics world
	mPhysWorld = new PhysWorld(this);
	
	// Initialize SDL_ttf
	if (TTF_Init() != 0)
	{
		SDL_Log("Failed to initialize SDL_ttf");
		return false;
	}

	LoadData();

	isReelingIn = false;

	mTicksCount = SDL_GetTicks();

	mMusicEvent = mAudioSystem->PlayEvent("event:/Music2");

	mReeling = mAudioSystem->PlayEvent("event:/ReelingIn");
	mReeling.SetPaused(true);

	mWin = mAudioSystem->PlayEvent("event:/Win");
	mWin.SetPaused(true);

	isScreenSaysFishOnOn = false;
	isScreenSaysFishOffOn = false;
	didFishGetAway = false;
	didJustCatchFish = false;
	
	new MainMenu(this);

	return true;
}

// Function which restarts the game by unloading and reloading the assets,
// as well as clearing all arrays and reseting booleans as well as spawning the menu - Rebecca Morris
bool Game::Restart()
{

	mBasicFishes.clear();

	UnloadData();

	if (mAudioSystem)
	{
		mAudioSystem->Shutdown();
	}

	mAudioSystem = new AudioSystem(this);
	if (!mAudioSystem->Initialize())
	{
		SDL_Log("Failed to initialize audio system");
		mAudioSystem->Shutdown();
		delete mAudioSystem;
		mAudioSystem = nullptr;
		return false;
	}

	LoadData();

	isReelingIn = false;

	mTicksCount = SDL_GetTicks();

	mMusicEvent = mAudioSystem->PlayEvent("event:/Music2");

	mReeling = mAudioSystem->PlayEvent("event:/ReelingIn");
	mReeling.SetPaused(true);

	mWin = mAudioSystem->PlayEvent("event:/Win");
	mWin.SetPaused(true);

	isScreenSaysFishOnOn = false;
	isScreenSaysFishOffOn = false;
	didFishGetAway = false;
	didJustCatchFish = false;

	for (int i = 0; i < 8; i++)
	{
		mAllCaughtFish[i] = false;
	}

	new MainMenu(this);

	return true;
}

void Game::RunLoop()
{
	while (mGameState != EQuit)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::AddPlane(PlaneActor* plane)
{
	mPlanes.emplace_back(plane);
}

void Game::RemovePlane(PlaneActor* plane)
{
	auto iter = std::find(mPlanes.begin(), mPlanes.end(), plane);
	mPlanes.erase(iter);
}

//An array of booleans which are set to true when that fish has been caught - Rebecca Morris
bool Game::GetAllCaughtFish(int index)
{
	return mAllCaughtFish[index];
}

//Sets the value of mCurrentFish, which is used by some functions
// to know what fish is currently on the line - Rebecca Morris
void Game::SetCurrentFish(BasicFish* fish)
{
	mCurrentFish = fish;
}

//A function which pauses the music so it does not need to be completely restarted every time - Rebecca Morris
void Game::PauseMusic()
{
	if(!mAudioSystem->GetBusPaused("bus:/"))
		mAudioSystem->SetBusPaused("bus:/", true);
	else
		mAudioSystem->SetBusPaused("bus:/", false);
}

//A function which returns true when all of the fish in the game have been caught - Rebecca Morris
bool Game::CheckIfAllFishCaught()
{
	bool allFishCaught = true;
	for (bool fish : mAllCaughtFish)
	{
		if (fish == false)
		{
			allFishCaught = false;
		}
	}

	return allFishCaught;
}

//An unused function which would have been used to spawn in a
// new main menu from other classes than the game class - Rebecca Morris
void Game::NewMainMenu()
{
	new MainMenu(this);
}



void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				mGameState = EQuit;
				break;
			// This fires when a key's initially pressed
			case SDL_KEYDOWN:
				if (!event.key.repeat)
				{
					if (mGameState == EGameplay)
					{
						HandleKeyPress(event.key.keysym.sym);
					}
					else if (!mUIStack.empty())
					{
						mUIStack.back()->
							HandleKeyPress(event.key.keysym.sym);
					}
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (mGameState == EGameplay)
				{
					HandleKeyPress(event.button.button);
				}
				else if (!mUIStack.empty())
				{
					mUIStack.back()->
						HandleKeyPress(event.button.button);
				}
				break;
			default:
				break;
		}
	}
	
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (mGameState == EGameplay)
	{
		for (auto actor : mActors)
		{
			if (actor->GetState() == Actor::EActive)
			{
				actor->ProcessInput(state);
			}
		}
	}
	else if (!mUIStack.empty())
	{
		mUIStack.back()->ProcessInput(state);
	}
}

void Game::HandleKeyPress(int key)
{
	switch (key)
	{
	case SDLK_ESCAPE:
		// Create pause menu
		new PauseMenu(this);
		break;
	case SDLK_m:
		// Mute the background music
		if (!mMusicEvent.GetPaused())
			mMusicEvent.SetPaused(true);
		else
			mMusicEvent.SetPaused(false);
		break;
	case '-':
	{
		// Reduce master volume
		float volume = mAudioSystem->GetBusVolume("bus:/");
		volume = Math::Max(0.0f, volume - 0.1f);
		mAudioSystem->SetBusVolume("bus:/", volume);
		break;
	}
	case '=':
	{
		// Increase master volume
		float volume = mAudioSystem->GetBusVolume("bus:/");
		volume = Math::Min(1.0f, volume + 0.1f);
		mAudioSystem->SetBusVolume("bus:/", volume);
		break;
	}
	case '1':
	{
		// Load English text
		LoadText("Assets/English.gptext");
		break;
	}
	case SDL_BUTTON_LEFT:
	{
		// Fire weapon

		if (mBobbers.size() > 0) // If there is already a bobber, remove it - Rebecca Morris
		{
			for (BobberActor* bobbers : mBobbers)
			{
				bobbers->SetState(Actor::EDead);
			}
		}

		mFPSActor->Shoot();
		break;
	}
	case SDLK_SPACE:
	{
		// Below is the code that handles what happens when we actually catcht the fish.  It moves the fish in front of the player
		// and then it resets all of the variables that let the game know that a fish is on the line.  It also handles how much
		// closer the fish gets to the player every time the player hits the space bar.  This was built by both Rebecca and Adam.
		if (isReelingIn)
		{
			mReeling.SetPaused(false); // this is the audio of the reeling noise
			mReeling.Restart();

			Vector3 bobberPos = mSingleBobber->GetPosition();

			Vector3 playerPos = mFPSActor->GetPosition();

			float offsetFloat = 25.0f;

			BasicFish* hookedFish = mYellowFish; //set as default, please ignore
			
			Vector3 fishPos;
			// Find out which fish was hooked
			for (BasicFish* fish : mBasicFishes)
			{
				if (fish->GetLineStatus() && fish->GetState() == Actor::EActive)
				{
					hookedFish = fish;
				}
			}

			fishPos = hookedFish->GetPosition(); // get the fishes position

			CurrentFishType = hookedFish->GetColor(); // grab the fishes color, which lets us know what kind of fish

			SetCurrentFish(hookedFish); // set teh current hooked fish to the fish we have hooked
			mCurrentFish = hookedFish;


			mCurrentFishDistance = hookedFish->GetFishDistance();

			//Updated for removal of RedFish and Yellowfish
			//Still needs some refactoring - Rebecca 


			Vector3 bobberFacePlayer = playerPos - bobberPos; // the direction of the bobber facing the player
			bobberFacePlayer.Normalize();

			// what we are doing below is taking the fish position and bobber position, and everytime a player presses space bar to reel things in
			// we move the bobber and the fish 25 units towards the player. the code below makes sure it moves directly towards the player and doesnt pull
			// things out of the water.
			Vector3 offsetFromReel;
			offsetFromReel.x = bobberFacePlayer.x * offsetFloat;
			offsetFromReel.y = bobberFacePlayer.y * offsetFloat;
			offsetFromReel.z = bobberFacePlayer.z * offsetFloat;

			Vector3 newBobberPos;

			Vector3 fishFacePlayer = playerPos - fishPos;
			fishFacePlayer.Normalize();

			Vector3 fishOffsetFromReel;
			fishOffsetFromReel.x = fishFacePlayer.x * offsetFloat;
			fishOffsetFromReel.y = fishFacePlayer.y * offsetFloat;
			fishOffsetFromReel.z = fishFacePlayer.z * offsetFloat;


			Vector3 newFishPos; //= Vector3(playerPos.x, playerPos.y + 25.0f, playerPos.z);
			
			if (!(hookedFish->GetCatchStatus())) // If the fish is not caught, bring the fish closer
			{
				if (bobberPos.z > -110.0) // if the bobber is coming out of the water, or the fish
				{// this keeps the bobber from coming up out of the water and looking like its floatin
					newBobberPos = Vector3(bobberPos.x + offsetFromReel.x, bobberPos.y + offsetFromReel.y, bobberPos.z);
				}
				else
				{
					newBobberPos = Vector3(bobberPos.x + offsetFromReel.x, bobberPos.y + offsetFromReel.y, bobberPos.z + offsetFromReel.z);
				}
				if (fishPos.z > -170.0) // if the bobber is coming out of the water, or the fish
				{ // this keeps the fish in the water so that it doesnt break the surface when spamming the reel in button
					newFishPos = Vector3(fishPos.x + fishOffsetFromReel.x, fishPos.y + fishOffsetFromReel.y, fishPos.z);
				}
				else
				{
					newFishPos = Vector3(fishPos.x + fishOffsetFromReel.x, fishPos.y + fishOffsetFromReel.y, fishPos.z + offsetFromReel.z);
				}

			}
			hookedFish->SetPosition(newFishPos);
			//Experiment with increasing the fish distance, this definitely gives closer to desired effect - Rebecca Morris
			hookedFish->SetFishDistance(25.0);
			mSingleBobber->SetPosition(newBobberPos);
		}
		else if(didJustCatchFish)
		{
			didJustCatchFish = false;

			mReeling.SetPaused(true);

			Vector3 playerPos = mFPSActor->GetPosition();

			BasicFish* caughtFish{};

			Vector3 fishPos;

			// If statements must be used here instead of a switch statement
			// due to needing to check the catch status and the state of the fish,
			// though it could be refactored since this code is repeated - Rebecca Morris
			if (mRedFish->GetCatchStatus() && (mRedFish->GetState() == Actor::EActive))
			{
				fishPos = mRedFish->GetPosition();
				caughtFish = mRedFish;

				mCaughtFishType = caughtFish->GetColor(); //This must be inside the if statement to work - Rebecca Morris

				mAllCaughtFish[0] = true;
			}
			if (mYellowFish->GetCatchStatus() && (mYellowFish->GetState() == Actor::EActive))
			{
				fishPos = mYellowFish->GetPosition();
				caughtFish = mYellowFish;

				mCaughtFishType = caughtFish->GetColor();

				mAllCaughtFish[1] = true;
			}
			if (mOrangeFish->GetCatchStatus() && (mOrangeFish->GetState() == Actor::EActive))
			{
				fishPos = mOrangeFish->GetPosition();
				caughtFish = mOrangeFish;

				mCaughtFishType = caughtFish->GetColor();

				mAllCaughtFish[2] = true;
			}
			if (mGreenFish->GetCatchStatus() && (mGreenFish->GetState() == Actor::EActive))
			{
				fishPos = mGreenFish->GetPosition();
				caughtFish = mGreenFish;

				mCaughtFishType = caughtFish->GetColor();

				mAllCaughtFish[3] = true;
			}
			if (mBlueFish->GetCatchStatus() && (mBlueFish->GetState() == Actor::EActive))
			{
				fishPos = mBlueFish->GetPosition();
				caughtFish = mBlueFish;

				mCaughtFishType = caughtFish->GetColor();

				mAllCaughtFish[4] = true;
			}
			if (mPurpleFish->GetCatchStatus() && (mPurpleFish->GetState() == Actor::EActive))
			{
				fishPos = mPurpleFish->GetPosition();
				caughtFish = mPurpleFish;

				mCaughtFishType = caughtFish->GetColor();

				mAllCaughtFish[5] = true;
			}
			if (mPinkFish->GetCatchStatus() && (mPinkFish->GetState() == Actor::EActive))
			{
				fishPos = mPinkFish->GetPosition();
				caughtFish = mPinkFish;

				mCaughtFishType = caughtFish->GetColor();

				mAllCaughtFish[6] = true;
			}
			if (mSpottedFish->GetCatchStatus() && (mSpottedFish->GetState() == Actor::EActive))
			{
				fishPos = mSpottedFish->GetPosition();
				caughtFish = mSpottedFish;

				mCaughtFishType = caughtFish->GetColor();

				mAllCaughtFish[7] = true;
			}

			Vector3 newFishPos = Vector3(playerPos.x, playerPos.y + 150.0f, playerPos.z - 25.0f);
			caughtFish->SetPosition(newFishPos);
			Quaternion tPose = Quaternion(newFishPos, 0.0f);
			caughtFish->SetRotation(tPose);
			caughtFish->UpdateActor(mCurrentTime);

			new ScreenSaysFishCaught(this);
			mWin.Restart();
			mWin.SetPaused(false);

			caughtFish->SetCatchStatus(true);
			caughtFish->SetState(Actor::EDead);
			Vector3 bobberSpawnPoint(20000.0, 20000.0, 0.0);
			mSingleBobber->SetPosition(bobberSpawnPoint);

		}

		break;
	}

	case SDLK_e:
	{
		if (!isReelingIn && GetState() == GameState::EGameplay) // Probably shouldn't open the inventory while reeling in a fish
		{														// or while there is another menu open - Rebecca Morris
			new InventoryMenu(this);
		}

		break;
	}
	default:
		break;
	}
}

void Game::UpdateGame()
{
	// Compute delta time
	// Wait until 16ms has elapsed since last frame

	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();
	mCurrentTime = deltaTime; // Rebecca Morris

	if (mGameState == EGameplay)
	{
		// Update all actors
		mUpdatingActors = true;
		for (auto actor : mActors)
		{
			actor->Update(deltaTime);
		}
		mUpdatingActors = false;

		// Move any pending actors to mActors
		for (auto pending : mPendingActors)
		{
			pending->ComputeWorldTransform();
			mActors.emplace_back(pending);
		}
		mPendingActors.clear();

		// Add any dead actors to a temp vector
		std::vector<Actor*> deadActors;
		for (auto actor : mActors)
		{
			if (actor->GetState() == Actor::EDead)
			{
				deadActors.emplace_back(actor);
			}
		}

		// Delete dead actors (which removes them from mActors)
		for (auto actor : deadActors)
		{
			delete actor;
		}
	}

	// Update audio system
	mAudioSystem->Update(deltaTime);
	
	// Update UI screens
	for (auto ui : mUIStack)
	{
		if (ui->GetState() == UIScreen::EActive)
		{
			ui->Update(deltaTime);
		}
	}
	// Delete any UIScreens that are closed
	auto iter = mUIStack.begin();
	while (iter != mUIStack.end())
	{
		if ((*iter)->GetState() == UIScreen::EClosing)
		{
			delete *iter;
			iter = mUIStack.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	if(mSingleBobber->GetInWaterStatus() == true)
	{
		Vector3 hooksSunkenPosition = mSingleBobber->GetPosition();
		// this is where we spawn the hook in for the game. - Adam Caligiuri
		hooksSunkenPosition.z = hooksSunkenPosition.z - 200;

		currentHook->SetPosition(hooksSunkenPosition);
	}

	if (mSingleBobber->GetInWaterStatus() == false)
	{
		currentHook->SetPosition(Vector3(20000, 20000, 20000));
	}

}

void Game::GenerateOutput()
{
	mRenderer->Draw();
}

void Game::LoadData()
{
	// Load English text
	LoadText("Assets/English.gptext");

	// Create actors
	Actor* a = nullptr;
	Quaternion q;
	//MeshComponent* mc = nullptr;
	const float start = -1250.0f;
	const float size = 250.0f;

	// Left/right walls under pond
	q = Quaternion(Vector3::UnitX, Math::PiOver2);
	for (int i = 0; i < 10; i++)
	{
		//Replaced Underplane
		a = new PlaneActor(this, "Assets/UnderPlane.gpmesh"); //Underplane right starts at -1250, -1000, -600 and ends at 1250, -1000, -600 -Rebecca Morris
		a->SetPosition(Vector3(start + i * size, -start + size, -600.0f));
		a->SetRotation(q);
	}

	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::PiOver2));
	// Forward/back walls under pond
	for (int i = 0; i < 10; i++)
	{
		//Replaced Underplane
		a = new PlaneActor(this, "Assets/UnderPlane.gpmesh"); //Underplane forward starts at -1500, -1250, -600 and ends at -1500, 1250, -600
		a->SetPosition(Vector3(start - size, start + i * size, -600.0f));
		a->SetRotation(q);

		//Replaced Underplane
		a = new PlaneActor(this, "Assets/UnderPlane.gpmesh"); //Underplane backward starts at 1500, -1250, -600 and ends at 1500, 1250, -600
		a->SetPosition(Vector3(-start + size, start + i * size, -600.0f));
		a->SetRotation(q);
	}
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			//Replaced UnderPlane
			a = new PlaneActor(this, "Assets/UnderPlane.gpmesh");
			a->SetPosition(Vector3(start + i * size, start + j * size, -750.0f)); //Should this be changed to -600?
		}
	}

	// Setup lights
	mRenderer->SetAmbientLight(Vector3(0.8f, 0.8f, 0.8f));
	DirectionalLight& dir = mRenderer->GetDirectionalLight();
	dir.mDirection = Vector3(0.0f, -0.707f, -0.707f);

	// UI elements
	mHUD = new HUD(this);
	
	// Start music
	mMusicEvent.Restart();

	// Enable relative mouse mode for camera look
	SDL_SetRelativeMouseMode(SDL_TRUE);
	// Make an initial call to get relative to clear out
	SDL_GetRelativeMouseState(nullptr, nullptr);

	// Different camera actors
	mFPSActor = new FPSActor(this);
	mRedFish = new BasicFish(this, 'r');
	mYellowFish = new BasicFish(this, 'y');
	mOrangeFish = new BasicFish(this, 'o');
	mGreenFish = new BasicFish(this, 'g');
	mBlueFish = new BasicFish(this, 'b');
	mPurpleFish = new BasicFish(this, 'p');
	mPinkFish = new BasicFish(this, 'k');
	mSpottedFish = new BasicFish(this, 's');
	mSingleBobber = new BobberActor(this);
	mSingleBobber->SetPosition(Vector3(20000, 20000, 0));
	currentHook = new Hook(this);
	currentHook->SetPosition(Vector3(20000, 20000, 2000));
	// Tweak start speed and rotation for each fish individually Adam
	mRedFish->SetPosition(Vector3(1000.0f, 400.0f, -450.0f));
	mYellowFish->SetPosition(Vector3(600.0f, 700.0f, -450.0f));
	mOrangeFish->SetPosition(Vector3(800.0f, 800.0f, -550.0f));
	mGreenFish->SetPosition(Vector3(1200.0f, 400.0f, -500.0f));
	mBlueFish->SetPosition(Vector3(400.0f, 700.0f, -500.0f));
	mPurpleFish->SetPosition(Vector3(200.0f, 400.0f, -500.0f));
	mPinkFish->SetPosition(Vector3(0.0f, 800.0f, -500.0f));
	mSpottedFish->SetPosition(Vector3(-200.0f, 400.0f, -500.0f));
	
	// Setup floor

	mSkybox = new SkyBox(this);

	// Everything invisible must be drawn after the skybox so it knows what the rest of the world should look like - Rebecca Morris

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//Replaced GrassPlane
			a = new PlaneActor(this, "Assets/GrassPlane.gpmesh");
			a->SetPosition(Vector3(start + i * size, start + j * size, -100.0f));
		}
		for (int j = 4; j < 5; j++)
		{
			//Replaced ShorePlane
			a = new PlaneActor(this, "Assets/ShorePlane.gpmesh");
			a->SetPosition(Vector3(start + i * size, start + j * size, -100.0f));
		}
		for (int j = 5; j < 10; j++) // Rebecca Morris
		{
			//Replaced WaterPlane
			a = new PlaneActor(this, "Assets/WaterPlane.gpmesh");
			a->SetPosition(Vector3(start + i * size, start + j * size, -100.0f));
		}

		for (int j = 5; j < 10; j++) // Rebecca Morris
		{
			//Replaced WaterPlane
			a = new PlaneActor(this, "Assets/InvisiblePlane.gpmesh");
			a->SetPosition(Vector3(start + i * size, start + j * size, -200.0f));
		}
		for (int j = 4; j < 5; j++)
		{
			//Replaced ShorePlane
			a = new PlaneActor(this, "Assets/ShorePlane.gpmesh");
			a->SetPosition(Vector3(start + i * size, start + j * size, -100.0f));
		}
		for (int j = 2; j < 4; j++)
		{
			//Replaced GrassPlane
			a = new PlaneActor(this, "Assets/GrassPlane.gpmesh");
			a->SetPosition(Vector3(start + i * size, start + j * size, -100.0f));
		}
	}

	q = Quaternion(Vector3::UnitX, Math::PiOver2);
	for (int i = 0; i < 10; i++)
	{
		//Replaced FencePlane
		a = new PlaneActor(this, "Assets/FencePlane.gpmesh"); //Wall starts at 1000, 1500, 400 and ends at ...
		a->SetPosition(Vector3((start + 300.0f) + i * size, start - size, 0.0f));
		a->SetRotation(q);

		//Replaced FencePlane
		a = new PlaneActor(this, "Assets/FencePlane.gpmesh");
		a->SetPosition(Vector3((start + 300.0f) + i * size, -start + size, 0.0f));
		a->SetRotation(q);
	}

	for (int i = 0; i < 10; i++)
	{
		//Replaced Underplane
		a = new PlaneActor(this, "Assets/InvisiblePlane.gpmesh"); //Underplane left starts at -1250, 250, -600 and ends at 1250, 250, -600
		a->SetPosition(Vector3(start + i * size, start - (size - 1750.0f), -600.0f));
		a->SetRotation(q);

	}

	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::PiOver2));
	// Forward/back walls
	for (int i = 0; i < 10; i++)
	{
		//Replaced FencePlane
		a = new PlaneActor(this, "Assets/FencePlane.gpmesh");
		a->SetPosition(Vector3(start - size, start + i * size, 0.0f));
		a->SetRotation(q);

		//Replaced FencePlane
		a = new PlaneActor(this, "Assets/FencePlane.gpmesh");
		a->SetPosition(Vector3(-start + size, start + i * size, 0.0f));
		a->SetRotation(q);
	}

}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Clear the UI stack
	while (!mUIStack.empty())
	{
		delete mUIStack.back();
		mUIStack.pop_back();
	}

	if (mRenderer)
	{
		mRenderer->UnloadData();
	}
}

void Game::Shutdown()
{
	UnloadData();
	TTF_Quit();
	delete mPhysWorld;
	if (mRenderer)
	{
		mRenderer->Shutdown();
	}
	if (mAudioSystem)
	{
		mAudioSystem->Shutdown();
	}
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	// If we're updating actors, need to add to pending
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	// Is it in pending actors?
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// Is it in actors?
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::AddBobber(BobberActor* bobber) // Rebecca Morris
{
	mBobbers.emplace_back(bobber);
}

void Game::RemoveBobber(BobberActor* bobber) // Rebecca Morris
{
	// Is it in pending actors?
	auto iter = std::find(mPendingBobbers.begin(), mPendingBobbers.end(), bobber);
	if (iter != mPendingBobbers.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mPendingBobbers.end() - 1);
		mPendingBobbers.pop_back();
	}

	// Is it in actors?
	iter = std::find(mBobbers.begin(), mBobbers.end(), bobber);
	if (iter != mBobbers.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mBobbers.end() - 1);
		mBobbers.pop_back();
	}
}

void Game::AddBasicFish(BasicFish* fish)
{
	mBasicFishes.emplace_back(fish);
}

void Game::RemoveBasicFish(BasicFish* fish)
{
	// Is it in pending actors?
	auto iter = std::find(mPendingBasicFish.begin(), mPendingBasicFish.end(), fish);
	if (iter != mPendingBasicFish.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mPendingBasicFish.end() - 1);
		mPendingBasicFish.pop_back();
	}

	// Is it in actors?
	iter = std::find(mBasicFishes.begin(), mBasicFishes.end(), fish);
	if (iter != mBasicFishes.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mBasicFishes.end() - 1);
		mBasicFishes.pop_back();
	}
}

void Game::PushUI(UIScreen* screen)
{
	mUIStack.emplace_back(screen);
}

void Game::PopUI(UIScreen* screen)
{
	mUIStack.pop_back();
	//mUIStack.emplace_back(screen);
}

Font* Game::GetFont(const std::string& fileName)
{
	auto iter = mFonts.find(fileName);
	if (iter != mFonts.end())
	{
		return iter->second;
	}
	else
	{
		Font* font = new Font(this);
		if (font->Load(fileName))
		{
			mFonts.emplace(fileName, font);
		}
		else
		{
			font->Unload();
			delete font;
			font = nullptr;
		}
		return font;
	}
}

void Game::LoadText(const std::string& fileName)
{
	// Clear the existing map, if already loaded
	mText.clear();
	// Try to open the file
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		SDL_Log("Text file %s not found", fileName.c_str());
		return;
	}
	// Read the entire file to a string stream
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	// Open this file in rapidJSON
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);
	if (!doc.IsObject())
	{
		SDL_Log("Text file %s is not valid JSON", fileName.c_str());
		return;
	}
	// Parse the text map
	const rapidjson::Value& actions = doc["TextMap"];
	for (rapidjson::Value::ConstMemberIterator itr = actions.MemberBegin();
		itr != actions.MemberEnd(); ++itr)
	{
		if (itr->name.IsString() && itr->value.IsString())
		{
			mText.emplace(itr->name.GetString(), 
				itr->value.GetString());
		}
	}
}

const std::string& Game::GetText(const std::string& key)
{
	static std::string errorMsg("**KEY NOT FOUND**");
	// Find this text in the map, if it exists
	auto iter = mText.find(key);
	if (iter != mText.end())
	{
		return iter->second;
	}
	else
	{
		return errorMsg;
	}
}

Skeleton* Game::GetSkeleton(const std::string& fileName)
{
	auto iter = mSkeletons.find(fileName);
	if (iter != mSkeletons.end())
	{
		return iter->second;
	}
	else
	{
		Skeleton* sk = new Skeleton();
		if (sk->Load(fileName))
		{
			mSkeletons.emplace(fileName, sk);
		}
		else
		{
			delete sk;
			sk = nullptr;
		}
		return sk;
	}
}

Animation* Game::GetAnimation(const std::string& fileName)
{
	auto iter = mAnims.find(fileName);
	if (iter != mAnims.end())
	{
		return iter->second;
	}
	else
	{
		Animation* anim = new Animation();
		if (anim->Load(fileName))
		{
			mAnims.emplace(fileName, anim);
		}
		else
		{
			delete anim;
			anim = nullptr;
		}
		return anim;
	}
}
