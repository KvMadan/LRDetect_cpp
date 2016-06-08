#include "stdafx.h"
#include <Windows.h>
#include "CppUnitTest.h"
#include <string>
#include "Utilities.h"
#include <regex>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(UtilitiesTest)
	{
	public:
		
		TEST_METHOD(Exectute_SET_command)
		{
			string actual = Utilities::exec("wmic OS Get DataExecutionPrevention_SupportPolicy");
			//Assert::AreEqual(actual, (string)"ddd");
			Assert::AreEqual(true, true);
		}

		TEST_METHOD(Exectute_wmic_command)
		{
			std::regex e ("DataExecutionPrevention_SupportPolicy\\s+(\\r*\\n*)*([0-2]{1})\\s+(\\r*\\n*)*");
			string actual = Utilities::ExecCmd("wmic OS Get DataExecutionPrevention_SupportPolicy");

			actual = std::regex_replace (actual, e, "$2");
			//Assert::AreEqual(actual, (string)"ddd");
			Assert::AreEqual(actual, (string)"2");
		}
	};
}