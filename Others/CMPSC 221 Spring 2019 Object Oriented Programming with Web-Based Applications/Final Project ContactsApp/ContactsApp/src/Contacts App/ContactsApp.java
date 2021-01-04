import javafx.application.Application;
import static javafx.application.Application.launch;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

public class ContactsApp extends Application {
   
   @Override
   public void start(Stage stage) throws Exception {
      Parent root = 
              FXMLLoader.load(getClass().getResource("ContactsApp.fxml")); //load the fxml file
      
      Scene scene = new Scene(root); 
      stage.setTitle("Contacts App"); // displayed in title bar
      stage.setScene(scene);
      stage.show(); //pop up the window
   }
   
   public static void main(String[] args) {
      launch(args); //launch the fxml file
   }
   
}
