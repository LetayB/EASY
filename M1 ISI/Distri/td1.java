class Compteur extends Thread{
	static final int NITER = 1000000;	//static : Attribut de classe : partagé par tous les compteurs, final : non modifiable
	static int compteur = 0;			//Attribut de class
	static  final Object obj = new Object(); //Objet de synchronisation non modifiable
	public void run(){
		for(int i=0; i<NITER; i++){
			synchronized(obj){
				compteur ++;
			}
		}
	}
}

class CompteurTest{
	public static void main(String[] args) throws InterruptedException{
		//Nombre de threads, par défaut 4
		int nbThreads = (args.length < 1) ? 4 : Integer.parseInt(args[0]);
		Compteur[] tc = new Compteur[nbThreads];

		//Crée et démarre les threads
		for(int i = 0;i < nbThreads; i++){
			tc[i] = new Compteur();
			tc[i].start();
		}

		//Attends que les threads aient terminé
		for(int i =0; i < nbThreads; i++){
			tc[i].join();
		}

		//Compare la valeur de compteur avec la valeur attendue
		if(COmpteur.compteur != Compteur.NITER*nbThreads)
			System.out.println("Erreur ! compteur = " + Compteur.compteur);
		else
			System.out.println("Ok ! compteur = " + Compteur.compteur);
	}
}




//Exercice 2

class Fibo extends Thread{

	long n;
	long res;
	public Fibo(long n){
		this.n = n;
	}

	public void run(){
		if(n == 0) res = 0;
		if(n == 1) res = 1;
		if(n > 1){
			Fibo f1 = new Fibo(n-1);
			Fibo f2 = new Fibo(n-2);
			f1.start();
			f2.start();
			try{
				fib1.join();
				fib2.join();
			}catch( InterruptedException e) {...}
			res = f1.res + f2.res;
		}
	}

	public static void main(String args[]){
		Fibo fib = new Fibo(Integer.parseInt(args[0]));
		fib.start();
		try{
			fib.join();
		}catch(InterruptedException e){...}
		System.out.println("Resultat fibonnaci(" + n + ")= " + fib.res);
	}
}


//Exercice 3

class Exemple extends Thread{
	public void run(){
		System.out.print("Exemple ");
	}
}

class Multithread extends Thread{
	Thread th;
	public Multithread(Thread th){this.th = th;}
	public void run(){
		try{
			th.join();
		}catch(InterruptedException e){...}
		System.out.print("Multithread ");
	}
}

class MT2{
	public static void main(String[] args){
		Exemple e = new Exemple();
		Multithread mult = new Multithread(ex);
		mult.start();
		ex.start();
		try{
			mult.join();
		}catch(InterruptedException e){...}
		System.out.print("simple ");
	}
}




//Exercice 4

class Exemple extends Thread{
	public void run(){
		System.out.print("Exemple ");
	}
}

class Multithread extends Thread{
	public void run(){
		System.out.print("Multithread ");
	}
}

class MT2{
	public static void main(String[] args){
		Exemple e = new Exemple();
		e.run();
		System.out.print("simple ");
	}
}




























