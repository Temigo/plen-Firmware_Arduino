/*!
	@file      Profiler.h
	@brief     Tiny metrics class for Arduino.
	@author    Kazuyuki TAKASE
	@copyright The MIT License - http://opensource.org/licenses/mit-license.php
*/

#pragma once

#ifndef PROFILER_H
#define PROFILER_H


class __FlashStringHelper;


namespace Utility
{
	class Profiler;
}


/*!
	@brief Tiny metrics class

	Refer to the usage below.
	@code
	void anyFunction()
	{
		// When instantiate the class, it outputs message given.
		volatile Utility::Profiler p(F("anyFunction()"));

		any code here...

		// Outputting metrics about this function when profiler instance has destroyed.
	}
	@endcode
*/
class Utility::Profiler
{
private:
	//! @brief Serial outputting delay
	inline static const int DELAY_MS() { return 50; }

	unsigned long m_begin;
	unsigned long m_end;

	void m_tabbing();

	// Disable copy constructor and operator =.
	Profiler(const Profiler&);
	Profiler& operator=(const Profiler&);

public:
	/*!
		@brief Constructor

		@param [in] fsh_ptr Please set startup message.

		@attention
		Arduino IDE is using optimization option -Os, so you should use volatile prefix.
	*/
	Profiler(const __FlashStringHelper* fsh_ptr);

	/*!
		@brief Destructor
	*/
	~Profiler();
};

#endif // PROFILER_H
