/*
 *                         OpenSplice DDS
 *
 *   This software and documentation are Copyright 2006 to 2010 PrismTech
 *   Limited and its licensees. All rights reserved. See file:
 *
 *                     $OSPL_HOME/LICENSE 
 *
 *   for full copyright notice and license terms. 
 *
 */
package org.opensplice.cm.meta;

import java.util.ArrayList;

/**
 * Represents a structure field in a type.
 * 
 * @date Jun 14, 2004
 */
public class MetaStruct extends MetaField{
    /**
     * The list of members of the structure.(<MetaField>)
     */ 
    private ArrayList members;
    
    /**
     * Creates a new structure field.
     * 
     * @param name The name of the structure.
     * @param typeName The type name of the structure.
     * @param _members The members of the structure.
     */ 
    public MetaStruct(String name, String typeName, ArrayList _members){
        super(name, typeName);
        members = _members;
    }
    
    public MetaField[] getFields(){
        MetaField[] result = new MetaField[members.size()];
    
        for(int i=0; i<members.size(); i++){
            result[i] = (MetaField)(members.get(i));
        }
        return result;
    
    }
    
    public String toString(){
        String s = super.toString();
        
        for(int i=0; i<members.size(); i++){
            s += "\n" + members.get(i).toString();
        }
        return s;
    }
       
    public MetaField getField(String fieldName){
        MetaField result;
    
        for(int i=0; i<members.size(); i++){
            result = (MetaField)(members.get(i));
        
            if(fieldName.equals(result.getName())){
                return result;
            }
        }
        return null;
    }
    
    public ArrayList getFieldNames(){
        MetaField field;
        ArrayList result = new ArrayList();
        ArrayList names;
    
        for(int i=0; i<members.size(); i++){
            field = (MetaField)(members.get(i));
            names = field.getFieldNames();
            
            for(int j=0; j<names.size(); j++){
                result.add(name + "." + ((String)names.get(j)));
            }
        }
        
        return result;
    }
}
