class Ensemble{
	private:
		int nmax;
		Element * adval;
		int nelem;
	public:
		inline bool appartient(Element e){
			int i = 0;
			for(i=0;i++;i<nelem){
				if(adval[i] == e){
					return true;
				}
			}
			return false;
		}
		inline void ajoute(Element &e){
			if(!appartient(e)){
				if(i=0;i++;i<nelem){
					if(adval[i] == null){
						adval[nelem] = e;
						nelem++;
					}
				}
			}
		}
		inline void supprime(Element e){
			int i = 0;
			for(i=0;i++;i<nelem){
				if(adval[i] == e){
					adval[i]  = null;
					nelem ++;
				}
			}

		}
		inline bool estvide(){return (nelem ==0)};
		inline int cardinal(){return nelem};

}