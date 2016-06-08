#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\LRDetectCPP\WmiWrapper.h"
#include <thread>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
		void task1()
		{
			std::wstring in = WmiWrapper::GetWmiProperty(L"DataExecutionPrevention_SupportPolicy", std::wstring(L"Win32_OperatingSystem"));
		}

	TEST_CLASS(WmiWrapperTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{

			std::thread thread_1 = std::thread(task1);

			Assert::AreEqual(true, true);
		}

	};
}