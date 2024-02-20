#pragma once

#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>

using std::string;

#include "../utilities/Singleton.h"
#include "../utilities/Logger.h"
#include "../utilities/Time.h"

using namespace regulus::utilities;


namespace regulus {
namespace db {

class Database
{
	SINGLETON(Database);
public:
	typedef std::map<string, string> Item;
	typedef std::map<string, string> BList_Item;

	typedef std::vector<Item>::iterator Item_Itr;
	typedef std::vector<BList_Item>::iterator BList_Item_Itr;

	bool load(const string & type, const string & path);
	bool load_items(const string & type, bool is_human);
	bool load_blist();
	bool load_history(int year, int month, int day);
	bool load_history(int year, int month, int day, const string & path);

	string trim(string s);
	string trim(string src, const char * remove_chars);

	Item_Itr find_item(const string & cardId);
	Item_Itr find_item(const string & cardId, const string & type);

	BList_Item_Itr find_blitem(const string & cardId);
	BList_Item_Itr find_borrower(const string & cardId);

	bool write_to_file(BList_Item borrower);
	bool write_to_history(BList_Item returner);

	bool has_titles(const string & type);

	bool update();

	Logger * m_logger;
	std::vector <Item> m_items;
	std::vector <BList_Item> m_blist;
	std::vector <BList_Item> m_rlist;

	string m_filename;
	string m_students_path;
	string m_staffs_path;
	string m_ipads_path;
	string m_blist_path;
	string m_history_folder;
	string m_history_path;

	static const char * b_list_titles[8];
	static const char * ipad_list_titles[2];
	static const char * staff_list_titles[4];
};

}
}


