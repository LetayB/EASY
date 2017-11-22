<h:selectOneMenu value ='#{languages.select}'> -> menu déroulant contenant la liste des languages

/******************************
*  LanguagesCOntroller.java   *
******************************/

//PROPERTY availables
public List<Language> getAvailables(){
	return languageSB.getLanguages();
}

public String doSUbmit(){
	if(newLanguage != null && !"".equals(newLanguage)){
		Language language = languageSB.createLanguage(newLanguage);
		select = language.etID();
		newLanguage = null;
	}
	if(select != null) return "edition";
	return null;
}

public Language.createLanguage(String name){
	Language language = null;
	try{
		query query = em.createQuery.("SELECT L FROM Language l WHERE l.name LIKE :param");
	}catch(NoResultException e){
		language = new Language();
		language.setName(name);
		em.persit(language);
	}
	return language;
}

@Local
public interface LanguageBeanController{
	public list<Language> getLanguages();
	public Language createLanguage(String name);
	public Language getLanguage(Long id);
}

@Stateless(name="languageSV")
public class LanguageBean implements LanguageBeanLocal{
	@PersistanceContext(unitName = "TutorialPU")
	private ENtityManager em;
	public List<Language> getLanguages(){
		return em.createQuery("SELECT L FROM Manguage l").getResultList();
	}
}

/******************************
*  LanguagesCOntroller.java   *
******************************/

...
@ManagedProperty("#{languages.select}");//APpel de la propriété select de LanguageCOntroller 
private Long actual; 
...

public void setActual(Long id){
	this.actual = id;
	this.language = languageSV.getLanguages(id);
	this.texts.addAll(this.language.getTexts());
	this.texts.add(new Text());
}

...
public String doAddText(){
	Text text = this.Texts.get(this.text.size()-1);
	if(Text.getValue() != null && ! "".equals(text.getKey()))
	this.language.setTexts(texts);
	language = languageSB.updat


}