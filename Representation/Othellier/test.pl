/****************************************/
/* Menu */
/****************************************/

menu :-
write('Voici la grille : '),nl,
grilleDepart(R),
afficheGrille(R),
write('Choisissez le premier joueur : '),nl,
read(Val),
write('Puis la ligne : '),nl,
read(Ligne),
write('Puis la colonne : '),nl,
read(Colonne),
coupJoue([sud,nord,est,ouest,sudEst,sudOuest,nordEst,nordOuest],R,Ligne,Colonne,Val,GrilleArrivee),
coupJoueDansGrille(R,Ligne,Colonne,Val,GrilleArrivee),
menu(GrilleArrivee,Val);
menu.


menu(R,o):-
write('Voici la grille : '),nl,
afficheGrille(R),
write('Au Joueur x de placer un pion'),nl,
write('Choix de la ligne : '),nl,
read(Ligne),
write('Puis de la colonne : '),nl,
read(Colonne),
coupJoue([sud,nord,est,ouest,sudEst,sudOuest,nordEst,nordOuest],R,Ligne,Colonne,x,GrilleArrivee),
not(R = GrilleArrivee),
menu(GrilleArrivee,x);
write('Erreur !!! Veuillez recommencer !'),
menu(R,o).



menu(R,x):-
write('Voici la grille : '),nl,
afficheGrille(R),
write('Au Joueur o de placer un pion'),nl,
write('Choix de la ligne : '),nl,
read(Ligne),
write('Puis de la colonne : '),nl,
read(Colonne),
coupJoue([sud,nord,est,ouest,sudEst,sudOuest,nordEst,nordOuest],R,Ligne,Colonne,o,GrilleArrivee),
not(R = GrilleArrivee),
menu(GrilleArrivee,o);
write('Erreur !!! Veuillez recommencer !'),
menu(R,x).
/****************************************/




/****************************************/
/* Tests */
/****************************************/
test2(Dir,X,Y,ListeCases):-
donneListeCasesDansDirection(Dir,[[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,o,x,-,-,-],[-,-,-,x,o,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-]],X,Y,[],ListeCases).

test3(R):-
donneListeCoordDansDirection(sudEst,[[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,o,x,-,-,-],[-,-,-,x,o,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-]],1,a,[],R).

test4(V,X,Y):-
leCoupEstValide([[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,o,x,-,-,-],[-,-,-,x,o,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-]],V,X,Y).
test5(V,X,Y,Liste,Taille):-
leCoupEstValideCoord([[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,o,x,-,-,-],[-,-,-,x,o,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-]],V,X,Y,Liste,Taille).

test6(X,Y):-
caseVide([[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,o,x,-,-,-],[-,-,-,x,o,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-]],X,Y).

test7(Dir,X,Y,Val,R):-
retournePionsDansDirection(Dir,[[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,o,x,-,-,-],[-,-,-,x,o,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-]],X,Y,Val,R),
afficheGrille(R).

test8(X,Y,Val):-
coupJoueDansGrille([[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,o,x,-,-,-],[-,-,-,x,o,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-]],X,Y,Val,R),
afficheGrille(R).

test9(X,O):-
grilleDepart(G),
comptePionsGrille(G,X,O).
/******************************************/

/* Q1 */
/* Donne la grille de départ */
grilleDepart([
	[-,-,-,-,-,-,-,-],
	[-,-,-,-,-,-,-,-],
	[-,-,-,-,-,-,-,-],
	[-,-,-,o,x,-,-,-],
	[-,-,-,x,o,-,-,-],
	[-,-,-,-,-,-,-,-],
	[-,-,-,-,-,-,-,-],
	[-,-,-,-,-,-,-,-]]).

/* Q2 */
/* Successeur numérique */
succNum(1,2).
succNum(2,3).
succNum(3,4).
succNum(4,5).
succNum(5,6).
succNum(6,7).
succNum(7,8).

/* Successeur alpha */
succAlpha(a,b).
succAlpha(b,c).
succAlpha(c,d).
succAlpha(d,e).
succAlpha(e,f).
succAlpha(f,g).
succAlpha(g,h).

/* Conversion de l'alpha en numérique */
alphaToNum(a,1).
alphaToNum(b,2).
alphaToNum(c,3).
alphaToNum(d,4).
alphaToNum(e,5).
alphaToNum(f,6).
alphaToNum(g,7).
alphaToNum(h,8).

/* Q3 */
/* Affiche la ligne */
afficheLigne([L]) :-
writeln(L),!.
afficheLigne([L|T]) :-
write(L),write(' '),
afficheLigne(T).

/* Q4 */
/* Affiche la grille Part 1 */
afficheGrille(G):-
write(' '),write(' '),
write(a),write(' '),
write(b),write(' '),
write(c),write(' '),
write(d),write(' '),
write(e),write(' '),
write(f),write(' '),
write(g),write(' '),
writeln(h),
afficheGrille2(G,1).
/* Affiche la grille Part 2 */
afficheGrille2([L],X) :-
write(X),write(' '),
afficheLigne(L),!.
afficheGrille2([L|C],X) :-
succNum(X,Y),
write(X),write(' '),
afficheLigne(L),
afficheGrille2(C,Y).

/* Q5 */
/* Numérotation des lignes */
ligneDansGrille(1,[A,_,_,_,_,_,_,_],A).
ligneDansGrille(2,[_,B,_,_,_,_,_,_],B).
ligneDansGrille(3,[_,_,C,_,_,_,_,_],C).
ligneDansGrille(4,[_,_,_,D,_,_,_,_],D).
ligneDansGrille(5,[_,_,_,_,E,_,_,_],E).
ligneDansGrille(6,[_,_,_,_,_,F,_,_],F).
ligneDansGrille(7,[_,_,_,_,_,_,G,_],G).
ligneDansGrille(8,[_,_,_,_,_,_,_,H],H).

/* Numérotation des élément d'une ligne */
caseDansLigne(a,[A,_,_,_,_,_,_,_],A).
caseDansLigne(b,[_,B,_,_,_,_,_,_],B).
caseDansLigne(c,[_,_,C,_,_,_,_,_],C).
caseDansLigne(d,[_,_,_,D,_,_,_,_],D).
caseDansLigne(e,[_,_,_,_,E,_,_,_],E).
caseDansLigne(f,[_,_,_,_,_,F,_,_],F).
caseDansLigne(g,[_,_,_,_,_,_,G,_],G).
caseDansLigne(h,[_,_,_,_,_,_,_,H],H).

/* Q6 */
/* Donne la valeur de la case x o ou - */
donneCaseValeur(Grille,NumColonne,NumLigne,Valeur):-
ligneDansGrille(NumLigne,Grille,Ligne),
caseDansLigne(NumColonne,Ligne,Valeur).

/* Récupère la valeur de la case si non vide */
donneValeurDeCase(Grille,NumColonne,NumLigne,Valeur):-
ligneDansGrille(NumLigne,Grille,Ligne),
caseDansLigne(NumColonne,Ligne,Valeur),
Valeur = "o";
ligneDansGrille(NumLigne,Grille,Ligne),
caseDansLigne(NumColonne,Ligne,Valeur),
Valeur = "x".

/* Q7 */
/* Test si case vide */
caseVide(Grille,NumLigne,NumColonne):-
ligneDansGrille(NumLigne,Grille,Ligne),
caseDansLigne(NumColonne,Ligne,Valeur),
Valeur \= "x", Valeur \= "o".

/* Q8 */
/* Définition des directions Dir,Y,X */
direction(nord,-1,0).
direction(sud,1,0).
direction(est,0,1).
direction(ouest,0,-1).
direction(nordEst,-1,1).
direction(nordOuest,-1,-1).
direction(sudEst,1,1).
direction(sudOuest,1,-1).


/* Donne le numéro de colonne correspondant à la direction */
nextColonne(1,NumColonne,NumColonneNext):-
succAlpha(NumColonne,NumColonneNext).
nextColonne(-1,NumColonne,NumColonneNext):-
succAlpha(NumColonneNext,NumColonne).
nextColonne(0,NumColonneNext,NumColonneNext).

/* Q9 */
/* Donne la liste des cases dans une direction donnée */
donneListeCasesDansDirection(sud,_,8,_,L,L).
donneListeCasesDansDirection(nord,_,1,_,L,L).
donneListeCasesDansDirection(est,_,_,h,L,L).
donneListeCasesDansDirection(ouest,_,_,a,L,L).
donneListeCasesDansDirection(nordEst,_,1,_,L,L).
donneListeCasesDansDirection(nordEst,_,_,h,L,L).
donneListeCasesDansDirection(nordOuest,_,1,_,L,L).
donneListeCasesDansDirection(nordOuest,_,_,a,L,L).
donneListeCasesDansDirection(sudEst,_,8,_,L,L).
donneListeCasesDansDirection(sudEst,_,_,h,L,L).
donneListeCasesDansDirection(sudOuest,_,8,_,L,L).
donneListeCasesDansDirection(sudOuest,_,_,a,L,L).

donneListeCasesDansDirection(Direction,Grille,NumLigne,NumColonne,ListeCases,R):-
direction(Direction,X,Y),
NumLigneNext is NumLigne + X,
nextColonne(Y,NumColonne,NumColonneNext),
donneCaseValeur(Grille,NumColonneNext,NumLigneNext,Valeur),
append(ListeCases,[Valeur],NewListeCases),
donneListeCasesDansDirection(Direction,Grille,NumLigneNext,NumColonneNext,NewListeCases,R),!.

/* Q10 */
/* Vérifie que la prise est faisable */
faitPrise(x,[o,x|_]).
faitPrise(x,[o,o,x|_]).
faitPrise(x,[o,o,o,x|_]).
faitPrise(x,[o,o,o,o,x|_]).
faitPrise(x,[o,o,o,o,o,x|_]).
faitPrise(x,[o,o,o,o,o,o,x]).
faitPrise(o,[x,o|_]).
faitPrise(o,[x,x,o|_]).
faitPrise(o,[x,x,x,o|_]).
faitPrise(o,[x,x,x,x,o|_]).
faitPrise(o,[x,x,x,x,x,o|_]).
faitPrise(o,[x,x,x,x,x,x,o]).

/* Q11 */
/* Vérifie que le coup est valide dans la direction donnée */
leCoupEstValide(Dir,Grille,Camp,NumLigne,NumColonne):-
caseVide(Grille,NumLigne,NumColonne),
donneListeCasesDansDirection(Dir,Grille,NumLigne,NumColonne,[],ListeCases),
faitPrise(Camp,ListeCases).

/* Pareil mais pour toutes les directions */
leCoupEstValide(Grille,Camp,NumLigne,NumColonne):-
leCoupEstValide(sud,Grille,Camp,NumLigne,NumColonne).

leCoupEstValide(Grille,Camp,NumLigne,NumColonne):-
leCoupEstValide(nord,Grille,Camp,NumLigne,NumColonne).

leCoupEstValide(Grille,Camp,NumLigne,NumColonne):-
leCoupEstValide(est,Grille,Camp,NumLigne,NumColonne).

leCoupEstValide(Grille,Camp,NumLigne,NumColonne):-
leCoupEstValide(ouest,Grille,Camp,NumLigne,NumColonne).

leCoupEstValide(Grille,Camp,NumLigne,NumColonne):-
leCoupEstValide(nordOuest,Grille,Camp,NumLigne,NumColonne).

leCoupEstValide(Grille,Camp,NumLigne,NumColonne):-
leCoupEstValide(nordEst,Grille,Camp,NumLigne,NumColonne).

leCoupEstValide(Grille,Camp,NumLigne,NumColonne):-
leCoupEstValide(sudEst,Grille,Camp,NumLigne,NumColonne).

leCoupEstValide(Grille,Camp,NumLigne,NumColonne):-
leCoupEstValide(sudOuest,Grille,Camp,NumLigne,NumColonne).

/* Q12 */
/* Retournes les pions */
retournePionsDansDirection(Dir,GrilleDepart,NumLigne,NumColonne,Val,GrilleArrivee):-
leCoupEstValideCoord(Dir,GrilleDepart,Val,NumLigne,NumColonne,ListCoord,TaillePrise),
slice(ListCoord,1,TaillePrise,ListCoordToReplace),
replaceAllByCoord(GrilleDepart,ListCoordToReplace,Val,GrilleArrivee).

replaceAllByCoord(GrilleDepart,[[X,Y]],Val,GrilleArrivee):-
ligneDansGrille(X,GrilleDepart,List),
replace(List,Y,Val,ListOut),
replace(GrilleDepart,X,ListOut,GrilleArrivee).

replaceAllByCoord(GrilleDepart,[[X,Y]|LCTR],Val,GrilleArrivee):-
ligneDansGrille(X,GrilleDepart,List),
replace(List,Y,Val,ListOut),
replace(GrilleDepart,X,ListOut,GrilleArriveeTemp),
replaceAllByCoord(GrilleArriveeTemp,LCTR,Val,GrilleArrivee).

/* Q13 */
/* Retourne une grille avec le coup joué */
coupJoueDansGrille(GrilleDepart,NumLigne,NumColonne,Val,GrilleArrivee):-
coupJoueDansGrille(sud,GrilleDepart,NumLigne,NumColonne,Val,GrilleArrivee);
coupJoueDansGrille(sudEst,GrilleDepart,NumLigne,NumColonne,Val,GrilleArrivee);
coupJoueDansGrille(sudOuest,GrilleDepart,NumLigne,NumColonne,Val,GrilleArrivee);
coupJoueDansGrille(nord,GrilleDepart,NumLigne,NumColonne,Val,GrilleArrivee);
coupJoueDansGrille(nordEst,GrilleDepart,NumLigne,NumColonne,Val,GrilleArrivee);
coupJoueDansGrille(nordOuest,GrilleDepart,NumLigne,NumColonne,Val,GrilleArrivee);
coupJoueDansGrille(est,GrilleDepart,NumLigne,NumColonne,Val,GrilleArrivee);
coupJoueDansGrille(ouest,GrilleDepart,NumLigne,NumColonne,Val,GrilleArrivee).

coupJoueDansGrille(Dir,GrilleDepart,NumLigne,NumColonne,Val,GrilleArrivee):-
retournePionsDansDirection(Dir,GrilleDepart,NumLigne,NumColonne,Val,GrilleArriveeTemp),
replaceAllByCoord(GrilleArriveeTemp,[[NumLigne,NumColonne]],Val,GrilleArrivee).


/* Retourne une grille avec toutes les directions possible jouées */
coupJoue([],GrilleDepart,_,_,_,GrilleDepart):-afficheGrille(GrilleDepart).

coupJoue([Dir|Liste],GrilleDepart,NumLigne,NumColonne,Val,GrilleArrivee):-
coupJoueDansGrille(Dir,GrilleDepart,NumLigne,NumColonne,Val,GrilleTemp),
coupJoue(Liste,GrilleTemp,NumLigne,NumColonne,Val,GrilleArrivee);
coupJoue(Liste,GrilleDepart,NumLigne,NumColonne,Val,GrilleArrivee).



/* Q14 */
/* Compte les pions de chaque camps */
comptePionsListe(Liste,X,O):-
occurrence(Liste,x,X),
occurrence(Liste,o,O).

occurrence([],_,0).
occurrence([X|L],X,N) :- occurrence(L,X,N1),N is N1+1.
occurrence([Y|L],X,N) :- X\==Y,occurrence(L,X,N).

comptePionsGrille([Liste],X,O):-
comptePionsListe(Liste,X,O).


comptePionsGrille([Liste|Grille],X,O):-
comptePionsListe(Liste,Xtemp,Otemp),
comptePionsGrille(Grille,Xtemp2,Otemp2),
X is Xtemp + Xtemp2,
O is Otemp + Otemp2.




/****************************/
/****************************/
/* Fonction supplémentaires */
/****************************/
/****************************/


/**********************************************************************************/
/* Replace une valeur dans une liste à un index donné, retourne la liste resultat */
/**********************************************************************************/
slice([X|_],1,1,[X]).
slice([X|Xs],1,K,[X|Ys]) :- K > 1, 
   K1 is K - 1, slice(Xs,1,K1,Ys).
slice([_|Xs],I,K,Ys) :- I > 1, 
   I1 is I - 1, K1 is K - 1, slice(Xs,I1,K1,Ys).

replace(Liste,1,Valeur,ListeOut):-
length(Liste,Size),
slice(Liste,2,Size,ListeTemp1),
append([Valeur],ListeTemp1,ListeOut).
replace(Liste,8,Valeur,ListeOut):-
length(Liste,Size),
Y is Size - 1,
slice(Liste,0,Y,ListeTemp1),
append(ListeTemp1,[Valeur],ListeOut).
replace(Liste,a,Valeur,ListeOut):-
length(Liste,Size),
slice(Liste,2,Size,ListeTemp1),
append([Valeur],ListeTemp1,ListeOut).

replace(Liste,h,Valeur,ListeOut):-
length(Liste,Ind),
Y is Ind - 1,
slice(Liste,0,Y,ListeTemp1),
append(ListeTemp1,[Valeur],ListeOut).

replace(Liste,Index,Valeur,ListeOut):-
succNum(X,Index),
succNum(Index,Y),
length(Liste,Size),
slice(Liste,1,X,ListeTemp1),
slice(Liste,Y,Size,ListeTemp2),
append(ListeTemp1,[Valeur],ListeTemp3),
append(ListeTemp3,ListeTemp2,ListeOut);
alphaToNum(Index,IndexBis),
succNum(XBis,IndexBis),
succNum(IndexBis,YBis),
length(Liste,SizeBis),
slice(Liste,1,XBis,ListeTemp1Bis),
slice(Liste,YBis,SizeBis,ListeTemp2Bis),
append(ListeTemp1Bis,[Valeur],ListeTemp3Bis),
append(ListeTemp3Bis,ListeTemp2Bis,ListeOut).


/************************************/
/* Donne les prochaines coordonnées */
/************************************/
getNextCoord(Dir,NumLigne,NumColonne,ListCoord):-
direction(Dir,X,Y),
alphaToNum(NumColonne,NumC),
Ynext is NumC + Y,
Xnext is NumLigne + X,
alphaToNum(Yind,Ynext),
append([Xnext,Yind],[],ListCoord),
Ynext \= 0, Ynext \= 9,
Xnext \= 0, Xnext \= 9.

/*******************************/
/* Donne la taille de la prise */
/*******************************/
taillePrise(x,[o,x|_],2).
taillePrise(x,[o,o,x|_],3).
taillePrise(x,[o,o,o,x|_],4).
taillePrise(x,[o,o,o,o,x|_],5).
taillePrise(x,[o,o,o,o,o,x|_],6).
taillePrise(x,[o,o,o,o,o,o,x],7).
taillePrise(o,[x,o|_],2).
taillePrise(o,[x,x,o|_],3).
taillePrise(o,[x,x,x,o|_],4).
taillePrise(o,[x,x,x,x,o|_],5).
taillePrise(o,[x,x,x,x,x,o|_],6).
taillePrise(o,[x,x,x,x,x,x,o],7).


/***********************************************/
/* Donne la liste des coords dans une direction*/
/***********************************************/
donneListeCoordDansDirection(sud,_,8,_,L,L).
donneListeCoordDansDirection(nord,_,1,_,L,L).
donneListeCoordDansDirection(est,_,_,h,L,L).
donneListeCoordDansDirection(ouest,_,_,a,L,L).
donneListeCoordDansDirection(nordEst,_,1,_,L,L).
donneListeCoordDansDirection(nordEst,_,_,h,L,L).
donneListeCoordDansDirection(nordOuest,_,1,_,L,L).
donneListeCoordDansDirection(nordOuest,_,_,a,L,L).
donneListeCoordDansDirection(sudEst,_,8,_,L,L).
donneListeCoordDansDirection(sudEst,_,_,h,L,L).
donneListeCoordDansDirection(sudOuest,_,8,_,L,L).
donneListeCoordDansDirection(sudOuest,_,_,a,L,L).
donneListeCoordDansDirection(Direction,Grille,NumLigne,NumColonne,ListeCases,R):-
direction(Direction,X,Y),
NumLigneNext is NumLigne + X,
nextColonne(Y,NumColonne,NumColonneNext),
getNextCoord(Direction,NumLigne,NumColonne,ListCoord),
append(ListeCases,[ListCoord],NewListeCases),
donneListeCoordDansDirection(Direction,Grille,NumLigneNext,NumColonneNext,NewListeCases,R),!.


/* Vérifie que le coup est valide dans la direction donnée */
leCoupEstValideCoord(Dir,Grille,Camp,NumLigne,NumColonne,ListCoord,TaillePrise):-
caseVide(Grille,NumLigne,NumColonne),
donneListeCasesDansDirection(Dir,Grille,NumLigne,NumColonne,[],ListeCases),
faitPrise(Camp,ListeCases),
donneListeCoordDansDirection(Dir,Grille,NumLigne,NumColonne,[],ListCoord),
taillePrise(Camp,ListeCases,TaillePrise).

/* Pareil mais pour toutes les directions */
leCoupEstValideCoord(Grille,Camp,NumLigne,NumColonne,ListCoord,TaillePrise):-
leCoupEstValideCoord(sud,Grille,Camp,NumLigne,NumColonne,ListCoord,TaillePrise);
leCoupEstValideCoord(nord,Grille,Camp,NumLigne,NumColonne,ListCoord,TaillePrise);
leCoupEstValideCoord(est,Grille,Camp,NumLigne,NumColonne,ListCoord,TaillePrise);
leCoupEstValideCoord(ouest,Grille,Camp,NumLigne,NumColonne,ListCoord,TaillePrise);
leCoupEstValideCoord(nordOuest,Grille,Camp,NumLigne,NumColonne,ListCoord,TaillePrise);
leCoupEstValideCoord(nordEst,Grille,Camp,NumLigne,NumColonne,ListCoord,TaillePrise);
leCoupEstValideCoord(sudEst,Grille,Camp,NumLigne,NumColonne,ListCoord,TaillePrise);
leCoupEstValideCoord(sudOuest,Grille,Camp,NumLigne,NumColonne,ListCoord,TaillePrise).