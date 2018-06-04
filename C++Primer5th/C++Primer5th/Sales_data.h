#ifndef SALES_DATA_H
#define SALES_DATA_H
#include <iostream>
#include <string>
#include "error.h"

using std::istream;
using std::ostream;

class Sales_data
{
	friend ostream& operator<<(ostream&, const Sales_data&);
	friend istream& operator >>(istream &, Sales_data&);
	friend ostream &print(ostream&, const Sales_data&);			// 一个友元函数print，便于Sales_data类直接访问
	friend istream &read(istream&, Sales_data&);
	friend bool operator==(const Sales_data&, const Sales_data&);
	
public:
	Sales_data() = default;
	Sales_data(const std::string &s) : bookNo(s){}
	Sales_data(istream &is){ read(is, *this); }
public:

	Sales_data  & combine(const Sales_data &);
	Sales_data  & operator +=(const Sales_data &);
	std::string isbn() const { return bookNo; }
private:
	std::string bookNo;
	unsigned units_sold = 0;
	double sellingprice = 0.0;
	double saleprice = 0.0;
	double discount = 0.0;
};

inline bool compareIsbn(const Sales_data &lhs, const Sales_data &rhs)
{
	return lhs.isbn() < rhs.isbn();
}

Sales_data operator +(const Sales_data &lhs, const Sales_data &rhs);

inline bool operator==(const Sales_data &lhs, const Sales_data &rhs)
{
	return lhs.bookNo == rhs.bookNo && lhs.units_sold == rhs.units_sold
		&& lhs.saleprice == rhs.saleprice && lhs.sellingprice ==
		rhs.sellingprice && rhs.discount == lhs.discount;
}

inline bool operator !=(const Sales_data &lhs, const Sales_data &rhs)
{
	return !(lhs == rhs);
}



inline Sales_data& Sales_data::combine(const Sales_data &s)
{
	if (isbn() != s.isbn())
	{
		throw isbn_mismatch("wrong isbns", isbn(), s.isbn());	//logic_error
	}
	units_sold += s.units_sold;
	saleprice = (saleprice * units_sold + s.saleprice * s.units_sold)
		/ (units_sold + s.units_sold);
	if (sellingprice != 0)
		discount = saleprice / sellingprice;
	return *this;
}
Sales_data operator +(const Sales_data &lhs, const Sales_data &rhs)
{
	Sales_data ret(lhs);
	ret.combine(rhs);
	return ret;
}


inline ostream& print(ostream &os, const Sales_data &s)
{
	os << s.bookNo << "\t" << s.units_sold << "\t" << s.sellingprice << "\t"
		<< s.saleprice << "\t" << s.discount << std::endl;
	return os;
}

ostream& operator<<(ostream &os, const Sales_data &s)
{
	os << s.bookNo << "\t" << s.units_sold << "\t" << s.sellingprice << "\t"
		<< s.saleprice << "\t" << s.discount << std::endl;
	return os;
}


inline istream& read(istream &is, Sales_data &s)
{
	is >> s.bookNo >> s.units_sold >> s.sellingprice >> s.saleprice;
	if (s.sellingprice != 0)
		s.discount = s.saleprice / s.sellingprice;
	else
		s = Sales_data();
	return is;

}



inline istream& operator>>(istream &is, Sales_data &s)
{

	is >> s.bookNo >> s.units_sold >> s.sellingprice >> s.saleprice;
	if (s.sellingprice != 0)
		s.discount = s.saleprice / s.sellingprice;
	else
		s = Sales_data();
	return is;
}

#endif // SALES_DATA_H
