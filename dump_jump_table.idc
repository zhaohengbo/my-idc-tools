#include <idc.idc>

static dump_table(start,end,is_thumb)
{
	auto ea;
	auto ref;
	for(ea = start;ea <= end;ea = ea + 4)
	{
		//create_dword(ea);
		OpOff(ea,0,0);
		ref = Dword(ea) - 1;
		if(ref != 0)
		{
			//ARM32 or Thumb mode
			if(is_thumb)
				SetReg(ref,"t",1);
			else
				SetReg(ref,"t",0);
			
			MakeCode(ref);
			MakeFunction(ref,-1);
		}
	}
}


static main()
{
	Message("ARM VM Table dump tools by ZhaoXiaoWei\n");
	
	dump_table(0x80037920,0x800389924,1);
	
	Message("dump done!\n");
}
