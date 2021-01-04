import javafx.fxml.FXML;
import javafx.scene.control.ListView;
import javafx.scene.image.ImageView;
import javafx.scene.image.Image;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.scene.control.ListCell;
import javafx.util.Callback;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import javafx.scene.input.MouseEvent;
import java.io.File;
import javafx.scene.layout.BorderPane;
import javafx.stage.FileChooser;
import java.io.BufferedWriter;
import java.io.BufferedReader;      
import java.io.IOException;      
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;
import javafx.stage.FileChooser.ExtensionFilter;
import javax.xml.bind.JAXB;

public class ContactsAppController {
   
    @FXML
    private BorderPane borderPane;
    
    @FXML
    private Button saveButton;

    @FXML
    private Button deleteButton;

    @FXML
    private ImageView contactImageView;

    @FXML
    private TextField lastNameTextField;

    @FXML
    private TextField firstNameTextField;

    @FXML
    private TextField emailAddressTextField;

    @FXML
    private TextField phoneNumberTextField;

    @FXML
    private TextField homeAddressTextField;

    @FXML
    private ListView<Contact> contactListView;

    @FXML
    private Button addButton;
    
    @FXML
    private Button importButton;

    @FXML
    private Button exportButton;
    
    //field
    private String currentImage;
    
    
    private final ObservableList<Contact> contacts = FXCollections.observableArrayList(); //Obervable list to store the contacts in the program
    
    public void initialize() {
       contactListView.setItems(contacts.sorted()); //sort our contact 
       contactListView.getSelectionModel().selectedItemProperty().addListener( //add a listener and bind contactListView 
         new ChangeListener<Contact>() {
            @Override
            public void changed(ObservableValue<? extends Contact> ov, //when we change our selected item display the new contact item's information
                    Contact oldValue, Contact newValue) {
               if (newValue == null) {
                  return;
               }
               if (!contacts.isEmpty()) {
                  firstNameTextField.setText(newValue.getFirstName());
                  lastNameTextField.setText(newValue.getLastName());
                  homeAddressTextField.setText(newValue.getHomeAddress());
                  emailAddressTextField.setText(newValue.getEmail());
                  phoneNumberTextField.setText(newValue.getPhoneNumber());
                  currentImage = newValue.getPhoto();
                  
                  if (currentImage == null) {           //check whether the contact has image
                     contactImageView.setImage(null);
                  } else {
                     contactImageView.setImage(new Image(currentImage));
                  }
               }
            }
         }
       );
       
       contactListView.setCellFactory( // use the list to display the thumb picture in the listView
         new Callback<ListView<Contact>, ListCell<Contact>>() {
            @Override
            public ListCell<Contact> call(ListView<Contact> listView) {
               return new ImageTextCell();
            }
         }
       );
    }
    
    @FXML
    private void addButtonPressed(ActionEvent event) {  //When we press the add button, clear every information on the scene
       firstNameTextField.clear();
       lastNameTextField.clear();
       emailAddressTextField.clear();
       homeAddressTextField.clear();
       phoneNumberTextField.clear();
       contactImageView.setImage(null);
    }

    @FXML
    private void deleteButtonPressed(ActionEvent event) {
      if (!(contacts.isEmpty())) { //chech whether we have contact in our system
         Contact  contact= contactListView.getSelectionModel().getSelectedItem(); //if the contact exists, remove the selected element
         if (contact != null) { 
            contacts.remove(contact);
            addButtonPressed(event);
         }
      } else { //if not just clear the information
         addButtonPressed(event);
      } 
    }
   
    
    
    @FXML
    private void contactImageViewMouseCliked(MouseEvent e) { //click picture field in order to upload our picture
      FileChooser fileChooser = new FileChooser();               
      fileChooser.setTitle("Select File");            //create a file chooser in order to select picture in our computer 

      // display files in folder from which the app was launched
      fileChooser.setInitialDirectory(new File(".")); 

      // display the FileChooser
      File file = fileChooser.showOpenDialog(
         borderPane.getScene().getWindow());               

      // process selected Path or display a message
      if (file != null) {
         currentImage = file.toURI().toString();
         contactImageView.setImage(new Image(currentImage));           
      } 

    }

    
   
    @FXML
    private void saveButtonPressed(ActionEvent event) {     //save and update current contact into our appliction
       String firstName = firstNameTextField.getText();
       String lastName = lastNameTextField.getText();
       String email = emailAddressTextField.getText();
       String homeAddress = homeAddressTextField.getText();
       String phoneNumber = phoneNumberTextField.getText();
       String photo;
       boolean isNewPhoto = true;

       if (firstName.isEmpty()) {  //if first Name and last Name is missing, reject the update
          if (lastName.isEmpty()) {
            return;
          }
       }
       
       if (contactImageView.getImage() == null) { //update the null value in the listView when the user doesn't provide the image
          photo = null;
       } else {
          photo = currentImage;
       }
         
       for (Contact contact : contacts) {  //check current value whether or not existed in our application 
          if (contact.getFirstName().equals(firstName)) { //if exist, just find the corresponding object and rewrite it
             if (contact.getLastName().equals(lastName)) {
               contacts.add(new Contact(firstName, lastName, email, homeAddress, phoneNumber, photo));
               contacts.remove(contact);
               isNewPhoto = false;    
               break;
            }
         }
       }
       
       if (isNewPhoto == true) { //if not found, just add the new contact in our program
          contacts.add(new Contact(firstName, lastName, email, homeAddress, phoneNumber, photo));
       } 
    }

    @FXML
    private void exportButtonPressed(ActionEvent event) {
       if (contacts.isEmpty()) { //check the contacts whethe is empty
          return;
       }
       
       
       FileChooser fileChooser = new FileChooser();               
       fileChooser.setTitle("Save the contacts");

       // display files in folder from which the app was launched
       fileChooser.setInitialDirectory(new File(".")); 
       
       //create the contact.xml to store our contacts
       fileChooser.getExtensionFilters().add(new ExtensionFilter("XML Files", "*.xml"));
       // display the FileChooser
       File file = fileChooser.showSaveDialog(
          borderPane.getScene().getWindow());
       if (file == null) {
          return;
       }
       
       
       // open clients.xml, write objects to it then close file
       try(BufferedWriter output = 
               Files.newBufferedWriter(Paths.get(file.getPath()))) {
        
         // stores the contacts before XML serialization
            if (output != null) {
               Contacts exportedContacts = new Contacts(); 

               contacts.forEach((contact) -> { //store the every elements of contact in our program into a not observalue array 
                  exportedContacts.getContacts().add(contact);
               });
               
               JAXB.marshal(exportedContacts, output);  //serializate our object and store them into our selected file
            }

            

         // write accountsList's XML to output
            
            
          
       }
       catch (IOException ioException) {
          System.err.println("Error opening file. Terminating.");
       } 
   } 
    

    @FXML
    private void importButtonPressed(ActionEvent event) {
       
       FileChooser fileChooser = new FileChooser();               
       fileChooser.setTitle("Load the contacts");

       // display files in folder from which the app was launched
       fileChooser.setInitialDirectory(new File(".")); 
       
       
       
       // display the FileChooser
       File file = fileChooser.showOpenDialog(
          borderPane.getScene().getWindow());
       
       if (file != null) {
          try(BufferedReader input = 
            Files.newBufferedReader(Paths.get(file.getPath()))) {
            // unmarshal the file's contents
            Contacts importedContacts = JAXB.unmarshal(input, Contacts.class);
            
            contacts.clear();
            
            // create a list to store the contact element
            List<Contact> importedContactsList = importedContacts.getContacts();
            
            
            importedContactsList.forEach((contact) -> { //input every element from read xml code into our program
               contacts.add(contact);
             });
          } 
          catch (IOException ioException) {
             System.err.println("Error opening file.");
          } 
       }

    }

}
