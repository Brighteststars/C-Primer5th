#ifndef MESSAGE_H
#define MESSAGE_H
#include <iostream>
#include <string>
#include <set>

class Message
{
	friend void swap(Message &, Message &);
	friend class Folder;
public:
	explicit Message(const std::string &s = "") :contents(s){}
	Message(const Message &);
	Message &operator=(const Message &);
	~Message();
	Message(Message &&);
	Message &operator=(Message &&);

	void save(Folder &);
	void remove(Folder &);
private:
	std::string contents;
	std::set<Folder*> folders;
	void add_to_folders(const Message &);
	void remove_from_folders();
	void addF(Folder *f){ folders.insert(f); }
	void remF(Folder *f){ folders.erase(f); }
};

class Folder
{
	friend void swap(Folder &, Folder &);
	friend class Message;
public:
	void addMsg(Message *m);
	void remMsg(Message *m);
	Folder() = default;
	Folder(const Folder &f) :msgs(f.msgs){
		add_to_message(f);
	}
	Folder &operator=(const Folder &);
	~Folder();
	void show();
private:
	std::set<Message*> msgs;
	void add_to_message(const Folder &);
	void remove_from_message();
};

void Message::save(Folder &f)
{
	folders.insert(&f);
	f.addMsg(this);
}

void Message::remove(Folder &f)
{
	folders.erase(&f);
	f.remMsg(this);
}

void Message::add_to_folders(const Message &m)
{
	for (auto f : m.folders)
		f->addMsg(this);
}

void Message::remove_from_folders()
{
	for (auto f : folders)
		f->remMsg(this);
}

Message::Message(const Message &m) :contents(m.contents), folders(m.folders)
{
	add_to_folders(m);
}

Message &Message::operator=(const Message &m)
{
	remove_from_folders();
	contents = m.contents;
	folders = m.folders;
	add_to_folders(m);
	return *this;
}

Message::Message(Message &&m)
{
	folders = std::move(m.folders);			//调用set右值拷贝赋值构造函数,拷贝指针
	for (auto f : folders)
	{
		f->remMsg(&m);
		f->addMsg(this);
	}
	m.folders.clear();			//将set置空,防止destroy指针拷贝
}

Message &Message::operator=(Message &&m)
{
	if (this == &m)
		return *this;
	remove_from_folders();			//把自身从folders中删除
	folders = std::move(m.folders);
	for (auto f : folders)
	{
		f->remMsg(&m);
		f->addMsg(this);
	}
	m.folders.clear();
}

Message::~Message()
{
	remove_from_folders();
}

void swap(Message &lhs, Message &rhs)
{
	using std::swap;
	if (&lhs == &rhs)
		return;
	for (auto f : lhs.folders)
		f->remMsg(&lhs);
	for (auto f : rhs.folders)
		f->remMsg(&rhs);
	swap(lhs.contents, rhs.contents);
	swap(lhs.folders, rhs.folders);
	for (auto f : lhs.folders)
		f->addMsg(&lhs);
	for (auto f : rhs.folders)
		f->addMsg(&rhs);
}

void Folder::addMsg(Message *m)
{
	msgs.insert(m);
}

void Folder::remMsg(Message *m)
{
	msgs.erase(m);
}

void Folder::add_to_message(const Folder &f)
{
	for (auto m : f.msgs)
		m->addF(this);
}

void Folder::remove_from_message()
{
	for (auto m : msgs)
		m->remF(this);
}



Folder &Folder::operator=(const Folder &f)
{
	remove_from_message();
	msgs = f.msgs;
	add_to_message(f);
	return *this;
}

Folder::~Folder()
{
	remove_from_message();
}

void Folder::show()
{
	for (auto m : msgs)
		std::cout << m->contents << "\t";
	std::cout << std::endl;
}

void swap(Folder &lhs, Folder &rhs)
{
	using std::swap;
	if (&lhs == &rhs)
		return;
	for (auto m : lhs.msgs)
		m->remove(lhs);
	for (auto m : rhs.msgs)
		m->remove(rhs);
	swap(lhs.msgs, rhs.msgs);
	for (auto m : lhs.msgs)
		m->save(lhs);
	for (auto m : rhs.msgs)
		m->save(rhs);
}

#endif