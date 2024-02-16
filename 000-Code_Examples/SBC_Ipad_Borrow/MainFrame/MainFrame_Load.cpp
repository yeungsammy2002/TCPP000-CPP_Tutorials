#include "../MainFrame.h"

void MainFrame::init()
{
	m_logger = Singleton<Logger>::instance();
	m_logger->open("C:\\Users\\Public\\Documents\\ipad_borrow_data\\log\\");
	m_logger->max(20480);
	m_logger->level(Logger::L_INFO);

	m_db = Singleton<Database>::instance();
	m_db->m_logger = Singleton<Logger>::instance();
	m_db->load("student", "C:\\Users\\Public\\Documents\\ipad_borrow_data\\eclass-user.csv");
	m_db->load("staff", "C:\\Users\\Public\\Documents\\ipad_borrow_data\\staff_list.txt");
	m_db->load("ipad", "C:\\Users\\Public\\Documents\\ipad_borrow_data\\ipad_list.txt");
	m_db->load("blist", "C:\\Users\\Public\\Documents\\ipad_borrow_data\\borrowed_list.txt");

	Time now = Time::now();
	m_history_year_p2 = now.m_year;
	m_history_month_p2 = now.m_month;
	m_history_day_p2 = now.m_day;
	m_history_unixtime_p2 = now.m_unixtime;
	m_db->load_history(m_history_year_p2, m_history_month_p2, m_history_day_p2, "C:\\Users\\Public\\Documents\\ipad_borrow_data\\history\\");
}