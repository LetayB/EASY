/* Exercice 4 */

public class BouilleurChocolat{
	private boolean vide;
	private boolean bouilli;

	public BouilleurChocolat(){
		vide = true;
		bouilli = false;
	}

	public void remplir(){
		if(estVide()){
			vide = false;
			bouilli = false;
		}
	}

	private static BouilleurChocolat instance = null;
	protected BouilleurChocolat() {
      // Exists only to defeat instantiation.
	}

	public static synchronized BouilleurChocolat getInstance() {
		if(instance == null) {
			instance = new BouilleurChocolat();
		}
		return instance;
	}
}

/* Exercice 2 */

public interface Observable {

    public void attach(Observateur o);

    public void detach(Observateur o);

    public void notifyObservers();
}

public interface Observateur {

    public void MiseAJour();
    
}

/* Exerice 1 */

public class Context {
   private Strategy strategy;

   public Context(Strategy strategy){
      this.strategy = strategy;
   }

   public Firework executeStrategy(){
      return strategy.recommend();
   }
}

public interface Strategy {
   public Firework recommend();
}

class AdapterLikeMySTuff(){

}

class AdapterRel8 implements Stretegy(){

}


/* Exercice 3 */





























