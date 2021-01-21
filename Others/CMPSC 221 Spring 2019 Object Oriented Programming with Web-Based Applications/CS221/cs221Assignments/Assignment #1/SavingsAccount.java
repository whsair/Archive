/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * 1/29/2019
 * Programming Assignment #1
 * CMPSC 221 Section 001
 * Author: Hongshuo Wang
 */
public class SavingsAccount {

   private static double annualInterestRate = 0.00;
   private double savingsBalance = 0.00;
   
   public SavingsAccount(double savingsBalance) {
      this.savingsBalance = savingsBalance;
   }
      
   public void calculateMonthlyInterest() {
      double annualInterest = savingsBalance * annualInterestRate;
      double monthlyInterest = annualInterest / 12;
      savingsBalance += monthlyInterest;  
   }
   
   public static void setInterestRate(double newAnnualInterestRate) {
      annualInterestRate  = newAnnualInterestRate;
   }
   
   public void setSavingsBalance(double savingsBalance) {
      this.savingsBalance = savingsBalance;
   }
   
   public double getSavingsBalance() {
      return savingsBalance;
   }
   
   public static void main(String[] args) {
      
      SavingsAccount saver1 = new SavingsAccount(2000.00);
      SavingsAccount saver2 = new SavingsAccount(3000.00);
      setInterestRate(0.04);
      
      System.out.printf("%s\t%s\t%s%n", "Month", "Saver1", "Saver2");
      //System.out.printf("%s%9s%8s%n", "Month", "Saver1", "Saver2");
      
      for (int month = 1; month <= 12; ++month) {
         saver1.calculateMonthlyInterest();
         saver2.calculateMonthlyInterest();
         System.out.printf("%d\t%.2f\t%.2f%n", month, saver1.getSavingsBalance(), saver2.getSavingsBalance());
         //System.out.printf("%-8d%.2f%8.2f%n", month, saver1.getSavingsBalance(), saver2.getSavingsBalance());
      }
      
      setInterestRate(0.05);
      int nextMonth = 13;
      saver1.calculateMonthlyInterest();
      saver2.calculateMonthlyInterest();
      System.out.printf("%d\t%.2f\t%.2f%n", nextMonth, saver1.getSavingsBalance(), saver2.getSavingsBalance());
      //System.out.printf("%-8d%.2f%8.2f%n", nextMonth, saver1.getSavingsBalance(), saver2.getSavingsBalance());
      
   }
   
}