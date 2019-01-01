type 'a t = {mutable arr: 'a option array; mutable f:int; mutable r:int; mutable empty:bool}
exception Empty of string
exception Full of string

let create size = {arr=Array.make size None; f=0; r=0; empty=true}

let enqueue elem queue =
  if not queue.empty && queue.f=queue.r
    then
      raise (Full "module Queue_mut: enqueue")
  else
    begin
      queue.arr.(queue.r) <- Some elem;
      queue.r <- (queue.r+1) mod (Array.length queue.arr);
      queue.empty <- false
    end
  
let dequeue queue =
  if queue.empty
    then
      raise (Empty "module Queue_mut: dequeue")
    else
      begin
        queue.f <- (queue.f+1) mod (Array.length queue.arr);
        if queue.f = queue.r then queue.empty <- true;
      end

let first queue =
  if queue.empty then raise (Empty "module Queue_mut: first")
  else get queue.arr.(queue.f)

let isEmpty queue = queue.empty

let isFull queue = not queue.empty && queue.f = queue.r
