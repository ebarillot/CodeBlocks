
#include <string.h>
#include "exploit.h"
#include "inout.h"

static int indent_abs = 0;
static char indent_elem[] = "  ";
#define L_indent_string   2000
static char indent_string[L_indent_string+1] = "";

static void makeIdentString (void);

void inFun (char* fun)
{
  makeIdentString();
  Print("%s{ IN[%d]  >>> %s()\n", indent_string, indent_abs, fun);
  indent_abs += 1;
  return;
}

void outFun (char* fun, int ret)
{
  indent_abs -= 1;
  makeIdentString();
  Print("%s} OUT[%d] <<< %s() [ret=%d]\n", indent_string, indent_abs, fun, ret);
  return;
}

static void makeIdentString (void)
{
  int i;
  memset (indent_string, 0, L_indent_string);
  for (i=0;i<indent_abs;i++) strcat(indent_string, indent_elem);
  return;
}

void resetInOut (void)
{
  Print(">>> resetInOut [%d] <<<\n",indent_abs);
  indent_abs = 0;
  return;
}
