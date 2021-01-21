/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package pizza;

import java.util.Date;
import javax.enterprise.context.SessionScoped;
import java.io.Serializable;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.text.SimpleDateFormat;
import java.util.HashMap;
import javax.annotation.Resource;
import javax.annotation.sql.DataSourceDefinition;
import javax.faces.bean.ManagedBean;
import javax.sql.DataSource;
import javax.sql.rowset.CachedRowSet;
import javax.sql.rowset.RowSetProvider;

/**
 *
 * @author wangh_qx6
 */
@ManagedBean(name = "pizzaBean")
@SessionScoped

// define the data source 
@DataSourceDefinition(
        name = "java:global/jdbc/pizzadb",
        className = "org.apache.derby.jdbc.ClientDataSource",
        url = "jdbc:derby://localhost:1527/pizzadb",
        databaseName = "pizzadb",
        user = "app",
        password = "app")

public class pizzaBean implements Serializable {

   /**
    * Creates a new instance of pizzaBean
    */
   // instance variables that represent one order
   private String firstName;
   private String lastName;
   private String phoneNumber;
   private String emailAddress;
   private static String pizza;
   private static String sides;
   private static String drink;
   private static int drinkID; //corresponding to the drink table
   private static int drinkQuantity;
   private static int sidesID; //corresponding to the sides table 
   private static int sidesQuantity;
   private static int pizzaID; //corresponding to the pizza table
   private static int pizzaQuantity;
   private static int orderID; // prime key of the order and gotton by random number
   private double pizzaPrice; //corresponding to the pizza table
   private double sidesPrice; //corresponding to the sides table 
   private double drinkPrice; //corresponding to the drink table

   // map of drink to drink id 
   private static final HashMap<String, Integer> drinkMap
           = new HashMap<>();

   // initialize drinkmap
   static {
      drinkMap.put("pepsi", 1);
      drinkMap.put("bottleWater", 2);
   }

   // map of sides to sidesid
   private static final HashMap<String, Integer> sidesMap
           = new HashMap<>();

   // initialize sidesMap
   static {
      sidesMap.put("cheesesticks", 1);
      sidesMap.put("buffaloWings", 2);
   }

   // map of pizza to pizza id
   private static final HashMap<String, Integer> pizzaMap
           = new HashMap<>();

   // initialize pizzamap
   static {
      pizzaMap.put("pepperoniPizza", 1);
      pizzaMap.put("sausagePizza", 2);
      pizzaMap.put("cheesePizza", 3);

   }

   // allow the server to inject the DataSource
   @Resource(lookup = "java:global/jdbc/pizzadb")
   DataSource dataSource;

   // get the first name
   public String getFirstName() {
      return firstName;
   }

   // set the first name
   public void setFirstName(String firstName) {
      this.firstName = firstName;
   }

   // get the last name
   public String getLastName() {
      return lastName;
   }

   // set the last name
   public void setLastName(String lastName) {
      this.lastName = lastName;
   }

   //set phone number
   public void setPhoneNumber(String phoneNumber) {
      this.phoneNumber = phoneNumber;
   }

   //get phone number
   public String getPhoneNumber() {
      return phoneNumber;
   }

   //get Email address
   public String getEmailAddress() {
      return emailAddress;
   }

   //set Email address 
   public void setEmailAddress(String emailAddress) {
      this.emailAddress = emailAddress;
   }

   //set the kind of ordered pizza
   public void setPizza(String pizza) {
      this.pizza = pizza;
      setPizzaID(pizza);
   }

   //set the kind of ordered drink
   public void setDrink(String drink) {
      this.drink = drink;
      setDrinkID(drink);
   }

   //set the kind of ordered sides   
   public void setSides(String sides) {
      this.sides = sides;
      setSidesID(sides);
   }

   //get the name of ordered pizza 
   public String getPizza() {
      return pizza;
   }

   //get the name of ordered drink
   public String getDrink() {
      return drink;
   }

   //get the name of ordered sides
   public String getSides() {
      return sides;
   }

   //set drink's id
   public void setDrinkID(String drink) {
      this.drinkID = drinkMap.get(drink);
   }

   //get drink's id
   public int getDrinkID() {
      return drinkID;
   }

   //set drink's quantity
   public void setDrinkQuantity(int drinkQuantity) {
      this.drinkQuantity = drinkQuantity;
   }

   //get drink quantity
   public int getDrinkQuantity() {
      return drinkQuantity;
   }

   //set sides
   public void setSidesID(String sides) {
      this.sidesID = sidesMap.get(sides);
   }

   //Get sides id
   public int getSidesID() {
      return sidesID;
   }

   //set sides' quantity
   public void setSidesQuantity(int sidesQuantity) {
      this.sidesQuantity = sidesQuantity;
   }

   //get sides' quantity
   public int getSidesQuantity() {
      return sidesQuantity;
   }

   //set pizza
   public void setPizzaID(String pizza) {
      this.pizzaID = pizzaMap.get(pizza);
   }

   //get pizzaID
   public int getPizzaID() {
      return pizzaID;
   }

   //set pizza's quantity
   public void setPizzaQuantity(int pizzaQuantity) {
      this.pizzaQuantity = (int) pizzaQuantity;
   }

   //get pizza's quantity
   public int getPizzaQuantity() {
      return pizzaQuantity;
   }

   //get order's id 
   public void setOrderID() {
      Date orderDate;
      orderDate = new Date();
      this.orderID = (int) (orderDate.getTime() & 0x0000000000ffffffL);
   }

   public int getOrderID() {
      return orderID;
   }

   //get order time 
   public String getOrderTime() {
      StringBuffer stringBuffer = new StringBuffer();
      Date now = new Date();

      SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");

      return simpleDateFormat.format(new Date());
   }

   //get the amount 
   public double getTotalPrice() throws SQLException {
      // check whether dataSource was injected by the server
      if (dataSource == null) {
         throw new SQLException("Unable to obtain DataSource");
      }

      // obtain a connection from the connection pool
      Connection connection = dataSource.getConnection();

      // check whether connection was successful
      if (connection == null) {
         throw new SQLException("Unable to connect to DataSource");
      }

      try {
         // find corresponding pizza's price
         PreparedStatement pizza = connection.prepareStatement(
                 "SELECT * " + "FROM PIZZA " + "Where PIZZAID = ?");
         pizza.setInt(1, getPizzaID());

         CachedRowSet rowPizzaSet
                 = RowSetProvider.newFactory().createCachedRowSet();
         rowPizzaSet.populate(pizza.executeQuery());

         if (rowPizzaSet.next()) {
            this.pizzaPrice = rowPizzaSet.getDouble("PRICE");

         }

         // find corresponding drinks' price
         PreparedStatement drink = connection.prepareStatement(
                 "SELECT * " + "FROM DRINKS " + "Where DRINKID = ?");
         drink.setInt(1, getDrinkID());

         CachedRowSet rowDrinkSet
                 = RowSetProvider.newFactory().createCachedRowSet();
         rowDrinkSet.populate(drink.executeQuery());

         if (rowDrinkSet.next()) {
            this.drinkPrice = rowDrinkSet.getDouble("PRICE");
         }

         //find corresponding sides' price
         PreparedStatement sides = connection.prepareStatement(
                 "SELECT * " + "FROM SIDES " + "Where SIDESID = ?");
         sides.setInt(1, getSidesID());

         CachedRowSet rowSidesSet
                 = RowSetProvider.newFactory().createCachedRowSet();
         rowSidesSet.populate(sides.executeQuery());

         if (rowSidesSet.next()) {
            this.sidesPrice = rowSidesSet.getDouble("PRICE");
         }

         // multiply each corresponding quantity and add up in order to get the total amount
         return sidesPrice * getSidesQuantity() + drinkPrice * getDrinkQuantity() + pizzaPrice * getPizzaQuantity();

      } finally {
         connection.close(); // return this connection to pool
      }
   }

   // return a ResultSet of entries
   public ResultSet getOrders() throws SQLException {
      // check whether dataSource was injected by the server
      if (dataSource == null) {
         throw new SQLException("Unable to obtain DataSource");
      }

      // obtain a connection from the connection pool
      Connection connection = dataSource.getConnection();

      // check whether connection was successful
      if (connection == null) {
         throw new SQLException("Unable to connect to DataSource");
      }

      try {
         // create a PreparedStatement to insert a new order entry
         PreparedStatement getOrders = connection.prepareStatement(
                 "SELECT *"
                 + "FROM ORDERS ORDER BY ORDERTIME DESC"); //get the orders in the descending ordering

         CachedRowSet rowSet
                 = RowSetProvider.newFactory().createCachedRowSet();
         rowSet.populate(getOrders.executeQuery()); //update the query
         return rowSet; //return the rowset
      } finally {
         connection.close(); // return this connection to pool
      }
   }

   // save a new address book entry
   public String save() throws SQLException {
      // check whether dataSource was injected by the server
      if (dataSource == null) {
         throw new SQLException("Unable to obtain DataSource");
      }

      // obtain a connection from the connection pool
      Connection connection = dataSource.getConnection();

      // check whether connection was successful
      if (connection == null) {
         throw new SQLException("Unable to connect to DataSource");
      }

      try {

         //before we save our data, first we generate an order id
         this.setOrderID();

         // insert the entry
         // create a PreparedStatement to insert a ordered items entry
         PreparedStatement addCustomer
                 = connection.prepareStatement(
                         "INSERT INTO CUSTOMERS (SELECT ? as PHONENUMBER, ? as FIRSTNAME, ? as LASTNAME, ? as EMAIL FROM CUSTOMERS WHERE PHONENUMBER = ? HAVING count(*)=0)");

         // specify the PreparedStatement's arguments
         addCustomer.setString(1, getPhoneNumber());
         addCustomer.setString(2, getFirstName());
         addCustomer.setString(3, getLastName());
         addCustomer.setString(4, getEmailAddress());
         addCustomer.setString(5, getPhoneNumber()); //if the phonenumber exist in the table then do not update the entry

         // create a PreparedStatement to insert a new order entry
         PreparedStatement addOrder
                 = connection.prepareStatement(
                         "INSERT INTO ORDERS "
                         + "SELECT ? as ORDERID, ? as PHONENUMBER, ? as ORDERTIME, ? as TOTALPRICE "
                         + "FROM ORDERS "
                         + "WHERE ORDERID = ? AND PHONENUMBER = ? "
                         + "HAVING count(*)=0");

         // specify the PreparedStatement's arguments
         addOrder.setInt(1, getOrderID());
         addOrder.setString(2, getPhoneNumber());
         addOrder.setString(3, getOrderTime());
         addOrder.setDouble(4, getTotalPrice());
         addOrder.setInt(5, getOrderID());
         addOrder.setString(6, getPhoneNumber()); //if the ordernumber and order id existed then do not update the entry

         // create a PreparedStatement to insert a new ordereditems entry
         PreparedStatement addOrderedItems
                 = connection.prepareStatement(
                         "INSERT INTO ORDEREDITEMS (SELECT ? as ORDERID, ? as PIZZAID, ? as PIZZAQN, ? as SIDESID, ? as SIDESQN, ? as DRINKID, ? as DRINKQN FROM ORDEREDITEMS WHERE ORDERID = ? HAVING count(*)=0)");

         // specify the PreparedStatement's arguments
         addOrderedItems.setInt(1, getOrderID());
         addOrderedItems.setInt(2, getPizzaID());
         addOrderedItems.setInt(3, getPizzaQuantity());
         addOrderedItems.setInt(4, getSidesID());
         addOrderedItems.setInt(5, getSidesQuantity());
         addOrderedItems.setInt(6, getDrinkID());
         addOrderedItems.setInt(7, getDrinkQuantity());
         addOrderedItems.setInt(8, getOrderID()); //if the orderid existed in the table then do not update the entry

         addCustomer.executeUpdate(); // create a PreparedStatement to upate customers' information
         addOrderedItems.executeUpdate(); // create a PreparedStatement to upate orderedItems' information
         addOrder.executeUpdate(); // create a PreparedStatement to upate orderss' information

         return "index"; // go back to index.xhtml page
      } finally {
         connection.close(); // return this connection to pool
      }
   }

}
