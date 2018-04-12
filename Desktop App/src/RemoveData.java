
import java.awt.HeadlessException;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Iterator;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.json.JSONException;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Jayed
 */
public class RemoveData {
    private String name;
    private String time;
    private String pills;
    private File file;
    JSONParser parser = new JSONParser();

    public RemoveData() {
    }

    public RemoveData(String name, String time, String pills, File file) {
        this.name = name;
        this.time = time;
        this.pills = pills;
        this.file = file;
    }
    
    public void removeData() {
        try {                                           
            // TODO add your handling code here:
            Object obj = parser.parse(new FileReader(file));
            JSONObject jsonObject = (JSONObject) obj;
            
            org.json.JSONObject jsonObject1 = new org.json.JSONObject(jsonObject);
            
            
            //JOptionPane.showMessageDialog(this, "jjj");
            BufferedReader br = null;
            BufferedWriter bw = null;
            
            try {
                bw = new BufferedWriter (new FileWriter(file));
                
                
                bw.write("{");
                //JOptionPane.showMessageDialog(this, "f");
                Iterator<String> keys = jsonObject1.keys();
                JSONObject medName = new JSONObject();
                while( keys.hasNext() ){
                    String line = "";
                    String key = (String)keys.next(); // First key in your the object
                    //JOptionPane.showMessageDialog(this, key);
                    
                    org.json.JSONArray jsonArray = jsonObject1.getJSONArray(key);
                    JSONObject timePill = new JSONObject();
                    if(jsonArray.length() < 1) {
                        continue;
                    } else {
                        for (int p = 0; p < jsonArray.length(); p++) {
                            org.json.JSONObject object = jsonArray.optJSONObject(p);
                            Iterator<String> iterator = object.keys();

                            while(iterator.hasNext()) {
                                String currentKey = iterator.next();
                                String amount = (String)object.get(currentKey);

                                if(key.equals(this.getName()) &&
                                        currentKey.equals(this.getTime()) &&
                                        amount.equals(this.getPills())){
                                    continue;
                                }
                                else {
                                    timePill.put(currentKey, amount);
                                }
                            }
                        }
                    }
                    
                    JSONArray timePillList = new JSONArray();
                    timePillList.add(timePill);
                    //System.out.println("array: "+timePillList);
                    medName.put("\""+key+"\"", timePillList);
                    //System.out.println(medName.put("\""+key+"\"", timePillList));
                    line += "\""+key+"\""+":"+timePillList+",";
                    //System.out.println(line);
                    bw.write(line);
                    
                }
                bw.write("}");
            } catch (IOException | HeadlessException | JSONException e) {
                e.printStackTrace();
            } finally {
                try {
                    if(bw != null) {
                        bw.close();
                    }
                } catch (Exception e) {
                    //
                }
            }
            
            BasicConsole bj = new BasicConsole(file);
            bj.setVisible(true);
            bj.showData();
        } catch (IOException ex) {
            Logger.getLogger(EditStorage.class.getName()).log(Level.SEVERE, null, ex);
        } catch (ParseException ex) {
            Logger.getLogger(EditStorage.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getTime() {
        return time;
    }

    public void setTime(String time) {
        this.time = time;
    }

    public String getPills() {
        return pills;
    }

    public void setPills(String pills) {
        this.pills = pills;
    }

    public File getFile() {
        return file;
    }

    public void setFile(File file) {
        this.file = file;
    }
    
    
}
