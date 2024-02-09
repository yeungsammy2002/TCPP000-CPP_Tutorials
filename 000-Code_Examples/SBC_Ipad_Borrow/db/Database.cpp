#include "Database.h"

using namespace regulus::db;

const char * Database::b_list_titles[8]{
	"Time",
	"BorrowerCardID",
	"ChineseName",
	"EnglishName",
	"ClassName",
	"ClassNumber",
	"IpadCardID",
	"IpadNumber"
};

const char * Database::ipad_list_titles[2]{
	"IpadNumber",
	"CardID"
};

const char * Database::staff_list_titles[4]{
	"EnglishName",
	"ChineseName",
	"Login",
	"CardID"
};

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
		return load_blist();
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
			s = trim_quote(s);
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

bool Database::load_blist()
{
	string line;
	std::vector<string> titles;
	std::ifstream fin;
	fin.open(m_blist_path, std::ios::in);
	if (fin.fail())
	{
		log_info("no borrowed_list.txt exist yet");
		return false;
	}
	log_info("borrowed list open successfully, location: %s", m_blist_path.c_str());

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
			s = trim_quote(s);
			blitem[titles[i]] = s;
		}

		m_blist.push_back(blitem);
	}

	//log_debug(m_blist[m_blist.size() - 1]["ChineseName"].c_str());
	fin.close();
	return true;
}

bool Database::load_history(int year, int month, int day, const string & path)
{
	m_history_folder = path;
	m_history_path = m_history_folder + "history_" + std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day) + ".txt";
	log_info(m_history_path.c_str());
	string line;
	std::vector<string> titles;
	std::ifstream fin;
	fin.open(m_history_path, std::ios::in);
	if (fin.fail())
	{
		log_info("no %s exist yet", m_history_path.c_str());
		return false;
	}
	log_info("returned list loaded successfully, location: %s", m_history_path.c_str());
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
			s = trim_quote(s);
			rlitem[titles[i]] = s;
		}

		m_rlist.push_back(rlitem);
	}
	fin.close();
	return true;
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

bool Database::write_to_file(BList_Item borrower)
{
	bool hastitles = has_titles("blist");
	std::ofstream fout;

	fout.open(m_blist_path, std::ios::app);
	if (fout.fail())
	{
		log_error("open m_blist_path failed: %s", m_blist_path.c_str());
		return false;
	}

	if (!hastitles)
	{
		string titles = string("Time") + '\t'
			+ "BorrowerCardID" + '\t'
			+ "ChineseName" + '\t'
			+ "EnglishName" + '\t'
			+ "ClassName" + '\t'
			+ "ClassNumber" + '\t'
			+ "IpadCardID" + '\t'
			+ "IpadNumber"
			+ '\n';

		fout << titles;
	}

	string blitem = '\"' + borrower["Time"] + "\"\t\""
		+ borrower["BorrowerCardID"] + "\"\t\""
		+ borrower["ChineseName"] + "\"\t\""
		+ borrower["EnglishName"] + "\"\t\""
		+ borrower["ClassName"] + "\"\t\""
		+ borrower["ClassNumber"] + "\"\t\""
		+ borrower["IpadCardID"] + "\"\t\""
		+ borrower["IpadNumber"]
		+ "\"";
	fout << blitem;

	fout << '\n';
	fout.flush();
	fout.close();

	return true;
}

bool Database::write_to_history(BList_Item returner)
{
	bool hastitles = has_titles("rlist");
	Time now = Time::now();
	string current_history_path = m_history_folder + "history_" + std::to_string(now.m_year) + "-" + std::to_string(now.m_month) + "-" + std::to_string(now.m_day) + ".txt";
	std::ofstream fout;

	fout.open(current_history_path, std::ios::app);
	if (fout.fail())
	{
		log_error("open m_history_folder failed: %s", current_history_path.c_str());
		return false;
	}

	if (!hastitles)
	{

		string titles = string("ChiTime") + '\t'
			+ "ChiReturnTime" + '\t'
			+ "BorrowerCardID" + '\t'
			+ "ChineseName" + '\t'
			+ "EnglishName" + '\t'
			+ "ClassName" + '\t'
			+ "ClassNumber" + '\t'
			+ "IpadCardID" + '\t'
			+ "IpadNumber" + '\t'
			+ "Time" + '\t'
			+ "ReturnTime" + '\t'
			+ '\n';

		fout << titles;
	}

	string rlitem = '\"' + returner["ChiTime"] + "\"\t\""
		+ returner["ChiReturnTime"] + "\"\t\""
		+ returner["BorrowerCardID"] + "\"\t\""
		+ returner["ChineseName"] + "\"\t\""
		+ returner["EnglishName"] + "\"\t\""
		+ returner["ClassName"] + "\"\t\""
		+ returner["ClassNumber"] + "\"\t\""
		+ returner["IpadCardID"] + "\"\t\""
		+ returner["IpadNumber"] + "\"\t\""
		+ returner["Time"] + "\"\t\""
		+ returner["ReturnTime"]
		+ "\"";

	fout << rlitem;

	fout << '\n';
	fout.flush();
	fout.close();

	return true;
}

bool Database::has_titles(const string & type)
{
	string line;
	string path = "";

	if ("blist" == type)
		path = m_blist_path;

	if ("rlist" == type)
	{
		Time now = Time::now();
		path = m_history_folder + "history_" + std::to_string(now.m_year) + "-" + std::to_string(now.m_month) + "-" + std::to_string(now.m_day) + ".txt";
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

bool Database::update()
{
	std::ofstream fout;
	fout.open(m_blist_path);

	if (fout.fail())
	{
		log_error("failed to open %s", m_blist_path.c_str());
		return false;
	}

	string titles = string("Time") + '\t'
		+ "BorrowerCardID" + '\t'
		+ "ChineseName" + '\t'
		+ "EnglishName" + '\t'
		+ "ClassName" + '\t'
		+ "ClassNumber" + '\t'
		+ "IpadCardID" + '\t'
		+ "IpadNumber"
		+ '\n';

	fout << titles;

	for (int i = 0; i < m_blist.size(); ++i)
	{
		BList_Item & blitem = m_blist[i];

		string target = '\"' + blitem["Time"] + "\"\t\"" +
			blitem["BorrowerCardID"] + "\"\t\"" +
			blitem["ChineseName"] + "\"\t\"" +
			blitem["EnglishName"] + "\"\t\"" +
			blitem["ClassName"] + "\"\t\"" +
			blitem["ClassNumber"] + "\"\t\"" +
			blitem["IpadCardID"] + "\"\t\"" +
			blitem["IpadNumber"]
			+ "\"";

		fout << target << '\n';
	}
	fout.flush();
	fout.close();

	return true;
}