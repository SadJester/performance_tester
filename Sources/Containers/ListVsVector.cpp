#include "ListVsVector.h"
#include <memory>
#include <list>
#include <vector>
#include <array>

#include "../utilities/test_utilities.h"


namespace containers
{
	struct SharedStruct
	{
		std::array<int, 5> x;
		SharedStruct(int xx)
		{
		}
	};

	template <typename Con>
	void init_container(Con& container, int count)
	{
		container.resize(count);
		for (int i = 0; i < count; ++i)
		{
			container.push_back(std::make_shared<SharedStruct>(i));
		}
	}

	template <typename Con>
	void test_container(const test_utilities::TestInfo<int>& info)
	{
		Con container;
		container.resize(info.number);
		for (size_t i = 0; i < info.number; ++i)
			container.push_back(std::make_shared<SharedStruct>(i));
		container.clear();
	}

	void test_list_vs_vector()
	{
		constexpr int count = 10000000;
		using namespace test_utilities;

		const size_t cycles = 2;
		const std::vector<size_t> values{ 10, 100, 20000, 10000000 };

		Tester::Compare<int>({
			{"vector", &test_container<std::vector<std::shared_ptr<SharedStruct>>>},
			{"list", &test_container<std::list<std::shared_ptr<SharedStruct>>>}
								},
								{ "Comparison", cycles, values });


		//Tester::Run(&test_container<std::vector<std::shared_ptr<SharedStruct>>>, { "vector", cycles, values });
		//Tester::Run(&test_container<std::list<std::shared_ptr<SharedStruct>>>, { "list", cycles, values });
	}
}