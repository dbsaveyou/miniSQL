#ifndef INTERPRETER_H
#define INTERPRETER_H

#include<string>
#include<Basic.h>
#define SYNTAX_ERROR -1
#define SYNTAX_SUCCESS 0
/* Error Code Here
 * ------>ALL
 *  -1 Syntax Error.
 *  0  Syntax success
 */
class Interpreter
{
    private:
        string cmd;
    public:
        Trans_in res;
        Interpreter(){}
        ~Interpreter(){}
        bool Interpreter_cmd();  
        bool Interpreter_file();
        void set_cmd(string &tmp);
        void Clear_frontspace();  // clear all spaces before first N-blank
        int Get_First();// create 1 drop 2 delete 3 insert 4 select 5 execfile 6
                         // And after, the first word is deleted + clear.
        int Get_Second(); // table 1 index 2 from 3 into 4 * 5 
                         // And after, the second word is deleted + clear.
        string Get_Next_CT(); 
        bool Syntax_CT();
        string Get_Next_CI();
        bool Syntax_CI();
        string Get_Next_DT();
        bool Syntax_DT();
        string Get_Next_DI();
        bool Syntax_DI();
        string Get_Next_DF();
        bool Syntax_DF();
        string Get_Next_II();
        bool Syntax_II();
        string Get_Next_SA();
        bool Syntax_SA();
        string Get_Next_EF();
        bool Syntax_EF();
        int min(int x,int y);
        int min(int x,int y,int z,int zz);
        /*
         * Functionality of Interpreter_cmd
         * Only check Syntax Error in string(cmd)
         * And new(malloc) obj of class(Create_Table,Drop_Table,Drop_Index...) to Trans_in(res)
         * Then API obj will use res to call other function
         */
};

