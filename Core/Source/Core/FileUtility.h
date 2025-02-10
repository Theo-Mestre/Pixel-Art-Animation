#pragma once

#include <string>

namespace Utility
{
	/// <summary>
	/// Remove the first part of the path and replace it with "..."
	/// Keep only the last part of the path and the file name
	/// </summary>
	/// <param name="_path"></param>
	/// <returns></returns>
	std::string TruncatePath(const std::string& _path);

	std::string OpenFileDialog(const std::string& _fileType, const std::string& _extension);

	std::string SaveFileDialog(const std::vector<std::string>& _extension);
}