#ifndef QUOTE_H
#define QUOTE_H
#include <iostream>
#include <string>
#include <memory>
#include <set>

class Quote
{
public:								//公有的 运行所有对象访问
	Quote() = default;
	Quote(const std::string &book, double sales_price)
		:bookNo(book), price(sales_price){}
	Quote(const Quote &q) :bookNo(q.bookNo), price(q.price){}
	Quote(Quote &&q) :bookNo(std::move(q.bookNo)), price(q.price){}
	Quote &operator=(const Quote &q)
	{
		bookNo = q.bookNo;
		price = q.price;
		
		return *this;
	}
	Quote &operator=(Quote &&q)
	{
		bookNo = std::move(q.bookNo);
		price = q.price;
		
		return *this;
	}
	virtual double net_price(std::size_t n)const { return n*price; }
	virtual void debug()const
	{
		std::cout << bookNo << " " << price << std::endl;
	}
	virtual Quote *clone()
	{
		return new Quote(*this);
	}
	virtual Quote *clone()
	{
		return new Quote(std::move(*this));
	}
	virtual ~Quote(){ }
	std::string isbn()const{ return bookNo; }
private:							//私有的 只允许对象成员访问
	std::string bookNo;				
protected:							//保护的 允许直接派生对象访问
	double price;
};

class Disc_quote:public Quote
{
public:
	Disc_quote() = default;
	Disc_quote(const std::string &book, double p, std::size_t qty, double disc)
		:Quote(book, p), quantity(qty), discount(disc){
		
	}
	double net_price(std::size_t n)const = 0;		//纯虚函数
protected:											//派生类需要访问,所以保护
	std::size_t quantity = 0;
	double discount = 0.0;
};

class Bulk_quote :public Disc_quote
{
public:
	Bulk_quote() = default;
	Bulk_quote(const std::string &book, double p, std::size_t qty, double disc)
		:Disc_quote(book, p, qty, disc){}
		
	Bulk_quote(const Bulk_quote &b) :Disc_quote(b){ }
	Bulk_quote(Bulk_quote &&b) :Disc_quote(std::move(b)){  }
	Bulk_quote &operator=(const Bulk_quote &b)
	{
		Disc_quote::operator=(b);
		
		return *this;
	}
	Bulk_quote &operator=(Bulk_quote &&b)
	{
		Disc_quote::operator=(b);
		
		return *this;
	}
	~Bulk_quote(){  }
	double net_price(std::size_t n)const override
	{
		if (n >= quantity)
			return n*discount*price;
		else
			return n*price;
	}
};

class Activity_quote :public Disc_quote
{
public:
	Activity_quote() = default;
	Activity_quote(const std::string &book, double p, std::size_t min_qty, std::size_t max_qty,double disc)
		:Disc_quote(book, p, min_qty, disc), max(max_qty){}
	double net_price(std::size_t n)const override
	{
		if (n >= quantity)
		{
			if (n > max)
				return (n - max)*price + max*discount*price;
			else
				return n*price*discount;
		}
	}
private:
	std::size_t max = 0;
};

double print_total(std::ostream &os, const Quote &item, std::size_t n)
{
	double ret = item.net_price(n);
	os << "ISBN: " << item.isbn() << " # total: " << n << " total due: "
		<< ret << std::endl;
	return ret;
}

class Basket
{
public:
	void add_item(const std::shared_ptr<Quote> &sale)
	{
		items.insert(sale);
	}
	void add_item(const Quote &q)
	{
		items.insert( std::shared_ptr<Quote>(q.clone()));
	}
	void add_item(Quote &&sale)
	{
		items.insert(std::shared_ptr<Quote>(std::move(sale).clone()));
	}
	double total_receipt(std::ostream &)const;
private:
	static bool compare(const std::shared_ptr<Quote> &lhs, const std::shared_ptr<Quote> &rhs)
	{
		return lhs->isbn() < rhs->isbn();
	}

	std::multiset<std::shared_ptr<Quote>, decltype(compare)*> items{ compare };
};

double Basket::total_receipt(std::ostream &os)const
{
	double sum = 0.0;
	for (auto iter = items.cbegin(); iter != items.cend(); iter = items.upper_bound(*iter))
	{
		sum += print_total(os, **iter, items.count(*iter));
	}
	os << "Total Sale: " << sum << std::endl;
	return sum;
}

#endif