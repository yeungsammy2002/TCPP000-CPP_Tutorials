#include "Database.h"

using namespace regulus::db;

Database::Database() : m_logger(nullptr)
{
}

Database::~Database()
{
	m_logger->close();
}

bool Database::load(const string & type, const string & path)
{
	bool is_person = false;

	if ("student" == type)
	{
		is_person = true;
		m_students_path = path;
		return load_items("student", true);
	}
	else if ("staff" == type)
	{
		is_person = true;
		m_staffs_path = path;
		return load_items("staff", true);
	}
	else if ("ipad" == type)
	{
		m_ipads_path = path;
		return load_items("ipad", false);
	}
	else if ("blist" == type)
	{
		m_blist_path = path;
		return load_blist("student");
	}
	else if ("bslist" == type)
	{
		m_bslist_path = path;
		return load_blist("staff");
	}
	return false;
}

bool Database::load_items(const string & type, bool is_person)
{
	string line;
	std::vector<string> titles;
	std::ifstream fin;

	if ("student" == type)
	{
		fin.open(m_students_path, std::ios::in);
		if (fin.fail())
		{
			log_error("failed to open student list, location: ", m_students_path.c_str());
			return false;
		}
		log_info("student list open successfully, location: %s", m_students_path.c_str());
	}

	if ("staff" == type)
	{
		fin.open(m_staffs_path, std::ios::in);
		if (fin.fail())
		{
			log_error("failed to open staff list, location: ", m_staffs_path.c_str());
			return false;
		}
		log_info("staff list open successfully, location: %s", m_staffs_path.c_str());
	}

	if ("ipad" == type)
	{
		fin.open(m_ipads_path, std::ios::in);
		if (fin.fail())
		{
			log_error("failed to open ipad list, location: %s", m_ipads_path.c_str());
			return false;
		}
		log_info("ipad list open successfully, location: %s", m_ipads_path.c_str());
	}

	//int line_count = 0;
	while (std::getline(fin, line))
	{
		//log_debug(line.c_str());

		line = trim(line);

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
			s = trim(s, "\"");
			item[titles[i]] = s;
		}

		item["type"] = type;
		item["is_person"] = is_person ? "true" : "false";

		m_items.push_back(item);

		//++line_count;
	}

	//log_debug(m_items[m_items.size() - 1]["ChineseName"].c_str());
	//log_debug(m_items[m_items.size() - 1]["IpadNumber"].c_str());

	fin.close();
	return true;
}

bool Database::load_blist(const string & type)
{
	string line;
	std::vector<string> titles;
	std::ifstream fin;

	string path;
	std::vector<BList_Item> * p_list = nullptr;
	if ("student" == type)
	{
		path = m_blist_path;
		p_list = &m_blist;
	}
	else if ("staff" == type)
	{
		path = m_bslist_path;
		p_list = &m_bslist;
	}
	else
	{
		string err_msg = "no such type: " + type + " for borrowed list txt file";
		log_error(err_msg.c_str());
		return false;
	}
	std::vector<BList_Item> & list = *p_list;

	fin.open(path, std::ios::in);
	if (fin.fail())
	{
		string err_msg = path + " not exist yet";
		log_info(err_msg.c_str());
		return false;
	}
	log_info("borrowed list open successfully, location: %s", path.c_str());

	while (std::getline(fin, line))
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
			s = trim(s, "\"");
			blitem[titles[i]] = s;
		}

		list.push_back(blitem);
	}

	//log_debug(m_blist[m_blist.size() - 1]["ChineseName"].c_str());
	fin.close();
	return true;
}

bool Database::load_history(int year, int month, int day)
{
	return load_history(year, month, day, m_history_folder);
}

bool Database::load_history(int year, int month, int day, const string & path)
{
	m_rlist.clear();
	m_history_folder = path;
	m_history_path = m_history_folder + "history_" + std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day) + ".txt";
	log_debug(m_history_path.c_str());
	string line;
	std::vector<string> titles;
	std::ifstream fin;
	fin.open(m_history_path, std::ios::in);
	if (fin.fail())
	{
		log_debug("no %s exist yet", m_history_path.c_str());
		return false;
	}
	log_debug("returned list loaded successfully, location: %s", m_history_path.c_str());
	while (std::getline(fin, line))
	{
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

		string s;
		BList_Item rlitem;
		std::stringstream ss(line);

		for (int i = 0; std::getline(ss, s, '\t'); ++i)
		{
			s = trim(s, "\"");
			rlitem[titles[i]] = s;
		}

		m_rlist.push_back(rlitem);
	}
	fin.close();
	return true;
}

bool Database::load_staff_history(int year, int month)
{
	return load_staff_history(year, month, m_staff_history_folder);
}

bool Database::load_staff_history(int year, int month, const string & path)
{
	m_rslist.clear();
	m_staff_history_folder = path;
	m_staff_history_path = m_staff_history_folder + "staff_history_" + std::to_string(year) + "-" + std::to_string(month) + ".txt";
	string line;
	std::vector<string> titles;
	std::ifstream fin;
	fin.open(m_staff_history_path, std::ios::in);
	if (fin.fail())
	{
		log_debug("no %s exist yet", m_staff_history_path.c_str());
		return false;
	}
	log_debug("returned list loaded successfully, location: %s", m_staff_history_path.c_str());
	while (std::getline(fin, line))
	{
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

		string s;
		BList_Item rslitem;
		std::stringstream ss(line);

		for (int i = 0; std::getline(ss, s, '\t'); ++i)
		{
			s = trim(s, "\"");
			rslitem[titles[i]] = s;
		}

		m_rslist.push_back(rslitem);
	}
	fin.close();
	return true;
}

string Database::trim(string src, const char * remove_chars)
{
	if (src.empty())
		return src;

	src.erase(0, src.find_first_not_of(remove_chars));
	src.erase(src.find_last_not_of(remove_chars) + 1);
	return src;
}

string Database::trim(string s)
{
	if (s.empty())
		return s;

	s.erase(0, s.find_first_not_of(" \n\r"));
	s.erase(s.find_last_not_of(" \n\r") + 1);
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

Database::BList_Item_Itr Database::find_bslitem(const string & cardId)
{
	for (auto it = m_bslist.begin(); it != m_bslist.end(); ++it)
	{
		if (cardId == (*it)["IpadCardID"])
			return it;
	}
	return m_bslist.end();
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

bool Database::write_blist_to_file(const string & type)
{
	string path;
	std::vector<string> * p_titles = nullptr;
	std::vector<BList_Item> * p_list = nullptr;
	std::stringstream ss;

	if ("student" == type)
	{
		path = m_blist_path;
		p_titles = &b_list_titles;
		p_list = &m_blist;
	}
	else if ("staff" == type)
	{
		path = m_bslist_path;
		p_titles = &bs_list_titles;
		p_list = &m_bslist;
	}
	else
	{
		log_error("type: %s doesn't exist", type.c_str());
		return false;
	}
	std::ofstream fout;
	fout.open(path);
	if (fout.fail())
	{
		log_error("failed to open: %s", path.c_str());
		return false;
	}
	std::vector<string> & titles = *p_titles;
	std::vector<BList_Item> & list = *p_list;

	// set titles
	for (int i = 0; i < titles.size(); ++i)
	{
		fout << titles[i];
		if (i != titles.size() - 1)
			fout << '\t';
	}
	fout << '\n';

	// rewrite borrowers
	for (int i = 0; i < list.size(); ++i)
	{
		for (int j = 0; j < titles.size(); ++j)
		{
			fout << '\"' << list[i][titles[j]] << "\"";
			if (j != titles.size() - 1)
				fout << '\t';
		}
		fout << '\n';
	}

	fout.flush();
	fout.close();
	return true;
}

bool Database::write_to_history(BList_Item returner, const string & type)
{
	string history_path;
	Time now = Time::now();
	std::vector<string> * p_titles = nullptr;
	string rlitem;
	bool hastitles = false;

	if ("student" == type)
	{
		history_path = m_history_folder + "history_" + std::to_string(now.m_year) + "-" + std::to_string(now.m_month) + "-" + std::to_string(now.m_day) + ".txt";
		p_titles = &m_history_titles;
		hastitles = has_titles("rlist");
	}
	else if ("staff" == type)
	{
		history_path = m_staff_history_folder + "staff_history_" + std::to_string(now.m_year) + "-" + std::to_string(now.m_month) + ".txt";
		p_titles = &m_staff_history_titles;
		hastitles = has_titles("rslist");
	}
	else
	{
		log_error("type: %s doesn't exist", type.c_str());
		return false;
	}

	std::ofstream fout;
	std::vector<string> & titles = *p_titles;

	fout.open(history_path, std::ios::app);
	if (fout.fail())
	{
		log_error("open file failed: %s", history_path.c_str());
		return false;
	}

	if (!hastitles)
	{
		for (int i = 0; i < titles.size(); ++i)
		{
			fout << titles[i];
			if (i != titles.size() - 1)
				fout << '\t';
		}
		fout << '\n';
	}

	for (int i = 0; i < titles.size(); ++i)
	{
		fout << '\"' << returner[titles[i]] << '\"';
		if (i != titles.size() - 1)
			fout << '\t';
	}
	fout << '\n';
	fout.flush();
	fout.close();

	return true;
}

bool Database::has_titles(const string & type)
{
	string line;
	string path = "";
	Time now = Time::now();

	if ("blist" == type)
		path = m_blist_path;

	if ("rlist" == type)
	{
		path = m_history_folder + "history_" + std::to_string(now.m_year) + "-" + std::to_string(now.m_month) + "-" + std::to_string(now.m_day) + ".txt";
	}

	if ("rslist" == type)
	{
		path = m_staff_history_folder + "staff_history_" + std::to_string(now.m_year) + "-" + std::to_string(now.m_month) + ".txt";
	}

	std::ifstream fin;
	fin.open(path, std::ios::in);

	if (fin.fail())
		return false;

	std::getline(fin, line);
	line = trim(line);
	if ("" == line)
	{
		fin.close();
		return false;
	}

	if ('\"' != line[0])
	{
		fin.close();
		return true;
	}

	fin.close();
	return false;
}