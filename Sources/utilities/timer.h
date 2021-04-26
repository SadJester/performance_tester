#pragma once

#include <chrono>
#include <iostream>


class Timer
{
public:
	Timer()
		: _begin(std::chrono::steady_clock::now())
		, _time{}
	{}

	Timer(const std::string& message)
		: _begin(std::chrono::steady_clock::now())
		, _message(message)
		, _time{}
	{
	}

	~Timer()
	{
		auto end = std::chrono::steady_clock::now();
		_time = std::chrono::duration_cast<std::chrono::microseconds>(end - _begin);
		if (_dump)
			dump();
	}

	void dump()
	{
		std::cout << "Time " << _message << ": " << std::chrono::duration_cast<std::chrono::seconds>(elapsed_time()).count() << std::endl;
	}

	std::chrono::microseconds elapsed_time() const {
		auto end = std::chrono::steady_clock::now();
		return std::chrono::duration_cast<std::chrono::microseconds>(end - _begin);
	}

private:
	std::chrono::microseconds _time;
	std::chrono::steady_clock::time_point  _begin;
	std::string _message;
	bool _dump = false;
};
