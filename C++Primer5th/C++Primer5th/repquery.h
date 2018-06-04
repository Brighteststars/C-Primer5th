#ifndef REPQUERY_H
#define REPQUERY_H
#include "query.h"
#include <algorithm>	//set_intersection
#include <iterator>		//inserter

class Query_base
{
	friend class Query;
protected:
	using line_no = std::vector<std::string>::size_type;
	virtual ~Query_base() = default;
private:
	virtual std::string rep()const = 0;
	virtual QueryResult eval(const TextQuery &)const = 0;
};

class Query
{
	friend Query operator~(const Query &);
	friend Query operator|(const Query &, const Query &);
	friend Query operator&(const Query &, const Query &);
public:
	Query(const std::string &);
	std::string rep()const { return q->rep(); }
	QueryResult eval(const TextQuery &t)const { return q->eval(t); }
private:
	Query(const std::shared_ptr<Query_base> &query) :q(query){}
	std::shared_ptr<Query_base> q;
};

class WordQuery :public Query_base
{
	friend class Query;
	WordQuery(const std::string &s) :query_word(s){}
	std::string rep()const { return query_word; }
	QueryResult eval(const TextQuery &t)const { return t.query(query_word); }
	std::string query_word;
};

inline Query::Query(const std::string &s) :q(new WordQuery(s)){}

class NotQuery :public Query_base
{
	friend Query operator~(const Query &);
	NotQuery(const Query &q) :query(q){}
	std::string rep()const { return "(~" + query.rep() + ")"; }
	QueryResult eval(const TextQuery &)const;
	Query query;
};

inline Query operator~(const Query &operand)			//operand²Ù×÷Êý
{
	return std::shared_ptr<Query_base>(new NotQuery(operand));
}

class BinaryQuery :public Query_base
{
protected:
	BinaryQuery(const Query &l, const Query &r,const std::string &s)
	: lhs(l), rhs(r), opSym(s){}
	std::string rep()const { return "(" + lhs.rep() + " " + opSym + " " + rhs.rep() + ")"; }
	Query lhs, rhs;
	std::string opSym;
};

class AndQuery :public BinaryQuery
{
	friend Query operator&(const Query &, const Query &);
	AndQuery(const Query &lhs, const Query &rhs) : BinaryQuery(lhs, rhs, "&"){}
	QueryResult eval(const TextQuery &)const;
};

inline Query operator&(const Query &lhs, const Query &rhs)
{
	return std::shared_ptr<Query_base>(new AndQuery(lhs, rhs));
}

class OrQuery : public BinaryQuery
{
	friend Query operator|(const Query &, const Query &);
	OrQuery(const Query &l, const Query &r) : BinaryQuery(l, r, "|"){}
	QueryResult eval(const TextQuery &)const;
};

inline Query operator|(const Query &l, const Query &r)
{
	return std::shared_ptr<Query_base>(new OrQuery(l, r));
}

QueryResult OrQuery::eval(const TextQuery &t) const
{
	auto left = lhs.eval(t), right = rhs.eval(t);
	auto ret_lines = std::make_shared<std::set<line_no>>(left.begin(), left.end());
	ret_lines->insert(right.begin(), right.end());
	return QueryResult(rep(), ret_lines, left.get_file());
}

QueryResult AndQuery::eval(const TextQuery &t)const
{
	auto left = lhs.eval(t), right = rhs.eval(t);
	auto ret_lines = std::make_shared<std::set<line_no>>();
	std::set_intersection(left.begin(), left.end(), right.begin(), right.end(),
		std::inserter(*ret_lines, ret_lines->begin()));
	return QueryResult(rep(), ret_lines, right.get_file());
}

QueryResult NotQuery::eval(const TextQuery &t)const
{
	auto result = query.eval(t);
	auto ret_lines = std::make_shared < std::set<line_no>>();
	auto beg = result.begin(), end = result.end();
	auto sz = result.get_file()->size();
	for (size_t n = 0; n != sz; ++n)
	{
		if (beg == end || *beg != n)
			ret_lines->insert(n);
		else if (beg != end)
			++beg;
	}
	return QueryResult(rep(), ret_lines, result.get_file());
}

#endif











