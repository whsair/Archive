

import javafx.geometry.Pos;
import javafx.scene.control.Label;
import javafx.scene.control.ListCell;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.VBox;
import javafx.scene.text.TextAlignment;


public class ImageTextCell extends ListCell<Contact> {
   private VBox vbox = new VBox(0.0); //no gap between each control
   private ImageView photoImageView = new ImageView(); //initially empty
   private Label label = new Label();
   
   //constructor configures VBOX, imageView and Label
   public ImageTextCell() {
      vbox.setAlignment(Pos.CENTER); //center VBOX centents horizontally
      
      photoImageView.setPreserveRatio(true);
      photoImageView.setFitHeight(100.0); //100 points tall photo
      vbox.getChildren().add(photoImageView); //attach to VBOX
      
      label.setWrapText(true); //wrap if text too wide to file in label
      label.setTextAlignment(TextAlignment.CENTER); //cneter text
      vbox.getChildren().add(label); //attach to VBOX
      
      setPrefWidth(USE_PREF_SIZE); //use preferred size for cell width
   }
   
   //called to configure each custom listView cell
   @Override
   protected void updateItem(Contact item, boolean empty) {
      //required to ensure that cell displays properly
      super.updateItem(item, empty);
      
      if (empty || item == null) {
         setGraphic(null); //dont's display anything
         
      }
      else {
         if (item.getPhoto() == null) {
            
            photoImageView.setImage(null); //don't display anything either when contact element doesn't have the photo
            
         } else {
            //set ImageView's photo image
            photoImageView.setImage(new Image(item.getPhoto()));
         }
         
         label.setText(item.toString()); //configure Label's text (first Name + last name)
         setGraphic(vbox); //attach custom layout to ListView cell
         
      }
   }
}
