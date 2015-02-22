//====================================================================================
// FILE:    Timer.cpp ----------------------------------------------------------------
//------------------------------------------------------------------------------------


//====================================================================================
// INTERNAL INCLUDES -----------------------------------------------------------------
//------------------------------------------------------------------------------------
#include "Timer.h"


//====================================================================================
// CONSTRUCTOR -----------------------------------------------------------------------
//------------------------------------------------------------------------------------
CTimer::CTimer()
{
	// SET INITIALISATION VALUES ----------
	//-------------------------------------
	mSecondsPerCount = 0.0;
	mDeltaTime = -1.0;

	mBaseTime = 0;
	mPausedTime = 0;
	mPrevTime = 0;
	mCurTime = 0;

	mAppIsStopped = false;

	// DETERMINE COUNTERS PER SECOND ------
	//-------------------------------------
	__int64 countsPerSecond;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecond);
	mSecondsPerCount = 1.0 / static_cast<double>(countsPerSecond);
}


//====================================================================================
// TIMER CLASS GETTERS -----------------------------------------------------------
//------------------------------------------------------------------------------------
float CTimer::GetTotalTime() const
{
	// Returns total time elapsed since the last time the function Reset() was called.
	// Removes the time in which the app has been paused/stopped from the total value

	// Check if the app has stopped
	if (mAppIsStopped)
	{
		// The app is stopped/out of focus. Don't count time which has passed whilst the app was stopped
		return static_cast<float>(((mStopTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
	}
	else
	{
		// Discount CurTime when the app is not paused
		return static_cast<float>(((mCurTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
	}
}

float CTimer::GetDeltaTime() const
{
	// Return the delta time
	return static_cast<float>(mDeltaTime);
}


//====================================================================================
// TIMER CLASS METHODS -----------------------------------------------------------
//------------------------------------------------------------------------------------
void CTimer::Reset()
{
	// Get the current time
	__int64 curTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);

	// Set variables
	mBaseTime = curTime;
	mPrevTime = curTime;
	mStopTime = 0;
	mAppIsStopped = false;
}

void CTimer::Start()
{
	// Get the time when timer is started
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	// Determine time passes between stopping and starting
	if (mAppIsStopped)
	{
		mPausedTime += (startTime - mStopTime);

		// Set variables
		mPrevTime = startTime;
		mStopTime = 0;
		mAppIsStopped = false;
	}
}

void CTimer::Stop()
{
	// Ensure app is not already stopped
	if (!mAppIsStopped)
	{
		// Get current time
		__int64 curTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&curTime);

		// Set variables
		mStopTime = curTime;
		mAppIsStopped = true;
	}
}

void CTimer::Tick()
{
	// If app is stopped, keep delta time at 0
	if (mAppIsStopped)
	{
		mDeltaTime = 0.0;

		// Exit Tick() function
		return;
	}

	// Get current time
	__int64 curTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);

	// Set variables
	mCurTime = curTime;

	// Calculate time passed (delta time)
	mDeltaTime = (mCurTime - mPrevTime) * mSecondsPerCount;

	// Prepare to begin counting next frame - set previuos time as current time
	mPrevTime = mCurTime;

	// Sometimes when CPU swaps application onto different processor or if it goes into sleep / power save
	// mode, delta time can be left with a negative number.
	// If this has happened, simply set delta time no 0
	if (mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}
}
