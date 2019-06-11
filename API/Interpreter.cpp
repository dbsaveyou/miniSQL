#include<Interpreter.h>
#include<string>
int Interpreter::Interpreter_cmd()
{
    int cho1 = Get_First();
    int cho2 = Get_Second();
    int cho = cho1*10+cho2;
    int id;
    switch(cho)
    {
        case 11: // create table
            res.id = 0;
            Create_Table &tmp11 = res.create_table;
            break;
        case 12: // create index
            res.id = 2;
            Create_Index &tmp12 = res.create_index;
            break;
        case 21: // drop table
            res.id = 1;
            Drop_Table &tmp21 = res.drop_table;
            break;
        case 22: // drop index
            res.id = 3;
            Drop_Index &tmp22 = res.drop_index;
        case 33: // delete from
            res.id = 6;
            Delete &tmp33 = res.deletee;
            break;
        case 44: // insert into
            res.id = 4;
            Insert &tmp44 = res.insert;
            break;
        case 55: // select *
            res.id = 5;
            Select &tmp55 = res.select;
            break;
        default:
            // Syntax Error
    }
}
int Interpreter::Get_Second()
{
    int fir_space = cmd.find(' ',0);
    string sub = cmd.substr(0,fir_space);
    int len = sub.length();
    cmd.erase(0,len);
    Clear_frontspace();
    if(sub!=NULL && 0 == sub.compare("table"))
        return 1;
    else if(sub!=NULL && 0 == sub.compare("index"))
        return 2;
    else if(sub!=NULL && 0 == sub.compare("from"))
        return 3;
    else if(sub!=NULL && 0 == sub.compare("into"))
        return 4;
    else if(sub!=NULL && 0 == sub.compare("*"))
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
    if(sub!=NULL &&0==sub.compare("create"))
    {
        return 1;
    }
    else if(sub!=NULL &&0==sub.compare("drop"))
    {
        return 2;
    }
    else if(sub!=NULL &&0==sub.compare("delete"))
    {
        return 3;
    }
    else if(sub!=NULL &&0==sub.compare("insert"))
    {
        return 4;
    }
    else if(sub!=NULL &&0==sub.compare("select"))
    {
        return 5;
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
    while(cmd[0]==' ')
        cmd.erase(0,1);
}


