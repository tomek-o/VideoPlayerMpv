struct LuaExample
{
	char* name;
	char* lua;
};
const LuaExample luaExamples [] =
{
	{
	"Lua basics / cheatsheet",
	
	"-- Lua introduction / cheatsheet\n"
	"\n"
	"-- This is single-line comment\n"
	"--[[\n"
	"   This is multi-line\n"
	"   comment.\n"
	"--]]\n"
	"\n"
	"-- Undefined variables return nil (not generating error)\n"
	"str = 'text'    -- Variable declaration/definition; global by default\n"
	"str = nil       -- Undefines \"str\" variable; Lua uses garbage collector\n"
	"\n"
	"print(\"\\n\\n##### Starting script #####\\n\\n\")\n"
	"\n"
	"-- Conditionals\n"
	"num = 5\n"
	"if num > 6 then\n"
	"    print('num is larger than 6\\n')   -- print output goes to tSIP log window\n"
	"elseif num == 23 then               -- equality operator: ==\n"
	"    print('num equals 23\\n')       -- also -> tSIP log window\n"
	"else\n"
	"\t-- local variable\n"
	"    local messageText = string.format(\"Num value = %d\\n\", num);\n"
	"    print(messageText)\n"
	"end\n"
	"\n"
	"if 0 then\n"
	"    print(\"Only nil and false values are interpreted as false condition\\n\")\n"
	"end\n"
	"\n"
	"-- Multi-line comments can be used as C's \"#if 0\"\n"
	"-- Remove one '-' from line below to disable this block\n"
	"---[[\n"
	"\tprint(\"Block of code - enabled\\n\")\n"
	"--]]\n"
	"\n"
	"-- Loops\n"
	"\n"
	"while num < 10 do\n"
	"\tprint(\"Incrementing num\\n\")\n"
	"\tnum = num + 1  -- No ++/-- or += type (shorthand) operators.\n"
	"end\n"
	"\n"
	"local sum = 0\n"
	"for i = 1, num do  -- Range includes both ends.\n"
	"\tsum = sum + i\n"
	"end\n"
	"print(string.format(\"Sum of numbers from 1 to %d: %d\\n\", num, sum))\n"
	"\n"
	"for j = 3, 1, -1 do\n"
	"\t-- \"..\" = concatenation operator\n"
	"\tprint(\"Counting down: \" .. j .. \"\\n\")\n"
	"end\n"
	"\n"
	"repeat\n"
	"\tnum = num - 1\n"
	"until num == 0\n"
	"\n"
	"-- Tables are indexed by default by integers (starting from 1)\n"
	"local myTable = {'first value', 'second value', 3, 4.76}\n"
	"print 'myTable values: '\n"
	"for i = 1, #myTable do  -- #myTable = table size\n"
	"\tif i ~= 1 then\n"
	"\t\tprint ', '\n"
	"\tend\n"
	"\tprint (myTable[i])\n"
	"end\n"
	"print '\\n'\n"
	"\n"
	"\n"
	"print(\"\\n\\n##### End of script #####\\n\\n\")"
	}
	,
	{
	"Beep",

	"-- ShowMessage(\"Press \\\"Break\\\" to stop\")\n"
	"pattern =\n"
	"{\n"
	"-- frequency, time\n"
	"{392 \t,350}, \n"
	"{392 \t,350}, \n"
	"{392 \t,350}, \n"
	"{311 \t,250}, \n"
	"{466 \t,250},\n"
	"{392 \t,350}, \n"
	"{311 \t,250}, \n"
	"{466 \t,250},\n"
	"{392 \t,700}, \n"
	"{587 ,350}, \n"
	"{587 ,350}, \n"
	"{587 ,350}, \n"
	"{622 ,250}, \n"
	"{466 \t,250},\n"
	"{369 ,350}, \n"
	"{311 \t,250}, \n"
	"{466 \t,250},\n"
	"{392 \t,700}, \n"
	"{784 \t,350}, \n"
	"{392 \t,250}, \n"
	"{392 \t,250},\n"
	"{784 \t,350}, \n"
	"{739 ,250}, \n"
	"{698 ,250},\n"
	"{659 ,250},\n"
	"{622 ,250},\n"
	"{659 ,500}, \n"
	"{0, 300},\n"
	"{415 \t,250},\n"
	"{0,100},\n"
	"{554 ,350}, \n"
	"{523 ,250}, \n"
	"{493 ,250},\n"
	"{466 ,250},\n"
	"{440 \t,250},\n"
	"{466 ,500}, \n"
	"{0,300},\n"
	"{311 ,250},\n"
	"{0,100},\n"
	"{369 ,350}, \n"
	"{311 ,250}, \n"
	"{392 \t,250},\n"
	"{466 ,350}, \n"
	"{392 \t,250}, \n"
	"{466 ,250},\n"
	"{587 ,700}, \n"
	"{784 \t,350}, \n"
	"{392 \t,250}, \n"
	"{392 \t,250}, \n"
	"{784 \t,350}, \n"
	"{739 ,250}, \n"
	"{698 ,250},\n"
	"{659 ,250},\n"
	"{622 ,250},\n"
	"{659 ,500}, \n"
	"{0,300},\n"
	"{415 \t,250},\n"
	"{0,100},\n"
	"{554 ,350}, \n"
	"{523 ,250}, \n"
	"{493 ,250},\n"
	"{466 ,250},\n"
	"{440 \t,250},\n"
	"{466 ,500},\n"
	"{0,300}, \n"
	"{311 ,250},\n"
	"{0,200}, \n"
	"{392 \t,250}, \n"
	"{311 ,250}, \n"
	"{466 ,250},\n"
	"{392 ,300}, \n"
	"{0,500},\n"
	"{311 ,250}, \n"
	"\n"
	"}\n"
	"\n"
	"local winapi = require(\"jsonedit_winapi\")\n"
	"\n"
	"for i = 1, #pattern do\n"
	"\twinapi.Beep(pattern[i][1], pattern[i][2])\n"
	"\tret = CheckBreak()\n"
	"\tSleep(100)\t-- process Win messages also\n"
	"\t-- break on user request\\n\"\n"
	"\tif ret ~= 0 then\n"
	"\t\tprint ('User break\\n')\n"
	"\t\tbreak\n"
	"\tend\n"
	"end\n"
	""
	}


};
