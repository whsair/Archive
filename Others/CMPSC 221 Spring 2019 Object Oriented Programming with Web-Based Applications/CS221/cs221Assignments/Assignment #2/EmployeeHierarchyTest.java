/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * Hongshuo Wang Assignment 02 CMPSC 001
 */
public class EmployeeHierarchyTest {

   /**
    * @param args the command line arguments
    */
   public static void main(String[] args) {
      try {
         CommissionEmployee employee1 = new CommissionEmployee("Fred", "Jones", "111-11-1111", 2000.0, .05);
         BasePlusCommissionEmployee employee2 = new BasePlusCommissionEmployee("Sue", "Smith", "222-22-2222", 3000.0, .05, 300);
         SalariedEmployee employee3 = new SalariedEmployee("Shan", "Yang", "333-33-3333", 1150.0);
         HourlyEmployee employee4 = new HourlyEmployee("Ian", "Tanning", "444-44-4444", 15.0, 50);
         HourlyEmployee employee5 = new HourlyEmployee("Angela", "Domchek", "555-55-5555", 20.0, 40);
         System.out.printf("%s%s%s%s%s", employee1, employee2, employee3, employee4, employee5);  
         
      }
      
      catch (IllegalArgumentException e) {
         System.out.printf("%nException while initializing t6: %s%n", e.getMessage());
      }
      
   }
      
   
}
