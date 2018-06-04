#include "query.h"
#include <algorithm>

TextQuery::TextQuery(std::ifstream &is) :file(new std::vector<std::string>)
{
	std::string text;
	while (getline(is, text))		//对文件中每一行
	{
		file->push_back(text);		//保存此行文本
		int n = file->size() - 1;	//当前行号
		std::istringstream line(text); //将文本分解为单词

		std::string word;

		while (line >> word) //对行中每一个单词
		{
			auto &lines = wm[word];				//lines是一个shared_ptr				
			if (!lines)				//在我们第一次遇见这个单词时,此指针为空						
				lines.reset(new std::set<line_no>); //分配一个新的set
			lines->insert(n);			//将次行号插入set中
		}
	}
}

QueryResult TextQuery::query(const std::string &s) const
{
	//指向set容器的智能指针nodata,set容器为空
	static std::shared_ptr<std::set<line_no>> nodata(new std::set<line_no>); //static局部静态对象,只需定义一次,生命周期和全局变量一样
	auto loc = wm.find(s); //map迭代器
	if (loc == wm.end())
		return QueryResult(s, file, nodata); //未找到
	else
		return QueryResult(s, file, loc->second);
}


//字符串复数
std::string make_plural(size_t sz, const std::string &word, const std::string &ending)
{
	return sz > 1 ? word + ending : word;
}

std::ostream &print(std::ostream &os, const QueryResult &qr)
{
	os << qr.sought << " occurs " << qr.line->size()
		<< " " << make_plural(qr.line->size(), "time", "s") << std::endl;
	for (auto num : *qr.line)
		os << "\t(line " << num + 1 << ")\t" << *(qr.file->begin() + num) << std::endl;
	return os;
}



QueryResult OrQuery::eval(const TextQuery &t) const
{
	auto right = rhs.eval(t), left = lhs.eval(t);
	auto ret_lines =
		std::make_shared<std::set<line_no>>(left.begin(), left.end());
	ret_lines->insert(left.begin(), left.end());
	return QueryResult(rep(), left.get_file(),ret_lines);
}

QueryResult AndQuery::eval(const TextQuery &t) const
{
	auto right = rhs.eval(t), left = lhs.eval(t);
	auto ret_lines = std::make_shared<std::set<line_no>>();
	std::set_intersection(left.begin(), left.end(), right.begin(), right.end(),
		std::inserter(*ret_lines, ret_lines->begin()));
	return QueryResult(rep(), left.get_file(), ret_lines);

}

QueryResult NotQuery::eval(const TextQuery &t) const
{
	auto result = query.eval(t);
	auto ret_lines = std::make_shared<std::set<line_no>>();
	auto beg = result.begin(), end = result.end();
	auto sz = result.get_file()->size();
	for (size_t n = 0; n != sz; ++n)
	{
		if (beg == end || *beg != n)
			ret_lines->insert(n);
		else if (beg != end)
			++beg;
	}
	return QueryResult(rep(), result.get_file(), ret_lines);
}
