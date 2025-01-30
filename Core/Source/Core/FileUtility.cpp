#include "corepch.h"
#include "FileUtility.h"

#include <windows.h>
#include <shobjidl.h>  
#include <string>

namespace Utility
{
    std::wstring StringToWString(const std::string& _str)
    {
        return std::wstring(_str.begin(), _str.end());
    }

    std::string WStringToString(const std::wstring& _wstr)
    {
        return std::string(_wstr.begin(), _wstr.end());
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
}