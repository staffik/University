(* Lista 6 -  ,  *)

#load "str.cma";;

(* Zadanie 1 *)

let zgadnij () =
  Random.self_init();
  let n = Random.int 101 and gameOver = ref false
  in while not !gameOver
     do       
       print_endline "podaj liczbe od 0 do 100";
       let guess = read_int() in
       if guess < n then print_endline "moja jest wieksza"
       else if guess > n then print_endline "moja jest mniejsza"
       else begin
           print_endline "Zgadles. Brawo!";
           gameOver := true
         end
     done;;


(* Zadanie 2 *)

type 'a bt = Empty | Node of 'a * 'a bt * 'a bt;;

let printBT =
  let rec repeat_dots = function
    0 -> ()
  | n -> begin
          print_string ".";
          repeat_dots (n-1)
         end
  in let rec helper depth = function
       Empty ->
        begin
            repeat_dots (3*depth);
            print_string "||";
            print_newline()
        end
     | Node(value, left_subtree, right_subtree) ->
        begin
					 helper (depth+1) right_subtree;
           repeat_dots (3*depth);
           print_int value;
           print_newline();
           helper (depth+1) left_subtree
        end
   in helper 0;;

let t = Node(1, Node(2, Empty, Node(3, Empty, Empty)), Empty);;


(* Zadanie 3 *)

let sortuj_plik () = 
  let rec floats_of_string s = List.map float_of_string (Str.split (Str.regexp " ") s) and
  save_to_array idx numbers arr = match numbers with
      [] -> ()
    | h::t -> begin
                arr.(idx) <- h;
                save_to_array (idx+1) t arr
              end
  and write_array idx n out arr = if idx=n then ()
                                      else
                                        begin
                                          let str_number = Printf.sprintf "%f " arr.(idx) in
                                          output_string out str_number;
                                          write_array (idx+1) n out arr
                                        end
  in begin
    print_string "Podaj nazwe pliku wejsciowego: ";
    let filename = read_line() in
    let input = open_in filename in
    let n = int_of_string (input_line input) in
    let numbers = floats_of_string (input_line input) in
    let arr = Array.make n 0.0 in
    begin
      save_to_array 0 numbers arr;
      Array.sort compare arr;
      print_string "Podaj nazwe pliku wyjsciowego: ";
      let outfilename = read_line() in
      let output = open_out outfilename in
      write_array 0 n output arr;
      flush output
    end
  end
