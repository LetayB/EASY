int Ensemble::appartient(Element nb){
	int i = 0;
	while((i<nelem)&&adval[i]!=nb))i++;
	return (i<nelem);
}

int Ensemble::cardinal(){
	return nelem;
}

void Ensemble::supprimer(Element nb){
	int i=0;
	while((i<nelem)&&(adval[i]!=nb))i++;
	if(i<nelem){
		for(;i<nelem;i++)adval[i]=adval[i+1];
			nelem--;

	}
}

Ensemble & Ensemble::operator[](int i){
	if(i<0||i>=nelem){
		cout << "index hors limite"<< i << endl;
		exit(1);
	}
	return adval[i];
}

Element Ensemble::operator[](int i)const{
	if(i<0||i>=nelem){
		cout << "index hors limite"<< i << endl;
		exit(1);
	}
	return adval[i];
}

ostream &operator << (ostream & flux,const Ensemble& e){
	for(int i=0;i<e.nelem;i++)flux <<e.adval[i] <<" ";
	return flux << endl;
}

istream &operator >>(istream & flux,const Ensemble & e){
	int nb;
	cout << "Donnez SVP le nombre d'element de l'ensemble :";
	flux >> nb;
		Element n;
		for(i=0)
}