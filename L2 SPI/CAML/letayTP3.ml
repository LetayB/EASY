(* Letay benoit *)
(* 12/02 TP 3 , groupe de TP 5 *)

(* type 'a liste = vide | cons of  'a* 'a liste;; *)

let lire_car =function nom ->
 let canal=open_in nom in
  let rec lirea =function  () -> 
         try let a = input_char canal in 
            cons (a ,lirea ())  with
       End_of_file -> begin close_in canal;vide end
      in    lirea ();;

let is_sep = function ` `| `\n` -> true
                     | _ -> false;;


let rec saute_mot = function vide -> vide
        | cons(car,reste) as l -> if is_sep car then l
                  else saute_mot reste;;

let char2str = function car -> make_string 1 car;;

let rec  premier_mot = function vide -> ""
     | cons(car,reste) -> if is_sep car then ""
                else char2str car ^  premier_mot reste;;

let rec analyselex = function vide->vide
  |cons (car,reste) as l -> if is_sep car then
            analyselex reste
    else cons(premier_mot l , analyselex (saute_mot l));;

let tout= function nom -> analyselex(lire_car nom);; 

(*Question 1*)



let rec ajoute = function (el,vide) -> cons ((el,1),vide)
		| (el,cons(tete,reste)) -> if el = fst(tete) then cons((el,(1 + snd(tete))),reste)
		else cons(tete,ajoute(el,reste));;
		
let rec transfo = function vide -> vide
		| cons(tete,reste) -> ajoute(tete,transfo(reste));;

let max2 = function (a,b) -> 
		if  a >  b 
			then a 
			else b ;;

let rec search = function vide -> failwith "impossible"
	    | cons(tete,reste) -> if reste = vide then tete
			else max2(tete,search(reste));;
			

