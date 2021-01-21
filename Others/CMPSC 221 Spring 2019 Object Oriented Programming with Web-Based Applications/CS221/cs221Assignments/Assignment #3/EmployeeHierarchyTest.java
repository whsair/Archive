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
         
         System.out.println("\nEmployee information after raises.");
         
         //Assingment #3
         //make the most of polymorphism to call the common method raised in order to update all the elements.
         Employee[] employee = new Employee[] {employee1, employee2, employee3, employee4, employee5}; //declaring a superclass Employee array and put all the declared different types of employee into Array
         for (Employee currentEmployee : employee) {        // use a for hanced loop to update raise rate into all the element in the array
            
            if (currentEmployee instanceof SalariedEmployee) { //check weather the current Employee is salariedEmployee
               currentEmployee.raise(0.04); //if it is salariedEmployee, raise 0.04 
              
            } else {
               currentEmployee.raise(0.02); //otherwise increase just 0.02
            }
            System.out.print(currentEmployee); //print out each element what we got after updaing
         }
            
         
         
      }
      
      catch (IllegalArgumentException e) {
         System.out.printf("%nException while initializing t6: %s%n", e.getMessage());
      }
      
   }
      
   
}
