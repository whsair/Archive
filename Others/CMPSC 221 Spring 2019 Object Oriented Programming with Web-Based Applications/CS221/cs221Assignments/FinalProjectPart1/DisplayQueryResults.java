// Fig. X: DisplayQueryResults.java
// Main application class that loads and displays the GUI.
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;


// the Class extends application and create a scene in our computer in order to diplay our GUI
public class DisplayQueryResults extends Application {
   @Override
   public void start(Stage stage) throws Exception {
      Parent root = 
         FXMLLoader.load(getClass().getResource("Display Query Results.fxml")); //targeted GUI
      
      Scene scene = new Scene(root);
      stage.setTitle("Display Query Results"); // show Scene's title 
      stage.setScene(scene);
      stage.show();
   }
   //Simple launch class just like we previous did. start our GUI control class
   
   public static void main(String[] args) {
      launch(args);
   }
}


