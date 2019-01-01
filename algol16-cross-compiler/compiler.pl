% SWI-Prolog

parse :- parse(_,_,_,_,_,_,_,_).

lexer(Tokens) -->
   white_space,
   (  (  ":=",      !, { Token = tokAssgn }
      ;  ";",       !, { Token = tokSColon }
      ;  "(",       !, { Token = tokLParen }
      ;  ")",       !, { Token = tokRParen }
      ;  "+",       !, { Token = tokPlus }
      ;  "-",       !, { Token = tokMinus }
      ;  "*",       !, { Token = tokTimes }
      ;  "=",       !, { Token = tokEq }
      ;  "<>",      !, { Token = tokNeq }
      ;  "<=",      !, { Token = tokLeq }
      ;  "<",       !, { Token = tokLt }
      ;  ">=",      !, { Token = tokGeq }
      ;  ">",       !, { Token = tokGt }
      ;  ",",       !, { Token = tokComma }
      ;  digit(D),  !,
            number(D, N),
            { Token = tokNumber(N) }
      ;  letter(L), !, identifier(L, Id),
            {  member((Id, Token), [ (and, tokAnd),
                                     (begin, tokBeg),
                                     (call, tokCall),
                                     (div, tokDiv),
                                     (do, tokDo),
                                     (done, tokDone),
                                     (else, tokElse),
                                     (end, tokEnd),
                                     (fi, tokFi),
                                     (if, tokIf),
                                     (local, tokLocal),
                                     (mod, tokMod),
                                     (not, tokNot),
                                     (or, tokOr),
                                     (procedure, tokProc),
                                     (program, tokProg),
                                     (read, tokRead),
                                     (return, tokRet),
                                     (then, tokThen),
                                     (value, tokVal),
                                     (while, tokWhile),
                                     (write, tokWrite)]),
               !
            ;  Token = tokVar(Id)
            }
      ;  [_],
            { Token = tokUnknown }
      ),
      !,
         { Tokens = [Token | TokList] },
      lexer(TokList)
   ;  [],
         { Tokens = [] }
   ).

white_space -->
   [Char], { code_type(Char, space) }, !, white_space.
white_space -->
	"(*", !, comment, white_space.
white_space -->
   [].

comment -->
	"*)", !.
comment -->
	[_], comment.

digit(D) -->
   [D],
      { code_type(D, digit) }.

digits([D|T]) -->
   digit(D),
   !,
   digits(T).
digits([]) -->
   [].

number(D, N) -->
   digits(Ds),
      { number_chars(N, [D|Ds]) }.

letter(L) -->
   [L], { code_type(L, alpha) }.

alphanum([A|T]) -->
   [A], { code_type(A, csym) ; A == 0'' }, !, alphanum(T).
alphanum([]) -->
   [].

identifier(L, Id) -->
   alphanum(As),
      { atom_codes(Id, [L|As]) }.

:- op(990, xfy, ';;').
:- op(900, xfy, :=).
:- op(820, xfy, and).
:- op(840, xfy, or).
:- op(700, xfy, <=).
:- op(700, xfy, <>).

program(Ast) -->
	[tokProg], variable(_), block(Ast).

block(block(D, CI)) --> declarations(D), [tokBeg], compound_instruction(CI), [tokEnd].

declarations(decs(D,Ds)) --> declaration(D), !, declarations(Ds).
declarations(decs(empty)) --> [].

declaration(dec(D)) --> declarator(D), !.
declaration(dec(P)) --> procedure(P).

declarator(decl(Vs)) --> [tokLocal], variables(Vs).

variables(vars(Var,Vs)) --> variable(Var), [tokComma], variables(Vs), !.
variables(vars(Var,empty)) --> variable(Var).

variable(var(Id)) --> [tokVar(Id)].

procedure(proc(N,Args,B)) --> [tokProc], procedure_name(N), [tokLParen], formal_arguments(Args), [tokRParen], block(B).

procedure_name(proc_name(N)) --> tokVar(N).

formal_arguments(args(Arg, As)) --> formal_argument(Arg), [tokComma], formal_arguments(As), !.
formal_arguments(args(Arg, empty)) --> formal_argument(Arg).

formal_argument(arg(Id)) --> [tokValue], !, variable(Id).

compound_instruction(compound(Instr,Instrs)) --> instruction(Instr), [tokSColon], compound_instruction(Instrs), !.
compound_instruction(compound(Instr,empty)) --> instruction(Instr).

instruction(assign(Var,Arithmx)) --> variable(Var), [tokAssgn], arithm_expression(Arithmx), !.
instruction(if(Logicx,C_instr1)) --> [tokIf], bool_expr(Logicx), [tokThen], compound_instruction(C_instr1), [tokFi], !.
instruction(if(Logicx,C_instr1,C_instr2)) --> [tokIf], bool_expr(Logicx), [tokThen], compound_instruction(C_instr1), [tokElse], compound_instruction(C_instr2), [tokFi], !.
instruction(while(Logicx, C_instr)) --> [tokWhile], bool_expr(Logicx), [tokDo], compound_instruction(C_instr), [tokDone], !.
instruction(call(Proc_call)) --> [tokCall], procedure_call(Proc_call), !.
instruction(return(Arithmx)) --> [tokRet], arithm_expression(Arithmx), !.
instruction(read(Var)) --> [tokRead], variable(Var), !.
instruction(write(Arithmx)) --> [tokWrite], arithm_expression(Arithmx), !.

arithm_expression(Arithmx) --> component(C), arithm_exp1(C,Arithmx).

arithm_exp1(Acc, Arithmx) -->
	additive_op(Op), !, component(C),
	{NewAcc = arithm(Op,Acc,C), gensym('(*var*)',Temp)},
	arithm_exp1(pack(variable(Temp),NewAcc), Arithmx).

arithm_exp1(Arithmx, arithm_exp(Arithmx)) --> [].

additive_op(plus) --> [tokPlus], !.
additive_op(minus) --> [tokMinus].

component(C) --> factor(Factor), component(Factor, C).

component(Acc, C) -->
	multiplicative_op(Op), !, factor(Factor),
	{ NewAcc = arithm(Op, Acc, Factor), gensym('(*var*)',Temp)},
	component(pack(variable(Temp),NewAcc), C).

component(Acc, Acc) --> [].

multiplicative_op(times) --> [tokTimes], !.
multiplicative_op(div) --> [tokDiv], !.
multiplicative_op(mod) --> [tokMod].

factor(Expr) -->
	[tokMinus], !, factor(MinusExpr), { Expr = pack(variable(Temp),arithm(minus,MinusExpr)), gensym('(*var*)',Temp) }.
factor(Expr) -->
	[tokLParen], !, arithm_expression(arithm_exp(Expr)), [tokRParen].
factor(Expr) -->
	[tokNumber(N)], !, { Expr = pack(variable(Temp),constant(N)), gensym('(*var*)',Temp) }.
factor(Expr) -->
	[tokVar(Var)], { Expr = pack(variable(Var),variable(Var)) }.

bool_expr(Bool) -->
   disjunct(Disjunct), bool_expr(Disjunct, Bool).

bool_expr(Acc, Bool) -->
   [tokOr], !, disjunct(Disjunct),
   { Acc1 = bool(or, Acc, Disjunct), gensym('(*var*)',Temp)},
   bool_expr(pack(variable(Temp), Acc1), Bool).

bool_expr(Acc, bool_expression(Acc)) --> [].

disjunct(Disjunct) --> conjunct(Conjunct), disjunct(Conjunct, Disjunct).

disjunct(Acc, Disjunct) -->
   [tokAnd], !, conjunct(Conjunct),
      { Acc1 = bool(and, Acc, Conjunct), gensym('(*var*)',Temp)},
      disjunct(pack(variable(Temp),Acc1), Disjunct).

disjunct(Acc, Acc) --> [].

conjunct(Conjunct) -->
	[tokLParen], bool_expr(bool_expression(Conjunct)), [tokRParen], !.
conjunct(Conjunct) -->
	[tokNot], !, conjunct(NotConjunct), { Conjunct = pack(variable(Temp), bool(not, NotConjunct)), gensym('(*var*)',Temp) }.
conjunct(Conjunct) -->
	[tokTrue], !, { Conjunct = pack(variable(Temp),constant(1)), gensym('(*var*)',Temp) }.
conjunct(Conjunct) -->
	[tokFalse], !, { Conjunct = pack(variable(Temp),constant(0)), gensym('(*var*)',Temp) }.
conjunct(Conjunct) -->
	arithm_expression(arithm_exp(LExpr)), rel_op(Op), arithm_expression(arithm_exp(RExpr)),
	{gensym('(*var*)',Temp1), gensym('(*var*)',Temp2),
	Conjunct = pack(variable(Temp2), bool(Op, LExpr, RExpr)) }.

rel_op(eq) --> [tokEq], !.
rel_op(neq) --> [tokNeq], !.
rel_op(lt) --> [tokLt], !.
rel_op(leq) --> [tokLeq], !.
rel_op(gt) --> [tokGt], !.
rel_op(geq) --> [tokGeq].

rel_op(eq) :- !.
rel_op(neq) :- !.
rel_op(lt) :- !.
rel_op(leq) :- !.
rel_op(gt) :- !.
rel_op(geq).

encode(block(_,C), D, X) :- encode(C,D,X-[]).

encode(compound(A,empty),D,X) :- !, encode(A,D,X).
encode(compound(A,B),D,X-TY) :- encode(A,D,X-Y), encode(B,D,Y-TY).

encode(assign(var(Var),Arithmx), D, X-TY) :-
	encode(Arithmx, D, X-Y), lookup(Var,D,Adr), Y = [swapd, const(Adr), swapa, swapd, store | TY].
	
encode(read(var(Var)), D, X-TX) :-
	lookup(Var, D, Adr), X = [const(Adr), swapa, const(1), syscall, store | TX].
	
encode(write(Arithmx), D, X-TY) :-
	encode(Arithmx, D, X-Y), Y = [swapd, const(2), syscall | TY].

encode(arithm_exp(Exp), D, X) :- encode_arxp(Exp, D, X).

encode_arxp(pack(variable(Temp),variable(Temp)), D, X-TX) :-
	!, lookup(Temp, D, Adr), X = [const(Adr), swapa, load | TX].
encode_arxp(pack(variable(Temp),constant(N)), D, X-TX) :-
	!, lookup(Temp, D, Adr), X = [const(Adr), swapa, const(N), store | TX].

encode_arxp(pack(variable(Temp), arithm(Ar_op, pack(variable(Temp1),Exp1), pack(variable(Temp2),Exp2))), D, X-TZ) :-
	op_code(Ar_op, Op), !, lookup(Temp, D, Adr),
	encode_arxp(pack(variable(Temp1),Exp1), D, X-Y), lookup(Temp1, D, Adr1),
	encode_arxp(pack(variable(Temp2),Exp2), D, Y-Z), lookup(Temp2, D, Adr2),
	Z = [const(Adr2), swapa, load, swapd, const(Adr1), swapa, load, Op, swapd, const(Adr), swapa, swapd, store | TZ].

encode_arxp(pack(variable(Temp), arithm(minus, pack(variable(Temp1),Exp1))), D, X-TY) :-
	!, lookup(Temp, D, Adr), encode_arxp(pack(variable(Temp1),Exp1), D, X-Y), lookup(Temp1, D, Adr1),
	Y = [const(Adr1), swapa, load, swapd, const(-1), mul, swapa, const(Adr), swapa, store | TY].

encode_arxp(pack(variable(Temp), arithm(mod,pack(variable(Temp1),Exp1),pack(variable(Temp2),Exp2))), D, X-TZ) :-
	!, encode_arxp(pack(variable(Temp1),Exp1), D, X-Y), encode_arxp(pack(variable(Temp2),Exp2), D, Y-Z),
	lookup(Temp1, D, Adr1), lookup(Temp2, D, Adr2), lookup(Temp, D, Adr),
	Z = [const(Adr2), swapa, load, swapd, const(Adr1), swapa, load, div,
		const(-16), swapd, shift, swapd, const(Adr), swapa, swapd, store | TZ].

op_code(plus, add) :- !.
op_code(minus, sub) :- !.
op_code(times, mul) :- !.
op_code(div, div).

encode(if(bool_expression(pack(variable(Temp),Boolxp)), Comp_Instr), D, X-TV) :-
	encode(bool_expression(pack(variable(Temp),Boolxp)), D, X-Y), lookup(Temp, D, Adr),
	gensym('(*var*)', Label), lookup(Label, D, LabAdr),
	Y = [const(Adr), swapa, load, swapd, const(LabAdr), swapa, swapd, branchz | Z],
	encode(Comp_Instr, D, Z-V), V = [ label(LabAdr) | TV].

encode(if(bool_expression(pack(variable(Temp),Boolxp)), Comp_Instr1, Comp_Instr2), D, X-TP) :-
	encode(bool_expression(pack(variable(Temp),Boolxp)), D, X-Y), lookup(Temp, D, Adr),
	gensym('(*var*)', Label1), lookup(Label1, D, LabAdr1),
	gensym('(*var*)', Label2), lookup(Label2, D, LabAdr2),
	Y = [const(Adr), swapa, load, swapd, const(LabAdr1), swapa, swapd, branchz | Z],
	encode(Comp_Instr1, D, Z-V), V = [const(LabAdr2), jump, label(LabAdr1) | W],
	encode(Comp_Instr2, D, W-P), P = [label(LabAdr2) | TP].

encode(while(bool_expression(pack(variable(Temp),Boolxp)), Comp_Instr), D, X-TV) :-
	gensym('(*var*)', Label1), lookup(Label1, D, LabAdr1),
	gensym('(*var*)', Label2), lookup(Label2, D, LabAdr2),
	X = [label(LabAdr1) | Y],
	encode(bool_expression(pack(variable(Temp),Boolxp)), D, Y-Z), lookup(Temp, D, Adr),
	Z = [const(Adr), swapa, load, swapd, const(LabAdr2), swapa, swapd, branchz | V],
	encode(Comp_Instr, D, V-W), W = [const(LabAdr1), jump, label(LabAdr2) | TV].

encode(bool_expression(Exp), D, Code) :- encode_boolxp(Exp, D, Code).

encode_boolxp(pack(variable(Temp), bool(and, pack(variable(Temp1), Expr1), pack(variable(Temp2),Expr2))), D, X-TZ) :-
	lookup(Temp, D, Adr), lookup(Temp1, D, Adr1), lookup(Temp2, D, Adr2),
	encode_boolxp(pack(variable(Temp1), Expr1), D, X-Y),
	encode_boolxp(pack(variable(Temp2), Expr2), D, Y-Z),
	Z = [const(Adr2), swapa, load, swapd, const(Adr1), swapa, load, mul, swapd, const(Adr), swapa, swapd, store | TZ].

encode_boolxp(pack(variable(Temp), bool(or, pack(variable(Temp1), Expr1), pack(variable(Temp2),Expr2))), D, X-TZ) :-
	lookup(Temp, D, Adr), lookup(Temp1, D, Adr1), lookup(Temp2, D, Adr2),
	encode_boolxp(pack(variable(Temp1), Expr1), D, X-Y),
	encode_boolxp(pack(variable(Temp2), Expr2), D, Y-Z),
	Z = [const(Adr2), swapa, load, swapd, const(1), sub, swapd, const(Adr), swapa,
		 swapd, store, const(Adr1), swapa, load, swapd, const(1), sub, swapd, const(Adr),
		 swapa, load, mul, swapd, const(1), sub, swapd, const(Adr), swapa, swapd, store |TZ].

encode_boolxp(pack(variable(Temp1), bool(not, pack(variable(Temp2),Expr))), D, X-TY) :- 
	encode_boolxp(pack(variable(Temp2),Exp), D, X-Y),
	lookup(Temp1, D, Adr1), lookup(Temp2, D, Adr2),
	Y = [const(Adr2), swapa, load, swapd, const(1), add, swapd, const(2), swapd,
		 div, const(-6), swapd, shift, swapd, const(Adr1), swapa, swapd, store | TY].
	
encode_boolxp(pack(variable(Temp),constant(N)), D, X-TX) :-
	!, lookup(Temp, D, Adr), X = [const(Adr), swapa, const(N), store | TX].

encode_boolxp(pack(variable(Temp), bool(Op, pack(variable(Temp1), LExpr), pack(variable(Temp2), RExpr))), D, X-TV) :-
	lookup(Temp, D, Adr), rel_op(Op), lookup(Temp1, D, Adr1), lookup(Temp2, D, Adr2),
	encode_arxp(pack(variable(Temp1), LExpr), D, X-Y), encode_arxp(pack(variable(Temp2), RExpr), D, Y-Z),
	gensym('(*var*)',TempLabel1), lookup(TempLabel1, D, Adrx),
	gensym('(*var*)',TempLabel2), lookup(TempLabel2, D, Adry),
	Z = [const(Adr1), swapa, load, swapd, const(2), swapd, div, swapa, const(Adr), swapa, store,
		 const(Adr2), swapa, load, swapd, const(2), swapd, div, swapd, const(Adr), swapa, load,
		 sub, swapa, const(Adrx), swapa, branchz, swapd, const(Adry), jump, label(Adrx),
		 const(Adr2), swapa, load, swapd, const(Adr1), swapa, load, sub, swapd, label(Adry) | V],
	gensym('(*var*)',Label), lookup(Label, D, Adrz),
	encode_comparison(Op, Adr, Adrz, V-TV).

encode_comparison(eq, Adr, Label, X-TX) :-
	X = [const(Adr), swapa, const(1), store,
		const(Label), swapa, swapd, branchz, const(Adr),
		swapa, const(0), store, label(Label) | TX].

encode_comparison(neq, Adr, Label, X-TX) :-
	X = [const(Adr), swapa, const(0), store,
		const(Label), swapa, swapd, branchz, const(Adr),
		swapa, const(1), store, label(Label) | TX].

encode_comparison(lt, Adr, Label, X-TX) :-
	X = [const(Adr), swapa, const(1), store,
		const(Label), swapa, swapd, branchn, const(Adr),
		swapa, const(0), store, label(Label) | TX].

encode_comparison(geq, Adr, Label, X-TX) :-
	X = [const(Adr), swapa, const(0), store,
		const(Label), swapa, swapd, branchn, const(Adr),
		swapa, const(1), store, label(Label) | TX].

encode_comparison(gt, Adr, Label, X-TX) :-
	X = [const(Adr), swapa, const(1), store, const(Label),
		swapa, const(-1), mul, branchn, const(Adr),
		swapa, const(0), store, label(Label) | TX].

encode_comparison(leq, Adr, Label, X-TX) :-
	X = [const(Adr), swapa, const(0), store, const(Label),
		swapa, const(-1), mul, branchn, const(Adr),
		swapa, const(1), store, label(Label) | TX].

fill_nop([], []) :- !.

fill_nop([A, B, C, D | T1], [ [A, B, C, D] | T2]) :-
	not(leap(A)), not(leap(B)), not(leap(C)), !, fill_nop(T1, T2).

fill_nop([A, B, C | T1], [ [A, B, C, nop] | T2]) :-
	not(leap(A)), not(leap(B)), !, fill_nop(T1, T2).

fill_nop([A, B | T1], [ [A, B, nop, nop] | T2]) :-
	not(leap(A)), !, fill_nop(T1, T2).

fill_nop([A | T1], [ [A, nop, nop, nop] | T2]) :- fill_nop(T1, T2).

leap(jump).
leap(branchn).
leap(branchz).

expand_const([], []) :- !.
expand_const([ [label(X)] | T1], [ [label(X)] | T2]) :- !, expand_const(T1, T2).
expand_const([ [const(A), const(B), const(C), const(D)] | T1], [ [const, const, const, const], [A], [B], [C], [D] | T2]) :- !, expand_const(T1,T2).
expand_const([ [const(A), const(B), const(C), D] | T1], [ [const, const, const, D], [A], [B], [C] | T2]) :- !, expand_const(T1,T2).
expand_const([ [const(A), const(B), C, const(D)] | T1], [ [const, const, C, const], [A], [B], [D] | T2]) :- !, expand_const(T1,T2).
expand_const([ [const(A), B, const(C), const(D)] | T1], [ [const, B, const, const], [A], [C], [D] | T2]) :- !, expand_const(T1,T2).
expand_const([ [A, const(B), const(C), const(D)] | T1], [ [A, const, const, const], [B], [C], [D] | T2]) :- !, expand_const(T1,T2).
expand_const([ [const(A), const(B), C, D] | T1], [ [const, const, C, D], [A], [B] | T2]) :- !, expand_const(T1,T2).
expand_const([ [const(A), B, const(C), D] | T1], [ [const, B, const, D], [A], [C] | T2]) :- !, expand_const(T1,T2).
expand_const([ [A, const(B), const(C), D] | T1], [ [A, const, const, D], [B], [C] | T2]) :- !, expand_const(T1,T2).
expand_const([ [const(A), B, C, const(D)] | T1], [ [const, B, C, const], [A], [D] | T2]) :- !, expand_const(T1,T2).
expand_const([ [A, const(B), C, const(D)] | T1], [ [A, const, C, const], [B], [D] | T2]) :- !, expand_const(T1,T2).
expand_const([ [A, B, const(C), const(D)] | T1], [ [A, B, const, const], [C], [D] | T2]) :- !, expand_const(T1,T2).
expand_const([ [const(A), B, C, D] | T1], [ [const, B, C, D], [A] | T2]) :- !, expand_const(T1,T2).
expand_const([ [A, const(B), C, D] | T1], [ [A, const, C, D], [B] | T2]) :- !, expand_const(T1,T2).
expand_const([ [A, B, const(C), D] | T1], [ [A, B, const, D], [C] | T2]) :- !, expand_const(T1,T2).
expand_const([ [A, B, C, const(D)] | T1], [ [A, B, C, const], [D] | T2]) :- !, expand_const(T1,T2).
expand_const([ [A, B, C, D] | T1], [ [A, B, C, D] | T2]) :- !, expand_const(T1,T2).

pull_labels([],[]) :- !.

pull_labels([ [X] | T1], [ [X] | T2]) :- !, pull_labels(T1,T2).

pull_labels([ [label(A), B, C, D] | T1], [ [label(A)] | T2]) :-
	!, pull_labels([ [nop, B, C, D] | T1], T2).
	
pull_labels([ [A, label(B), C, D] | T1], [ [A, nop, nop, nop], [label(B)] | T2]) :- 
	!, pull_labels([ [nop, nop, C, D] | T1], T2).
	
pull_labels([ [A, B, label(C), D] | T1], [ [A, B, nop, nop], [label(C)] | T2]) :-
	!, pull_labels([ [nop, nop, nop, D] | T1], T2).
	
pull_labels([ [A, B, C, label(D)] | T1], [ [A, B, C, nop], [label(D)] | T2]) :-
	!, pull_labels(T1, T2).

pull_labels([ [A, B, C, D] | T1], [ [A, B, C, D] | T2]) :- pull_labels(T1, T2).

lookup(Key, [(Key, Val) | _], Val) :- !.

lookup(Key, [(Key1, Val1) | Dict], Val) :-
	not(Key == Key1), lookup(Key,Dict,Val).

unify_labels([], Pos, []) :- !.

unify_labels([[X] | T1], Pos, [ [nop, nop, nop, nop] | T2]) :-
	not(var(X)), not(number(X)), !, X = label(Y), Y = Pos,
	NextPos is Pos + 1, unify_labels(T1, NextPos, T2).

unify_labels([X | T1], Pos, [X | T2]) :-
	NextPos is Pos + 1, unify_labels(T1, NextPos, T2).

unify_names([], Nr, []) :- !.

unify_names([ [X] | T1], Nr, [ [X] | T2]) :-
	not(number(X)), !, X = Nr, NextNr is Nr + 1, unify_names(T1, NextNr, T2).

unify_names([X | T1], Nr, [X | T2]) :- unify_names(T1, Nr, T2).

convert_to_dec([], []) :- !.

convert_to_dec([ [X] | T1], [Y | T2]) :-
	X < 0, !, Y is 65536 + X, convert_to_dec(T1, T2).

convert_to_dec([ [X] | T1], [X | T2]) :-
	!, convert_to_dec(T1, T2).

convert_to_dec([ [A, B, C, D] | T1], [N | T2]) :-
	command(A, N1), command(B, N2), command(C, N3), command(D, N4),
	N is N4+16*(N3+16*(N2+16*N1)), convert_to_dec(T1, T2).

command(nop, 0) :- !.
command(syscall, 1) :- !.
command(load, 2) :- !.
command(store, 3) :- !.
command(swapa, 4) :- !.
command(swapd, 5) :- !.
command(branchz, 6) :- !.
command(branchn, 7) :- !.
command(jump, 8) :- !.
command(const, 9) :- !.
command(add, 10) :- !.
command(sub, 11) :- !.
command(mul, 12) :- !.
command(div, 13) :- !.
command(shift, 14) :- !.
command(nand, 15).

return_output(Stream, []) :- !.

return_output(Stream, [H | T]) :-
	 write(Stream, H),  nl(Stream), return_output(Stream, T). 

parse(_, CodeAp,CodeB,CodeC,CodeD,CodeE,CodeF,CodeG) :-
	set_prolog_flag(answer_write_options,[max_depth(100)]),
	open('input', read, Str),
	read_stream_to_codes(Str, CharCodeList),
	close(Str),
	phrase(lexer(TokList), CharCodeList),
	phrase(program(Ast), TokList),
	encode(Ast, Dict, CodeA),
	append(CodeA, [const(0), syscall], CodeAp),
	fill_nop(CodeAp, CodeB),
	pull_labels(CodeB, CodeC),
	expand_const(CodeC, CodeD),
	unify_labels(CodeD, 0, CodeE),
	length(CodeD, Len),
	unify_names(CodeE, Len, CodeF),
	convert_to_dec(CodeF, CodeG),
	open('output', write, Str2),
	return_output(Str2, CodeG),
	close(Str2).
