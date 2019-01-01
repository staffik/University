(* Lista 5 -  ,  *)

type 'a llist = LNil | LCons of 'a * (unit -> 'a llist);;
type 'a lBT = LEmpty | LNode of 'a * ('a lBT Lazy.t) * ('a lBT Lazy.t);;

let rec lfrom n = LCons (n, function () -> lfrom (n+1));;

let rec ltake = function
    (0, _) -> []
  | (_, LNil) -> []
  | (k, LCons(x,xf)) -> x:: (ltake (k-1, xf()));;

let rec toLazyList = function
    [] -> LNil
  | x::xs -> LCons(x, function () -> toLazyList xs);;

(* Zadanie 1 *)

let lFib =
  let rec fibGen = function
      (curr, next) -> LCons(curr,
                           fun () -> fibGen(next, curr+next))
  in fibGen(0,1);;

ltake(10, lFib);;
ltake(0, lFib);;


(* Zadanie 2 *)

let lrepeat f =
  let rec repeatElem elem k suffix =
    if k=0 then suffix
    else LCons(elem, fun () -> repeatElem elem (k-1) suffix)
  in let rec repeatTmp curr_idx = function
         LNil -> LNil
       | LCons(head, tail) -> LCons(head, fun () ->
                                          repeatElem head ((f curr_idx)-1) (repeatTmp (curr_idx+1) (tail())))
     in repeatTmp 0;;


ltake(10, lrepeat (fun i -> i+1) (toLazyList [0;1;2]));;
ltake(10, LNil);;
ltake(20, lrepeat (fun i -> i+1) lFib);;


(* Zadanie 3 *)

let sublist indices list =
  let rec sublistTmp curr_idx indices_to_remove lazy_list =
    match lazy_list with
      LNil -> LNil
    | LCons(value, lazy_tail) ->
       if indices_to_remove = [] then lazy_list
       else let next_idx=curr_idx+1 in
            if curr_idx <> List.hd indices_to_remove
            then LCons(value, fun () -> sublistTmp next_idx indices_to_remove (lazy_tail()))
            else sublistTmp next_idx (List.tl indices_to_remove) (lazy_tail())
  in sublistTmp 0 (List.sort_uniq compare indices) list;;      

ltake(10, sublist [1;4;7;2] (lfrom 10));;
ltake(10, sublist [] (lfrom 10));;
ltake(10, sublist [1;1;2;3;5;4;1] (toLazyList [0;1;2]));;


(* Zadanie 4 *)

let toLBST list =
  let rec helper = function
      [] -> LEmpty
    | value::tail -> LNode(value, lazy LEmpty, lazy (helper tail))
  in helper (List.sort_uniq compare list);;

let infixLBST lbst =
  let rec helper acc = function
      LEmpty -> acc
    | LNode(value, left_subtree, right_subtree) -> helper (value::(helper acc (Lazy.force left_subtree))) (Lazy.force right_subtree)
  in List.rev (helper [] lbst);;

infixLBST (toLBST []);;
infixLBST (toLBST [0]);;
infixLBST (toLBST [1; 0]);;
infixLBST (toLBST [3; 1; 0; 2; 4]);;
