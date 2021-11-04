package motor.panel.project;

import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.URL;

public class MotorRequests {
	
	private String baseUrl;
	
	public void reqLeft() throws IOException {
		URL url = new URL(baseUrl + "/left");
		HttpURLConnection con = (HttpURLConnection) url.openConnection();
		con.setRequestMethod("GET");
		
		int responseCode = con.getResponseCode();
		System.out.println("GET Response Code :: " + responseCode);
	}
	
	public void reqRight() throws IOException {
		URL url = new URL(baseUrl + "/right");
		HttpURLConnection con = (HttpURLConnection) url.openConnection();
		con.setRequestMethod("GET");
		
		int responseCode = con.getResponseCode();
		System.out.println("GET Response Code :: " + responseCode);
	}
	
	public void reqStop() throws IOException {
		URL url = new URL(baseUrl + "/stop");
		HttpURLConnection con = (HttpURLConnection) url.openConnection();
		con.setRequestMethod("GET");
		
		int responseCode = con.getResponseCode();
		System.out.println("GET Response Code :: " + responseCode);
	}
	
	public void setURL(String url) {
		baseUrl = "http://" + url;
	}

}
