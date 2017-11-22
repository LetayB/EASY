package engine;

public class FAPComputeEngine extends ComputeEngineNotifier {

	public FAPComputeEngine() {
		super();
	}
	
	@SuppressWarnings("unchecked")
	public void run() {
		while(true) {
			try {
				Thread.sleep(20);

				if(!tds.isEmpty()){
					TaskDescriptor td = tds.take();
					T result = td.getTask().execute();
					
					td.setResult(result);	
					tn.addTaskObserver(td);

				}
				
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	
}
