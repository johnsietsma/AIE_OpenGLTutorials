#pragma once

#include <iostream>
#include <iomanip>
#include <string>

#include <windows.h> 


// A simple timer designed to be used a stack variable. Prints out the results in the destruction.
class Timer
{
public:
	Timer( std::string timerName, bool formatted=true) :
		m_timerName(timerName),
        m_formatted(formatted)
    {
        QueryPerformanceCounter(&m_startTime);
	}

	~Timer()
	{
        LARGE_INTEGER frequency;        // ticks per second
        QueryPerformanceFrequency(&frequency);

        LARGE_INTEGER endTime;
        QueryPerformanceCounter(&endTime);
        auto elapsed = (endTime.QuadPart - m_startTime.QuadPart) * 1000 / frequency.QuadPart;

		// Print the results
        if (m_formatted) std::cout << std::left << std::setw(25);
        std::cout << m_timerName;
        if (m_formatted) std::cout << std::right << std::setw(10);
        std::cout << std::to_string(elapsed) << "ms" << std::endl;
	}

private:
    LARGE_INTEGER m_startTime;
    bool m_formatted;
    float m_averageDenom;
	std::string m_timerName;

};