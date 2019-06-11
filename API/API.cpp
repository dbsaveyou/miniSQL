#include<API.h>
/* 
 * Function to deal Trans_in(res) from Interpreter 
 * And set Select_Res(select_res)
 */
int API::Deal_Trans(Trans_in& tmp)
{
    int id=tmp.id;
    bool ext;
    // All need set code(member var of api)
    switch(id)
    {
        case 0:// Create_Table
            /*
             * First check whether table exists(Catalog)
             * if yes -> return error
             * else ->create table(catalog)
             */
            /* Function Needed
             * ------->Catalog Manager interface
             *  1.check table if exists ->parameter(table_name)
             *  2.add or create information for Create_Table->parameter(obj)
             * ------->Record Manager interface
             *  1.create data file for Create_Table
             * ------->Index Manager interface
             *  1.create index on primary key
             */
            Create_Table &tmp0=tmp.create_table;
             ext = cm->Table_Exist(tmp0.table_name);
            if(true == ext)
            {
                return 11;
            }
            else
            {
                // Check 0<n<=255
                rm->Create_table(tmp0.table_name);
                cm->CreateTable(tmp0);
                im->CreateTable(tmp0);
            }
            break;
        case 1:// Drop_Table
            /*
             * First check whether table exists(Catalog)
             * if yes -> drop operation
             * else -> return error
             */
            /* Function Needed
             * ------>Catalog Manager interface
             *  1.
             *  3.delete information for Drop_Table
             *  4.find index in this table (for Index Manager to delete)
             * ------>Record Manager interface
             *  2.delete data file for Drop_Table
             * ------>Index Manager interface
             *  1.delete some index
             */
            Drop_Table &tmp1 = tmp.drop_table;
             ext = cm->Table_Exist(tmp1.table_name);
            if(false == ext)
            {
                return 21;
            }
            else
            {
                rm->Drop_table(tmp1.table_name);
                cm->DropTable(tmp1);
                im->DropTable(tmp1);
            }
            break;
        case 2:// Create_Index
            /* 
             * First check whether index exists(Catalog)
             * if yes -> Index_EXIST
             * else -> create
             */
            /* Function Needed
             * ----->Catalog Manager interface
             *  5.check index exist
             *  6.add index information about a table
             *  ---->Index Manager interface
             *  2.create index on this table
             */
            Create_Index &tmp2 = tmp.create_index;
             ext = cm->Index_Exist(tmp2.index_name);
            if(true == ext)
            {
                return 33;
            }
            else
            {
                cm->Create_Index(tmp2);
                im->Create_Index(tmp2);
            }
            break;
        case 3:// Drop_Index
            /*
             * First check whether index exists(Catalog)
             * if yes -> dorp op
             * else error
             */
            /* Function Needed
             * ---->Catalog Manager interface
             *  7.delete index information on table  
             * ---->Index Manager interface
             *  1.
             */
            Drop_Index &tmp3 = tmp.drop_index;
            ext = cm->Index_Exist(tmp3.index_name);
            if(false==ext)
            {
                return 41;
            }
            else
            {
                cm->Delete_Index(tmp3);
                im->Delete_Index(tmp3);
            }

            break;
        case 4:// Insert
            /* 
             * check error (table does't exist | num of values does't match)
             * ( from catalog to check)
             * then can insert 
             */
            /* 
             * ---->Catalog Manager interface
             *  8.acquire information about column numbers
             * ---->Record Manager interface
             *  3.insert new tuple
             * ---->Index Manager interface
             *  4.update B+ tree
             */
            Insert &tmp4 = tmp.insert;
            // Check
            // bool checknum=cm->Check_AttributeNum(tmp4.table_name,tmp4.num_value) all in rm
            int ecode=rm->Insert_record(tmp4);  // call function to check and insert
            // 
            break;
        case 5:// Select
            /* 
             * check column types match?(catalog)
             * call Record to select
             */
            /*
             * ---->Catalog Manager interface
             *  9.check match?
             * ---->Record Manager interface
             *  4.find operation (single or more conditions)-->may call Index
             */
            // malloc Select_Res 
            Select &tmp5 = tmp.select;
            // rm->select  return value->Select_Res not char *
            break;
        case 6:// Delete
            /*
             * check match?
             * delete operation
             */
            /*
             * ---->Catalog Manager interface
             *  9.
             * ---->Record Manager interface
             *  5.delete operation
             */
            // one question here:
            // how record call index funtion?
            // by api or directly?
            Delete &tmp6 = tmp.deletee;
            int ecode6 = rm->delete_record(tmp6);// include check and return error code 
            break;
        default:
            break;
}
