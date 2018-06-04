#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <string>

class NPC
{
public:
	NPC() = default;
	NPC(const std::string &_name, unsigned _level, unsigned _hp, unsigned _atk)
		:name(_name), level(_level), hp(_hp), attack(_atk){}
	virtual void hurt(unsigned n) = 0;
	virtual void Attack(NPC &n) = 0;
	virtual void show()const
	{
		std::cout << name << "\nlevle:\t" << level << "\nhp:\t" << hp
			<< "\nattack:\t" << attack << std::endl;
	}
private:
	std::string name;
	unsigned level = 0;
	unsigned hp = 0;
protected:
	virtual void ChangeHp(unsigned n)
	{
		if (n <= hp)
			hp -= n;
		else
			hp = 0;
		std::cout << name << "受到了 " << n << " 伤害" << "\n\n";
	}
	bool IsDeath()const
	{
		if (hp == 0)
			return true;
		else
			return false;
	}
	unsigned attack = 0;
	std::string get_name()const { return name; }
};

class monster :public NPC
{
public:
	monster() = default;
	monster(const std::string &_name, unsigned _level, unsigned _hp, unsigned _atk, unsigned _def)
		:NPC(_name, _level, _hp, _atk), defence(_def){}
	void hurt(unsigned n)
	{
		if (IsDeath())
		{
			std::cout << "目标已死亡" << std::endl;
			return;
		}
			
		ChangeHp(n - defence / 5);
	}
	
	void Attack(NPC &n)
	{
		std::cout << get_name() << "发起进攻" << std::endl;
		n.hurt(attack);
	}
	void show()const
	{
		NPC::show();
		std::cout << "defence:\t" << defence << "\n\n";
	}
private:
	unsigned defence = 0;					//每5点防御减少1点伤害
};

class player :public NPC
{
public:
	player() = default;
	player(const std::string &_name, unsigned _level, unsigned _hp, unsigned _atk, unsigned _def, unsigned _ski)
		:NPC(_name, _level, _hp, _atk), defence(_def), skill(_ski){}
	void hurt(unsigned n)
	{
		if (IsDeath())
		{
			std::cout << "目标已死亡" << std::endl;
			return;
		}
		ChangeHp(n - defence / 5 - skill / 2);
	}
	void Attack(NPC &n)
	{
		std::cout << get_name() << "发动进攻" << std::endl;
		n.hurt(attack + skill *1);
	}
	void show()const
	{
		NPC::show();
		std::cout << "defence:\t" << defence << "\nskill:\t" << skill << "\n\n";
	}
private:
	unsigned defence = 0;
	unsigned skill = 0;					//每1点skill增加1攻击和0.5护甲
};

#endif