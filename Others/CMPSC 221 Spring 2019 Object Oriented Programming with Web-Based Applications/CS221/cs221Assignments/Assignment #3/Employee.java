/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author wangh_qx6
 */
public abstract class Employee {  //creating a abstract class in order to help us handle the same attitude and method and also simplify our coding
   private String firstName;
   private String lastName;
   private String socialSecurityNumber;
   
   public Employee(String firstName, String lastName, String socialSecurityNumber){
      this.firstName = firstName;
      this.lastName = lastName;
      this.socialSecurityNumber = socialSecurityNumber;  
   }
   
   public String getFirstName(){return firstName;}
   
   public String getLastName(){return lastName;}
   
   public String getSocialSecurityNumber(){return socialSecurityNumber;}
   
   public void setFirstName(String firstName) {
      this.firstName = firstName;
   }
   
   public void setLastName(String lastName) {
      this.lastName = lastName;
   }
   
   public void setSocialSecurityNumber(String socialSecurityNumber) {
      this.socialSecurityNumber = socialSecurityNumber;
   }
   
   // all the above are the common attitudes and methord are all the subclass needed.
   
   
   
   public String toString() {
      return String.format(": %s %s%s: %s%n", 
              getFirstName(), getLastName(), 
              " with ssn", getSocialSecurityNumber());
   }

   public abstract double earnings(); //each subclass has to have their own unique earnings so we definte abstract to force them overwrite the method 
   
   public abstract void raise(double percent); //another method that need subclass overwrite which increase the employee's salary by specific rate. 
   
}
