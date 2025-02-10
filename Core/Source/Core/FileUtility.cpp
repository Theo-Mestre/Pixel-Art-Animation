#include "corepch.h"
#include "FileUtility.h"

#include <windows.h>
#include <shobjidl.h>  
#include <string>

#include <vector>

namespace Utility
{
	static std::wstring StringToWString(const std::string& _str)
	{
		return std::wstring(_str.begin(), _str.end());
	}

	static std::string WStringToString(const std::wstring& _wstr)
	{
		return std::string(_wstr.begin(), _wstr.end());
	}

	std::string TruncatePath(const std::string& _path)
	{
		std::string truncatedPath = _path;
		size_t pos = _path.find_last_of("\\");
		if (pos != std::string::npos)
		{
			truncatedPath = "..." + _path.substr(pos, _path.size());
		}
		return truncatedPath;
	}

	std::string OpenFileDialog(const std::string& _fileType, const std::string& _extension)
	{
		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		std::string selectedFilePath;

		if (SUCCEEDED(hr))
		{
			IFileOpenDialog* pFileOpen;

			// Create the FileOpenDialog object
			hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, (void**)&pFileOpen);
			if (SUCCEEDED(hr))
			{
				std::wstring wExtension = StringToWString(_extension);
				std::wstring pattern = L"*." + wExtension;
				std::wstring fileType = StringToWString(_fileType);

				COMDLG_FILTERSPEC fileTypes[] =
				{
					{ fileType.c_str(), pattern.c_str()}
				};

				pFileOpen->SetFileTypes(1, fileTypes);

				// Show the Open dialog box
				hr = pFileOpen->Show(NULL);
				if (SUCCEEDED(hr))
				{
					// Get the selected file
					IShellItem* pItem;
					hr = pFileOpen->GetResult(&pItem);
					if (SUCCEEDED(hr))
					{
						PWSTR pszFilePath;
						hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
						if (SUCCEEDED(hr))
						{
							selectedFilePath = WStringToString(pszFilePath);  // Convert to std::string
							CoTaskMemFree(pszFilePath);
						}
						pItem->Release();
					}
				}
				pFileOpen->Release();
			}
			CoUninitialize();
		}
		return selectedFilePath;
	}

	std::string SaveFileDialog(const std::vector<std::string>& _extension)
	{
		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		std::string selectedFilePath;

		if (SUCCEEDED(hr))
		{
			IFileSaveDialog* pFileSave;

			// Create the SaveFileDialog object
			hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, (void**)&pFileSave);
			if (SUCCEEDED(hr))
			{
				// Convert extensions to wstring and set up filter specs
				std::vector<COMDLG_FILTERSPEC> fileTypes(_extension.size());
				std::vector<std::wstring> filterStrings(_extension.size()); // Storage to keep filter names valid

                for (int i = 0; i < _extension.size(); i++)
                {
                    std::wstring wExt = StringToWString(_extension[i]);
                    filterStrings[i] = L"*" + wExt;
                    fileTypes[i].pszName = filterStrings[i].c_str();
                    fileTypes[i].pszSpec = filterStrings[i].c_str();
                }

				pFileSave->SetFileTypes(static_cast<UINT>(fileTypes.size()), fileTypes.data());

				// Default extension (first in the list)
				if (!_extension.empty())
				{
					std::wstring defaultExt = StringToWString(_extension[0]);
					pFileSave->SetDefaultExtension(defaultExt.c_str());
				}

				// Show the Save dialog box
				hr = pFileSave->Show(NULL);
				if (SUCCEEDED(hr))
				{
					// Get the selected file
					IShellItem* pItem;
					hr = pFileSave->GetResult(&pItem);
					if (SUCCEEDED(hr))
					{
						PWSTR pszFilePath;
						hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
						if (SUCCEEDED(hr))
						{
							selectedFilePath = WStringToString(pszFilePath);  // Convert to std::string
							CoTaskMemFree(pszFilePath);
						}
						pItem->Release();
					}
				}
				pFileSave->Release();
			}
			CoUninitialize();
		}
		return selectedFilePath;
	}

}