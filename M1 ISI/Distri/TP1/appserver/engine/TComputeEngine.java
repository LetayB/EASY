package engine;

import java.util.Calendar;

public class TComputeEngine extends ComputeEngineNotifier {

	public TComputeEngine() {
		super();
	}

	@SuppressWarnings("unchecked")
	public void run() {
		while(true) {
			try {
				Thread.sleep(20);
				
				if (!tds.isEmpty()) {
					TaskDescriptor td = tds.peek();
					int diff = td.getTime().get(Calendar.MINUTE) - Calendar.getInstance().get(Calendar.MINUTE);
					if(diff =< 0){
						if(diff =< -1){
							System.out.println("Data too old, task impossible");
							tds.remove(td);
						}
						else{
							td = tds.take();
							T result = td.getTask().execute();
							td.setResult(result);	
							tn.addTaskObserver(td);

						}
					}

				}
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
}
