#include <idc.idc>

//this is for TI's CGT compress

/*---------------------------------------------------------------------*/
/* The first byte is the delimiter. With the following sequences:      */
/* 1. zero, 16-bit run length, repeated char.                          */
/* 2. zero, zero, 24-bit run length, repeated char.                    */
/* 3. 8-bit length with value less then 4. This means delimiter is     */
/*    repeated by the run length times.                                */
/* 4. 8-bit run length followed by repeat char.                        */
/* 5. zero, zero, zero marks the end of data.                          */
/*---------------------------------------------------------------------*/

static ti_decompress(compress_addr,decompress_addr,is_rle24)
{
	auto delim;
	auto ch;
	auto run_len;
	
	delim = byte(compress_addr);
	compress_addr++;
	
	while(1)
	{
		ch = byte(compress_addr);
		compress_addr++;
		
		if (ch != delim)
		{
			patch_byte(decompress_addr, ch);
			decompress_addr++;
			continue;
		}
		
		run_len = byte(compress_addr);
		compress_addr++;
		
		if (run_len == 0)
		{
			run_len = byte(compress_addr) << 8;
			compress_addr++;
			run_len = run_len + byte(compress_addr);
			compress_addr++;

			//end of data
			if (run_len == 0) 
				break;
			
			if ((is_rle24 != 0) && (run_len <= 0xff))
			{
				//Warning: in IDC, 'i' will be INITILIZED ONLY WHEN ENTER THIS FUNC
				auto i;
				for (i = 1; i < 3; i++)
				{
					run_len = ((run_len << 8) | byte(compress_addr));
					compress_addr++;
				}
			}
			
			ch = byte(compress_addr);
			compress_addr++;
		}
		else if (run_len < 4)
		{
			ch = delim;
		}
		else
		{
			ch = byte(compress_addr);
			compress_addr++;
		}
		
		while(run_len--) 
		{
			patch_byte(decompress_addr, ch);
			decompress_addr++;
		}
	}
}

static main()
{
	Message("TI CGT decompress tool by ZhaoXiaoWei\n");
	
	ti_decompress(0x80046001,0x80032710,1);
	
	Message("decompress done!\n");
}
