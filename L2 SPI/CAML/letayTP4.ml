(* Letay benoit *)
(* 19/02 TP 4 , groupe de TP 5 *)

type 'a liste = vide | cons of  'a* 'a liste;; 

type 'a arbre = V | N of 'a * ('a arbre) * ('a arbre);;

(* Question 1 *)

let rec q1a = function 
		vide -> 0
		| cons(tete,reste) -> snd tete + q1a(reste);;
		
let rec q1b = function 
		V -> 0
		| N(r,ag,ad) -> snd r + q1b(ag) +q1b(ad);;
		
		
(* Question 2 *)

let rec q2a = function 
		(el,vide) -> cons(el,vide)
		| (el,cons(tete ,reste)) ->  if fst el = fst tete then cons ((fst el,snd el + snd tete),reste)
								else cons(tete,q2a(el,reste));;
								
let rec q2b = function 
		(el,vide) -> cons(el,vide)
		| (el,cons(tete ,reste)) ->  if fst el = fst tete then cons ((fst el,snd el + snd tete),reste)
								else if fst tete > fst el then cons(el,cons(tete,reste))
								else cons(tete,q2b(el,reste));;
		
let rec q2c = function
		(c,V) -> N(c,V,V)
		| (c,N(r,ag,ad)) ->   if fst c = fst r then N((fst c , snd c + snd r),ag,ad)
							else if fst c < fst r then  N(r,q2c(c,ag),ad)
							else N(r,ag,q2c(c,ad));;
							
(* Question 3 *)

let rec map = function
		(V,res) -> res
		| (N(r,ag,ad),res) -> map(ag,cons(r,map(ad,res)));;
		
let rec q3 = function a -> map (a,vide) ;;

(* Question 4 *)

let rec max2 = function (c1,c2) ->  if snd c1 > snd c2
							then c1
							else c2;;
							
let rec q4 = function 
		V -> failwith "impossible" 
		|N(r,V,V)  -> r
		|N(r,ag,V) -> max2(r,q4(ag))
		|N(r,V,ad) -> max2(r,q4(ad))
		|N(r,ag,ad) -> max2 (r,(max2(q4(ag),q4(ad))));;
		

