(* Lista 4 -  ,  *)

type 'a bt = Empty | Node of 'a * 'a bt * 'a bt;;

let t2 = Node(1,Node(2,Empty,Node(3,Empty,Empty)),Empty);;
let t1 = Node(-1,Empty,Empty)
let t0 = Empty;;

(* Zadanie 1 *)

let rec sumBT = function
    Empty -> 0
  | Node (i, l, r) -> i + sumBT l + sumBT r;;

sumBT t0;;
sumBT t1;;
sumBT t2;;

(* Zadanie 2 *)

let rec foldBT f acc = function
    Empty -> acc
  | Node (i, l, r) -> f i (foldBT f acc l, foldBT f acc r);;


(* Zadanie 3 *)

(* a *)

let sumBTfold = foldBT (fun i (suma_l,suma_r) -> i+suma_l+suma_r) 0;;

sumBTfold t0;;
sumBTfold t1;;
sumBTfold t2;;

(* b *)

let inorderBTfold = foldBT (fun x (list_left,list_right) -> list_left @ x::list_right) [];;

inorderBTfold t2;;
inorderBTfold t1;;
inorderBTfold t0;;


(* Zadanie 4 *)

let rec mapBT f = foldBT (fun value (left_subtree, right_subtree) -> Node (f value, left_subtree, right_subtree)) Empty;;

mapBT (fun v -> 2*v) t0;;
mapBT (fun v -> 2*v) t1;;
mapBT (fun v -> 2*v) t2;;


type 'a tree = L of 'a | N of 'a tree * 'a tree;;

let t2 = N(L 1, N(N(L 2, L 3), L 4));;
let t1 = N(L (-1), L 0)
let t0 = L 7;;


let rec store = function
    L value -> [Some value]
  | N (left_subtree, right_subtree) -> (store left_subtree) @ (store right_subtree) @ [None];; 

store t0;;
store t1;;
store t2;;


(* Zadanie 6 *)

let load opt_list =
  let rec load_rev = function
      (Some value) :: tail -> (L value, tail)
    | None :: tail -> let (right_subtree, tail_1) = load_rev tail
                      in let (left_subtree, tail_2) = load_rev tail_1
                         in (N(left_subtree, right_subtree), tail_2)
    | _ -> failwith "Load"
  in let (result_tree, remaining_list) = load_rev (List.rev opt_list)
     in if remaining_list <> []
        then failwith "Load"
        else result_tree;;

load (store t0);;
load (store t1);;
load (store t2);;

load [Some 'a'; Some 'b'; None];;
load [Some 'a'; Some 'b'; Some 'c'; None; None];;

load [];;
load [None];;
load [None; None];;
load [None; None; None];;
load [Some 'a'; None];;
load [None; Some 'a'];;
load [Some 'a'; Some 'b'];;
load [Some 'a'; Some 'b'; Some 'c'; None];;
