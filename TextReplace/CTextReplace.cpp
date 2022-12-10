#include "CTextReplace.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <future>
#include <utility>

std::tstring& CTextReplace::rtrim(std::tstring& s, const TCHAR* t)
{
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}

std::tstring CTextReplace::read_file(const std::filesystem::path& _path)
{
	std::tifstream ifs(_path);
	if (!ifs.is_open())
	{
		return std::tstring();
	}

	return std::tstring(std::tifstream::_Iter(ifs), std::tifstream::_Iter());
}

bool CTextReplace::file_replace(const std::filesystem::path& _path, std::size_t _fileSize)
{
	std::tstring oldName(_path.filename());
	std::tstring newName(_path.filename());
	newName += _T("new");

	std::filesystem::path tempPath(_path);
	tempPath.replace_filename(newName);

	std::tofstream ofs(tempPath, std::ios_base::out);
	if (!ofs.is_open())
	{
		return false;
	}

	auto data = read_file(_path);

	text_replace(data);
	ofs << data;
	ofs.close();

	try
	{
		std::filesystem::remove(_path);
		std::filesystem::rename(tempPath, _path);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return true;
}

bool CTextReplace::text_replace(std::tstring& _text)
{
	for (const auto& r : m_replace)
	{
		auto pos = _text.find(r.first);

		do
		{
			if (std::tstring::npos == pos)
			{
				break;
			}

			_text.replace(pos, r.first.length(), r.second);
		} while (pos = _text.find(r.first, pos));
	}

	rtrim(_text);

	return true;
}

void CTextReplace::parallel_file_replace(file_list_iter start, file_list_iter end, std::size_t number_thread)
{
	auto len = end - start;
	if (len <= 1)
	{
		return;
	}

	CTextReplace::file_list_iter newStart = (end - start >= number_thread) ? start + number_thread : end;

	auto handle = std::async(&CTextReplace::parallel_file_replace, this, newStart, end, number_thread);

	std::for_each(start, newStart, [&](CTextReplace::file_info& info) {
		file_replace(info.first, info.second);
	});

	handle.get();
}


void CTextReplace::push_replace(const std::tstring& pairs)
{
	std::tregex reg(_T("[, ]+"));
	std::tregex_token_iterator iter(pairs.begin(), pairs.end(), reg, -1);
	std::tregex_token_iterator end;

	for (; iter != end; ++iter)
	{
		auto&& splitString = iter->str();
		auto position = splitString.find(_T("/"));

		if (std::tstring::npos == position)
		{
			continue;
		}

		auto&& left = splitString.substr(0, position);
		auto&& right = splitString.substr(position + 1);

		m_replace.push_back({ left, right });
	}
}

void CTextReplace::push_file(const std::filesystem::path& path, std::uintmax_t file_size)
{
	m_list.push_back({path, file_size });
}

bool CTextReplace::run(std::size_t thread_count)
{
	auto len = m_list.end() - m_list.begin();
	if (0 >= len)
	{
		return false;
	}

	parallel_file_replace(m_list.begin(), m_list.end(), thread_count);

	return true;
}
