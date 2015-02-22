//====================================================================================
// FILE:    Timer.h ------------------------------------------------------------------
//------------------------------------------------------------------------------------

#ifndef _TIMER_H_
#define _TIMER_H_


//====================================================================================
// EXTERNAL INCLUDES -----------------------------------------------------------------
//------------------------------------------------------------------------------------
#include <Windows.h>
#include <windowsx.h>


//====================================================================================
// TIMER CLASS -------------------------------------------------------------------
//------------------------------------------------------------------------------------
class CTimer
{
private:
	// -----------------------------------
	// VARIABLES -------------------------
	// =============
	double mSecondsPerCount;			// How many counts are in a second - determined via QueryPerformance
	double mDeltaTime;					// Time since last frame

	__int64 mBaseTime;						// Time passed without pauseTime etc. being docked
	__int64 mPausedTime;					// How much of BaseTime has been spent in pause (app not in focus)
	__int64 mStopTime;						// Similar to previous
	__int64 mPrevTime;
	__int64 mCurTime;

	bool mAppIsStopped;			// Stores whether the app is not in focus

public:
	// -----------------------------------
	// CONSTRUCTOR -----------------------
	// ===============
	CTimer();

	// -----------------------------------
	// GETTERS ---------------------------
	// ===========
	float GetTotalTime() const;			// Returns the total time passed in seconds
	float GetDeltaTime() const;			// Returns change in time passed in seconds

	// -----------------------------------
	// METHODS ---------------------------
	// ===========
	void Reset();						// Resets the timer. Called before message loop
	void Start();						// Starts the timer. Called after unpasuing app
	void Stop();						// Pauses the timer. Called when app loses focus
	void Tick();						// Increment the timer. Called every frame
};


#endif /* _TIMER_H_ */
