#include "../MainFrame.h"

void MainFrame::init()
{
	m_logger = Singleton<Logger>::instance();
	m_logger->open("C:\\Users\\Public\\Documents\\ipad_borrow_data\\log.txt");
	//m_logger->level(Logger::L_ERROR);

	m_db = Singleton<Database>::instance();
	m_db->m_logger = Singleton<Logger>::instance();
	m_db->load("student", "C:\\Users\\Public\\Documents\\ipad_borrow_data\\eclass-user.csv");
	m_db->load("ipad", "C:\\Users\\Public\\Documents\\ipad_borrow_data\\ipad_list.txt");
	m_db->load("blist", "C:\\Users\\Public\\Documents\\ipad_borrow_data\\borrowed_list.txt");

	Time now = Time::now();
	m_db->load_history(now.m_year, now.m_month, now.m_day, "C:\\Users\\Public\\Documents\\ipad_borrow_data\\history\\");
}