#pragma once


#include <vector>
#include <map>
#include <string>
#include <memory>
#include <chrono>
#include <iostream>
#include <iomanip>


namespace test_utilities
{
	struct TestRange
	{
		std::string name;
		size_t cycles;
		std::vector<size_t> setsValues;

		std::chrono::microseconds duration;

		TestRange() = default;
		TestRange(std::string name_, size_t cycles_, std::vector<size_t> setsValues_)
			: name(name_)
			, cycles(cycles_)
			, setsValues(setsValues_)
		{}
	};

	struct StatisticsEntry
	{
		std::string name;
		std::chrono::microseconds averageDuration;
	};


	namespace generators
	{
		void set_srand();

		template <typename T>
		struct Generator
		{
			static T random();
		};

		template <>
		struct Generator <std::string>
		{
			static std::string random(const size_t maxLength = 50, bool strictLength = false)
			{
				set_srand();
				std::string tmp;
				static const char alphanum[] =
					"0123456789"
					"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
					"abcdefghijklmnopqrstuvwxyz";

				const size_t length = strictLength ? maxLength : (rand() % maxLength);
				tmp.reserve(length);

				for (size_t i = 0; i < length; ++i)
					tmp += alphanum[rand() % (sizeof(alphanum) - 1)];


				return tmp;
			}
		};


		template <>
		struct Generator <int>
		{
			static int random(const int from = 0, const int to = 50)
			{
				return from + rand() % to;
			}
		};
	}


	enum class Topology
	{
		Sorted,
		Reversed,
		Organpipe,
		Random,
		Rotated // Sorted, but the least is last: 1, 2, 3, ... n-1, 0
	};


	enum class GenerationType
	{
		Heap,
		Stack,
		None
	};


	template <typename T>
	struct DataSet
	{
		using Data = std::unique_ptr<std::vector<T>>;
		using DataOnStack = std::vector<T>;

		static void __insertion(size_t number, std::vector<T>& out)
		{
			out.resize(number);
			for (size_t i = 0; i < number; ++i)
			{
				out[i] = generators::Generator<T>::random();
			}
		}

		static Data generate(size_t number, Topology topology)
		{
			Data data(new std::vector<T>{});
			auto& _raw = *data;
			DataSet::__insertion(number, *data);

			return data;
		}

		static DataOnStack generateOnStack(size_t number, Topology topology)
		{
			std::vector<T> data;
			DataSet::__insertion(number, data);
			return data;
		}
	};

	template <typename DataType>
	struct TestInfo
	{
		typename DataSet<DataType>::DataOnStack dataOnStack;
		typename DataSet<DataType>::Data dataOnHeap;
		size_t number;
	};

	struct Tester
	{
		template <typename DataType>
		using TestMethod = void(*)(const TestInfo<DataType>&);

		template <typename DataType>
		using TestPair = std::pair<std::string, TestMethod<DataType>>;


		template <typename DataType, typename CastToTime = std::chrono::microseconds>
		static void RunWithData(void(*method)(const TestInfo<DataType>&), TestRange&& range, GenerationType generationType = GenerationType::Stack)
		{
			using _DataSet = DataSet<DataType>;
			using _TestInfo = TestInfo<DataType>;

			for (size_t valueForSet : range.setsValues)
			{
				_TestInfo info;
				info.number = valueForSet;
				
				if (generationType == GenerationType::Stack) {
					info.dataOnStack = _DataSet::generateOnStack(valueForSet, Topology::Random);
				}
				else if (generationType == GenerationType::Heap) {
					info.dataOnHeap = _DataSet::generate(valueForSet, Topology::Random);
				}

				range.duration = std::chrono::microseconds{};
				for (size_t i = 0; i < range.cycles; ++i)
				{
					Timer t;
					method(info);
					range.duration += t.elapsed_time();
				}
				auto average = range.duration / range.cycles;
				std::cout << "Time " << range.name << " (set=" << valueForSet << "): " << std::chrono::duration_cast<CastToTime>(average).count() << std::endl;
			}
		}



		template <typename DataType, typename CastToTime = std::chrono::microseconds>
		static void Compare(std::vector<TestPair<DataType>>&& methods, TestRange&& range, GenerationType generationType = GenerationType::Stack)
		{
			using _DataSet = DataSet<DataType>;
			using _TestInfo = TestInfo<DataType>;

			std::map<size_t, std::vector<StatisticsEntry>> statistics;


			std::cout << "       " << std::setw(10) << "|";
			for (auto& testPair : methods)
				std::cout << std::setw(20) << testPair.first;
			std::cout << std::endl;

			for (size_t valueForSet : range.setsValues)
			{
				std::cout << "set =" << std::setw(10) << valueForSet << " | ";
				_TestInfo info;
				info.number = valueForSet;

				auto& entries = statistics[valueForSet];

				if (generationType == GenerationType::Stack) {
					info.dataOnStack = _DataSet::generateOnStack(valueForSet, Topology::Random);
				}
				else if (generationType == GenerationType::Heap) {
					info.dataOnHeap = _DataSet::generate(valueForSet, Topology::Random);
				}


				for (auto& testPair : methods)
				{
					range.duration = std::chrono::microseconds{};
					for (size_t i = 0; i < range.cycles; ++i)
					{
						Timer t;
						testPair.second(info);
						range.duration += t.elapsed_time();
					}
					auto average = range.duration / range.cycles;
					
					entries.push_back({ testPair.first, average });
					std::cout << std::setw(15) << std::chrono::duration_cast<CastToTime>(average).count();
				}
				std::cout << std::endl;
			}
		}

	};

}
