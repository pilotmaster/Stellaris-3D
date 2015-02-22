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

#define WIN32_LEAN_AND_MEAN


namespace sge
{
	//====================================================================================
	// TIMER CLASS -----------------------------------------------------------------------
	//------------------------------------------------------------------------------------
	class CTimer
	{
	public:
		// DECONSTRUCTOR
		//---------------------------------
		~CTimer();


		// ACCESSORS
		//---------------------------------
		float GetTotalTime() const;			// Returns the total time passed in seconds
		float GetDeltaTime() const;			// Returns change in time passed in seconds

		static CTimer* GetTimerInstace()			// Returns instance of the class. Ensures there is only one instance
		{
			// Check if timer instance does not exist & create a new instance
			if (!mpTimer)
			{
				mpTimer = new CTimer();
			}

			// Return timer instance
			return mpTimer;
		}


		// METHODS
		//---------------------------------
		void Reset();						// Resets the timer. Called before message loop
		void Start();						// Starts the timer. Called after unpasuing app
		void Stop();						// Pauses the timer. Called when app loses focus
		void Tick();						// Increment the timer. Called every frame

	private:
		// PRIVATE CONSTRUCTOR
		//---------------------------------
		CTimer();


		// ATTRIBUTES
		//---------------------------------
		static CTimer* mpTimer;					// Self instance - ensures there is only 1

		double mSecondsPerCount;			// How many counts are in a second - determined via QueryPerformance
		double mDeltaTime;					// Time since last frame

		__int64 mBaseTime;						// Time passed without pauseTime etc. being docked
		__int64 mPausedTime;					// How much of BaseTime has been spent in pause (app not in focus)
		__int64 mStopTime;						// Similar to previous
		__int64 mPrevTime;
		__int64 mCurTime;

		bool mAppIsStopped;			// Stores whether the app is not in focus
	};
}

#endif /* _TIMER_H_ */
