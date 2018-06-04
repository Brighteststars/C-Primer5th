#ifndef QUERY_H
#define QUERY_H
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <set>
#include <vector>
#include <map>
#include <memory>
#include "make_plural.h"


class QueryResult;
using line_no = std::vector<std::string>::size_type;
class TextQuery
{
public:
	
	TextQuery(std::ifstream &);
	QueryResult query(const std::string &)const;
private:
	std::shared_ptr<std::vector<std::string>> file;											//�����ļ�����,���з�
	std::map<std::string, std::shared_ptr<std::set<line_no>>> wm;			//ÿ�����ʶ�Ӧ���к�
};

class QueryResult
{
	friend std::ostream &print(std::ostream &, const QueryResult &);
public:
	
	QueryResult(std::string s, std::shared_ptr<std::set<line_no>> p,
		std::shared_ptr<std::vector<std::string>> f) : sought(s), lines(p), file(f){}

	std::set<line_no>::iterator begin()const{ return lines->begin(); }
	std::set<line_no>::iterator end()const{ return lines->end(); }
	std::shared_ptr<std::vector<std::string>> get_file()const{ return file; }
private:
	std::string sought;
	std::shared_ptr<std::set<line_no>> lines;
	std::shared_ptr<std::vector<std::string>> file;

};

TextQuery::TextQuery(std::ifstream &is) :file(new std::vector<std::string>)
{
	std::string text;
	while (getline(is, text))
	{
		file->push_back(text);
		int n = file->size() - 1;
		std::istringstream line(text);

		std::string word;

		while (line >> word)
		{
			auto &lines = wm[word];								//���ص�ǰquery_word��Ӧshared_ptr<set<line_no>>
			if (!lines)											//��һ�β���word,lines = nullpter
				lines.reset(new std::set<line_no>);
			lines->insert(n);
		}
	}
}

QueryResult TextQuery::query(const std::string &s)const
{
	static std::shared_ptr<std::set<line_no>> nodata(new std::set<line_no>);			//��������һ������,��̬����ֻ�趨��һ��,�ڳ��������ڶ�����
	auto loc = wm.find(s);										//���s����,����wm�е�����


	if (loc == wm.end())
		return QueryResult(s, nodata, file);
	else
		return QueryResult(s, loc->second, file);
}

std::ostream &print(std::ostream &os, const QueryResult &qr)
{
	os << qr.sought << " occurs " << qr.lines->size() << " "
		<< make_plural(qr.lines->size(), "time", "s") << std::endl;
	for (auto x : *qr.lines)
		os << "\t(line " << x + 1 << ") "
		<< *(qr.file->begin()+x) << std::endl;
	return os;

}

#endif