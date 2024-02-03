#pragma once

#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>

using std::string;

#include "../utilities/Singleton.h"
#include "../utilities/Logger.h"

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

	bool in_open(const string & filename);
	void in_close();

	void set_path(const string & type, const string & path);
	bool load(const string & type);
	void load_items(const string & type, bool is_human);
	void load_blist();

	string trim(string s);
	string trim_quote(string s);

	Item_Itr find_item(const string & cardId);
	Item_Itr find_item(const string & cardId, const string & type);

	BList_Item_Itr find_blitem(const string & cardId);
	BList_Item_Itr find_borrower(const string & cardId);

	Logger * m_logger;
	std::vector <Item> m_items;
	std::vector <BList_Item> m_blist;
	std::vector <BList_Item> m_rlist;
private:
	string m_filename;
	std::ifstream m_fin;
	std::ofstream m_fout;

	string m_students_path;
	string m_staffs_path;
	string m_ipads_path;
	string m_blist_path;
};

}
}


