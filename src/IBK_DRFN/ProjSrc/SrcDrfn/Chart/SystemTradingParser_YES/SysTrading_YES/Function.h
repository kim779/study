#ifndef __DECLAREDEFINE_PARSER__
#define __DECLAREDEFINE_PARSER__

int CnvtBufInit();
int CnvtBufClose();
int CnvtBufInsert(FILE *ptpFp, char *psFormat, ...);
int CnvtBufWriteFile(int piMethod);

#endif
