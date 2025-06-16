#ifndef NANOSHELL_H
#define NANOSHELL_H
// add new command or useful here
typedef enum
{
	// commands
    HELP = 0,
    REGISTERS,
    TIME,
    ELIMINATOR,
    ECHO,
    CLEAR,
    CHANGE_FONT,
    NANO_SONG,
    TEST_ZERO_DIVISION,
    TEST_INVALID_OPCODE,
	TEST_MALLOC,
	MAN,
	TODO,
	FUNCTIONS,
	MINI_PROCESS,
	TEST_PRIORITY,
	SH,
	MEM,
	PS,
	LOOP,
	KILL,
	NICE,
	BLOCK,
	CAT,
	WC,
	FILTER,
	PHYLO,
	// useful
	MALLOC,
	REALLOC,
	CALLOC,
	FREE,
	CREATE_PROCESS,
	GET_PRIORITY,
	SET_PRIORITY,

	// leave this at the end
	INSTRUCTION_COUNT,
} INSTRUCTION;

void startNanoShell();

#endif