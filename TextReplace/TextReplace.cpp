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

	std::filesystem::path cur_path = std::filesystem::current_path();
	if (false == std::filesystem::exists(cur_path))
	{
		std::cout << "not exsists path: " << cur_path << std::endl;
		return 0;
	}

	CTextReplace replace;

	for (const auto& entry : std::filesystem::recursive_directory_iterator(cur_path))
	{
		if (".go" != entry.path().extension())
		{
			continue;
		}

		replace.push_file(entry.path(), entry.file_size());
	}

	if (replace.empty())
	{
		return 0;
	}

	replace.push_replace(argv[1]);
	replace.run(std::thread::hardware_concurrency());

	return 0;
}