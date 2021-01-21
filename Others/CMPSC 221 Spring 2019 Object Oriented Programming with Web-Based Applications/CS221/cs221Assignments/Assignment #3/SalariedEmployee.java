/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * Hongshuo Wang Assignment 02 CMPSC 001
 */
public class SalariedEmployee extends Employee{
   
   private double salary;
   
   public SalariedEmployee(String firstName, String lastName, String socialSecurityNumber, double salary) {
      super(firstName, lastName, socialSecurityNumber);
      
      if (salary < 0.0) {
         throw new IllegalArgumentException("Salary must be >= 0.0");
      }
      
      this.salary = salary;
      
   }
   
   public double getSalary() {return salary;}

   public void setSalary(double salary) {
      if (salary < 0.0) {
         throw new IllegalArgumentException("Salary must be >= 0.0");
      }
      
      this.salary = salary;
      
   }
   
   @Override
   public double earnings() {return getSalary();}
   
   @Override
   public String toString() {
      return String.format("%s%s  %s: %.2f%n  %s: $%.2f%n", 
         "Salaried Employee", super.toString(),
         "Salary", getSalary(),
         "Earnings", earnings());
   } 
   
   @Override
   public void raise(double percent) {
      this.setSalary((1 + percent) * this.getSalary()); // increase the salary by the percent specified. 
   }
    
}
