#include "stdafx.h"
#include <windows.h>
#include <string>
#include <vector>
//#define private public // dirty. for testing purposes only. Or maybe it's better to use "friend"?
#define private protected // dirty. for testing purposes only. Or maybe it's better to use "friend"?
#include "MsiProgram.h"
using namespace std;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(MsiProgramTest)
	{
	public:
		
		TEST_METHOD(Create_Base_MsiProgram_Class)
		{
			MsiProgram m;
			bool actual = m.is_installed;
			Assert::AreEqual(actual, false);
		}

		//Test to find out if Visual studio is installed
		//Input is the VS upgrade id 822F50511C8BA103EAEE1A9F3C6B5002
		//Expected result 8AC9843965660A335ACA0EDEDE1BC7E3
		/*TEST_METHOD(MsiProgram_Get_PID)
		{
			MsiProgram m("822F50511C8BA103EAEE1A9F3C6B5002");
			string actual = m.product_code;
			Assert::AreEqual(actual, (string)"8AC9843965660A335ACA0EDEDE1BC7E3");
		}*/

		TEST_METHOD(MsiProgram_VS_is_installed)
		{
			MsiProgram m("822F50511C8BA103EAEE1A9F3C6B5002");
			bool actual = m.is_installed;
			Assert::AreEqual(actual, true);
		}

		TEST_METHOD(MsiProgram_VS_install_date)
		{
			MsiProgram m("822F50511C8BA103EAEE1A9F3C6B5002");
			string actual = m.get_ProgramProperty("InstallDate");
			Assert::AreEqual(actual, (string)"20140523");
		}

		/*TEST_METHOD(MsiProgram_VS_patches_installed)
		{
			MsiProgram m("822F50511C8BA103EAEE1A9F3C6B5002");
			vector<string> patches = m.get_InstalledPatchIds();
			Assert::AreEqual( patches.size(), (size_t)3);
		}*/

		
/*		TEST_METHOD(MsiProgram_Convert_Date)
		{
			MsiProgram m("822F50511C8BA103EAEE1A9F3C6B5002");
			double input_date = 20141220;
			DWORD date = m.get_FormattedDate(input_date);
			Assert::AreEqual( date, (DWORD)2014);
		}*/

	};


}
