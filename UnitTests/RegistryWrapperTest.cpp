#include "stdafx.h"
#include <Windows.h>
#include "CppUnitTest.h"
#include "RegistryWrapper.h"
#include <winreg.h>
#include <vector>

using namespace std;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
//#define NativeAssert Assert

namespace UnitTests
{		
	TEST_CLASS(RegWrapperTest)
	{
	public:
		
		TEST_METHOD(Get_RegSZ_Value_64)
		{
			auto actual = RegWrapper::regGetValue(HKLM, "SOFTWARE\\1LRDETCTCPP_TEST", "RegSzName", "");
			Assert::AreEqual(actual, (string)"This is REG_SZ value from 64bit reg");
		}
		
		TEST_METHOD(Get_RegSZ_Unexisting_Value)
		{
			auto actual = RegWrapper::regGetValue(HKLM, "SOFTWARE\\1LRDETCTCPP_TEST_", "RegSzName", "IamDefaultValue");
			Assert::AreEqual(actual, (string)"IamDefaultValue");
		}
		
		TEST_METHOD(Get_RegMultiSZ_Value)
		{
			auto actual = RegWrapper::regGetValue(HKLM, "SOFTWARE\\1LRDETCTCPP_TEST", "RegMltSz", "");
			Assert::AreEqual(actual.size(), (size_t)339);
		}		
		TEST_METHOD(Get_RegDwd_Value_64)
		{
			auto actual = RegWrapper::regGetValue(HKLM, "SOFTWARE\\1LRDETCTCPP_TEST", "RegDwd", "");
			Assert::AreEqual(actual, (string)"1298");
		}

		TEST_METHOD(Get_RegSZ_Value_32)
		{
			auto actual = RegWrapper::regGetValue32(HKLM, "SOFTWARE\\1LRDETCTCPP_TEST", "RegSzName", "");
			Assert::AreEqual(actual, (string)"This is REG_SZ value from 32bit reg");
		}
		
		TEST_METHOD(Find_RegSZ_Value_From_32)
		{
			auto actual = RegWrapper::find_reg_value(HKLM, "SOFTWARE\\1LRDETCTCPP_TEST", "RegSzKeyExistingOnlyHere", "");
			Assert::AreEqual(actual, (string)"This is REG_SZ value from 32bit reg");
		}
		
		TEST_METHOD(Enumerate_value_names)
		{
			auto key = "SOFTWARE\\1LRDETCTCPP_TEST\\UpgradeCodes\\43JL3K45L34KJ3L45LK4L34K53L4534345";
			auto actual = RegWrapper::enumerateValueNames(HKLM, key);
			Assert::AreEqual((actual.size() == (size_t)3), actual[1] == "2222");
		}

		TEST_METHOD(Get_First_Value_64)
		{
			auto key = "SOFTWARE\\1LRDETCTCPP_TEST\\UpgradeCodes\\822F50511C8BA103EAEE1A9F3C6B5002";
			auto actual = RegWrapper::get_first_value_name(HKLM, key);
			Assert::AreEqual(actual, (string)"8AC9843965660A335ACA0EDEDE1BC7E3");
		}

		TEST_METHOD(Enumerate_RegKeys)
		{
			auto actual = RegWrapper::enumerateKeys(HKLM, "SOFTWARE\\1LRDETCTCPP_TEST\\UpgradeCodes");
			Assert::AreEqual((actual.size() == (size_t)2), actual[1] == "822F50511C8BA103EAEE1A9F3C6B5002");
		}

		//Enumerate the keys in HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\1LRDETCTCPP_TEST32
		//There are 3 keys there
		TEST_METHOD(Enumerate_RegKeys_32)
		{
			auto actual = RegWrapper::enumerateKeys(HKLM, "SOFTWARE\\1LRDETCTCPP_TEST", KEY32);
			Assert::AreEqual((actual.size() == (size_t)3), actual[2] == "SubKey3");
		}

		TEST_METHOD(RegGetValue_RegSz_64)
		{
			string actual = RegWrapper::regGetValue(HKLM, "SOFTWARE\\1LRDETCTCPP_TEST", "RegSzName", "");
			Assert::AreEqual(actual, (string)"This is REG_SZ value from 64bit reg");
		}

		TEST_METHOD(RegGetValue_RegSz_32)
		{
			string actual = RegWrapper::regGetValue(HKLM, "SOFTWARE\\1LRDETCTCPP_TEST", "RegSzName", "", KEY32);
			Assert::AreEqual(actual, (string)"This is REG_SZ value from 32bit reg");
		}

		TEST_METHOD(RegGetValue_RegDword_64)
		{
			string actual = RegWrapper::regGetValue(HKLM, "SOFTWARE\\1LRDETCTCPP_TEST", "RegDwd", "");
			Assert::AreEqual(actual, (string)"1298");
		}
		
		TEST_METHOD(RegGetValue_RegDword_32)
		{
			string actual = RegWrapper::regGetValue(HKLM, "SOFTWARE\\1LRDETCTCPP_TEST", "RegDwdName", "", KEY32);
			Assert::AreEqual(actual, (string)"1");
		}
				
		TEST_METHOD(RegGetDwd_RegDword_64)
		{
			string actual = RegWrapper::regGetDwdValue(HKLM, "SOFTWARE\\1LRDETCTCPP_TEST", "RegDwdBigName", "");
			Assert::AreEqual(actual, (string)"1255000");
		}

		TEST_METHOD(RegGetValue_RegMultiSz_64)
		{
			string actual = RegWrapper::regGetValue(HKLM, "SOFTWARE\\1LRDETCTCPP_TEST", "RegMltSz", "");
			Assert::AreEqual(actual.size(), (size_t)339);
		}
	};
}