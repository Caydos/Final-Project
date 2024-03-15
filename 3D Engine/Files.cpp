#include "Files.h"
#include <iostream>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <windows.h>
#include <direct.h> // For _wgetcwd and _wchdir
#include <commdlg.h> // Common dialogs
#include <string>

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

const char DIRECTORY_SEPARATOR = '/';
std::string Files::GetParentDirectory(const std::string& path)
{
	size_t last_sep = path.find_last_of(DIRECTORY_SEPARATOR);
	if (last_sep == std::string::npos) {
		// No directory separator found, return the current directory or an empty string
		return ".";
	}
	else if (last_sep == 0) {
		// The only directory separator is at the beginning (root directory)
		return std::string(1, DIRECTORY_SEPARATOR);
	}
	else {
		// Return the substring from the beginning to the last separator
		// It removes the trailing directory name
		return path.substr(0, last_sep);
	}
}

std::string Files::AddPaths(std::string path1, std::string path2) {
	std::replace(path1.begin(), path1.end(), '/', '\\');
	std::string newPath2 = path2;
	std::replace(newPath2.begin(), newPath2.end(), '/', '\\');

	std::vector<std::string> dirs;
	std::stringstream pathStream(path1);
	std::string segment;

	while (std::getline(pathStream, segment, '\\')) {
		if (!segment.empty() && segment != ".") {
			if (segment == ".." && !dirs.empty()) {
				dirs.pop_back();
			}
			else if (segment != "..") {
				dirs.push_back(segment);
			}
		}
	}

	std::stringstream path2Stream(newPath2);
	while (std::getline(path2Stream, segment, '\\')) {
		if (!segment.empty() && segment != ".") {
			if (segment == ".." && !dirs.empty()) {
				dirs.pop_back();
			}
			else if (segment != "..") {
				dirs.push_back(segment);
			}
		}
	}

	std::string result;
	for (const auto& dir : dirs) {
		if (!result.empty()) {
			result += "\\";
		}
		result += dir;
	}

	return result;
}


// Function to convert const char* to std::wstring
// Function to convert const char* to std::wstring
std::wstring ConvertToWideString(const char* str) {
	// Calculate the size of the destination wide string
	int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	if (len == 0) {
		return L"";
	}

	// Allocate a buffer for the wide string
	std::wstring wstr(len, L'\0'); // Allocate space for characters including the null terminator
	// Perform the conversion
	MultiByteToWideChar(CP_UTF8, 0, str, -1, &wstr[0], len);

	return wstr;
}
// Function to convert std::wstring to std::string (UTF-8)
std::string ConvertToNarrowString(const std::wstring& wstr)
{
	if (wstr.empty())
	{
		return "";
	}

	// Calculate the length of the buffer needed for the narrow string (including null terminator)
	int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	if (len == 0)
	{
		// Conversion failed
		return "";
	}

	// Allocate a buffer for the narrow string
	std::string str(len, '\0');
	// Perform the conversion
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], len, NULL, NULL);

	// Remove the null terminator inserted by WideCharToMultiByte
	str.pop_back();

	return str;
}
std::wstring RemoveInitialDirectory(const std::wstring& initialDir, const std::wstring& finalPath)
{
	// Check if the final path starts with the initial directory
	if (finalPath.substr(0, initialDir.length()) == initialDir) {
		// If so, remove the initial directory (and potentially the slash) from the final path
		size_t startPos = initialDir.length();

		// Remove the separator if present
		if (finalPath[startPos] == L'\\' || finalPath[startPos] == L'/') {
			startPos++;
		}

		return finalPath.substr(startPos);
	}
	// If the initial directory is not at the start of the final path, return the final path as is
	return finalPath;
}

Files::FileSearch Files::OpenSearchDialog()
{
	FileSearch searchResults;
	std::string returnValue;
	std::wstring outFilePath;
	std::wstring filter = L"\0";

	OPENFILENAMEW ofn;
	WCHAR szFile[260];
	WCHAR initialDir[260];
	HWND hwnd = NULL;

	WCHAR savedCwd[260];
	_wgetcwd(savedCwd, 260);

	// Get the full path name of the executable
	GetModuleFileNameW(NULL, initialDir, 260);
	// Extract the directory
	for (size_t i = 0; i < 2; i++)
	{
		std::wstring::size_type pos = std::wstring(initialDir).find_last_of(L"\\/");
		if (pos != std::wstring::npos)
		{
			initialDir[pos] = '\0'; // Remove the executable name, leaving the directory
		}
	}

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile) / sizeof(WCHAR);
	ofn.lpstrFilter = filter.c_str();
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = initialDir; // Use the directory path
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameW(&ofn) == TRUE)
	{
		outFilePath = ofn.lpstrFile;
	}
	_wchdir(savedCwd);

	searchResults.originalPath = "../" + ConvertToNarrowString(RemoveInitialDirectory(initialDir, outFilePath));
	std::filesystem::path pathObj(searchResults.originalPath);

	searchResults.path = pathObj.parent_path().string() + "/";

	searchResults.name = pathObj.stem().string();

	searchResults.extension = pathObj.extension().string();
	searchResults.achieved = true;
	return searchResults;
}
