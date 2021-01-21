// DisplayQueryResultsController.java
// Controller for the DisplayQueryResults app

import java.sql.SQLException;
import java.time.LocalDate;
import java.util.regex.PatternSyntaxException;
import javafx.embed.swing.SwingNode;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import javafx.scene.layout.BorderPane;
import javafx.scene.control.DatePicker;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.RowFilter;
import javax.swing.table.TableModel;
import javax.swing.table.TableRowSorter;

public class DisplayQueryResultsController {

   @FXML
   private BorderPane borderPane;

   @FXML
   private TextField filterTextField;

   @FXML
   private TextField phoneTextField;

   @FXML
   private TextField lastNameTextField;

   @FXML
   private TextArea queryTextArea;

   @FXML
   private DatePicker fromDatePicker;

   @FXML
   private DatePicker toDatePicker;

   // database URL, username and password
   private static final String DATABASE_URL = "jdbc:derby://localhost:1527/pizzadb";
   private static final String USERNAME = "app";
   private static final String PASSWORD = "app";

   // default query retrieves all data from orders table
   private static final String DEFAULT_QUERY = "SELECT * FROM orders";

   //Q1:   query “number of orders and income per day” 
   private static final String NUM_ORDERS_PRICE_PER_DAY_QUERY
           = "select date(ordertime) as Order_Day, count(*) as Order_Number, sum(totalprice) as   Order_Total \n"
           + "from orders \n"
           + "group by date(ordertime) \n"
           + "order by order_day desc";

   //Q3:   query the database by phonenumber (entered in a textfield)
   private static final String ORDER_SEARCH_BY_PHONENUMBER = "select firstname, lastname, ordertime, pizza.pizzades, ordereditems.pizzaqn, sides.sidesdes, ordereditems.sidesqn, drinks.drinkdes, ordereditems.drinkqn\n"
           + "from customers\n"
           + "inner join orders on customers.phonenumber = orders.phonenumber\n"
           + "inner join ordereditems on ordereditems.orderid = orders.orderid\n"
           + "inner join pizza on pizza.pizzaid = ordereditems.pizzaid\n"
           + "inner join sides on sides.sidesid=ordereditems.sidesid\n"
           + "inner join drinks on drinks.drinkid=ordereditems.drinkid\n"
           + "where customers.phonenumber =";

   //Q4:   query the database by lastName (entered in a textfield)
   private static final String ORDER_SEARCH_BY_LASTNAME = "select firstname, lastname, ordertime, pizza.pizzades, ordereditems.pizzaqn, sides.sidesdes, ordereditems.sidesqn, drinks.drinkdes, ordereditems.drinkqn\n"
           + "from customers\n"
           + "inner join orders on customers.phonenumber = orders.phonenumber\n"
           + "inner join ordereditems on ordereditems.orderid = orders.orderid\n"
           + "inner join pizza on pizza.pizzaid = ordereditems.pizzaid\n"
           + "inner join sides on sides.sidesid=ordereditems.sidesid\n"
           + "inner join drinks on drinks.drinkid=ordereditems.drinkid\n"
           + "where customers.lastname =";

   // used for configuring JTable to display and sort data
   private ResultSetTableModel tableModel;
   private TableRowSorter<TableModel> sorter;

   public void initialize() {
      queryTextArea.setText(DEFAULT_QUERY);

      // create ResultSetTableModel and display database table
      try {
         // create TableModel for results of DEFAULT_QUERY
         tableModel = new ResultSetTableModel(DATABASE_URL,
                 USERNAME, PASSWORD, DEFAULT_QUERY);

         // create JTable based on the tableModel    
         JTable resultTable = new JTable(tableModel);

         // set up row sorting for JTable
         sorter = new TableRowSorter<>(tableModel);
         resultTable.setRowSorter(sorter);

         // configure SwingNode to display JTable, then add to borderPane
         SwingNode swingNode = new SwingNode();
         swingNode.setContent(new JScrollPane(resultTable));
         borderPane.setCenter(swingNode);

      } catch (SQLException sqlException) {
         displayAlert(AlertType.ERROR, "Database Error",
                 sqlException.getMessage());
         tableModel.disconnectFromDatabase(); // close connection  
         System.exit(1); // terminate application
      }
   }

   // query the database and display results in JTable
   @FXML
   private void submitQueryButtonPressed(ActionEvent event) {
      // perform a new query
      try {
         tableModel.setQuery(queryTextArea.getText());
      } catch (SQLException sqlException) {
         displayAlert(AlertType.ERROR, "Database Error",
                 sqlException.getMessage());

         // try to recover from invalid user query 
         // by executing default query
         try {
            tableModel.setQuery(DEFAULT_QUERY);
            queryTextArea.setText(DEFAULT_QUERY);
         } catch (SQLException sqlException2) {
            displayAlert(AlertType.ERROR, "Database Error",
                    sqlException2.getMessage());
            tableModel.disconnectFromDatabase(); // close connection  
            System.exit(1); // terminate application
         }
      }
   }

   // apply specified filter to results
   @FXML
   private void applyFilterButtonPressed(ActionEvent event) {
      String text = filterTextField.getText();

      if (text.length() == 0) {
         sorter.setRowFilter(null);
      } else {
         try {
            sorter.setRowFilter(RowFilter.regexFilter(text));
         } catch (PatternSyntaxException pse) {
            displayAlert(AlertType.ERROR, "Regex Error",
                    "Bad regex pattern");
         }
      }
   }

   //Q1 query “number of orders and income per day” 
   @FXML
   private void simpleQueriesSearchButtonPressed(ActionEvent event) {
      // perform a new query
      try {
         
         // input simple order queries string in the text field and update it into the modal
         queryTextArea.setText(NUM_ORDERS_PRICE_PER_DAY_QUERY);
         tableModel.setQuery(queryTextArea.getText());
      } catch (SQLException sqlException) {
         displayAlert(AlertType.ERROR, "Database Error",
                 sqlException.getMessage());

         // try to recover from invalid user query 
         // by executing  query NUM_ORDERS_PRICE_PER_DAY_QUERY
         try {
            tableModel.setQuery(NUM_ORDERS_PRICE_PER_DAY_QUERY);
            queryTextArea.setText(NUM_ORDERS_PRICE_PER_DAY_QUERY);
         } catch (SQLException sqlException2) {
            displayAlert(AlertType.ERROR, "Database Error",
                    sqlException2.getMessage());
            tableModel.disconnectFromDatabase(); // close connection  
            System.exit(1); // terminate application
         }
      }
   }

   // Q3:  query the database by phonenumber (entered in a textfield)
   @FXML
   private void queryByPhoneSearchButtonPressed(ActionEvent event) {
      // perform a new query
      try {
         
         // input search by phone number queries string in the text field and update it into the table modal
         queryTextArea.setText(ORDER_SEARCH_BY_PHONENUMBER + "'" + phoneTextField.getText() + "'");
         tableModel.setQuery(queryTextArea.getText());
      } catch (SQLException sqlException) {
         displayAlert(AlertType.ERROR, "Database Error",
                 sqlException.getMessage());

         // try to recover from invalid user query 
         // by executing  query ORDER_SEARCH_BY_PHONENUMBER
         try {
            tableModel.setQuery(ORDER_SEARCH_BY_PHONENUMBER);
            queryTextArea.setText(ORDER_SEARCH_BY_PHONENUMBER);
         } catch (SQLException sqlException2) {
            displayAlert(AlertType.ERROR, "Database Error",
                    sqlException2.getMessage());
            tableModel.disconnectFromDatabase(); // close connection  
            System.exit(1); // terminate application
         }
      }
   }

   // Q4:  query the database by lastname (entered in a textfield)
   @FXML
   private void queryByLastNameSearchButtonPressed(ActionEvent event) {
      // perform a new query
      try {

         //// input search by lastName queries string in the text field and update it into the table modal
         queryTextArea.setText(ORDER_SEARCH_BY_LASTNAME + "'" + lastNameTextField.getText() + "'");
         tableModel.setQuery(queryTextArea.getText());
      } catch (SQLException sqlException) {
         displayAlert(AlertType.ERROR, "Database Error",
                 sqlException.getMessage());

         // try to recover from invalid user query 
         // by executing  query ORDER_SEARCH_BY_PHONENUMBER
         try {
            tableModel.setQuery(ORDER_SEARCH_BY_LASTNAME);
            queryTextArea.setText(ORDER_SEARCH_BY_LASTNAME);
         } catch (SQLException sqlException2) {
            displayAlert(AlertType.ERROR, "Database Error",
                    sqlException2.getMessage());
            tableModel.disconnectFromDatabase(); // close connection  
            System.exit(1); // terminate application
         }
      }
   }

   // Q2 range query. use the DatePicker class to implement the function of picking a date.  
   @FXML
   private void rangleQueriesButtonPressed(ActionEvent event) {
      //form the following search string for execution. Here fromdate and todate are the string representation of dates you retrieve from the corresponding DatePicker objects. 

      //get the local date from the sslected datepicker 
      LocalDate fromdate = fromDatePicker.getValue();
      LocalDate todate = toDatePicker.getValue();
      
      // simply combine the date into searchString in order to create rangle queries 
      String searchString = DEFAULT_QUERY + " where date(OrderTime) >= '" + fromdate + "' AND date(OrderTime) <= '" + todate + "'";
      // perform a new query
      try {
         
         // input the rangle query and update it into table model
         queryTextArea.setText(searchString);
         tableModel.setQuery(queryTextArea.getText());
      } catch (SQLException sqlException) {
         displayAlert(AlertType.ERROR, "Database Error",
                 sqlException.getMessage());

         // try to recover from invalid user query 
         // by executing  RANGLE Query
         try {
            tableModel.setQuery(searchString);
            queryTextArea.setText(searchString);
         } catch (SQLException sqlException2) {
            displayAlert(AlertType.ERROR, "Database Error",
                    sqlException2.getMessage());
            tableModel.disconnectFromDatabase(); // close connection  
            System.exit(1); // terminate application
         }
      }
   }

   // display an Alert dialog
   private void displayAlert(
           AlertType type, String title, String message) {
      Alert alert = new Alert(type);
      alert.setTitle(title);
      alert.setContentText(message);
      alert.showAndWait();
   }
}
