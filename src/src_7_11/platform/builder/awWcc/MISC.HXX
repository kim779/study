//
// SYNTAX TABLE
//
struct _mapSyntax {
	char	token[32];
	WORD	flag;	
} mapSyntax[] = {
		{"GLOBAL DIVISION", 	TYPE_GLOBAL},
		{"FORM DIVISION",  	TYPE_FORM},
		{"RESOURCE DIVISION", 	TYPE_RESOURCE},
		{"FGRAPH DIVISION",	TYPE_GRAPH},
		{"{",			TYPE_BEGIN}, 
		{"}", 			TYPE_END},
		{"//",			TYPE_COMMENT}
};
#define	MAPSYNTAX	sizeof(mapSyntax) / sizeof(struct _mapSyntax)


struct _lineSyntax {
	char	token[16];
	BYTE	flag;
} lineSyntax[] = {
		{"ONSTART",	LTYPE_ONSTART},
		{"ONSEND",	LTYPE_ONSEND},
		{"ONRECEIVE",	LTYPE_ONRECEIVE},
		{"ONRECEIVE",	LTYPE_ONALERT},
		{"ONSERVICE",	LTYPE_ONSERVICE},
		{"ONFILE",	LTYPE_ONFILE},
		{"ONSELECT",	LTYPE_ONSELECT},
		{"ONTIMER",	LTYPE_ONTIMER},
		{"ONTIMERX",	LTYPE_ONTIMERX},
		{"ONFOCUS",	LTYPE_ONFOCUS},
		{"ONCLOSE",	LTYPE_ONCLOSE},
		{"ONDEVICE",	LTYPE_ONDEVICE},
		{"ONKEY",	LTYPE_ONKEY},
		{"ONTRIGGER",	LTYPE_ONTRIGGER},
		{"SCRIPT",	LTYPE_SCPBEGIN},
		{"ENDSCRIPT",	LTYPE_SCPEND},
		{"ONEDIT",	LTYPE_ONEDIT},
		{"ONCLICK",	LTYPE_ONCLICK},
		{"ONDBLCLK",	LTYPE_ONDBLCLK},
		{"ONCHANGE",	LTYPE_ONCHANGE},
		{"ONCHARCHANGE",LTYPE_ONCHAR},
		{"PROCEDURE",	LTYPE_PROC},
		{"ENDPROC",	LTYPE_PROCEND}
};
#define LINESYNTAX	sizeof(lineSyntax) / sizeof(struct _lineSyntax)

struct _cellSyntax {
	char	token[7];
	BYTE	flag;
} cellSyntax[] = {
		{"GRID",	1},
		{"TABLE",	2},
		{"SEMI",	3}	// delete
};
#define	CELLSYNTAX	sizeof(cellSyntax) / sizeof(struct _cellSyntax)
