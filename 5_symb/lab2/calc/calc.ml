open Pl
open Interpreter

let rec mainLoop env =
  try 
    let newEnv =
      try 
	let line = print_string "% "; read_line () in
	let stmt = parseStmt line in
	interpret env stmt
      with
	  End_of_file -> raise End_of_file
	| e -> Printf.printf "%s\n" (Printexc.to_string e); env
    in mainLoop newEnv
  with End_of_file -> env
;;

mainLoop (emptyEnv ())
