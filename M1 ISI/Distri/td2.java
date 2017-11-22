//Exercice 1

public interface ILamp extends java.rmi.Remote{
	public void light() throws java.rmi.RemoteException;
	public void shut() throws java.rmi.RemoteException;
}

//Exportation automatique
public class LampImpl extends java.rmi.servu.UnicastRemoteObject implements ILamp{
	public void light() throws java.rmi.RemoteException{
		System.out.println("Lampe allumée");
	}

	public void shut() throws java.ri.RemoteException{
		System.out.println("Lampe Eteinte")
	}

	public LampImpl() throws java.rmo.RemoteException{}

}


public class Server implements ILamp {
	public static void main(String args[]) {
		try {
			ILamp stub = new LampImpl();
			int rmiport = 6272;
			Registry registry = LocateRegistry.createRegistry(rmiport);
			registry.rebind("Lamp", lampe); //Enregistrement
			
		} catch (Exception e) {...}
		System.out.println("Server ready");
	}
} 

public CLient{
	public static void main(String args[]){
		String server = "localhost";
		int rmiport = 6272;
		try{
			Registry registry = LocateRegistry.getRegistry(serveur,rmiport);
			ILamp lamp = (Ilamp) registry.lookup("lamp");
			lamp.light();
			Thread.sleep(500);
			lamp.shut();
		}catch(Exception e){...}
	}
}


//Exerice 2



//Exportation manuelle
public interface IPi extends java.rmi.Remote{
	public double calcul() throws java.rmi.RemoteException;
}


public class Server implements IPi {
	public Server() {}
		public double calcul(int n) throws RemoteException {double resultat = 0.0;...;return resultat}
		public static void main(String args[]) {
		try {
			
			Server serveur = new Server();
			IPi stub = (IHello)UnicastRemoteObject.exportObject(serveur,0);
			Registry registry = LocateRegistry.getRegistry();	
			registry.rebind("piserveur", stub);
			System.out.println("Server ready");

		} catch (Exception e) {...}
	}
} 

public CLient{
	public static void main(String args[]){
		try{
			int n = Integer.parseInt(args[0]);
			IPi ipi = (Ipi) Naming("rmi://localhost/piserveur");
			double resultat = ipi.calcul(n);
			System.out.println("Resultat = " + resultat);
		}catch(Exception e){...}
	}
}


//Version 2

public interface IPi extends java.rmi.Remote{
	public viud calcul(int n,INotifier notifieur) throws RemoteException;
}

public interface INotifier extends Remote{
	public void notifier (double resultat) throws RemoteException;
}

public class Serveur implemnts Ipi{
	public void calcul (int n, INotifier notifieur) throws RemoteException{
		double resyktat = 0;
		notifieur.notifier(resultat);	
	}

	public static void main(String args[]){
		try{
			Serveur serveur = new Serveur();
			IPi stub = (IPi) UnicastRemoteObject.exportObject(serveur,0);
			Registry registry = LocateRegistry.getRegistry();
			registry.rebind("piserveur",stub);
			System.out.println("Serveur pret");
		}catch(Exception e) {...}
	}
}


public class notifierImpl implemnts INOtifier{
	double resultat = 0.0;
	public NotifierImpl() throws RemoteException{
		UnicastRemoteObject.exportObject(this);
	}
	public void notifier(double resultat) throws RemoteException{
		this.resultat = resultat;
	}
	public double getResultat(){return this.resultat;}

}

public class CLient extends Thread{
	public static void main(string args[]){ //1er thread
		int nt = new NotifierImpl();
		CLient client = new Client();
		client.start();
		Ipi ipi = (Ipi) Naming.lookup("rei/localhost/piserver");
		ipi.calcul(n,nt);
		System.out.println("Reuslat" + resultat);
	}catch(Exception e){...}
}

public void run(){...} // 2eme thread

































