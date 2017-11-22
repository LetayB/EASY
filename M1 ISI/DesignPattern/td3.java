public void setBPM(int bpm){
	this.bpm = bpm;
	sequenceur.setTempoInBPM;
	notifierObservateurBPM;
}

void evenemebtBattement(){
	notifierObservateurBattement;
}


public class VueDj{
	IModeleTempo modele;
	IControleur controleur;


	public VueDj(){
		this.modele = modele;
		this.controleur = controleur;
		modele.enregistrerObservateur((IObservateurBattement)this);
		modele.enregistrerObservateur((IObservateurBPM)this)
	}

	publc void creerVue(){

	}
}

public class ControleurTempo{
	IModeleTempo modele
	...
	public controleurTempo(...){
		this.modele = modele;
		vue = new Vue(modele);
	}
}











ProcessusComposite(){

	getMahineCount(){
		
	}
}






























