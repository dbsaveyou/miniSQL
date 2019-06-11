#ifndef API_H
#define API_H
#include<Basic.h>
/* Error Code Here
 * ------>Create_Table
 *  10 Success.
 *  11 Table exists.
 * ------>Drop_Table
 *  20 Success.
 *  21 Table does't exists.
 *  ----->Create_Index
 *  30 Success.
 *  31 Can't find table
 *  32 Can't find column
 *  33 Index name exist.
 *  ----->Drop_Index
 *  40 Success.
 *  41 Index does't exist.
 *  ----->Insert
 *  50 Success.
 *  51 Can't find table
 *  52 num of values does't match
 *  ----->Select
 *  60 Success.
 *  61 Can't find table
 *  62 type of column does't match
 *  ----->Delete
 *  70 Success.
 *  71 Can't find table.
 */

class API
{
    public:
        int code;
        Select_Res *select_res;
        RecordManager *rm;
        CatalogManager *cm;
        IndexManager *im;



        API(int tcode):code(tcode){}
        ~API(){}
        int Deal_Trans(Trans_in& tmp);
        /* 
         * Function to deal Trans_in(res) from Interpreter 
         * And set Select_Res(select_res)
         */
}

#endif 
