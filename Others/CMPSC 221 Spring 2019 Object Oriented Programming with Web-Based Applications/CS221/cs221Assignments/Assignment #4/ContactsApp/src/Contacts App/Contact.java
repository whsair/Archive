/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author wangh_qx6
 */
public class Contact { //create a contact object containing six attitude shows below and create set and get to each project
   
   private String firstName;
   private String lastName;
   private String email;
   private String homeAddress;
   private String phoneNumber;
   private String photo;
   
   public Contact() {this(" ", " ", " ", " ", " ", " ");} //initalize the contact 
   
   public Contact (String firstName, String lastName, String email, String homeAddress, String phoneNumber, String photo) {
      this.firstName = firstName;
      this.lastName = lastName;
      this.email = email;
      this.homeAddress = homeAddress;
      this.phoneNumber = phoneNumber;
      this.photo = photo;
      
   }
   
   public String getFirstName() {return firstName;}
   
   public void setFirstName(String firstName) {this.firstName = firstName;}
   
   public String getLastName() {return lastName;}
   
   public void setLastName(String lastName) {this.lastName = lastName;}
   
   public String getEmail() {return email;}
   
   public void setEmail(String email) {this.email = email;}
   
   public String getHomeAddress() {return homeAddress;}
   
   public void setHomeAddress(String homeAddress) {this.homeAddress = homeAddress;}
   
   public String getPhoneNumber() {return phoneNumber;}
   
   public void setPhoneNumber(String phoneNumber) {this.phoneNumber = phoneNumber;}
   
   public String getPhoto() {return photo;}
   
   public void setPhoto(String photo) {this.photo = photo;}
   
   @Override
   public String toString() {return String.format("%s %s", firstName, lastName);}
}
   
  
