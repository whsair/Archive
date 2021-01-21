/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * Hongshuo Wang Assignment 02 CMPSC 001
 */
public class HourlyEmployee extends Employee {
   
   private double hourlyWage;
   private double hoursWorked;
    
   
   public HourlyEmployee(String firstName, String lastName, String socialSecurityNumber, double hourlyWage, double hoursWorked) {
      super(firstName, lastName, socialSecurityNumber);
      
      if (hourlyWage < 0.0) {
         throw new IllegalArgumentException("Hourly Wage must be greater than 0.0.");
      }
      
      if (hoursWorked < 1.0 || hoursWorked > 168.0) {
         throw new IllegalArgumentException("Worked hours must be greater between 1 to 168.");
      }
      
      this.hourlyWage = hourlyWage;
      this.hoursWorked = hoursWorked;  
      
   }
   
   public double getHourlyWage() {return hourlyWage;}
   
   public double getHoursWorked() {return hoursWorked;}
   
   public void setHourlyWage(double hourlyWage) {
      
      if (hourlyWage < 0.0) {
         throw new IllegalArgumentException("Hourly Wage must be greater than 0.0.");
      }
      
      this.hourlyWage = hourlyWage;
     
   }
   
   public void setHoursWorked(double hoursWorked) {
      
      if (hoursWorked < 0.0 || hoursWorked > 168.0) {
         throw new IllegalArgumentException("Worked hours must be greater between 1 to 168.");
      }
      this.hoursWorked = hoursWorked;
      
   }
   
   @Override
   public double earnings() {
      
      if (hoursWorked <= 40.0) {
         return hoursWorked * hourlyWage;
      } else {
         return 40 * hourlyWage + (hoursWorked - 40.0) * 1.5 * hourlyWage;
      }
      
   }
   
   @Override
   public String toString() {
      return String.format("%s%s  %s: %.2f%n  %s: %.2f%n  %s: $%.2f%n", 
         "Hourly Employee", super.toString(),
         "Hourly Wage", getHourlyWage(), 
         "Hours Worked", getHoursWorked(),
         "Earnings", earnings());
   } 
   
   @Override
   public void raise(double percent) {
      this.setHourlyWage((1 + percent)* this.getHourlyWage());  //increase the hourly wage by the percent specified.
   }
   
   
   
   
}
