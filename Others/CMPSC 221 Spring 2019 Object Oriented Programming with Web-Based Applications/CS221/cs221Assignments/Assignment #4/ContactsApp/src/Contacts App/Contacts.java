/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author wangh_qx6
 */
// Maintains a List<Contact>

import java.util.ArrayList;
import java.util.List;
import javax.xml.bind.annotation.*;

public class Contacts {
   //@XmlElement specifices XML element name for each object in the list 
   @XmlElement(name="contact") 
    
   private List<Contact> contacts = new ArrayList<>(); //store contacts
   

   // returns the List<Contacts>
   public List<Contact> getContacts() {return contacts;}
}
