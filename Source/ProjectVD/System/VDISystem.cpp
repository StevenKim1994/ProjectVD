// Fill out your copyright notice in the Description page of Project Settings.


#include "System/VDISystem.h"

// Add default functionality here for any IVDISystem functions that are not pure virtual.

void IVDISystem::Initialize()
{
	bIsInitialized = true;
}

void IVDISystem::Release()
{
	bIsInitialized = false;
}	