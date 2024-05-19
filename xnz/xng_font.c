
#include "../xng.h"
#include "crUtils/cr_byte_utils.h"
#include <math.h>

typedef union cr_uint32_union
{
	uint32_t i;
	uint16_t shorts[2];
	char c[4];
    
}cr_uint32_union;

uint32_t CalcTableChecksum(uint32_t* Table, uint32_t Length)
{
	uint32_t Sum = 0L;
	uint32_t* Endptr = Table + ((Length + 3) & ~3) / sizeof(uint32_t);
	while (Table < Endptr)
		Sum += cr_be32toh(*Table++); //must convert each uin32_t table to byte order in order to sum the checksum correctly
	return Sum;
}

uint32_t xng_ttf_calc_table_checksum(uint32_t* table, uint32_t numberOfBytesInTable)
{
	uint32_t sum = 0;
	uint32_t nLongs = (numberOfBytesInTable + 3) / 4;
	printf("\nnumLongs = %u\n", nLongs);
	while (nLongs-- > 0)
		sum += cr_be32toh(*table++); //must convert each uin32_t table to byte order in order to sum the checksum correctly
	return sum;
}

int compare_font_table_offsets(const void* elem1, const void* elem2)
{
	//xng_ttf_offset_table *table1, *table2;
	//table1 = (xng_ttf_offset_table*)elem1;
	//table2 = (xng_ttf_offset_table*)elem2;


	//if( table1->offset < table2->offset)
	if (((xng_ttf_offset_table*)elem1)->offset < ((xng_ttf_offset_table*)elem2)->offset)
		return -1;
	//else
	return 1;

	//return 0;
}

XNG_FONT_API XNG_FONT_INLINE void xng_font_print_details(xng_font* font)
{
	unsigned long tableOffset;
	//cr_uint32_union uint32union;
	int i = 0;

	if (font)
	{
		fprintf(stderr, "\nxng_font structure size = %lld\n", sizeof(xng_font));
		//printf("\nxng_font_offset_table size = %d\n", sizeof(xng_ttf_offset_table));
		fprintf(stderr, "\nxng_font_directory_table size = %lld\n", sizeof(xng_ttf_directory_table));
		fprintf(stderr, "fileSize = %li\n", font->file.size);
		fprintf(stderr, "\nFont Directory Table:\n");

		/*
		if( font->directory_table.scalerType == 0x00010000)
		{
			//deserialize scalerType value
			uint32union.i = cr_be32toh(font->directory_table.scalerType);
			//printf("Scaler Type = %u\n", font->offset_table.scaler_type);

			printf("Version number = %hu.%hu\n", cr_be16toh(uint32union.shorts[0]), cr_be32toh(uint32union.shorts[1]));
		}
		else
			printf("scalerType = %u\n", font->directory_table.scalerType);

		printf("numTables = %hu\n", font->directory_table.numTables);
		printf("searchRange = %hu\n", font->directory_table.searchRange);
		printf("entrySelector = %hu\n", font->directory_table.entrySelector);
		printf("rangeShift = %hu\n", font->directory_table.rangeShift);


		//xng_ttf_offset_table_print(&(font->offset_table));
		//xng_ttf_directory_table_print(&(font->offset_table));


		printf("\nFont Table Directory:\n\n");
		printf("tag = %u\n", font->directory_table.tag);
		printf("checkSUm = %u\n", font->directory_table.checksum);
		printf("offset = %u\n", font->directory_table.offset);
		printf("length = %u\n", font->directory_table.length);
		//printf("rangeShift = %hu\n", font->table.rangeShift);
		*/

		xng_ttf_directory_table_print(&(font->directory_table));
		//printf("\n\n");

		for (i = 0; i < font->numTables; i++)
		{
			fprintf(stderr, "\n\nFont Offset Table:  %.4s\n", (unsigned char*)&(font->offset_tables[i].tag));
			xng_ttf_offset_table_print(&(font->offset_tables[i]));
		}

		tableOffset = 0;
		for (i = 0; i < font->numTables; i++)
		{
			unsigned int rmod4 = (font->offset_tables[i].length % 4); if (rmod4 > 0) rmod4 = 4 - rmod4;

			if (font->offset_tables[i].tag == *(uint32_t*)XNG_TTF_TABLE_TAGS[XNG_TTF_HEAD_TABLE])
			{
				printf(stderr, "\n\nFound HEAD Table!\n");
				xng_ttf_head_table_print((void*)(font->table_data + tableOffset));
			}
			else if (font->offset_tables[i].tag == *(uint32_t*)XNG_TTF_TABLE_TAGS[XNG_TTF_HHEA_TABLE])
			{
				fprintf(stderr, "\n\nFound HHEA Table!\n");
				xng_ttf_hhea_table_print((void*)(font->table_data + tableOffset));
			}
			else if (font->offset_tables[i].tag == *(uint32_t*)XNG_TTF_TABLE_TAGS[XNG_TTF_MAXP_TABLE])
			{
				fprintf(stderr, "\n\nFound MAXP Table!\n");
				xng_ttf_maxp_table_print((void*)(font->table_data + tableOffset));
			}
			else if (font->offset_tables[i].tag == *(uint32_t*)XNG_TTF_TABLE_TAGS[XNG_TTF_LOCA_TABLE])
			{
				fprintf(stderr, "\n\nFound LOCA Table!\n");
				xng_ttf_loca_table_print((void*)(font->table_data + tableOffset), font->numGlyphs, font->indexToLocFormat);
			}
			else if (font->offset_tables[i].tag == *(uint32_t*)XNG_TTF_TABLE_TAGS[XNG_TTF_NAME_TABLE])
			{
				//printf("\nFound NAME Table!\n");
				//xng_ttf_name_table_print( (void*)(font->table_data + tableOffset) );
			}
			else
				fprintf(stderr, "\nFound table tag:  %u vs %u\n", font->offset_tables[i].tag, *(uint32_t*)XNG_TTF_TABLE_TAGS[XNG_TTF_HEAD_TABLE]);

			tableOffset += font->offset_tables[i].length + rmod4;

		}

		fprintf(stderr, "\n\n");
	}
}

void xng_font_serialize_head_table(xng_font* font)
{
	xng_ttf_offset_table* offset_table;
	int i;
	font->head_table = NULL;
	//use the offset tables to find the pointer to the maxp_table

	font->head_table = (xng_ttf_head_table*)xng_font_get_ttf_table(font, *(uint32_t*)XNG_TTF_TABLE_TAGS[XNG_TTF_HEAD_TABLE], &offset_table);
	//ensure that the table data is valid
	//assert( xng_ttf_calc_table_checksum( (uint32_t*)(font->loca_table), offset_table->length ) == offset_table->checksum);
	//serialize the loca table
	font->head_table = xng_ttf_head_table_serialize((font->head_table));

	//printf("\nhead table offset = %u\n", offset_table->offset);

	//xng_ttf_head_table_print( (void*)(font->head_table) );

	font->indexToLocFormat = font->head_table->indexToLocFormat;
	font->unitsPerEm = font->head_table->unitsPerEm;
}


void xng_font_serialize_hhea_table(xng_font* font)
{
	xng_ttf_offset_table* offset_table;
	int i;
	font->hhea_table = NULL;
	//use the offset tables to find the pointer to the maxp_table

	font->hhea_table = (xng_ttf_hhea_table*)xng_font_get_ttf_table(font, *(uint32_t*)XNG_TTF_TABLE_TAGS[XNG_TTF_HHEA_TABLE], &offset_table);
	//ensure that the table data is valid
	//assert( xng_ttf_calc_table_checksum( (uint32_t*)(font->hhea_table), offset_table->length ) == offset_table->checksum);
	//serialize the hhea table
	font->hhea_table = xng_ttf_hhea_table_serialize((font->hhea_table));

	font->numLongHorMetrics = font->hhea_table->numOfLongHorMetrics;

	//xng_ttf_hhea_table_print( (void*)(font->table_data + tableOffset) );

}

void xng_font_serialize_maxp_table(xng_font* font)
{
	xng_ttf_offset_table* offset_table;
	offset_table = NULL;
	assert(font->maxp_table == NULL);

	//use the offset tables to find the pointer to the maxp_table
	font->maxp_table = (xng_ttf_maxp_table*)xng_font_get_ttf_table(font, *(uint32_t*)XNG_TTF_TABLE_TAGS[XNG_TTF_MAXP_TABLE], &offset_table);

	//printf("\nError: table calculated checksum (%u) != offset table checksum (%u)\n", xng_ttf_calc_table_checksum( (uint32_t*)(font->table_data + (long)offset_table->offset), offset_table->length ) == offset_table->checksum);	
	//assert( xng_ttf_calc_table_checksum( (uint32_t*)(font->maxp_table), offset_table->length ) == offset_table->checksum);
	//serialize the maxp table
	font->maxp_table = xng_ttf_maxp_table_serialize((font->maxp_table));

	//store pointer to the the maxp table on the font object
	//font->maxp_table = (xng_ttf_maxp_table* )(font->table_data + offset_table->offset ); 
	font->numGlyphs = font->maxp_table->numGlyphs;
}

//FYI, this method must be defined below xng_font_serialize_maxp_table
void xng_font_serialize_loca_table(xng_font* font)
{
	xng_ttf_offset_table* offset_table;

	//we need to get the indexToLoicFormat property from the head table if it hasn't already been read, because it determines the format of the loca table
	if (!font->head_table)
		xng_font_serialize_head_table(font);

	//we need to get the numGlyphs property from the maxp table if it hasn't already been read, because we need it to know the length of the loca table
	if (!font->maxp_table)
		xng_font_serialize_maxp_table(font);

	//use the offset tables to find the pointer to the maxp_table
	font->loca_table = xng_font_get_ttf_table(font, *(uint32_t*)XNG_TTF_TABLE_TAGS[XNG_TTF_LOCA_TABLE], &offset_table);
	//ensure that the table data is valid
	//assert( xng_ttf_calc_table_checksum( (uint32_t*)(font->loca_table), offset_table->length ) == offset_table->checksum);
	//serialize the loca table
	font->loca_table = xng_ttf_loca_table_serialize((font->loca_table), font->numGlyphs, font->indexToLocFormat);

	xng_font_print_details(font);
}

/*
void xng_font_serialize_maxp_table( void * table_data )
{
			//xng_ttf_maxp_table * maxp_table;
			//printf("\nSerialize MAXP Table!\n");
			xng_ttf_maxp_table_serialize( (void*)(table_data) );
			font->maxp_table = (xng_ttf_maxp_table* )(table_data );
			font->numGlyphs = font->maxp_table->numGlyphs;
}
*/

XNG_FONT_API XNG_FONT_INLINE void xng_font_buffer_serialize(xng_font* font)
{
	//store variables for reading through file
	unsigned long bytesRead = 0;
	int i = 0;
	uint32_t test;
	unsigned long numTableBytes, tableOffset, structPaddingBytes;
	//void *loca_table;
	
	xng_ttf_directory_table directory_table = *(font->directory_table);

	//convert the bytes we read from file to system byte format
	xng_ttf_directory_table_serialize(&(directory_table));
	bytesRead += sizeof(xng_ttf_directory_table);
	//printf("bytesRead = %lu", bytesRead);

	font->numTables = directory_table.numTables;
	//font->offset_table.scalerType = cr_be32toh(font->offset_table.scalarType);

	//Apple stores value as 32 bit int
	//whereas windows stores two 16 bit version numbers in the same value
	//so we must unpack the big endian bytes differently based on the appropriate OS that wrote the TTF file
	//we assume Apple packing first, check for a value that indicates Windows/Adobe, and then undo to handle that case
	if (directory_table.sfntVersion == 0x74727565) //'true' or 'typ1'
	{
		//printf("\nFound an Apple iOS/OSX TTF font package!");
	}
	else if (directory_table.sfntVersion == 0x00010000)
	{
		//printf("\nFound a Microsoft/Adobe OpenType font containing a TrueType outline!\n");
	}
	else if (directory_table.sfntVersion == 0x4F54544F)
	{
		//printf("\nFound a Microsoft/Adobe OpenType font containing CFF data!\n");
	}
	else
	{
		//printf("\nUnknown TTF package type!");
	}

	//alloc memory for offset tables
	font->offset_tables = (xng_ttf_offset_table*)malloc(sizeof(xng_ttf_offset_table) * directory_table.numTables);//(xng_ttf_offset_table*)_aligned_malloc( sizeof(xng_ttf_offset_table) * font->directory_table.numTables, 16 );

	numTableBytes = 0;
	//read offset tables from file and serialize them
	for (i = 0; i < directory_table.numTables; i++)
	{
		//fread(&(font->offset_tables[i]), sizeof(xng_ttf_offset_table), 1, file);
		memcpy(&(font->offset_tables[i]), font->file.buffer + bytesRead, sizeof(xng_ttf_offset_table)); bytesRead += sizeof(xng_ttf_offset_table); //printf("bytesRead = %lu", bytesRead);
		xng_ttf_offset_table_serialize(&(font->offset_tables[i]));

		unsigned int rmod4 = (font->offset_tables[i].length % 4); if (rmod4 > 0) rmod4 = 4 - rmod4;
		numTableBytes += font->offset_tables[i].length + rmod4;
	}


	//allocate memory to store table data bytes read from file
	font->table_data = (unsigned char*)malloc(numTableBytes);// (unsigned char *)_aligned_malloc( numTableBytes, 16 );
	//fread(font->table_data, numTableBytes, 1, file);
	memcpy(font->table_data, font->file.buffer + bytesRead, numTableBytes); bytesRead += numTableBytes;

	//font offset tables should have been read in alphabetical order based on the "tag" property
	//but upon inspection the ttf file lists capitalized tags in alphabetical order first and then lower case tags
	//in alphabetical order, we will resort the list so we can read through the file sequentially without jumping around
	//later we can avoid the sort by checking the tag of each and storing an index from within the serialize loop
	qsort((void*)font->offset_tables, directory_table.numTables, sizeof(xng_ttf_offset_table), compare_font_table_offsets);

	for (i = 0; i < directory_table.numTables; i++)
	{
		printf("\n\nFont Offset Table:  %.4s\n", (unsigned char*)&(font->offset_tables[i].tag));
		xng_ttf_offset_table_print(&(font->offset_tables[i]));
	}
	//the list now sorted so we can traverse the file in order, the "head" table generally comes first
	//fseek(file, font->offset_tables[0]., SEEK_SET);

	tableOffset = 0;
	for (i = 0; i < directory_table.numTables; i++)
	{
		unsigned int rmod4 = (font->offset_tables[i].length % 4); if (rmod4 > 0) rmod4 = 4 - rmod4;

		//read the table bytes from file
		//fread(font->table_data + tableOffset, font->offset_tables[i].length + rmod4, 1, file);

		if (bytesRead != font->offset_tables[i].offset)
		{
			printf("\nError: bytesRead (%lu) != table offset (%u)\n", bytesRead, font->offset_tables[i].offset);
		}

		if (!font->head_table && font->offset_tables[i].tag == *(uint32_t*)XNG_TTF_TABLE_TAGS[XNG_TTF_HEAD_TABLE])
		{
			//printf("\nSerialize HEAD Table!\n");
			//TO DO: Calculate HEAD table checksum appropriately
			//assert( xng_ttf_calc_table_checksum( (uint32_t*)(font->table_data + tableOffset), font->offset_tables[i].length ) != font->offset_tables[i].checksum);

			font->head_table = xng_ttf_head_table_serialize((void*)(font->table_data + tableOffset));
			//font->head_table = (xng_ttf_head_table*)(font->table_data + tableOffset);

			//store the head table indexToLocFormat for use as we are loading name table name record strings			
			font->indexToLocFormat = font->head_table->indexToLocFormat;
			font->unitsPerEm = font->head_table->unitsPerEm;
		}
		else if (!font->hhea_table && font->offset_tables[i].tag == *(uint32_t*)XNG_TTF_TABLE_TAGS[XNG_TTF_HHEA_TABLE])
		{
			//xng_ttf_hhea_table * hhea_table;
			//printf("\nSerialize HHEA Table!\n");

			//in order to verify the table checksums, we need to interpret the data as read from the file
			//as an array of uint32_t, then convert each uint32_t to system byte representation in order to sum them correctly
			//in order to read the tables appropriately we also need to convert each var in each table as read from file directly to system byte order
			//assert( xng_ttf_calc_table_checksum( (uint32_t*)(font->table_data + tableOffset), font->offset_tables[i].length ) == font->offset_tables[i].checksum );

			xng_ttf_hhea_table_serialize((void*)(font->table_data + tableOffset));
			font->hhea_table = (xng_ttf_hhea_table*)((uint8_t*)font->table_data + tableOffset);

			font->numLongHorMetrics = font->hhea_table->numOfLongHorMetrics;
		}
		else if (font->offset_tables[i].tag == *(uint32_t*)XNG_TTF_TABLE_TAGS[XNG_TTF_HMTX_TABLE])
		{
			//we need to serialize the hhea table if not already done, because the hmtx table relies on it's numLongHorMetrics property
			if (!font->hhea_table)
				xng_font_serialize_hhea_table(font);

			//we also need numGlyphs from the maxp table
			if (!font->maxp_table)
				xng_font_serialize_maxp_table(font);

			//assert( xng_ttf_calc_table_checksum( (uint32_t*)(font->table_data + tableOffset), font->offset_tables[i].length ) == font->offset_tables[i].checksum );
			xng_ttf_hmtx_table_serialize((void*)(font->table_data + tableOffset), font->numLongHorMetrics, font->numGlyphs);
			font->hmtx_table = (void*)(font->table_data + tableOffset);

		}
		else if (font->offset_tables[i].tag == *(uint32_t*)XNG_TTF_TABLE_TAGS[XNG_TTF_VHEA_TABLE])
		{
			//xng_ttf_vhea_table * vhea_table;
			//printf("\nSerialize HHEA Table!\n");

			//in order to verify the table checksums, we need to interpret the data as read from the file
			//as an array of uint32_t, then convert each uint32_t to system byte representation in order to sum them correctly
			//in order to read the tables appropriately we also need to convert each var in each table as read from file directly to system byte order
			assert(xng_ttf_calc_table_checksum((uint32_t*)(font->table_data + tableOffset), font->offset_tables[i].length) == font->offset_tables[i].checksum);

			xng_ttf_vhea_table_serialize((void*)(font->table_data + tableOffset));
			font->vhea_table = (xng_ttf_vhea_table*)((uint8_t*)font->table_data + tableOffset);

			font->numLongVerMetrics = font->vhea_table->numOfLongVerMetrics;

		}
		else if (font->offset_tables[i].tag == *(uint32_t*)XNG_TTF_TABLE_TAGS[XNG_TTF_VMTX_TABLE])
		{
			assert(xng_ttf_calc_table_checksum((uint32_t*)(font->table_data + tableOffset), font->offset_tables[i].length) == font->offset_tables[i].checksum);
			xng_ttf_vmtx_table_serialize((void*)(font->table_data + tableOffset), font->numLongVerMetrics, font->numGlyphs);
			font->vmtx_table = (void*)(font->table_data + tableOffset);

		}
		else if (!font->maxp_table && font->offset_tables[i].tag == *(uint32_t*)XNG_TTF_TABLE_TAGS[XNG_TTF_MAXP_TABLE])
		{
			//printf("\nSerialize MAXP Table!\n");

			//in order to verify the table checksums, we need to interpret the data as read from the file
			//as an array of uint32_t, then convert each uint32_t to system byte representation in order to sum them correctly
			//in order to read the tables appropriately we also need to convert each var in each table as read from file directly to system byte order
			assert(xng_ttf_calc_table_checksum((uint32_t*)(font->table_data + tableOffset), font->offset_tables[i].length) == font->offset_tables[i].checksum);

			//serialize the table 
			font->maxp_table = xng_ttf_maxp_table_serialize((void*)(font->table_data + tableOffset));
			//font->maxp_table = (xng_ttf_maxp_table* )(font->table_data + tableOffset ); 

			//store pointer to the the maxp table on the font object
			font->numGlyphs = font->maxp_table->numGlyphs;
		}
		else if (!font->loca_table && font->offset_tables[i].tag == *(uint32_t*)XNG_TTF_TABLE_TAGS[XNG_TTF_LOCA_TABLE])
		{
			//printf("\nSerialize LOCA Table!\n");
			//printf("\nError: table calculated checksum (%u) != offset table checksum (%u)\n", xng_ttf_calc_table_checksum( (uint32_t*)(font->table_data + tableOffset), font->offset_tables[i].length ) == font->offset_tables[i].checksum);

			//in order to verify the table checksums, we need to interpret the data as read from the file
			//as an array of uint32_t, then convert each uint32_t to system byte representation in order to sum them correctly
			//in order to read the tables appropriately we also need to convert each var in each table as read from file directly to system byte order
			assert(xng_ttf_calc_table_checksum((uint32_t*)(font->table_data + tableOffset), font->offset_tables[i].length) == font->offset_tables[i].checksum);

			//we need to get the numGlyphs property from the maxp table if it hasn't already been read, because we need it to know the length of the loca table
			if (!font->maxp_table)
				xng_font_serialize_maxp_table(font);

			//serialize the loca table which contains the glyph offsets
			font->loca_table = xng_ttf_loca_table_serialize((void*)(font->table_data + tableOffset), font->numGlyphs, font->indexToLocFormat);
		}
		else if (font->offset_tables[i].tag == *(uint32_t*)XNG_TTF_TABLE_TAGS[XNG_TTF_CMAP_TABLE])
		{
			//printf("\nSerialize CMAP Table!\n");

			//in order to verify the table checksums, we need to interpret the data as read from the file
			//as an array of uint32_t, then convert each uint32_t to system byte representation in order to sum them correctly
			//in order to read the tables appropriately we also need to convert each var in each table as read from file directly to system byte order
			//assert( xng_ttf_calc_table_checksum( (uint32_t*)(font->table_data + tableOffset), font->offset_tables[i].length ) == font->offset_tables[i].checksum );

			font->cmap_table = xng_ttf_cmap_table_serialize((void*)(font->table_data + tableOffset));
			//font->cmap_table = (void*)(font->table_data + tableOffset);

		}
		else if (font->offset_tables[i].tag == *(uint32_t*)XNG_TTF_TABLE_TAGS[XNG_TTF_GLYF_TABLE])
		{
			uint16_t* offsets16;
			uint32_t* offsets32;
			xng_ttf_glyf_table_header* glyf_header;
			int glyphIndex;
			unsigned int glyfTableLengthRMod4;
			unsigned long subTableOffset = tableOffset;
			int tableBytesRead = 0;

			//void * glyf_table;
			xng_ttf_glyf_table* glyf_table;
			uint32_t glyfTableLength;
			//xng_ttf_loca_table *loca_table;
			uint32_t locaTableLength;
			uint32_t prevGlyphOffset;
			unsigned long currentGlyphOffset;

			//printf("\nSerialize GLYF Table!\n");

			//in order to verify the table checksums, we need to interpret the data as read from the file
			//as an array of uint32_t, then convert each uint32_t to system byte representation in order to sum them correctly
			//in order to read the tables appropriately we also need to convert each var in each table as read from file directly to system byte order
			//assert( xng_ttf_calc_table_checksum( (uint32_t*)(font->table_data + tableOffset), font->offset_tables[i].length ) == font->offset_tables[i].checksum );

			//we need to serialize the loca table if it hasn't already been serialized, because we need it to read through the glyf table
			if (!font->loca_table)
				xng_font_serialize_loca_table(font);
			//font->loca_table = (xng_ttf_loca_table*)xng_font_get_ttf_table(font, *(uint32_t*)XNG_TTF_TABLE_TAGS[XNG_TTF_LOCA_TABLE], &locaTableLength );

			font->glyf_table = (font->table_data + tableOffset);

			assert(font->loca_table != NULL);

			glyfTableLength = 0;
			prevGlyphOffset = 1;

			offsets16 = (uint16_t*)(font->loca_table);
			offsets32 = (uint32_t*)(font->loca_table);
			//Note:  assumes loca table offsets are in increasing order
			for (glyphIndex = 0; glyphIndex < font->numGlyphs; glyphIndex++)
			{
				//glyf_table = (xng_ttf_glyf_table*)(font->table_data + subTableOffset);

				//determine the glyph offset data type so we can read the offset of a particular glyf table corresponding to a glyf index
				if (font->indexToLocFormat == 0)
				{

					//printf("\n indexToLocFormat = %hd \n", font->indexToLocFormat);
					//printf("\n tableBytesRead = %u \n", tableBytesRead);
					//printf("\n loca offset = %hu \n", offsets[glyphIndex] * 2);

					//assert( tableBytesRead == offsets[glyphIndex] * 2 );

					currentGlyphOffset = offsets16[glyphIndex] * 2;
				}
				else
				{


					//printf("\n indexToLocFormat = %hd \n", font->indexToLocFormat);
					//printf("\n tableBytesRead = %u \n", tableBytesRead);
					//printf("\n loca offset = %u \n", offsets[glyphIndex]);

					//assert( tableBytesRead == offsets[glyphIndex] );

					currentGlyphOffset = offsets32[glyphIndex];
				}

				if (currentGlyphOffset != prevGlyphOffset)
				{
					//uncomment this to serialize all glyf tables sequentially
					/*
					assert( tableBytesRead == currentGlyphOffset );

					glyfTableLength = xng_ttf_glyf_table_serialize( (void*)(font->table_data + subTableOffset ), NULL );

					glyfTableLengthRMod4 = glyfTableLength % 4;
					if(glyfTableLengthRMod4 != 0 )
						glyfTableLengthRMod4 = 4 - glyfTableLengthRMod4;

					printf("\n glyfTableLength = %u\n", glyfTableLength);
					tableBytesRead += glyfTableLength + glyfTableLengthRMod4;
					subTableOffset += glyfTableLength + glyfTableLengthRMod4;
					//subTableOffset += sizeof(xng_ttf_glyf_table_header);

					prevGlyphOffset = currentGlyphOffset;
					*/

					//here we will access each glyf based on the loca table offset
					//and we will just serialize the header data of the glyf tables
					//and leave the serialization of the remaining glyf table data only for when
					//a glyf is actually accessed the first time
					///*glyfTableLength = */xng_ttf_glyf_table_serialize_header_data( (void*)(font->table_data + tableOffset + currentGlyphOffset), NULL );
					prevGlyphOffset = currentGlyphOffset;

				}
				/*
				else
				{
					printf("\nGlyph index %d already serialized \n", glyphIndex);
				}
				*/
			}



		}
		else if (font->offset_tables[i].tag == *(uint32_t*)XNG_TTF_TABLE_TAGS[XNG_TTF_NAME_TABLE])
		{
			xng_ttf_name_table* name_table;
			int nameTableIndex = 0;
			unsigned long subTableOffset = tableOffset;
			unsigned long tableBytesRead = 0;
			//printf("\nSerialize NAME Table!\n");

			//we already read the entire name table into table bytes, but we need to serialize
			//the header fields first so we can know how many name records there are

			//printf("\nName table name record size = %d\n", sizeof(xng_ttf_name_record));
			//printf("\nName table header size = %d\n", sizeof(xng_ttf_name_table_header));
			//printf("\nName table name record size = %d\n", sizeof(xng_ttf_name_record));

			//in order to verify the table checksums, we need to interpret the data as read from the file
			//as an array of uint32_t, then convert each uint32_t to system byte representation in order to sum them correctly
			//in order to read the tables appropriately we also need to convert each var in each table as read from file directly to system byte order
			if (xng_ttf_calc_table_checksum((uint32_t*)(font->table_data + tableOffset), font->offset_tables[i].length) != font->offset_tables[i].checksum)
			{
				printf("\nError: table calculated checksum (%u) != offset table checksum (%u)\n", xng_ttf_calc_table_checksum((uint32_t*)(font->table_data + tableOffset), font->offset_tables[i].length), font->offset_tables[i].checksum);
			}
			else
				xng_ttf_name_table_header_serialize((void*)(font->table_data + tableOffset));


			//we'll use a local variable to store the offset into the table_data as we read through the name records
			//so we can serialize them
			subTableOffset = tableOffset + sizeof(xng_ttf_name_table_header);
			tableBytesRead += sizeof(xng_ttf_name_table_header);

			name_table = (xng_ttf_name_table*)(font->table_data + tableOffset);

			//store a pointer to nameRecords in the name table struct
			name_table->nameRecords = (xng_ttf_name_record*)(font->table_data + subTableOffset);

			//store a pointer to the beginning of the name strings after the name_records
			//name_table->names = font->table_data + name_table->stringOffset;

			//name_table->names = font->table_data + subTableOffset;


			//serialize each name record and name string sequentially
			for (nameTableIndex = 0; nameTableIndex < name_table->count; nameTableIndex++)
			{
				xng_ttf_name_record_serialize((void*)(font->table_data + subTableOffset));
				subTableOffset += sizeof(xng_ttf_name_record);
				tableBytesRead += sizeof(xng_ttf_name_record);

			}


			if (tableBytesRead != name_table->stringOffset)
				printf("\nError:  table bytes read (%hu) != name table string offset (%hu) after reading name records\n", tableBytesRead, name_table->stringOffset);

			//TO DO:  check the format var to see if there are LangTagRecord entries
			//name_table->names = font->table_data + subTableOffset;
			name_table->names = font->table_data + tableOffset + name_table->stringOffset;

			//serialize each name record string
			xng_ttf_name_strings_serialize((void*)(name_table));

		}
		//else
		//	printf("\nFound unserialized table tag:  %u vs %u\n", font->offset_tables[i].tag, *(uint32_t*)XNG_TTF_TABLE_TAGS[XNG_TTF_HEAD_TABLE]  );


		bytesRead += font->offset_tables[i].length + rmod4;
		//printf("\nbytesRead = %lu\n", bytesRead);

		tableOffset += font->offset_tables[i].length + rmod4;

	}

	//font->unitsPerEm = MAX(font->head_table->xMax - font->head_table->xMin, font->head_table->yMax - font->head_table->yMin);
}


XNG_FONT_API XNG_FONT_INLINE void* xng_font_get_ttf_table(xng_font* font, uint32_t table_id, xng_ttf_offset_table** offset_table_ptr)
{
	unsigned int headerTableBytes;
	int i;
	unsigned int rmod4;

	uint32_t tableOffset;
	if (!font)
		return NULL;

	headerTableBytes = sizeof(xng_ttf_directory_table) + sizeof(xng_ttf_offset_table) * font->numTables;

	tableOffset = 0;
	for (i = 0; i < font->numTables; i++)
	{
		if (font->offset_tables[i].tag == table_id)
		{
			//printf("\n\nFound HEAD Table!\n");
			printf("\nAccumulated offset = %u\n", tableOffset);
			printf("\nTable offset = %u\n", font->offset_tables[i].offset - headerTableBytes);
			printf("\nTable Length = %u\n", font->offset_tables[i].length);

			*offset_table_ptr = &(font->offset_tables[i]);
			return (void*)(font->table_data + tableOffset);
		}

		rmod4 = (font->offset_tables[i].length % 4);
		if (rmod4 > 0)
			rmod4 = 4 - rmod4;
		tableOffset += font->offset_tables[i].length + rmod4;

	}

	printf("\n xng_font_get_ttf_table failed to find table for id: %u\n\n", table_id);

	return NULL;
}
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

XNG_FONT_API XNG_FONT_INLINE void xng_font_load_glyph(xng_font* font, xng_ttf_glyph* glyph, unsigned long char_code, float font_size, float* dpi)
{
	float l, r, t, b;
	float ppem, scale;
	bool XNG_TTF_MISSING_OR_EMTPY_GLYPH, XNG_TTF_GLYPH_ALREADY_SERIALIZED;
	uint32_t next_offset;
	size_t formatSize, glyph_size;
	xng_ttf_glyf_table_header empty_header;
	int16_t ascent, descent;

	XNG_TTF_GLYPH_ALREADY_SERIALIZED = false;
	xng_ttf_cmap_glyph_index_for_char_code(font->cmap_table, char_code, &(glyph->index));


	XNG_TTF_MISSING_OR_EMTPY_GLYPH = false;

	glyph->offset = 0;
	next_offset = 0;

	//get the glyf offset without a conditional statement
	formatSize = ((font->indexToLocFormat + 1) * sizeof(uint16_t));
	memcpy(&(glyph->offset), ((uint8_t*)font->loca_table + glyph->index * formatSize), formatSize);
	//glyph->offset = glyph->offset >> (32 - formatSize*8);

	memcpy(&(next_offset), ((uint8_t*)font->loca_table + (glyph->index + 1) * formatSize), formatSize);
	//next_offset = next_offset >> (32 - formatSize*8);

	//with the glyph index, we can get glyph horizontal metrics (i.e. advanceWidth and leftSideBearing) from the hmtx table 
	//which is always required to be present for TTF compliant fonts (though maybe not SFNT housed fonts)
	xng_ttf_hmtx_table_get_metrics_for_glyph_at_index(font->hmtx_table, font->numLongHorMetrics, font->numGlyphs, glyph);

	//if the vmtx table is present, we can get the horizontal metrics (i.e. advanceHeight and topSideBearing)
	//but it is not required to be present. if it is not we fallback to using ascent/descent info in other tables
	if (font->vmtx_table)
		xng_ttf_vmtx_table_get_metrics_for_glyph_at_index(font->vmtx_table, font->numLongVerMetrics, font->numGlyphs, glyph);
	/*else if ( face->os2.version != 0xFFFFU )
	{
	  *tsb = (FT_Short)( face->os2.sTypoAscender - yMax );
	  *ah  = (FT_UShort)FT_ABS( face->os2.sTypoAscender -
								face->os2.sTypoDescender );
	}*/
	else //fallback to hhea_table
	{
		ascent = font->hhea_table->ascent; //store ascent we may need it later
		descent = font->hhea_table->descent; //store ascent we may need it later

		glyph->metrics.advanceHeight = ascent - font->hhea_table->descent;
		//we can not yet compute the topSideBearing here because it is dependent on the glyph bounding box yMax value
		//glyph->topSideBearing  = font->hhea_table->ascent - glyph->header->yMax;
	}


	printf("\nglyphIndex = %hu\n", glyph->index);
	printf("\nglyph offset = %u\n", glyph->offset * (2 - font->indexToLocFormat));
	printf("\nindex to loc format = %hd\n", font->indexToLocFormat);
	//if the glyf offset is equal to the next offset (i.e. has 0 size)
	//then it is either a MISSING glyph or and EMPTY glyph then it has no glyf_table entry and thus no glyf table header
	//so we populate an empty glyph header so that a bounding box may be calculated for
	//empty or missing character without an outline (i.e. a glyf without any glyf contour points)	
	if (glyph->offset == next_offset)
	{
		XNG_TTF_MISSING_OR_EMTPY_GLYPH = true;
		empty_header.xMin = empty_header.yMin = 0;
		empty_header.xMax = glyph->metrics.advanceWidth;
		empty_header.yMax = glyph->metrics.advanceHeight; //set the ymax to the same as the advanceHeight

		glyph->header = &empty_header;
		glyph->bb.width = glyph->bb.height = 0;
		//glyph->bufferWidth = 0;// MAX( (r-l), ppem);
		//glyph->bufferHeight = 0;//MAX( (b-t) , ppem);
		//all we need to do for an empty glyf is populate the affine_transform and return

	}
	else //only get the header if their is a glyph associated with the char code
	{
		//glyph offset needs to be multiplied by 2 if indexToLocFormat i 0 (ie offsets are of type uint16 as opposed to uint32)
		glyph->offset *= (2 - font->indexToLocFormat);

		//get the glyph table header to get its bounding box in units per em (FontUnits)
		//note that it has already been serialized to system byte format in the ram buffer
		//empty_header = *(xng_ttf_glyf_table_header*)((uint8_t*)(font->glyf_table) + glyph->offset);
		memcpy(&empty_header, ((uint8_t*)(font->glyf_table) + glyph->offset), sizeof(xng_ttf_glyf_table_header));

		xng_ttf_glyf_table_serialize_header_data(&empty_header, glyph);
		//empty_header.numberOfContours = cr_be16toh(empty_header.numberOfContours);
		//empty_header.xMin = cr_be16toh(empty_header.xMin);
		//empty_header.yMin = cr_be16toh(empty_header.yMin);
		//empty_header.xMax = cr_be16toh(empty_header.xMax);
		//empty_header.yMax = cr_be16toh(empty_header.yMax);
		glyph->header = &empty_header;
		//xng_ttf_glyf_table_serialize_header_data( &empty_header, glyph);

		printf("\n numContours = %hd\n", empty_header.numberOfContours);
		printf("xMin = %hd\n", empty_header.xMin);
		printf("xMax = %hd\n", empty_header.xMax);
		printf("yMin = %hd\n", empty_header.yMin);
		printf("yMax = %hd\n", empty_header.yMax);

		//use the header data to calculate the bounding box width/height for the glyph at unitsPerEm resolution
		//these values should always be positive
		glyph->bb.width = glyph->header->xMax - glyph->header->xMin;//glyph.bb.right - glyph.bb.left;
		glyph->bb.height = glyph->header->yMax - glyph->header->yMin;//glyph.bb.top - glyph.bb.bottom;

		/*
		//catch some glyphs that are already serialized (i.e. the font is malformed, but we still want to try to read it)
		if( glyph->header->numberOfContours > font->maxp_table->maxContours || (glyph->bb.width < 0 || glyph->bb.height < 0) || ( glyph->bb.width > font->unitsPerEm || glyph->bb.height > font->unitsPerEm) )
		{
			XNG_TTF_GLYPH_ALREADY_SERIALIZED = true;
			//deserialize the header then check it again to see if it is still malformed
			xng_ttf_glyf_table_serialize_header_data(&empty_header, glyph);
			//glyph->header = &empty_header;

			//use the header data to calculate the bounding box width/height for the glyph at unitsPerEm resolution
			//these values should always be positive
			glyph->bb.width = glyph->header->xMax - glyph->header->xMin;//glyph.bb.right - glyph.bb.left;
			glyph->bb.height = glyph->header->yMax - glyph->header->yMin;//glyph.bb.top - glyph.bb.bottom;

			//catch malformed glyphs and map them to the missing character glyf or to an empty bitmap
			if( glyph->header->numberOfContours > font->maxp_table->maxContours || (glyph->bb.width < 0 || glyph->bb.height < 0) || ( glyph->bb.width > font->unitsPerEm || glyph->bb.height > font->unitsPerEm) )
			{
				XNG_TTF_MISSING_OR_EMTPY_GLYPH = true;
				empty_header.xMin = empty_header.yMin = 0;
				empty_header.xMax = glyph->metrics.advanceWidth;
				empty_header.yMax = glyph->metrics.advanceHeight; //set the ymax to the same as the advanceHeight

				glyph->header = &empty_header;
				glyph->bb.width = glyph->bb.height = 0;
			}


		}
		*/
		//glyph->metrics.leftSideBearing;
	}

	/*
	printf(" glyph offset = %u \n", glyph->offset);
	if( font->indexToLocFormat == 0 )
	{
		uint16_t * offsets = (uint16_t*)(font->loca_table);
		glyph->offset = offsets[glyph->index] * 2;

		if( glyph->offset == offsets[glyph->index+1]*2)
		{
			XNG_TTF_MISSING_OR_EMTPY_GLYPH = true;
			//assert(1==0);
			//glyph->advanceHeight = 0;
			//glyph->topSideBearing = 0;
			//glyph->leftSideBearing = 0;
			//glyph->advanceWidth = glyph->bb.width;
			//glyph->bb.width = glyph->bb.height = 0;
			//return;
		}
	}
	else
	{
		uint32_t * offsets = (uint32_t*)(font->loca_table);
		glyph->offset = offsets[glyph->index];

		if( glyph->offset == offsets[glyph->index+1] )
		{
			XNG_TTF_MISSING_OR_EMTPY_GLYPH = true;
			//assert(2==0);
			//glyph->advanceHeight = 0;
			//glyph->topSideBearing = 0;
			//glyph->leftSideBearing = 0;
			//glyph->advanceWidth = glyph->bb.width;
			//glyph->bb.width = glyph->bb.height = 0;
			//return;
		}
	}
	*/

	//system("pause");

	//now that we have the glyph header->yMax value we can calculate the topSideBearing for the glyph
	//if it wasn't already provided to us by the vmtx table above
	if (!font->vmtx_table)
		glyph->metrics.topSideBearing = ascent - glyph->header->yMax;
	glyph->metrics.bottomSideBearing = descent - glyph->header->yMin;

	ppem = font_size / 72.0f * (*dpi);//* (144.f / 96.f ) /  72.f ;
	//glyph.point_size = font_size;
	scale = (float)font_size / (float)font->unitsPerEm;//(float)MAX(glyph->header->xMax - glyph->header->xMin, glyph->header->yMax - glyph->header->yMin);
	//xscale = (float)ppem/(float)glyph.bb.width;
	//yscale = (float)ppem/(float)glyph.bb.height;

	printf("\n scale = %f\n", scale);
	printf("\n scaled width= %hd\n", glyph->bb.width);
	printf("\n scaled height  = %hd\n", glyph->bb.height);
	printf("\n advance width before scale = %hu\n", glyph->metrics.advanceWidth);
	//scale the glyph horizontal and vertical metrics
	glyph->metrics.advanceWidth = (uint16_t)((float)glyph->metrics.advanceWidth * scale);
	glyph->metrics.leftSideBearing = (int16_t)((float)glyph->metrics.leftSideBearing * scale);
	glyph->metrics.advanceHeight = (uint16_t)((float)glyph->metrics.advanceHeight * scale);
	glyph->metrics.topSideBearing = (int16_t)((float)glyph->metrics.topSideBearing * scale);
	glyph->metrics.bottomSideBearing = (int16_t)((float)glyph->metrics.bottomSideBearing * scale);
	glyph->metrics.ascent = (int16_t)((float)glyph->metrics.ascent * scale);
	glyph->metrics.descent = (int16_t)((float)glyph->metrics.descent * scale);

	printf("\n width= %hd\n", glyph->bb.width);
	printf("\n height  = %hd\n", glyph->bb.height);

	if (!XNG_TTF_MISSING_OR_EMTPY_GLYPH)
	{
		l = floor((float)glyph->header->xMin * scale);
		t = floor((float)glyph->header->yMax * -scale);
		r = ceil((float)glyph->header->xMax * scale);
		b = ceil((float)glyph->header->yMin * -scale);;

		glyph->bb.width = r - l;//MAX(glyph->bb.width * scale, ppem);//
		glyph->bb.height = b - t;//MAX(glyph->bb.height * scale, ppem);// scale;

		//glyph->bufferWidth = r-l;// MAX( (r-l), ppem);
		//glyph->bufferHeight = b - t;//MAX( (b-t) , ppem);
	}



}

static const xnz_byte4_packed draw_color = { 128, 128, 128, 255 };

XNG_FONT_API XNG_FONT_INLINE void xng_font_draw_glyph_bitmap_for_char_code(xng_font* font, xng_ttf_glyph* glyph, unsigned long char_code, float font_size, float* dpi, float** accumulation_buffer)// uint8_t** bitmap_buffer)
{
	float l, r, t, b;
	unsigned int coordIndex, pixelIndex, rowIndex, colIndex;
	float fontSize, scale, ppem, acc, y;
	bool XNG_TTF_MISSING_OR_EMTPY_GLYPH, XNG_TTF_GLYPH_ALREADY_SERIALIZED;
	uint32_t next_offset;
	size_t formatSize, glyph_size;
	xng_ttf_glyf_table_header empty_header;
	int16_t ascent, descent;
	//uint32_t currentGlyphOffset;
	//xng_ttf_glyph glyph;

	//TO DO:  amend calls for BIG ENDIAN system byte order (e.g. Motorolla devices)s
	XNG_TTF_GLYPH_ALREADY_SERIALIZED = false;

	xng_ttf_cmap_glyph_index_for_char_code(font->cmap_table, char_code, &(glyph->index));


	assert(glyph->index < font->numGlyphs);
	printf("\nglyph char = %lu\n", (unsigned long)char_code);
	printf("\nglyph index = %hu\n", glyph->index);

	XNG_TTF_MISSING_OR_EMTPY_GLYPH = false;

	glyph->offset = 0;
	next_offset = 0;

	//get the glyf offset without a conditional statement
	formatSize = ((font->indexToLocFormat + 1) * sizeof(uint16_t));
	memcpy(&(glyph->offset), ((uint8_t*)font->loca_table + glyph->index * formatSize), formatSize);
	//glyph->offset = glyph->offset >> (32 - formatSize*8);

	memcpy(&(next_offset), ((uint8_t*)font->loca_table + (glyph->index + 1) * formatSize), formatSize);
	//next_offset = next_offset >> (32 - formatSize*8);

	//with the glyph index, we can get glyph horizontal metrics (i.e. advanceWidth and leftSideBearing) from the hmtx table 
	//which is always required to be present for TTF compliant fonts (though maybe not SFNT housed fonts)
	xng_ttf_hmtx_table_get_metrics_for_glyph_at_index(font->hmtx_table, font->numLongHorMetrics, font->numGlyphs, glyph);

	//if the vmtx table is present, we can get the vertical metrics (i.e. advanceHeight and topSideBearing)
	//but it is not required to be present. if it is not we fallback to using ascent/descent info in other tables
	if (font->vmtx_table) xng_ttf_vmtx_table_get_metrics_for_glyph_at_index(font->vmtx_table, font->numLongVerMetrics, font->numGlyphs, glyph);
	/*else if ( face->os2.version != 0xFFFFU )
	{
	  *tsb = (FT_Short)( face->os2.sTypoAscender - yMax );
	  *ah  = (FT_UShort)FT_ABS( face->os2.sTypoAscender -
								face->os2.sTypoDescender );
	}*/
	else //fallback to hhea_table
	{
		ascent  = font->hhea_table->ascent; //store ascent we may need it later
		descent = font->hhea_table->descent;
		glyph->metrics.advanceHeight = ascent - font->hhea_table->descent;
		//we can not yet compute the topSideBearing here because it is dependent on the glyph bounding box yMax value
		//glyph->topSideBearing  = font->hhea_table->ascent - glyph->header->yMax;
	}

	printf("\nglyphIndex = %hu\n", glyph->index);
	printf("\nglyph offset = %u\n", glyph->offset * (2 - font->indexToLocFormat));
	//if the glyf offset is equal to the next offset (i.e. has 0 size)
	//then it is either a MISSING glyph or and EMPTY glyph then it has no glyf_table entry and thus no glyf table header
	//so we populate an empty glyph header so that a bounding box may be calculated for
	//empty or missing character without an outline (i.e. a glyf without any glyf contour points)	
	if (glyph->offset == next_offset)
	{
		XNG_TTF_MISSING_OR_EMTPY_GLYPH = true;
		empty_header.xMin = empty_header.yMin = 0;
		empty_header.xMax = glyph->metrics.advanceWidth;
		empty_header.yMax = glyph->metrics.advanceHeight; //set the ymax to the same as the advanceHeight

		glyph->header = &empty_header;
		glyph->bb.width = glyph->bb.height = 0;
		//glyph->bufferWidth = 0;// MAX( (r-l), ppem);
		//glyph->bufferHeight = 0;//MAX( (b-t) , ppem);

		//all we need to do for an empty glyf is populate the affine_transform and return

	}
	else //only get the header if their is a glyph associated with the char code
	{
		//glyph offset needs to be multiplied by 2 if indexToLocFormat i 0 (ie offsets are of type uint16 as opposed to uint32)
		glyph->offset *= (2 - font->indexToLocFormat);


		//get the glyph table header to get its bounding box in units per em (FontUnits)
		//note that it has already been serialized to system byte format in the ram buffer
		//glyph->header = (xng_ttf_glyf_table_header*)((uint8_t*)(font->glyf_table) + glyph->offset);
			//empty_header = *(xng_ttf_glyf_table_header*)((uint8_t*)(font->glyf_table) + glyph->offset);
		memcpy(&empty_header, ((uint8_t*)(font->glyf_table) + glyph->offset), sizeof(xng_ttf_glyf_table_header));

		xng_ttf_glyf_table_serialize_header_data(&empty_header, glyph);
		//empty_header.numberOfContours = cr_be16toh(empty_header.numberOfContours);
		//empty_header.xMin = cr_be16toh(empty_header.xMin);
		//empty_header.yMin = cr_be16toh(empty_header.yMin);
		//empty_header.xMax = cr_be16toh(empty_header.xMax);
		//empty_header.yMax = cr_be16toh(empty_header.yMax);
		glyph->header = &empty_header;

		//use the header data to calculate the bounding box width/height for the glyph at unitsPerEm resolution
		//these values should always be positive
		glyph->bb.width  = glyph->header->xMax - glyph->header->xMin;//glyph.bb.right - glyph.bb.left;
		glyph->bb.height = glyph->header->yMax - glyph->header->yMin;//glyph.bb.top - glyph.bb.bottom;

		/*
		//catch some glyphs that are already serialized (i.e. the font is malformed, but we still want to try to read it)
		if( glyph->header->numberOfContours > font->maxp_table->maxContours || (glyph->bb.width < 0 || glyph->bb.height < 0) || ( glyph->bb.width > font->unitsPerEm || glyph->bb.height > font->unitsPerEm) )
		{
			XNG_TTF_GLYPH_ALREADY_SERIALIZED = true;
			//deserialize the header then check it again to see if it is still malformed
			xng_ttf_glyf_table_serialize_header_data(&empty_header, glyph);
			//glyph->header = &empty_header;

			//use the header data to calculate the bounding box width/height for the glyph at unitsPerEm resolution
			//these values should always be positive
			glyph->bb.width = glyph->header->xMax - glyph->header->xMin;//glyph.bb.right - glyph.bb.left;
			glyph->bb.height = glyph->header->yMax - glyph->header->yMin;//glyph.bb.top - glyph.bb.bottom;

			//catch malformed glyphs and map them to the missing character glyf or to an empty bitmap
			if( glyph->header->numberOfContours > font->maxp_table->maxContours || (glyph->bb.width < 0 || glyph->bb.height < 0) || ( glyph->bb.width > font->unitsPerEm || glyph->bb.height > font->unitsPerEm) )
			{
				XNG_TTF_MISSING_OR_EMTPY_GLYPH = true;
				empty_header.xMin = empty_header.yMin = 0;
				empty_header.xMax = glyph->metrics.advanceWidth;
				empty_header.yMax = glyph->metrics.advanceHeight; //set the ymax to the same as the advanceHeight

				glyph->header = &empty_header;
				glyph->bb.width = glyph->bb.height = 0;
			}



		}
		*/

		printf("\n numContours = %hd\n", empty_header.numberOfContours);
		printf("xMin = %hd\n", empty_header.xMin);
		printf("xMax = %hd\n", empty_header.xMax);
		printf("yMin = %hd\n", empty_header.yMin);
		printf("yMax = %hd\n", empty_header.yMax);


		glyph->metrics.leftSideBearing = glyph->header->xMin;
	}

	/*
	printf(" glyph offset = %u \n", glyph->offset);
	if( font->indexToLocFormat == 0 )
	{
		uint16_t * offsets = (uint16_t*)(font->loca_table);
		glyph->offset = offsets[glyph->index] * 2;

		if( glyph->offset == offsets[glyph->index+1]*2)
		{
			XNG_TTF_MISSING_OR_EMTPY_GLYPH = true;
			//assert(1==0);
			//glyph->advanceHeight = 0;
			//glyph->topSideBearing = 0;
			//glyph->leftSideBearing = 0;
			//glyph->advanceWidth = glyph->bb.width;
			//glyph->bb.width = glyph->bb.height = 0;
			//return;
		}
	}
	else
	{
		uint32_t * offsets = (uint32_t*)(font->loca_table);
		glyph->offset = offsets[glyph->index];

		if( glyph->offset == offsets[glyph->index+1] )
		{
			XNG_TTF_MISSING_OR_EMTPY_GLYPH = true;
			//assert(2==0);
			//glyph->advanceHeight = 0;
			//glyph->topSideBearing = 0;
			//glyph->leftSideBearing = 0;
			//glyph->advanceWidth = glyph->bb.width;
			//glyph->bb.width = glyph->bb.height = 0;
			//return;
		}
	}
	*/

	//system("pause");

	//now that we have the glyph header->yMax value we can calculate the topSideBearing for the glyph
	//if it wasn't already provided to us by the vmtx table above
	if (!font->vmtx_table)
	{
		glyph->metrics.topSideBearing = ascent - glyph->header->yMax;
		//glyph->metrics.bottomSideBearing = descent - glyph->header->yMin;
	}
	glyph->metrics.bottomSideBearing = descent - glyph->header->yMin;

	ppem = font_size / 72.0f * (*dpi);//* (144.f / 96.f ) /  72.f ;
	//glyph.point_size = font_size;
	scale = (float)font_size / (float)font->unitsPerEm;//(float)MAX(glyph->header->xMax - glyph->header->xMin, glyph->header->yMax - glyph->header->yMin);
	//xscale = (float)ppem/(float)glyph.bb.width;
	//yscale = (float)ppem/(float)glyph.bb.height;

	//scale the glyph horizontal and vertical metrics
	glyph->metrics.advanceWidth = (uint16_t)((float)glyph->metrics.advanceWidth * scale);
	glyph->metrics.leftSideBearing = (int16_t)((float)glyph->metrics.leftSideBearing * scale);
	glyph->metrics.advanceHeight = (uint16_t)((float)glyph->metrics.advanceHeight * scale);
	glyph->metrics.topSideBearing = (int16_t)((float)glyph->metrics.topSideBearing * scale);
	glyph->metrics.bottomSideBearing = (int16_t)((float)glyph->metrics.topSideBearing * scale);




	// glyph->bufferWidth = ppem;..r-l;//MAX( (r-l), ppem);
	// glyph->bufferHeight = ppem;//b-t;//MAX( (b-t) , ppem);


	 //glyph->bb.width = glyph->bb.width * scale + 1;
	 //glyph->bb.height = glyph->bb.height * scale + 1;



	 //calculate the affine transform to scale the glyph outline points
	 //for compound glyphs, this affine transform will be concatenated with that read from each compound glyph's component glyphs
	 /*
	 if( XNG_TTF_MISSING_OR_EMTPY_GLYPH )
	 {
		 glyph->affine_transform.a = scale;
		 glyph->affine_transform.b = 0.f;
		 glyph->affine_transform.c = 0.f;
		 glyph->affine_transform.d = -scale;
		 glyph->affine_transform.e = 0;//((float)glyph->header->xMin * -scale ) ;
		 glyph->affine_transform.f = -((float)glyph->bb.height * -scale )  ;


	 }
	 else
	 {
	 */


	 //}	

	 //if the glyph is not empty or missing, then run the accumulation step to render the accumulation buffer containing the glyph countour outlines to bitmap buffer 
	if (!XNG_TTF_MISSING_OR_EMTPY_GLYPH)
	{
		//calculate the scaled bounding box for the glyph
		l = floor((float)glyph->header->xMin *  scale);
		t = floor((float)glyph->header->yMax * -scale);
		r =  ceil((float)glyph->header->xMax *  scale);
		b =  ceil((float)glyph->header->yMin * -scale);

		//set the width and height based on the rounded bounding box
		glyph->bb.width  = r - l;//MAX(glyph->bb.width * scale, ppem);//
		glyph->bb.height = b - t;//MAX(glyph->bb.height * scale, ppem);// scale;


		//glyph->bufferWidth = r-l;//MAX( (r-l), ppem);
		//glyph->bufferHeight = b-t;//MAX( (b-t) , ppem);

		//set the transform to scale the glyph contour points as they are being read
		glyph->affine_transform.a = scale;
		glyph->affine_transform.b = 0.f;
		glyph->affine_transform.c = 0.f;
		glyph->affine_transform.d = -scale;
		glyph->affine_transform.e = -((float)glyph->header->xMin *  scale);
		glyph->affine_transform.f = -((float)glyph->header->yMax * -scale);

		//create the accumulation buffer for the signed area algorithm if its pointer input parameter points to null memory
		//that is if the memory hasn't been allocated by the calling client code, note that this memory will need to be freed by the client
		if (!*accumulation_buffer) *accumulation_buffer = (float*)malloc(glyph->bb.width * glyph->bb.height * sizeof(float));//glyph_bb.width * glyph_bb.height * sizeof(float));

		//create the bitmap buffer as output for the signed area algorithm if its pointer input parameter points to null memory
		//that is fi memory hasn't been allocated by the calling client code, note that this memory will need to be freed by the client
		//if (!*bitmap_buffer) *bitmap_buffer = (uint8_t*)malloc(glyph->bb.width * glyph->bb.height);//glyph_bb.width * glyph_bb.height * sizeof(float));

		//glyph.accumulation_buffer = (float*)malloc( glyph.bb.width * glyph.bb.height * sizeof(float));//glyph_bb.width * glyph_bb.height * sizeof(float));

		//clear the accumulation and bitmap buffer, clearing the bitmap buffer might not be necessary
		//if the entire accumulation buffer is parsed and copied to the bitmap buffer
		memset(*accumulation_buffer, 0, sizeof(float) * glyph->bb.width * glyph->bb.height);
		//memset(*bitmap_buffer, 0, glyph->bb.width * glyph->bb.height);

		glyph->accumulation_buffer = *accumulation_buffer;

		//serialize the contour data associated with the glyph from the data read from file,
		//this will parse the glyph bezier curve path to create a glyph outline floating point accumulation buffer internally

		if (XNG_TTF_GLYPH_ALREADY_SERIALIZED)
			xng_ttf_glyf_table_draw_contour_data(font->indexToLocFormat, font->loca_table, font->glyf_table, (void*)((uint8_t*)(font->glyf_table) + glyph->offset + sizeof(xng_ttf_glyf_table_header)), glyph);//, &glyph_bb );
		else
			xng_ttf_glyf_table_serialize_and_draw_contour_data(font->indexToLocFormat, font->loca_table, font->glyf_table, (void*)((uint8_t*)(font->glyf_table) + glyph->offset + sizeof(xng_ttf_glyf_table_header)), glyph);//, &glyph_bb );

		/*
		//run the final accumulation algorithm steps using sse intrinsics algorithm
		//adding 4 to the size ensures that we read and process the entire buffer since sse processes 4 floats at a time in parallel
#ifdef XNG_FONT_USE_SIMD_SSE
		xng_ttf_accumulate_sse(*accumulation_buffer, *bitmap_buffer, (uint32_t)(glyph->bb.width * glyph->bb.height + 4));
#else
		acc = 0.f;
		y = 0.f;
		for (pixelIndex = 0; pixelIndex < glyph->bb.width * glyph->bb.height; pixelIndex++)
		{
			acc += glyph->accumulation_buffer[pixelIndex]; //sum the scanline values
			y = fabs(acc);     //take absolute value
			y = MIN(y, 1.f);
			(*bitmap_buffer)[pixelIndex] = (uint8_t)(y * 255.f); //put the value in a bitmap
		}
#endif
		*/
	}

	//set the accumulation_buffer on the glyph to null
	//glyph->accumulation_buffer = NULL;

	/*
	for(pixelIndex = 0; pixelIndex < glyph->numContourPoints; pixelIndex++)
	{
		//current_point.vector[0] = glyph_transform->a * (float)(xCoords[repeatFlagIndex]) + glyph->affine_transform.c * (float)(yCoords[repeatFlagIndex]) + glyph_transform->e;
		//				current_point.vector[1] = glyph->affine_transform.b * (float)(xCoords[repeatFlagIndex]) + glyph_transform->d * (float)(yCoords[repeatFlagIndex]) + glyph_transform->f;

		glyph->xCoords[pixelIndex] = glyph->affine_transform.a * (float)glyph->xCoords[pixelIndex] + glyph->affine_transform.e;
		glyph->yCoords[pixelIndex] = glyph->affine_transform.d * (float)glyph->yCoords[pixelIndex] + glyph->affine_transform.f;

		if( glyph->xCoords[pixelIndex] < 0 || glyph->yCoords[pixelIndex] < 0 )
		{
			printf("\nglyphCoords[%d] = %hu, %hu\n", (int)pixelIndex, glyph->xCoords[pixelIndex], glyph->yCoords[pixelIndex]);
		}
		//glyph->yCoords[pixelIndex] = glyph->affine_transform.d * (float)glyph->yCoords[pixelIndex] + glyph->affine_transform.f;

	}

	//only draw the starting and last point of each contour
	//for( pixelIndex = cr_be16toh(glyph->endPtsOfContours[0]) + 1; pixelIndex < cr_be16toh(glyph->endPtsOfContours[1]); pixelIndex ++ )
//	{
		cr_image_buffer_rasterize_circles_from_uint16_x_y_points(*bitmap_buffer, glyph->bb.width, glyph->bb.height, 1, 8, false, &(((uint16_t*)(glyph->xCoords))[cr_be16toh(glyph->endPtsOfContours[0]) + 1]), &(((uint16_t*)(glyph->yCoords))[cr_be16toh(glyph->endPtsOfContours[0]) + 1]), 1, 2, &draw_color);
		//cr_image_buffer_rasterize_circles_from_uint16_x_y_points(*bitmap_buffer, glyph->bb.width, glyph->bb.height, 1, 8, false, &(((uint16_t*)(glyph->xCoords))[cr_be16toh(glyph->endPtsOfContours[1])]), &(((uint16_t*)(glyph->yCoords))[cr_be16toh(glyph->endPtsOfContours[1])]), 1, 2, &draw_color);

		//cr_image_buffer_rasterize_circles_from_uint16_x_y_points(*bitmap_buffer, glyph->bb.width, glyph->bb.height, 1, 8, false, &(((uint16_t*)(glyph->xCoords))[glyph->numContourPoints-6]), &(((uint16_t*)(glyph->yCoords))[glyph->numContourPoints-6]), 6, 2, &draw_color);

		//cr_image_buffer_rasterize_circles_from_uint16_x_y_points(*bitmap_buffer, glyph->bb.width, glyph->bb.height, 1, 8, false, (uint16_t*)(glyph->xCoords[glyph->endPtsOfContours[pixelIndex]]), (uint16_t*)(glyph->yCoords[glyph->endPtsOfContours[pixelIndex]]), 1, 2, &draw_color);

//	}
	//cr_image_buffer_rasterize_circles_from_uint16_x_y_points(*bitmap_buffer, glyph->bb.width, glyph->bb.height, 1, 8, false, (uint16_t*)(glyph->xCoords), (uint16_t*)(glyph->yCoords), glyph->numContourPoints, 2, &draw_color);

	cr_image_buffer_export_to_bmp(*bitmap_buffer, glyph->bb.width, glyph->bb.height, 1, 8, "Test.bmp");

	if( glyph->xCoords )
		free(glyph->xCoords);
	if( glyph->yCoords )
		free(glyph->yCoords);
	*/

	return;//xng_font_status_ok;
}



static const int start_char_code = 32;
static const int   end_char_code = 128;

XNG_FONT_API XNG_FONT_INLINE  void xng_prepare_font_atlas(xng_font * font, xng_font_atlas * atlas)
{
	xnz_uint2  texture_atlas_size;

	//we will use the an array of glyph atlas nodes mapped to the cr_glyph_info struct for each character in the cr_font_atlas
	//to both sort the glyph bitmaps by size and then find their placement in the atlas texture by traversing the binary tree
	xng_binary_node* current_node;
	xng_binary_node   root_node;
	xng_binary_node   glyph_atlas_nodes[4096];

	xng_ttf_glyph glyph = {0};
	//cr_glyph_info sorted_glyphs[128]; // an array for storing the sorted glyphs by area

	uint32_t w, h, i, node_index;
	uint32_t maxw, maxh;
	uint64_t area = 0; uint64_t dim = 16;

	float dpi = ((float)font->display_resolution.x / (((float)font->display_dimensions.x) / 25.4f));    
	maxw = maxh = w = h = i = node_index = 0;


    //the retina scaling factor does not affect the font atlas preparation and font drawing procedures AT ALL,
    //however, it does affect calculation of line breaks when drawing text so we store it here for later
    //if retina scaling is not applicable on your device, or disabled just set this input value to 1.0f;
    //atlas->retinaScale = retinaScale;
    
    atlas->minTopSideBearing = font->unitsPerEm;
    
    //populate the root node rect (which is the size of our entire atlas texture)
    memset(&root_node,        0, sizeof(xng_binary_node));
    memset(glyph_atlas_nodes, 0, sizeof(xng_binary_node) * 4096);
    //glyph_atlas_nodes[0].rect.x = 0;
    //glyph_atlas_nodes[0].rect.y = 0;
    root_node.rect.z = INT_MAX; //set width  to max desired graphics texture size
    root_node.rect.w = INT_MAX; //set height to max desired graphics texture size

    fprintf(stderr, "\n node rect x = %u, y = %u, w = %u, h = %u\n", root_node.rect.x, root_node.rect.y , root_node.rect.size.x, root_node.rect.size.y);

	uint32_t borderWidth = 0;

	// load glyph properties for each character of interest in the font 
	for(i = start_char_code; i < end_char_code; i++) 
	{
		xng_font_load_glyph(font, &glyph, i, atlas->fontSize, &dpi);



		w += glyph.bb.width;
		h += glyph.bb.height;
		area += (uint64_t)(glyph.bb.width + borderWidth) * (uint64_t)(glyph.bb.width + borderWidth);

		maxh = MAX(maxh, glyph.bb.height);
		maxw = MAX(maxw, glyph.bb.width);

        atlas->c[i].bw = glyph.bb.width;
        atlas->c[i].bh = glyph.bb.height;
        
        glyph_atlas_nodes[i].rect.z = glyph.bb.width;
        glyph_atlas_nodes[i].rect.w = glyph.bb.height;
        glyph_atlas_nodes[i].index  = i;
        
        atlas->c[i].ax = glyph.metrics.advanceWidth;
        atlas->c[i].ay = glyph.metrics.advanceHeight;    //glyph.topSideBearing -  glyph.advanceHeight;//glyph.advanceHeight;// >> 6;
              
		atlas->c[i].bl = glyph.metrics.leftSideBearing;
        atlas->c[i].bt = glyph.metrics.topSideBearing ;  //_fontGlyph->bitmap_top;
        atlas->c[i].bb = glyph.metrics.bottomSideBearing;//_fontGlyph->bitmap_top;
        
        atlas->minTopSideBearing = MIN(  glyph.metrics.topSideBearing, atlas->minTopSideBearing );
        
        atlas->c[i].tx = (float)w;
        //atlas->ascent  = glyph.metrics.ascent;
        //atlas->descent = glyph.metrics.descent;
        fprintf(stderr, "bw = %f\n", atlas->c[i].bw);
        fprintf(stderr, "bh = %f\n", atlas->c[i].bh);
        fprintf(stderr, "ax = %f\n", atlas->c[i].ax);
        fprintf(stderr, "ay = %f\n", atlas->c[i].ay);
        fprintf(stderr, "bl[%d] = %f\n", i, atlas->c[i].bl);
        fprintf(stderr, "bt = %f\n", atlas->c[i].bt);
        fprintf(stderr, "tx = %f\n", atlas->c[i].tx);
        
        fprintf(stderr, "advanceWidth = %f\n", atlas->c[i].ax);

  	    //x += glyph.bb.width;//_fontGlyph->bitmap.width;
	}

	/* you might as well save this value as it is needed later on */
    //atlas->w = 16;//root_node.rect.z;
    //atlas->h = 16;//root_node.rect.w;

	while (dim * dim < area) dim = dim << 1;
	atlas->w = atlas->h = dim;
	atlas->mw = maxw; atlas->mh = maxh;

    //TEXTURE ATLAS PACKING
    
    //we need to create a binary tree of the size num glyphs + a single root node for the texture packing algorithm    
    //then sort the glyph rendered bitmap sizes at the desired font size
    //qsort ((void*)&(glyph_atlas_nodes[start_char_code]), end_char_code - start_char_code, sizeof(xng_binary_node), compare_font_glyph_areas);
    
    //populate root node children
    //root_node.child[0] =
    //glyph_atlas_node[0].rect.z = atlas->c[0]
    
    //traverse the glyphs sorted by area
    //to find their position in the binary tree
    
    texture_atlas_size.x = atlas->w; texture_atlas_size.y = atlas->h;
    for(i = start_char_code; i < end_char_code; i++)
    {
		xnz_uint2 bitmap_size = { atlas->c[i].bw, atlas->c[i].bh };
        fprintf(stderr, "\n bitmap width = %u, height = %u\n", bitmap_size.x, bitmap_size.y);
        if( bitmap_size.x == 0 || bitmap_size.y == 0 ) continue;
        
        current_node = xng_binary_node_pack(&root_node, &texture_atlas_size, &bitmap_size, glyph_atlas_nodes, &node_index);

        if( current_node )
		{
			atlas->c[i].tx = current_node->rect.x ; // atlas->w;
			atlas->c[i].ty = current_node->rect.y ; // atlas->h;
			fprintf(stderr, "\n node rect x = %u, y = %u, w = %u, h = %u\n", current_node->rect.x, current_node->rect.y , current_node->rect.size.x, current_node->rect.size.y);
			current_node->filled = true;
		}
		else
		{
			assert(current_node);
			fprintf(stderr, "\nInsert failed at index %d!\n", i);
		}	

		//atlas->c[i].tx /= (float)atlas->w;
    }
    
    /*
    for(i = start_char_code; i < end_char_code; i++)
    {
        atlas->c[i].tx /= (float)atlas->w;
    }
     
    */

	//atlas->w = atlas->c[59].bw;
	//atlas->h = atlas->c[59].bh;

	//TO DO: create platform independent align memory routines so that SSE intrinics may be used for font glyph rendering
    //atlas->accumulation_buffer = (float*)malloc( width * height * sizeof(float));  //, 16);
    //atlas->bitmap_buffer     = (uint8_t*)malloc( width * height * sizeof(uint8_t));//, 16);
}


XNG_FONT_API XNG_FONT_INLINE void xng_load_font_atlas(xng_font* font, xng_font_atlas* atlas, uint8_t** bitmap_buffer)
{
	uint32_t pixelIndex = 0;
	uint32_t char_code  = 0;

	float         dpi   = ((float)font->display_resolution.x / (((float)font->display_dimensions.x) / 25.4f)); ;
	float		  acc = 0.f;
	float		  y   = 0.f;
	xng_ttf_glyph glyph = {0}; memset(&glyph, 0, sizeof(xng_ttf_glyph));

	/*
	#ifdef CR_PLATFORM_API
		display_resolution = cr_display_get_resolution();
		display_dimensions = cr_display_get_dimensions();
	#else

	#endif
	*/

	//render the glyph for each character as bitmap between the unicode characters for empty character and 'del' character
	for (char_code = start_char_code; char_code < end_char_code; char_code++)
	{
		//draw glyph to floating point accumulation buffer
		xng_font_draw_glyph_bitmap_for_char_code(font, &glyph, char_code, atlas->fontSize, &dpi, &(atlas->accumulation_buffer));// bitmap_buffer);
		//glTexSubImage2D(GL_TEXTURE_2D, 0, atlas->c[i].tx, atlas->c[i].ty, glyph.bb.width, glyph.bb.height, GL_ALPHA, GL_UNSIGNED_BYTE, atlas->bitmap_buffer);

		//run the final accumulation algorithm steps using sse intrinsics algorithm
		//adding 4 to the size ensures that we read and process the entire buffer since sse processes 4 floats at a time in parallel
#ifdef XNG_FONT_USE_SIMD_SSE
		xng_ttf_accumulate_sse(*accumulation_buffer, *bitmap_buffer, (uint32_t)(glyph->bb.width * glyph->bb.height + 4));
#else
		acc = 0.f; y = 0.f; //reset values
		for (pixelIndex = 0; pixelIndex < glyph.bb.width * glyph.bb.height; pixelIndex++)
		{
			acc += glyph.accumulation_buffer[pixelIndex];		    //sum the scanline values
			glyph.accumulation_buffer[pixelIndex] = fabs(acc);	    //map to positive range

			//y = fabs(acc); y = MIN(y, 1.f);						//take absolute value, clamp to 1			
			//glyph.accumulation_buffer[pixelIndex] = y *255.f;	    //map to bitmap pixel range
			//(*bitmap_buffer)[pixelIndex] = (uint8_t)(y * 255.f);
		}

		//translate each floating point pixel to a byte pixel value (will clamp and round)
		xng_floats_to_bytes_insitu(glyph.accumulation_buffer, glyph.bb.width * glyph.bb.height);


		
		//for each scanline copy to the bitmap buffer
		uint64_t dstOffset = atlas->c[char_code].ty * atlas->w + atlas->c[char_code].tx;
		uint8_t* pData = &((*bitmap_buffer)[dstOffset]); uint8_t* srcData = (uint8_t*)glyph.accumulation_buffer;
		//for (pixelIndex = 0; pixelIndex < glyph.bb.width * glyph.bb.height; pixelIndex++) (*bitmap_buffer)[pixelIndex] = srcData[pixelIndex];// (uint8_t)(glyph.accumulation_buffer[pixelIndex]);// (uint8_t)(y * 255.f); //put the value in a bitmap
		for (pixelIndex = 0; pixelIndex < glyph.bb.width*glyph.bb.height; pixelIndex+= glyph.bb.width)
		{
			memcpy(pData, &srcData[pixelIndex], glyph.bb.width); pData += atlas->w; //copy + advance destination ptr by the size of atlas scanline row
			//(*bitmap_buffer)[pixelIndex] = (uint8_t)(glyph.accumulation_buffer[pixelIndex]);// (uint8_t)(y * 255.f); //put the value in a bitmap
		}
		
#endif

		//xng_image_draw_image_in_rect(&atlas[CR_PBR_MTL_BASE_COLOR_MAP],
		//	&img[imgIndex],
		//	mesh->materialPartitions[mtlIndex].mtl->textures[CR_PBR_MTL_BASE_COLOR_MAP]->rect);

		atlas->c[char_code].tx = atlas->c[char_code].tx / atlas->w;
		atlas->c[char_code].ty = atlas->c[char_code].ty / atlas->h;
		//x += glyph.bb.width;//_fontGlyph->bitmap.width;
	}
}

XNG_FONT_API XNG_FONT_INLINE xng_font_parse_status xng_font_open(xng_font* font, const char* fontPath)
{

	//1 OPEN THE GBL OR GLTF FILE FOR READING AND GET FILESIZE USING LSEEK
	font->file.fd   = cr_file_open(fontPath);
	font->file.size = cr_file_size(font->file.fd);
	font->file.path = fontPath;

	fprintf(stderr, "\nxng_font_open::File Size =  %lu bytes\n", font->file.size);

	//2 MAP THE FILE TO BUFFER FOR READING
#ifndef _WIN32
	font->file.buffer = cr_file_map_to_buffer(&(font->file.buffer), font->file.size, PROT_READ, MAP_SHARED | MAP_NORESERVE, font->file.fd, 0);
	if (madvise(font->file.buffer, (size_t)font->file.size, MADV_SEQUENTIAL | MADV_WILLNEED) == -1) {
		printf("\nread madvise failed\n");
	}
#else
	font->file.mFile = cr_file_map_to_buffer(&(font->file.buffer), font->file.size, PROT_READ, MAP_SHARED | MAP_NORESERVE, font->file.fd, 0);
#endif

	//3  READ THE PNG (.png) file signature
	font->directory_table = (xng_ttf_directory_table*)font->file.buffer;

	xng_font_buffer_serialize(font);
	xng_font_print_details(font);

	return xng_font_status_ok;
}

/*
XNG_FONT_API XNG_FONT_INLINE xng_font_parse_status xng_font_load(xng_font* font, const char* fontPath)
{
	FILE* file;
	typedef long length_type;
	length_type length;
	unsigned long result;
	xng_font* fontPtr;
	//xng_font font ;//= {0};

	//*font = (xng_font*)malloc( sizeof(xng_font));
	fontPtr = font;
	memset(fontPtr, 0, sizeof(xng_font));
	file = fopen(fontPath, "rb");

	//check file handle is valid
	if (!file)
	{
		printf("\n Failed to load font file!\n");
		return xng_font_status_io_error;
	}

	// get file size (can result in I/O errors)
	// if this is a 32-bit OS, long is enough; if this is a unix system, long is 64-bit, which is enough; otherwise we can't do anything anyway.

	fseek(file, 0, SEEK_END);
	length = ftell(file);
	fseek(file, 0, SEEK_SET); //rewind(file);

	// check for I/O errors
	if (length < 0) return xng_font_status_io_error;

	// check for overflow
	result = (size_t)length;

	if ((length_type)(result) != length) return xng_font_status_out_of_memory;

	// finalize
	//*out_result = result;
	fontPtr->sourceFileSize = result;

	xng_font_file_serialize(file, fontPtr);

	xng_font_print_details(fontPtr);

	fclose(file);

	//xng_font_delete(fontPtr);

	return xng_font_status_ok;
}

*/

XNG_FONT_API XNG_FONT_INLINE xng_font_parse_status xng_font_close(xng_font* font)
{
	//int i=0;

	if (font)
	{
		if (font->offset_tables)
			free(font->offset_tables);

		if (font->table_data)
			free(font->table_data);

		//free(font);
		//font = NULL;
	}

	CRFileClose(&(font->file));

	return xng_font_status_ok;
}





