#ifndef SCREEN_H
#define SCREEN_H
#include <iostream>
#include <string>

template<int H,int W>			//width height column¡– row––
class Screen
{
public:
	Screen() :contents(W*H, ' '){}
	Screen(char ch) :contents(W*H, ch){}

	char get()const { return contents[cursor]; }
	inline char get(int , int )const;
	Screen &clear(char c = bkground);
private:
	static const char bkground = ' ';
public:
	Screen &move(int r, int c);
	Screen &set(char c);
	Screen &set(int r, int c, char ch);
	Screen &display(std::ostream &os) { do_display(os); return *this; }
	const Screen &display(std::ostream &os)const { do_display(os); return *this; }
private:
	int cursor = 0;
	std::string contents;
	void do_display(std::ostream &os) { os << contents; }
};

template<int H,int W>
Screen<H,W> &Screen<H,W>::clear(char c)
{
	contents = std::string(W*H, c);
	return *this;
}

template<int H,int W>
Screen<H,W> &Screen<H,W>::move(int r, int c)
{
	cursor = r*W + c;
	return *this;
}

template<int H,int W>
char Screen<H,W>::get(int r, int c)const
{
	int row = r * W;
	return  contents[row + c];

}

template<int H,int W>
Screen<H,W> &Screen<H,W>::set(char c)
{
	contents[cursor] = c;
	return *this;
}

template<int H,int W>
Screen<H,W> &Screen<H,W>::set(int r, int c, char ch)
{
	contents[r*W + c] = ch;
	return *this;
}

#endif