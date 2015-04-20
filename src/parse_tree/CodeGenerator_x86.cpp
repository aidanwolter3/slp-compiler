#include "CodeGenerator_x86.h"

CodeGenerator_x86::CodeGenerator_x86() {

  //initialize the code
  code = (char*)malloc(2000*sizeof(char));
  len = 0;

  //set that no registers are in use
  regs.eax = 0;
  regs.ebx = 0;
  regs.ecx = 0;
  regs.edx = 0;

  //add the necessary functions for printing an integer
  len += sprintf(code+len, "global start\n"
                            "section .text\n");
  len += sprintf(code+len, "\nputint:\n"
                              "push eax\n"
                              "push ebx\n"
                              "push ecx\n"
                              "push edx\n"
                              "mov eax,dword[esp+20]\n"
                              "mov ecx,0\n"
                              "push 0\n"
                              "push 0x0A\n"
                              "mov ebx,10\n"
                              ".putint0:\n"
                              "add ecx,1\n"
                              "mov dx,0\n"
                              "div bx\n"
                              "add edx,0x30\n"
                              "push edx\n"
                              "cmp ax,0\n"
                              "jz .putint1\n"
                              "jmp .putint0\n"
                              ".putint1:\n"
                              "push dword esp\n"
                              "call putstr\n"
                              "add esp,4\n"
                              "lea ecx,[8+4*ecx]\n"
                              "add esp,ecx\n"
                              "pop edx\n"
                              "pop ecx\n"
                              "pop ebx\n"
                              "pop eax\n"
                              "ret\n");
  len += sprintf(code+len, "\nputstr:\n"
                              "push eax\n"
                              "mov esi,dword[esp+8]\n"
                              ".putstr0:\n"
                              "mov byte al,[esi]\n"
                              "cmp al,0\n"
                              "je .putstr1\n"
                              "push eax\n"
                              "call putchar\n"
                              "add esp,4\n"
                              "add esi,4\n"
                              "jmp .putstr0\n"
                              ".putstr1:\n"
                              "pop eax\n"
                              "ret\n");
  len += sprintf(code+len, "\nputchar:\n"
                              "push dword [esp+4]\n"
                              "mov edx,esp\n"
                              "push dword 1\n"
                              "push dword edx\n"
                              "push dword 1\n"
                              "mov eax,4\n"
                              "sub esp,4\n"
                              "int 0x80\n"
                              "add esp,20\n"
                              "ret\n");
  len += sprintf(code+len, "\nstart:\n"
                              "mov ebp,esp\n"
                              "sub esp,%d\n", symbol_table_get_size()*4);
}
const char* CodeGenerator_x86::next_reg() {
  if(regs.eax == 0) {
    const char *eax = "eax"+0;
    regs.eax = 1;
    return eax;
  }
  if(regs.ebx == 0) {
    const char *ebx = "ebx"+0;
    regs.ebx = 1;
    return ebx;
  }
  if(regs.ecx == 0) {
    const char *ecx = "ecx"+0;
    regs.ecx = 1;
    return ecx;
  }
  if(regs.edx == 0) {
    const char *edx = "edx"+0;
    regs.edx = 1;
    return edx;
  }
  const char *noreg = ""+0;
  return noreg;
}
void CodeGenerator_x86::release_reg(char *reg) {
  const char *eax = "eax"+0;
  const char *ebx = "ebx"+0;
  const char *ecx = "ecx"+0;
  const char *edx = "edx"+0;
  if(strcmp(reg, eax) == 0) {
    regs.eax = 0;
  }
  else if(strcmp(reg, ebx) == 0) {
    regs.ebx = 0;
  }
  else if(strcmp(reg, ecx) == 0) {
    regs.ecx = 0;
  }
  else if(strcmp(reg, edx) == 0) {
    regs.edx = 0;
  }
}
void CodeGenerator_x86::print_code() {
  len += sprintf(code+len, "add esp,%d\n"
                           "\n;exit\n"
                           "push dword 0\n"
                           "mov eax,1\n"
                           "sub esp,12\n"
                           "int 0x80\n"
                           "add esp,4\n", symbol_table_get_size()*4);

  FILE *f = fopen("output.asm", "w");
  printf("%s", code);
  fprintf(f, "%s", code);
  fclose(f);
}
void* CodeGenerator_x86::visit(CompoundStatement *s) {
  CodeReturn *c1 = (CodeReturn*)s->stm1->accept(this);
  CodeReturn *c2 = (CodeReturn*)s->stm2->accept(this);
  return new CodeReturn(0, 0);
}
void* CodeGenerator_x86::visit(AssignStatement *stm) {
  CodeReturn *c1 = (CodeReturn*)stm->id->accept(this);
  CodeReturn *c2 = (CodeReturn*)stm->exp->accept(this);

  len += sprintf(code+len, "mov %s,%s\n", c1->tmp, c2->tmp);

  release_reg(c2->tmp);

  return new CodeReturn(0, 1);
}
void* CodeGenerator_x86::visit(PrintStatement *stm) {
  CodeReturn *c = (CodeReturn*)stm->list->accept(this);

  len += sprintf(code+len, "push dword %s\n", c->tmp);
  len += sprintf(code+len, "call putint\n");

  release_reg(c->tmp);

  return new CodeReturn(0, 2);
}
void* CodeGenerator_x86::visit(IdExpression *exp) {
  CodeReturn *c = new CodeReturn(0, 3);
  int offset = symbol_table_get_symbol_loc(exp->lexem);
  sprintf(c->tmp, "[ebp-%d]", offset);
  return c;
}
void* CodeGenerator_x86::visit(NumExpression *exp) {
  const char *reg = next_reg();
  len += sprintf(code+len, "mov %s,%d\n", reg, exp->val);

  CodeReturn *c = new CodeReturn(4, 4);
  strcpy(c->tmp, reg);
  return c;
}
void* CodeGenerator_x86::visit(OperationExpression *exp) {
  CodeReturn *c1 = (CodeReturn*)exp->exp1->accept(this);
  CodeReturn *c2 = (CodeReturn*)exp->op->accept(this);
  CodeReturn *c3 = (CodeReturn*)exp->exp2->accept(this);

  if(c2->type == 9) {
    len += sprintf(code+len, "add %s,%s\n", c1->tmp, c3->tmp);
  }
  else if(c2->type == 10) {
    len += sprintf(code+len, "sub %s,%s\n", c1->tmp, c3->tmp);
  }
  else if(c2->type == 11) {
    len += sprintf(code+len, "push eax\n"
                             "push edx\n"
                             "mov eax,%s\n"
                             "div dword %s\n"
                             "push %s\n"
                             "add esp,4\n"
                             "pop edx\n"
                             "pop eax\n"
                             "mov %s,[esp-12]\n", c1->tmp, c3->tmp, c1->tmp, c1->tmp);
  }
  else if (c2->type == 12) {
    len += sprintf(code+len, "push eax\n"
                             "push edx\n"
                             "mov eax,%s\n"
                             "mul dword %s\n"
                             "push %s\n"
                             "add esp,4\n"
                             "pop edx\n"
                             "pop eax\n"
                             "mov %s,[esp-12]\n", c1->tmp, c3->tmp, c1->tmp, c1->tmp);
  }

  release_reg(c3->tmp);

  CodeReturn *c = new CodeReturn(0, 5);
  strcpy(c->tmp, c1->tmp);
  return c;
}
void* CodeGenerator_x86::visit(SequenceExpression *exp) {
  CodeReturn *c1 = (CodeReturn*)exp->stm->accept(this);
  CodeReturn *c2 = (CodeReturn*)exp->exp->accept(this);
  CodeReturn *c = new CodeReturn(0, 6);
  strcpy(c->tmp, c2->tmp);
  return c;
}
void* CodeGenerator_x86::visit(PairExpressionList *exp) {
  CodeReturn *c1 = (CodeReturn*)exp->exp->accept(this);
  CodeReturn *c2 = (CodeReturn*)exp->list->accept(this);
  CodeReturn *c = new CodeReturn(0, 7);
  strcpy(c->tmp, c2->tmp);
  return c;
}
void* CodeGenerator_x86::visit(LastExpressionList *exp) {
  CodeReturn *c1 = (CodeReturn*)exp->exp->accept(this);
  CodeReturn *c = new CodeReturn(0, 8);
  strcpy(c->tmp, c1->tmp);
  return c;
}
void* CodeGenerator_x86::visit(Plus *op) {
  return new CodeReturn(0, 9);
}
void* CodeGenerator_x86::visit(Minus *op) {
  return new CodeReturn(0, 10);
}
void* CodeGenerator_x86::visit(Divide *op) {
  return new CodeReturn(0, 11);
}
void* CodeGenerator_x86::visit(Multiply *op) {
  return new CodeReturn(0, 12);
}
