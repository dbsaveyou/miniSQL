
#include"Basic.h"
#include"API.h"
#include"Catalog.h"
#include"RecordManager.h"
#include"IndexInfo.h"
#include"IndexManager.h"
/*
 * Function to deal Trans_in(res) from Interpreter
 * And set Select_Res(select_res)
 */
API::API(int tcode) :code(tcode)
{
	select_res = new Select_Res;
	rm = new RecordManager(this);
	cm = new Catalog;
	im = new IndexManager(this);
}
API::~API()
{
	//delete select_res;
	delete rm;
	delete cm;
	delete im;
}
int API::Deal_Trans(Trans_in& tmp)
{
	int ecode;
	int ecode6;
	int id = tmp.id;
	Delete &tmp6 = tmp.deletee;
	Select &tmp5 = tmp.select;
	Insert &tmp4 = tmp.insert;
	Drop_Index &tmp3 = tmp.drop_index;
	Create_Index &tmp2 = tmp.create_index;
	Drop_Table &tmp1 = tmp.drop_table;
	Create_Table &tmp0 = tmp.create_table;
	Select_Res &sres = tmp.sres;
	bool ext;
	// All need set code(member var of api)
	switch (id)
	{
	case 0:
		ext = cm->Table_Exist(tmp0.table_name);
		if (true == ext)
		{
			return 11;
		}
		else
		{
			// Check 0<n<=255
			ecode6 = rm->Create_table(tmp0.table_name);
			ecode = cm->CreateTable(tmp0);
			im->CreateTable(tmp0);
		}
		return 10;
		break;
	case 1:
		ext = cm->Table_Exist(tmp1.table_name);
		if (false == ext)
		{
			return 21;
		}
		else
		{
			ecode6 = rm->Drop_table(tmp1.table_name);
			ecode = cm->DropTable(tmp1);
			im->DropTable(tmp1);
		}
		return 20;
		break;
	case 2:
		ext = cm->Index_Exist(tmp2.index_name);
		if (true == ext)
		{
			return 33;
		}
		else
		{
			ecode = cm->CreateIndex(tmp2);
			if (ecode != 30)
				return ecode;
			else
				im->CreateIndex(tmp2);
		}
		return ecode;
		break;
	case 3:// Drop_Index
		ext = cm->Index_Exist(tmp3.index_name);
		if (false == ext)
		{
			return 41;
		}
		else
		{
			im->DeleteIndex(tmp3);
			ecode = cm->Delete_Index(tmp3);
		}
		return ecode;
		break;
	case 4:// Insert
		return ecode = rm->Insert_record(tmp4);  // call function to check and insert
		break;
	case 5:// Select
		ecode6 = rm->select_record(tmp5);
		sres = rm->res;
		select_res = &sres;
		rm->res_pop();
		return ecode6;
		break;
	case 6:// Delete
		ecode6 = rm->delete_record(tmp6);// include check and return error code 
		return ecode6;
		break;
	default:
		break;
	}
}
