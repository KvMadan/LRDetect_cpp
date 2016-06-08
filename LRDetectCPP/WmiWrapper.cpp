#include "WmiWrapper.h"

namespace WmiWrapper {

	std::wstring GetWmiProperty(LPCWSTR property_name, std::wstring class_name)
	{
		std::wstring out;

		HRESULT hRes = CoInitializeEx(NULL, COINIT_MULTITHREADED);  
		if(FAILED(hRes))  
			return L"Unable to launch COM";  
  
		hRes = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, 0);
		if(FAILED(hRes))		
		  return L"Unable to initialize security";  

		IWbemLocator* pLocator = NULL;  
		if(FAILED(hRes = CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pLocator))))  
			return L"Unable to create a WbemLocator";  
		
		IWbemServices* pService = NULL; 
		if(FAILED(hRes = pLocator->ConnectServer(L"root\\CIMV2", NULL, NULL, NULL, WBEM_FLAG_CONNECT_USE_MAX_WAIT, NULL, NULL, &pService)))  
		{  
			pLocator->Release();  
			return L"Unable to connect to CIMV2";  
		}  

		IEnumWbemClassObject* pEnumerator = NULL;
		std::wstring query = L"SELECT * FROM ";
		query.append(class_name);
		BSTR bstrQuery = SysAllocStringLen(query.data(), query.size());

		if(FAILED(hRes = pService->ExecQuery(L"WQL", bstrQuery, WBEM_FLAG_FORWARD_ONLY, NULL, &pEnumerator)))  
		{  
			pLocator->Release();  
			pService->Release();  
			return L"Unable to retrive data";
		} 

		IWbemClassObject* clsObj = NULL; 
		int numElems;

		while((hRes = pEnumerator->Next(WBEM_INFINITE, 1, &clsObj, (ULONG*)&numElems)) != WBEM_S_FALSE)  
	  {
			if(FAILED(hRes))  
				break;  
  
			VARIANT vRet;  
			VariantInit(&vRet);  
			if(SUCCEEDED(clsObj->Get(property_name, 0, &vRet, NULL, NULL)) && vRet.vt == VT_BSTR)  
			{  
				out = std::wstring(vRet.bstrVal);  
				VariantClear(&vRet);  
			} 
			clsObj->Release();  
		}

		pEnumerator->Release();  
		pService->Release();  
		pLocator->Release(); 

		return out;
	}

}