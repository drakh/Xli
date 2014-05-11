class HttpWrappedResponse {
	public Object body;
    public long functionPointer;
    public String[] headers;
    public int responseCode;
    public String responseMessage;

    HttpWrappedResponse(Object body, String[] headers, int responseCode, String responseMessage, long fpointer)
    {
    	this.body = body;
    	this.functionPointer = fpointer;
    	this.headers = headers;
    	this.responseCode = responseCode;
    	this.responseMessage = responseMessage;
    }
} 