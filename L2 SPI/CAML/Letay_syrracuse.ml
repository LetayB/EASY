(* benoit letay *)
(* groupe de TP 5  , TP syrracuse , 05/02 *)

let max = function (a,b) -> if a > b then a else b ;;

(* Question1 *)

let suivant = function n ->
		if n mod 2 = 0 then n/2
		else n * 3 + 1;;
		
(* Question 2 *)
		
let rec duree = function n ->
		if suivant n = 1 then 1
		else 1 + duree(suivant n) ;;
		
let rec hauteur = function n ->
		if suivant n = 1 then n
		else max(n,hauteur(suivant n));; 
		
(* Question 3 *)

let rec duree_T = function (n,res) -> 
		if suivant n = 1 then res 
		else duree_T(suivant n , res + 1 );;
		
let duree_Termi = function n -> duree_T(n,1);;

let rec hauteur_T = function (n,maximun) ->
		if suivant n = 1 then maximun
		else hauteur_T(suivant n , max(suivant n,maximun));; 
		
let hauteur_Termi = function n -> hauteur_T(n,n);;


duree 2 ;;
duree_Termi 2 ;;
hauteur 2;;
hauteur_Termi 2;;