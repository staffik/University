(* Lista 2 -  ,  *)


(* Zadanie 1 *)

let rec fib1 n =
  if n<0 then failwith("n must be nonnegative")
  else if n<2 then n
  else fib1 (n-2) + fib1 (n-1);;

fib1 42;;
fib1 0;;
fib1 (-1);;
fib1 5;;

let fib2 n =
  if n<0 then failwith("n must be nonnegative")
  else
    let rec fibtmp f1 f2 n =
      if n=0 then f1
      else fibtmp f2 (f2+f1) (n-1)
    in fibtmp 0 1 n;;

fib2 42;;
fib2 0;;
fib2 (-1);;
fib2 5;;

(* Zadanie 2 *)

let (<->) (x0,y0,z0) (x1,y1,z1) =
  let x=x1-.x0 and y=y1-.y0 and z=z1-.z0
  in sqrt (x*.x+.y*.y+.z*.z);;

(1.,1.,1.) <-> (1.,1.,0.);;
(0.,0.,0.) <-> (0.,0.,0.);;
(-1.,0.,0.) <-> (0.,0.,0.);;


(* Zadanie 3 *)

let rec (<--) list x =
  match list with
    [] -> [x]
  | h::t ->
     if x<=h then  x::list
     else h :: (t <-- x);;

[1;3;5;5;7] <-- 2;;
[1;2;3] <-- 4;;
[1;1] <-- 1;;
[] <-- 1;;
[2;3] <-- 1;;


(* Zadanie 4 *)

let rec take k list =
  match (k,list) with
     (x,_) when x<=0 -> []
   | (_,[]) -> []
   | (_,h::t) -> h::take (k-1) t;;

take 2 [1;2;3;5;6];;
take (-2) [1;2;3;5;6];;
take 8 [1;2;3;5;6];;
take 0 [];;
take 5 [];;
take (-1) [];;
take 4 [1;2;3;4];;


(* Zadanie 5 *)

let rec drop k list =
  match (k,list) with
    (i,_) when i<=0 -> list
  | (_,[]) -> []
  | (_,h::t) -> drop (k-1) t;;

drop 2 [1;2;3;5;6];;
drop (-2) [1;2;3;5;6];;
drop 8 [1;2;3;5;6];;
drop 0 [];;
drop 5 [];;
drop (-1) [];;
drop 4 [1;2;3;4];;


(* Zadanie 6 *)

let rec repeatList list =
  let rec repeatElem x k =
    if k=0 then []
    else x :: repeatElem x (k-1)
  in match list with
       [] -> []
     | h::t -> repeatElem h h @ repeatList t;;

repeatList [1;0;4;3];;
repeatList [0;0;0;0];;
repeatList [2;2];;
repeatList [1];;
repeatList [0];;
repeatList [];;
