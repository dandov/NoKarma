/* 
Copyright 2015 Alvaro Herrasti and Daniel Dovali

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#pragma once

#include <iostream>
#include <map>
#include <unordered_map>
#include <functional>
#include <cstdarg>
#include "GameSystem.h"
#include "SystemMessageSender.h"


template <typename KeyType>
class GameLoop
{
public:
	// Constructs gameloop, recieves comparator function to order templated GameLoops, systemManager owning this gameloop,
	// maxSubSteps maximum number of steps for simulation clamps when time since last render is big and loop runs multiple times with each fixed step,
	// fixedTimeStep discrete timestep that will be passed to each system's update, if zero it passes delta time since last render
	// priority of this gameloop with respect to others in SystemManager to which this is added
	GameLoop(std::function<bool(const KeyType&, const KeyType&)> comparator, SystemMessageSender* sysManager, 
		double maxSubSteps = 1.0, double fixedTimeStep = 0.0, int priority = 0) :
		mMaxSubSteps(maxSubSteps),
		mFixedTimeStep(fixedTimeStep),
		mSystemMap(comparator),
		mComparator(comparator),
		mSysManager(sysManager),
		mTimeAccumulator(0.0),
		mPriority(priority)
	{
	}

	// lvalue copy constructor
	GameLoop(GameLoop&& other) :
		mMaxSubSteps(other.mMaxSubSteps),
		mFixedTimeStep(other.mFixedTimeStep),
		mSystemMap(std::move(other.mSystemMap)),
		mComparator(other.mComparator),
		mSysManager(other.mSysManager),
		mTimeAccumulator(other.mTimeAccumulator),
		mPriority(other.mPriority)
	{
	}

	GameLoop() :
		mMaxSubSteps(0),
		mFixedTimeStep(0),
		mSysManager(nullptr),
		mTimeAccumulator(0.0),
		mPriority(0)
	{
	}


	// Adds new system to run, recieves priority key and object to copy
	template <class T>
	std::shared_ptr<GameSystem> addNewSystem(const KeyType& priorityKey, T&& newSystem) {
		std::shared_ptr<GameSystem> system = std::shared_ptr<GameSystem>(new T(std::move(newSystem)));
		this->mSystemMap[priorityKey] = system;
		const std::type_info* type = &typeid(T);
		mSysManager->addSystem(type, system);
		return system;
	}

	// Adds new system to run, recieves priority and initializes new object with default constructor
	template <class T>
	std::shared_ptr<GameSystem> addNewSystem(const KeyType& priorityKey) {
		std::shared_ptr<GameSystem> system = std::shared_ptr<GameSystem>(new T());
		this->mSystemMap[priorityKey] = system;
		const std::type_info* type = &typeid(T);
		mSysManager->addSystem(type, system);
		return system;
	}

	// removes system based on priority key
	template <class T>
	bool removeSystem(const KeyType& priorityKey) {
		auto sysIt = this->ms
		this->mSystemMap[priorityKey] = system;
		const std::type_info* type = &typeid(T);
		mSysManager->removeSystem(type);
		return system;
	}

	// recieves deltaTime since last render
	void run(double deltaTimeStep, SystemMessageSender* msgSender)
	{
		int loops = 0;
		mTimeAccumulator += deltaTimeStep;

		// runs systems in discrete fixed timesteps for deterministic behavior
		// and clamps to max number of steps if simulation is runing too slow
		while (mTimeAccumulator >= mFixedTimeStep && loops < mMaxSubSteps) {

			// if mFixedTimeStep
			double deltaTimeUpdate = mFixedTimeStep != 0.0 ? mFixedTimeStep : deltaTimeStep;
			for (auto it = this->mSystemMap.begin(); it != this->mSystemMap.end(); it++)
			{
				it->second->update(deltaTimeUpdate, msgSender);
			}
			mTimeAccumulator -= deltaTimeUpdate;
			loops++;
		}
	}

	// Sets priority of this gameloop
	void setPriority(int priority) {
		this->mPriority = priority;
	}

	// gets priority of this gameloop, reference value used to look for inside a SystemManager
	int getPriority() {
		return this->mPriority;
	}

	~GameLoop()
	{
	}
private:
	double mMaxSubSteps;
	double mFixedTimeStep;

	typedef std::map < KeyType, std::shared_ptr<GameSystem>, std::function<bool(const KeyType&, const KeyType&)> >  SystemMap;
	SystemMap mSystemMap;
	typedef std::function<bool(const KeyType&, const KeyType&)> CompareFunc;
	CompareFunc mComparator;
	SystemMessageSender* mSysManager;
	double mTimeAccumulator;
	int mPriority;
};


#endif