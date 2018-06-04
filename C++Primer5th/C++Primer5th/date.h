#ifndef DATE
#define DATE
#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

class Date
{
public:
	friend ostream& operator<<(ostream&, const Date&);
	Date() = default;
	Date(string &ds);

	unsigned y() const { return year; }
	unsigned m() const { return month; }
	unsigned d() const { return day; }
	bool operator==(const Date &);
	bool operator!=(const Date &);
private:
	unsigned year, month, day;
};

const string month_name[] = { "January", "February", "March",
"April", "May", "June", "July", "August", "September",
"October", "November", "December" };

const string month_abbr[] = { "Jan", "Feb", "Mar", "Apr", "May",
"Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec" };

const int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

inline int get_month(string &ds, int &end_of_month)
{
	int i, j;
	for (i = 0; i < 12; ++i)
	{
		for (j = 0; j < month_abbr[i][j]; ++j)
		{
			if (ds[j] != month_abbr[i][j])
				break;
		}
		if (j == month_abbr[i].size())
			break;
	}

	if (i == 12)
		throw invalid_argument("非法月份名");
	if (ds[j] == ' ')
	{
		end_of_month = j + 1;
		return i + 1;
	}

	for (; j < month_name[i][j]; ++j)
	{
		if (ds[j] != month_name[i][j])
			break;
	}

	if (ds[j] == ' ' && j == month_name[i].size())
	{
		end_of_month = j + 1;
		return i + 1;
	}

	throw invalid_argument("非法月份名");
}

inline int get_day(string &ds, int month, int &p)
{
	size_t q;
	int day = stoi(ds.substr(p), &q);
	if (day < 1 || day > days[month-1])
	{
		throw invalid_argument("非法日期值");
	}
	p += q;
	return day;
}

inline int get_year(string &ds, int &p)
{
	size_t q;
	int year = stoi(ds.substr(p), &q);
	if (p + q < ds.size())
		throw invalid_argument("非法结尾内容");
	return year;
}

Date::Date(string &ds)
{
	size_t q;
	int p;
	if ((p = ds.find_first_of("0123456789")) == string::npos)
		throw invalid_argument("没有数字，非法日期");
	if (p > 0)			//月份名格式
	{
		month = get_month(ds, p);
		day = get_day(ds, month, p);
		if (ds[p] != ' ' && ds[p] != ',')
			throw invalid_argument("非法间隔符");
		++p;
		year = get_year(ds, p);
	}
	else              //月份值格式
	{
		month = stoi(ds, &q);
		p = q;
		if (month < 1 || month > 12)
			throw invalid_argument("非法月份值");
		if (ds[p++] != '/')
			throw invalid_argument("非法间隔符");
		day = get_day(ds, month, p);
		if (ds[p++] != '/')
			throw invalid_argument("非法间隔符");
		year = get_year(ds, p);
	}
}

ostream& operator<<(ostream &os, const Date &ds)
{
	os << ds.y() << "年" << ds.m() << "月" << ds.d() << "日" << endl;
	return os;
}

bool Date::operator==(const Date &d)
{
	return year == d.year && month == d.month && day == d.day;
}

bool Date::operator!=(const Date &d)
{
	return !(*this == d);
}
#endif