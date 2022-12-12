#pragma once

#include "pch.h"
#include <filesystem>
#include <vector>

class CTextReplace
{
public:
	using replaces_type = std::vector<std::pair<std::tstring, std::tstring>>;
	using file_info = std::pair<const std::filesystem::path, std::uintmax_t>;
	using file_list = std::vector<file_info>;
	using file_list_iter = file_list::iterator;

private:
	replaces_type m_replace;
	file_list m_list;

	std::tstring& rtrim(std::tstring& s, const TCHAR* t = _T("\t\r\n\f\v"));
	std::tstring read_file(const std::filesystem::path& _path);
	bool file_replace(const std::filesystem::path& _path, std::size_t _fileSize);
	bool text_replace(std::tstring& _text);
	void parallel_file_replace(file_list_iter start, file_list_iter end, std::size_t number_thread);

public:
	bool empty() const;
	void push_replace(const std::tstring& pairs);
	void push_file(const std::filesystem::path& path, std::uintmax_t file_size);
	bool run(std::size_t thread_count);
};