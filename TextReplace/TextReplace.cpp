#include "CTextReplace.h"
#include <iostream>
#include <thread>
#include <filesystem>

int _tmain(int argc, TCHAR* argv[])
{
	if (argc < 2)
	{
		return 0;
	}

	CTextReplace replace;
	replace.push_replace(argv[1]);
	
	std::filesystem::path curPath = std::filesystem::current_path();
	if (false == std::filesystem::exists(curPath))
	{
		std::cout << "not exsists path: " << curPath << std::endl;
		return 0;
	}

	for (const auto& entry : std::filesystem::recursive_directory_iterator(curPath))
	{
		if (".go" != entry.path().extension())
		{
			continue;
		}

		replace.push_file(entry.path(), entry.file_size());
	}

	replace.run(std::thread::hardware_concurrency());

	return 0;
}