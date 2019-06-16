#include<Interpreter.h>
#include<string>
bool Interpreter::Interpreter_cmd()
{
    int cho1 = Get_First();
    bool errorcode;
    if(6==cho1)
    {
        errorcode = Syntax_EF();// cmd = filename; or filename ;~~
        if(errorcode == true)
            errorcode = Interpreter_file();
        return errorcode;
    }
    int cho2 = Get_Second();
    int cho = cho1*10+cho2;
    int id;
    switch(cho)
    {
        case 11: // create table
            res.id = 0;
            Create_Table &tmp11 = res.create_table;
            errorcode = Syntax_CT();
            // Then assign to parameters
            break;
        case 12: // create index
            res.id = 2;
            Create_Index &tmp12 = res.create_index;
            errorcode = Syntax_CI();
            break;
        case 21: // drop table
            res.id = 1;
            Drop_Table &tmp21 = res.drop_table;
            errorcode = Syntax_DT();
            break;
        case 22: // drop index
            res.id = 3;
            Drop_Index &tmp22 = res.drop_index;
            errorcode = Syntax_DI();
        case 33: // delete from
            res.id = 6;
            Delete &tmp33 = res.deletee;
            errorcode = Syntax_DF();
            break;
        case 44: // insert into
            res.id = 4;
            Insert &tmp44 = res.insert;
            errorcode = Syntax_II();
            break;
        case 55: // select *
            res.id = 5;
            Select &tmp55 = res.select;
            errorcode = Syntax_SA();
            break;
        default:
            errorcode = false;
            // Syntax Error
    }
    return errorcode;
}
int Interpreter::Get_Second()
{
    int fir_space = cmd.find(' ',0);
    string sub = cmd.substr(0,fir_space);
    int len = sub.length();
    cmd.erase(0,len);
    Clear_frontspace();
    if(!sub.empty() && 0 == sub.compare("table"))
        return 1;
    else if(!sub.empty() && 0 == sub.compare("index"))
        return 2;
    else if(!sub.empty() && 0 == sub.compare("from"))
        return 3;
    else if(!sub.empty() && 0 == sub.compare("into"))
        return 4;
    else if(!sub.empty() && 0 == sub.compare("*"))
        return 5;
    else
        return -1;
}
int Interpreter::Get_First()
{
    int fir_space = cmd.find(' ',0);
    string sub = cmd.substr(0,fir_space);
    int len = sub.length();
    cmd.erase(0,len);
    Clear_frontspace();
    if(!sub.empty() &&0==sub.compare("create"))
    {
        return 1;
    }
    else if(!sub.empty() &&0==sub.compare("drop"))
    {
        return 2;
    }
    else if(!sub.empty() &&0==sub.compare("delete"))
    {
        return 3;
    }
    else if(!sub.empty() &&0==sub.compare("insert"))
    {
        return 4;
    }
    else if(!sub.empty() &&0==sub.compare("select"))
    {
        return 5;
    }
    else if(!sub.empty() && 0 == sub.compare("execfile"))
    {
        return 6;
    }
    else
    {
        return -1
    }
}
void Interpreter::set_cmd(string tmp)
{
    cmd = tmp;
    Clear_frontspace();
}
void Interpreter::Clear_frontspace()
{
    
    while(!cmd.empty() &&( cmd[0]==0x20 || cmd[0] == 0x9 || cmd[0]==0x0D ||cmd[0]==0x0A ))
        cmd.erase(0,1);
}
string Interpreter::Get_Next_CT()
{
	char first = cmd[0];
	if (';' == first)
	{
		return cmd;
	}
	else if ('(' == first)
	{
		cmd.erase(0, 1);
		Clear_frontspace();
		return string("(");
	}
	else if (')' == first)
	{
		cmd.erase(0, 1);
		Clear_frontspace();
		return string(")");
	}
	else if (',' == first)
	{
		cmd.erase(0, 1);
		Clear_frontspace();
		return string(",");
	}
	else  // other word
	{
		int fir_space = min(cmd.find(' ', 0), cmd.find('(', 0), cmd.find(')', 0), cmd.find(',', 0));
		fir_space = min(fir_space, cmd.find('\n', 0));
		string sub = cmd.substr(0, fir_space);
		int len = sub.length();
		cmd.erase(0, len);
		Clear_frontspace();
		return sub;
	}
}
bool Interpreter::Syntax_CT()
{
	string tmp;
	tmp = Get_Next_CT();
	res.create_table.table_name = tmp;
	Attribute *tmpa;
	tmp = Get_Next_CT();
	if (tmp.compare("(") != 0)
		return false;
	res.create_table.num_attr = 0;
	while (true)
	{
		/*-------Name--------*/
		tmp = Get_Next_CT();
		if (tmp.compare("primary") == 0)
			break;
		tmpa = new Attribute();
		tmpa->AttributeName = tmp;
		/*-------Type--------*/
		tmp = Get_Next_CT();
		if (!tmp.empty() && tmp.compare("int") == 0)
			tmpa->AttributeType = -1;
		else if (!tmp.empty() && tmp.compare("float") == 0)
			tmpa->AttributeType = 0;
		else if (!tmp.empty() && tmp.compare("char") == 0)
		{
			tmp = Get_Next_CT();
			if (tmp.compare("(") != 0)
				return false;
			tmp = Get_Next_CT();
			tmpa->AttributeType = std::stoi(tmp);
			tmp = Get_Next_CT();
			if (tmp.compare(")") != 0)
				return false;
		}
		/*-------Unique--------*/
		res.create_table.num_attr++;
		tmp = Get_Next_CT();
		if (tmp.compare(")") == 0)
		{
			res.create_table.attr.push_back(*tmpa);
			return true;
		}
		else if (tmp.compare("unique") == 0)
		{
			tmpa->Unique = true;
			res.create_table.attr.push_back(*tmpa);
			tmp = Get_Next_CT();
			if (tmp.compare(")") == 0)
				return true;
			else if (tmp.compare(",") ==0 )
				;
		}
		else if (tmp.compare(",") == 0)
		{
			res.create_table.attr.push_back(*tmpa);
		}
		else
			return false;
	}
	// deal with primary key 
	tmp = Get_Next_CT();
	if (tmp.compare("key") != 0)
		return false;
	else
	{
		tmp = Get_Next_CT();
		if (tmp.compare("(") != 0)
			return false;
		tmp = Get_Next_CT();
		string ntmp = Get_Next_CT();
		if (ntmp.compare(")") != 0)
			return false;
		ntmp = Get_Next_CT();
		if (ntmp.compare(")") != 0)
			return false;
		for (vector<Attribute>::iterator pa = res.create_table.attr.begin(); pa != res.create_table.attr.end(); pa++)
		{
			if (pa->AttributeName.compare(tmp) == 0)
			{
				pa->Unique = true;
				pa->primarykey = true;
				pa->withindex = true;
				break;
			}
		}
	}
	return true;

}
string Interpreter::Get_Next_CI()
{
    char first = cmd[0];
    if(';' == first)
        return cmd;
    else if('(' == first)
    {
        cmd.erase(0,1);
        Clear_frontspace();
        return string("(");
    }
    else if(')'==first)
    {
        cmd.erase(0,1);
        Clear_frontspace();
        return string(")");
    }
    else
    {
		int fir_space = min(cmd.find(' ', 0), cmd.find('(', 0), cmd.find(')', 0), cmd.find('\n', 0));
                string sub = cmd.substr(0,fir_space);
                int len = sub.length();
                cmd.erase(0,len);
                Clear_frontspace();
                return sub;
    }
}
bool Interpreter::Syntax_CI()
{
    string tmp;
    tmp = Get_Next_CI();
    res.create_index.index_name = tmp;
    tmp = Get_Next_CI();
    if(tmp.compare("on") != 0)
        return false;
    tmp = Get_Next_CI();
    res.create_index.table_name = tmp;
    tmp = Get_Next_CI();
    if(tmp.compare("(")!=0)
        return false;
    tmp = Get_Next_CI();
    string ntmp = Get_Next_CI();
    if(ntmp.compare(")")!=0)
        return false;
    res.create_index.col_name = tmp;
    return true;
}
string Get_Next_DT()
{
    char first = cmd[0];
    if(';' == first)
        return cmd;
    int fir_space = min(cmd.find(' ',0),cmd.find(';',0),cmd.find('(',0),cmd.find('\n',));
    string sub = cmd.substr(0,fir_space);
    int len = sub.length();
    cmd.erase(0,len);
    Clear_frontspace();
    return sub;
}
bool Syntax_DT()
{
    string tmp;
    tmp = Get_Next_DT();
    res.drop_table.table_name = tmp;
    return true;
}
string Get_Next_DI()
{
    int fir_space = min(cmd.find(' ',0),cmd.find(';',0),cmd.find('(',0),cmd.find('\n',));
    string sub = cmd.substr(0,fir_space);
    int len = sub.length();
    cmd.erase(0,len);
    Clear_frontspace();
    return sub;
}
bool Syntax_DI()
{
    string tmp;
    tmp = Get_Next_DI();
    res.drop_index.table_name = tmp;
    return true;
}
string Get_Next_II()
{
}
bool Syntax_II()
{
}
int min(int x,int y)
{
    if(x<0&&y<0)
        return -1;
    else if(x<0 || y<0)
        return (x<0)?y:x;
    else
	return (x <= y) ? x : y;
}
int min(int x,int y,int z,int zz)
{
    return min(min(x,y),min(z,zz));
}
