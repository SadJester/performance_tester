#include "test_utilities.h"

namespace test_utilities
{
	namespace generators
	{
		void set_srand()
		{
			static bool srandSet = false;
			if (srandSet)
				return;
			srand((unsigned)time(NULL));
			srandSet = true;
		}
	}
}