#include "Database.h"

using namespace regulus::db;

Database::Database() : m_logger(nullptr)
{
}

Database::~Database()
{
	m_logger->close();
}

bool Database::in_open(const string & filename)
{
	m_fin.open(filename, std::ios::in);

	if (m_fin.fail())
		return false;

	return true;
}

void Database::in_close()
{
	m_fin.close();
}

void Database::set_path(const string & type, const string & path)
{
	if ("student" == type)
		m_students_path = path;

	if ("staff" == type)
		m_staffs_path = path;

	if ("ipad" == type)
		m_ipads_path = path;

	if ("blist == type")
		m_blist_path = path;
}

bool Database::load(const string & type)
{
	bool is_person = false;

	if ("student" == type)
	{
		is_person = true;

		if (!in_open(m_students_path))
			return false;

		load_items("student", true);
	}
	else if ("staff" == type)
	{
		is_person = true;

		if (!in_open(m_staffs_path))
			return false;

		load_items("staff", true);
	}
	else if ("ipad" == type)
	{
		if (!in_open(m_ipads_path))
			return false;

		load_items("ipad", false);
	}
	else if ("blist" == type)
	{
		if (!in_open(m_blist_path))
			return false;

		load_blist();
	}
	else
		return false;

	in_close();
	return true;
}

void Database::load_items(const string & type, bool is_person)
{
	string line;
	std::vector<string> titles;

	while (std::getline(m_fin, line))
	{
		//log_debug(line.c_str());

		line = trim(line);

		if ("" == line)
			continue;

		// load the titles
		if ('\"' != line[0])
		{
			string title;
			std::stringstream ss(line);

			while (std::getline(ss, title, '\t'))
				titles.push_back(title);

			continue;
		}

		//log_debug(line.c_str());

		string s;
		Item item;
		std::stringstream ss(line);

		// merge each key to appopriate value
		for (int i = 0; std::getline(ss, s, '\t'); ++i)
		{
			s = trim_quote(s);
			item[titles[i]] = s;
		}

		item["type"] = type;
		item["is_person"] = is_person ? "true" : "false";

		m_items.push_back(item);
	}

	//log_debug(m_items[m_items.size() - 1]["ChineseName"].c_str());
	//log_debug(m_items[m_items.size() - 1]["IpadNumber"].c_str());
}

void Database::load_blist()
{
	string line;
	std::vector<string> titles;

	while (std::getline(m_fin, line))
	{
		//log_debug(line.c_str());

		line = trim(line);

		if ("" == line)
			continue;

		// load the titles
		if ('\"' != line[0])
		{
			string title;
			std::stringstream ss(line);

			while (std::getline(ss, title, '\t'))
				titles.push_back(title);

			continue;
		}

		//log_debug(line.c_str());

		string s;
		BList_Item blitem;
		std::stringstream ss(line);

		// merge each key to appopriate value
		for (int i = 0; std::getline(ss, s, '\t'); ++i)
		{
			s = trim_quote(s);
			blitem[titles[i]] = s;
		}

		m_blist.push_back(blitem);
	}

	//log_debug(m_blist[m_blist.size() - 1]["ChineseName"].c_str());
}

string Database::trim(string s)
{
	if (s.empty())
		return s;

	s.erase(0, s.find_first_not_of(" \n\r"));
	s.erase(s.find_last_not_of(" \n\r") + 1);
	return s;
}

string Database::trim_quote(string s)
{
	if (s.empty())
		return s;

	s.erase(0, s.find_first_not_of("\""));
	s.erase(s.find_last_not_of("\"") + 1);
	return s;
}

Database::Item_Itr Database::find_item(const string & cardId)
{
	for (auto it = m_items.begin(); it != m_items.end(); ++it)
	{
		if (cardId == (*it)["CardID"])
			return it;
	}
	return m_items.end();
}

Database::Item_Itr Database::find_item(const string & cardId, const string & type)
{
	for (auto it = m_items.begin(); it != m_items.end(); ++it)
	{
		if (cardId == (*it)["CardID"] && type == (*it)["type"])
			return it;
	}
	return m_items.end();
}

Database::BList_Item_Itr Database::find_blitem(const string & cardId)
{
	for (auto it = m_blist.begin(); it != m_blist.end(); ++it)
	{
		if (cardId == (*it)["IpadCardID"])
			return it;
	}
	return m_blist.end();
}

Database::BList_Item_Itr Database::find_borrower(const string & cardId)
{
	for (auto it = m_blist.begin(); it != m_blist.end(); ++it)
	{
		if (cardId == (*it)["BorrowerCardID"])
			return it;
	}
	return m_blist.end();
}