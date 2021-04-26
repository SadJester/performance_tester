#include "StringsTest.h"

#include <vector>
#include <string>
#include <string_view>
#include <iostream>


namespace miscellaneous
{
	namespace strings
	{

		std::vector<std::string_view> SplitStringInplace(const std::string& value, char separator)
		{
			size_t beginIdx = 0u;
			size_t idx = value.find(separator);

			std::vector<std::string_view> result;

			while (idx != std::string::npos)
			{
				std::string_view chunk{ &value[beginIdx], idx - beginIdx };
				result.push_back(chunk);
				beginIdx = idx + 1;
				idx = value.find(separator, beginIdx);
			}

			std::string_view chunk{ &value[beginIdx], value.size() - beginIdx };
			result.push_back(chunk);

			return result;
		}

		bool fnmatch(std::string_view mask, std::string_view value)
		{


			return false;
		}

		void test_split()
		{
			std::string t = "test";
			for (auto chunk : SplitStringInplace(t, ';'))
				std::cout << chunk << "|";
			std::cout << std::endl;


			std::string tt = "test;test1;";
			for (auto chunk : SplitStringInplace("test;test1;", ';'))
				std::cout << chunk << "|";
			std::cout << std::endl;
		}


		void test_fnmatch()
		{
			std::cout << "full match: " << std::boolalpha << fnmatch("full/match.txt", "full/match.txt") << std::endl;
			std::cout << "all files: " << std::boolalpha << fnmatch("full/*", "full/match.txt") << std::endl;
			std::cout << "no ext match: " << std::boolalpha << fnmatch("full/*.txt", "full/match.xml") << std::endl;
			std::cout << "no folder match: " << std::boolalpha << fnmatch("full/*.txt", "full/match.xml") << std::endl;
		}

	}

	void test_strings()
	{
		strings::test_fnmatch();
		strings::test_split();
	}
}
