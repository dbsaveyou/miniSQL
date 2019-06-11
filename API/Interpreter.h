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
        int Interpreter_cmd();  
        void set_cmd(string &tmp);
        void Clear_frontspace();  // clear all spaces before first N-blank
        int Get_First(); // create 1 drop 2 delete 3 insert 4 select 5
                         // And after, the first word is deleted + clear.
        int Get_Second(); // table 1 index 2 from 3 into 4 * 5 
                         // And after, the second word is deleted + clear.

        /*
         * Functionality of Interpreter_cmd
         * Only check Syntax Error in string(cmd)
         * And new(malloc) obj of class(Create_Table,Drop_Table,Drop_Index...) to Trans_in(res)
         * Then API obj will use res to call other function
         */
}

