(* Letay benoit *)
(* 26/02 TP 5 , groupe de TP 5 *)

type 'a liste = vide | cons of  'a* 'a liste;; 

type 'a arbre = V | N of 'a * ('a arbre) * ('a arbre);;

type terme = op of string | entier of int ;;


let lire_car =function nom ->
 let canal=open_in nom in
  let rec lirea =function  () -> 
         try let a = input_char canal in 
            cons (a ,lirea ())  with
       End_of_file -> begin close_in canal;vide end
      in    lirea ();;

let is_sep = function ` `| `\n` -> true
                     | _ -> false;;
					 


let char2str = function car -> make_string 1 car;;

let rec analysepost = function vide->vide
  |cons (x,reste) as l -> if is_sep x then
            analysepost reste
    else cons(x , analysepost (reste));;

let tout= function nom -> analysepost(lire_car nom);; 

let ch2terme = function
	("+"|"-"|"/"|"*") as ope -> op ope
	| v -> entier (int_of_string v);;
	
let rec trans = fun 
	f vide -> vide
	| f (cons(tete,reste)) -> cons ((f tete),(trans f reste));;
	
let empiler = fun
	el vide -> cons(el,vide)
	| el l -> cons(el,l);;

	
let tete = function 
	cons(x,y) -> x 
	| vide -> failwith"impossible" ;;
	
let suite = function 
	cons(x,y) -> y
	| vide -> failwith"impossible" ;;
	
let operation = function 
	op "+" -> prefix +
	| op "-" -> prefix -
	| op "*" -> prefix *
	| op "/" -> prefix /
	| _ -> failwith "operateur non recconu";;
	
(* Question 1 *)
	
let rec calc_post = function 
	(vide,pile) -> tete pile
	| (cons(op ope as operateur,reste),pile) -> calc_post(reste,cons((operation operateur) (tete(suite(pile))) (tete(pile)) ,(suite(suite(pile)))))
	| (cons(entier v,reste),pile) -> calc_post(reste,(cons(v,pile)));; 

(* Attention au sens de prise dans la pile => second membre puis premier membre , sinon probleme pour les divisions et soustractions *) 	

(* Question 2 *)
	
let rec arbre_synt = function 
	(vide,pile) -> tete pile
	| (cons(op ope as operateur,reste),pile) -> arbre_synt(reste,cons( (N( operateur , (tete(suite(pile))) ,(tete(pile)) )) ,(suite(suite(pile)))))
	| (cons(entier v,reste),pile) -> arbre_synt(reste,cons((N(entier v,V,V)),pile));;


	
let l = tout ("exprinv.txt") ;;
let l2 = trans char2str l;;  
let l3 = trans ch2terme l2;;  
calc_post(l3,vide);;
arbre_synt(l3,vide);;

(* include("//IUPDATA4/spi2358/Documents/workspace/CAML/letayTP5");; *)