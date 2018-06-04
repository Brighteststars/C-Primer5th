#ifndef SALES_H
#define SALES_H
#include <iostream>  
#include <string>
#include "error.h"

class Sales_data
{
	friend Sales_data operator+(const Sales_data& lhs, const Sales_data& rhs);
	friend std::ostream &operator << (std::ostream& os, const Sales_data& s);
	friend std::istream &operator >> (std::istream& is, Sales_data& s);
	friend Sales_data add(const Sales_data&, const Sales_data&);
	friend std::ostream &print(std::ostream&, const Sales_data&);
	friend std::istream &read(std::istream&, Sales_data&);
public:
	Sales_data() = default;
	Sales_data(const std::string &s) : bookNo(s) { }
	Sales_data(const std::string &s, unsigned n, double p) :bookNo(s), units_sold(n), revenue(p*n){}
	Sales_data(const Sales_data &s) :bookNo(s.bookNo), units_sold(s.units_sold), revenue(s.revenue){}
	Sales_data(Sales_data&& s) :bookNo(s.bookNo), units_sold(s.units_sold), revenue(s.revenue){}
	~Sales_data() {}
	Sales_data(std::istream &);
	std::string isbn() const { return bookNo; }
	Sales_data& combine(const Sales_data&);
	Sales_data& operator  =(const Sales_data& rhs);
	Sales_data& operator  =(const std::string&  rhs);
	Sales_data& operator +=(const Sales_data& rhs);
	explicit operator std::string() const { return bookNo; }
	explicit operator double() const { return revenue; }
	double avg_price() const;
private:
	std::string bookNo;
	unsigned units_sold = 0;
	double revenue = 0.0;
};
inline Sales_data operator+(const Sales_data& lhs, const Sales_data& rhs)
{
	if (lhs.isbn() != rhs.isbn())
	{
		throw isbn_mismatch("wrong isbns", lhs.isbn(), rhs.isbn());
	}
	Sales_data sum = lhs;
	sum += rhs;

	return sum;
}



 
using std::istream; using std::ostream;
Sales_data::Sales_data(std::istream &is)
{
	read(is, *this);
}
double Sales_data::avg_price() const {
	if (units_sold)
		return revenue / units_sold;
	else
		return 0;
}
Sales_data &Sales_data::combine(const Sales_data &rhs)
{
	units_sold += rhs.units_sold;
	revenue += rhs.revenue;
	return *this;
}
Sales_data &Sales_data::operator =(const Sales_data &rhs)
{
	this->bookNo = rhs.bookNo;
	this->revenue = rhs.revenue;
	this->units_sold = rhs.units_sold;
	return *this;
}
Sales_data &Sales_data::operator =(const std::string &rhs)
{
	*this = Sales_data(rhs);
	return *this;
}
Sales_data &Sales_data::operator +=(const Sales_data &rhs)
{
	this->revenue += rhs.revenue;
	this->units_sold += rhs.units_sold;
	return *this;
}
Sales_data add(const Sales_data &lhs, const Sales_data &rhs)
{
	Sales_data sum = lhs;
	sum.combine(rhs);
	return sum;
}
istream &read(istream &is, Sales_data &item)
{
	double price = 0;
	is >> item.bookNo >> item.units_sold >> price;
	item.revenue = price * item.units_sold;
	return is;
}
ostream &print(ostream &os, const Sales_data &item)
{
	os << item.isbn() << " " << item.units_sold << " " << item.revenue << " " << item.avg_price();
	return os;
}
std::ostream &operator <<(std::ostream &os, const Sales_data &item)
{
	os << item.isbn() << " " << item.units_sold << " " << item.revenue << " " << item.avg_price();
	return os;
}
std::istream &operator >>(std::istream &is, Sales_data &s)
{
	double price;
	is >> s.bookNo >> s.units_sold >> price;
	if (is)
		s.revenue = s.units_sold * price;
	else
		s = Sales_data();

	return is;
}

#endif