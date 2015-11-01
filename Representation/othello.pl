/* Letay Benoit */

/* set_prolog_flag(encoding, utf8). */

/* 20 */
campAdverse(o,●).
campAdverse(●,o).
campAdverse(-,-).

/* 1 */
grilleDeDepart([
[-,-,-,-,-,-,-,-],
[-,-,-,-,-,-,-,-],
[-,-,o,o,o,-,-,-],
[-,-,o,-,-,-,-,-],
[-,-,o,-,-,-,-,-],
[-,-,●,-,-,-,-,-],
[-,-,-,-,-,-,-,-],
[-,-,-,-,-,-,-,-]
]).

grilleTest([
[-,●,-,-,-,-,o,-],
[●,●,●,●,●,-,-,-],
[●,●,●,●,●,●,●,●],
[●,●,●,●,●,●,●,●],
[●,●,●,o,●,●,●,●],
[●,●,o,●,●,●,●,●],
[●,●,●,●,●,●,●,●],
[-,●,o,-,●,●,o,o]
]).


/* 2 */
succNum(9,1).
succNum(1,2).
succNum(2,3).
succNum(3,4).
succNum(4,5).
succNum(5,6).
succNum(6,7).
succNum(7,8).
succNum(8,9).	

succAlpha(z,a).
succAlpha(a,b).
succAlpha(b,c).
succAlpha(c,d).
succAlpha(d,e).
succAlpha(e,f).
succAlpha(f,g).
succAlpha(g,h).
succAlpha(h,z).

/* 3 */
afficheLigne([]) :-nl.
afficheLigne([H|T])  :- write(H),write(' ┃ '),afficheLigne(T).

/* 4 */
afficheGrille([H|T]) :- 
						write('┏━━━┳━━━┳━━━┳━━━┳━━━┳━━━┳━━━┳━━━┳━━━┓'),
						nl,
						write('┃   ┃ a ┃ b ┃ c ┃ d ┃ e ┃ f ┃ g ┃ h ┃'),
						nl,
						afficheGrille([H|T],1),
						write('┗━━━┻━━━┻━━━┻━━━┻━━━┻━━━┻━━━┻━━━┻━━━┛').


afficheGrille([],_).

afficheGrille([H|T],X) :-   
						write('┣━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━┫'),
						nl,
						write('┃ '),write(X),write(' ┃'),
						write(' '),											
						afficheLigne(H),	
						Y is X + 1,
						afficheGrille(T,Y).




/* 5 */
ligneDansGrille(1,[H|_],H).
ligneDansGrille(NumLigne,[_|T],Ligne) :- succNum(X,NumLigne), ligneDansGrille(X,T,Ligne).

caseDansLigne(a,[H|_],H).
caseDansLigne(Col,[_|T],Valeur) :- succAlpha(X,Col), caseDansLigne(X,T,Valeur).


/* 6 */
donneValeurCase(Grille,Col,NumLigne,Valeur) :-  ligneDansGrille(NumLigne,Grille,Ligne), caseDansLigne(Col,Ligne,Valeur).

/* 7 */
caseVide(Grille,Col,Ligne) :- donneValeurCase(Grille,Col,Ligne,'-').

/* 8 */
direction(no,-1,-1).
direction( n,-1, 0).
direction(ne,-1, 1).
direction( o, 0,-1).
direction( e, 0, 1).
direction(so, 1,-1).
direction( s, 1, 0).
direction(se, 1, 1).

dirLigne(-1,X,Y) :- succNum(Y,X).
dirLigne( 1,X,Y) :- succNum(X,Y).
dirLigne( 0,X,X).
dirLigne( Dir,X,Y) :- direction(Dir,Z,_),dirLigne(Z,X,Y).

dirColonne(-1,X,Y) :- succAlpha(Y,X).
dirColonne( 1,X,Y) :- succAlpha(X,Y).
dirColonne( 0,X,X).
dirColonne( Dir,X,Y) :- direction(Dir,_,Z),dirColonne(Z,X,Y).



bordOthellierX(Dir,NumColonne) :- 	dirColonne(Dir,NumColonne,NewNumColonne), NewNumColonne == z.

bordOthellierY(Dir,NumLigne) :- 	dirLigne(Dir,NumLigne,NewNumLigne), NewNumLigne == 9.

	
/* 9 */
donneListeCasesDansDirection(Dir,_,_,NumLigne,[]) :- 		bordOthellierY(Dir,NumLigne).
donneListeCasesDansDirection(Dir,_,NumColonne,_,[]) :- 		bordOthellierX(Dir,NumColonne).
donneListeCasesDansDirection(Dir,Grille,NumColonne,NumLigne,[Valeur|T]) :- 	dirColonne(Dir,NumColonne,NewNumColonne),
																			dirLigne(Dir,NumLigne,NewNumLigne),
																	 	 	donneValeurCase(Grille,NewNumColonne,NewNumLigne,Valeur),
																		 	donneListeCasesDansDirection(Dir,Grille,NewNumColonne,NewNumLigne,T).
/* 10 */
fait_priseF(●,[●|_]) :- !.
fait_priseF(●,[o|T]) :- fait_priseF(●,T).
fait_priseF(o,[o|_]) :- !.
fait_priseF(o,[●|T]) :- fait_priseF(o,T).


fait_prise(●,[o|T]) :-  fait_priseF(●,T).
fait_prise(o,[●|T]) :-  fait_priseF(o,T).

/* 11 */
peutFairePrise(Grille,●,NumColonne,NumLigne) :- donneListeCasesDansDirection(no,Grille,NumColonne,NumLigne,Liste), fait_prise(●,Liste).
peutFairePrise(Grille,●,NumColonne,NumLigne) :- donneListeCasesDansDirection( n,Grille,NumColonne,NumLigne,Liste), fait_prise(●,Liste).
peutFairePrise(Grille,●,NumColonne,NumLigne) :- donneListeCasesDansDirection(ne,Grille,NumColonne,NumLigne,Liste), fait_prise(●,Liste).
peutFairePrise(Grille,●,NumColonne,NumLigne) :- donneListeCasesDansDirection( o,Grille,NumColonne,NumLigne,Liste), fait_prise(●,Liste).
peutFairePrise(Grille,●,NumColonne,NumLigne) :- donneListeCasesDansDirection( e,Grille,NumColonne,NumLigne,Liste), fait_prise(●,Liste).
peutFairePrise(Grille,●,NumColonne,NumLigne) :- donneListeCasesDansDirection(so,Grille,NumColonne,NumLigne,Liste), fait_prise(●,Liste).
peutFairePrise(Grille,●,NumColonne,NumLigne) :- donneListeCasesDansDirection( s,Grille,NumColonne,NumLigne,Liste), fait_prise(●,Liste).
peutFairePrise(Grille,●,NumColonne,NumLigne) :- donneListeCasesDansDirection(se,Grille,NumColonne,NumLigne,Liste), fait_prise(●,Liste).

peutFairePrise(Grille,o,NumColonne,NumLigne) :- donneListeCasesDansDirection(no,Grille,NumColonne,NumLigne,Liste), fait_prise(o,Liste).
peutFairePrise(Grille,o,NumColonne,NumLigne) :- donneListeCasesDansDirection( n,Grille,NumColonne,NumLigne,Liste), fait_prise(o,Liste).
peutFairePrise(Grille,o,NumColonne,NumLigne) :- donneListeCasesDansDirection(ne,Grille,NumColonne,NumLigne,Liste), fait_prise(o,Liste).
peutFairePrise(Grille,o,NumColonne,NumLigne) :- donneListeCasesDansDirection( o,Grille,NumColonne,NumLigne,Liste), fait_prise(o,Liste).
peutFairePrise(Grille,o,NumColonne,NumLigne) :- donneListeCasesDansDirection( e,Grille,NumColonne,NumLigne,Liste), fait_prise(o,Liste).
peutFairePrise(Grille,o,NumColonne,NumLigne) :- donneListeCasesDansDirection(so,Grille,NumColonne,NumLigne,Liste), fait_prise(o,Liste).
peutFairePrise(Grille,o,NumColonne,NumLigne) :- donneListeCasesDansDirection( s,Grille,NumColonne,NumLigne,Liste), fait_prise(o,Liste).
peutFairePrise(Grille,o,NumColonne,NumLigne) :- donneListeCasesDansDirection(se,Grille,NumColonne,NumLigne,Liste), fait_prise(o,Liste).

leCoupEstValide(Grille,Camp,NumColonne,NumLigne) :-	caseVide(Grille,NumColonne,NumLigne),
													peutFairePrise(Grille,Camp,NumColonne,NumLigne).


/* 14 */
comptePionsLigne([],0,0).
comptePionsLigne([-|T],NbX,NbO) :- comptePionsLigne(T,NbX,NbO).
comptePionsLigne([●|T],NbX,NbO) :- comptePionsLigne(T,X,NbO),  NbX is X+1.
comptePionsLigne([o|T],NbX,NbO) :- comptePionsLigne(T,NbX,O), NbO is O+1.

comptePions([],0,0).
comptePions([H|T],NbX,NbO):- comptePions(T,X,O),
							 comptePionsLigne(H,NbXLigne,NbOLigne),
							 NbX is NbXLigne + X, 
							 NbO is NbOLigne + O .


/* 15 */
coordonneesOuListe(NumColonne,NumLigne,[NumColonne,NumLigne]).



parcoursGaucheDroite([h,8],[h,8]) :- !.
parcoursGaucheDroite([h,NumLigne],[a,NewNumLigne]) :- succNum(NumLigne,NewNumLigne),!.
parcoursGaucheDroite([NumColonne,NumLigne],[NewNumColonne,NumLigne]) :- succAlpha(NumColonne,NewNumColonne).


/*

listeCasesVides(_,[],[h,8]) :- !.
listeCasesVides(Grille,ListeCasesVides,Position) :- coordonneesOuListe(NumColonne,NumLigne,Position),
													donneValeurCase(Grille,NumColonne,NumLigne,Valeur),
													Valeur \== -,
													parcoursGaucheDroite(Position,NewPosition),
													listeCasesVides(Grille,ListeCasesVides,NewPosition).

listeCasesVides(Grille,ListeCasesVides,Position) :- parcoursGaucheDroite(Position,NewPosition),print(Position),
													listeCasesVides(Grille,[Position|ListeCasesVides],NewPosition).



listeCasesVides([],0,0).
listeCasesVides([H|T],NbX,NbO):- comptePions(T,X,O),listeCasesVidesLigne(H,NbXLigne,NbOLigne),NbX is NbXLigne + NbX, NbO is NbOLigne + O .
listeCasesVidesLigne([],[]).
listeCasesVidesLigne([H|T],ListeCasesVides) :- H \== -,listeCasesVidesLigne(T,ListeCasesVides).
listeCasesVidesLigne([-|T],ListeCasesVides) :- listeCasesVidesLigne(T,ListeCasesVides).

*/
listeCasesVidesLigne([], [],_,z).
listeCasesVidesLigne(['-'|T], [Q2|T2], NumLigne, NumColonne):- coordonneesOuListe(NumColonne, NumLigne, Q2), succAlpha(NumColonne, NewNumColonne), listeCasesVidesLigne(T, T2, NumLigne, NewNumColonne).
listeCasesVidesLigne(['●'|T], T2, NumLigne, NumColonne):- succAlpha(NumColonne, NewNumColonne), listeCasesVidesLigne(T, T2, NumLigne, NewNumColonne).
listeCasesVidesLigne(['o'|T], T2, NumLigne, NumColonne):- succAlpha(NumColonne, NewNumColonne), listeCasesVidesLigne(T, T2, NumLigne, NewNumColonne).

listeCasesVides2([],[], 9).
listeCasesVides2([H|T], [H2|T2], NumLigne):- succNum(NumLigne, NewNumLigne), listeCasesVidesLigne(H, H2, NumLigne, a), listeCasesVides2(T, T2, NewNumLigne).
listeCasesVides(Grille, L):- listeCasesVides2(Grille, Liste, 1), append(Liste, L).


listeCasesValides(_,_,[],[]).
listeCasesValides(Grille,Camp,[H|T],[H|T2]):- 	listeCasesValides(Grille,Camp,T,T2),
												coordonneesOuListe(NumColonne,NumLigne,H),
												peutFairePrise(Grille,Camp,NumColonne,NumLigne),!.
listeCasesValides(Grille,Camp,[_|T],L):- 	listeCasesValides(Grille,Camp,T,L).	
listeDesCoupsCamps(Grille,Camp,L):- listeCasesVides(Grille,ListeCasesVides),listeCasesValides(Grille,Camp,ListeCasesVides,L).


/* grilleTest(X),listeDesCoupsCamps(X,o,L). */

changePionLigne([], [],_,z,_,_,_):- !.
changePionLigne([_|T], [Value|T2], NumLigne, NumColonne, NumLigne, NumColonne,Value):-  succAlpha(NumColonne, NewNumColonne), 
																						changePionLigne(T, T2, NumLigne, NewNumColonne, NumLigne, NumColonne,Value),!.
changePionLigne([Q|T], [Q|T2], NumLigne, NumColonne, L, C,V):-  succAlpha(NumColonne, NewNumColonne), 
															    changePionLigne(T, T2, NumLigne, NewNumColonne,L,C,V).


changePion2([],[], 9,_,_,_).
changePion2([H|T], [H2|T2], NumLigne,LignePion,ColonnePion,NewValue):- succNum(NumLigne, NewNumLigne), changePionLigne(H, H2, NumLigne, a,LignePion,ColonnePion,NewValue), changePion2(T, T2, NewNumLigne,LignePion,ColonnePion,NewValue).
changePion(Grille,Liste,LignePion,ColonnePion,NewValue):- changePion2(Grille, Liste, 1,LignePion,ColonnePion,NewValue).

retournePionsDansDirection2(_,G,NumColonne,NumLigne,Valeur,G) :-  donneValeurCase(G,NumColonne,NumLigne,Valeur).
/* Ligne potentiellement inutile (On ne peut retourner une ligne avec un espace vide) */
retournePionsDansDirection2(_,G,NumColonne,NumLigne,_,G) :-  donneValeurCase(G,NumColonne,NumLigne,-).

retournePionsDansDirection2(Dir,GrilleDep,NumColonne,NumLigne,Valeur,GrilleArr) :-  donneValeurCase(GrilleDep,NumColonne,NumLigne,OldValue), 
																					campAdverse(Valeur,OldValue),
																					changePion(GrilleDep,NewGrille,NumLigne,NumColonne,Valeur),
																					dirColonne(Dir,NumColonne,NewNumColonne),
																					dirLigne(Dir,NumLigne,NewNumLigne),
																					retournePionsDansDirection2(Dir,NewGrille,NewNumColonne,NewNumLigne,Valeur,GrilleArr).

retournePionsDansDirection(Dir,GrilleDep,NumColonne,NumLigne,Valeur,GrilleArr) :- 	dirColonne(Dir,NumColonne,NewNumColonne),
																					dirLigne(Dir,NumLigne,NewNumLigne),
																					retournePionsDansDirection2(Dir,GrilleDep,NewNumColonne,NewNumLigne,Valeur,GrilleArr).


/* grilleDeDepart(X),retournePionsDansDirection(e,X,b,3,●,L). */