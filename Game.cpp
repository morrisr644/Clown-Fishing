// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

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
#include "YellowFish.h"
#include "RedFish.h"
#include "PlaneActor.h"
#include "WaterPlaneActor.h"
#include "UnderPlaneActor.h"
#include "InvisiblePlaneActor.h"
#include "TargetActor.h"
#include "BobberActor.h"
#include "PauseMenu.h"
#include "InventoryMenu.h"
#include "CatchScreen.h"
#include "SkyBox.h"
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "Font.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>

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
	if (!mRenderer->Initialize(1024.0f, 768.0f))
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
	//mMusicEvent.SetPaused(true);

	mReeling = mAudioSystem->PlayEvent("event:/ReelingIn");
	mReeling.SetPaused(true);
	
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

void Game::AddWaterPlane(WaterPlaneActor* water) // Rebecca Morris
{
	mWaterPlanes.emplace_back(water);
}

void Game::RemoveWaterPlane(WaterPlaneActor* water) // Rebecca Morris
{
	auto iter = std::find(mWaterPlanes.begin(), mWaterPlanes.end(), water);
	mWaterPlanes.erase(iter);
}

void Game::AddUnderPlane(UnderPlaneActor* under) // Rebecca Morris
{
	mUnderPlanes.emplace_back(under);
}

void Game::RemoveUnderPlane(UnderPlaneActor* under) // Rebecca Morris
{
	auto iter = std::find(mUnderPlanes.begin(), mUnderPlanes.end(), under);
	mUnderPlanes.erase(iter);
}

void Game::AddInvisiblePlane(InvisiblePlaneActor* invis) // Rebecca Morris
{
	mInvisiblePlanes.emplace_back(invis);
}

void Game::RemoveInvisiblePlane(InvisiblePlaneActor* invis) // Rebecca Morris
{
	auto iter = std::find(mInvisiblePlanes.begin(), mInvisiblePlanes.end(), invis);
	mInvisiblePlanes.erase(iter);
}

bool Game::GetAllCaughtFish(int index)
{
	return mAllCaughtFish[index];
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
	case '2':
	{
		// Load Russian text
		LoadText("Assets/Russian.gptext");
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
		if (isReelingIn)
		{
			mReeling.SetPaused(false);
			//mReeling.Restart();

			Vector3 bobberPos = mSingleBobber->GetPosition();

			Vector3 playerPos = mFPSActor->GetPosition();

			float offsetFloat = 25.0f;

			auto hookedFish = mBasicFish;

			Vector3 fishPos;
			// Find out which fish was hooked
			//for (auto fish : mBasicFishes)
			//{
			//	if (fish->GetLineStatus())
			//	{
			//		fishPos = fish->GetPosition();
			//		fish->SetPosition(playerPos);

			//		hookedFish = fish;
			//		//mSingleBobber->SetPosition(playerPos);
			//	}
			//}

			YellowFish* yfish = mYellowFish; 
			if (yfish->GetLineStatus() && yfish->GetState() == Actor::EActive)
			{
				fishPos = yfish->GetPosition();
				//yfish->SetPosition(playerPos);

				hookedFish = yfish;
				//mSingleBobber->SetPosition(playerPos);
			}
			RedFish* rfish = mRedFish;
			if (rfish->GetLineStatus() && rfish->GetState() == Actor::EActive)
			{
				fishPos = rfish->GetPosition();
				//rfish->SetPosition(playerPos);

				hookedFish = rfish;
				//mSingleBobber->SetPosition(playerPos);
			}

			Vector3 bobberFacePlayer = playerPos - bobberPos;
			bobberFacePlayer.Normalize();

			Vector3 offsetFromReel;
			offsetFromReel.x = bobberFacePlayer.x * offsetFloat;
			offsetFromReel.y = bobberFacePlayer.y * offsetFloat;
			offsetFromReel.z = bobberFacePlayer.z * offsetFloat;

			Vector3 newBobberPos;// = Vector3(playerPos.x, playerPos.y + 25.0f, playerPos.z);
			//newBobberPos.x = bobberPos.x + offsetFromReel.x;
			//newBobberPos.y = bobberPos.y + offsetFromReel.y;
			//newBobberPos.z = bobberPos.z + offsetFromReel.z;



			Vector3 fishFacePlayer = playerPos - fishPos;
			fishFacePlayer.Normalize();

			Vector3 fishOffsetFromReel;
			fishOffsetFromReel.x = fishFacePlayer.x * offsetFloat;
			fishOffsetFromReel.y = fishFacePlayer.y * offsetFloat;
			fishOffsetFromReel.z = fishFacePlayer.z * offsetFloat;


			Vector3 newFishPos; //= Vector3(playerPos.x, playerPos.y + 25.0f, playerPos.z);
			
			if (!(hookedFish->GetCatchStatus()))
			{
				if (bobberPos.z > -100.0)
				{
					newBobberPos = Vector3(bobberPos.x + offsetFromReel.x, bobberPos.y + offsetFromReel.y, bobberPos.z);
					newFishPos = Vector3(fishPos.x + fishOffsetFromReel.x, fishPos.y + fishOffsetFromReel.y, fishPos.z);
				}
				else
				{
					newBobberPos = Vector3(bobberPos.x + offsetFromReel.x, bobberPos.y + offsetFromReel.y, bobberPos.z + offsetFromReel.z);
					newFishPos = Vector3(fishPos.x + fishOffsetFromReel.x, fishPos.y + fishOffsetFromReel.y, fishPos.z + offsetFromReel.z);
				}



				//StopReeling();
			}

			hookedFish->SetPosition(newFishPos);
			mSingleBobber->SetPosition(newBobberPos);
		}
		else
		{
			mReeling.SetPaused(true);

			Vector3 playerPos = mFPSActor->GetPosition();

			auto caughtFish = mBasicFish;

			Vector3 fishPos;

			if (mRedFish->GetCatchStatus() && (mRedFish->GetState() == Actor::EActive))
			{
				fishPos = mRedFish->GetPosition();
				caughtFish = mRedFish;
				mCaughtFishType = 1;

				mAllCaughtFish[0] = true;

				Vector3 newFishPos = Vector3(playerPos.x, playerPos.y + 150.0f, playerPos.z - 25.0f);
				caughtFish->SetPosition(newFishPos);
				Quaternion tPose = Quaternion(newFishPos, 0.0f);
				caughtFish->SetRotation(tPose);
				caughtFish->UpdateActor(mCurrentTime);

				new CatchScreen(this);

				caughtFish->SetState(Actor::EDead);
			}

			if (mYellowFish->GetCatchStatus() && (mYellowFish->GetState() == Actor::EActive))
			{
				fishPos = mYellowFish->GetPosition();
				caughtFish = mYellowFish;
				mCaughtFishType = 2;

				mAllCaughtFish[1] = true;

				Vector3 newFishPos = Vector3(playerPos.x, playerPos.y + 150.0f, playerPos.z - 25.0f);
				caughtFish->SetPosition(newFishPos);
				Quaternion tPose = Quaternion(newFishPos, 0.0f);
				caughtFish->SetRotation(tPose);
				caughtFish->UpdateActor(mCurrentTime);

				new CatchScreen(this);

				caughtFish->SetState(Actor::EDead);
			}

		}

		break;
	}

	case SDLK_e:
	{
		if (!isReelingIn && GetState() == GameState::EGameplay) // Probably shouldn't open the inventory while reeling in a fish
		{														// or while there is another menu open
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
	

	// Left/right walls
	q = Quaternion(Vector3::UnitX, Math::PiOver2);
	for (int i = 0; i < 10; i++)
	{
		a = new PlaneActor(this); //Wall starts at 1000, 1500, 400 and ends at ...
		a->SetPosition(Vector3((start + 250.0f) + i * size, start - size, 400.0f));
		a->SetRotation(q);

		/*a = new PlaneActor(this);
		a->SetPosition(Vector3((start + 250.0f) + i * size, start - size, -400.0f)); // What is this? -R
		a->SetRotation(q);*/
		
		a = new PlaneActor(this);
		a->SetPosition(Vector3((start + 250.0f) + i * size, -start + size, 400.0f));
		a->SetRotation(q);
	}

	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::PiOver2));
	// Forward/back walls
	for (int i = 0; i < 10; i++)
	{
		a = new PlaneActor(this);
		a->SetPosition(Vector3(start - size, start + i * size, 400.0f));
		a->SetRotation(q);

		a = new PlaneActor(this);
		a->SetPosition(Vector3(-start + size, start + i * size, 400.0f));
		a->SetRotation(q);
	}

	// Left/right walls under pond
	q = Quaternion(Vector3::UnitX, Math::PiOver2);
	for (int i = 0; i < 10; i++)
	{
		a = new UnderPlaneActor(this); //Underplane left starts at -1250, 250, -600 and ends at 1250, 250, -600
		a->SetPosition(Vector3(start + i * size, start - (size - 1750.0f), -600.0f));
		a->SetRotation(q);

		a = new UnderPlaneActor(this); //Underplane right starts at -1250, -1000, -600 and ends at 1250, -1000, -600
		a->SetPosition(Vector3(start + i * size, -start + size, -600.0f));
		a->SetRotation(q);
	}

	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::PiOver2));
	// Forward/back walls under pond
	for (int i = 0; i < 10; i++)
	{
		a = new UnderPlaneActor(this); //Underplane forward starts at -1500, -1250, -600 and ends at -1500, 1250, -600
		a->SetPosition(Vector3(start - size, start + i * size, -600.0f));
		a->SetRotation(q);

		a = new UnderPlaneActor(this); //Underplane backward starts at 1500, -1250, -600 and ends at 1500, 1250, -600
		a->SetPosition(Vector3(-start + size, start + i * size, -600.0f));
		a->SetRotation(q);
	}

	// Setup lights
	mRenderer->SetAmbientLight(Vector3(0.8f, 0.8f, 0.8f));
	DirectionalLight& dir = mRenderer->GetDirectionalLight();
	dir.mDirection = Vector3(0.0f, -0.707f, -0.707f);
	//dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	//dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	// UI elements
	mHUD = new HUD(this);
	
	// Start music
	mMusicEvent = mAudioSystem->PlayEvent("event:/Music2");
	//mMusicEvent.SetPaused(false);
	mMusicEvent.Restart();

	// Enable relative mouse mode for camera look
	SDL_SetRelativeMouseMode(SDL_TRUE);
	// Make an initial call to get relative to clear out
	SDL_GetRelativeMouseState(nullptr, nullptr);

	// Different camera actors
	mFPSActor = new FPSActor(this);
	mBasicFish = new BasicFish(this);
	mRedFish = new RedFish(this);
	mYellowFish = new YellowFish(this);
	mSingleBobber = new BobberActor(this);
	mSingleBobber->SetPosition(Vector3(2000, 2000, 0));
	mRedFish->SetPosition(Vector3(1000.0f, 400.0f, -350.0f)); // why does the fish float?
	mYellowFish->SetPosition(Vector3(1000.0f, 500.0f, -350.0f));
	//mBasicFish->SetPosition(Vector3(1000.0f, 300.0f, -250.0f)); // why does the fish float?

	// Create target actors
	a = new TargetActor(this);
	a->SetPosition(Vector3(1450.0f, 0.0f, 100.0f));
	a = new TargetActor(this);
	a->SetPosition(Vector3(1450.0f, 0.0f, 400.0f));
	a = new TargetActor(this);
	a->SetPosition(Vector3(1450.0f, -500.0f, 200.0f));
	a = new TargetActor(this);
	a->SetPosition(Vector3(1450.0f, 500.0f, 200.0f));
	a = new TargetActor(this);
	a->SetPosition(Vector3(0.0f, -1450.0f, 200.0f));
	a->SetRotation(Quaternion(Vector3::UnitZ, Math::PiOver2));
	a = new TargetActor(this);
	a->SetPosition(Vector3(0.0f, 1450.0f, 200.0f));
	a->SetRotation(Quaternion(Vector3::UnitZ, -Math::PiOver2));
	
	// Setup floor
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			a = new UnderPlaneActor(this);
			a->SetPosition(Vector3(start + i * size, start + j * size, -750.0f)); //Should this be changed to -600?
		}
		for (int j = 0; j < 5; j++)
		{
			a = new PlaneActor(this);
			a->SetPosition(Vector3(start + i * size, start + j * size, -100.0f));
		}
		for (int j = 5; j < 10; j++) // Rebecca Morris
		{
			a = new WaterPlaneActor(this);
			a->SetPosition(Vector3(start + i * size, start + j * size, -100.0f));
		}
	} 

	//const float largerSize = 500.0f;

	// Draw the invisible wall
	q = Quaternion(Vector3::UnitX, Math::PiOver2);
	for (int i = 0; i < 10; i++)
	{
		a = new InvisiblePlaneActor(this); //Invisible Wall starts at -1250, 250, -300 and ends at 1250, 250, -300
		a->SetPosition(Vector3(start + i * size, start - (size - 1750.0f), -300.0f));
		a->SetRotation(q);
	}

	mSkybox = new SkyBox(this);
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
