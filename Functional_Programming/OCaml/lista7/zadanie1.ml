(* Lista 7 -  ,  *)

#use "queue_fun_type.ml";;

(* a *)

module Queue : QUEUE_FUN =
  struct
    type 'a t = EmptyQueue | Enqueue of 'a * 'a t
    exception Empty of string

    let create () = EmptyQueue
    let rec enqueue elem = function
        EmptyQueue -> Enqueue(elem, EmptyQueue)
      | Enqueue(head, tail) -> Enqueue(head, enqueue elem tail)

    let dequeue = function
        EmptyQueue -> raise (Empty "module Queue: dequeue")
      | Enqueue(head, tail) -> tail

    let first = function
        EmptyQueue -> raise (Empty "module Queue: first")
      | Enqueue(head, tail) -> head

    let isEmpty queue = queue=EmptyQueue
  end;;


(* b *)

module Queue : QUEUE_FUN =
  struct
    type 'a t = 'a list

    let create () = []
    let rec enqueue (elem, list) = match list with
        [] -> [elem]
      | head::tail -> head::(enqueue (elem,tail))

    let dequeue = function
        [] -> raise (Empty "module Queue: dequeue")
      | head::tail -> tail

    let first = function
        [] -> raise (Empty "module Queue: first")
      | head::tail -> head

    let isEmpty queue = queue=[]
  end;;


(* c *)

module Queue : QUEUE_FUN =
  struct
    type 'a t = 'a list * 'a list

    let create () = ([],[])
    let rec enqueue elem (xs, ys) =
      if xs=[] then ([elem],[]) else (xs, elem::ys)

    let dequeue = function
        ([], _) -> raise (Empty "module Queue: dequeue")
      | ([x], ys) -> (List.rev ys, [])
      | (x::xs, ys) -> (xs, ys)

    let first = function
        ([],_) -> raise (Empty "module Queue: first")
      | (x::xs, _) -> x

    let isEmpty queue = queue=([], [])
  end;;
