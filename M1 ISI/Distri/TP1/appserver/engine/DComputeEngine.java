package engine;

public class DComputeEngine implements ComputeEngine {

	public <T> T execute(TaskDescriptor<T> td) {
		T result = td.getTask().execute();
		td.setResult(result);
		return result;
	}

}
