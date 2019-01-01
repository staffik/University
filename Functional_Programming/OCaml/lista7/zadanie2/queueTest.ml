
let menu (opt) =
  let numItems = Array.length opt-1
  in
    begin
      print_string "\n\n=================================================== \n";
      print_string opt.(0);print_newline();
      for i=1 to numItems do  print_int i; print_string (". "^opt.(i)); print_newline() done;
      print_string "\nSelect an option: ";
      flush stdout;
      let choice = ref (read_int())
      in 
	while !choice < 1 || !choice > numItems do 
	  print_string ("Choose number between 1 and " ^ string_of_int numItems ^ ": ");
	  choice := read_int();
	done; 
	!choice
    end
;;

let q = ref (Queue_mut.create 3);;
let menuItems = Array.make 6 "";;
let quit = ref false;;
let choice = ref 7;;

menuItems.(0) <- "Queue Operations (implementation on mutable array)";
menuItems.(1) <- "enqueue";
menuItems.(2) <- "dequeue";
menuItems.(3) <- "first";
menuItems.(4) <- "isEmpty";
menuItems.(5) <- "isFull";
menuItems.(6) <- "quit testing";
while not !quit do
  begin
    choice := menu(menuItems);
    match !choice with
	1 ->
	  begin
	    print_string "Queue item = ";
	    Queue_mut.enqueue (read_int(), !q);
	  end  
	| 2 ->
	  begin
	    Queue_mut.dequeue !q;
	  end  
      | 3 ->
	  begin
	    print_int (Queue_mut.first !q);
      print_string "\n"
	  end
      | 4 ->
	    print_string ("Queue is "^(if Queue_mut.isEmpty !q then "" else "not ")^"empty.\n");
      | 5 ->
	    print_string ("Queue is "^(if Queue_mut.isFull !q then "" else "not ")^"full.\n");
      | 6 ->
	    quit := true
      | _ ->
	    print_string "IMPOSSIBLE!!!\n"
  end
done
