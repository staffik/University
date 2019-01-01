(* Lista 3 - ,   *)

  
(* Zadanie 1 *)

(* a *)

let rec czyIstnieje p = function
  [] -> false
| h::t -> if p h
          then true
          else czyIstnieje p t;;

czyIstnieje (function x -> x=2) [1;2;3;5];;
czyIstnieje (function x -> x=4) [1;2;3;5];;
czyIstnieje (function x -> x=2) [];;

(* b *)

let czyIstnieje p xs = List.fold_left (||) false @@ List.map p xs;;

czyIstnieje (function x -> x=2) [1;2;3;5];;
czyIstnieje (function x -> x=4) [1;2;3;5];;
czyIstnieje (function x -> x=2) [];;

(* c *)

let czyIstnieje p xs = List.fold_right (||) (List.map p xs) false;;

czyIstnieje (function x -> x=2) [1;2;3;5];;
czyIstnieje (function x -> x=4) [1;2;3;5];;
czyIstnieje (function x -> x=2) [];;


(* Zadanie 2 *)

let filter predicat list = List.fold_right
                             (fun value acc -> if predicat value then value::acc else acc)
                             list [];;

filter (fun x -> x>2) [1;2;3;5];;
filter (fun x -> x>2) [];;


(* Zadanie 3 *)

(* a *)

let rec usun1 predicat = function
    [] -> []
  | h::t -> if predicat h
            then t
            else h::usun1 predicat t;;

usun1 (function x -> x=2) [1;2;3;2;5];;
usun1 (function x -> x=2) [1;3;3;5];;
usun1 (function x -> x=2) [];;

(* b *)

let usun1 predicat =
  let rec usun1_tmp acc = function
      [] -> List.rev acc
    | h::t -> if predicat h
              then List.rev_append acc t
              else usun1_tmp (h::acc) t
  in usun1_tmp [];;

usun1 (function x -> x=2) [1;2;3;2;5];;
usun1 (function x -> x=2) [1;3;3;5];;
usun1 (function x -> x=2) [];;


(* Zadanie 4 *)

let mergesort comparator list_to_sort =
  let rec partition acc = function
      (0, list) -> (List.rev acc, list)
    | (n,h::t) -> partition (h::acc) (n-1,t)
    | _ -> failwith "index out of bound"
  and merge xs ys =
    match (xs,ys) with
      ([],list) -> list
    | (list,[]) -> list
    | (hx::tx,hy::ty) -> if comparator hy hx
                         then hy::merge xs ty
                         else hx::merge tx ys
  and msort list size =
    if size<2 then list
    else let half=size/2
         in let (ls,rs)=partition [] (half,list)
            in merge (msort ls half) (msort rs (size-half))
  in msort list_to_sort (List.length list_to_sort);;

mergesort (<) [3;2;5;2;4];;
mergesort (<) [1;1;1];;
mergesort (<) ['d';'c';'b';'a'];;
mergesort (fun (x0,x1) (y0,y1) -> x0<y0) [(1,1);(2,1);(1,2);(2,2);(1,3);(2,3)];;


(* Zadanie 5 *)

type 'a binTree = Node of 'a * 'a binTree * 'a binTree | Empty;;
type 'a set = { bST : 'a binTree; lessThan : 'a -> 'a -> bool; };;


let emptySet comparator = {bST=Empty; lessThan=comparator};;

let isSetEmpty set = set.bST=Empty;;

let contains value set =
  let rec contains_tmp = function
      Empty -> false
     |Node(node_value, left_subtree, right_subtree) ->
       if node_value=value then true
       else if set.lessThan value node_value
       then contains_tmp left_subtree
       else contains_tmp right_subtree
  in contains_tmp set.bST;;

let include2set value set =
  let rec include_tmp = function
      Empty -> Node(value, Empty, Empty)
    | Node(node_value, left_subtree, right_subtree) as current_subtree ->
       if value=node_value then current_subtree
       else if set.lessThan value node_value
       then Node(node_value, include_tmp left_subtree, right_subtree)
       else Node(node_value, left_subtree, include_tmp right_subtree)
  in {bST=include_tmp set.bST; lessThan=set.lessThan};;

let filterSet predicate set =
  let rec filter_tmp acc = function
      Empty -> acc
    | Node(value, left_subtree, right_subtree) ->
       let subtrees_filtered = filter_tmp (filter_tmp acc left_subtree) right_subtree in
       if predicate value
       then (include2set value {bST=subtrees_filtered; lessThan=set.lessThan}).bST
       else subtrees_filtered
  in {bST=filter_tmp Empty set.bST; lessThan=set.lessThan};;

let unionSet setA setB =
  let rec unionSetTmp acc = function
      Empty -> acc
    | Node(value, left_subtree, right_subtree) -> include2set value (unionSetTmp (unionSetTmp acc left_subtree) right_subtree)
  in unionSetTmp setA setB.bST;;

let remove value set =
  let rec removeTmp acc = function
      Empty -> acc
    | Node(node_value, left_subtree, right_subtree) ->
       let left_set = {bST=left_subtree; lessThan=set.lessThan} and right_set = {bST=right_subtree; lessThan=set.lessThan} in
       if node_value=value
       then unionSet acc (unionSet left_set right_set)
       else if node_value<value
       then include2set node_value (unionSet left_set (removeTmp acc right_subtree))
       else include2set node_value (unionSet (removeTmp acc left_subtree) right_set)
  in removeTmp {bST=Empty; lessThan=set.lessThan} set.bST;;

let rec maxElem set = match set.bST with
    Empty -> None
  | Node(value, _, Empty) -> Some value
  | Node(_, _, right_subtree) -> maxElem {bST=right_subtree; lessThan=set.lessThan};;

let descendingElements set =
  let rec descTmp acc_list = function
      Empty -> acc_list
    | Node(value, left_subtree, right_subtree) -> descTmp (value::(descTmp acc_list left_subtree)) right_subtree
  in descTmp [] set.bST;;

let setSize set =
  let rec sizeTmp = function
      Empty -> 0
    | Node(_, left_subtree, right_subtree) -> 1 + sizeTmp left_subtree + sizeTmp right_subtree
  in sizeTmp set.bST;;

let testSet() =
  let set0 = emptySet (<)
  in let set1 = include2set "e" set0
     in let set2 = include2set "a" set1
        in let set3 = include2set "b" set2
           in let set4 = include2set "f" set3
              in let set5 = include2set "g" set3
                 in
                 begin
                   print_endline "isSetEmpty: on set0";
                   assert(isSetEmpty set0);
                   print_endline "isSetEmpty: on set4";
                   assert(not (isSetEmpty set4));
                   print_endline "setSize: on set0";
                   assert(setSize set0 = 0);
                   print_endline "setSize: on set4";
                   assert(setSize set4 = 4);
                   print_endline "contains: on set4";
                   assert(contains "b" set4);
                   print_endline "maxElem: on set4";
                   assert(maxElem set4 = Some "f");
                   print_endline "descendingElements: on set4";
                   assert(descendingElements set4 = ["f"; "e"; "b"; "a"]);
                   print_endline "filterSet: on set4";
                   assert(descendingElements(filterSet ((<) "b") set4) = ["f"; "e"]);
                   print_endline ("unionSet: on set4 and set5");
                   assert(descendingElements(unionSet set4 set5) = ["g"; "f"; "e"; "b"; "a"]);
                   print_endline ("unionSet: on set0 and set5");
                   assert(descendingElements(unionSet set0 set5) = ["g"; "e"; "b"; "a"]);
                   print_endline ("remove: b from set4");
                   assert(descendingElements(remove "b" set4) = ["f"; "e"; "a"])
                 end
;;
testSet();;
