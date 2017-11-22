(* Benoit Letay *)
(* TP numero 2, groupe de TP 5 31/01/14 *)

type 'a liste = vide | cons of  'a* 'a liste;;
let rec lignes = function canal ->
          try let une_ligne = input_line canal in
                cons(une_ligne, lignes canal)
          with End_of_file -> vide;;
let lire = function nom ->
       let canal = open_in nom in
          let resu = lignes canal in
               close_in canal; resu;;


(* Question 1 *)

let rec sum_l = function vide -> 0 
		| cons(tete,reste) -> tete + sum_l(reste);;
let rec int_of_string_l = function vide -> vide
		| cons(tete,reste) -> cons(int_of_string(tete),int_of_string_l(reste));;
		



(* Question 2 *)
type duo ={nom:string;nombre:int};;
let rec trans = function 
		vide -> vide
		| cons(s,cons(i,reste)) ->
			cons({nom = s ; nombre = int_of_string(i)},trans(reste))
		| _ -> failwith "duo imcomplet" ;;

		

(* Question 3 *)

let rec sum_duo = function (name,vide) -> 0
		| (name,cons(tete,reste)) ->
			if tete.nom = name then tete.nombre + sum_duo(name,reste) 
			else sum_duo(name,reste);;

(* Question 4 *)

let max_2 = function ( c1 , c2 ) ->
		if c1.nombre > c2.nombre 
			then c1
		else c2 ;;

let rec max_duo = function vide -> failwith "impossible"
		| cons(tete,vide) -> tete
		| cons(tete,reste) ->
			max_2( tete , max_duo(reste));;
		
		
		










