#include "basic.h"
#include <sstream>
using namespace std;
bool Condition::judge(int content)
{
	stringstream ss;
	ss << value;
	int my_content;
	ss >> my_content;

	switch (op)
	{
	case Condition::OPERATOR_EQUAL:
		return content == my_content;
		break;
	case Condition::OPERATOR_NOT_EQUAL:
		return content != my_content;
		break;
	case Condition::OPERATOR_LESS:
		return content < my_content;
		break;
	case Condition::OPERATOR_MORE:
		return content > my_content;
		break;
	case Condition::OPERATOR_LESS_EQUAL:
		return content <= my_content;
		break;
	case Condition::OPERATOR_MORE_EQUAL:
		return content >= my_content;
		break;
	default:
		return true;
		break;
	}
}

bool Condition::judge(float content)
{
	stringstream ss;
	ss << value;
	float my_content;
	ss >> my_content;

	switch (op)
	{
	case Condition::OPERATOR_EQUAL:
		return content == my_content;
		break;
	case Condition::OPERATOR_NOT_EQUAL:
		return content != my_content;
		break;
	case Condition::OPERATOR_LESS:
		return content < my_content;
		break;
	case Condition::OPERATOR_MORE:
		return content > my_content;
		break;
	case Condition::OPERATOR_LESS_EQUAL:
		return content <= my_content;
		break;
	case Condition::OPERATOR_MORE_EQUAL:
		return content >= my_content;
		break;
	default:
		return true;
		break;
	}
}

bool Condition::judge(string content)
{
	string my_content = value;
	switch (op)
	{
	case Condition::OPERATOR_EQUAL:
		return content == my_content;
		break;
	case Condition::OPERATOR_NOT_EQUAL:
		return content != my_content;
		break;
	case Condition::OPERATOR_LESS:
		return content < my_content;
		break;
	case Condition::OPERATOR_MORE:
		return content > my_content;
		break;
	case Condition::OPERATOR_LESS_EQUAL:
		return content <= my_content;
		break;
	case Condition::OPERATOR_MORE_EQUAL:
		return content >= my_content;
		break;
	default:
		return true;
		break;
	}
}

Condition::Condition(string a, string v, int o, int t) {
	col = a;
	value = v;
	op = o;
	type = t;
}
