/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
import org.json.*;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.Iterator;
import java.io.File;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.table.DefaultTableModel;
/**
 *
 * @author Jayed
 */
public class JsonReader {
    private File file;
    JSONParser parser = new JSONParser();
    
    public JsonReader(File file) {
        this.file=file;
    }
    
    public void showTableData(DefaultTableModel model) {
        Object obj = null;
        try {
            obj = parser.parse(new FileReader(file));
        } catch (IOException ex) {
            //Logger.getLogger(JsonReader.class.getName()).log(Level.SEVERE, null, ex);
        } catch (ParseException ex) {
            //Logger.getLogger(JsonReader.class.getName()).log(Level.SEVERE, null, ex);
        }

            JSONObject jsonObject = (JSONObject) obj;
            
            org.json.JSONObject jsonObject1 = new org.json.JSONObject(jsonObject);
            
            Iterator<String> keys = jsonObject1.keys();
            //boolean x = false;
            while( keys.hasNext() ){
                String key = (String)keys.next(); // First key in your the object
                //System.out.println(key);
                org.json.JSONArray jsonArray = jsonObject1.getJSONArray(key);
                for (int p = 0; p < jsonArray.length(); p++) {
                    org.json.JSONObject object = jsonArray.optJSONObject(p);
                    Iterator<String> iterator = object.keys();
                    while(iterator.hasNext()) {
                        //x= true;
                        String currentKey = iterator.next();
                        if(currentKey.equals("quantity")) {
                            continue;
                        } else {
                            String amount = (String)object.get(currentKey);
                            //System.out.println(currentKey+" asd");
                            model.addRow(new Object[]{key, currentKey, amount});
                        }
                    }
                }
            }
//        try {
//
//            
//
//        } catch (FileNotFoundException e) {
//            e.printStackTrace();
//        } catch (IOException e) {
//            e.printStackTrace();
//        } catch (ParseException e) {
//            e.printStackTrace();
//        }
    }
    
    public void showStorageData(DefaultTableModel model) {
        try {

            Object obj = parser.parse(new FileReader(file));
            //DefaultTableModel model = (DefaultTableModel) storageTable.getModel();

            JSONObject jsonObject = (JSONObject) obj;
            
            org.json.JSONObject jsonObject1 = new org.json.JSONObject(jsonObject);
            
            Iterator<String> keys = jsonObject1.keys();
            //boolean x = false;
            while( keys.hasNext() ){
                String key = (String)keys.next(); // First key in the json object
                //System.out.println(key);
                org.json.JSONArray jsonArray = jsonObject1.getJSONArray(key);
                for (int p = 0; p < jsonArray.length(); p++) {
                    org.json.JSONObject object = jsonArray.optJSONObject(p);
                    Iterator<String> iterator = object.keys();
                    while(iterator.hasNext()) {
                        String currentKey = iterator.next();
                        if(currentKey.equals("quantity")) {
                            String amount = (String)object.get(currentKey);
                            //System.out.println(currentKey+" asd");
                            model.addRow(new Object[]{key, amount});
                        }
                   }
                }
                
            }

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (ParseException e) {
            e.printStackTrace();
        }
    }
}
