(* Lista1 -  ,  *)


(* Zadanie 1 *)

(* a *)
let funa (x, y) = (y+2, x+1);;
funa (1, 2);;
(* funa ("1", "2");; *)

(* b *)
let funb (x, y) = x+.1.0<y;;
funb (1.0, 2.0);;
(* funb (1, 2);; *)

(* c *)
let func (x, y) = let z=y+1 in x@x;;
func ("o"::"caml"::[], 4);;
(* func ("o"::"caml"::[], 4.0);; *)


(* Zadanie 2 *)

let ends x = if  x <> []
             then (List.hd x, List.hd (List.rev x))
             else failwith("The list is empty");;

ends [1;2;3;5];;
ends [];;
ends [1];;

(* Zadanie 3 *)

let rec isSorted list = if list=[] then true
                        else if List.tl list = []
                        then true
                        else let x = List.hd list and tail = List.tl list in
                             let y = List.hd tail in
                             x<=y && isSorted(tail);;

isSorted [1;3;3;5;6;7];;
isSorted [2;1];;
isSorted [1];;
isSorted [];;


(* Zadanie 4 *)

let powers (a,b) = if b<0 then failwith("Exponent must be nonnegative")
                   else let rec revPowers(x,y) = if y=0 then [1]
                                                 else let list = revPowers(x,y-1) in
                                                      let last = List.hd list in
                                                      (x*last)::list in
                        List.rev (revPowers (a,b));;

powers(2,3);;
powers(-2,3);;
powers(2,-1);;
powers(3,0);;


(* Zadanie 5 *)

let rec 
