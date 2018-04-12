/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.Iterator;
import java.util.Scanner;

/**
 *
 * @author Jayed
 */
public class RawCodeForReader {
    JSONParser parser = new JSONParser();
    
    public RawCodeForReader () {
    }
    public static void main(String args[]) {
        Scanner scanner = new Scanner(System.in);
	String json = scanner.next();
	org.json.JSONObject jsonObject = new org.json.JSONObject(json);
	Iterator<String> keys = jsonObject.keys();
	while( keys.hasNext() ){
            String key = (String)keys.next(); // First key in your json object
            //System.out.println(key);
            org.json.JSONArray jsonArray = jsonObject.getJSONArray(key);
 	    for (int p = 0; p < jsonArray.length(); p++) {
 	        org.json.JSONObject object = jsonArray.optJSONObject(p);
		Iterator<String> iterator = object.keys();
                while(iterator.hasNext()) {
                    String currentKey = iterator.next();
                    System.out.println(key+": "+currentKey);
	        }
            }
	}
    }

}
