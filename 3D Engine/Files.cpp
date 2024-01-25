#include "Files.h"
#include <iostream>
#include <filesystem>
#include <fstream>


namespace fs = std::filesystem;
std::vector<std::string> Files::GetAllAtPath(const char* _path)
{
	std::vector<std::string> paths;

	try {
		if (fs::exists(_path) && fs::is_directory(_path))
		{
			for (const auto& entry : fs::directory_iterator(_path))
			{
				paths.push_back(entry.path().filename().generic_string());
			}
		}
		else
		{
			std::cout << "[Files::GetAllAtPath] - Unable to locate folder : " << _path << std::endl;
		}
	}
	catch (const fs::filesystem_error& err)
	{
		std::cerr << "[Files::GetAllAtPath] - Filesystem error: " << err.what() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "[Files::GetAllAtPath] - Standard exception: " << e.what() << std::endl;
	}
	return paths;
}

std::string Files::GetFileContent(const char* _path)
{
	std::string content;
	std::ifstream file(_path);

	if (!file.is_open()) {
		std::cerr << "Error opening file at path : " << _path << std::endl;
		return content;
	}

	std::string line;
	while (std::getline(file, line)) {
		content += line;
	}

	file.close();
	return content;
}

void Files::Create(const char* _path, const char* _name, const char* _extension, const char* _content)
{
	std::string fileName = _path;
	fileName += _name;
	fileName += _extension;

	std::ofstream file;

	// Open the file with the given filename
	file.open(fileName);

	// Check if the file is open
	if (file.is_open()) {
		// Write the content to the file
		file << _content;

		// Close the file
		file.close();
	}
	else {
		// Handle the case where the file couldn't be opened
		std::cerr << "Unable to open file: " << fileName << std::endl;
	}
}


#include <tchar.h>
#include <ShlObj_core.h>
#include <atlcore.h>

std::wstring GetProcessName()
{
	std::wstring wstrPath;
	TCHAR buffer[MAX_PATH] = { 0 };  // Buffer to hold the path

	// Get the full path of the executable
	if (GetModuleFileName(NULL, buffer, MAX_PATH) > 0)
	{
		// Convert TCHAR array to std::wstring
		std::wstring wstrPath(buffer);

		// Extract the file name from the path
		size_t pos = wstrPath.find_last_of(L"\\/");
		if (pos != std::wstring::npos)
		{
			std::wstring exeName = wstrPath.substr(pos + 1);

			return exeName;
		}
		else
		{
			std::cout << "Failed to extract executable name" << std::endl;
		}
	}
	else
	{
		std::cout << "GetModuleFileName failed" << std::endl;
	}
	return wstrPath;
}

char* Files::GetFolderPath(const char* _searchTitle)
{
	TCHAR szDir[MAX_PATH];
	BROWSEINFO bInfo;
	bInfo.hwndOwner = FindWindowA(NULL, (LPCSTR) GetProcessName().c_str());
	bInfo.pidlRoot = NULL;
	bInfo.pszDisplayName = szDir; // Address of a buffer to receive the display name of the folder selected by the user
	//bInfo.lpszTitle = (LPCWSTR) _searchTitle; // Title of the dialog
	bInfo.lpszTitle = (LPCWSTR)_searchTitle; // Title of the dialog
	bInfo.ulFlags = 0;
	bInfo.lpfn = NULL;
	bInfo.lParam = 0;
	bInfo.iImage = -1;

	LPITEMIDLIST lpItem = SHBrowseForFolder(&bInfo);
	if (lpItem != NULL)
	{
		SHGetPathFromIDList(lpItem, szDir);
		char path[MAX_PATH];
		wcstombs(path, szDir, wcslen(szDir) + 1);

		// Replace backslashes with forward slashes in the folder path
		for (int i = 0; path[i]; i++)
		{
			if (path[i] == '\\')
			{
				path[i] = '/';
			}
		}

		return path;
	}
	return NULL;
}



char* Files::GetFilePath(const char* _filters)
{
	OPENFILENAME ofn = { 0 };
	TCHAR szFile[260] = { 0 };
	// Initialize remaining fields of OPENFILENAME structure
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = FindWindowA(NULL, (LPCSTR)GetProcessName().c_str());
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = (LPCWSTR)_filters;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		char path[MAX_PATH];
		wcstombs(path, ofn.lpstrFile, wcslen(ofn.lpstrFile) + 1);

		return path;
	}
	return NULL;

}
